#include<stdio.h>
#include "cc.h"

void gen_lval(Node *node){
    if (node->ty == TK_IDENT){
        printf("\tmov rax, rbp\n");
        printf("\tsub rax, %d\n", ('z' - node->name + 1) * 8);
        printf("\tpush rax\n");
        return;
    }
    fprintf(stderr, "代入の左辺値が変数ではありません");
}

void gen(Node *node){
    if (node->ty == TK_NUM){
        printf("\tpush %d\n", node->val);
        return;
    }

    if (node->ty == TK_IDENT){
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