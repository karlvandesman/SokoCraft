#ifndef GERADOR_H_INCLUDED
#define GERADOR_H_INCLUDED

#include<stdio.h>
#include<stdlib.h>
#include <time.h>
#define N 13

typedef struct no
{
    struct no *next;
    int key1;
    int key2;
}no;

no *head=NULL, *aux=NULL;
volatile int anterior = 3, passos_caixas = 0, dificuldade_gerador= 3, cont_tunel = 0;
int quant_tunel, quant_caixas, Q, mapa[N][N], alvos_x[N], alvos_y[N], cela_x = N, cela_y = N, contador_g = 0;


void inserir(int key1, int key2);
void _remove();
void atualizaFila(int i, int j, int M[N][N]);
int valida_mapa();
int verificar(int a, int b);
int verifica_tunel(int x, int y);
void coloca_tunel(int *x, int *y, int D_ant, int D_prox);
void abrir_borda(int x, int y);
void baixo(int x, int y);
void cima(int x, int y);
void esquerda(int x, int y);
void direita(int x, int y);
void andar_gerador(int x, int y);
void coloca_cela(int x, int y, int lado);
int verifica_alvo(int x, int y, int k);
int gerar_mapa();


#endif // GERADOR_H_INCLUDED
