extra_test_LDFLAGS = -Wl,--undefined=look_up_identifier
extra_CXXFLAGS = -std=c++17 -g -O3 -fvisibility=hidden

.PHONY: all
all: check

.PHONY: check
check: test
	./test

.PHONY: clean
clean:
	rm -f libtoken-gperf-generated.so test

.PHONY: clean-all
clean-all: clean
	rm -f token-gperf-generated.cpp

test: libtoken-gperf-generated.so test.cpp token.h Makefile
	$(CXX) $(extra_CXXFLAGS) $(CXXFLAGS) $(LDFLAGS) -o $(@) test.cpp

libtoken-gperf-generated.so: token-gperf-generated.cpp token.h Makefile
	$(CXX) $(extra_CXXFLAGS) $(CXXFLAGS) $(extra_test_LDFLAGS) $(LDFLAGS) -shared -o $(@) token-gperf-generated.cpp

token-gperf-generated.cpp: token.gperf Makefile
	gperf $(<) >$(@)
