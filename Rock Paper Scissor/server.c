#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <time.h>

#pragma comment(lib, "ws2_32.lib")

#define PORT 12345
#define BUFFER_SIZE 256

int get_choice();
int determine_winner(int client_choice, int server_choice);
void display_winner(int winner);


int main() {
    WSADATA wsa;
    SOCKET server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    int addr_len = sizeof(client_addr);

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("Failed to initialize Winsock\n");
        return 1;
    }

    // Create server socket
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        printf("Error creating server socket: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    // Set up server address struct
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Bind server socket to address
    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        printf("Error binding server socket: %d\n", WSAGetLastError());
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }

    // Listen for incoming connections
    if (listen(server_socket, 1) == SOCKET_ERROR) {
        printf("Error listening: %d\n", WSAGetLastError());
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }

    printf("Server listening on port %d...\n", PORT);

    // Accept a client connection
    client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &addr_len);

    if (client_socket == INVALID_SOCKET) {
        printf("Error accepting connection: %d\n", WSAGetLastError());
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }

    printf("Connection accepted from %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
    // Game loop
    while (1) {
        int client_choice, server_choice, winner;

        // Receive client's choice
        recv(client_socket, (char*)&client_choice, sizeof(int), 0);

        // Generate server's choice
        server_choice = get_choice();

        // Send server's choice to the client
        send(client_socket, (char*)&server_choice, sizeof(int), 0);

        // Display the client's choice
        printf("Client's choice: %d\n", client_choice);

        // Determine the winner
        winner = determine_winner(client_choice, server_choice);

        // Send the winner to the client
        send(client_socket, (char*)&winner, sizeof(int), 0);
        // Display the winner
        display_winner(winner);
    }

    // Close sockets
    closesocket(server_socket);
    closesocket(client_socket);
    WSACleanup();
    return 0;
}

int get_choice() {
    int choice;
    printf("Enter your choice (0 for Rock, 1 for Paper, 2 for Scissors): ");
    scanf("%d", &choice);

    // Validate the user input
    while (choice < 0 || choice > 2) {
        printf("Invalid choice. Please enter 0 for Rock, 1 for Paper, or 2 for Scissors: ");
        scanf("%d", &choice);
    }

    return choice;
}

int determine_winner(int client_choice, int server_choice) {
    // Determine the winner based on choices
    if (client_choice == server_choice) {
        return 0;  // It's a tie
    } else if ((client_choice == 0 && server_choice == 2) ||
               (client_choice == 1 && server_choice == 0) ||
               (client_choice == 2 && server_choice == 1)) {
        return 1;  // Client wins
    } else {
        return 2;  // Server wins
    }
}
void display_winner(int winner) {
    // Display the winner
    if (winner == 0) {
        printf("It's a tie!\n");
    } else if (winner == 1) {
        printf("You lose!\n");
    } else {
        printf("You win!\n");
    }
}
