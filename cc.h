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

extern Token tokens[100];
extern Node *code[100];

void tokenize(char *p);
void program();
void gen(Node *node);
#endif