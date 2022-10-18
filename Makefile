OUTDIR=bin

BINARIES=$(subst .c,, \
	$(subst \
		src, \
		$(OUTDIR), \
		$(wildcard src/*.c) \
	) \
)

.PHONY: all clean

all: $(BINARIES)

clean:
	rm -r $(OUTDIR)

$(OUTDIR)/%: src/%.c
	@mkdir -p "$(dir $@)"
	$(CC) $< -o $@

# Example: make mess_step_over
%: $(OUTDIR)/%
	@true Do nothing here...
