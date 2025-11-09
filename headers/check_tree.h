#ifdef CHECK

#ifndef CHECK_TREE_H
#define CHECK_TREE_H

#define treeVerify(tree, arg) {                                                \
    if (verify(tree, __FILE__, __func__, __LINE__, arg)) {                     \
        printErrorLines("../" __FILE__, __PRETTY_FUNCTION__, __LINE__);        \
        return CHECK_FAILED;                                                   \
    }                                                                          \
}

#define checkStatement(arg) {                                                  \
    if (arg) {                                                                 \
        colorPrintf(RED, BOLD, "\nTree died, because: " #arg "\n");            \
        return INVALID;                                                        \
    }                                                                          \
}

#define myAssert(arg) {                                                        \
    if (!(arg)) {                                                              \
        colorPrintf(RED, BOLD, "\nAssertion failed: " #arg "\n");              \
        printErrorLines("../" __FILE__, __PRETTY_FUNCTION__, __LINE__);        \
        return ASSERTED_S;                                                     \
    }                                                                          \
}

enum Errors {
  VALID,
  INVALID
};

Errors verify(const Tree * tree, const char * file, const char * func, const size_t len, const char * position);
Errors checkCycle(const Tree * tree);
void dump(const Tree * tree, const char * file, const char * func, const size_t len, const char * position, Errors img);
void createSquareNodes(FILE * file, Node_t * node);
void createDirectEdges(FILE * file, Node_t * node);
void createGraph(const Tree * tree, size_t picture_number);
void printErrorLines(const char * file, const char * func, size_t line_number);

#endif // CHECK_TREE_H

#else

#define treeVerify(tree, arg)
#define myAssert(arg)
#define dump(arg)

#endif // CHECK
