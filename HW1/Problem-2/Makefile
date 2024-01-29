CXX = g++
CXXFLAGS = -std=c++11 -Wall
LDFLAGS = $(shell pkg-config --libs opencv4)
INCLUDES = $(shell pkg-config --cflags opencv4)

SRCS = hw1_p2_cs190i.cpp
OBJS = $(SRCS:.cpp=.o)
EXEC = a.out

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c -o $@ $<

clean:
	rm -f $(OBJS) $(EXEC)
