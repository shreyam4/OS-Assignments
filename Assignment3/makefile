TARGETS = assignment3

.PHONY: clean

all: $(TARGETS)

%.o: %.c
	gcc -c -o $@ $<

assignment3: builtin.o launch.o main.o
	gcc -o assignment3 builtin.o launch.o main.o

clean:
	rm -f $(TARGETS) *.o

