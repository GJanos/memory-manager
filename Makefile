PRODUCT=MemManager
TEST_PRODUCT=MemManagerTest
CPPFLAGS=-Iproduct -Itest
CXXFLAGS=-Wall -Werror -Wextra -pedantic -Wno-unused-parameter -ggdb -O0 $(CPPFLAGS)

SOURCES=$(wildcard product/*.cpp) main.cpp

TEST_SOURCES=$(wildcard product/*.cpp) \
         $(wildcard test/*.cpp) 

HEADERS=$(wildcard product/*.h) \
        $(wildcard test/*.h) \
        $(wildcard *.h)

LDFLAGS=-lgtest -lgmock_main -lpthread -lgmock

CXX=g++

.PHONY: all clean test

all: $(PRODUCT)

$(PRODUCT): $(SOURCES) $(HEADERS)
	@$(CXX) $(CXXFLAGS) $(SOURCES) -o $@


$(TEST_PRODUCT): $(TEST_SOURCES) $(HEADERS)
	$(CXX) $(CXXFLAGS) $(TEST_SOURCES) -o $@ $(LDFLAGS)

clean:
	@rm -rf $(PRODUCT) $(TEST_PRODUCT) 

test: $(TEST_PRODUCT)
	@./$(TEST_PRODUCT)
