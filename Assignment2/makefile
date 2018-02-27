TARGETS = assignment2

.PHONY: clean

all: $(TARGETS)

%.o: %.c
	gcc -c -o $@ $<

assignment2: builtin.o launch.o assignment2.o
	gcc -o assignment2 builtin.o launch.o assignment2.o

clean:
	rm -f $(TARGETS) *.o

