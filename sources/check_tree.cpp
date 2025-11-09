#ifdef CHECK

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "my_printf.h"
#include "tree.h"
#include "private_tree.h"
#include "check_tree.h"

Errors verify(const Tree * tree, const char * file, const char * func, const size_t len, const char * position) {
  if (checkCycle(tree) != VALID) {
    dump(tree, file, func, len, position, INVALID);
    return INVALID;
  }
  dump(tree, file, func, len, position, VALID);
  checkStatement(tree == NULL);
  return VALID;
}

Errors checkCycle(const Tree * tree) {
  if (tree->root == NULL) {
    return VALID;
  }
  Node_t ** stack = (Node_t **) calloc(tree->len + 1, sizeof(Node_t *));
  if (stack == NULL) {
    return INVALID;
  }
  stack[0] = tree->root;
  size_t len = 0;
  int i = 0;
  while (i >= 0 && len < tree->len + 1) {
    Node_t * node = stack[i];
    for (int j = i; j < len; j++) {
        stack[j] = stack[j + 1];
    }
    i--;
    if (node->left != NULL) {
        stack[++i] = node->left;
        len++;
    }
    if (node->right != NULL) {
        stack[++i] = node->right;
        len++;
    }
  }
  free(stack);
  return (i == -1) ? VALID : INVALID;
}

void dump(const Tree * tree, const char * file, const char * func, const size_t strnum, const char * position, Errors img) {
  FILE * fp = fopen("./log.html", "a");
  if (fp == NULL) {
    return;
  }
  fprintf(fp, "<pre>\n");
  fprintf(fp, "<h3> DUMP <font color=red> %s </font> %s (%zu) </h3>\n", position, func, strnum);
  fprintf(fp, "Tree { ./%s:%zu }\n", file, strnum);
  size_t max_print = tree->len < 50 ? tree->len : 50;
  static size_t picture_number = 0;
  if (tree) {
    Node_t ** deque = (Node_t **) calloc(tree->len + 1, sizeof(Node_t *));
    if (deque == NULL) {
      return;
    }
    deque[0] = tree->root;
    size_t len = 0;
    int rk = 0, i = 0, j = 0;
    while (j <= i && len < max_print) {
      fprintf(fp, "( ");
      int current = i;
      for (; j <= current; j++) {
        fprintf(fp, " \"%s\" ", deque[j]->data);
        if (deque[j]->left != NULL) {
          deque[++i] = deque[j]->left;
          len++;
        }
        else {
          fprintf(fp, " ( \"nil\" ) ");
        }
        if (deque[j]->right != NULL) {
          deque[++i] = deque[j]->right;
          len++;
        }
        else {
          fprintf(fp, " ( \"nil\" ) ");
        }
      }
      fprintf(fp, ") ");
      rk++;
    }
    fprintf(fp, "\n");
    if (!img) {
        createGraph(tree, picture_number);
        fprintf(fp, "\nImage:\n<img src=./img/img%zu.png width=500px>\n", picture_number);
        picture_number++;
      }
    free(deque);
  }
  else {
    fprintf(fp, "Нулевой указатель на дерево.\n");
  }
  fclose(fp);
}

void createSquareNodes(FILE * file, Node_t * node) {
  if (!node) return;
  fprintf(file, "  node_%p [label=<\n", (void*)node);
  fprintf(file, "    <table border=\"1\" cellborder=\"1\" cellspacing=\"0\" bgcolor=\"#6e7681\" color=\"#FFFF00\">\n");
  fprintf(file, "      <tr><td border=\"1\" color=\"#ffff00\" colspan=\"2\"><b>%s</b></td></tr>\n", node->data);
  fprintf(file, "      <tr>\n");
  if (node->left == NULL) {
    fprintf(file, "      <td border=\"1\" color=\"#ffff00\">nil</td>\n");
  }
  else {
    fprintf(file, "      <td border=\"1\" color=\"#ffff00\">да</td>\n");
  }
  if (node->right == NULL) {
    fprintf(file, "      <td border=\"1\" color=\"#ffff00\">nil</td>\n");
  }
  else {
    fprintf(file, "      <td border=\"1\" color=\"#ffff00\">нет</td>\n");
  }
  fprintf(file, "      </tr>\n");
  fprintf(file, "    </table>\n");
  fprintf(file, "  >];\n");
  createSquareNodes(file, node->left);
  createSquareNodes(file, node->right);
}

void createDirectEdges(FILE * file, Node_t * node) {
  if (!node) return;
  if (node->left) {
    fprintf(file, "  node_%p -> node_%p [color=\"#4d78cc\"];\n", (void*)node, (void*)node->left);
  }
  if (node->right) {
    fprintf(file, "  node_%p -> node_%p [color=\"#109b48\"];\n", (void*)node, (void*)node->right);
  }
  createDirectEdges(file, node->left);
  createDirectEdges(file, node->right);
}

void createGraph(const Tree * tree, size_t picture_number) {
  char filename[100];
  sprintf(filename, "./img/img%zu.txt", picture_number);
  FILE * file = fopen(filename, "w");
  if (file == NULL) {
    printf("Error opening file!\n");
    return;
  }
  fprintf(file, "digraph G {\n");
  fprintf(file, "  bgcolor=\"#1f1f1f\";\n");
  fprintf(file, "  rankdir=TB;\n");
  fprintf(file, "  nodesep=0.5;\n");
  fprintf(file, "  ranksep=0.8;\n\n");
  fprintf(file, "  node [shape=plain, fontname=\"Arial\", fontsize=12, style=\"filled\", fillcolor=\"#6e7681\", color=\"#FFFF00\", fontcolor=\"black\"];\n");
  fprintf(file, "  edge [penwidth=2, arrowsize=0.8, color=\"#FFFF00\"];\n\n");
  fprintf(file, "  info [label=<\n");
  fprintf(file, "    <table border=\"1\" cellborder=\"1\" cellspacing=\"0\" bgcolor=\"#6e7681\" color=\"#FFFF00\">\n");
  if (tree->root == NULL) {
    fprintf(file, "      <tr><td border=\"1\" color=\"#ffff66\"><b>root = nil</b></td></tr>\n");
  }
  else {
    fprintf(file, "      <tr><td border=\"1\" color=\"#ffff66\"><b>root = %p</b></td></tr>\n", (void*)tree->root);
  }
  fprintf(file, "      <tr><td border=\"1\" color=\"#ffff66\">size = %zu</td></tr>\n", tree->len);
  fprintf(file, "    </table>\n");
  fprintf(file, "  >];\n\n");
  fprintf(file, "  // Tree nodes\n");
  createSquareNodes(file, tree->root);
  if (tree->root != NULL) {
    fprintf(file, "  info -> node_%p [color=\"#f112bdff\"];\n", (void*)tree->root);
  }
  createDirectEdges(file, tree->root);
  fprintf(file, "}\n");
  fclose(file);
  char command[256];
  snprintf(command, sizeof(command), "dot -Tpng \"./img/img%zu.txt\" -o \"./img/img%zu.png\" -n2", picture_number, picture_number);
  system(command);
}

void printErrorLines(const char * file, const char * func, size_t line_number) {
  colorPrintf(RED, BOLD, "file: %s:%zu\nfunc: %s\n", file, line_number, func);

  const int MAX_LINE = 150;
  char line[MAX_LINE] = {};
  FILE * fp = fopen(file, "r");
  if (fp == NULL) {
    return;
  }

  for (size_t i = 0; i <= line_number; i++) {
    fgets(line, MAX_LINE, fp);
    if (i >= line_number - 2)
      colorPrintf(RED, BOLD, "%zu: %s", i + 1, line);
  }

  fclose(fp);
}

#endif // CHECK
