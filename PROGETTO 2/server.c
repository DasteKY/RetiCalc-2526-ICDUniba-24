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
    #include <netdb.h>
    typedef int socket_t;
    #define CLOSE_SOCKET close
    #define INVALID_SOCKET -1
    #define SOCKET_ERROR -1
#endif

#define PORT 12345
#define BUFFER_SIZE 1024

// Funzione per rimuovere le vocali da una stringa
void remove_vowels(char *str, char *result) {
    int j = 0;
    for (int i = 0; str[i] != '\0'; i++) {
        char c = str[i];
        if (c != 'a' && c != 'e' && c != 'i' && c != 'o' && c != 'u' &&
            c != 'A' && c != 'E' && c != 'I' && c != 'O' && c != 'U') {
            result[j++] = c;
        }
    }
    result[j] = '\0';
}

int main() {
#ifdef _WIN32
    WSADATA wsa;
    // Inizializzazione Winsock
    printf("Inizializzazione Winsock...\n");
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("Errore WSAStartup. Codice errore: %d\n", WSAGetLastError());
        return 1;
    }
#endif
    
    socket_t server_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    char buffer[BUFFER_SIZE];
    char response[BUFFER_SIZE];
    int recv_len;
    struct hostent *client_host;
    
    // Creazione socket UDP
    if ((server_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == INVALID_SOCKET) {
#ifdef _WIN32
        printf("Errore creazione socket. Codice errore: %d\n", WSAGetLastError());
        WSACleanup();
#else
        perror("Errore creazione socket");
#endif
        return 1;
    }
    
    // Configurazione indirizzo server
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);
    
    // Bind del socket
    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
#ifdef _WIN32
        printf("Errore bind. Codice errore: %d\n", WSAGetLastError());
        WSACleanup();
#else
        perror("Errore bind");
#endif
        CLOSE_SOCKET(server_socket);
        return 1;
    }
    
    printf("Server UDP avviato su porta %d\n", PORT);
    printf("In attesa di connessioni...\n\n");
    
    // Loop principale del server
    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        
        // Ricezione dati dal client
        recv_len = recvfrom(server_socket, buffer, BUFFER_SIZE - 1, 0, 
                           (struct sockaddr*)&client_addr, &client_addr_len);
        
        if (recv_len == SOCKET_ERROR) {
#ifdef _WIN32
            printf("Errore recvfrom. Codice errore: %d\n", WSAGetLastError());
#else
            perror("Errore recvfrom");
#endif
            continue;
        }
        
        buffer[recv_len] = '\0';
        
        // Ottieni informazioni sul client
        char client_ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &(client_addr.sin_addr), client_ip, INET_ADDRSTRLEN);
        
        client_host = gethostbyaddr((char*)&client_addr.sin_addr, sizeof(client_addr.sin_addr), AF_INET);
        char *client_hostname = (client_host != NULL) ? client_host->h_name : "unknown";
        
        // Visualizza informazioni sul client
        printf("Ricevuti dati dal client nome: %s indirizzo: %s\n", client_hostname, client_ip);
        
        // Se è il messaggio "Hello", continua ad aspettare
        if (strcmp(buffer, "Hello") == 0) {
            printf("Messaggio ricevuto: %s\n\n", buffer);
            continue;
        }
        
        // Visualizza la stringa ricevuta
        printf("Stringa ricevuta: %s\n", buffer);
        
        // Rimuovi le vocali
        memset(response, 0, BUFFER_SIZE);
        remove_vowels(buffer, response);
        printf("Stringa senza vocali: %s\n", response);
        
        // Invia la risposta al client
        if (sendto(server_socket, response, strlen(response), 0, 
                  (struct sockaddr*)&client_addr, client_addr_len) == SOCKET_ERROR) {
#ifdef _WIN32
            printf("Errore sendto. Codice errore: %d\n", WSAGetLastError());
#else
            perror("Errore sendto");
#endif
        } else {
            printf("Risposta inviata al client\n\n");
        }
    }
    
    // Chiusura socket e cleanup
    CLOSE_SOCKET(server_socket);
#ifdef _WIN32
    WSACleanup();
#endif
    
    return 0;
}
