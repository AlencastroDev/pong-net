#include "rede.hpp"
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>

using namespace std;

int criar_socket_servidor(int porta){
    int sock = socket(AF_INET, SOCK_DGRAM, 0);

    if(sock <= 0 ){
        return -1;
    }

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(porta);
    addr.sin_addr.s_addr = INADDR_ANY;

    bind(sock, (struct sockaddr*)&addr, sizeof(addr));
    fcntl(sock, F_SETFL, O_NONBLOCK);

    return sock;
}

int criar_socket_cliente(const char* host, int porta, sockaddr_in* addr_servidor){
    struct addrinfo hints, *res;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family   = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;

    char porta_str[8];
    snprintf(porta_str, sizeof(porta_str), "%d", porta);

    if (getaddrinfo(host, porta_str, &hints, &res) != 0)
        return -1;

    *addr_servidor = *(sockaddr_in*)res->ai_addr;
    freeaddrinfo(res);

    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    fcntl(sock, F_SETFL, O_NONBLOCK);
    return sock;
}

int enviar_para(int sock, const void* dados, int tamanho, const sockaddr_in* destino){
    return sendto(sock, dados, tamanho, 0, (struct sockaddr*)destino, sizeof(*destino));
}

int receber_de(int sock, void* buffer, int tamanho, sockaddr_in* remetente){
    socklen_t tamanho_remetente = sizeof(*remetente);
    return recvfrom(sock, buffer, tamanho, 0, (struct sockaddr*)remetente, &tamanho_remetente);
}
