#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

#define PORT 12345
#define BUFFER_SIZE 256

void display_winner(int winner);
int main() {

    WSADATA wsa;
    SOCKET client_socket;
    struct sockaddr_in server_addr;


    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("Failed to initialize Winsock\n");
        return 1;
    }

    // Create socket
    if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        printf("Failed to create socket\n");
        WSACleanup();
        return 1;
    }

    // Set up server address struct
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("112.134.151.103");  // Replace with the server's IP address

    // Connect to server
    if (connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        printf("Failed to connect to server\n");
        closesocket(client_socket);
        WSACleanup();
        return 1;
    }

    printf("Connected to server\n");

    // Game loop
    while (1) {
        int choice, server_choice, winner;

        // Prompt the user to enter a choice
        printf("Enter your choice (0 for Rock, 1 for Paper, 2 for Scissors): ");
        scanf("%d", &choice);

        // Send the choice to the server
        send(client_socket, (char*)&choice, sizeof(int), 0);

        // Receive the server's choice
        recv(client_socket, (char*)&server_choice, sizeof(int), 0);

        // Display the server's choice
        printf("Server's choice: %d\n", server_choice);

        // Receive the winner from the server
        recv(client_socket, (char*)&winner, sizeof(int), 0);

        // Display the winner
        display_winner(winner);
    }

    // Cleanup Winsock
    closesocket(client_socket);
    WSACleanup();

    return 0;
}

void display_winner(int winner) {
    // Display the winner
    if (winner == 0) {
        printf("It's a tie!\n");
    } else if (winner == 1) {
        printf("You win!\n");
    } else {
        printf("You lose!\n");
    }
}
