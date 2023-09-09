CC=clang
CFLAGS=-Wall
FRAMEWORKS=-framework ApplicationServices
OUTPUT=omniscroll

all:
	$(CC) $(CFLAGS) -o $(OUTPUT) omniscroll.c $(FRAMEWORKS)

clean:
	rm -f $(OUTPUT)