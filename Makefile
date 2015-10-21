PREFIX?=/usr/X11R6
CFLAGS?=-Os -pedantic -Wall

all:
	$(CC) $(CFLAGS) -I$(PREFIX)/include librebox.c -L$(PREFIX)/lib -lX11 -o librebox

clean:
	rm -f librebox

