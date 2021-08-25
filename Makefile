BINARIES=$(shell ls *.c | sed "s/\.c//")

all: $(BINARIES)

%: %.c
	$(CC) $< -o $@

clean:
	rm -f $(BINARIES)
