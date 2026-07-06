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

int criar_socket_cliente(const char* ip, int porta, sockaddr_in* addr_servidor){
    int sock = socket(AF_INET, SOCK_DGRAM, 0);

    memset(addr_servidor, 0, sizeof(*addr_servidor));
    addr_servidor->sin_family = AF_INET;
    addr_servidor->sin_port = htons(porta);
    inet_pton(AF_INET, ip, &addr_servidor->sin_addr);

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
