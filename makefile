all:
	gcc -o objectmover *.c -Wall

clean:
	rm -f $(OBJS) $(OUT)