SHELL = bash

extra_test_LDFLAGS = -Wl,--undefined=look_up_identifier
extra_CXXFLAGS = -std=c++20 -g -O3 -fvisibility=hidden -fPIC

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

gperf_sos = $(foreach flags,$(gperf_combinations),build/gperf$(flags).so)
gperf_cpps = $(foreach flags,$(gperf_combinations),generated/gperf$(flags).cpp)

# Keep in sync with generate-pht.cpp.
pht_combinations = \
	small-15 \
	small-23 \
	small-27 \
	small-29 \
	small-31 \
	pot-15 \
	pot-23 \
	pot-27 \
	pot-29 \
	pot-31

pht_sos = $(foreach flags,$(pht_combinations),build/pht-$(flags).so)
pht_cpps = $(foreach flags,$(pht_combinations),generated/pht-$(flags).cpp)

custom_combinations = \
	linear-packed-sized \
	linear-packed-z \
	std-unordered-map

custom_sos = $(foreach name,$(custom_combinations),build/$(name).so)
custom_cpps = $(foreach name,$(custom_combinations),$(name).cpp)

sos = $(gperf_sos) $(pht_sos) $(custom_sos)

.PHONY: all
all: check build/benchmark build/benchmark-lite keywords.txt non-keywords.txt mixed.txt

.PHONY: bench
bench: build/benchmark keywords.txt non-keywords.txt mixed.txt
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

build/test: $(sos) test.cpp token.h implementations.h generated/implementations.inc Makefile build/stamp
	$(CXX) $(extra_CXXFLAGS) $(CXXFLAGS) $(LDFLAGS) -o $(@) test.cpp

build/benchmark: $(sos) benchmark.cpp token.h implementations.h generated/implementations.inc Makefile build/stamp
	$(CXX) -fno-lto $(extra_CXXFLAGS) $(CXXFLAGS) $(LDFLAGS) -o $(@) benchmark.cpp -lbenchmark

build/benchmark-lite: benchmark-lite.cpp token.h implementations.h generated/implementations.inc Makefile build/stamp
	$(CXX) $(extra_CXXFLAGS) $(CXXFLAGS) $(LDFLAGS) -o $(@) benchmark-lite.cpp -lbenchmark

build/map-to-tokens: build/gperf_.so map-to-tokens.cpp file.h lex.h token.h Makefile build/stamp
	$(CXX) $(extra_CXXFLAGS) $(CXXFLAGS) $(LDFLAGS) -o $(@) map-to-tokens.cpp build/gperf_.so

generated/implementations.inc: Makefile generated/stamp
	printf '"./%s",\n' $(sos) >$(@)

define make_custom_so
build/$(name).so: $(name).cpp token.h Makefile build/stamp
	$$(CXX) $$(extra_CXXFLAGS) $$(CXXFLAGS) $$(extra_test_LDFLAGS) $$(LDFLAGS) -shared -o $$(@) $$(<)
endef
$(foreach name,$(custom_combinations),$(eval $(call make_custom_so)))

build/generate-pht: generate-pht.cpp token.h pht.h fnv.h Makefile build/stamp
	$(CXX) $(extra_CXXFLAGS) $(CXXFLAGS) $(LDFLAGS) -o $(@) generate-pht.cpp

define make_pht_so
build/pht-$(flags).so: generated/pht-$(flags).cpp token.h pht.h fnv.h Makefile build/stamp
	$$(CXX) $$(extra_CXXFLAGS) $$(CXXFLAGS) $$(extra_test_LDFLAGS) $$(LDFLAGS) -shared -o $$(@) $$(<)
endef
$(foreach flags,$(pht_combinations),$(eval $(call make_pht_so)))

# We use one command to generate multiple .cpp files. Use a stamp file and
# order-only dependencies to trick Make into only running the command once.
build/pht-cpps-stamp: build/generate-pht Makefile generated/stamp
	./build/generate-pht
	touch $(@)
$(pht_cpps): | build/pht-cpps-stamp

define make_gperf_so
build/gperf$(flags).so: generated/gperf$(flags).cpp token.h Makefile build/stamp
	$$(CXX) -I. $$(extra_CXXFLAGS) $$(CXXFLAGS) $$(extra_test_LDFLAGS) $$(LDFLAGS) $$(extra_gperf_CXXFLAGS) -shared -o $$(@) $$(<)

ifeq ($(findstring --pic,$(flags)), --pic)
build/gperf$(flags).so: extra_gperf_CXXFLAGS = -DGPERF_PIC
endif

generated/gperf$(flags).cpp: token.gperf Makefile generated/stamp
	set -e -o pipefail ; gperf $(subst _, ,$(flags)) $$(<) | sed -e '/^#line/d' >$$(@)
endef
$(foreach flags,$(gperf_combinations),$(eval $(call make_gperf_so)))

keywords.txt non-keywords.txt mixed.txt: build/map-to-tokens jquery-3.5.1.js
	./build/map-to-tokens

build/stamp:
	mkdir -p build
	touch $(@)

generated/stamp:
	mkdir -p generated
	touch $(@)
