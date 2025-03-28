CC= gcc
CFLAGS= -Wall -pthread
TARGET= taller5

##Directiva principal

all: $(TARGET)

##Acciones
$(TARGET): principal.o barbero.o
	$(CC) $(CFLAGS) -o $(TARGET) principal.o barbero.o

principal.o: principal.c barbero.h
	$(CC) $(CFLAGS) -c principal.c

barbero.o: barbero.c barbero.h
	$(CC) $(CFLAGS) -c barbero.c

##Limpiar

clean:
	rm -f *.o $(TARGET)
