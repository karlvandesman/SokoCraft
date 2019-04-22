#include "gerador.h"

void inserir(int key1, int key2)
{
    no *temp = (no *) malloc(sizeof(no));
    temp->key1 = key1;
    temp->key2 = key2;
    temp->next = NULL;
    if(!head)
    {
        head = aux = temp;
        return;
    }
    aux->next = temp;
    aux = temp;
}

void _remove()
{
    if(!head)
        return;
    no *temp = head;
    head = head->next;
    free(temp);
}

void atualizaFila(int i, int j, int M[N][N])  ///Vamos utilizar 9 para significar que o boneco já passou naquele ponto
{
    _remove();

    if(((i-1) >= 0) && (M[i-1][j] != 1) && (M[i-1][j] != 9) && (M[i-1][j] != 7))
        inserir(i-1,j);

    if(((i+1) < N) && (M[i+1][j] != 1) && (M[i+1][j] != 9) && (M[i+1][j] != 7))
        inserir(i+1,j);

    if(((j-1) >= 0) && (M[i][j-1] != 1) && (M[i][j-1] != 9) && (M[i][j-1] != 7))
        inserir(i,j-1);

    if(((j+1) < N) && (M[i][j+1] != 1) && (M[i][j+1] != 9) && (M[i][j+1] != 7))
        inserir(i,j+1);

}

///Retorna 1 caso o mapa seja válido (boneco alcança todos os alvos) e 0 caso contrário
int valida_mapa()
{
    int acessiveis = 0, x, y, M[N][N], i, j, cont=0;

    if(contador_g >= 5000)
        return 0;

    for(i=0;i<N;i++)
    {
        for(j=0;j<N;j++)
        {
            M[i][j] = mapa[i][j];
            if((mapa[i][j] == 4) || (mapa[i][j] == 6))
                cont++;
        }
    }

    do
    {
        x = rand()%(N-2) + 1;
        y = rand()%(N-2) + 1;
    }while(M[x][y] != 0);

    M[x][y] = 9; //Coloca o "boneco" em algum ponto vazio
    inserir(x, y);  //Põe o primeiro nó da fila

    //Atualiza a fila, calcula a quantidade de alvos acessíveis e marca onde passou
    while(head)
    {
        atualizaFila(head->key1, head->key2, M);
        if((head) && ((M[head->key1][head->key2] == 2) || (M[head->key1][head->key2] == 6)))
            acessiveis++;
        if(head)
            M[head->key1][head->key2] = 9;
    }

    if(dificuldade_gerador == 3)
        quant_caixas += 3;

    ///Se conseguir chegar em todas as caixas o mapa é válido :D
    if((acessiveis == quant_caixas) && (cont == quant_caixas))
        return 1;
    else
        return 0;
}

int verificar(int a, int b)
{
    if((a<0) || (b<0) || (a>=N) || (b>=N))
        return 0;
    else
        return 1;
}

///FUNÇÃO PARA VERIFICAR SE O CAMINHO DO TÚNEL ESTÁ LIVRE
int verifica_tunel(int x, int y) //Manda a coordanada do candidato a muro pétreo
{
    if((mapa[x][y] == 1)&&(mapa[x][y-1] == 0 || mapa[x][y-1] == 1 || mapa[x][y-1] == 2) && (mapa[x][y+1] == 0 || mapa[x][y+1] == 1 || mapa[x][y+1] == 2) &&
       (mapa[x+1][y-1] == 0 || mapa[x+1][y-1] == 1 || mapa[x+1][y-1] == 2) && (mapa[x+1][y] == 0 || mapa[x+1][y] == 1 || mapa[x+1][y] == 2 ) &&
       (mapa[x+1][y+1] == 0 || mapa[x+1][y+1] == 1 || mapa[x+1][y+1] == 2) && (mapa[x-1][y-1] == 0 || mapa[x-1][y-1] == 1 || mapa[x-1][y-1] == 2) &&
       (mapa[x-1][y] == 0 || mapa[x-1][y] == 1 || mapa[x-1][y] == 2) && (mapa[x-1][y+1] == 0 || mapa[x-1][y+1] == 1 || mapa[x-1][y+1] == 2))
        return 1;
    else
        return 0;
}

///FUNÇÃO PARA COLOCAR OS TÚNEIS
void coloca_tunel(int *x, int *y, int D_ant, int D_prox)  //Considerar cada túnel como 3 passos
{
    if((D_ant == 0) && (D_prox == 2)) //Anterior para baixo e agora para esquerda
    {
        if((verificar((*x), (*y)+2)) && (verificar((*x)-3, (*y))))
        {
            if(verifica_tunel((*x)-1, (*y)+1) == 0)
                return;

            mapa[(*x)-1][(*y)+1] = 7;  //Coloca um muro pétreo
            if(mapa[(*x)][(*y)+2] == 1)
                mapa[(*x)][(*y)+2] = 0;
            if(mapa[(*x)-1][(*y)+2] == 1)
                mapa[(*x)-1][(*y)+2] = 0;
            if(mapa[(*x)-2][(*y)+2] == 1)
                mapa[(*x)-2][(*y)+2] = 0;
            if(mapa[(*x)-2][(*y)+1] == 1)
                mapa[(*x)-2][(*y)+1] = 0;
            if(mapa[(*x)-2][(*y)] == 1)
                mapa[(*x)-2][(*y)] = 0;
            if(mapa[(*x)-1][(*y)] == 1)
                mapa[(*x)-1][(*y)] = 0;
            if(mapa[(*x)-3][(*y)] == 1)
                mapa[(*x)-3][(*y)] = 0;
            passos_caixas+=3;
            cont_tunel++;
            (*x) = (*x) - 2;
            return;
        }
        else
            return;
    }
    if((D_ant == 0) && (D_prox == 3)) //Anterior para baixo e agora para direita
    {
        if((verificar((*x), (*y)+2)) && (verificar((*x)+3, (*y))))
        {
            if(verifica_tunel((*x)+1, (*y)+1) == 0)
                return;

            mapa[(*x)+1][(*y)+1] = 7;  //Coloca um muro pétreo
            if(mapa[(*x)][(*y)+2] == 1)
                mapa[(*x)][(*y)+2] = 0;
            if(mapa[(*x)+1][(*y)+2] == 1)
                mapa[(*x)+1][(*y)+2] = 0;
            if(mapa[(*x)+2][(*y)+2] == 1)
                mapa[(*x)+2][(*y)+2] = 0;
            if(mapa[(*x)+2][(*y)+1] == 1)
                mapa[(*x)+2][(*y)+1] = 0;
            if(mapa[(*x)+2][(*y)] == 1)
                mapa[(*x)+2][(*y)] = 0;
            if(mapa[(*x)+1][(*y)] == 1)
                mapa[(*x)+1][(*y)] = 0;
            if(mapa[(*x)+3][(*y)] == 1)
                mapa[(*x)+3][(*y)] = 0;
            passos_caixas+=3;
            cont_tunel++;
            (*x) = (*x) + 2;
            return;
        }
        else
            return;
    }
    if((D_ant == 1) && (D_prox == 2)) //Anterior para cima e agora para esquerda
    {
        if((verificar((*x), (*y)-2)) && (verificar((*x)-3, (*y))))
        {
            if(verifica_tunel((*x)-1, (*y)-1) == 0)
                return;

            mapa[(*x)-1][(*y)-1] = 7;  //Coloca um muro pétreo
            if(mapa[(*x)][(*y)-2] == 1)
                mapa[(*x)][(*y)-2] = 0;
            if(mapa[(*x)-1][(*y)-2] == 1)
                mapa[(*x)-1][(*y)-2] = 0;
            if(mapa[(*x)-2][(*y)-2] == 1)
                mapa[(*x)-2][(*y)-2] = 0;
            if(mapa[(*x)-2][(*y)-1] == 1)
                mapa[(*x)-2][(*y)-1] = 0;
            if(mapa[(*x)-2][(*y)] == 1)
                mapa[(*x)-2][(*y)] = 0;
            if(mapa[(*x)-1][(*y)] == 1)
                mapa[(*x)-1][(*y)] = 0;
            if(mapa[(*x)-3][(*y)] == 1)
                mapa[(*x)-3][(*y)] = 0;
            passos_caixas+=3;
            cont_tunel++;
            (*x) = (*x) - 2;
            return;
        }
        else
            return;
    }
    if((D_ant == 1) && (D_prox == 3)) //Anterior para cima e agora para direita
    {
        if((verificar((*x), (*y)-2)) && (verificar((*x)+3, (*y))))
        {
            if(verifica_tunel((*x)+1, (*y)-1) == 0)
                return;

            mapa[(*x)+1][(*y)-1] = 7;  //Coloca um muro pétreo
            if(mapa[(*x)][(*y)-2] == 1)
                mapa[(*x)][(*y)-2] = 0;
            if(mapa[(*x)+1][(*y)-2] == 1)
                mapa[(*x)+1][(*y)-2] = 0;
            if(mapa[(*x)+2][(*y)-2] == 1)
                mapa[(*x)+2][(*y)-2] = 0;
            if(mapa[(*x)+2][(*y)-1] == 1)
                mapa[(*x)+2][(*y)-1] = 0;
            if(mapa[(*x)+2][(*y)] == 1)
                mapa[(*x)+2][(*y)] = 0;
            if(mapa[(*x)+1][(*y)] == 1)
                mapa[(*x)+1][(*y)] = 0;
            if(mapa[(*x)+3][(*y)] == 1)
                mapa[(*x)+3][(*y)] = 0;
            passos_caixas+=3;
            cont_tunel++;
            (*x) = (*x) + 2;
            return;
        }
        else
            return;
    }
    if((D_ant == 2) && (D_prox == 0)) //Anterior para esquerda e agora para baixo
    {
        if((verificar((*x), (*y)+3)) && (verificar((*x)-2, (*y))))
        {
            if(verifica_tunel((*x)-1, (*y)+1) == 0)
                return;

            mapa[(*x)-1][(*y)+1] = 7;  //Coloca um muro pétreo
            if(mapa[(*x)][(*y)-2] == 1)
                mapa[(*x)][(*y)-2] = 0;
            if(mapa[(*x)-1][(*y)-2] == 1)
                mapa[(*x)-1][(*y)-2] = 0;
            if(mapa[(*x)-2][(*y)-2] == 1)
                mapa[(*x)-2][(*y)-2] = 0;
            if(mapa[(*x)-2][(*y)-1] == 1)
                mapa[(*x)-2][(*y)-1] = 0;
            if(mapa[(*x)-2][(*y)] == 1)
                mapa[(*x)-2][(*y)] = 0;
            if(mapa[(*x)-1][(*y)] == 1)
                mapa[(*x)-1][(*y)] = 0;
            if(mapa[(*x)-3][(*y)] == 1)
                mapa[(*x)-3][(*y)] = 0;
            passos_caixas+=3;
            cont_tunel++;
            (*y) = (*y) + 2;
            return;
        }
        else
            return;
    }
    if((D_ant == 2) && (D_prox == 1)) //Anterior para esquerda e agora para cima
    {
        if((verificar((*x), (*y)-3)) && (verificar((*x)-2, (*y))))
        {
            if(verifica_tunel((*x)-1, (*y)-1) == 0)
                return;

            mapa[(*x)-1][(*y)-1] = 7;  //Coloca um muro pétreo
            if(mapa[(*x)-2][(*y)] == 1)
                mapa[(*x)-2][(*y)] = 0;
            if(mapa[(*x)-2][(*y)-1] == 1)
                mapa[(*x)-2][(*y)-1] = 0;
            if(mapa[(*x)-2][(*y)-2] == 1)
                mapa[(*x)-2][(*y)-2] = 0;
            if(mapa[(*x)-1][(*y)-2] == 1)
                mapa[(*x)-1][(*y)-2] = 0;
            if(mapa[(*x)][(*y)-2] == 1)
                mapa[(*x)][(*y)-2] = 0;
            if(mapa[(*x)][(*y)-1] == 1)
                mapa[(*x)][(*y)-1] = 0;
            if(mapa[(*x)][(*y)-3] == 1)
                mapa[(*x)][(*y)-3] = 0;
            passos_caixas+=3;
            cont_tunel++;
            (*y) = (*y) - 2;
            return;
        }
        else
            return;
    }
    if((D_ant == 3) && (D_prox == 0)) //Anterior para direita e agora para baixo
    {
        if((verificar((*x), (*y)+3)) && (verificar((*x)+2, (*y))))
        {
            if(verifica_tunel((*x)+1, (*y)+1) == 0)
                return;

            mapa[(*x)+1][(*y)+1] = 7;  //Coloca um muro pétreo
            if(mapa[(*x)+2][(*y)] == 1)
                mapa[(*x)+2][(*y)] = 0;
            if(mapa[(*x)+2][(*y)+1] == 1)
                mapa[(*x)+2][(*y)+1] = 0;
            if(mapa[(*x)+2][(*y)+2] == 1)
                mapa[(*x)+2][(*y)+2] = 0;
            if(mapa[(*x)+1][(*y)+2] == 1)
                mapa[(*x)+1][(*y)+2] = 0;
            if(mapa[(*x)][(*y)+2] == 1)
                mapa[(*x)][(*y)+2] = 0;
            if(mapa[(*x)][(*y)+1] == 1)
                mapa[(*x)][(*y)+1] = 0;
            if(mapa[(*x)][(*y)+3] == 1)
                mapa[(*x)][(*y)+3] = 0;
            passos_caixas+=3;
            cont_tunel++;
            (*y) = (*y) + 2;
            return;
        }
        else
            return;
    }
    if((D_ant == 3) && (D_prox == 1)) //Anterior para direita e agora para cima
    {
        if((verificar((*x), (*y)-3)) && (verificar((*x)+2, (*y))))
        {
            if(verifica_tunel((*x)+1, (*y)-1) == 0)
                return;

            mapa[(*x)+1][(*y)-1] = 7;  //Coloca um muro pétreo
            if(mapa[(*x)+2][(*y)] == 1)
                mapa[(*x)+2][(*y)] = 0;
            if(mapa[(*x)+2][(*y)-1] == 1)
                mapa[(*x)+2][(*y)-1] = 0;
            if(mapa[(*x)+2][(*y)-2] == 1)
                mapa[(*x)+2][(*y)-2] = 0;
            if(mapa[(*x)+1][(*y)-2] == 1)
                mapa[(*x)+1][(*y)-2] = 0;
            if(mapa[(*x)][(*y)-2] == 1)
                mapa[(*x)][(*y)-2] = 0;
            if(mapa[(*x)][(*y)-1] == 1)
                mapa[(*x)][(*y)-1] = 0;
            if(mapa[(*x)][(*y)-3] == 1)
                mapa[(*x)][(*y)-3] = 0;
            passos_caixas+=3;
            cont_tunel++;
            (*y) = (*y) - 2;
            return;
        }
        else
            return;
    }
}

void abrir_borda(int x, int y)  //POSIÇÃO DA CAIXA
{
    if((anterior == 0) || (anterior == 1))
    {
        if((verificar(x+1, y+1)) && (verificar(x+1, y-1)))
        {
            if(mapa[x+1][y] == 1)
                mapa[x+1][y] = 0;
            if(mapa[x+1][y+1] == 1)
                mapa[x+1][y+1] = 0;
            if(mapa[x+1][y-1] == 1)
                mapa[x+1][y-1] = 0;
        }
        if((verificar(x-1, y-1)) && (verificar(x-1, y+1)))
        {
            if(mapa[x-1][y] == 1)
                mapa[x-1][y] = 0;
            if(mapa[x-1][y+1] == 1)
                mapa[x-1][y+1] = 0;
            if(mapa[x-1][y-1] == 1)
                mapa[x-1][y-1] = 0;
            return;
        }
    }
    if((anterior == 2) || (anterior == 3))
    {
        if((verificar(x-1, y+1)) && (verificar(x+1,y+1)))
        {
            if(mapa[x][y+1] == 1)
                mapa[x][y+1] = 0;
            if(mapa[x+1][y+1] == 1)
                mapa[x+1][y+1] = 0;
            if(mapa[x-1][y+1] == 1)
                mapa[x-1][y+1] = 0;
        }
        if((verificar(x-1, y-1)) && (verificar(x+1, y-1)))
        {
            if(mapa[x][y-1] == 1)
                mapa[x][y-1] = 0;
            if(mapa[x+1][y-1] == 1)
                mapa[x+1][y-1] = 0;
            if(mapa[x-1][y-1] == 1)
                mapa[x-1][y-1] = 0;
            return;
        }
    }
}

void baixo(int x, int y) //direção 0
{
    contador_g++;
    if((verificar(x, y+2)) && (passos_caixas < Q) && (mapa[x][y+2] != 7) && (mapa[x][y+1] != 7)) //Verifica se é possível andar_gerador primeiro passo para baixo
    {
        if(anterior == 0) ///anterior pra baixo e agora pra baixo
        {
                if(mapa[x][y+1] == 1)
                    mapa[x][y+1] = 0;
                if(mapa[x][y+3] == 1)
                    mapa[x][y+3] = 0;
                passos_caixas++;
                anterior = 0;
                if(passos_caixas == Q)
                {
                    if(mapa[x][y+1] == 2)
                        mapa[x][y+1] = 6;  //COLOCAR A CAIXA E ALVO NO MESMO LUGAR
                    else
                        mapa[x][y+1] = 4;  //COLOCAR A CAIXA

                    abrir_borda(x,y+1);  //MANDA A POSIÇÃO DA CAIXA
                }
                if((verificar(x, y+3)) && (passos_caixas < Q) && (mapa[x][y+3] != 7)&&(mapa[x][y+2] != 7)) //VERIFICA SE PODE DAR O SEGUNDO PASSO PARA BAIXO
                {
                    if(mapa[x][y+2] == 1)
                        mapa[x][y+2] = 0;
                    if(mapa[x][y+3] == 1)
                        mapa[x][y+3] = 0;
                    anterior = 0;
                    passos_caixas++;
                    if(passos_caixas == Q)
                        {
                            if(mapa[x][y+2] == 2)
                                mapa[x][y+2] = 6;  //COLOCAR A CAIxA E ALVO NO MESMO LUGAR
                            else
                                mapa[x][y+2] = 4;  //COLOCAR A CAIxA

                            abrir_borda(x,y+2);  //MANDA A POSIÇÃO DA CAIXA
                        }
                    andar_gerador(x, y+2); //MANDA A POSIÇÃO ATUAL DA CAIxA
                    return;
                }
                andar_gerador(x, y+1);  //MANDA A POSIÇÃO ATUAL DA CAIxA
                return;
        }
        else if(anterior == 1) ///anterior pra cima e agora pra baixo
        {
            andar_gerador(x,y);
            return;
        }
        else if(anterior == 2) ///anterior pra esquerda e agora pra baixo
        {
                if((cont_tunel < quant_tunel) && (dificuldade_gerador == 2 || dificuldade_gerador == 3))
                    {
                        coloca_tunel(&x, &y, 2, 0);  //Manda a posição da caixa
                    }
                if(mapa[x-1][y] == 1)
                    mapa[x-1][y] = 0;
                if(mapa[x-1][y+1] == 1)
                    mapa[x-1][y+1] = 0;
                if(mapa[x][y+1] == 1)
                    mapa[x][y+1] = 0;
                if(mapa[x][y+2] == 1)
                    mapa[x][y+2] = 0;
                passos_caixas++;
                anterior = 0;
                if(passos_caixas == Q)
                {
                    if(mapa[x][y+1] == 2)
                        mapa[x][y+1] = 6;  //COLOCAR A CAIxA E ALVO NO MESMO LUGAR
                    else
                        mapa[x][y+1] = 4;  //COLOCAR A CAIxA

                    abrir_borda(x,y+1);  //MANDA A POSIÇÃO DA CAIXA
                }
                if((verificar(x, y+3)) && (passos_caixas < Q) && (mapa[x][y+3] != 7)&&(mapa[x][y+2] != 7)) //VERIFICA SE PODE DAR O SEGUNDO PASSO PARA BAIXO
                {
                    if(mapa[x][y+2] == 1)
                        mapa[x][y+2] = 0;
                    if(mapa[x][y+3] == 1)
                        mapa[x][y+3] = 0;
                    anterior = 0;
                    passos_caixas++;
                    if(passos_caixas == Q)
                        {
                            if(mapa[x][y+2] == 2)
                                mapa[x][y+2] = 6;  //COLOCAR A CAIxA E ALVO NO MESMO LUGAR
                            else
                                mapa[x][y+2] = 4;  //COLOCAR A CAIxA

                            abrir_borda(x,y+2);  //MANDA A POSIÇÃO DA CAIXA
                        }
                    andar_gerador(x, y+2);
                    return;
                }
                andar_gerador(x, y+1);
                return;
        }
        else if(anterior == 3) ///anterior pra direita e agora pra baixo
        {
                if((cont_tunel < quant_tunel) && (dificuldade_gerador == 2 || dificuldade_gerador == 3))
                    {
                        coloca_tunel(&x, &y, 3, 0);  //Manda a posição da caixa
                    }
                if(mapa[x+1][y] == 1)
                    mapa[x+1][y] = 0;
                if(mapa[x+1][y+1] == 1)
                    mapa[x+1][y+1] = 0;
                if(mapa[x][y+1] == 1)
                    mapa[x][y+1] = 0;
                if(mapa[x][y+2] == 1)
                    mapa[x][y+2] = 0;
                passos_caixas++;
                anterior = 0;
                if(passos_caixas == Q)
                        {
                            if(mapa[x][y+1] == 2)
                                mapa[x][y+1] = 6;  //COLOCAR A CAIxA E ALVO NO MESMO LUGAR
                            else
                                mapa[x][y+1] = 4;  //COLOCAR A CAIxA

                            abrir_borda(x,y+1);  //MANDA A POSIÇÃO DA CAIXA
                        }
                if((verificar(x, y+3)) && (passos_caixas < Q) && (mapa[x][y+3] != 7)&&(mapa[x][y+2] != 7)) //VERIFICA SE PODE DAR O SEGUNDO PASSO PARA BAIxO
                {
                    if(mapa[x][y+2] == 1)
                        mapa[x][y+2] = 0;  //abrir caminho onde estava o boneco
                    if(mapa[x][y+3] == 1)
                        mapa[x][y+3] = 0;  //boneco andar_gerador para baixo (puxando a caixa)
                    anterior = 0;
                    passos_caixas++;
                    if(passos_caixas == Q)
                        {
                            if(mapa[x][y+2] == 2)
                                mapa[x][y+2] = 6;  //COLOCAR A CAIxA E ALVO NO MESMO LUGAR
                            else
                                mapa[x][y+2] = 4;  //COLOCAR A CAIxA

                            abrir_borda(x,y+2);  //MANDA A POSIÇÃO DA CAIXA
                        }
                    andar_gerador(x, y+2);
                    return;
                }
                andar_gerador(x, y+1);
                return;
        }
    }
    if(passos_caixas < Q)
    {
        andar_gerador(x,y);
        return;
    }
}

void cima(int x, int y) //direção 1
{
    contador_g++;
    if((verificar(x, y-2)) && (passos_caixas < Q) && (mapa[x][y-2] != 7)&&(mapa[x][y-1] != 7)) //Verifica se é possível andar_gerador primeiro passo para cima
    {
        if(anterior == 0) ///anterior pra baixo e agora pra cima
        {
            andar_gerador(x,y);
            return;
        }
        else if(anterior == 1) ///anterior pra cima e agora pra cima
        {
                if(mapa[x][y-1] == 1)
                    mapa[x][y-1] = 0;
                if(mapa[x][y-2] == 1)
                    mapa[x][y-2] = 0;
                passos_caixas++;
                anterior = 1;
                if(passos_caixas == Q)
                        {
                            if(mapa[x][y-1] == 2)
                                mapa[x][y-1] = 6;  //COLOCAR A CAIxA E ALVO NO MESMO LUGAR
                            else
                                mapa[x][y-1] = 4;  //COLOCAR A CAIxA

                            abrir_borda(x,y-1);  //MANDA A POSIÇÃO DA CAIXA
                        }
                if((verificar(x, y-3)) && (passos_caixas < Q) && (mapa[x][y-3] != 7)&&(mapa[x][y-2] != 7)) //VERIFICA SE PODE DAR O SEGUNDO PASSO PARA CIMA
                {
                    if(mapa[x][y-2] == 1)
                        mapa[x][y-2] = 0;
                    if(mapa[x][y-3] == 1)
                        mapa[x][y-3] = 0;
                    anterior = 1;
                    passos_caixas++;
                    if(passos_caixas == Q)
                        {
                            if(mapa[x][y-2] == 2)
                                mapa[x][y-2] = 6;  //COLOCAR A CAIxA E ALVO NO MESMO LUGAR
                            else
                                mapa[x][y-2] = 4;  //COLOCAR A CAIxA

                            abrir_borda(x,y-2);  //MANDA A POSIÇÃO DA CAIXA
                        }
                    andar_gerador(x, y-2); //MANDA A POSIÇÃO ATUAL DA CAIxA
                    return;
                }
                andar_gerador(x, y-1);  //MANDA A POSIÇÃO ATUAL DA CAIxA
                return;
        }
        else if(anterior == 2) ///anterior pra esquerda e agora pra cima
        {
                if((cont_tunel < quant_tunel) && (dificuldade_gerador == 2 || dificuldade_gerador == 3))
                    {
                        coloca_tunel(&x, &y, 2, 1);  //Manda a posição da caixa
                    }
                if(mapa[x-1][y] == 1)
                    mapa[x-1][y] = 0;
                if(mapa[x-1][y-1] == 1)
                    mapa[x-1][y-1] = 0;
                if(mapa[x][y-1] == 1)
                    mapa[x][y-1] = 0;
                if(mapa[x][y-2] == 1)
                    mapa[x][y-2] = 0;
                passos_caixas++;
                anterior = 1;
                if(passos_caixas == Q)
                        {
                            if(mapa[x][y-1] == 2)
                                mapa[x][y-1] = 6;  //COLOCAR A CAIxA E ALVO NO MESMO LUGAR
                            else
                                mapa[x][y-1] = 4;  //COLOCAR A CAIxA

                            abrir_borda(x,y-1);  //MANDA A POSIÇÃO DA CAIXA
                        }
                if((verificar(x, y-3)) && (passos_caixas < Q) && (mapa[x][y-3] != 7)&&(mapa[x][y-2] != 7)) //VERIFICA SE PODE DAR O SEGUNDO PASSO PARA CIMA
                {
                    if(mapa[x][y-2] == 1)
                        mapa[x][y-2] = 0;
                    if(mapa[x][y-3] == 1)
                        mapa[x][y-3] = 0;
                    anterior = 1;
                    passos_caixas++;
                    if(passos_caixas == Q)
                        {
                            if(mapa[x][y-2] == 2)
                                mapa[x][y-2] = 6;  //COLOCAR A CAIxA E ALVO NO MESMO LUGAR
                            else
                                mapa[x][y-2] = 4;  //COLOCAR A CAIxA

                            abrir_borda(x,y-2);  //MANDA A POSIÇÃO DA CAIXA
                        }
                    andar_gerador(x, y-2);
                    return;
                }
                andar_gerador(x, y-1);
                return;
        }
        else if(anterior == 3) ///anterior pra direita e agora pra cima
        {
                if((cont_tunel < quant_tunel) && (dificuldade_gerador == 2 || dificuldade_gerador == 3))
                    {
                        coloca_tunel(&x, &y, 3, 1);  //Manda a posição da caixa
                    }
                if(mapa[x+1][y] == 1)
                    mapa[x+1][y] = 0;
                if(mapa[x+1][y-1] == 1)
                    mapa[x+1][y-1] = 0;
                if(mapa[x][y-1] == 1)
                    mapa[x][y-1] = 0;
                if(mapa[x][y-2] == 1)
                    mapa[x][y-2] = 0;
                passos_caixas++;
                anterior = 1;
                if(passos_caixas == Q)
                        {
                            if(mapa[x][y-1] == 2)
                                mapa[x][y-1] = 6;  //COLOCAR A CAIxA E ALVO NO MESMO LUGAR
                            else
                                mapa[x][y-1] = 4;  //COLOCAR A CAIxA

                            abrir_borda(x,y-1);  //MANDA A POSIÇÃO DA CAIXA
                        }
                if((verificar(x, y-3)) && (passos_caixas < Q) && (mapa[x][y-3] != 7)&&(mapa[x][y-2] != 7)) //VERIFICA SE PODE DAR O SEGUNDO PASSO PARA CIMA
                {
                    if(mapa[x][y-2] == 1)
                        mapa[x][y-2] = 0;  //abrir caminho onde estava o boneco
                    if(mapa[x][y-3] == 1)
                        mapa[x][y-3] = 0;  //espaço pro boneco andar_gerador para esquerda (puxando a caixa)
                    anterior = 1;
                    passos_caixas++;
                    if(passos_caixas == Q)
                        {
                            if(mapa[x][y-2] == 2)
                                mapa[x][y-2] = 6;  //COLOCAR A CAIxA E ALVO NO MESMO LUGAR
                            else
                                mapa[x][y-2] = 4;  //COLOCAR A CAIxA

                            abrir_borda(x,y-2);  //MANDA A POSIÇÃO DA CAIXA
                        }
                    andar_gerador(x, y-2);
                    return;
                }
                anterior = 1;
                andar_gerador(x, y-1);
                return;
        }
    }
    if(passos_caixas < Q)
        {
            andar_gerador(x,y);
            return;
        }
}

void esquerda(int x, int y) //direção 2
{
    contador_g++;
    if((verificar(x-2, y)) && (passos_caixas < Q) && (mapa[x-2][y] != 7)&&(mapa[x-1][y] != 7)) //Verifica se é possível andar_gerador primeiro passo para esquerda
    {
        if(anterior == 0) ///anterior pra baixo e agora pra esquerda
        {
                if((cont_tunel < quant_tunel) && (dificuldade_gerador == 2 || dificuldade_gerador == 3))
                    {
                        coloca_tunel(&x, &y, 0, 2);  //Manda a posição da caixa
                    }
                if(mapa[x][y+1] == 1)
                    mapa[x][y+1] = 0;
                if(mapa[x-1][y+1] == 1)
                    mapa[x-1][y+1] = 0;
                if(mapa[x-1][y] == 1)
                    mapa[x-1][y] = 0;
                if(mapa[x-2][y] == 1)
                    mapa[x-2][y] = 0;
                passos_caixas++;
                anterior = 2;
                if(passos_caixas == Q)
                        {
                            if(mapa[x-1][y] == 2)
                                mapa[x-1][y] = 6;  //COLOCAR A CAIxA E ALVO NO MESMO LUGAR
                            else
                                mapa[x-1][y] = 4;  //COLOCAR A CAIxA

                            abrir_borda(x-1,y);  //MANDA A POSIÇÃO DA CAIXA
                        }
                if((verificar(x-3, y)) && (passos_caixas < Q) && (mapa[x-3][y] != 7)&&(mapa[x-2][y] != 7)) //VERIFICA SE PODE DAR O SEGUNDO PASSO PARA ESQUERDA
                {
                    if(mapa[x-2][y] == 1)
                        mapa[x-2][y] = 0;
                    if(mapa[x-3][y] == 1)
                        mapa[x-3][y] = 0;
                    anterior = 2;
                    passos_caixas++;
                    if(passos_caixas == Q)
                        {
                            if(mapa[x-2][y] == 2)
                                mapa[x-2][y] = 6;  //COLOCAR A CAIxA E ALVO NO MESMO LUGAR
                            else
                                mapa[x-2][y] = 4;  //COLOCAR A CAIxA

                            abrir_borda(x-2,y);  //MANDA A POSIÇÃO DA CAIXA
                        }
                    andar_gerador(x-2, y);
                    return;
                }
                andar_gerador(x-1, y);
                return;
        }
        else if(anterior == 1) ///anterior pra cima e agora pra esquerda
        {
                if((cont_tunel < quant_tunel) && (dificuldade_gerador == 2 || dificuldade_gerador == 3))
                    {
                        coloca_tunel(&x, &y, 1, 2);  //Manda a posição da caixa
                    }
                if(mapa[x][y-1] == 1)
                    mapa[x][y-1] = 0;
                if(mapa[x-1][y-1] == 1)
                    mapa[x-1][y-1] = 0;
                if(mapa[x-1][y] == 1)
                    mapa[x-1][y] = 0;
                if(mapa[x-2][y] == 1)
                    mapa[x-2][y] = 0;
                passos_caixas++;
                anterior = 2;
                if(passos_caixas == Q)
                        {
                            if(mapa[x-1][y] == 2)
                                mapa[x-1][y] = 6;  //COLOCAR A CAIxA E ALVO NO MESMO LUGAR
                            else
                                mapa[x-1][y] = 4;  //COLOCAR A CAIxA

                            abrir_borda(x-1,y);  //MANDA A POSIÇÃO DA CAIXA
                        }
                if((verificar(x-3, y)) && (passos_caixas < Q) && (mapa[x-3][y] != 7)&&(mapa[x-2][y] != 7)) //VERIFICA SE PODE DAR O SEGUNDO PASSO PARA A ESQUERDA
                {
                    if(mapa[x-2][y] == 1)
                        mapa[x-2][y] = 0;
                    if(mapa[x-3][y] == 1)
                        mapa[x-3][y] = 0;
                    anterior = 2;
                    passos_caixas++;
                    if(passos_caixas == Q)
                        {
                            if(mapa[x-2][y] == 2)
                                mapa[x-2][y] = 6;  //COLOCAR A CAIxA E ALVO NO MESMO LUGAR
                            else
                                mapa[x-2][y] = 4;  //COLOCAR A CAIxA

                            abrir_borda(x-2,y);  //MANDA A POSIÇÃO DA CAIXA
                        }
                    andar_gerador(x-2, y);
                    return;
                }
                andar_gerador(x-1, y);
                return;
        }
        else if(anterior == 2) ///anterior pra esquerda e agora pra esquerda
        {
                if(mapa[x-1][y] == 1)
                    mapa[x-1][y] = 0;
                if(mapa[x-2][y] == 1)
                    mapa[x-2][y] = 0;
                passos_caixas++;
                anterior = 2;
                if(passos_caixas == Q)
                        {
                            if(mapa[x-1][y] == 2)
                                mapa[x-1][y] = 6;  //COLOCAR A CAIxA E ALVO NO MESMO LUGAR
                            else
                                mapa[x-1][y] = 4;  //COLOCAR A CAIxA

                            abrir_borda(x-1,y);  //MANDA A POSIÇÃO DA CAIXA
                        }
                if((verificar(x-3, y)) && (passos_caixas < Q) && (mapa[x-3][y] != 7)&&(mapa[x-2][y] != 7)) //VERIFICA SE PODE DAR O SEGUNDO PASSO PARA A ESQUERDA
                {
                    if(mapa[x-2][y] == 1)
                        mapa[x-2][y] = 0;
                    if(mapa[x-3][y] == 1)
                        mapa[x-3][y] = 0;
                    anterior = 2;
                    passos_caixas++;
                    if(passos_caixas == Q)
                        {
                            if(mapa[x-2][y] == 2)
                                mapa[x-2][y] = 6;  //COLOCAR A CAIxA E ALVO NO MESMO LUGAR
                            else
                                mapa[x-2][y] = 4;  //COLOCAR A CAIxA

                            abrir_borda(x-2,y);  //MANDA A POSIÇÃO DA CAIXA
                        }
                    andar_gerador(x-2, y);
                    return;
                }
                andar_gerador(x-1, y);
                return;
        }
        else if(anterior == 3) ///anterior pra direita e agora pra esquerda
        {
            andar_gerador(x,y);
            return;
        }
    }
    if(passos_caixas < Q)
    {
        andar_gerador(x, y);
        return;
    }
}

void direita(int x, int y) //direção 3
{
    contador_g++;
    if((verificar(x+2, y)) && (passos_caixas < Q) && (mapa[x+2][y] != 7)&&(mapa[x+1][y] != 7)) //Verifica se é possível andar_gerador primeiro passo para direita
    {
        if(anterior == 0) ///anterior pra baixo e agora pra direita
        {
                if((cont_tunel < quant_tunel) && (dificuldade_gerador == 2 || dificuldade_gerador == 3))
                    {
                        coloca_tunel(&x, &y, 0, 3);  //Manda a posição da caixa
                    }
                if(mapa[x][y+1] == 1)
                    mapa[x][y+1] = 0;
                if(mapa[x+1][y+1] == 1)
                    mapa[x+1][y+1] = 0;
                if(mapa[x+1][y] == 1)
                    mapa[x+1][y] = 0;
                if(mapa[x+2][y] == 1)
                    mapa[x+2][y] = 0;
                passos_caixas++;
                anterior = 3;
                if(passos_caixas == Q)
                        {
                            if(mapa[x+1][y] == 2)
                                mapa[x+1][y] = 6;  //COLOCAR A CAIxA E ALVO NO MESMO LUGAR
                            else
                                mapa[x+1][y] = 4;  //COLOCAR A CAIxA

                            abrir_borda(x+1,y);  //MANDA A POSIÇÃO DA CAIXA
                        }
                if((verificar(x+3, y)) && (passos_caixas < Q) && (mapa[x+3][y] != 7)&&(mapa[x+2][y] != 7)) //VERIFICA SE PODE DAR O SEGUNDO PASSO PARA DIREITA
                {
                    if(mapa[x+2][y] == 1)
                        mapa[x+2][y] = 0;
                    if(mapa[x+3][y] == 1)
                        mapa[x+3][y] = 0;
                    anterior = 3;
                    passos_caixas++;
                    if(passos_caixas == Q)
                        {
                            if(mapa[x+2][y] == 2)
                                mapa[x+2][y] = 6;  //COLOCAR A CAIxA E ALVO NO MESMO LUGAR
                            else
                                mapa[x+2][y] = 4;  //COLOCAR A CAIxA

                            abrir_borda(x+2,y);  //MANDA A POSIÇÃO DA CAIXA
                        }
                    andar_gerador(x+2, y);
                    return;
                }
                andar_gerador(x+1, y);
                return;
        }
        else if(anterior == 1) ///anterior pra cima e agora direita
        {
                if((cont_tunel < quant_tunel) && (dificuldade_gerador == 2 || dificuldade_gerador == 3))
                    {
                        coloca_tunel(&x, &y, 1, 3);  //Manda a posição da caixa
                    }
                if(mapa[x][y-1] == 1)
                    mapa[x][y-1] = 0;
                if(mapa[x+1][y-1] == 1)
                    mapa[x+1][y-1] = 0;
                if(mapa[x+1][y] == 1)
                    mapa[x+1][y] = 0;
                if(mapa[x+2][y] == 1)
                    mapa[x+2][y] = 0;
                passos_caixas++;
                anterior = 3;
                if(passos_caixas == Q)
                        {
                            if(mapa[x+1][y] == 2)
                                mapa[x+1][y] = 6;  //COLOCAR A CAIxA E ALVO NO MESMO LUGAR
                            else
                                mapa[x+1][y] = 4;  //COLOCAR A CAIxA

                            abrir_borda(x+1,y);  //MANDA A POSIÇÃO DA CAIXA
                        }
                if((verificar(x+3, y)) && (passos_caixas < Q) && (mapa[x+3][y] != 7)&&(mapa[x+2][y] != 7)) //VERIFICA SE PODE DAR O SEGUNDO PASSO PARA A DIREITA
                {
                    if(mapa[x+2][y] == 1)
                        mapa[x+2][y] = 0;
                    if(mapa[x+3][y] == 1)
                        mapa[x+3][y] = 0;
                    anterior = 3;
                    passos_caixas++;
                    if(passos_caixas == Q)
                        {
                            if(mapa[x+2][y] == 2)
                                mapa[x+2][y] = 6;  //COLOCAR A CAIxA E ALVO NO MESMO LUGAR
                            else
                                mapa[x+2][y] = 4;  //COLOCAR A CAIxA

                            abrir_borda(x+2,y);  //MANDA A POSIÇÃO DA CAIXA
                        }
                    andar_gerador(x+2, y);
                    return;
                }
                andar_gerador(x+1, y);
                return;
        }
        else if(anterior == 2) ///anterior pra esquerda e agora pra direita
        {
            andar_gerador(x,y);
            return;
        }
        else if(anterior == 3) ///anterior pra direita e agora pra direita
        {
                if(mapa[x+1][y] == 1)
                    mapa[x+1][y] = 0;
                if(mapa[x+2][y] == 1)
                    mapa[x+2][y] = 0;
                passos_caixas++;
                anterior = 3;
                if(passos_caixas == Q)
                        {
                            if(mapa[x+1][y] == 2)
                                mapa[x+1][y] = 6;  //COLOCAR A CAIXA E ALVO NO MESMO LUGAR
                            else
                                mapa[x+1][y] = 4;  //COLOCAR A CAIXA

                            abrir_borda(x+1,y);  //MANDA A POSIÇÃO DA CAIXA
                        }
                if((verificar(x+3, y)) && (passos_caixas < Q) && (mapa[x+3][y] != 7)&&(mapa[x+2][y] != 7)) //VERIFICA SE PODE DAR O SEGUNDO PASSO PARA A DIREITA
                {
                    if(mapa[x+2][y] == 1)
                        mapa[x+2][y] = 0;
                    if(mapa[x+3][y] == 1)
                        mapa[x+3][y] = 0;
                    anterior = 3;
                    passos_caixas++;
                    if(passos_caixas == Q)
                        {
                            if(mapa[x+2][y] == 2)
                                mapa[x+2][y] = 6;  //COLOCAR A CAIxA E ALVO NO MESMO LUGAR
                            else
                                mapa[x+2][y] = 4;  //COLOCAR A CAIxA

                            abrir_borda(x+2,y);  //MANDA A POSIÇÃO DA CAIXA
                        }
                    andar_gerador(x+2, y);
                    return;
                }
                andar_gerador(x+1, y);
                return;
        }
    }
    if(passos_caixas < Q)
    {
        andar_gerador(x, y);
        return;
    }
}

void andar_gerador(int x, int y)
{
    int direcao;

    if((passos_caixas == Q)||(contador_g >= 5000))
        return;

    direcao = rand()%4;

    switch(direcao)
    {
        case 0: //anda para baixo
            baixo(x, y);
            break;
        case 1: //anda para cima
            cima(x, y);
            break;
        case 2: //anda para esquerda
            esquerda(x, y);
            break;
        case 3: //anda para direita
            direita(x, y);
            break;
    }
    return;
}

void coloca_cela(int x, int y, int lado)
{
    int alvo;

    ///Colocar os muros pétreos da carcaça da cela
    mapa[y][x] = mapa[y][x+1] = mapa[y][x+2] = mapa[y][x+3] = mapa[y][x+4] = mapa[y][x+5] = mapa[y][x+6] = 7;
    mapa[y+4][x] = mapa[y+4][x+1] = mapa[y+4][x+2] = mapa[y+4][x+3] = mapa[y+4][x+4] = mapa[y+4][x+5] = mapa[y+4][x+6] = 7;
    mapa[y+1][x] = mapa[y+3][x] = mapa[y+1][x+3] = mapa[y+3][x+3] = mapa[y+1][x+6] = mapa[y+3][x+6] = 7;
    mapa[y+1][x+1] = mapa[y+2][x+1] = mapa[y+3][x+1] = 0;
    mapa[y+1][x+2] = mapa[y+2][x+2] = mapa[y+3][x+2] = 0;
    mapa[y+1][x+4] = mapa[y+2][x+4] = mapa[y+3][x+4] = 0;
    mapa[y+1][x+5] = mapa[y+2][x+5] = mapa[y+3][x+5] = mapa[y+2][x+3] = 0;


    if(lado == 0) ///CELA ABERTA À DIREITA
    {
        mapa[y+2][x] = 7;
        mapa[y+2][x+6] = 0;

        alvo = rand()%2;
        if(alvo == 0)           //COLOCA ALVO 1 A CIMA
            mapa[y+1][x+1] = 2;
        else                    //COLOCA ALVO 1 A BAIXO
            mapa[y+3][x+1] = 2;

        alvo = rand()%2;
        if(alvo == 0)          //COLOCA ALVO 2 A CIMA
            mapa[y+1][x+4] = 2;
        else                   //COLOCA ALVO 2 A BAIXO
            mapa[y+3][x+4] = 2;

        alvo = rand()%6 + 1;
            mapa[y+2][x+alvo] = 2;  //COLOCA ALVO 3

        ///COLOCAR AS 3 CAIXAS FORA DA CELA
        mapa[y+2][x+7] = mapa[y+2][x+8] = 0;
        mapa[y+1][x+8] = mapa[y+3][x+7] = mapa[y+3][x+8] = 0;
        Q = 3; //Cada caixa dá 3 passos
        andar_gerador(y+1, x+8);
        passos_caixas = 0;
        andar_gerador(y+3, x+7);
        passos_caixas = 0;
        andar_gerador(y+3, x+8);
        passos_caixas = 0;
    }

    else ///CELA ABERTA À ESQUERDA
    {
        mapa[y+2][x+6] = 7;
        mapa[y+2][x] = 0;

        alvo = rand()%2;
        if(alvo == 0)           //COLOCA ALVO 1 A CIMA
            mapa[y+1][x+5] = 2;
        else                    //COLOCA ALVO 1 A BAIXO
            mapa[y+3][x+5] = 2;

        alvo = rand()%2;
        if(alvo == 0)          //COLOCA ALVO 2 A CIMA
            mapa[y+1][x+2] = 2;
        else                   //COLOCA ALVO 2 A BAIXO
            mapa[y+3][x+2] = 2;

        alvo = rand()%6;
            mapa[y+2][x+alvo] = 2;  //COLOCA ALVO 3
        ///COLOCAR AS 3 CAIXAS FORA DA CELA
        mapa[y+2][x-1] = mapa[y+2][x-2] = 0;
        mapa[y+1][x-2] = mapa[y+3][x-1] = mapa[y+3][x-2] = 0;
        Q = 3; //Cada caixa dá 3 passos
        andar_gerador(y+1, x-2);
        passos_caixas = 0;
        andar_gerador(y+3, x-1);
        passos_caixas = 0;
        andar_gerador(y+3, x-2);
        passos_caixas = 0;
    }
}

int verifica_alvo(int x, int y, int k)
{
    int j=0;

    while(j<k)
    {
        if((alvos_x[j] == x) && (alvos_y[j] == y))  //ESTÁ NO MESMO LUGAR QUE OUTRO ALVO
            return 0;
        j++;
    }
    if(mapa[x][y] == 7)  //É MURO PÉTREO
        return 0;
    if((cela_x <= x) && (x <= (cela_x + 6)) && (cela_y <= y) && (y <= (cela_y + 4)))  //ESTÁ DENTRO DE UMA CELA
        return 0;

    return 1;
}

int gerar_mapa()
{
    int x, y, i, j, k, lado_cela;

    ///Definir o nível do mapa a ser gerado
    if(dificuldade_gerador == 1)
    {
        Q = 7;
        quant_caixas = 3;
        quant_tunel = 0;
        //QUANTIDADE DE PASSOS: Q = 7
    }
    else if(dificuldade_gerador == 2)
    {
        Q = 9;
        quant_caixas = 4;
        quant_tunel = 5;
        //QUANTIDADE DE PASSOS: Q = 9
    }
    else if(dificuldade_gerador == 3)
    {
        Q = 6;
        quant_caixas = 5;
        quant_tunel = 4;
        //QUANTIDADE DE PASSOS: Q = 6
        //MAIS 3 CAIXAS DA CELA
    }

    for(i=0;i<N;i++)
    {
        for(j=0;j<N;j++)
        {
            mapa[i][j] = 1;
        }
    }

    if(dificuldade_gerador == 3)
    {
        ///COLOCA A CELA ANTES DAS OUTRAS CAIXAS

        lado_cela = rand()%2;
        if(lado_cela == 0) //CELA COM ABERTURA A DIREITA
        {
            cela_x = rand()%(N-10);
            cela_y = rand()%(N-5);
            coloca_cela(cela_x, cela_y, 0);
        }
        else //CELA COM ABERTURA A ESQUERDA
        {
            cela_x = rand()%(N-10) + 3;
            cela_y = rand()%(N-5);
            coloca_cela(cela_x, cela_y, 1);
        }
        Q = 6;
    }

    for(k=0;k<quant_caixas;k++)
    {
        do
        {
        x = rand()%N;
        y = rand()%N;
        }while(verifica_alvo(x, y, k) == 0);

        mapa[x][y] = 2;  //COLOCA O ALVO

        andar_gerador(x, y);  ///Tá dando muita muita merda aqui!!!

        if(contador_g >= 5000) ///Entrou em loop (mapa inválido)
        {
            valida_mapa();
            return;
        }

        passos_caixas = 0;
        alvos_x[k] = x;  //Estas variáveis servem para que não
        alvos_y[k] = y;  //tenhamos dois alvos no mesmo ponto
    }

    do
    {
    x = rand()%(N-2) + 1;
    y = rand()%(N-2) + 1;
    }while(mapa[x][y] != 0);

    mapa[x][y] = 3;  //COLOCA O BONECO NUMA POSIÇÃO LIVRE

    return;
}

void imprime_mapa_gerado()
{
    int i,j;
    FILE* p = fopen("Mapas Salvos/mapa_0_0.txt","w");
    fprintf(p, "%d %d\n", N, N);

    for(i=0;i<N;i++)
        {
            for(j=0;j<N;j++)
                    fprintf(p, "%d ",mapa[i][j]);
            fprintf(p, "\n");
        }
fclose(p);
}

