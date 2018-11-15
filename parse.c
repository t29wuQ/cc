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
    if (((Token*)(tokens->data[pos]))->ty == TK_EOF)
        return;
    program();
}

Node* assign(){
    Node* lhs = expr();
    if (((Token*)(tokens->data[pos]))->ty == ';'){
        pos++;
        return lhs;
    }
    if (((Token*)(tokens->data[pos]))->ty == '='){
        pos++;
        return  new_node('=', lhs, assign());
    }
    return lhs;
}

Node* expr(){
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

Node* mul(){
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

Node* term(){
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