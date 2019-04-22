#include "teclado.h"

void keyboard_input()
{
   int i;

   for(i = 0; i < KEY_MAX; i++)
     teclas_anteriores[i] = key[i];

   poll_keyboard();
}

int apertou(int TECLA)
{
    return(teclas_anteriores[TECLA] == FALSE && key[TECLA] == TRUE);
}

int segurou(int TECLA)
{
    return(teclas_anteriores[TECLA] == TRUE && key[TECLA] == TRUE);
}

int soltou(int TECLA)
{
    return(teclas_anteriores[TECLA] == TRUE && key[TECLA] == FALSE);
}
