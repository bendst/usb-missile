PROG		=	main
CC		=	gcc
CFLAGS		=	-Wall -Og
LDFLAGS		=	-ludev

all: $(PROG)

$(PROG): $(PROG).c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

clean:
	rm -f $(PROG)
