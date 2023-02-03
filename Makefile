SHELL = bash

extra_test_LDFLAGS = -Wl,--undefined=look_up_identifier
extra_CXXFLAGS = -std=c++17 -g -O3 -fvisibility=hidden

gperf_combinations = \
	_ \
	--pic \
	--compare-lengths \
	--readonly-tables \
	--seven-bit \
	--pic_--compare-lengths \
	--pic_--readonly-tables \
	--pic_--seven-bit \
	--compare-lengths_--readonly-tables \
	--compare-lengths_--seven-bit \

gperf_sos = $(foreach flags,$(gperf_combinations),build/libtoken-gperf-$(flags)-generated.so)
gperf_cpps = $(foreach flags,$(gperf_combinations),generated/token-gperf-$(flags)-generated.cpp)

.PHONY: all
all: check build/benchmark

.PHONY: bench
bench: build/benchmark
	./build/benchmark

.PHONY: check
check: build/test
	./build/test

.PHONY: clean
clean:
	rm -rf build/

.PHONY: clean-all
clean-all: clean
	rm -rf generated/

build/test: $(gperf_sos) test.cpp token.h implementations.h generated/implementations-generated.inc Makefile build/stamp
	$(CXX) $(extra_CXXFLAGS) $(CXXFLAGS) $(LDFLAGS) -o $(@) test.cpp

build/benchmark: $(gperf_sos) benchmark.cpp token.h implementations.h generated/implementations-generated.inc Makefile build/stamp
	$(CXX) -fno-lto $(extra_CXXFLAGS) $(CXXFLAGS) $(LDFLAGS) -o $(@) benchmark.cpp -lbenchmark -lbenchmark_main

generated/implementations-generated.inc: Makefile generated/stamp
	printf '"./%s",\n' $(gperf_sos) >$(@)

define make_gperf_so
build/libtoken-gperf-$(flags)-generated.so: generated/token-gperf-$(flags)-generated.cpp token.h Makefile build/stamp
	$$(CXX) -I. $$(extra_CXXFLAGS) $$(CXXFLAGS) $$(extra_test_LDFLAGS) $$(LDFLAGS) $$(extra_gperf_CXXFLAGS) -shared -o $$(@) $$(<)

ifeq ($(findstring --pic,$(flags)), --pic)
build/libtoken-gperf-$(flags)-generated.so: extra_gperf_CXXFLAGS = -DGPERF_PIC
endif

generated/token-gperf-$(flags)-generated.cpp: token.gperf Makefile generated/stamp
	set -e -o pipefail ; gperf $(subst _, ,$(flags)) $$(<) | sed -e '/^#line/d' >$$(@)
endef
$(foreach flags,$(gperf_combinations),$(eval $(call make_gperf_so)))

build/stamp:
	mkdir -p build
	touch $(@)

generated/stamp:
	mkdir -p generated
	touch $(@)
