SHELL = bash

CXX = g++-12
clang_CXX = ~/Toolchains/clang-stage2/bin/clang++

extra_test_LDFLAGS = -Wl,--undefined=look_up_identifier
extra_CXXFLAGS = -std=c++20 -g -O3 -fvisibility=hidden -fPIC \
	-I ~/tmp/Projects/xxhash/ -DXXH_INLINE_ALL \
	-I ~/tmp/Projects/frozen/include/ \
	-msse4 -maes -fcf-protection=full

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
	--inline-strings \
	--inline-strings_--compare-lengths

gperf_sos = $(foreach flags,$(gperf_combinations),build/gperf$(flags).so build/gperf$(flags)-clang.so)
gperf_cpps = $(foreach flags,$(gperf_combinations),generated/gperf$(flags).cpp)

pht_combinations = \
	--table-size=pot_--characters=15_--hasher=xx364 \
	--table-size=pot_--characters=23_--hasher=xx364 \
	--table-size=pot_--characters=27_--hasher=xx364 \
	--table-size=pot_--characters=29_--hasher=xx364 \
	--table-size=pot_--characters=15_--hasher=fnv1a32 \
	--table-size=pot_--characters=23_--hasher=fnv1a32 \
	--table-size=pot_--characters=27_--hasher=fnv1a32 \
	--table-size=pot_--characters=29_--hasher=fnv1a32 \
	--table-size=pot_--characters=15_--hasher=lehmer128 \
	--table-size=pot_--characters=23_--hasher=lehmer128 \
	--table-size=pot_--characters=27_--hasher=lehmer128 \
	--table-size=pot_--characters=29_--hasher=lehmer128 \
	--table-size=pot_--characters=15_--hasher=lehmer128_--inline-hash \
	--table-size=pot_--characters=23_--hasher=lehmer128_--inline-hash \
	--table-size=pot_--characters=27_--hasher=lehmer128_--inline-hash \
	--table-size=pot_--characters=29_--hasher=lehmer128_--inline-hash \
	--table-size=pot_--characters=15_--hasher=lehmer128_--string-compare=sse2_--cmov \
	--table-size=pot_--characters=15_--hasher=lehmer128_--string-compare=ptest_--cmov \
	--table-size=pot_--characters=23_--hasher=lehmer128_--string-compare=ptest_--cmov \
	--table-size=pot_--characters=27_--hasher=lehmer128_--string-compare=ptest_--cmov \
	--table-size=pot_--characters=29_--hasher=lehmer128_--string-compare=ptest_--cmov \
	--table-size=small_--characters=15_--hasher=icrc32 \
	--table-size=small_--characters=23_--hasher=icrc32 \
	--table-size=small_--characters=27_--hasher=icrc32 \
	--table-size=small_--characters=29_--hasher=icrc32 \
	--table-size=small_--characters=15_--hasher=icrc32_--inline-hash \
	--table-size=small_--characters=23_--hasher=icrc32_--inline-hash \
	--table-size=small_--characters=27_--hasher=icrc32_--inline-hash \
	--table-size=small_--characters=29_--hasher=icrc32_--inline-hash \
	--table-size=small_--characters=15_--hasher=icrc32_--string-compare=check1memcmp \
	--table-size=small_--characters=23_--hasher=icrc32_--string-compare=check1memcmp \
	--table-size=small_--characters=27_--hasher=icrc32_--string-compare=check1memcmp \
	--table-size=small_--characters=29_--hasher=icrc32_--string-compare=check1memcmp \
	--table-size=small_--characters=15_--hasher=icrc32_--string-compare=cmpestri \
	--table-size=small_--characters=23_--hasher=icrc32_--string-compare=cmpestri \
	--table-size=small_--characters=27_--hasher=icrc32_--string-compare=cmpestri \
	--table-size=small_--characters=29_--hasher=icrc32_--string-compare=cmpestri \
	--table-size=small_--characters=15_--hasher=icrc32_--string-compare=cmpestri_--cmov \
	--table-size=small_--characters=23_--hasher=icrc32_--string-compare=cmpestri_--cmov \
	--table-size=small_--characters=27_--hasher=icrc32_--string-compare=cmpestri_--cmov \
	--table-size=small_--characters=29_--hasher=icrc32_--string-compare=cmpestri_--cmov \
	--table-size=small_--characters=15_--hasher=icrc32_--string-compare=sse2 \
	--table-size=small_--characters=15_--hasher=icrc32_--string-compare=sse2_--cmov \
	--table-size=small_--characters=15_--hasher=icrc32_--string-compare=ptest \
	--table-size=small_--characters=15_--hasher=icrc32_--string-compare=ptest_--cmov \
	--table-size=small_--characters=15_--hasher=aes \
	--table-size=small_--characters=23_--hasher=aes \
	--table-size=small_--characters=27_--hasher=aes \
	--table-size=small_--characters=29_--hasher=aes \
	--table-size=pot_--characters=15_--hasher=aes \
	--table-size=pot_--characters=23_--hasher=aes \
	--table-size=pot_--characters=27_--hasher=aes \
	--table-size=pot_--characters=29_--hasher=aes \
	--table-size=pot_--characters=15_--hasher=aes_--entry-size=16 \
	--table-size=pot_--characters=15_--hasher=aes_--shiftless-index_--entry-size=16 \
	--characters=15_--hasher=pearson8

pht_sos = $(foreach flags,$(pht_combinations),build/pht$(subst =,-,$(flags)).so build/pht$(subst =,-,$(flags))-clang.so)
pht_cpps = $(foreach flags,$(pht_combinations),generated/pht$(subst =,-,$(flags)).cpp)

mygperf_combinations = \
	--string-compare=memcmp \
	--string-compare=check1memcmp \
	--string-compare=sse2 \
	--string-compare=sse2_--cmov 

mygperf_sos = $(foreach flags,$(mygperf_combinations),build/mygperf$(subst =,-,$(flags)).so build/mygperf$(subst =,-,$(flags))-clang.so)
mygperf_cpps = $(foreach flags,$(mygperf_combinations),generated/mygperf$(subst =,-,$(flags)).cpp)

custom_combinations = \
	frozen-unordered-map \
	frozen-unordered-map-lehmer128-firsttwolasttwo \
	linear-packed-sized \
	linear-packed-z \
	std-unordered-map \
	std-unordered-map-lehmer128-firsttwolasttwo

custom_sos = $(foreach name,$(custom_combinations),build/$(name).so build/$(name)-clang.so)
custom_cpps = $(foreach name,$(custom_combinations),$(name).cpp)

sos = $(gperf_sos) $(pht_sos) $(mygperf_sos) $(custom_sos)

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
build/$(name)-clang.so: $(name).cpp token.h Makefile build/stamp
	$$(clang_CXX) $$(extra_CXXFLAGS) $$(CXXFLAGS) $$(extra_test_LDFLAGS) $$(LDFLAGS) -shared -o $$(@) $$(<)
endef
$(foreach name,$(custom_combinations),$(eval $(call make_custom_so)))

build/generate-pht: generate-pht.cpp token.h pht.h fnv.h Makefile build/stamp
	$(CXX) $(extra_CXXFLAGS) $(CXXFLAGS) $(LDFLAGS) -o $(@) generate-pht.cpp

define make_pht_so
build/pht$(subst =,-,$(flags)).so: generated/pht$(subst =,-,$(flags)).cpp token.h pht.h fnv.h Makefile build/stamp
	$$(CXX) $$(extra_CXXFLAGS) $$(CXXFLAGS) $$(extra_test_LDFLAGS) $$(LDFLAGS) -shared -o $$(@) $$(<)
build/pht$(subst =,-,$(flags))-clang.so: generated/pht$(subst =,-,$(flags)).cpp token.h pht.h fnv.h Makefile build/stamp
	$$(clang_CXX) $$(extra_CXXFLAGS) $$(CXXFLAGS) $$(extra_test_LDFLAGS) $$(LDFLAGS) -shared -o $$(@) $$(<)
generated/pht$(subst =,-,$(flags)).cpp: build/generate-pht Makefile generated/stamp
	zsh -c 'time ./build/generate-pht $(subst _, ,$(flags)) --output $$(@)'
endef
$(foreach flags,$(pht_combinations),$(eval $(call make_pht_so)))

define make_gperf_so
build/gperf$(flags).so: generated/gperf$(flags).cpp token.h Makefile build/stamp
	$$(CXX) -I. $$(extra_CXXFLAGS) $$(CXXFLAGS) $$(extra_test_LDFLAGS) $$(LDFLAGS) $$(extra_gperf_CXXFLAGS) -shared -o $$(@) $$(<)
build/gperf$(flags)-clang.so: generated/gperf$(flags).cpp token.h Makefile build/stamp
	$$(clang_CXX) -I. $$(extra_CXXFLAGS) $$(CXXFLAGS) $$(extra_test_LDFLAGS) $$(LDFLAGS) $$(extra_gperf_CXXFLAGS) -shared -o $$(@) $$(<)

ifeq ($(findstring --pic,$(flags)), --pic)
build/gperf$(flags).so: extra_gperf_CXXFLAGS = -DGPERF_PIC
build/gperf$(flags)-clang.so: extra_gperf_CXXFLAGS = -DGPERF_PIC
endif
ifeq ($(findstring --inline-strings,$(flags)), --inline-strings)
build/gperf$(flags).so: extra_gperf_CXXFLAGS = -DGPERF_INLINE
build/gperf$(flags)-clang.so: extra_gperf_CXXFLAGS = -DGPERF_INLINE
endif

generated/gperf$(flags).cpp: token.gperf Makefile generated/stamp
	set -e -o pipefail ; gperf $(subst --inline-strings,,$(subst _, ,$(flags))) $$(<) | sed -e '/^#line/d' >$$(@)
endef
$(foreach flags,$(gperf_combinations),$(eval $(call make_gperf_so)))

define make_mygperf_so
build/mygperf$(subst =,-,$(flags)).so: generated/mygperf$(subst =,-,$(flags)).cpp token.h Makefile build/stamp
	$$(CXX) -I. $$(extra_CXXFLAGS) $$(CXXFLAGS) $$(extra_test_LDFLAGS) $$(LDFLAGS) -shared -o $$(@) $$(<)
build/mygperf$(subst =,-,$(flags))-clang.so: generated/mygperf$(subst =,-,$(flags)).cpp token.h Makefile build/stamp
	$$(clang_CXX) -I. $$(extra_CXXFLAGS) $$(CXXFLAGS) $$(extra_test_LDFLAGS) $$(LDFLAGS) -shared -o $$(@) $$(<)
generated/mygperf$(subst =,-,$(flags)).cpp: generate-mygperf.py Makefile generated/stamp
	python ./generate-mygperf.py $(subst _, ,$(flags)) --output $$(@)
endef
$(foreach flags,$(mygperf_combinations),$(eval $(call make_mygperf_so)))

keywords.txt non-keywords.txt mixed.txt: build/map-to-tokens jquery-3.5.1.js
	./build/map-to-tokens

build/stamp:
	mkdir -p build
	touch $(@)

generated/stamp:
	mkdir -p generated
	touch $(@)
