#include "keyboard.h"
#include "screen.h"
#include "timer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

enum {
  KEY_ESC = 27,
  KEY_ENTER = 10,
  ARROW_UP = 119,
  ARROW_DOWN = 115,
  ARROW_LEFT = 97,
  ARROW_RIGHT = 100
};

struct Snakenode {
  int Nodex;
  int Nodey;
  struct Snakenode *next;
};

struct Jogador {
  char Nome[4];
  int pont;
};

struct Highscore {
  struct Jogador jogador;
  struct Highscore *next;
};

void printPlacar(int placar);
void printGameOver();
void AdicionarSnake(struct Snakenode **head, int x, int y);
void printSnake(struct Snakenode *head);
void LimparSnake(struct Snakenode *head);
void FreeSnake(struct Snakenode **head);
void MoveSnake(struct Snakenode **head, int x, int y);
void PrintMaca(int x, int y);
int ColisaoCorpo(struct Snakenode *head, int x, int y);
void ProxMaca(int *x, int *y);
void OrdenarLista(struct Highscore **cabeca, struct Jogador nick);
void EscreverLista(struct Highscore *cabeca, FILE *fptr);
void PrintarLista(struct Highscore *cabeca);
void FreeLista(struct Highscore **head);

int incX = 1, incY = 1;

int main() {
  struct Snakenode *head = NULL;
  static int ch = 0;
  int placar = 0;
  int dirX = 1, dirY = 0; // Inicialmente movendo para a direita
  FILE *fptr;
  struct Jogador player;
  printf("O jogo começará assim que digitar seu nick!\n");
  printf("Digite sua sigla de 3 letras: ");
  scanf("%s", player.Nome);

  screenInit(1);
  keyboardInit();
  timerInit(50);

  AdicionarSnake(&head, 34, 12);
  srand((unsigned int)time(NULL));
  int PosMacaX = rand() % 68 + 8, PosMacaY = rand() % 16 + 4;
  PrintMaca(PosMacaX, PosMacaY);
  screenUpdate();

  while (ch != KEY_ESC) {
    if (keyhit()) {
      ch = readch();
      // Atualizar direção com base na tecla de seta
      switch (ch) {
      case ARROW_UP:
        if (dirY != 1) {
          dirX = 0;
          dirY = -1;
        }
        break;
      case ARROW_DOWN:
        if (dirY != -1) {
          dirX = 0;
          dirY = 1;
        }
        break;
      case ARROW_LEFT:
        if (dirX != 1) {
          dirX = -1;
          dirY = 0;
        }
        break;
      case ARROW_RIGHT:
        if (dirX != -1) {
          dirX = 1;
          dirY = 0;
        }
        break;
      }
      screenUpdate();
    }

    if (timerTimeOver() == 1) {
      int newX = head->Nodex + dirX;
      int newY = head->Nodey + dirY;

      // Verifica colisão
      if (newX >= (MAXX) || newX <= MINX || newY >= MAXY || newY <= MINY) {
        break; // Colisão com a parede
      }

      if (ColisaoCorpo(head, newX, newY) == 1) {
        break; // Colisão com o próprio corpo
      }

      if (newX == PosMacaX && newY == PosMacaY) {
        AdicionarSnake(&head, PosMacaX, PosMacaY);
        ProxMaca(&PosMacaX, &PosMacaY);
        PrintMaca(PosMacaX, PosMacaY);
        placar++;
      }
      printPlacar(placar);
      // Mover a cobra
      LimparSnake(head); // Limpa a posição anterior
      MoveSnake(&head, newX, newY);
      printSnake(head); // Desenha a cobra na nova posição
      screenUpdate();
    }
  }

  FreeSnake(&head);
  keyboardDestroy();
  screenDestroy();
  player.pont = placar;
  fptr = (fopen("placar.txt", "a")); // começo placar
  fwrite(&player, sizeof(struct Jogador), 1, fptr);
  fclose(fptr);
  struct Highscore *lista = NULL;
  fptr = (fopen("placar.txt", "r"));
  while (fread(&player, sizeof(struct Jogador), 1, fptr) == 1) {
    OrdenarLista(&lista, player);
  }
  fclose(fptr);
  fptr = (fopen("placar.txt", "w"));
  EscreverLista(lista, fptr);
  fclose(fptr);
  PrintarLista(lista);
  FreeLista(&lista); // fim do placar
  printGameOver();
  timerDestroy();

  return 0;
}

void printGameOver() {
  screenSetColor(YELLOW, DARKGRAY);
  screenGotoxy(30, 10);
  printf("Game over!");
}

void printPlacar(int placar) {
  screenSetColor(YELLOW, DARKGRAY);
  screenGotoxy(35, 22);
  printf("Placar :");

  screenGotoxy(34, 23);
  printf("       ");
  screenGotoxy(39, 23);
  printf("%d", placar);
}

void AdicionarSnake(struct Snakenode **head, int x, int y) {
  if (*head == NULL) {
    *head = (struct Snakenode *)malloc(sizeof(struct Snakenode));
    (*head)->Nodex = x;
    (*head)->Nodey = y;
    (*head)->next = NULL;
  } else {
    struct Snakenode *n = *head;
    struct Snakenode *novo =
        (struct Snakenode *)malloc(sizeof(struct Snakenode));
    novo->Nodex = x;
    novo->Nodey = y;
    while (n->next != NULL) {
      n = n->next;
    }
    novo->next = NULL;
    n->next = novo;
  }
}

void printSnake(struct Snakenode *head) {
  struct Snakenode *n = head;
  while (n != NULL) {
    screenSetColor(GREEN, DARKGRAY);
    screenGotoxy(n->Nodex, n->Nodey);
    printf("X");
    n = n->next;
  }
}

void LimparSnake(struct Snakenode *head) {
  struct Snakenode *n = head;
  while (n != NULL) {
    screenGotoxy(n->Nodex, n->Nodey);
    printf(" ");
    n = n->next;
  }
}

void FreeSnake(struct Snakenode **head) {
  struct Snakenode *n = *head;
  while (n != NULL) {
    struct Snakenode *temp = n;
    n = n->next;
    free(temp);
  }
}

void MoveSnake(struct Snakenode **head, int x, int y) {
  struct Snakenode *newHead =
      (struct Snakenode *)malloc(sizeof(struct Snakenode));
  if (newHead == NULL) {
    // Tratamento de erro, se a alocação de memória falhar
    exit(1);
  }

  newHead->Nodex = x;
  newHead->Nodey = y;
  newHead->next = *head;
  *head = newHead;

  // Remover a última parte da cauda
  struct Snakenode *temp = *head;
  while (temp->next->next != NULL) {
    temp = temp->next;
  }

  free(temp->next);
  temp->next = NULL;
}

void PrintMaca(int x, int y) {
  screenSetColor(RED, DARKGRAY);
  screenGotoxy(x, y);
  printf("0");
}

int ColisaoCorpo(struct Snakenode *head, int x, int y) {
  struct Snakenode *n = head;
  while (n != NULL) {
    if (n->Nodex == x && n->Nodey == y) {
      return 1; // Colisão com o corpo
    }
    n = n->next;
  }
  return 0; // Sem colisão
}

void ProxMaca(int *x, int *y) {
  *x = rand() % 68 + 8;
  *y = rand() % 16 + 4;
}

void OrdenarLista(struct Highscore **cabeca, struct Jogador nick) {
  if (*cabeca == NULL) {
    *cabeca = (struct Highscore *)malloc(sizeof(struct Highscore));
    (*cabeca)->jogador = nick;
    (*cabeca)->next = NULL;
  } else {
    struct Highscore *n = *cabeca;
    struct Highscore *novo =
        (struct Highscore *)malloc(sizeof(struct Highscore));
    novo->jogador = nick;
    while (n->next != NULL && nick.pont < novo->next->jogador.pont) {
      n = n->next;
    }
    if (nick.pont > (*cabeca)->jogador.pont) {
      novo->next = *cabeca;
      *cabeca = novo;
    } else if (n->next == NULL) {
      novo->next = NULL;
      n->next = novo;
    } else {
      novo->next = n->next;
      n->next = novo;
    }
  }
}

void EscreverLista(struct Highscore *cabeca, FILE *fptr) {
  struct Highscore *n = cabeca;
  struct Jogador Ojogador;
  while (n != NULL) {
    Ojogador = n->jogador;
    if (fwrite(&Ojogador, sizeof(struct Jogador), 1, fptr) != 1) {
      break;
    }
    n = n->next;
  }
}

void PrintarLista(struct Highscore *cabeca) {
  struct Highscore *n = cabeca;
  struct Jogador Ojogador;
  int i = 1;
  while (n != NULL && (i < 4)) {
    printf("%d colocado!:\n", i);
    printf("Nome: %s\n", n->jogador.Nome);
    printf("Pontuação: %d\n", n->jogador.pont);
    n = n->next;
    i++;
  }
}

void FreeLista(struct Highscore **head) {
  struct Highscore *n = *head;
  while (n != NULL) {
    struct Highscore *temp = n;
    n = n->next;
    free(temp);
  }
}
