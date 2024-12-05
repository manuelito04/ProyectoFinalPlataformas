CC = gcc # Ejecutar el programa
CFLAGS = -Wall -O2
SRC = proyecto.c # Archivo a ejecutar
OBJS = $(SRC:.c=.o)
LIBS = -lm # Incluir librerias

p2: $(OBJS) # Comandos a ejecutar con el puntero p2.
	$(CC) $(CFLAGS) -o exe $(OBJS) $(LIBS) 

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f p2 exe $(OBJS) data.txt subdominio.txt plot.gnuplot

run_p2: p2 # Obtener ejecutable exe a partir del puntero p2.
	./exe 


