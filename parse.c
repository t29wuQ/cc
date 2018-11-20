#include "cc.h"

Vector *code;

static Node *new_node(int op, Node *lhs, Node *rhs){
    Node *node = (Node*)malloc(sizeof(Node));
    node->ty = op;
    node->lhs = lhs;
    node->rhs = rhs;
    return node;
}

static Node *new_node_num(int val){
     Node *node = (Node*)malloc(sizeof(Node));
     node->ty = TK_NUM;
     node->val = val;
     return node;
 }

 static Node *new_node_ident(char *name){
     Node *node = (Node*)malloc(sizeof(Node));
     node->ty = TK_IDENT;
     node->name = name;
    return node;
 }

static Node* statement();
static Node* assign();
static Node* expr();
static Node* mul();
static Node* term();

int pos = 0;
Vector *tokens;

void program(Vector *t){
    tokens = t;
    Node *lhs = statement();
    vec_push(code, lhs);
    if (((Token*)(tokens->data[pos]))->ty == TK_EOF)
        return;
    program(t);
}

static Node* statement(){
    if(((Token*)(tokens->data[pos]))->ty == TK_RETURN){
        pos++;
        Node* lhs = assign();
        ((Token*)(tokens->data[pos]))->ty = TK_EOF;
        return lhs;
    }
    return assign();
}

static Node* assign(){
    Node* lhs = expr();
    if (((Token*)(tokens->data[pos]))->ty == ';'){
        pos++;
        return lhs;
    }
    if (((Token*)(tokens->data[pos]))->ty == '=' ||
    ((Token*)(tokens->data[pos]))->ty == TK_EQUAL ||
    ((Token*)(tokens->data[pos]))->ty == TK_NEQUAL){
        int ty = ((Token *)(tokens->data[pos]))->ty;
        pos++;
        return  new_node(ty, lhs, assign());
    }
    return lhs;
}

static Node* expr(){
    Node* lhs = mul();
    if (((Token*)(tokens->data[pos]))->ty == ')')
        return lhs;
    if (((Token*)(tokens->data[pos]))->ty == '+'){
        pos++;
        return new_node('+', lhs, expr());
    }
    if (((Token*)(tokens->data[pos]))->ty == '-'){
        pos++;
        return new_node('-', lhs, expr());
    }
    return lhs;
}

static Node* mul(){
    Node *lhs = term();
    if (((Token*)(tokens->data[pos]))->ty == TK_EOF)
        return lhs;
    if (((Token*)(tokens->data[pos]))->ty == '*'){
        pos++;
        return new_node('*', lhs, mul());
    }
    if (((Token*)(tokens->data[pos]))->ty == '/'){
        pos++;
        return new_node('/', lhs, mul());
    }
    return lhs;
}

static Node* term(){
    if (((Token*)(tokens->data[pos]))->ty == TK_NUM)  
        return new_node_num(((Token*)(tokens->data[pos++]))->val);
    if (((Token*)(tokens->data[pos]))->ty == TK_IDENT)
        return new_node_ident(((Token*)(tokens->data[pos++]))->input);
    if (((Token*)(tokens->data[pos]))->ty == '('){
        pos++;
        Node *node = expr();
        pos++;
        return node;
    }
    fprintf(stderr, "数値でも開きカッコでもないトークンです: %d,  %s", pos, ((Token*)(tokens->data[pos]))->input);
}