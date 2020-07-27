all: ols 

fourth: ols.c
	gcc -o ols ols.c

clean:
	rm -rf ols