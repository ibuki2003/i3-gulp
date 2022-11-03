NAME = gulp
SRCDIR = src
OUTDIR = bin

LIBS = -lX11

all: $(NAME)
$(NAME): $(wildcard $(SRCDIR)/*.c)
	$(CC) $(CFLAGS) $(LIBS) -o $@ $^

clean:
	rm -f $(NAME)

.PHONY: all clean
