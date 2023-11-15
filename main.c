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

void printKey(int ch);
void printGameOver();
void AdicionarSnake(struct Snakenode **head, int x, int y);
void printSnake(struct Snakenode *head);
void LimparSnake(struct Snakenode *head);
void FreeSnake(struct Snakenode **head);
void MoveSnake(struct Snakenode **head, int x, int y);
void PrintMaca(int x, int y);

int incX = 1, incY = 1;

int main() {
  struct Snakenode *head = NULL;
  static int ch = 0;
  int dirX = 1, dirY = 0; // Inicialmente movendo para a direita
  screenInit(1);
  keyboardInit();
  timerInit(50);

  AdicionarSnake(&head, 34, 12);
  srand((unsigned int)time(NULL));
  int PosMacaX = rand() % 76 + 4, PosMacaY = rand() % 16 + 4;
  PrintMaca(PosMacaX, PosMacaY);
  screenUpdate();

  while (ch != KEY_ESC) {
    if (keyhit()) {
      ch = readch();
      // Atualizar direção com base na tecla de seta
      switch (ch) {
      case ARROW_UP:
        dirX = 0;
        dirY = -1;
        break;
      case ARROW_DOWN:
        dirX = 0;
        dirY = 1;
        break;
      case ARROW_LEFT:
        dirX = -1;
        dirY = 0;
        break;
      case ARROW_RIGHT:
        dirX = 1;
        dirY = 0;
        break;
      }
      printKey(ch);
      screenUpdate();
    }

    if (timerTimeOver() == 1) {
      int newX = head->Nodex + dirX;
      int newY = head->Nodey + dirY;

      // Verifica colisão
      if (newX >= (MAXX - strlen("Snake") - 1) || newX <= MINX + 1 ||
          newY >= MAXY - 1 || newY <= MINY + 1) {
        break; // Colisão com a parede
      }

      if (newX == PosMacaX && newY == PosMacaY) {
        AdicionarSnake(&head, PosMacaX, PosMacaY);
      }

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
  printGameOver();
  timerDestroy();

  return 0;
}

void printGameOver() {
  screenSetColor(YELLOW, DARKGRAY);
  screenGotoxy(30, 10);
  printf("Game over!");
}

void printKey(int ch) {
  screenSetColor(YELLOW, DARKGRAY);
  screenGotoxy(35, 22);
  printf("Key code :");

  screenGotoxy(34, 23);
  printf("            ");

  if (ch == 10)
    screenGotoxy(36, 23);
  else
    screenGotoxy(39, 23);

  printf("%d ", ch);
  while (keyhit()) {
    printf("%d ", readch());
  }
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
  free(*head);
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
