#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tree.h"
#include "private_tree.h"
#include "check_tree.h"

static Node_t * findElem(Node_t * current, TreeElem_t elem, Node_t ** parent);
static void DestroyNode(Node_t * node, size_t * len);
static void PrintNode(Node_t * node, const char * type, FILE * file=stdout);
static void NodesToArray(Node_t * node, TreeElem_t * array);
static TreeErr parse(Node_t ** node, char ** str, size_t * len);

TreeErr treeInit(Tree ** tree) {
  *tree = (Tree *) calloc(1, sizeof(Tree));
  if (*tree == NULL) {
    return CREATE_FAILED;
  }
  (*tree)->len = 1;
  (*tree)->root = (Node_t *) calloc(1, sizeof(Node_t));
  if ((*tree)->root == NULL) {
    return CREATE_FAILED;
  }
  (*tree)->root->data = (TreeElem_t) calloc(strlen("неизвестно что") + 1, sizeof(char));
  strcpy((*tree)->root->data, "неизвестно что");
  (*tree)->root->left = (*tree)->root->right = NULL;
  treeVerify(*tree, "AFTER");
  return SUCCESS;
}

TreeErr treeInsert(Tree * tree, Node_t * current, TreeElem_t result, TreeElem_t question, int flag) {
  treeVerify(tree, "BEFORE");
  tree->len += 2;
  Node_t * tree_elem = tree->root;
  if (tree_elem == NULL) {
    return EMPTY_TREE;
  }
  current->left = (Node_t *) calloc(1, sizeof(Node_t));
  current->right = (Node_t *) calloc(1, sizeof(Node_t));
  if (flag) {
    current->left->data = (TreeElem_t) calloc(strlen(result) + 1, sizeof(char));
    strcpy(current->left->data, result);
    current->left->data[strlen(result) - 1] = '\0';
    current->right->data = current->data;
    current->data = (TreeElem_t) calloc(strlen(question) + 1, sizeof(char));
    strcpy(current->data, question);
    current->data[strlen(question) - 1] = '\0';
  }
  else {
    current->right->data = (TreeElem_t) calloc(strlen(result) + 1, sizeof(char));
    strcpy(current->right->data, result);
    current->right->data[strlen(result) - 1] = '\0';
    current->left->data = current->data;
    current->data = (TreeElem_t) calloc(strlen(question) + 1, sizeof(char));
    strcpy(current->data, question);
    current->data[strlen(question) - 1] = '\0';
  }
  current->left->left = current->left->right = 0;
  current->right->left = current->right->right = 0;
  treeVerify(tree, "AFTER");
  return SUCCESS;
}

TreeErr subtreeDelete(Tree * tree, TreeElem_t elem) {
  treeVerify(tree, "BEFORE");
  Node_t * parent = NULL;
  Node_t * current = findElem(tree->root, elem, &parent);
  if (current == NULL) {
    return DELETE_FAILED;
  }
  if (parent == NULL) {
    treeDestroy(tree);
    treeInit(&tree);
  }
  Node_t * left = parent->left;
  Node_t * right = parent->right;
  if (parent->left == current) {
    free(parent->data);
    parent->data = right->data;
    parent->left = right->left;
    parent->right = right->right;
    DestroyNode(left, &tree->len);
    free(right);
  }
  else {
    free(parent->data);
    parent->data = left->data;
    parent->left = left->left;
    parent->right = left->right;
    DestroyNode(right, &tree->len);
    free(left);
  }
  treeVerify(tree, "AFTER");
  return SUCCESS;
}

static Node_t * findElem(Node_t * current, TreeElem_t elem, Node_t ** parent) {
  if (current == NULL) {
    return NULL;
  }
  if (!strcmp(current->data, elem)) {
    return current;
  }
  if (current->left == NULL || current->right == NULL) {
    return NULL;
  }
  if (!strcmp(current->left->data, elem) || !strcmp(current->right->data, elem)) {
    *parent = current;
  }
  Node_t * left = findElem(current->left, elem, parent);
  Node_t * right = findElem(current->right, elem, parent);
  if (left != NULL) {
    return left;
  }
  else {
    return right;
  }
}

void treeDestroy(Tree * tree) {
  DestroyNode(tree->root, &tree->len);
  free(tree);
}

static void DestroyNode(Node_t * node, size_t * len) {
  if (node->left != NULL) {
    DestroyNode(node->left, len);
  }
  if (node->right != NULL) {
    DestroyNode(node->right, len);
  }
  free(node->data);
  free(node);
  (*len)--;
  return;
}

TreeErr treeGetFirst(Tree * tree, Node_t ** result) {
  treeVerify(tree, "BEFORE");
  *result = tree->root;
  treeVerify(tree, "AFTER");
  return SUCCESS;
}

TreeErr treeGetLeft(Tree * tree, Node_t * elem, Node_t ** result) {
  treeVerify(tree, "BEFORE");
  *result = elem->left;
  treeVerify(tree, "AFTER");
  return SUCCESS;
}

TreeErr treeGetRight(Tree * tree, Node_t * elem, Node_t ** result) {
  treeVerify(tree, "BEFORE");
  *result = elem->right;
  treeVerify(tree, "AFTER");
  return SUCCESS;
}

TreeErr treeGetValue(Tree * tree, Node_t * elem, TreeElem_t * result) {
  treeVerify(tree, "BEFORE");
  *result = elem->data;
  treeVerify(tree, "AFTER");
  return SUCCESS;
}

TreeErr treeGetLen(Tree * tree, size_t * len) {
  treeVerify(tree, "BEFORE");
  *len = tree->len;
  treeVerify(tree, "AFTER");
  return SUCCESS;
}

TreeErr treePrint(Tree * tree, char * type) {
  treeVerify(tree, "BEFORE");
  PrintNode(tree->root, type);
  treeVerify(tree, "AFTER");
  return SUCCESS;
}

static void PrintNode(Node_t * node, const char * type, FILE * file) {
  fprintf(file, "(");
  if (!strcmp(type, "PREORDER")) {
    fprintf(file, "\"%s\"", node->data);
  }

  if (node->left != NULL) {
    PrintNode(node->left, type, file);
  }
  else {
    fprintf(file, "(\"nil\")");
  }

  if (!strcmp(type, "INORDER")) {
    fprintf(file, "\"%s\"", node->data);
  }

  if (node->right != NULL) {
    PrintNode(node->right, type, file);
  }
  else {
    fprintf(file, "(\"nil\")");
  }

  if (!strcmp(type, "POSTORDER")) {
    fprintf(file, "\"%s\"", node->data);
  }
  fprintf(file, ")");
  return;
}

TreeErr getTree(const char * filename, Tree ** tree) {
  if (*tree != NULL) {
    return CREATE_FAILED;
  }
  FILE * file = fopen(filename, "r");
  if (file == NULL) {
    return FILE_ERR;
  }
  char * str = NULL;
  size_t n = 0;
  getline(&str, &n, file);
  if (str == NULL) {
    return ALLOC_FAILED;
  }
  if (str[0] != '(') {
    free(str);
    return treeInit(tree);
  }
  char * old_str = str;
  *tree = (Tree *) calloc(1, sizeof(Tree));
  if (*tree == NULL) {
    return CALLOC_FAILED;
  }
  (*tree)->len = 0;
  (*tree)->root = NULL;
  TreeErr result = parse(&(*tree)->root, &str, &(*tree)->len);
  fclose(file);
  free(old_str);
  treeVerify(*tree, "AFTER");
  return result;
}

static TreeErr parse(Node_t ** node, char ** str, size_t * len) {
  if (**str == '(') {
    if (!strncmp(*str, "(\"nil\")", 7)) {
      *node = NULL;
      *str += 7;
      return SUCCESS;
    }
    *node = (Node_t *) calloc(1, sizeof(Node_t));
    if (*node == NULL) {
      return CALLOC_FAILED;
    }
    *str += 2;
    if (strchr(*str, '"') == NULL) {
      return INCORRECT_DATA;
    }
    size_t substr_len = strchr(*str, '"') - *str + 1;
    (*node)->data = (TreeElem_t) calloc(substr_len, sizeof(char));
    if ((*node)->data == NULL) {
      return CALLOC_FAILED;
    }
    strncpy((*node)->data, *str, substr_len - 1);
    (*node)->data[substr_len - 1] = '\0';
    (*node)->left = (*node)->right = NULL;
    *str += substr_len;
    (*len)++;
  }
  if (**str == '(') {
    TreeErr result = parse(&(*node)->left, str, len);
    if (result != SUCCESS) {
      return result;
    }
  }
  if (**str == '(') {
    TreeErr result = parse(&(*node)->right, str, len);
    if (result != SUCCESS) {
      return result;
    }
  }
  if (**str == ')') {
    (*str)++;
  }
  return SUCCESS;
}

TreeErr saveTree(Tree * tree, const char * filename) {
  treeVerify(tree, "BEFORE");
  FILE * file = fopen(filename, "w");
  if (file == NULL) {
    return FILE_ERR;
  }
  PrintNode(tree->root, "PREORDER", file);
  treeVerify(tree, "AFTER");
  fclose(file);
  return SUCCESS;
}

TreeErr TreeToArray(Tree * tree, TreeElem_t ** array, size_t * len) {
  treeVerify(tree, "BEFORE");
  *len = tree->len;
  *array = (TreeElem_t *) calloc(tree->len, sizeof(TreeElem_t));
  NodesToArray(tree->root, *array);
  treeVerify(tree, "AFTER");
  return SUCCESS;
}

static void NodesToArray(Node_t * node, TreeElem_t * array) {
  static size_t index = 0;
  if (node->left != NULL) {
    NodesToArray(node->left, array);
  }
  array[index] = (TreeElem_t) calloc(strlen(node->data) + 1, sizeof(char));
  strcpy(array[index++], node->data);
  if (node->right != NULL) {
    NodesToArray(node->right, array);
  }
  return;
}
