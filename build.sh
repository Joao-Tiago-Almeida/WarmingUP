# correr este fichero para compilar. Escrever no terminal "sh build.sh"
gcc *.c -g -I/usr/local/include -Wall -pedantic -std=c99 -L/usr/local/lib -lm -lSDL2 -lSDL2_ttf -lSDL2_image -o WarmingUp -g
