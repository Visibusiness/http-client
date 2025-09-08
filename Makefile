CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 

DEPS = client.hpp helper.hpp buffer.hpp
SRC = client.cpp helper.cpp buffer.cpp

TARGET = client

$(TARGET): $(SRC) $(DEPS)
	$(CXX) $(CXXFLAGS) -o $@ $(SRC) $(LDFLAGS)

.PHONY: clean

clean:
	rm -f $(TARGET)
