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

gperf_sos = $(foreach flags,$(gperf_combinations),libtoken-gperf-$(flags)-generated.so)
gperf_cpps = $(foreach flags,$(gperf_combinations),token-gperf-$(flags)-generated.cpp)

.PHONY: all
all: check

.PHONY: check
check: test
	./test

.PHONY: clean
clean:
	rm -f $(gperf_sos) test

.PHONY: clean-all
clean-all: clean
	rm -f $(gperf_cpps) implementations-generated.inc

test: $(gperf_sos) test.cpp token.h implementations.h implementations-generated.inc Makefile
	$(CXX) $(extra_CXXFLAGS) $(CXXFLAGS) $(LDFLAGS) -o $(@) test.cpp

implementations-generated.inc: Makefile
	printf '"./%s",\n' $(gperf_sos) >$(@)

define make_gperf_so
libtoken-gperf-$(flags)-generated.so: token-gperf-$(flags)-generated.cpp token.h Makefile
	$$(CXX) $$(extra_CXXFLAGS) $$(CXXFLAGS) $$(extra_test_LDFLAGS) $$(LDFLAGS) $$(extra_gperf_CXXFLAGS) -shared -o $$(@) $$(<)

ifeq ($(findstring --pic,$(flags)), --pic)
libtoken-gperf-$(flags)-generated.so: extra_gperf_CXXFLAGS = -DGPERF_PIC
endif

token-gperf-$(flags)-generated.cpp: token.gperf Makefile
	set -e -o pipefail ; gperf $(subst _, ,$(flags)) $$(<) | sed -e '/^#line/d' >$$(@)
endef
$(foreach flags,$(gperf_combinations),$(eval $(call make_gperf_so)))
