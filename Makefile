CC := gcc
CFLAGS ?= -std=c11 -Wall -Wextra -Wpedantic -g
LDFLAGS ?=
TEST_CFLAGS ?= -DUNIT_TEST

BUILD_DIR := build
APP := $(BUILD_DIR)/toursys
TEST_BIN := $(BUILD_DIR)/test_runner
PERF_BIN := $(BUILD_DIR)/perf_benchmark

APP_SOURCES := main.c menu.c global.c input.c graph.c graph_console.c travels.c travels_console.c userManager.c user_console.c password.c stats.c
TEST_SOURCES := tests/test_runner.c global.c input.c graph.c travels.c userManager.c password.c stats.c
PERF_SOURCES := tests/perf_benchmark.c global.c graph.c travels.c

.PHONY: all build test coverage perf clean dist

all: build

build:
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(APP_SOURCES) $(LDFLAGS) -o $(APP)

test:
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(TEST_CFLAGS) $(TEST_SOURCES) $(LDFLAGS) -o $(TEST_BIN)
	$(TEST_BIN)

coverage: CFLAGS += --coverage -O0
coverage: LDFLAGS += --coverage
coverage: clean test

perf:
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(PERF_SOURCES) $(LDFLAGS) -o $(PERF_BIN)
	$(PERF_BIN)

dist: clean build
	mkdir -p $(BUILD_DIR)/dist
	cp $(APP) $(BUILD_DIR)/dist/
	cp graphParams.txt graphVertex.txt graphEdge.txt user.txt $(BUILD_DIR)/dist/

clean:
	rm -rf $(BUILD_DIR) *.gcda *.gcno *.gcov tests/*.gcda tests/*.gcno tests/*.gcov
