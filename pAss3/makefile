CC=g++
CFLAGS=-c -Wall
LDFLAGS=
SOURCES=project2_encryption.cpp project2_decryption.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=project2

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf *.o $(EXECUTABLE)
