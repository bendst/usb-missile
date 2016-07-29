PROG		=	main
CC		=	gcc
CFLAGS		=	-Wall
LDFLAGS		=	-ludev

all: $(PROG)

$(PROG): $(PROG).c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

clean:
	rm -f $(PROG)
