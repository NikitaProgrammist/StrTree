#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tree.h"
#include "utils.h"

TreeErr mainCycle(Tree * tree);

int main() {
  clear();
  Tree * tree = NULL;

  TreeErr result = getTree("./tree.txt", &tree);
  if (result != SUCCESS) {
    treeDestroy(tree);
    treeInit(&tree);
    parseErrors(result);
  }

  result = mainCycle(tree);
  if (result != SUCCESS) {
    parseErrors(result);
  }

  saveTree(tree, "./tree.txt");
  treeDestroy(tree);
  return 0;
}

TreeErr mainCycle(Tree * tree) {
  Node_t * curr = NULL;
  Node_t * left = NULL, * right = NULL;
  TreeElem_t elem = NULL;
  TreeErr result = treeGetFirst(tree, &curr);
  if (result != SUCCESS || curr == NULL) {
    return result;
  }
  char * str = NULL;
  size_t n = 0;
  while (true) {
    result = treeGetLeft(tree, curr, &left);
    if (result != SUCCESS) {
      return result;
    }
    result = treeGetRight(tree, curr, &right);
    if (result != SUCCESS) {
      return result;
    }
    result = treeGetValue(tree, curr, &elem);
    if (result != SUCCESS) {
      return result;
    }
    printf("Это %s?\n", elem);
    str = NULL;
    getline(&str, &n, stdin);
    if (!strncmp(str, "да", 2)) {
      if (left == NULL && right == NULL) {
        printf("Я угадал.\n");
        free(str);
        return SUCCESS;
      }
      curr = left;
    }
    else if (!strncmp(str, "нет", 3)) {
      if (left == NULL && right == NULL) {
        printf("Вы хотите добавить новый объект?\n");
        char * get_new = NULL;
        size_t new_n = 0;
        getline(&get_new, &new_n, stdin);
        if (!strncmp(get_new, "да", 2)) {
          printf("Введите название объекта и на следующей строке признак который отличает его от данного.\n");
          char * new_obj = NULL, * new_pr = NULL;
          getline(&new_obj, &new_n, stdin);
          getline(&new_pr, &new_n, stdin);
          result = treeInsert(tree, curr, new_obj, new_pr, 1);
          free(new_pr);
          free(new_obj);
          free(get_new);
          free(str);
          return result;
        }
        else if (!strncmp(get_new, "нет", 3)) {
          free(get_new);
          free(str);
          return SUCCESS;
        }
        else {
          free(get_new);
          free(str);
          return INCORRECT_ANSWER;
        }
        free(get_new);
        free(str);
        return SUCCESS;
      }
      curr = right;
    }
    else {
      free(str);
      return INCORRECT_ANSWER;
    }
    free(str);
  }
  return SUCCESS;
}
