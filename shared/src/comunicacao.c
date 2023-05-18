#include <stdio.h>
#include <stdlib.h>
#include <proto.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <comunicacao.h>
#include <arpa/inet.h>
#include <shared_util.h>

MensagemIn *monta_request(EstadoEstacionamento *e)
{
    MensagemIn *req = (MensagemIn *)malloc(sizeof(MensagemIn));
    return req;
}

char *tranformar_request_em_string(MensagemIn *req)
{
    char *frame = (char *)malloc(MAX_FRAME_SIZE * sizeof(char));
    memcpy(frame, &req, sizeof(frame));
    return frame;
}

MensagemOut *parse_string_resposta(char *res_str)
{
    MensagemOut *res = (MensagemOut *)malloc(sizeof(MensagemOut));
    memcpy(res_str, &res, sizeof(res_str));
    return res;
}

char *message_tcp_ip_port(char *request, char *ip, int port)
{
    log_print("[COMM] message_tcp_ip_port()\n", LEVEL_DEBUG);
    printf("connecting to %s:%d\n", ip, port);
    fflush(NULL);
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char *buffer = (char *)malloc(MAX_FRAME_SIZE * sizeof(char));
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return NULL;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, ip, &serv_addr.sin_addr) <= 0)
    {
        printf("\nInvalid address/ Address not supported \n");
        return NULL;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        return NULL;
    }

    send(sock, request, strlen(request), 0);
    valread = read(sock, buffer, 1024);
    return buffer;
}

int listen_tcp_ip_port(char *(*get_response)(void *), char *ip, int port)
{
    log_print("[COMM] listen_tcp_ip_port\n", LEVEL_DEBUG);
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};

    // Create socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        log_print("[COMM] socket failed\n", LEVEL_ERROR);
        perror("socket failed");
        return -1;
    }
    log_print("[COMM] listen_tcp_ip_port\n", LEVEL_DEBUG);

    log_print("[COMM] socket opened\n", LEVEL_DEBUG);
    // Set socket options
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEADDR, &opt, sizeof(opt)))
    {
        log_print("[COMM] setsockopt failed\n", LEVEL_ERROR);
        perror("setsockopt failed");
        return -1;
    }
    log_print("[COMM] socket options set\n", LEVEL_DEBUG);

    // Bind socket to local address and port
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        log_print("[COMM] bind failed\n", LEVEL_ERROR);
        perror("bind failed");
        return -1;
    }
    log_print("[COMM] socket binded\n", LEVEL_DEBUG);

    // Listen for incoming connections
    if (listen(server_fd, 3) < 0)
    {
        log_print("[COMM] listen failed\n", LEVEL_ERROR);
        perror("listen failed");
        return -1;
    }
    log_print("[COMM] listen worked!\n", LEVEL_DEBUG);

    // Accept incoming connections and handle them
    while (1)
    {
        log_print("[COMM] waiting for connection\n", LEVEL_ERROR);
        printf("Waiting for connection...\n");
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
        {
            log_print("[COMM] accept failed\n", LEVEL_ERROR);
            perror("accept failed");
            return -1;
        }
        log_print("[COMM] got request!\n", LEVEL_DEBUG);

        // Receive data from client
        valread = read(new_socket, buffer, 1024);
        log_print("[COMM] getting response!\n", LEVEL_DEBUG);
        char *response = get_response(buffer);
        log_print("[COMM] got response!\n", LEVEL_DEBUG);

        // Send data to client
        send(new_socket, &response, strlen(response), 0);
        log_print("[COMM] response sent!\n", LEVEL_DEBUG);

        // Close the new socket
        close(new_socket);
    }
    return 0;
}
