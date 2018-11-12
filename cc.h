enum{
    TK_NUM = 256, //Number literal
    TK_IDENT, //Identifier
    TK_EOF //End marker
};

typedef struct{
    int ty; //token type (ASCII code)
    int val; //token value (only integer)
    char *input; //token str(error message)
} Token;

Token tokens[100];

typedef struct Node{
    int ty;
    struct Node *lhs;
    struct Node *rhs;
    int val;
    char name;
} Node;

Node *code[100];

void tokenize(char *p);
void program();
void gen(Node *node);