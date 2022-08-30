#ifndef CONSTS_HPP
#define CONSTS_HPP

#define TEMPO_ENTRE_REQUEST 50

#define ENDERECO 0x00
#define ENDERECO_ENVIO 0x01
#define CMD_SOLICITACAO 0x23
#define CMD_SOLICITA_TEMPERATURA_INTERNA 0xC1
#define CMD_SOLICITA_TEMPERATURA_REFERENCIA 0xC2
#define CMD_SOLICITA_COMANDO_USUARIO 0xC3
#define CMD_ENVIO 0x16
#define CMD_ENVIA_SINAL_CONTROLE 0xD1
#define CMD_ENVIA_SINAL_REFERENCIA 0xD2
#define CMD_ENVIA_ESTADO_SISTEMA 0xD3
#define CMD_ENVIA_ESTADO_FUNCIONAMENTO 0xD4
#define CMD_ENVIA_TEMPORIZADOR 0xD4

#define RESISTOR 4
#define VENTOINHA 5

#define MATRICULA_SIZE 4

extern const char MATRICULA[4];

extern bool run, turnOn, heating;
extern int timer;

#endif