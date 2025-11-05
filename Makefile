# Compiler and flags
CC = gcc
# -Wall enables all warnings, -g adds debug symbols, -Iinc tells gcc where to find headers
CFLAGS = -Wall -g -Iinc -std=c99

# Directories
SRCDIR = src
OBJDIR = obj
INCDIR = inc

# Target executable
TARGET = sudoku

# Find all .c files in src directory and add main.c
SRCS = $(wildcard $(SRCDIR)/*.c) main.c

# Object files: Create a corresponding .o file in OBJDIR for each .c file
OBJS = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(filter $(SRCDIR)/%.c, $(SRCS)))
# --- THIS IS THE CORRECTED LINE ---
# It now correctly filters out the src/ files, only processing root files like main.c
OBJS += $(patsubst %.c, $(OBJDIR)/%.o, $(filter-out $(SRCDIR)/%.c, $(SRCS)))

# Header files (for dependency tracking)
DEPS = $(wildcard $(INCDIR)/*.h)

# Default rule: build all
all: $(TARGET)

# Rule to link the executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Rule to compile .c files from src/ into .o files in obj/
# It depends on the source file and all header files (DEPS)
# The '| $(OBJDIR)' is an "order-only prerequisite" to ensure the obj dir exists
$(OBJDIR)/%.o: $(SRCDIR)/%.c $(DEPS) | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Rule to compile .c files from the root (i.e., main.c) into obj/
$(OBJDIR)/%.o: %.c $(DEPS) | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Rule to create the object directory
$(OBJDIR):
	mkdir -p $(OBJDIR)

# Rule to clean up build files
clean:
	rm -rf $(OBJDIR) $(TARGET)

# Explicitly state non-file targets
.PHONY: all clean