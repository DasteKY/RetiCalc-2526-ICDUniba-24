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

#define BUFFER_SIZE 1024

int main() {
#ifdef _WIN32
    WSADATA wsa;
    // Inizializzazione Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("Errore WSAStartup. Codice errore: %d\n", WSAGetLastError());
        return 1;
    }
#endif
    
    socket_t client_socket;
    struct sockaddr_in server_addr;
    char server_host[256];
    int server_port;
    char buffer[BUFFER_SIZE];
    char user_input[BUFFER_SIZE];
    int recv_len;
    struct hostent *server_host_info;
    struct sockaddr_in from_addr;
    socklen_t from_len = sizeof(from_addr);
    
    // Lettura host e porta da tastiera
    printf("Inserisci il nome dell'host del server: ");
    fgets(server_host, sizeof(server_host), stdin);
    server_host[strcspn(server_host, "\n")] = '\0'; // Rimuovi newline
    
    printf("Inserisci il numero di porta del server: ");
    scanf("%d", &server_port);
    getchar(); // Consuma il newline
    
    // Creazione socket UDP
    if ((client_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == INVALID_SOCKET) {
#ifdef _WIN32
        printf("Errore creazione socket. Codice errore: %d\n", WSAGetLastError());
        WSACleanup();
#else
        perror("Errore creazione socket");
#endif
        return 1;
    }
    
    // Risoluzione nome host
    server_host_info = gethostbyname(server_host);
    if (server_host_info == NULL) {
        printf("Errore risoluzione host\n");
        CLOSE_SOCKET(client_socket);
#ifdef _WIN32
        WSACleanup();
#endif
        return 1;
    }
    
    // Configurazione indirizzo server
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    memcpy(&server_addr.sin_addr, server_host_info->h_addr_list[0], server_host_info->h_length);
    server_addr.sin_port = htons(server_port);
    
    // Invio messaggio iniziale "Hello"
    char *initial_message = "Hello";
    if (sendto(client_socket, initial_message, strlen(initial_message), 0, 
              (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
#ifdef _WIN32
        printf("Errore sendto. Codice errore: %d\n", WSAGetLastError());
        WSACleanup();
#else
        perror("Errore sendto");
#endif
        CLOSE_SOCKET(client_socket);
        return 1;
    }
    printf("Messaggio '%s' inviato al server\n\n", initial_message);
    
    // Lettura stringa da tastiera
    printf("Inserisci una stringa da inviare al server: ");
    fgets(user_input, sizeof(user_input), stdin);
    user_input[strcspn(user_input, "\n")] = '\0'; // Rimuovi newline
    
    // Invio stringa al server
    if (sendto(client_socket, user_input, strlen(user_input), 0, 
              (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
#ifdef _WIN32
        printf("Errore sendto. Codice errore: %d\n", WSAGetLastError());
        WSACleanup();
#else
        perror("Errore sendto");
#endif
        CLOSE_SOCKET(client_socket);
        return 1;
    }
    printf("Stringa '%s' inviata al server\n\n", user_input);
    
    // Ricezione risposta dal server
    memset(buffer, 0, BUFFER_SIZE);
    recv_len = recvfrom(client_socket, buffer, BUFFER_SIZE - 1, 0, 
                       (struct sockaddr*)&from_addr, &from_len);
    
    if (recv_len == SOCKET_ERROR) {
#ifdef _WIN32
        printf("Errore recvfrom. Codice errore: %d\n", WSAGetLastError());
        WSACleanup();
#else
        perror("Errore recvfrom");
#endif
        CLOSE_SOCKET(client_socket);
        return 1;
    }
    
    buffer[recv_len] = '\0';
    
    // Ottieni informazioni sul server
    char server_ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(from_addr.sin_addr), server_ip, INET_ADDRSTRLEN);
    
    struct hostent *from_host = gethostbyaddr((char*)&from_addr.sin_addr, 
                                              sizeof(from_addr.sin_addr), AF_INET);
    char *from_hostname = (from_host != NULL) ? from_host->h_name : server_host;
    
    // Visualizza la risposta
    printf("Stringa '%s' ricevuta dal server nome: %s indirizzo: %s\n", 
           buffer, from_hostname, server_ip);
    
    // Chiusura socket e cleanup
    CLOSE_SOCKET(client_socket);
#ifdef _WIN32
    WSACleanup();
#endif
    
    printf("\nProcesso client terminato\n");
    
    return 0;
}
