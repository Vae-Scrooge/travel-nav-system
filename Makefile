CC := gcc
CFLAGS ?= -std=c11 -Wall -Wextra -Wpedantic -g -Isrc/core -Isrc/console -Isrc/manager
LDFLAGS ?=
TEST_CFLAGS ?= -DUNIT_TEST

BUILD_DIR := build
APP := $(BUILD_DIR)/toursys
TEST_BIN := $(BUILD_DIR)/test_runner
PERF_BIN := $(BUILD_DIR)/perf_benchmark

APP_SOURCES := main.c \
               src/core/global.c \
               src/core/input.c \
               src/core/graph.c \
               src/core/travels.c \
               src/core/password.c \
               src/console/menu.c \
               src/console/graph_console.c \
               src/console/travels_console.c \
               src/console/user_console.c \
               src/manager/userManager.c \
               src/manager/stats.c \
               src/manager/backup.c

TEST_SOURCES := tests/test_runner.c \
                src/core/global.c \
                src/core/input.c \
                src/core/graph.c \
                src/core/travels.c \
                src/core/password.c \
                src/manager/userManager.c \
                src/manager/stats.c \
                src/manager/backup.c

PERF_SOURCES := tests/perf_benchmark.c \
                src/core/global.c \
                src/core/graph.c \
                src/core/travels.c

.PHONY: all build test coverage perf clean dist

all: build

ifeq ($(OS),Windows_NT)
	MKDIR = mkdir
	RMDIR = rd /s /q
	CP = copy
else
	MKDIR = mkdir -p
	RMDIR = rm -rf
	CP = cp
endif

build:
	$(MKDIR) $(BUILD_DIR)
	$(CC) $(CFLAGS) $(APP_SOURCES) $(LDFLAGS) -o $(APP)

test:
	$(MKDIR) $(BUILD_DIR)
	$(CC) $(CFLAGS) $(TEST_CFLAGS) $(TEST_SOURCES) $(LDFLAGS) -o $(TEST_BIN)
	$(TEST_BIN)

coverage: CFLAGS += --coverage -O0
coverage: LDFLAGS += --coverage
coverage: clean test

perf:
	$(MKDIR) $(BUILD_DIR)
	$(CC) $(CFLAGS) $(PERF_SOURCES) $(LDFLAGS) -o $(PERF_BIN)
	$(PERF_BIN)

dist: clean build
	$(MKDIR) $(BUILD_DIR)/dist
	$(CP) $(APP) $(BUILD_DIR)/dist/
	$(CP) data/graphParams.txt data/graphVertex.txt data/graphEdge.txt data/user.txt $(BUILD_DIR)/dist/

clean:
	-$(RMDIR) $(BUILD_DIR)
	-$(RM) *.gcda *.gcno *.gcov
	-$(RM) tests/*.gcda tests/*.gcno tests/*.gcov
	-$(RM) src/core/*.gcda src/core/*.gcno src/core/*.gcov
	-$(RM) src/console/*.gcda src/console/*.gcno src/console/*.gcov
	-$(RM) src/manager/*.gcda src/manager/*.gcno src/manager/*.gcov
