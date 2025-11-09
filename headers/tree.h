#ifndef TREE_H
#define TREE_H

typedef struct Node_t Node_t;
typedef struct Tree Tree;
typedef char * TreeElem_t;

enum TreeErr {
  SUCCESS,
  ASSERTED,
  CREATE_FAILED,
  NULL_POINTER,
  DELETE_FAILED,
  ALLOC_FAILED,
  CALLOC_FAILED,
  REALLOC_FAILED,
  CHECK_FAILED,
  CYCLE_ERR,
  EMPTY_TREE,
  FILE_ERR,
  INCORRECT_DATA,
  INCORRECT_ANSWER
};

TreeErr treeInit(Tree ** tree);
TreeErr treeInsert(Tree * tree, Node_t * current, TreeElem_t result, TreeElem_t question, int flag);
TreeErr subtreeDelete(Tree * tree, TreeElem_t elem);
void treeDestroy(Tree * tree);
TreeErr treeGetFirst(Tree * tree, Node_t ** result);
TreeErr treeGetLeft(Tree * tree, Node_t * elem, Node_t ** result);
TreeErr treeGetRight(Tree * tree, Node_t * elem, Node_t ** result);
TreeErr treeGetValue(Tree * tree, Node_t * elem, TreeElem_t * result);
TreeErr treeGetLen(Tree * tree, size_t * len);
TreeErr treePrint(Tree * tree, char * type);
TreeErr getTree(const char * filename, Tree ** tree);
TreeErr saveTree(Tree * tree, const char * filename);
TreeErr TreeToArray(Tree * tree, TreeElem_t ** array, size_t * len);

#endif // TREE_H
