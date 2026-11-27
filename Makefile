CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -pedantic
OBJ = main.o entrada.o paciente.o fila.o atendimento.o painel.o

clinica: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJ)

%.o: %.c $(wildcard *.h)
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(OBJ) clinica clinica.exe
