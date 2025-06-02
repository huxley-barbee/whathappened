ifeq ($(fortify),true)
EXTRA_CFLAGS = -D_FORTIFY_SOURCE=2 -O2
endif

CC = gcc
CFLAGS = -Wall -Wextra -Werror -D_GNU_SOURCE $(EXTRA_CFLAGS) \
         -fsanitize=address -I./parser -I./explain -I./input -I./output \
         -I./narration -I./plugins
LDFLAGS = -fsanitize=address

SRC = \
    main.c \
    parser/audit_line.c \
    explain/explanation.c \
    input/input_textlog.c \
    output/render_pretty.c \
    narration/narration_engine.c \
    plugins/plugin_syscall.c

OBJ = $(SRC:.c=.o)
TARGET = whathappened

# Unit Test sources and binary
TEST_SRCS = tests/test_audit_line.c tests/test_utils.c
TEST_BIN  = tests/test_audit_line

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) $(LDFLAGS) -o $@

check:
	cppcheck --enable=all --inconclusive --std=c99 --quiet --suppress=missingIncludeSystem .

test: $(TEST_BIN)
	@for t in $(TEST_BIN); do echo "Running $$t..."; ./$$t || exit 1; done

tests/test_audit_line: $(TEST_SRCS) parser/audit_line.c parser/audit_line.h
	$(CC) $(CFLAGS) -o $@ $^ -lcmocka $(LDFLAGS)

clean:
	rm -f $(OBJ) $(TARGET) $(TEST_BIN)

