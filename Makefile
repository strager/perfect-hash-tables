extra_LDFLAGS = -Wl,--undefined=look_up_identifier
extra_CXXFLAGS = -std=c++17 -g -O3 -fvisibility=hidden

.PHONY: all
all: libtoken-gperf-generated.so

.PHONY: clean
clean:
	rm -f libtoken-gperf-generated.so

.PHONY: clean-all
clean-all: clean
	rm -f token-gperf-generated.cpp

libtoken-gperf-generated.so: token-gperf-generated.cpp token.h Makefile
	$(CXX) $(extra_CXXFLAGS) $(CXXFLAGS) $(extra_LDFLAGS) $(LDFLAGS) -shared -o $(@) token-gperf-generated.cpp

token-gperf-generated.cpp: token.gperf Makefile
	gperf $(<) >$(@)
