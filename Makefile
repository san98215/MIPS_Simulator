solution: main.o instructions.o
	gcc -o instructions main.o instructions.o -lm
%.o: %.c
	gcc -c -o $@ $<

clean:
	rm -f *.o instructions