BINARIES=$(shell ls src/*.c | sed -E "s/src\/([^.]+)\.c/\1/")

all: $(BINARIES)

%: src/%.c
	$(CC) $< -o $@

clean:
	rm -f $(BINARIES)
