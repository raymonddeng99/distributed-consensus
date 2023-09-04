CXX = g++
CXXFLAGS = -Wall -std=c++11

# List of source files
SRCS = lcr.cpp coordinator.cpp node.cpp server.cpp

# List of object files to generate
OBJS = $(SRCS:.cpp=.o)

# Name of the executable
EXEC = lcr

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS) $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(EXEC)