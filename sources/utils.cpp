#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "tree.h"
#include "utils.h"
#include "my_printf.h"

void clear() {
  system("rm -rf \"./img/\" && mkdir \"./img/\"");
  FILE *file = fopen("./log.html", "w");
  if (file != NULL) {
    fclose(file);
  }
}

void parseErrors(TreeErr error) {
  switch (error) {
    case SUCCESS:
      break;
    case CREATE_FAILED:
      colorPrintf(RED, BOLD, "Ошибка при создании");
      break;
    case NULL_POINTER:
      colorPrintf(RED, BOLD, "Нулевой указатель");
      break;
    case ALLOC_FAILED:
      colorPrintf(RED, BOLD, "Ошибка при аллокации");
      break;
    case CALLOC_FAILED:
      colorPrintf(RED, BOLD, "Ошибка при аллокации");
      break;
    case REALLOC_FAILED:
      colorPrintf(RED, BOLD, "Ошибка при реаллокации");
      break;
    case DELETE_FAILED:
      colorPrintf(RED, BOLD, "Ошибка при удалении элементов из дерева");
      break;
    case ASSERTED:
      break;
    case CHECK_FAILED:
      colorPrintf(RED, BOLD, "Ошибка при проверке");
      break;
    case CYCLE_ERR:
      colorPrintf(RED, BOLD, "Список некорректно зациклился");
      break;
    case EMPTY_TREE:
      colorPrintf(RED, BOLD, "Удален корень дерева");
      break;
    case FILE_ERR:
      colorPrintf(RED, BOLD, "Ошибка при открытии файла");
      break;
    case INCORRECT_DATA:
      colorPrintf(RED, BOLD, "Некорректное представление дерева");
      break;
    case INCORRECT_ANSWER:
      colorPrintf(RED, BOLD, "Некорректный ответ");
      break;
    default:
      colorPrintf(RED, BOLD, "Неизвестная ошибка");
      break;
  }
}
