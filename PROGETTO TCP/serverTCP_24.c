#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #pragma comment(lib, "ws2_32.lib")
    typedef SOCKET socket_t;
    #define CLOSE_SOCKET closesocket
#else
    #include <sys/socket.h>
    #include <arpa/inet.h>
    #include <unistd.h>
    #include <netinet/in.h>
    typedef int socket_t;
    #define CLOSE_SOCKET close
    #define INVALID_SOCKET -1
    #define SOCKET_ERROR -1
#endif

#define PORT 8080
#define BUFFER_SIZE 1024

// Funzione per rimuovere le vocali da una stringa
void rimuovi_vocali(char *str) {
    char *src = str;
    char *dst = str;
    
    while (*src) {
        char c = *src;
        // Controlla se il carattere è una vocale (maiuscola o minuscola)
        if (c != 'a' && c != 'e' && c != 'i' && c != 'o' && c != 'u' &&
            c != 'A' && c != 'E' && c != 'I' && c != 'O' && c != 'U') {
            *dst = c;
            dst++;
        }
        src++;
    }
    *dst = '\0';
}

int main() {
#ifdef _WIN32
    WSADATA wsa;
    // Inizializza Winsock
    printf("Inizializzazione Winsock...\n");
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("Errore nell'inizializzazione di Winsock. Codice errore: %d\n", WSAGetLastError());
        return 1;
    }
#endif
    
    socket_t server_fd, client_socket;
    struct sockaddr_in address, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    char buffer[BUFFER_SIZE] = {0};
    char client_ip[INET_ADDRSTRLEN];
    
    // Crea socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
#ifdef _WIN32
        printf("Errore nella creazione del socket. Codice errore: %d\n", WSAGetLastError());
        WSACleanup();
#else
        perror("Errore nella creazione del socket");
#endif
        return 1;
    }
    
    printf("Socket creato.\n");
    
    // Configura indirizzo server
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    
    // Bind del socket
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) == SOCKET_ERROR) {
#ifdef _WIN32
        printf("Bind fallito. Codice errore: %d\n", WSAGetLastError());
        WSACleanup();
#else
        perror("Bind fallito");
#endif
        CLOSE_SOCKET(server_fd);
        return 1;
    }
    
    printf("Bind effettuato sulla porta %d.\n", PORT);
    
    // Metti il socket in ascolto
    if (listen(server_fd, 3) == SOCKET_ERROR) {
#ifdef _WIN32
        printf("Listen fallito. Codice errore: %d\n", WSAGetLastError());
        WSACleanup();
#else
        perror("Listen fallito");
#endif
        CLOSE_SOCKET(server_fd);
        return 1;
    }
    
    printf("Server in ascolto sulla porta %d...\n", PORT);
    
    // Loop principale del server
    while (1) {
        printf("\nIn attesa di connessioni...\n");
        
        // Accetta connessione
        client_socket = accept(server_fd, (struct sockaddr *)&client_addr, &client_addr_len);
        if (client_socket == INVALID_SOCKET) {
#ifdef _WIN32
            printf("Accettazione fallita. Codice errore: %d\n", WSAGetLastError());
#else
            perror("Accettazione fallita");
#endif
            continue;
        }
        
        // Ottieni l'indirizzo IP del client
        inet_ntop(AF_INET, &(client_addr.sin_addr), client_ip, INET_ADDRSTRLEN);
        
        // Ricevi il messaggio iniziale "Hello"
        memset(buffer, 0, BUFFER_SIZE);
        int valread = recv(client_socket, buffer, BUFFER_SIZE, 0);
        if (valread > 0) {
            printf("Ricevuti dati dal client con indirizzo: %s\n", client_ip);
            printf("Messaggio ricevuto: %s\n", buffer);
        }
        
        // Ricevi la stringa dal client
        memset(buffer, 0, BUFFER_SIZE);
        valread = recv(client_socket, buffer, BUFFER_SIZE, 0);
        if (valread > 0) {
            printf("Stringa ricevuta dal client: %s\n", buffer);
            
            // Rimuovi le vocali
            rimuovi_vocali(buffer);
            
            // Invia la risposta al client
            send(client_socket, buffer, strlen(buffer), 0);
            printf("Stringa senza vocali inviata al client: %s\n", buffer);
        }
        
        // Chiudi la connessione con questo client
        CLOSE_SOCKET(client_socket);
        printf("Connessione chiusa con il client.\n");
    }
    
    // Chiudi il socket del server (non raggiunto in questo loop infinito)
    CLOSE_SOCKET(server_fd);
#ifdef _WIN32
    WSACleanup();
#endif
    
    return 0;
}
