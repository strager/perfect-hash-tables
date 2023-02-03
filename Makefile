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

gperf_sos = $(foreach flags,$(gperf_combinations),build/libtoken-gperf-$(flags)-generated.so)
gperf_cpps = $(foreach flags,$(gperf_combinations),generated/token-gperf-$(flags)-generated.cpp)

sos = $(gperf_sos) build/libtoken-std-unordered-map.so build/libperfect-hash-table-generated.so

.PHONY: all
all: check build/benchmark keywords.txt non-keywords.txt mixed.txt

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

build/test: $(sos) test.cpp token.h implementations.h generated/implementations-generated.inc Makefile build/stamp
	$(CXX) $(extra_CXXFLAGS) $(CXXFLAGS) $(LDFLAGS) -o $(@) test.cpp

build/benchmark: $(sos) benchmark.cpp token.h implementations.h generated/implementations-generated.inc Makefile build/stamp
	$(CXX) -fno-lto $(extra_CXXFLAGS) $(CXXFLAGS) $(LDFLAGS) -o $(@) benchmark.cpp -lbenchmark -lbenchmark_main

build/map-to-tokens: build/libtoken-gperf-_-generated.so map-to-tokens.cpp file.h lex.h token.h Makefile build/stamp
	$(CXX) $(extra_CXXFLAGS) $(CXXFLAGS) $(LDFLAGS) -o $(@) map-to-tokens.cpp build/libtoken-gperf-_-generated.so

generated/implementations-generated.inc: Makefile generated/stamp
	printf '"./%s",\n' $(sos) >$(@)

build/libtoken-std-unordered-map.so: token-std-unordered-map.cpp token.h Makefile build/stamp
	$(CXX) $(extra_CXXFLAGS) $(CXXFLAGS) $(extra_test_LDFLAGS) $(LDFLAGS) -shared -o $(@) $(<)

build/generate-perfect-hash-table: generate-perfect-hash-table.cpp token.h perfect-hash-table.h fnv.h Makefile build/stamp
	$(CXX) $(extra_CXXFLAGS) $(CXXFLAGS) $(LDFLAGS) -o $(@) generate-perfect-hash-table.cpp

build/libperfect-hash-table-generated.so: generated/perfect-hash-table-generated.cpp token.h perfect-hash-table.h fnv.h Makefile build/stamp
	$(CXX) $(extra_CXXFLAGS) $(CXXFLAGS) $(extra_test_LDFLAGS) $(LDFLAGS) -shared -o $(@) $(<)
generated/perfect-hash-table-generated.cpp: build/generate-perfect-hash-table Makefile generated/stamp
	./build/generate-perfect-hash-table

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

keywords.txt non-keywords.txt mixed.txt: build/map-to-tokens jquery-3.5.1.js
	./build/map-to-tokens

build/stamp:
	mkdir -p build
	touch $(@)

generated/stamp:
	mkdir -p generated
	touch $(@)
