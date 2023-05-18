#ifndef COMUNICACAO_H
#define COMUNICACAO_H 1

#include <proto.h>
#define MAX_FRAME_SIZE 10000

char *message_tcp_ip_port(char *request, char *ip, int port);
int listen_tcp_ip_port(char *(*get_response)(void *), char *ip, int port);

MensagemIn *monta_request(EstadoEstacionamento *e);
char *tranformar_request_em_string(MensagemIn *req);
MensagemOut *parse_string_resposta(char *res_str);

#endif