# Makefile cross-platform per compilare client e server TCP
# Supporta Windows, Linux e macOS

CC = gcc
CFLAGS = -Wall

# Rileva il sistema operativo
ifeq ($(OS),Windows_NT)
    # Windows
    LIBS = -lws2_32
    SERVER_TARGET = server.exe
    CLIENT_TARGET = client.exe
    RM = del /Q
    EXE_EXT = .exe
else
    # Unix-like (Linux, macOS)
    LIBS = 
    SERVER_TARGET = server
    CLIENT_TARGET = client
    RM = rm -f
    EXE_EXT = 
endif

all: $(SERVER_TARGET) $(CLIENT_TARGET)

$(SERVER_TARGET): server.c
	$(CC) $(CFLAGS) -o $(SERVER_TARGET) server.c $(LIBS)

$(CLIENT_TARGET): client.c
	$(CC) $(CFLAGS) -o $(CLIENT_TARGET) client.c $(LIBS)

clean:
ifeq ($(OS),Windows_NT)
	$(RM) server.exe client.exe 2>nul || true
else
	$(RM) server client
endif

.PHONY: all clean
