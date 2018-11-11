#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum{
    ND_NUM = 256,
    TK_NUM = 256, //integer token (token type num)
    ND_IDENT,
    TK_EOF, //end of input
};

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

Token tokens[100];
int pos = 0;
int c_pos = 0;

Node *code[100];

Node *new_node(int op, Node *lhs, Node *rhs){
    Node *node = (Node*)malloc(sizeof(Node));
    node->ty = op;
    node->lhs = lhs;
    node->rhs = rhs;
    return node;
}

Node *new_node_num(int val){
     Node *node = (Node*)malloc(sizeof(Node));
     node->ty = ND_NUM;
     node->val = val;
     return node;
 }

 Node *new_node_ident(char *name){
     Node *node = (Node*)malloc(sizeof(Node));
     node->ty = ND_IDENT;
     node->name = *name;
     return node;
     
 }

void program();
Node* assign();
Node* expr();
Node* mul();
Node* term();

void program(){
    Node *lhs = assign();
    code[c_pos++] = lhs;
    if (tokens[pos].ty == TK_EOF)
        return;
    program();
}

Node* assign(){
    Node* lhs = expr();
    if (tokens[pos].ty == ';'){
        pos++;
        return lhs;
    }
    if (tokens[pos].ty == '='){
        pos++;
        return  new_node('=', lhs, assign());
    }
    return lhs;
}

Node* expr(){
    Node* lhs = mul();
    if (tokens[pos].ty == ')')
        return lhs;
    if (tokens[pos].ty == '+'){
        pos++;
        return new_node('+', lhs, expr());
    }
    if (tokens[pos].ty == '-'){
        pos++;
        return new_node('-', lhs, expr());
    }
    return lhs;
}

Node* mul(){
    Node *lhs = term();
    if (tokens[pos].ty == TK_EOF)
        return lhs;
    if (tokens[pos].ty == '*'){
        pos++;
        return new_node('*', lhs, mul());
    }
    if (tokens[pos].ty == '/'){
        pos++;
        return new_node('/', lhs, mul());
    }
    return lhs;
}

Node* term(){
    if (tokens[pos].ty == TK_NUM)  
        return new_node_num(tokens[pos++].val);
    if (tokens[pos].ty == ND_IDENT)
        return new_node_ident(tokens[pos++].input);
    if (tokens[pos].ty == '('){
        pos++;
        Node *node = expr();
        pos++;
        return node;
    }
    fprintf(stderr, "数値でも開きカッコでもないトークンです: %s", tokens[pos].input);
}

void gen_lval(Node *node){
    if (node->ty == ND_IDENT){
        printf("\tmov rax, rbp\n");
        printf("\tsub rax, %d\n", ('z' - node->name + 1) * 8);
        printf("\tpush rax\n");
        return;
    }
    fprintf(stderr, "代入の左辺値が変数ではありません");
}

void gen(Node *node){
    if (node->ty == ND_NUM){
        printf("\tpush %d\n", node->val);
        return;
    }

    if (node->ty == ND_IDENT){
        gen_lval(node);
        printf("\tpop rax\n");
        printf("\tmov rax, [rax]\n");
        printf("\tpush rax\n");
        return;
    }

    if (node->ty == '='){
        gen_lval(node->lhs);
        gen(node->rhs);

        printf("\tpop rdi\n");
        printf("\tpop rax\n");
        printf("\tmov [rax], rdi\n");
        printf("\tpush rdi\n");
        return;
        
    }

    gen(node->lhs);
    gen(node->rhs);

    printf("\tpop rdi\n");
    printf("\tpop rax\n");

    switch (node->ty){
        case '+':
            printf("\tadd rax, rdi\n");
            break;
        case '-':
            printf("\tsub rax, rdi\n");
            break;
        case '*':
            printf("\tmul rdi\n");
            break;
        case '/':
            printf("\tmov rdx, 0\n");
            printf("\tdiv rdi\n");
            break;
    }

    printf("\tpush rax\n");
}


void tokenize(char *p){
    int i = 0;
    while(*p){
        if (isspace(*p)){
            p++;
            continue;
        }

        if (*p == '+' || *p == '-' ||
         *p == '*' || *p == '/' ||
         *p == '(' || *p == ')' ||
         *p == '=' || *p == ';'){
            tokens[i].ty = *p;
            tokens[i].input = p;
            p++;
            i++;
            continue;
        }

        if (isdigit(*p)) {
            tokens[i].ty = TK_NUM;
            tokens[i].input = p;
            tokens[i].val = strtol(p, &p, 10);
            i++;
            continue;
        }

        if ('a' <= *p && *p <= 'z'){
            tokens[i].ty = ND_IDENT;
            tokens[i].input = p;
            i++;
            p++;
            continue;
        }

        fprintf(stderr, "トークナイズできません: %s\n", p);
        exit(1);
    }

    tokens[i].ty = TK_EOF;
    tokens[i].input = p;
}

//error message
void error(int i){
    fprintf(stderr, "予期せぬトークンです: %s\n", tokens[i].input);
    exit(1);
}

int main(int argc, char **argv){
    if (argc != 2){
        fprintf(stderr, "引数の個数が正しくありません\n");
        return 1;
    }
    
    tokenize(argv[1]);
    program();

    printf(".intel_syntax noprefix\n");
    printf(".global _main\n");
    printf("_main:\n");

    printf("\tpush rbp\n");
    printf("\tmov rbp, rsp\n");
    printf("\tsub rsp, 208\n");

    for (int i = 0;code[i];i++){
        gen(code[i]);
        printf("\tpop rax\n");
    }

    printf("\tmov rsp, rbp\n");
    printf("\tpop rbp\n");
    printf("\tret\n");
    return 0;
    
}