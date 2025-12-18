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
    
    socket_t sock;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0};
    char input[BUFFER_SIZE];
    
    // Crea socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
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
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    
    // Converti indirizzo IPv4 da testo a binario
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        printf("Indirizzo non valido o non supportato.\n");
        CLOSE_SOCKET(sock);
#ifdef _WIN32
        WSACleanup();
#endif
        return 1;
    }
    
    // Connetti al server
    printf("Connessione al server...\n");
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == SOCKET_ERROR) {
#ifdef _WIN32
        printf("Connessione fallita. Codice errore: %d\n", WSAGetLastError());
#else
        perror("Connessione fallita");
#endif
        CLOSE_SOCKET(sock);
#ifdef _WIN32
        WSACleanup();
#endif
        return 1;
    }
    
    printf("Connesso al server.\n");
    
    // Passo 1: Invia messaggio iniziale "Hello"
    char *hello = "Hello";
    send(sock, hello, strlen(hello), 0);
    printf("Messaggio 'Hello' inviato al server.\n");
    
    // Passo 3: Leggi stringa dallo standard input
    printf("\nInserisci una stringa: ");
    fgets(input, BUFFER_SIZE, stdin);
    
    // Rimuovi il newline se presente
    input[strcspn(input, "\n")] = 0;
    
    // Invia la stringa al server
    send(sock, input, strlen(input), 0);
    printf("Stringa inviata al server: %s\n", input);
    
    // Passo 5: Ricevi la risposta dal server
    memset(buffer, 0, BUFFER_SIZE);
    int valread = recv(sock, buffer, BUFFER_SIZE, 0);
    if (valread > 0) {
        printf("Risposta ricevuta dal server: %s\n", buffer);
    }
    
    // Chiudi la connessione
    CLOSE_SOCKET(sock);
#ifdef _WIN32
    WSACleanup();
#endif
    
    printf("Connessione chiusa.\n");
    
    system("pause");

    return 0;
}
