# Applicazione UDP Client/Server in C

Applicazione UDP client/server in C che implementa un protocollo di comunicazione per la rimozione delle vocali da stringhe.

## Descrizione del Protocollo

1. Il client legge hostname e porta del server da tastiera
2. Il client invia il messaggio iniziale "Hello" al server
3. Il server visualizza informazioni sul client (nome host e indirizzo IP)
4. Il client legge una stringa da tastiera e la invia al server
5. Il server riceve la stringa, la visualizza, rimuove tutte le vocali e la invia al client
6. Il server resta in attesa di altri dati
7. Il client visualizza la risposta ricevuta e termina

## Requisiti

- Windows (usa Winsock2)
- Compilatore C (MinGW/GCC o Visual Studio)

## Compilazione

### Con MinGW/GCC:
```bash
gcc server.c -o server.exe -lws2_32
gcc client.c -o client.exe -lws2_32
```

### Con Visual Studio (Developer Command Prompt):
```bash
cl server.c ws2_32.lib
cl client.c ws2_32.lib
```

## Utilizzo

### Avvio del Server

```bash
server.exe
```

Il server si avvierà sulla porta **12345** (modificabile nel codice) e resterà in ascolto.

### Avvio del Client

In un altro terminale:

```bash
client.exe
```

Il client chiederà:
1. Nome dell'host del server (es. `localhost` o `127.0.0.1`)
2. Numero di porta del server (es. `12345`)
3. Una stringa da inviare al server

## Esempio di Esecuzione

### Server
```
Inizializzazione Winsock...
Server UDP avviato su porta 12345
In attesa di connessioni...

Ricevuti dati dal client nome: DESKTOP-XXX indirizzo: 127.0.0.1
Messaggio ricevuto: Hello

Ricevuti dati dal client nome: DESKTOP-XXX indirizzo: 127.0.0.1
Stringa ricevuta: Ciao mondo
Stringa senza vocali: C mnd
Risposta inviata al client
```

### Client
```
Inserisci il nome dell'host del server: localhost
Inserisci il numero di porta del server: 12345
Messaggio 'Hello' inviato al server

Inserisci una stringa da inviare al server: Ciao mondo
Stringa 'Ciao mondo' inviata al server

Stringa 'C mnd' ricevuta dal server nome: localhost indirizzo: 127.0.0.1

Processo client terminato
```

## Note

- Il protocollo UDP non garantisce la consegna dei pacchetti
- Il server può gestire più client in sequenza
- Per terminare il server, premere Ctrl+C
- Il codice usa Winsock2 (Windows Socket API)
