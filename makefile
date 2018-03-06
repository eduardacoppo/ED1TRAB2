all: prog prog1

prog:
    gcc bitmap.c Compacta.c compress.c list.c tree.c -o Compacta 

prog1:
    gcc bitmap.c Descompacta.c compress.c list.c tree.c -o Descompacta 
