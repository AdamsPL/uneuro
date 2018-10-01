CXXFLAGS=-std=c++17 -march=native -O3 -pipe -Wall -pedantic -Werror -I./include/

.SUFFIXES:

HDRS=$(wildcard include/*.hpp include/layers/*.hpp include/costs/*.hpp)
SRCS=$(wildcard demos/*.cpp)
OBJS=$(SRCS:.cpp=.o)
DEMOS=$(OBJS:.o=)

all: $(DEMOS)

%.o: %.cpp $(HDRS) Makefile
	$(CXX) $(CXXFLAGS) -c $< -o $@

% : %.o
	$(CXX) $^ -o $@

stats:
	wc -l $(SRCS) $(HDRS)

print-%:
	@echo $($*)

clean:
	rm -f $(DEMOS) $(OBJS)

run: $(DEMOS)
	./$<

dis: $(DEMOS)
	objdump -d -C $< | less
