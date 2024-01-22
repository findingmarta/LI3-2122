all: guiao1-ex1 guiao1-ex2 exercicio1 exercicio2

guiao1-ex1: src/guiao1-ex1.c src/guiao1-ex1.h
        gcc -o obj/guiao1-ex1.o -c src/guiao1-ex1.c


guiao1-ex2: src/guiao1-ex2.c src/guiao1-ex2.h
        gcc -o obj/guiao1-ex2.o -c src/guiao1-ex2.c


exercicio1:
        gcc obj/guiao1-ex1.o main1.c -o guiao-1\ exercicio-1

exercicio2:
        gcc obj/guiao1-ex2.o main2.c -o guiao-1\ exercicio-2

clean:
        rm -rf obj/*.o all
