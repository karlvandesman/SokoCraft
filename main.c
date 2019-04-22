#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <allegro.h>
#include <time.h>
#include <math.h>
#include "button.h"
#include "button.c"
#include "teclado.h"
#include "teclado.c"
#include "gerador.h"
#include "gerador.c"

///LEGENDAS:
///DIFICULDADE: 1- FÁCIL / 2- MÉDIO / 3- DIFÍCIL
///RAÇAS: 1- Terran 2- Protoss 3- Zerg

///ENUM
enum {TITLESCREEN, MAINMENU, JOGAR, EDITOR, MANUAL, SELECIONAR_DIFICULDADE,
        SELECIONAR_MAPA, SELECIONAR_RACA, PASSWORD, PROGRESSO,
        MAPAS_SALVOS};
enum
{ livre = 0, muro = 1, acerto = 2, pes = 3, caixa = 4, acerto_pes = 5, acerto_caixa = 6, muro_petreo=7};

///VARIÁVEIS GLOBAIS
int screen_state = TITLESCREEN;
const int TILESIZE = 50;
int cont_passos = 0;
int** mapa_j, startar, Tam = 13;
volatile int Race=0;
int linhas, colunas;
int dificuldade, nivel;
char nome_do_arq [45];
volatile int ticks, exit_program, timer;
BITMAP* Heroi;
BITMAP* Caixa;
BITMAP* cursor;

///FUNÇÕES
void salvar_minimapa();
void f_selecionar_dificuldade();
int** Carregar_Mapa(char* nome_arquivo);
void manual();
void titlescreen();
void mainmenu();
void init();
void Desenhar_Mapa(BITMAP* buffer, BITMAP* Alvo, BITMAP* Solo, BITMAP* Parede, int* cont_caixas);
void Libera_Mapa();
void EscreveMapa();
void Button_action(BITMAP* buffer, Button* b_caixa, Button* b_alvo, Button* b_parede, Button* b_heroi, Button* b_lixo, Button* b_salvar, Button* b_sair, Button* b_playp, BITMAP** playp_bot, char* nome_arquivo, int* exit_screen);
void IdentCoord(int *abscissa, int *ordenada);
void andar(int *abscissa, int *ordenada);
void ContaCaixas(int *cont_caixas);
void editor();
void select_race();
void jogar();
void password();
void progresso();
int verifica_password(int senha_tentativa);
void f_mapas_salvos();
void selecionar_dificuldade();

void tick_counter() { ticks++; }
END_OF_FUNCTION(tick_counter)

void fecha_programa() { exit_program = TRUE; }
END_OF_FUNCTION(fecha_programa)

void incrementa_timer() {timer++;}
END_OF_FUNCTION(incrementa_timer);

 ///POSICAO DOS BOTOES
	int pos_bot1x[8];//x
	int pos_bot1y[8];//y
	int i;
	//final da criacao dos botoes

///DANDO OS VALORES PARA X E Y NOS POS_BOTs
	void AlteraBot(){
		int i, j=0;
		for(i=0; i<8; i++){
			if(i%2==0){
			pos_bot1x[i]=700;
			//pos_bot2x[i]=100;
			pos_bot1y[i]=75 + j;
			//pos_bot2y[i]=200 + j;
			j=j+80;
			}
			else{
				pos_bot1x[i]=700+80;
				pos_bot1y[i]=75+(j-80);
			}
		}
	}
	//final da entrada de valores
END_OF_FUNCTION(AlteraBot);

int main()
{
	init();
	srand(time(NULL));

    while(!exit_program)
    {
        while(ticks>0 && !exit_program)
        {
            if(key[KEY_ESC])
                    fecha_programa();

            if(screen_state == MAINMENU)
                            mainmenu();
            else if(screen_state == TITLESCREEN)
                            titlescreen();
            else if(screen_state==MANUAL)
                                manual();
			else if(screen_state==EDITOR)
								editor();
            else if(screen_state==SELECIONAR_RACA)
                                select_race();
            else if(screen_state==PASSWORD)
                                password();
            else if(screen_state==JOGAR)
                                jogar();
            else if(screen_state==PROGRESSO)
                                progresso();
            else if(screen_state==MAPAS_SALVOS)
                                f_mapas_salvos();
            else if (screen_state==SELECIONAR_DIFICULDADE)
                                f_selecionar_dificuldade();
            else
                mainmenu();

        ticks--;
        }
    }

    Libera_Mapa();
    if(cursor)
        destroy_bitmap(cursor);
    if(Heroi)
        destroy_bitmap(Heroi);
    if(Caixa)
        destroy_bitmap(Caixa);

    allegro_exit();

    return 0;
}
END_OF_MAIN();

void manual()
{
    int exit_screen = FALSE;

    SAMPLE* click = load_sample("Sons/click2.wav");

    BITMAP* next = load_bitmap("Sprites/next.bmp", NULL);
    BITMAP* next_h = load_bitmap("Sprites/next_h.bmp", NULL);
    BITMAP* fundo_manual1= load_bitmap("Fundo/fundo_manual1.bmp", NULL);
    BITMAP* sair = load_bitmap("Sprites/sair.bmp" ,NULL);
    BITMAP* sair_h = load_bitmap("Sprites/sair_h.bmp" ,NULL);
    BITMAP* img_manual= load_bitmap("Sprites/botao_menu2.bmp",NULL);
    BITMAP* img_manual_h= load_bitmap("Sprites/botao_menu2_highlight.bmp",NULL);
    BITMAP* buffer = create_bitmap(SCREEN_W,SCREEN_H);
    BITMAP* cursor = load_bitmap("Sprites/cursor_principal.bmp",NULL);

    Button* botao_sair = create_button(sair, sair_h, click, SCREEN_W/2-35, 625);

    while(!exit_screen && !exit_program){

            if(key[KEY_ESC])
                    fecha_programa();

            if(key[KEY_DEL]){
                screen_state= MAINMENU;
                exit_screen=TRUE;
                            }

            draw_sprite(buffer, fundo_manual1, 0, 0);

            button_input(botao_sair);

            button_draw(botao_sair, buffer);

            if(botao_sair->ativado){
                    botao_sair->ativado=FALSE;
                    screen_state=MAINMENU;
                    exit_screen= TRUE;
                                    }

            draw_sprite(buffer, cursor, mouse_x, mouse_y);
            draw_sprite(screen,buffer,0,0);

            clear(buffer);
                                        }

    destroy_bitmap(sair);
    destroy_bitmap(sair_h);
    destroy_bitmap(next);
    destroy_bitmap(next_h);
    destroy_bitmap(fundo_manual1);
    destroy_bitmap(cursor);
    destroy_bitmap(img_manual);
    destroy_bitmap(img_manual_h);
    destroy_bitmap(buffer);

    destroy_button (botao_sair);

    destroy_sample (click);

}
END_OF_FUNCTION(manual);

void titlescreen()
{
    int exit_screen = FALSE;
    float x=0,vel=0.1;

    ///SONS
    SAMPLE* abertura = load_sample("Sons/abertura.wav");

    ///BITMAP
    BITMAP* buffer = create_bitmap(SCREEN_W,SCREEN_H);
    BITMAP* fundo = load_bitmap("Fundo/fundo.bmp",NULL);
    BITMAP* botao = load_bitmap("Sprites/botao_jogar.bmp",NULL);

    play_sample(abertura,255,128,1000,FALSE);

    while(!exit_program && !exit_screen)
    {
        while(ticks>0 && !exit_program && !exit_screen)
            {

            if(key[KEY_ESC])
                    fecha_programa();

            if(key[KEY_ENTER]){
                exit_screen=TRUE;
                screen_state= SELECIONAR_RACA;
                            }

             set_trans_blender(255,255,255,x);

             draw_sprite_ex(buffer,fundo,0,0,DRAW_SPRITE_TRANS,DRAW_SPRITE_NORMAL);

             if(x>255){
                        draw_sprite(buffer,botao,SCREEN_W/2-90,SCREEN_H/2+180);
                        if(timer%2)
                        textout_centre_ex(buffer,font,"PRESS ENTER BUTTON", SCREEN_W/2,SCREEN_H/2+200,makecol(255,255,255),-1);
                        }
            draw_sprite(screen,buffer,0,0);

            clear(buffer);
            if(x<255){
                    x=x+vel;
                        }
            vel=vel+0.0008;
            ticks--;
            }
    }
destroy_bitmap(buffer);
destroy_bitmap(fundo);
destroy_bitmap(botao);

destroy_sample(abertura);
}
END_OF_FUNCTION(titlescreen);

void mainmenu()
{
    timer=0;

    int exit_screen = FALSE;
    ///Fonts

    FONT* pixel_14 = load_font("Fontes/pixel_14.pcx", NULL,NULL);

    BITMAP* buffer = create_bitmap(SCREEN_W,SCREEN_H);
    BITMAP* botao_menu = load_bitmap("Sprites/botao_menu.bmp",NULL);
    BITMAP* botao_menu_h= load_bitmap("Sprites/botao_menu_h.bmp",NULL);
    BITMAP* botao_menu3 = load_bitmap("Sprites/botao_menu2_highlight.bmp",NULL);
    BITMAP* fundo_menu = load_bitmap("Fundo/menu.bmp",NULL);
    BITMAP* cursor_principal= load_bitmap("Sprites/cursor_principal.bmp",NULL);

    SAMPLE* click2 = load_sample("Sons/click2.wav");
    SAMPLE* menu = load_sample("Sons/menu.wav");

    Button* botao_menu_j = create_button(botao_menu, botao_menu_h, click2, 0 , SCREEN_H/2-50);
    Button* botao_menu_e = create_button(botao_menu, botao_menu_h, click2, 0 , SCREEN_H/2+50);
    Button* botao_menu_m = create_button(botao_menu, botao_menu_h, click2, 0 , SCREEN_H/2+150);

    play_sample(menu,255,128,1000,TRUE);

    while(!exit_program && !exit_screen)
        {

        while( (!exit_program) && (!exit_screen) && ticks>0)
                {
                if(key[KEY_ESC])
                    fecha_programa();

                ///FUNDO
                draw_sprite (buffer, fundo_menu, 0, 0);

                ///BOTÕES
                button_input(botao_menu_j);
                button_input(botao_menu_e);
                button_input(botao_menu_m);

                    if(botao_menu_j->ativado){
                                botao_menu_j->ativado=FALSE;
                                exit_screen=TRUE;
                                screen_state=SELECIONAR_DIFICULDADE;
                                                }
                    if(botao_menu_e->ativado){
                                botao_menu_e->ativado=FALSE;
                                exit_screen = TRUE;
                                screen_state=EDITOR;
                                                }

                    if(botao_menu_m->ativado){
                                botao_menu_m->ativado=FALSE;
                                screen_state=MANUAL;
                                exit_screen=TRUE;
                                            }

                        button_draw(botao_menu_j,buffer);
                        button_draw(botao_menu_e,buffer);
                        button_draw(botao_menu_m,buffer);

                ///TEXTOS

                textout_centre_ex(buffer, pixel_14, "JOGAR", 200, SCREEN_H/2+37, makecol(0,0,0), -1 );
                textout_centre_ex(buffer, pixel_14, "EDITOR", 200, SCREEN_H/2+137, makecol(0,0,0), -1 );
                textout_centre_ex(buffer, pixel_14, "MANUAL", 200, SCREEN_H/2+237, makecol(0,0,0), -1 );

                ///MOUSE
                draw_sprite(buffer, cursor_principal,mouse_x,mouse_y);

                draw_sprite(screen,buffer,0,0);

                clear(buffer);

                ticks--;
                }
        }

    ///DESTROY BITMAP
    destroy_bitmap(buffer);
    destroy_font(pixel_14);
    destroy_bitmap(botao_menu);
    destroy_bitmap(botao_menu3);
    destroy_bitmap(botao_menu_h);
    destroy_bitmap(cursor_principal);
    destroy_bitmap(fundo_menu);



    ///DESTROY MÚSICA
    destroy_sample(click2);
    destroy_sample(menu);

    ///DESTROY BOTÃO
    destroy_button(botao_menu_j);
    destroy_button(botao_menu_e);
    destroy_button(botao_menu_m);
}
END_OF_FUNCTION(mainmenu);

void init()
{
  allegro_init();
  install_keyboard();
  install_mouse();
  set_color_depth(32);
  set_gfx_mode(GFX_AUTODETECT_WINDOWED, 900, 700, 0, 0);
  set_window_title("Sokocraft");
  install_sound(DIGI_AUTODETECT,MIDI_AUTODETECT,NULL);
  set_display_switch_mode(SWITCH_BACKGROUND);

  install_timer();
  LOCK_FUNCTION(incrementa_timer);
  LOCK_VARIABLE(timer);
  install_int_ex(incrementa_timer, BPS_TO_TIMER(1));

  install_int_ex(tick_counter,BPS_TO_TIMER(60));
  ticks = 0;
  LOCK_FUNCTION(tick_counter);
  LOCK_VARIABLE(ticks);

  exit_program = FALSE;
  LOCK_FUNCTION(fecha_programa);
  LOCK_VARIABLE(exit_program);
  set_close_button_callback(fecha_programa);

}
END_OF_FUNCTION(init);

void EscreveMapa(){
     int i, j, Contador=0;
     FILE *fcont = fopen("Mapas Salvos/ContMap.txt", "r");
     fscanf(fcont, " %d", &Contador);
     fclose(fcont);
     sprintf(nome_do_arq, "Mapas Salvos/mapa_0_%d.txt", Contador+1);
     FILE *fout = fopen(nome_do_arq, "w");

     fprintf(fout, "%d %d\n", linhas,colunas);
     for(i =0; i<linhas; i++){
           for(j=0; j<colunas; j++)
                   fprintf(fout, "%d ", mapa_j[i][j]);
                   fprintf(fout, "\n");
                                }
     fclose(fout);
     fcont = fopen("Mapas Salvos/ContMap.txt", "w");
     ++Contador;
     fprintf(fcont, "%d", Contador);
     fclose(fcont);
}
END_OF_FUNCTION(EscreveMapa);

void  Button_action (BITMAP* buffer, Button* b_caixa, Button* b_alvo, Button* b_parede, Button* b_heroi, Button* b_lixo, Button* b_salvar, Button* b_sair, Button* b_playp, BITMAP** playp_bot, char* nome_arquivo, int* exit_screen){

    BITMAP* img_salvar = load_bitmap("Sprites/botao_jogar.bmp", NULL);

	startar=0;
	char temp[360];
	int j, i, x, y;
	int correcao_x= 25+ (13-colunas)*TILESIZE/2, correcao_y=25+ (13-linhas)*TILESIZE/2 ;
	x = (mouse_x-correcao_x);
	y = (mouse_y-correcao_y);
	j = (x/TILESIZE);
	i = (y/TILESIZE);
	///acoes dos botoes //->
    if(b_caixa->ativado){
                        if( mouse_x>correcao_x && mouse_x<(correcao_x +(50*colunas)) && mouse_y> correcao_y && mouse_y < (correcao_y+(50*linhas)) ){
                                    if(mouse_b == 1)
                                        {
                                            //BOTÃO ESQUERDO ATIVADO
                                        }
                                    if(mouse_b == 2)
                                      {
                                        mapa_j[i][j] = 4;
                                      }
                                                                                                                        }
                        textout_centre_ex(buffer,  font, "Botao CAIXA Ativado!", 793, 470, makecol(255,255,255),  -1);
                        cursor = Caixa;
                        }

     else if(b_alvo->ativado){

                            if( mouse_x>correcao_x && mouse_x<(correcao_x +(50*colunas)) && mouse_y> correcao_y && mouse_y < (correcao_y+(50*linhas)) ){
                                            if(mouse_b == 1)
                                                            {
                                                                //BOTÃO ESQUERDO ATIVADO
                                                            }
                                            if(mouse_b == 2)
                                                            {
                                                            mapa_j[i][j] = 2;
                                                            }
                                                                                                                        }
                            textout_centre_ex(buffer,  font, "Botao ALVO Ativado!", 793, 470, makecol(255,255,255),  -1);
                            cursor = load_bitmap("Sprites/Objetos/acerto.bmp", NULL);
     }

	 else if(b_parede->ativado){

                                if( mouse_x>correcao_x && mouse_x<(correcao_x +(50*colunas)) && mouse_y> correcao_y && mouse_y < (correcao_y+(50*linhas)) ){
                                                                if(mouse_b == 1)
                                                              {
                                                                  //BOTÃO ESQUERDO ATIVADO
                                                              }
                                                              if(mouse_b == 2)
                                                              {
                                                                    mapa_j[i][j] = 1;
                                                              }
                                                                                                                            }
                                textout_centre_ex (buffer,  font, "Botao PAREDE Ativado!", 793, 470, makecol(255,255,255),  -1);
                                cursor = load_bitmap("Sprites/Objetos/muro.bmp", NULL);
                                }

	 else if(b_heroi->ativado){

                            if( mouse_x>correcao_x && mouse_x<(correcao_x +(50*colunas)) && mouse_y> correcao_y && mouse_y < (correcao_y+(50*linhas)) ){
                                                            if(mouse_b == 1)
                                                          {
                                                              //BOTÃO ESQUERDO ATIVADO
                                                          }
                                                          if(mouse_b == 2)
                                                          {
                                                              mapa_j[i][j] = 3;
                                                          }
                                                                                                                            }
                            textout_centre_ex (buffer,  font, "Botao HEROI Ativado!", 793, 470, makecol(255,255,255),  -1);
                            cursor = Heroi;
                                }

     else if(b_lixo->ativado){

                        if( mouse_x>correcao_x && mouse_x<(correcao_x +(50*colunas)) && mouse_y> correcao_y && mouse_y < (correcao_y+(50*linhas)) ){
                                                                        if(mouse_b == 1)
                                                                      {
                                                                          //BOTÃO ESQUERDO ATIVADO
                                                                      }
                                                                      if(mouse_b == 2)
                                                                      {
                                                                          mapa_j[i][j] = 0;
                                                                      }
                                                                                                                    }
                     textout_centre_ex (buffer,  font, "Botao LIXEIRA Ativado!", 793, 470, makecol(255,255,255),  -1);
                            }

	else if(b_salvar->ativado){

                            salvar_minimapa();
                            EscreveMapa();
                            textout_centre_ex(buffer,  font, "Salvo!", 793, 470, makecol(255,255,255),  -1);
                            b_salvar->ativado = FALSE;
                                }

     else if(b_sair->ativado){
                            screen_state=MAINMENU;
                            *exit_screen= TRUE;
                            b_sair->ativado = FALSE;
                                }

     else if(b_playp->ativado){
                            *playp_bot = load_bitmap("Sprites/pause_bot.bmp", NULL);
                            startar=1;
     }

     else   {
            *playp_bot = load_bitmap("Sprites/play_bot.bmp", NULL);
            if(Race==1)
            cursor = load_bitmap("Sprites/cursor_terran.bmp", NULL);
            else if(Race==2)
            cursor = load_bitmap("Sprites/cursor_protoss.bmp", NULL);
            else if(Race==3)
            cursor = load_bitmap("Sprites/cursor_zerg.bmp", NULL);
            }

	if(!b_playp->ativado){
                         if(apertou(KEY_T)){
                         FILE *fout1 = fopen("Mapas Salvos/Mapa Ram.txt", "w");
                         fprintf(fout1, "%d %d\n", linhas,colunas);
                         for(i =0; i<linhas; i++){
                               for(j=0; j<colunas; j++)
                                       fprintf(fout1, "%d ", mapa_j[i][j]);
                                       fprintf(fout1, "\n");
                         }
                         fclose(fout1);
                            }

                        if(apertou(KEY_R)){
                         FILE *fout2 = fopen("Mapas Salvos/Mapa Ram.txt", "r");
                         fscanf(fout2, " %d %d\n", &linhas, &colunas);
                         for(i =0; i<linhas; i++){
                               for(j=0; j<colunas; j++)
                                       fscanf(fout2, " %d", &mapa_j[i][j]);
                         }
                         fclose(fout2);
                            }

                        if(apertou(KEY_S)){
                            if(linhas > 12)
                                return;
                            else{
                            FILE *flp = fopen("Mapas Salvos/Mapa Ram.txt", "w");
                            fprintf(flp, "%d %d\n", (linhas+1), colunas);
                            for(i =0; i<(linhas+1); i++){
                               for(j=0; j<colunas; j++){
                                       if(i<linhas)
                                       fprintf(flp, "%d ", mapa_j[i][j]);
                                       if(i==linhas)
                                       fprintf(flp, "%d ", 0);
                                }
                                       fprintf(flp, "\n");
                            }
                            fclose(flp);
                            Libera_Mapa();
                            mapa_j = Carregar_Mapa("Mapas Salvos/Mapa Ram.txt");
                            }
                        }
                        if(apertou(KEY_D)){
                                         if(colunas > 12)
                                                return;
                                            else{
                                            FILE *fcp = fopen("Mapas Salvos/Mapa Ram.txt", "w");
                                            fprintf(fcp, "%d %d\n", (linhas), (colunas+1));

                                            for(i =0; i<linhas; i++){
                                               for(j=0; j<(colunas+1); j++){
                                                       if(j<colunas)
                                                       fprintf(fcp, "%d ", mapa_j[i][j]);
                                                       if(j==colunas)
                                                       fprintf(fcp, "%d ", 0);
                                                                            }
                                                fprintf(fcp, "\n");
                                                                        }
                                            fclose(fcp);
                                            Libera_Mapa();
                                            mapa_j = Carregar_Mapa("Mapas Salvos/Mapa Ram.txt");
                                            }
                                            }
                        if(apertou(KEY_W)){
                                             if(linhas < 4)
                                                    return;
                                                else{
                                                FILE *flm = fopen("Mapas Salvos/Mapa Ram.txt", "w");
                                                fprintf(flm, "%d %d\n", (linhas-1), colunas);
                                                for(i =0; i<(linhas-1); i++){
                                                   for(j=0; j<colunas; j++){
                                                           fprintf(flm, "%d ", mapa_j[i][j]);
                                                    }
                                                           fprintf(flm, "\n");
                                                }
                                                fclose(flm);
                                                Libera_Mapa();
                                                mapa_j = Carregar_Mapa("Mapas Salvos/Mapa Ram.txt");
                                                }
                                            }
                        if(apertou(KEY_A)){
                         if(colunas < 4)
                                return;
                            else{
                                    FILE *fcm = fopen("Mapas Salvos/Mapa Ram.txt", "w");
                                    fprintf(fcm, "%d %d\n", (linhas), (colunas-1));
                                    for(i =0; i<(linhas); i++){
                                       for(j=0; j<(colunas-1); j++){
                                               fprintf(fcm, "%d ", mapa_j[i][j]);
                                        }
                                               fprintf(fcm, "\n");
                                    }
                                    fclose(fcm);
                                    Libera_Mapa();
                                    mapa_j = Carregar_Mapa("Mapas Salvos/Mapa Ram.txt");
                            }
                        }
                        }
    destroy_bitmap(img_salvar);
}
END_OF_FUNCTION(Button_action);

int** Carregar_Mapa(char* nome_arquivo){
    printf("CARREGOU O MAPA!!\n");
   FILE* F = fopen(nome_arquivo, "r");
   int** matriz;

   if(F != NULL)
   {
     int i, j;

     fscanf(F, "%d %d", &linhas, &colunas);

     //ALOCA O MAPA
     matriz = (int**) malloc ( linhas * sizeof(int*));
     for(i = 0; i < linhas; i++)
      matriz[i] = (int*) malloc( colunas * sizeof(int));

     //CARREGA TILES
     for(i = 0; i < linhas; i++)
     {
        for(j = 0; j < colunas; j++)
        {
          fscanf(F, " %d", &matriz[i][j]);
        }
     }
   }

    fclose(F);

   return matriz;
}
END_OF_FUNCTION(Carregar_Mapa);

void Desenhar_Mapa(BITMAP* buffer, BITMAP* Alvo, BITMAP* Solo, BITMAP* Parede, int *cont_caixas)
{
    int i, j, correcao_x= 25+ (13-colunas)*TILESIZE/2, correcao_y= 25 + (13-linhas)*TILESIZE/2;
	*cont_caixas = 0;

    BITMAP* fundo = load_bitmap("Sprites/mapa_fundo.bmp", NULL);

    for(i = 0; i < linhas; i++)
     {
        for(j = 0; j < colunas; j++)
        {
           set_trans_blender(255,255,255,130);
           draw_sprite_ex (buffer, fundo, ((j *TILESIZE)+ correcao_x), ((i * TILESIZE)+correcao_y), DRAW_SPRITE_TRANS, DRAW_SPRITE_NO_FLIP);

           if(mapa_j[i][j] == acerto){
             draw_sprite(buffer, Alvo, ((j * TILESIZE)+ correcao_x) , ((i * TILESIZE)+correcao_y));
                                    }
           else if(mapa_j[i][j] == livre){
             //draw_sprite_ex(buffer, Solo, (j * TILESIZE)+ correcao_x , ((i * TILESIZE)+correcao_y), DRAW_SPRITE_NORMAL, DRAW_SPRITE_NO_FLIP);
                                        }
           else if(mapa_j[i][j] == muro){
             draw_sprite(buffer, Parede, ((j * TILESIZE)+ correcao_x) , ((i * TILESIZE)+correcao_y));
                                        }

           else if(mapa_j[i][j] == muro_petreo){
             draw_sprite(buffer, Parede, ((j * TILESIZE)+ correcao_x) , ((i * TILESIZE)+correcao_y));
                                            }
           else if(mapa_j[i][j] == pes){
             draw_sprite (buffer, Heroi, (j * TILESIZE)+ correcao_x , ((i * TILESIZE)+ 25 + (13-linhas)*(TILESIZE/2)));
                                    }
           else if(mapa_j[i][j] == caixa){
             draw_sprite(buffer, Caixa, (j * TILESIZE)+ correcao_x , ((i * TILESIZE)+25 + (13-linhas)*(TILESIZE/2)));
             (*cont_caixas) = (*cont_caixas) + 1;
                                        }

		   else if(mapa_j[i][j] == acerto_pes){
			 draw_sprite (buffer, Alvo, (j * TILESIZE)+ correcao_x , ((i * TILESIZE)+ 25 + (13-linhas)*(TILESIZE/2)));
             draw_sprite (buffer, Heroi, (j * TILESIZE)+ correcao_x , ((i * TILESIZE)+ 18 + (13-linhas)*(TILESIZE/2)));
                                            }

		   else if(mapa_j[i][j] == acerto_caixa){
			 draw_sprite(buffer, Alvo, (j * TILESIZE)+ correcao_x , ((i * TILESIZE)+correcao_y) );
			 set_burn_blender (255, 255, 255, 170);
             draw_sprite_ex(buffer, Caixa, (j * TILESIZE)+ correcao_x , ((i * TILESIZE)+ 15 + (13-linhas)*TILESIZE/2), DRAW_SPRITE_TRANS, DRAW_SPRITE_NO_FLIP);
                                                }
        }
     }

destroy_bitmap(fundo);
}
END_OF_FUNCTION(Desenhar_Mapa);

void Libera_Mapa(){
    printf("LIBEROU O MAPA!!\n");
   int i;

   if(mapa_j == NULL)
        return;

   for(i = 0; i < linhas; i++)
    free(mapa_j[i]);

   free(mapa_j);
}
END_OF_FUNCTION(Libera_Mapa);

void IdentCoord(int *abscissa, int *ordenada){
    int i, j;
     for(i=0; i<linhas; i++){
            for(j=0; j<colunas; j++){

                    if(mapa_j[i][j]==pes || mapa_j[i][j]==acerto_pes){
                        *abscissa = i; *ordenada = j;
                    }

                }
            }

}
END_OF_FUNCTION(IdentCoord);

void andar(int *abscissa, int *ordenada){
        int x, y, i, j;
        ///ponteiros para realizar a troca por referencia.
        x = *abscissa;
        y = *ordenada;
	///condicoes de troca de cada movimento.
        if(apertou(KEY_RIGHT)){
            if( ((y+1)>(colunas-1)) ||  ( ((mapa_j[x][y+1] == caixa) || (mapa_j[x][y+1]==acerto_caixa))  && ( (y+2)>(colunas-1))  ) )
				return;
			else {
						if(Race==1)
						Heroi = load_bitmap("Sprites/Objetos/terran_right.bmp", NULL);
						if(Race==2)
						Heroi = load_bitmap("Sprites/Objetos/protoss_right.bmp", NULL);
						if(Race==3)
						Heroi = load_bitmap("Sprites/Objetos/zerg_right.bmp", NULL);
				}
            if(mapa_j[x][y+1] == livre){
                mapa_j[x][y+1]=pes;
                if((mapa_j[x][y])==acerto_pes){
                    mapa_j[x][y] = acerto;
                    ++cont_passos;}
                else{
                    mapa_j[x][y] = livre;
                    ++cont_passos;
                }
                *ordenada = *ordenada + 1;
            }
            else if(mapa_j[x][y+1] == acerto){
                mapa_j[x][y+1]=acerto_pes;
                if((mapa_j[x][y])==acerto_pes){
                    mapa_j[x][y] = acerto;
                    ++cont_passos;}
                else{
                    mapa_j[x][y] = livre;
                    ++cont_passos;
                }
                *ordenada = *ordenada + 1;
            }
            else if((mapa_j[x][y+1] == caixa)&&(mapa_j[x][y+2] == livre)){
                mapa_j[x][y+1]=pes;
                mapa_j[x][y+2]=caixa;
                if((mapa_j[x][y])==acerto_pes){
                    mapa_j[x][y] = acerto;
                    ++cont_passos;}
                else{
                    mapa_j[x][y] = livre;
                    ++cont_passos;
                }
                *ordenada = *ordenada + 1;
            }
            else if((mapa_j[x][y+1] == caixa)&&(mapa_j[x][y+2] == acerto)){
                mapa_j[x][y+1]=pes;
                mapa_j[x][y+2]=acerto_caixa;
                if((mapa_j[x][y])==acerto_pes){
                    mapa_j[x][y] = acerto;
                    ++cont_passos;}
                else{
                    mapa_j[x][y] = livre;
                    ++cont_passos;
                }
                *ordenada = *ordenada + 1;
            }
            else if((mapa_j[x][y+1] == acerto_caixa)&&(mapa_j[x][y+2] == acerto)){
                mapa_j[x][y+1]=acerto_pes;
                mapa_j[x][y+2]=acerto_caixa;
                if((mapa_j[x][y])==acerto_pes){
                    mapa_j[x][y] = acerto;
                    ++cont_passos;}
                else{
                    mapa_j[x][y] = livre;
                    ++cont_passos;
                }
                *ordenada = *ordenada + 1;
            }
            else if((mapa_j[x][y+1] == acerto_caixa)&&(mapa_j[x][y+2] == livre)){
                mapa_j[x][y+1]=acerto_pes;
                mapa_j[x][y+2]=caixa;
                if((mapa_j[x][y])==acerto_pes){
                    mapa_j[x][y] = acerto;
                    ++cont_passos;}
                else{
                    mapa_j[x][y] = livre;
                    ++cont_passos;
                }
                *ordenada = *ordenada + 1;
            }
        }

        else if(apertou(KEY_DOWN)){
            if(((x+1)>(linhas-1)) ||  ( ((mapa_j[x+1][y] == caixa) || (mapa_j[x+1][y]==acerto_caixa))  && ( (x+2)>(linhas-1))))
				return;
			else {	if(Race==1)
						Heroi = load_bitmap("Sprites/Objetos/terran_down.bmp", NULL);
						if(Race==2)
						Heroi = load_bitmap("Sprites/Objetos/protoss_down.bmp", NULL);
						if(Race==3)
						Heroi = load_bitmap("Sprites/Objetos/zerg_down.bmp", NULL);
				}
            if(mapa_j[x+1][y] == livre){
                mapa_j[x+1][y]=pes;
                if((mapa_j[x][y])==acerto_pes){
                    mapa_j[x][y] = acerto;
                    ++cont_passos;}
                else{
                    mapa_j[x][y] = livre;
                    ++cont_passos;
                }
                *abscissa = *abscissa + 1;
            }
           else if(mapa_j[x+1][y] == acerto){
                mapa_j[x+1][y]=acerto_pes;
                if((mapa_j[x][y])==acerto_pes){
                    mapa_j[x][y] = acerto;
                    ++cont_passos;}
                else{
                    mapa_j[x][y] = livre;
                    ++cont_passos;
                }
                *abscissa = *abscissa + 1;
            }
            else if((mapa_j[x+1][y] == caixa)&&(mapa_j[x+2][y] == livre)){
                mapa_j[x+1][y]=pes;
                mapa_j[x+2][y]=caixa;
                if((mapa_j[x][y])==acerto_pes){
                    mapa_j[x][y] = acerto;
                    ++cont_passos;}
                else{
                    mapa_j[x][y] = livre;
                    ++cont_passos;
                }
                *abscissa = *abscissa + 1;
            }
           else if((mapa_j[x+1][y] == caixa)&&(mapa_j[x+2][y] == acerto)){
                mapa_j[x+1][y]=pes;
                mapa_j[x+2][y]=acerto_caixa;
                if((mapa_j[x][y])==acerto_pes){
                    mapa_j[x][y] = acerto;
                    ++cont_passos;}
                else{
                    mapa_j[x][y] = livre;
                    ++cont_passos;
                }
                *abscissa = *abscissa + 1;
            }
           else if((mapa_j[x+1][y] == acerto_caixa)&&(mapa_j[x+2][y] == acerto)){
                mapa_j[x+1][y]=acerto_pes;
                mapa_j[x+2][y]=acerto_caixa;
                if((mapa_j[x][y])==acerto_pes){
                    mapa_j[x][y] = acerto;
                    ++cont_passos;}
                else{
                    mapa_j[x][y] = livre;
                    ++cont_passos;
                }
                *abscissa = *abscissa + 1;
            }
           else if((mapa_j[x+1][y] == acerto_caixa)&&(mapa_j[x+2][y] == livre)){
                mapa_j[x+1][y]=acerto_pes;
                mapa_j[x+2][y]=caixa;
                if((mapa_j[x][y])==acerto_pes){
                    mapa_j[x][y] = acerto;
                    ++cont_passos;}
                else{
                    mapa_j[x][y] = livre;
                    ++cont_passos;
                }
                *abscissa = *abscissa + 1;
            }
        }

        else if(apertou(KEY_LEFT)){    //ESQUERDA (codigo ascii para seta)
                if(((y-1)<0) ||  (((mapa_j[x][y-1] == caixa) || (mapa_j[x][y-1]==acerto_caixa))  && ( (y-2)<0)))
				return;
				else {	if(Race==1)
						Heroi = load_bitmap("Sprites/Objetos/terran_left.bmp", NULL);
						if(Race==2)
						Heroi = load_bitmap("Sprites/Objetos/protoss_left.bmp", NULL);
						if(Race==3)
						Heroi = load_bitmap("Sprites/Objetos/zerg_left.bmp", NULL);
				}
                if(mapa_j[x][y-1] == livre){
                mapa_j[x][y-1]=pes;
                if((mapa_j[x][y])==acerto_pes){
                    mapa_j[x][y] = acerto;
                    ++cont_passos;}
                else{
                    mapa_j[x][y] = livre;
                    ++cont_passos;
                }
                *ordenada = *ordenada - 1;
            }
            else if(mapa_j[x][y-1] == acerto){
                mapa_j[x][y-1]=acerto_pes;
                if((mapa_j[x][y])==acerto_pes){
                    mapa_j[x][y] = acerto;
                    ++cont_passos;}
                else{
                    mapa_j[x][y] = livre;
                    ++cont_passos;
                }
                *ordenada = *ordenada - 1;
            }
            else if((mapa_j[x][y-1] == caixa)&&(mapa_j[x][y-2] == livre)){
                mapa_j[x][y-1]=pes;
                mapa_j[x][y-2]=caixa;
                if((mapa_j[x][y])==acerto_pes){
                    mapa_j[x][y] = acerto;
                    ++cont_passos;}
                else{
                    mapa_j[x][y] = livre;
                    ++cont_passos;
                }
                *ordenada = *ordenada - 1;
            }
            else if((mapa_j[x][y-1] == caixa)&&(mapa_j[x][y-2] == acerto)){
                mapa_j[x][y-1]=pes;
                mapa_j[x][y-2]=acerto_caixa;
                if((mapa_j[x][y])==acerto_pes){
                    mapa_j[x][y] = acerto;
                    ++cont_passos;}
                else{
                    mapa_j[x][y] = livre;
                    ++cont_passos;
                }
                *ordenada = *ordenada - 1;
            }
            else if((mapa_j[x][y-1] == acerto_caixa)&&(mapa_j[x][y-2] == acerto)){
                mapa_j[x][y-1]=acerto_pes;
                mapa_j[x][y-2]=acerto_caixa;
                if((mapa_j[x][y])==acerto_pes){
                    mapa_j[x][y] = acerto;
                    ++cont_passos;}
                else{
                    mapa_j[x][y] = livre;
                    ++cont_passos;
                }
                *ordenada = *ordenada - 1;
            }
            else if((mapa_j[x][y-1] == acerto_caixa)&&(mapa_j[x][y-2] == livre)){
                mapa_j[x][y-1]=acerto_pes;
                mapa_j[x][y-2]=caixa;
                if((mapa_j[x][y])==acerto_pes){
                    mapa_j[x][y] = acerto;
                    ++cont_passos;}
                else{
                    mapa_j[x][y] = livre;
                    ++cont_passos;
                }
                *ordenada = *ordenada - 1;
            }
        }

        else if(apertou(KEY_UP)){    //CIMA (codigo ascii para seta)
            if(((x-1)<0) ||  (((mapa_j[x-1][y] == caixa) || (mapa_j[x-1][y]==acerto_caixa))  && ( (x-2)<0)))
				return;
			else {		if(Race==1)
						Heroi = load_bitmap("Sprites/Objetos/terran_up.bmp", NULL);
						if(Race==2)
						Heroi = load_bitmap("Sprites/Objetos/protoss_up.bmp", NULL);
						if(Race==3)
						Heroi = load_bitmap("Sprites/Objetos/zerg_up.bmp", NULL);
			}
            if(mapa_j[x-1][y] == livre){
                mapa_j[x-1][y]=pes;
                if((mapa_j[x][y])==acerto_pes){
                    mapa_j[x][y] = acerto;
                    ++cont_passos;}
                else{
                    mapa_j[x][y] = livre;
                    ++cont_passos;
                }
                *abscissa = *abscissa - 1;
            }
            else if(mapa_j[x-1][y] == acerto){
                mapa_j[x-1][y]=acerto_pes;
                if((mapa_j[x][y])==acerto_pes){
                    mapa_j[x][y] = acerto;
                    ++cont_passos;}
                else{
                    mapa_j[x][y] = livre;
                    ++cont_passos;
                }
                *abscissa = *abscissa - 1;
            }
            else if((mapa_j[x-1][y] == caixa)&&(mapa_j[x-2][y] == livre)){
                mapa_j[x-1][y]=pes;
                mapa_j[x-2][y]=caixa;
                if((mapa_j[x][y])==acerto_pes){
                    mapa_j[x][y] = acerto;
                    ++cont_passos;}
                else{
                    mapa_j[x][y] = livre;
                    ++cont_passos;
                }
                *abscissa = *abscissa - 1;
            }
            else if((mapa_j[x-1][y] == caixa)&&(mapa_j[x-2][y] == acerto)){
                mapa_j[x-1][y]=pes;
                mapa_j[x-2][y]=acerto_caixa;
                if((mapa_j[x][y])==acerto_pes){
                    mapa_j[x][y] = acerto;
                    ++cont_passos;}
                else{
                    mapa_j[x][y] = livre;
                    ++cont_passos;
                }
                *abscissa = *abscissa - 1;
            }
            else if((mapa_j[x-1][y] == acerto_caixa)&&(mapa_j[x-2][y] == acerto)){
                mapa_j[x-1][y]=acerto_pes;
                mapa_j[x-2][y]=acerto_caixa;
                if((mapa_j[x][y])==acerto_pes){
                    mapa_j[x][y] = acerto;
                    ++cont_passos;}
                else{
                    mapa_j[x][y] = livre;
                    ++cont_passos;
                }
                *abscissa = *abscissa - 1;
            }
            else if((mapa_j[x-1][y] == acerto_caixa)&&(mapa_j[x-2][y] == livre)){
                mapa_j[x-1][y]=acerto_pes;
                mapa_j[x-2][y]=caixa;
                if((mapa_j[x][y])==acerto_pes){
                    mapa_j[x][y] = acerto;
                    ++cont_passos;}
                else{
                    mapa_j[x][y] = livre;
                    ++cont_passos;
                }
                *abscissa = *abscissa - 1;
            }
    }
}
END_OF_FUNCTION(andar);

void ContaCaixas(int *cont_caixas){

int i, j;
(*cont_caixas)=0;
for(i=0; i<linhas; i++){
                        for(j=0; j<colunas; j++){
                                                if(mapa_j[i][j] == 4 )
                                                                        (*cont_caixas) = (*cont_caixas) + 1;
                                                }
                        }
}
END_OF_FUNCTION(ContaCaixas);

void editor()
{
  ///VARIAVEIS
  int i,j, xpes = 0, ypes = 0, cont_caixas = 0;
  int exit_screen = FALSE;

  ///SONS
  SAMPLE* click = load_sample("Sons/click2.wav");
  SAMPLE* musica;

  if(Race==1){
    cursor = load_bitmap("Sprites/cursor_terran.bmp",NULL);
    musica = load_sample("Soundtracks/12-terran.wav");
            }
  else if(Race==2){
	musica = load_sample("Soundtracks/04-protoss.wav");
    cursor= load_bitmap("Sprites/cursor_protoss.bmp",NULL);
            }
  else if(Race==3){
	musica = load_sample("Soundtracks/18-zerg.wav");
	cursor= load_bitmap("Sprites/cursor_zerg.bmp",NULL);
                    }

  ///BITMAPS

	///fundo
  BITMAP* fundo = load_bitmap("Fundo/fundo_editor.bmp", NULL);
  BITMAP* fundo_mapa = load_bitmap("Sprites/mapa_fundo.bmp",NULL);
	///botao geral
  BITMAP* caixa_ativado = load_bitmap("Sprites/botao_jogar.bmp", NULL);
  BITMAP* editor_bot = load_bitmap("Sprites/botao_bot.bmp", NULL);
	///objetos //ajeitar
  BITMAP* Alvo = load_bitmap("Sprites/Objetos/acerto.bmp", NULL);
  BITMAP* Parede = load_bitmap("Sprites/Objetos/muro.bmp", NULL);
  BITMAP* Solo = load_bitmap("Sprites/mapa_fundo.bmp", NULL);
  ///objetos nos botoes
  BITMAP* lixo_bot = load_bitmap("Sprites/lixo_bot.bmp", NULL);
  BITMAP* salvar_bot = load_bitmap("Sprites/salvar_bot.bmp", NULL);
  BITMAP* sair_bot = load_bitmap("Sprites/sair_bot.bmp", NULL);
  BITMAP* playp_bot = load_bitmap("Sprites/play_bot.bmp", NULL);
	///highlitght
  BITMAP* lixo_highlight = load_bitmap("Sprites/lixo_highlight.bmp", NULL);
  BITMAP* ok_highlight = load_bitmap("Sprites/ok_highlight.bmp", NULL);
	///buffer
  BITMAP* buffer = create_bitmap(SCREEN_W, SCREEN_H);

  ///MAPAS
  Libera_Mapa();
  mapa_j = Carregar_Mapa("Mapas Salvos/Mapa Inicial.txt");

  ///alterando os botoes
		AlteraBot();

  ///BOTÕES
  Button* b_caixa = create_button(editor_bot, lixo_highlight, click, pos_bot1x[0], pos_bot1y[0]);
  Button* b_alvo = create_button(editor_bot, lixo_highlight, click, pos_bot1x[1], pos_bot1y[1]);
  Button* b_parede = create_button(editor_bot, lixo_highlight, click, pos_bot1x[2], pos_bot1y[2]);
  Button* b_heroi = create_button(editor_bot, lixo_highlight, click, pos_bot1x[3], pos_bot1y[3]);
  Button* b_lixo = create_button(editor_bot, lixo_highlight, click, pos_bot1x[4], pos_bot1y[4]);
  Button* b_salvar = create_button(editor_bot, lixo_highlight, click, pos_bot1x[5], pos_bot1y[5]);
  Button* b_sair = create_button(editor_bot, lixo_highlight, click, pos_bot1x[6], pos_bot1y[6]);
  Button* b_playp = create_button(editor_bot, lixo_highlight, click, pos_bot1x[7], pos_bot1y[7]);

  int ativacao[8];

  play_sample(musica, 255, 128, 1000, TRUE);

  ///GAME LOOP
  while(!exit_program && !exit_screen)
  {
  	while(ticks > 0 && !exit_program && !exit_screen){
                //INPUT
                keyboard_input();

                if(key[KEY_ESC])
                 fecha_programa();

                ///input botoes
                 button_input(b_caixa);
                 button_input(b_alvo);
                 button_input(b_parede);
                 button_input(b_heroi);
                 button_input(b_lixo);
                 button_input(b_salvar);
                 button_input(b_sair);
                 button_input(b_playp);

                //UPDATE
                if(startar==0){
                ContaCaixas(&cont_caixas);
                                }
                if(startar>0){
                Desenhar_Mapa(buffer, Alvo, Solo, Parede, &cont_caixas);
                IdentCoord(&xpes, &ypes);

                                if(cont_caixas == 0){

                                            printf("\n\aParabens! Ganhastes o jogo!\n");
                                            exit_screen=TRUE;
                                            screen_state=MAINMENU;
                                                        }


                                andar(&xpes, &ypes);

                                }

                ///DRAW
                draw_sprite(buffer, fundo, 0, 0);

                draw_sprite(buffer, caixa_ativado, 700, 450);

                Desenhar_Mapa(buffer, Alvo, Solo, Parede, &cont_caixas);

                ///if mais de ativado
                for(i=0; i<7;i++)
                for(j=(i+1);j<8; j++){
                    if(ativacao[i]&&ativacao[j]){
                                                b_caixa->ativado = FALSE;
                                                b_alvo->ativado = FALSE;
                                                b_parede->ativado = FALSE;
                                                b_heroi->ativado = FALSE;
                                                b_lixo->ativado = FALSE;
                                                b_salvar->ativado = FALSE;
                                                b_sair->ativado = FALSE;
                                                b_playp->ativado = FALSE;
                                                }
                                        }
                ///acoes dos botoes
                Button_action(buffer, b_caixa, b_alvo, b_parede, b_heroi, b_lixo, b_salvar, b_sair, b_playp, &playp_bot, "Mapas Salvos/Mapa Ram.txt", &exit_screen);
                ativacao[0]=b_caixa->ativado;ativacao[1]=b_alvo->ativado;ativacao[2]=b_parede->ativado;ativacao[3]=b_heroi->ativado;ativacao[4]=b_lixo->ativado;ativacao[5]=b_salvar->ativado;ativacao[6]=b_sair->ativado;ativacao[7]=b_playp->ativado;

                ///botoes
                button_draw(b_caixa, buffer);
                button_draw(b_alvo, buffer);
                button_draw(b_parede, buffer);
                button_draw(b_heroi, buffer);
                button_draw(b_lixo, buffer);
                button_draw(b_salvar, buffer);
                button_draw(b_sair, buffer);
                button_draw(b_playp, buffer);

                ///objetos nos botoes hahaha
                draw_sprite(buffer, Caixa, pos_bot1x[0]+7, pos_bot1y[0]+6);
                draw_sprite(buffer, Alvo, pos_bot1x[1]+8, pos_bot1y[1]+6);
                draw_sprite(buffer, Parede, pos_bot1x[2]+5, pos_bot1y[2]+6);
                draw_sprite(buffer, Heroi, pos_bot1x[3]+8, pos_bot1y[3]+6);
                draw_sprite(buffer, lixo_bot, pos_bot1x[4]+8, pos_bot1y[4]+6);
                draw_sprite(buffer, salvar_bot, pos_bot1x[5]+8, pos_bot1y[5]+6);
                draw_sprite(buffer, sair_bot, pos_bot1x[6]+8, pos_bot1y[6]+6);
                draw_sprite(buffer, playp_bot, pos_bot1x[7]+8, pos_bot1y[7]+6);

                ///mouse e tela
                draw_sprite(buffer, cursor, mouse_x, mouse_y);
                draw_sprite(screen, buffer, 0, 0);
                clear_to_color(buffer, makecol(255,255,255));
                clear(buffer);

                ticks--;
  	}
  }

  ///DESTRUCTION
  //botoes
  destroy_button(b_caixa);
  destroy_button(b_alvo);
  destroy_button(b_parede);
  destroy_button(b_heroi);
  destroy_button(b_lixo);
  destroy_button(b_salvar);
  destroy_button(b_sair);
  destroy_button(b_playp);

  //sons
  destroy_sample(click);
  if(musica)
    destroy_sample(musica);

  //botao editor
  destroy_bitmap(editor_bot);
  destroy_bitmap(caixa_ativado);

  //objetos
  destroy_bitmap(Alvo);
  destroy_bitmap(Parede);
  destroy_bitmap(Solo);
  //bitmaps
  destroy_bitmap(lixo_bot);
  destroy_bitmap(salvar_bot);
  destroy_bitmap(sair_bot);
  destroy_bitmap(playp_bot);
  //highlights
  destroy_bitmap(ok_highlight);
  destroy_bitmap(lixo_highlight);
  destroy_bitmap(buffer);
  //fundo
  destroy_bitmap(fundo);
  destroy_bitmap(fundo_mapa);
}
END_OF_FUNCTION(editor);

void select_race(){

  int exit_screen = FALSE;

  ///SONS
  SAMPLE* click2 = load_sample("Sons/click2.wav");
  SAMPLE* musica = load_sample("Soundtracks/10-scbw-radiofreezerg.wav");

  ///BITMAPS
	///fundo
    BITMAP* fundo_selecionar_raca = load_bitmap("Fundo/fundo_selecionar_raca.bmp", NULL);
    BITMAP* fundo = load_bitmap("Fundo/fundo_char.bmp", NULL);
	///mouse
  cursor = load_bitmap("Sprites/cursor_principal.bmp", NULL);
  ///objetos nos botoes
  BITMAP* terran_bot = load_bitmap("Sprites/botoes/terran_select.bmp", NULL);
  BITMAP* protoss_bot = load_bitmap("Sprites/botoes/protoss_select.bmp", NULL);
  BITMAP* zerg_bot = load_bitmap("Sprites/botoes/zerg_select.bmp", NULL);

	///highlitght
  BITMAP* terran_highlight = load_bitmap("Sprites/botoes/terran_highlight.bmp", NULL);
  BITMAP* protoss_highlight = load_bitmap("Sprites/botoes/protoss_highlight.bmp", NULL);
  BITMAP* zerg_highlight = load_bitmap("Sprites/botoes/zerg_highlight.bmp", NULL);
	///buffer
  BITMAP* buffer = create_bitmap(SCREEN_W, SCREEN_H);

  ///BOTÕES
  Button* b_terran = create_button(terran_bot, terran_highlight, click2, 130, (SCREEN_W/2)-200);
  Button* b_protoss = create_button(protoss_bot, protoss_highlight, click2, 130+(SCREEN_H/3), (SCREEN_W/2)-200);
  Button* b_zerg = create_button(zerg_bot, zerg_highlight, click2, 130+2*(SCREEN_H/3), (SCREEN_W/2)-200);

	play_sample(musica, 255, 128, 1000, TRUE);

  ///GAME LOOP
  while(!exit_program && !exit_screen)
  {
        while(ticks > 0 && !exit_program && !exit_screen){
    //INPUT
    keyboard_input();

    if(key[KEY_ESC])
     fecha_programa();

	///input botoes
     button_input(b_terran);
     button_input(b_protoss);
     button_input(b_zerg);

    //UPDATE

	if(b_terran->ativado){
        b_terran->ativado=FALSE;
		Race=1;
		Heroi = load_bitmap("Sprites/Objetos/terran_down.bmp", NULL);
		Caixa = load_bitmap("Sprites/Objetos/caixa_terran.bmp", NULL);
        screen_state=MAINMENU;
        exit_screen= TRUE;

    }

	if(b_protoss->ativado){
        b_protoss->ativado =FALSE;
		Race=2;
		Heroi = load_bitmap("Sprites/Objetos/protoss_down.bmp", NULL);
		Caixa = load_bitmap("Sprites/Objetos/caixa_protoss.bmp", NULL);
        screen_state= MAINMENU;
        exit_screen= TRUE;
	}

	if(b_zerg->ativado){
        b_zerg->ativado=FALSE;
		Race=3;
		Heroi = load_bitmap("Sprites/Objetos/zerg_down.bmp", NULL);
		Caixa = load_bitmap("Sprites/Objetos/caixa_zerg.bmp", NULL);
        screen_state=MAINMENU;
        exit_screen= TRUE;
	}

    //DRAW
	draw_sprite(buffer, fundo, 0, 0);
    draw_sprite(buffer, fundo_selecionar_raca, 100, SCREEN_W/2-335);

	///botoes
    button_draw(b_terran, buffer);
    button_draw(b_protoss, buffer);
    button_draw(b_zerg, buffer);

	///mouse e tela
    draw_sprite(buffer, cursor, mouse_x, mouse_y);
    draw_sprite(screen, buffer, 0, 0);
    clear_to_color(buffer, makecol(255,255,255));
    clear(buffer);

    ticks--;
  	}
  }

  ///DESTRUCTION

  //botoes
  destroy_button(b_terran);
  destroy_button(b_protoss);
  destroy_button(b_zerg);

  //sons
  destroy_sample(click2);
  destroy_sample(musica);

  //botoes
  destroy_bitmap(terran_bot);
  destroy_bitmap(protoss_bot);
  destroy_bitmap(zerg_bot);
  //highlights
  destroy_bitmap(protoss_highlight);
  destroy_bitmap(terran_highlight);
  destroy_bitmap(zerg_highlight);
  destroy_bitmap(buffer);
  //fundo
  destroy_bitmap(fundo);
  destroy_bitmap(fundo_selecionar_raca);
}
END_OF_FUNCTION(select_race);

void password()
{
    int exit_screen =FALSE;
    int i=0, j=0, digitos[6], password;

    SAMPLE* click2 = load_sample("Sons/click2.wav");
    SAMPLE* click = load_sample("Sons/click.wav");
    SAMPLE* click_del= load_sample("Sons/click_del.wav");
    SAMPLE* falha = load_sample("Sons/falha.wav");
    SAMPLE* correto = load_sample("Sons/correto.wav");

    BITMAP* img_password= load_bitmap("Fundo/fundo_password.bmp", NULL);
    BITMAP* sair = load_bitmap("Sprites/sair.bmp", NULL);
    BITMAP* sair_h = load_bitmap("Sprites/sair_h.bmp", NULL);
    BITMAP* img_bola = load_bitmap("Sprites/p_bola.bmp", NULL);
    BITMAP* img_tri = load_bitmap("Sprites/p_tri.bmp", NULL);
    BITMAP* img_x = load_bitmap("Sprites/p_x.bmp", NULL);
    BITMAP* img_quad = load_bitmap("Sprites/p_quad.bmp", NULL);
    BITMAP* img_del = load_bitmap("Sprites/p_del.bmp", NULL);
    BITMAP* img_confirma = load_bitmap("Sprites/p_confirma.bmp", NULL);
    BITMAP* img_highlight = load_bitmap("Sprites/p_highlight.bmp", NULL);
    BITMAP* img_grade = load_bitmap("Sprites/p_grade.bmp",NULL);
    BITMAP* img_confirma_highlight= load_bitmap("Sprites/p_confirma_highlight.bmp",NULL);
    BITMAP* img_fundo = load_bitmap("Fundo/fundo_char.bmp",NULL);
    BITMAP* img_del_highlight= load_bitmap ("Sprites/p_del_highlight.bmp", NULL);

    cursor= load_bitmap("Sprites/cursor_principal.bmp" ,NULL);

    BITMAP* buffer = create_bitmap(SCREEN_W,SCREEN_H);

    Button* b_voltar = create_button(sair, sair_h, click2, SCREEN_W/2-35, 575);
    Button* b_bola = create_button(img_bola, img_highlight, click2, SCREEN_W/2+40, 225);
    Button* b_quad = create_button(img_quad, img_highlight, click2, SCREEN_W/2-120,225);
    Button* b_x = create_button(img_x, img_highlight, click2, SCREEN_W/2-35, 300);
    Button* b_tri = create_button(img_tri, img_highlight, click2, SCREEN_W/2-35, 150);
    Button* b_del = create_button(img_del, img_del_highlight, click2, 730 ,490);
    Button* b_confirma = create_button(img_confirma, img_confirma_highlight, click2, SCREEN_W/2-30, 400);

    for(j=0;j<5;j++) digitos[j]=0;

    while(!exit_program && !exit_screen)
    {
        while(ticks>0 && !exit_program && !exit_screen)
            {
            password=0;
            if(key[KEY_ESC])
                    fecha_programa();

            ///INPUT

            button_input(b_bola);
            button_input(b_quad);
            button_input(b_tri);
            button_input(b_x);
            button_input(b_del);
            button_input(b_confirma);
            button_input(b_voltar);

            ///FUNDO

            draw_sprite(buffer, img_fundo,0,0);
            draw_sprite(buffer, img_password, 127, 50);
            //textout_centre_ex(buffer, starcraft_46, "P A S S W O R D", SCREEN_W/2, 50, makecol(0, 0, 0), -1 );

            button_draw(b_bola,buffer);
            button_draw(b_quad,buffer);
            button_draw(b_x,buffer);
            button_draw(b_tri,buffer);
            button_draw(b_del,buffer);
            button_draw(b_confirma,buffer);
            button_draw(b_voltar,buffer);

                ///UPDATE
                draw_sprite(buffer,img_grade, SCREEN_W/2-253, 473);

                if(b_x->ativado){
                                b_x->ativado=FALSE;
                                    if(i<6){
                                            play_sample(click,50,128,1000,FALSE);
                                            digitos[i]=1;
                                            i++;
                                            }
                                }

                else if(b_quad->ativado){
                                        b_quad->ativado=FALSE;
                                        if(i<6){
                                                play_sample(click,50,128,1000,FALSE);
                                                digitos[i]=2;
                                                i++;
                                                    }
                                        }

                else if(b_bola->ativado){
                                        b_bola->ativado=FALSE;
                                        if(i<6){
                                                play_sample(click,50,128,1000,FALSE);
                                                digitos[i]=3;
                                                i++;
                                                    }
                                        }

                else if(b_tri->ativado){
                                        b_tri->ativado=FALSE;
                                        if(i<6){
                                                play_sample(click,50,128,1000,FALSE);
                                                digitos[i]=4;
                                                i++;
                                                }
                                        }

                else if(b_del->ativado){
                                            play_sample(click_del, 255,128,1000,FALSE);
                                            b_del->ativado=FALSE;
                                            digitos[i-1]=0;
                                            if(i) i--;
                                        }

                else if(b_voltar->ativado){
                                            b_voltar->ativado=FALSE;
                                            exit_screen=TRUE;
                                            screen_state=PROGRESSO;
                                            }

                else if(b_confirma->ativado){
                                            b_confirma->ativado=FALSE;
                                            if(i==6)
                                                {
                                                for(j=0;j<6;j++) password=password+digitos[j]*pow(10,(5-j));
                                                printf("%d\n",password);
                                                nivel= verifica_password(password); ///Vai retornar valor diferente de zero
                                                                                    ///quando o password for correto.
                                                if(nivel){
                                                        play_sample(correto, 255, 128, 1000,FALSE);
                                                        rest (1000);
                                                        exit_screen=TRUE;
                                                        screen_state=JOGAR;
                                                        Libera_Mapa();
                                                        }
                                                else play_sample(falha, 255, 128, 1000,FALSE);

                                                }
                                                else play_sample(falha, 255, 128, 1000,FALSE);
                                            }


                ///DESENHAR SENHA APRESENTADA
                for (j=0;j<i;j++){
                                    if(digitos[j]==1)
                                        draw_sprite(buffer,img_x, SCREEN_W/2-240+80*j, 480);

                                    else if(digitos[j]==2)
                                        draw_sprite(buffer,img_quad, SCREEN_W/2-240+80*j, 480);

                                    else if(digitos[j]==3)
                                        draw_sprite(buffer,img_bola, SCREEN_W/2-240+80*j, 480);

                                    else if(digitos[j]==4)
                                        draw_sprite(buffer,img_tri, SCREEN_W/2-240+80*j, 480);
                                }

            draw_sprite(buffer, cursor, mouse_x, mouse_y);

            draw_sprite(screen,buffer,0,0);

            clear(buffer);

            ticks--;
            }
    }

///DESTROY BITMAPS
destroy_bitmap(buffer);
destroy_bitmap(img_password);
destroy_bitmap(img_bola);
destroy_bitmap(img_tri);
destroy_bitmap(img_x);
destroy_bitmap(img_quad);
destroy_bitmap(img_del);
destroy_bitmap(img_confirma);
destroy_bitmap(img_highlight);
destroy_bitmap(img_grade);
destroy_bitmap(img_fundo);
destroy_bitmap(img_confirma_highlight);
destroy_bitmap(img_del_highlight);
destroy_bitmap(sair);
destroy_bitmap(sair_h);

///DESTROY BOTÕES
destroy_button(b_bola);
destroy_button(b_quad);
destroy_button(b_tri);
destroy_button(b_x);
destroy_button(b_del);
destroy_button(b_confirma);
destroy_button(b_voltar);

destroy_sample(click);
destroy_sample(click_del);
destroy_sample(click2);
destroy_sample(falha);
destroy_sample(correto);
}
END_OF_FUNCTION(password);

int verifica_password(int senha_tentativa)
{
    int cont,senha_real;
    FILE *p;
    ///DIFICULDADE: 1- FÁCIL / 2- MÉDIO / 3- DIFÍCIL
    if(dificuldade==1)
                    p= fopen("passwords/password_facil.txt","r");

    else if(dificuldade==2)
                    p= fopen("passwords/password_medio.txt","r");

    else if(dificuldade==3)
                    p= fopen("passwords/password_dificil.txt","r");

    if(p){
            for(cont=1;cont<3;cont++)
            {
                fscanf(p,"%d",&senha_real);

                if(senha_tentativa==senha_real){
                                        fclose(p);
                                        return cont;
                                                }

            }
        }

    fclose(p);
    return 0;
}
END_OF_FUNCTION(verifica_password);

void jogar()
{
    int digitos_password[6];

    ///VARIÁVEIS
    int xpes = 0, ypes = 0, cont_caixas;

    ticks=0;
    timer=0;
    int inicializador=0;
    int x, y, i, j, exit_screen= FALSE;
    cont_passos=0;

    FONT* starcraft = load_font("Fontes/starcraft_14.pcx", NULL, NULL);
    FONT* starcraft_maior = load_font("Fontes/starcraft_52.pcx", NULL, NULL);

    Libera_Mapa();
    sprintf(nome_do_arq, "Mapas Salvos/mapa_%d_%d.txt",dificuldade, nivel);
    mapa_j = Carregar_Mapa(nome_do_arq);

    ///SONS

    SAMPLE* click = load_sample("Sons/click.wav");
    SAMPLE* click2 = load_sample("Sons/click2.wav");

   // FILE* p;

    ///BITMAPS ESTRUTURA
        ///JOGO
        BITMAP* Alvo = load_bitmap("Sprites/Objetos/acerto.bmp", NULL);
        BITMAP* Parede = load_bitmap("Sprites/Objetos/muro.bmp", NULL);
        BITMAP* Solo = load_bitmap("Sprites/Objetos/livre.bmp", NULL);
        BITMAP* fundo_mapa= load_bitmap("Sprites/mapa_fundo.bmp",NULL);

        ///PASSWORD
        BITMAP* p_bola = load_bitmap("Sprites/p_bola.bmp", NULL);
        BITMAP* p_tri = load_bitmap("Sprites/p_tri.bmp", NULL);
        BITMAP* p_quad = load_bitmap("Sprites/p_quad.bmp", NULL);
        BITMAP* p_x = load_bitmap("Sprites/p_x.bmp", NULL);

        ///ESTRUTURA
        BITMAP* fundo_vitoria;
        BITMAP* botao_ok = load_bitmap("Sprites/botao2_dificuldade.bmp", NULL);
        BITMAP* fundo_jogar = load_bitmap("Fundo/fundo_jogar.bmp",NULL);
        BITMAP* botao_jogar= load_bitmap("Sprites/botao_jogar.bmp",NULL);
        BITMAP* botao_jogar_h= load_bitmap("Sprites/botao_jogar_h.bmp",NULL);
        BITMAP* buffer = create_bitmap(SCREEN_W,SCREEN_H);

    ///SELECIONANDO MOUSE (SEGUNDO A RAÇA)
    if(Race==1){
                cursor=load_bitmap("Sprites/cursor_terran.bmp", NULL);
                fundo_vitoria= load_bitmap("Fundo/fundo_vitoria_terran.bmp", NULL);
                }
    else if(Race==2){
                cursor= load_bitmap("Sprites/cursor_protoss.bmp", NULL);
                fundo_vitoria= load_bitmap("Fundo/fundo_vitoria_protoss.bmp", NULL);
                    }
    else if(Race==3){
                cursor=load_bitmap("Sprites/cursor_zerg.bmp", NULL);
                fundo_vitoria= load_bitmap("Fundo/fundo_vitoria_zerg.bmp", NULL);
                    }

    Button* b_reiniciar = create_button(botao_jogar, botao_jogar_h, click, 689 ,200);
    Button* b_sair = create_button(botao_jogar, botao_jogar_h, click, 689 , 270);

    while(!exit_screen && !exit_program){

            keyboard_input();

            if(key[KEY_ESC])
                    fecha_programa();
                        ///UPDATE

            if(inicializador==0){
                        ContaCaixas(&cont_caixas);
                        inicializador++;
                            }
            if(inicializador>0){
                        Desenhar_Mapa(buffer, Alvo, Solo, Parede, &cont_caixas);
                        IdentCoord(&xpes, &ypes);

                                if(cont_caixas == 0){

                                        if(dificuldade){


                                                    while(!mouse_b==1){
                                                                        draw_sprite(buffer, fundo_vitoria, 0, 0);

                                                                            if (nivel<10)
                                                                            {
                                                                                textprintf_centre_ex(buffer, starcraft, SCREEN_W/2, SCREEN_H/2-75, makecol(255, 255, 255), -1, "Nivel: %d", nivel+1);
                                                                                draw_sprite(buffer, botao_ok, SCREEN_W/2-175, SCREEN_H/2-35);
                                                                                textout_centre_ex(buffer, starcraft,"CLIQUE PARA CONTINUAR", SCREEN_W/2, SCREEN_H/2 ,makecol(255,255,255),-1);
                                                                            }

                                                                            if (nivel==10)
                                                                            {
                                                                                textout_centre_ex(buffer, starcraft_maior, "THE END", SCREEN_W/2, SCREEN_H/2-200 ,makecol(255,255,255),-1);
                                                                                textout_centre_ex(buffer, starcraft,"APERTE ENTER", SCREEN_W/2, SCREEN_H/2 ,makecol(255,255,255),-1);
                                                                            }
                                                                            draw_sprite(buffer, cursor, mouse_x, mouse_y);
                                                                            draw_sprite(screen, buffer, 0 , 0);
                                                                            }
                                                            mouse_b=0;
                                                            if(nivel<10){
                                                                nivel++;
                                                                exit_screen=TRUE;
                                                                screen_state=JOGAR;
                                                            }
                                                            if(nivel==10){
                                                                exit_screen=TRUE;
                                                                screen_state=MAINMENU;
                                                                            }
                                                                    }
                                                        if(!dificuldade)
                                                                        {
                                                                        exit_screen=TRUE;
                                                                        screen_state=MAINMENU;
                                                                        }


                                                        }
                                andar(&xpes, &ypes);

                            }

            draw_sprite(buffer,fundo_jogar,0,0);

            ///BOTÕES INPUT
            button_input(b_reiniciar);
            button_input(b_sair);

            ///BOTÕES DRAW
            button_draw(b_reiniciar,buffer);
            button_draw(b_sair,buffer);

            if(b_sair->ativado){
                                    screen_state=MAINMENU;
                                    exit_screen = TRUE;
                                    b_sair->ativado = FALSE;
                                }
            if(b_reiniciar->ativado)
                                {
                                play_sample(click, 100, 255, 255, FALSE);
                                Libera_Mapa();
                                mapa_j = Carregar_Mapa(nome_do_arq);
                                b_reiniciar->ativado=FALSE;
                                timer=0;
                                cont_passos=0;
                                }
            ///TEXT
            textprintf_centre_ex(buffer, starcraft, 773, 130, makecol(255, 255, 255),-1,"%d PASSOS", cont_passos);
            textprintf_centre_ex(buffer, starcraft, 773, 165, makecol(255, 255, 255),-1,"Tempo: %.2d:%.2d",timer/60,timer%60);
            textout_centre_ex(buffer, starcraft,"REINICIAR", 773, 218,makecol(255,255,255),-1);
            textout_centre_ex(buffer, starcraft,"SAIR", 773, 288,makecol(255,255,255),-1);

            Desenhar_Mapa(buffer, Alvo, Solo, Parede, &cont_caixas);

            draw_sprite(buffer, cursor, mouse_x, mouse_y);
            draw_sprite(screen, buffer, 0, 0);

            clear_to_color(buffer, makecol(255,255,255));
            clear(buffer);
                                        }

    ///DESTRUCTIONS

    destroy_font(starcraft);
    destroy_font(starcraft_maior);

    destroy_button (b_reiniciar);
    destroy_button (b_sair);

    destroy_bitmap(Alvo);
    destroy_bitmap(Parede);
    destroy_bitmap(Solo);
    destroy_bitmap(fundo_mapa);
    destroy_bitmap(botao_ok);
    if(fundo_vitoria)
        destroy_bitmap(fundo_vitoria);
    destroy_bitmap(p_tri);
    destroy_bitmap(p_bola);
    destroy_bitmap(p_quad);
    destroy_bitmap(p_x);
    destroy_bitmap(fundo_jogar);
    destroy_bitmap(botao_jogar);
    destroy_bitmap(botao_jogar_h);
    destroy_bitmap(buffer);

    destroy_sample(click);
    destroy_sample(click2);
}
END_OF_FUNCTION(jogar);

void progresso()
{
int exit_screen=FALSE;

FONT* starcraft_16 = load_font("Fontes/starcraft_22.pcx", NULL,NULL);

SAMPLE* click = load_sample("Sons/click2.wav");

///BITMAPS
BITMAP* fundo = load_bitmap("Fundo/fundo_char.bmp", NULL);
BITMAP* sair = load_bitmap("Sprites/sair.bmp", NULL);
BITMAP* sair_h = load_bitmap("Sprites/sair_h.bmp", NULL);
BITMAP* botao = load_bitmap("Sprites/botao2_dificuldade.bmp",NULL);
BITMAP* botao_h = load_bitmap("Sprites/botao2_dificuldade_h.bmp",NULL);
BITMAP* buffer = create_bitmap(SCREEN_W, SCREEN_H);
BITMAP* fundo_progresso = load_bitmap("Fundo/fundo_progresso.bmp", NULL);

cursor = load_bitmap("Sprites/cursor_principal.bmp",NULL);

///BOTÕES
Button* b_voltar = create_button (sair, sair_h, click, SCREEN_W/2-35, 575);
Button* b_novojogo = create_button (botao, botao_h, click, SCREEN_W/2-160, SCREEN_H/2-120);
Button* b_continuar = create_button(botao, botao_h, click, SCREEN_W/2-160, SCREEN_H/2+85);

    while(!exit_screen && !exit_program){

            if(key[KEY_ESC])
                    fecha_programa();

            draw_sprite(buffer,fundo,0,0);

            ///DRAW SPRITES
            draw_sprite(buffer, fundo,0 ,0 );
            draw_sprite(buffer, fundo_progresso, SCREEN_W/2-302, 50);

            ///BOTÕES INPUT
            button_input(b_novojogo);
            button_input(b_continuar);
            button_input (b_voltar);
            ///BOTÕES DRAW
            button_draw(b_novojogo,buffer);
            button_draw(b_continuar,buffer);
            button_draw (b_voltar, buffer);
            ///TEXT
            textout_centre_ex(buffer, starcraft_16, "NOVO JOGO", SCREEN_W/2, SCREEN_H/2-95, makecol(255, 255, 255), -1 );
            textout_centre_ex(buffer, starcraft_16, "CONTINUAR", SCREEN_W/2, SCREEN_H/2+110, makecol(255, 255, 255), -1 );

            draw_sprite(buffer, cursor, mouse_x, mouse_y);
            draw_sprite(screen,buffer,0,0);

            if(b_novojogo->ativado){
                    nivel=1;
                    exit_screen=TRUE;
                    screen_state= JOGAR;
                    b_novojogo->ativado = FALSE;
                                    }

            if(b_continuar->ativado){
                    exit_screen=TRUE;
                    screen_state=PASSWORD;
                    b_continuar->ativado = FALSE;
                                    }

            if(b_voltar->ativado){
                                exit_screen = TRUE;
                                screen_state= SELECIONAR_DIFICULDADE;
                                b_voltar->ativado = FALSE;
                                }



            clear(buffer);
                                        }
destroy_bitmap(fundo);
destroy_bitmap(buffer);
destroy_bitmap(sair);
destroy_bitmap(sair_h);
destroy_bitmap(botao);
destroy_bitmap(botao_h);
destroy_bitmap(fundo_progresso);

destroy_button(b_voltar);
destroy_button(b_continuar);
destroy_button(b_novojogo);

destroy_font(starcraft_16);

destroy_sample(click);
}
END_OF_FUNCTION(progresso);

void salvar_minimapa()
{
    char nome[40];
    int i, j, qnt_mapas ;

    BITMAP* r_acerto = load_bitmap("Sprites/r_acerto.bmp", NULL);
    BITMAP* r_muro = load_bitmap("Sprites/r_muro.bmp", NULL);
    BITMAP* r_livre = load_bitmap("Sprites/r_livre.bmp", NULL);
    BITMAP* r_heroi = load_bitmap("Sprites/r_heroi.bmp", NULL);
    BITMAP* r_caixa = load_bitmap("Sprites/r_caixa.bmp", NULL);

    FILE *fcont = fopen("Mapas Salvos/ContMap.txt", "r");
    fscanf(fcont, "%d", &qnt_mapas);
    fclose(fcont);

    BITMAP* minimapa= create_bitmap(20*linhas, 20*colunas);

    for(i=0;i<linhas;i++){
            for(j=0;j<colunas;j++){
                                    if(mapa_j[i][j] == acerto){
                                        draw_sprite(minimapa, r_livre, j*20, i*20);
                                        draw_sprite(minimapa, r_acerto, j*20, i*20);
                                                            }

                                    else if(mapa_j[i][j] == livre)
                                        draw_sprite(minimapa, r_livre, j*20, i*20);

                                    else if(mapa_j[i][j] == muro){
                                        draw_sprite(minimapa, r_livre, j*20, i*20);
                                        draw_sprite(minimapa, r_muro, j*20, i*20);
                                                                }

                                    else if(mapa_j[i][j] == muro_petreo){
                                        draw_sprite(minimapa, r_livre, j*20, i*20);
                                        draw_sprite(minimapa, r_muro, j*20, i*20);
                                                                        }
                                    else if(mapa_j[i][j] == pes){
                                        draw_sprite(minimapa, r_livre, j*20, i*20);
                                        draw_sprite(minimapa, r_heroi, j*20, i*20);
                                                                }
                                    else if(mapa_j[i][j] == caixa){
                                        draw_sprite(minimapa, r_livre, j*20, i*20);
                                        draw_sprite(minimapa, r_caixa, j*20, i*20);
                                                                }
                                   else if(mapa_j[i][j] == acerto_pes){
                                        draw_sprite(minimapa, r_livre, j*20, i*20);
                                        draw_sprite(minimapa, r_heroi, j*20, i*20);
                                                                    }

                                   else if(mapa_j[i][j] == acerto_caixa){
                                                                        draw_sprite(minimapa, r_caixa, j*20, i*20);
                                                                        draw_sprite(minimapa, r_acerto, j*20, i*20);

                                                                        }
                                    }
                            }

sprintf(nome, "Sprites/mapa_0_%d.bmp",qnt_mapas+1);
save_bitmap(nome, minimapa, NULL);

destroy_bitmap(r_acerto);
destroy_bitmap(r_caixa);
destroy_bitmap(r_heroi);
destroy_bitmap(r_livre);
destroy_bitmap(r_muro);
destroy_bitmap(minimapa);
}
END_OF_FUNCTION(salvar_minimapa);

void f_mapas_salvos()
{
    char nome_minimapa[25];
    int exit_screen=FALSE, qnt_mapas, pag=1, novo_mapa=1;

    FILE* p = fopen("Mapas Salvos/ContMap.txt", "r");
    fscanf(p, "%d", &qnt_mapas);
    fclose(p);

    FONT* starcraft = load_font("Fontes/starcraft_22.pcx", NULL,NULL);

    cursor = load_bitmap ("Sprites/cursor_principal.bmp", NULL);

    SAMPLE* click_move = load_sample("Sons/click2.wav");
    SAMPLE* click_select = load_sample("Sons/click.wav");
    SAMPLE* correto = load_sample ("Sons/correto.wav");

    BITMAP* sair = load_bitmap("Sprites/sair.bmp", NULL);
    BITMAP* sair_h = load_bitmap("Sprites/sair_h.bmp", NULL);
    BITMAP* confirmar = load_bitmap("Sprites/botao2_dificuldade.bmp", NULL);
    BITMAP* confirmar_h = load_bitmap("Sprites/botao2_dificuldade_h.bmp", NULL);
    BITMAP* buffer = create_bitmap(SCREEN_W, SCREEN_H);
    BITMAP* fundo_mapas_salvos = load_bitmap ("Fundo/fundo_mapas_salvos.bmp", NULL);
    BITMAP* fundo = load_bitmap("Fundo/fundo_char.bmp", NULL);
    BITMAP* prox = load_bitmap("Sprites/next.bmp", NULL);
    BITMAP* ant = load_bitmap("Sprites/prev.bmp", NULL);
    BITMAP* prox_h = load_bitmap("Sprites/next_highlight.bmp", NULL);
    BITMAP* ant_h = load_bitmap("Sprites/prev_highlight.bmp", NULL);
    BITMAP* minimapa;

    Button* b_next = create_button(prox, prox_h, click_move, 821, SCREEN_H/2-30);
    Button* b_prev = create_button(ant, ant_h, click_move, 25, SCREEN_H/2-30);
    Button* b_enter = create_button(confirmar, confirmar_h, click_move, SCREEN_W/2-165, 500);
    Button* b_sair = create_button (sair, sair_h, click_move, SCREEN_W/2-35, 575);

            while(!exit_screen && !exit_program)
                                {
                                if(key[KEY_ESC])
                                        fecha_programa();

                                draw_sprite(buffer, fundo, 0, 0);
                                draw_sprite(buffer, fundo_mapas_salvos, SCREEN_W/2-335, 50);

                                if(qnt_mapas-pag)
                                        {
                                        button_input(b_next);
                                        button_draw(b_next, buffer);
                                        }

                                if(pag-1)
                                        {
                                        button_input(b_prev);
                                        button_draw(b_prev, buffer);
                                        }

                                if(novo_mapa)
                                            {
                                            sprintf(nome_minimapa, "Sprites/mapa_0_%d.bmp",pag);
                                            minimapa = load_bitmap(nome_minimapa, NULL);
                                            novo_mapa--;
                                            }

                                draw_sprite(buffer, minimapa, 200, 200);

                                button_input(b_enter);
                                button_draw(b_enter, buffer);
                                textout_centre_ex(buffer, starcraft, "SELECIONAR", SCREEN_W/2, 525, makecol(255, 255, 255), -1 );

                                button_input(b_sair);
                                button_draw(b_sair, buffer);

                                if((b_next->ativado) && (qnt_mapas-pag) )
                                            {
                                            b_next->ativado= FALSE;
                                            play_sample(click_select, 255, 128, 1000, FALSE);
                                            novo_mapa++;
                                            pag++;
                                            }

                                if(b_enter->ativado)
                                            {
                                            b_enter->ativado= FALSE;
                                            play_sample(correto, 255, 128, 1000, FALSE);
                                            nivel=pag;
                                            exit_screen=TRUE;
                                            screen_state=JOGAR;
                                            }

                                if((b_prev->ativado) && (pag-1) )
                                            {
                                            b_prev->ativado= FALSE;
                                            play_sample(click_select, 255, 128, 1000, FALSE);
                                            novo_mapa++;
                                            pag--;
                                            }

                                if(b_sair->ativado)
                                            {
                                             b_sair->ativado=FALSE;
                                             screen_state= SELECIONAR_DIFICULDADE;
                                             exit_screen=TRUE;
                                            }

                                draw_sprite (buffer, cursor, mouse_x, mouse_y);
                                draw_sprite (screen, buffer, 0, 0);

                                clear(buffer);
                                }

destroy_font(starcraft);

destroy_button(b_sair);
destroy_button(b_next);
destroy_button(b_prev);
destroy_button(b_enter);

destroy_sample(click_move);
destroy_sample(click_select);
destroy_sample(correto);

destroy_bitmap(sair);
destroy_bitmap(sair_h);
destroy_bitmap(confirmar);
destroy_bitmap(confirmar_h);
destroy_bitmap(fundo_mapas_salvos);
destroy_bitmap(fundo);
if(minimapa)
    destroy_bitmap(minimapa);
destroy_bitmap(buffer);
destroy_bitmap(prox);
destroy_bitmap(prox_h);
destroy_bitmap(ant);
destroy_bitmap(ant_h);
}
END_OF_FUNCTION(f_mapas_salvos);

void f_selecionar_dificuldade()
{
    int dificuldade_aleatoria, k, exit_screen = FALSE;

    SAMPLE* click_move = load_sample("Sons/click2.wav");

    cursor = load_bitmap("Sprites/cursor_principal.bmp", NULL);

    FONT* starcraft = load_font("Fontes/starcraft_22.pcx", NULL,NULL);

    BITMAP* buffer = create_bitmap(SCREEN_W, SCREEN_H);
    BITMAP* fundo = load_bitmap("Fundo/fundo_char.bmp", NULL);

    BITMAP* sair = load_bitmap("Sprites/sair.bmp", NULL);
    BITMAP* sair_h = load_bitmap("Sprites/sair_h.bmp", NULL);
    BITMAP* botao_dificuldade = load_bitmap("Sprites/botao_dificuldade.bmp", NULL);
    BITMAP* botao_dificuldade_h = load_bitmap("Sprites/botao_dificuldade_h.bmp", NULL);
    BITMAP* botao2_dificuldade = load_bitmap("Sprites/botao2_dificuldade.bmp", NULL);
    BITMAP* botao2_dificuldade_h = load_bitmap("Sprites/botao2_dificuldade_h.bmp", NULL);

    Button* b_facil = create_button (botao_dificuldade, botao_dificuldade_h, click_move, 100, 175);
    Button* b_medio = create_button (botao_dificuldade, botao_dificuldade_h, click_move, 350, 175);
    Button* b_dificil = create_button (botao_dificuldade, botao_dificuldade_h, click_move, 600, 175);
    Button* b_mapas_salvos = create_button (botao2_dificuldade, botao2_dificuldade_h, click_move, SCREEN_W/2-165, 350);
    Button* b_gerador = create_button (botao2_dificuldade, botao2_dificuldade_h, click_move, SCREEN_W/2-165, 475);
    Button* b_voltar = create_button (sair, sair_h, click_move, SCREEN_W/2-35, 575);

    while(!exit_screen && !exit_program)
                                {

                                if(key[KEY_ESC])
                                        fecha_programa();

                                draw_sprite(buffer, fundo, 0, 0);

                                button_input(b_facil);
                                button_input(b_medio);
                                button_input(b_dificil);
                                button_input(b_mapas_salvos);
                                button_input(b_gerador);
                                button_input(b_voltar);

                                button_draw(b_facil, buffer);
                                button_draw(b_medio, buffer);
                                button_draw(b_dificil, buffer);
                                button_draw(b_mapas_salvos, buffer);
                                button_draw(b_gerador, buffer);
                                button_draw(b_voltar, buffer);

                                textout_centre_ex(buffer, starcraft, "FACIL", 200, 195, makecol(255, 255, 255), -1 );
                                textout_centre_ex(buffer, starcraft, "MEDIO", 450, 195, makecol(255, 255, 255), -1 );
                                textout_centre_ex(buffer, starcraft, "DIFICIL", 700, 195, makecol(255, 255, 255), -1 );
                                textout_centre_ex(buffer, starcraft, "MAPAS SALVOS", SCREEN_W/2, 375, makecol(255, 255, 255), -1 );
                                textout_centre_ex(buffer, starcraft, "ALEA JACTA EST", SCREEN_W/2, 500, makecol(255, 255, 255), -1 );

                                if(b_facil->ativado)
                                                    {
                                                    b_facil->ativado=FALSE;
                                                    dificuldade=1;
                                                    exit_screen = TRUE;
                                                    screen_state= PROGRESSO ;
                                                    }

                                if(b_medio->ativado)
                                                    {
                                                    b_medio->ativado=FALSE;
                                                    dificuldade=2;
                                                    exit_screen = TRUE;
                                                    screen_state= PROGRESSO ;
                                                    }

                                if(b_dificil->ativado)
                                                    {
                                                    b_dificil->ativado=FALSE;
                                                    dificuldade=3;
                                                    exit_screen = TRUE;
                                                    screen_state= PROGRESSO ;
                                                    }

                                if(b_mapas_salvos->ativado)
                                                    {
                                                    b_mapas_salvos->ativado=FALSE;
                                                    dificuldade=0;
                                                    exit_screen = TRUE;
                                                    screen_state= MAPAS_SALVOS;
                                                    }

                                if(b_voltar->ativado){
                                                    b_voltar->ativado=FALSE;
                                                    exit_screen = TRUE;
                                                    screen_state= MAINMENU;
                                                    }

                                if(b_gerador->ativado)
                                                    {
                                                    b_gerador->ativado=FALSE;
                                                    dificuldade=0;
                                                    nivel=0;
                                                    dificuldade_gerador= (rand()%3) + 1;
                                                    do
                                                    {
                                                        if(contador_g<5000)
                                                                {
                                                                gerar_mapa();
                                                                k=valida_mapa();
                                                                }
                                                        else
                                                                {
                                                                contador_g=0;
                                                                gerar_mapa();
                                                                k= valida_mapa();
                                                                }

                                                    } while(k==0);

                                                    imprime_mapa_gerado();
                                                    exit_screen = TRUE;
                                                    screen_state= JOGAR;
                                                    }

                                draw_sprite(buffer, cursor, mouse_x, mouse_y);

                                draw_sprite(screen, buffer, 0, 0);
                                clear(buffer);
                                }

    destroy_button (b_facil);
    destroy_button (b_medio);
    destroy_button (b_dificil);
    destroy_button (b_mapas_salvos);
    destroy_button (b_gerador);
    destroy_button  (b_voltar);

    destroy_sample(click_move);

    destroy_font(starcraft);

    destroy_bitmap(sair);
    destroy_bitmap(sair_h);
    destroy_bitmap(fundo);
    destroy_bitmap(buffer);
    destroy_bitmap(botao2_dificuldade);
    destroy_bitmap(botao_dificuldade_h);
    destroy_bitmap(botao_dificuldade);
    destroy_bitmap(botao2_dificuldade_h);
}
END_OF_FUNCTION(f_selecionar_dificuldade);
