#include <ncurses.h>
#include <stdio.h>
#include "jogo.h"

int main(void)
{
    Jogo jogo = {0};
    int comando;

    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    if (!carregarMapa("mapa1.txt", &jogo)) {
        endwin();
        printf("Erro ao carregar mapa.\n");
        return 1;
    }

    if (!telaCabe(&jogo)) {
        endwin();
        printf("Terminal pequeno.\n");
        return 1;
    }

    do {
        desenharJogo(&jogo);
        comando = getch();

        switch (comando) {
            case KEY_UP:
                jogo.linhaJ--;
                break;
            case KEY_DOWN:
                jogo.linhaJ++;
                break;
            case KEY_LEFT:
                jogo.colunaJ--;
                break;
            case KEY_RIGHT:
                jogo.colunaJ++;
                break;
        }

    } while (comando != 'q');

    endwin();
    return 0;
}
