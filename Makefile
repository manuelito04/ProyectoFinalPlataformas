CC = gcc
CFLAGS = -Wall -O2
SRC = proyecto.c
OBJS = $(SRC:.c=.o)
LIBS = -lm  # Agregar la biblioteca matemática

p2: $(OBJS)
	$(CC) $(CFLAGS) -o exe $(OBJS) $(LIBS) 

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f p2 exe $(OBJS) data.txt subdominio.txt plot.gnuplot

run_p2: p2
	./exe 


