TARGET_EXEC := main
SRCS := main.c lfsr42.c

GCC := /usr/bin/gcc

GCCFLAGS := -Wall -O3

all: $(TARGET_EXEC)

$(TARGET_EXEC): $(SRCS) *.h
	$(GCC) $(GCCFLAGS) -o $@ $(SRCS)

.PHONY: clean

clean:
	rm -f $(TARGET_EXEC)
