#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum{
    TK_NUM = 256, //Number literal
    TK_IDENT, //Identifier
    TK_RETURN, //return
    //TK_IF, //if
    TK_EQUAL, //==
    TK_NEQUAL, //!=
    TK_EOF //End marker
};

#ifndef CC_H
#define CC_H
typedef struct{
    int ty; //Token type
    int val; //Number value(only number literal)
    char *input; //token str(Identifer name)
} Token;

typedef struct Node{
    int ty;
    struct Node *lhs;
    struct Node *rhs;
    int val;
    char *name;
} Node;

typedef struct{
    void **data;
    int capacity;
    int len;
} Vector;

typedef struct{
    Vector *keys;
    Vector *vals;
} Map;

extern Vector *code;
extern Vector *variables;

Vector *new_vector();
void vec_push(Vector *vec, void *elem);
Map *new_map();
void map_put(Map *map, char *key, void *val);
void *map_get(Map *map, char *key);

Vector *tokenize(char *p);
void program(Vector *tokens);
int get_variable(char *name);
void gen(Node *node);
#endif