all:
	gcc -o main *.c -Wall

clean:
	rm -f $(OBJS) $(OUT)