BIN=test
SRCS=logger.cpp test.cpp

CFLAGS += -Wall -Wextra -s -O2 -std=c++11

OBJS=$(SRCS:%.cpp=%.o)

.PHONY: all clean

all: $(BIN)

$(BIN): $(OBJS)
	$(CXX) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CFLAGS) -c -o $@ $^

clean:
	rm -f $(OBJS) $(BIN) test.log
