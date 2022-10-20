OUTDIR = bin
CFLAGS = -O2
1 = $(word 1, $(MAKECMDGOALS))
2 = $(word 2, $(MAKECMDGOALS))
3 = $(word 3, $(MAKECMDGOALS))
4 = $(word 4, $(MAKECMDGOALS))

BINARIES=$(subst .c,, \
	$(subst \
		src, \
		$(OUTDIR), \
		$(wildcard src/*.c) \
	) \
)

.PHONY: all clean debug asm

all: $(BINARIES)

clean:
	rm -r $(OUTDIR)

debug: CFLAGS += -g3
debug: $(OUTDIR)/$(2)
	gdb ./$<

asm:
	$(CC) $(CFLAGS) -S -fno-asynchronous-unwind-tables src/$(2).c -o $(2).s

$(OUTDIR)/%: src/%.c
	@mkdir -p "$(dir $@)"
	$(CC) $(CFLAGS) $< -o $@

# Example: make mess_step_over
%: $(OUTDIR)/%
	@:

# If target doesn't exists, do nothing...
%:
	@:
