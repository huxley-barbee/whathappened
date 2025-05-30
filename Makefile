CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c99 \
         -I./parser -I./explain -I./input -I./output -I./narration -I./plugins
LDFLAGS =

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
TESTS = tests/test_audit_line

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) $(LDFLAGS) -o $@

check:
	cppcheck --enable=all --inconclusive --std=c99 --quiet --suppress=missingIncludeSystem .

test: $(TESTS)
	@for t in $(TESTS); do echo "Running $$t..."; ./$$t || exit 1; done

tests/test_audit_line: tests/test_audit_line.c parser/audit_line.c parser/audit_line.h
	$(CC) $(CFLAGS) -o $@ $^ -lcmocka

clean:
	rm -f $(OBJ) $(TARGET) $(TESTS)

