#include<stdio.h>
#include <stdlib.h>
#include "cc.h"

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
     node->ty = TK_NUM;
     node->val = val;
     return node;
 }

 Node *new_node_ident(char *name){
     Node *node = (Node*)malloc(sizeof(Node));
     node->ty = TK_IDENT;
     node->name = *name;
     return node;
 }

Node* assign();
Node* expr();
Node* mul();
Node* term();

int pos = 0;
int c_pos = 0;

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
    if (tokens[pos].ty == TK_IDENT)
        return new_node_ident(tokens[pos++].input);
    if (tokens[pos].ty == '('){
        pos++;
        Node *node = expr();
        pos++;
        return node;
    }
    fprintf(stderr, "数値でも開きカッコでもないトークンです: %s", tokens[pos].input);
}