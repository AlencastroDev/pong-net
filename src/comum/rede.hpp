#pragma once
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>


int criar_socket_servidor(int porta);

int criar_socket_cliente(const char* ip, int porta, sockaddr_in* addr_servidor);

int enviar_para(int sock, const void* dados, int tamanho, const sockaddr_in* destino);

int receber_de(int sock, void* buffer, int tamanho, sockaddr_in* remetente);
