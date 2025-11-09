#ifndef PRIVATE_TREE_H
#define PRIVATE_TREE_H

struct Node_t {
  TreeElem_t data;
  Node_t * left;
  Node_t * right;
};

struct Tree {
  Node_t * root;
  size_t len;
};

#endif // PRIVATE_TREE_H
