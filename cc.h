enum{
    TK_NUM = 256, //Number literal
    TK_IDENT, //Identifier
    TK_EOF //End marker
};

#ifndef CC_H
#define CC_H
typedef struct{
    int ty; //token type (ASCII code)
    int val; //token value (only integer)
    char *input; //token str(error message)
} Token;

typedef struct Node{
    int ty;
    struct Node *lhs;
    struct Node *rhs;
    int val;
    char name;
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

extern Vector *tokens;
extern Vector *code;

Vector *new_vector();
void vec_push(Vector *vec, void *elem);
void tokenize(char *p);
void program();
void gen(Node *node);
#endif