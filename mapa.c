// carregamento e validação do arquivo, desenho, validação de movimentos
/*
 *  mapa.c
int carregarMapa(const char *nomeArquivo, Jogo *j); /* 1 = ok, 0 = erro
int telaCabe(Jogo *j); /* ncurses: 1 se mapa + painel cabem
void desenharJogo(Jogo *j);
int movimentoValido(Jogo *j, int comando); */

#include<stdio.h>
#include<ncurses.h>
#include "jogo.h"

int carregarMapa(const char *nomeArquivo, Jogo *j)
{
    FILE *fp = fopen(nomeArquivo, "r");
    if(fp==NULL) return 0;

    int c;
    int i = 0, k = 0;

    while ((c = getc(fp)) != EOF && i<MAX_LINHAS && (k<MAX_COLUNAS || c=='\n')) {
        if(k==0 && c!='#') continue;
        if(c=='P') {
            j->linhaJ = i;
            j->colunaJ = k;
            c = '.';
        }
        if(c=='\n') {
            i++;
            j->colunas = k;
            k = 0;
            continue;
        }
        j->mapa[i][k] = c;
        k++;
    }
    fclose(fp);

    j->linhas = i;

    // contar os cristais
    if(c != EOF) return 0;
    return 1;
}

int telaCabe(Jogo *j)
{
    // void getmaxyx(WINDOW *win, int y, int x);  https://linux.die.net/man/3/getmaxyx
    // macro que coloca i = numero de linha k = numero de colunas: logo return o if verifica se cabe
    int i, k;
    getmaxyx(stdscr, i, k);
    return (j->linhas <= i && j->colunas <= k);
}
/*
 *  A cada turno, desenharJogo limpa a tela, desenha o mapa, desenha P na posição
do jogador sem alterar a matriz, e mostra a linha de estado (energia, cristais coletados/mínimo, turnos) e a
última mensagem. Como a tela é limpa a cada turno, uma mensagem impressa fora de desenharJogo some
antes de ser lida. Por isso, guarde a mensagem no campo mensagem do estado do jogo (Seção 8) e deixe
desenharJogo exibi-la. */
void desenharJogo(Jogo *j)
{
    int i, k;
    int posi = j->linhaJ;
	int posk = j->colunaJ;

    clear();


    for(i=0; i<j->linhas; i++) {
		for(k=0; k<j->colunas; k++) {
			if(i==posi && k==posk) {
				addch('P');
            } else {
                addch(j->mapa[i][k]);
            }
        }
        addch('\n');
    }
    printw("Energia: %d\n", j->energia);
    refresh();
}
