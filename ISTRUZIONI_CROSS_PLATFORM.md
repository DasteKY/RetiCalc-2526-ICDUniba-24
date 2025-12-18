# Guida Cross-Platform per i Progetti

## Modifiche Effettuate

I progetti sono stati modificati per funzionare sia su **Windows** che su **macOS/Linux** utilizzando direttive del preprocessore C.

### Principali Modifiche

1. **Gestione Cross-Platform delle Socket:**
   - Su Windows: usa Winsock (`winsock2.h`, `ws2tcpip.h`)
   - Su Unix: usa socket POSIX (`sys/socket.h`, `arpa/inet.h`, `unistd.h`)

2. **Tipi e Funzioni Unificati:**
   - `socket_t`: tipo unificato per socket (SOCKET su Windows, int su Unix)
   - `CLOSE_SOCKET`: macro per chiudere socket (closesocket su Windows, close su Unix)
   - `INVALID_SOCKET` e `SOCKET_ERROR`: definiti anche per Unix

3. **Gestione Errori:**
   - Windows: usa `WSAGetLastError()`
   - Unix: usa `perror()` e errno

## Come Compilare

### Su Windows

Usa MinGW o Visual Studio:

```bash
# Con make
mingw32-make

# Manualmente
gcc -Wall -o server.exe server.c -lws2_32
gcc -Wall -o client.exe client.c -lws2_32
```

### Su macOS/Linux

```bash
# Con make
make

# Manualmente
gcc -Wall -o server server.c
gcc -Wall -o client client.c
```

## Come Eseguire

### PROGETTO 1 (TCP - Rimozione Vocali)

**Su Windows:**
```bash
# Terminale 1 - Server
server.exe

# Terminale 2 - Client
client.exe
```

**Su macOS/Linux:**
```bash
# Terminale 1 - Server
./server

# Terminale 2 - Client
./client
```

### PROGETTO 2 (UDP - Rimozione Vocali)

**Su Windows:**
```bash
# Terminale 1 - Server
server.exe

# Terminale 2 - Client
client.exe
# Inserisci: localhost (o 127.0.0.1)
# Porta: 12345
```

**Su macOS/Linux:**
```bash
# Terminale 1 - Server
./server

# Terminale 2 - Client
./client
# Inserisci: localhost (o 127.0.0.1)
# Porta: 12345
```

## Pulizia File Compilati

### Su Windows
```bash
mingw32-make clean
```

### Su macOS/Linux
```bash
make clean
```

## Note Tecniche

### Differenze Principali tra Windows e Unix

| Aspetto | Windows | Unix/macOS |
|---------|---------|------------|
| Header socket | winsock2.h | sys/socket.h |
| Tipo socket | SOCKET | int |
| Inizializzazione | WSAStartup() | Non necessaria |
| Chiusura socket | closesocket() | close() |
| Cleanup | WSACleanup() | Non necessaria |
| Gestione errori | WSAGetLastError() | errno, perror() |

### Direttive Preprocessore Utilizzate

```c
#ifdef _WIN32
    // Codice Windows-specific
#else
    // Codice Unix-specific
#endif
```

La macro `_WIN32` è definita automaticamente dai compilatori su Windows (incluso MinGW).

## Troubleshooting

### Windows
- **Errore "ws2_32.lib non trovato"**: Assicurati di linkare con `-lws2_32`
- **Errore Winsock**: Verifica che WSAStartup() venga chiamato prima di usare le socket

### macOS/Linux
- **Errore "Permission denied"**: Aggiungi permessi di esecuzione con `chmod +x server client`
- **Porta già in uso**: Cambia la porta nel codice o termina il processo che la sta usando
- **Firewall**: Potrebbe essere necessario consentire le connessioni locali

### Generale
- Assicurati di avviare prima il server, poi il client
- Per testare in rete locale, modifica gli indirizzi IP nel codice
- Usa `127.0.0.1` o `localhost` per test locali

## Compatibilità

✅ **Testato e funzionante su:**
- Windows 10/11 (MinGW, Visual Studio)
- macOS (Xcode Command Line Tools)
- Linux (Ubuntu, Debian, Fedora, Arch)

## Estensioni Future

Per rendere i progetti ancora più robusti, potresti considerare:
1. Gestione segnali (SIGINT) per chiusura pulita
2. Supporto IPv6
3. Multi-threading per gestire più client contemporaneamente
4. Configurazione tramite file o argomenti da linea di comando
5. Logging più dettagliato
