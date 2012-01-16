CFLAGS=-fPIC -O2

hijack.so: hijack.o
	$(CC) -shared -o hijack.so hijack.o -ldl

clean:
	rm -f *~ *.o *.so
