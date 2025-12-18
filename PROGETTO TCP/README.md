# Applicazione TCP Client/Server in C

## Descrizione
Applicazione TCP client/server che implementa il seguente protocollo:
1. Il client invia "Hello" al server
2. Il server visualizza l'indirizzo IP del client
3. Il client legge una stringa da stdin e la invia al server
4. Il server elimina le vocali dalla stringa e la rinvia al client
5. Il client visualizza la risposta e termina
6. Il server resta in attesa di nuove connessioni

## Compilazione

### Con Make (MinGW richiesto)
```bash
make
```

### Compilazione manuale
```bash
gcc -Wall -o server.exe server.c -lws2_32
gcc -Wall -o client.exe client.c -lws2_32
```

## Utilizzo

### 1. Avviare il server
Aprire un terminale ed eseguire:
```bash
./server.exe
```

Il server rimarrà in ascolto sulla porta 8080.

### 2. Avviare il client
Aprire un altro terminale ed eseguire:
```bash
./client.exe
```

Il client:
- Si connetterà al server
- Invierà il messaggio "Hello"
- Chiederà di inserire una stringa
- Visualizzerà la risposta del server (stringa senza vocali)
- Chiuderà la connessione

### Esempio di esecuzione

**Server:**
```
Inizializzazione Winsock...
Socket creato.
Bind effettuato sulla porta 8080.
Server in ascolto sulla porta 8080...

In attesa di connessioni...
Ricevuti dati dal client con indirizzo: 127.0.0.1
Messaggio ricevuto: Hello
Stringa ricevuta dal client: Ciao mondo
Stringa senza vocali inviata al client: C mnd
Connessione chiusa con il client.
```

**Client:**
```
Inizializzazione Winsock...
Socket creato.
Connessione al server...
Connesso al server.
Messaggio 'Hello' inviato al server.

Inserisci una stringa: Ciao mondo
Stringa inviata al server: Ciao mondo
Risposta ricevuta dal server: C mnd
Connessione chiusa.
```

## Note
- Il server è configurato per accettare connessioni multiple in sequenza
- La porta predefinita è 8080
- L'applicazione è progettata per Windows e utilizza Winsock2
