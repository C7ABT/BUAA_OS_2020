.PHONY: all ML0 ML1 clean

all: ML0

ML0: ML1
	gcc ./code/fibo.c ./code/main.c -o fibo -I./include/  

ML1:
	make -C ./code

clean:
	rm -f ./code/*.o
