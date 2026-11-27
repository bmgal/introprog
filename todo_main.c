#include <ncurses.h>
#include <stdio.h>
#include <string.h>
#include "jogo.h"
#define ARQUIVO_MAPA "mapa1.txt"

int main(void) {
    Jogo jogo;
    int comando, resultado, jogarDeNovo;
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    do {

        if (!carregarMapa(ARQUIVO_MAPA, &jogo)) {
        endwin(); /* restaura o terminal ANTES do printf */
        printf("Erro ao carregar %s: %s.\n", ARQUIVO_MAPA, jogo.mensagem);
        return 1;
        }

        if (!telaCabe(&jogo)) {
        endwin();
        printf("Terminal pequeno para este mapa: aumente a janela.\n");
        return 1;
        }
        jogo.cristais = 0;
        jogo.turnos = 0;
        jogo.mensagem[0] = '\0';
        resultado = EM_ANDAMENTO;

            while (resultado == EM_ANDAMENTO) {
            desenharJogo(&jogo);
            comando = getch();
            jogo.mensagem[0] = '\0';

            if (comando == 'q' || comando == 'Q') {
            resultado = DESISTENCIA;
            } else if (movimentoValido(&jogo, comando)) {
                moverJogador(&jogo.linhaJ, &jogo.colunaJ, comando);
                jogo.energia--;
                jogo.turnos++;
                processarCasa(&jogo);

                if (chegouNaSaida(&jogo)) {
                    if (jogo.cristais >= jogo.minimoCristais) {
                    resultado = VITORIA;
                    } else {
                    strcpy(jogo.mensagem, "Cristais insuficientes para sair!");
                    }
                }

                if (resultado == EM_ANDAMENTO && jogo.energia <= 0) {
                resultado = DERROTA;
                }

                } else {
                strcpy(jogo.mensagem, "Movimento invalido!");
                }
            }

            jogarDeNovo = mostrarFimDeJogo(&jogo, resultado);
        } while (jogarDeNovo);

    endwin();
    return 0;
}

/* retorna 1 para jogar novamente e 0 para sair */
int mostrarFimDeJogo(Jogo *j, int resultado) {
    int tecla;
    clear();
    if (resultado == VITORIA) {
    mvprintw(2, 2, "VITORIA! Voce escapou da ilha.");
    } else if (resultado == DERROTA) {
    mvprintw(2, 2, "DERROTA - sua energia acabou.");
    } else {
    mvprintw(2, 2, "DESISTENCIA - voce abandonou a expedicao.");
    }
    mvprintw(4, 2, "Cristais: %d (minimo exigido: %d)",
    j->cristais, j->minimoCristais);
    mvprintw(5, 2, "Turnos: %d", j->turnos);
    mvprintw(6, 2, "Energia restante: %d", j->energia);
    mvprintw(8, 2, "[R] Jogar novamente [Q] Sair");
    refresh();
    do {
    tecla = getch();
    } while (tecla != 'r' && tecla != 'R' &&
    tecla != 'q' && tecla != 'Q');
    return tecla == 'r' || tecla == 'R';
}
