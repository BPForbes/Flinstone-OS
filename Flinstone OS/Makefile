CXX = g++
CXXFLAGS = -Wall -O2 -std=c++17

TOOLS = ping netcat wget

all: $(TOOLS)

ping: cmd/ping.cpp
	$(CXX) $(CXXFLAGS) -o $@ cmd/ping.cpp

netcat: cmd/netcat.cpp
	$(CXX) $(CXXFLAGS) -o $@ cmd/netcat.cpp

wget: cmd/wget.cpp
	$(CXX) $(CXXFLAGS) -o $@ cmd/wget.cpp

clean:
	rm -f $(TOOLS)