#include "cc.h"

Vector *variables;
int l_pos = 1;
int if_pos = 0;

int get_variable(char *name){
    for (int i = 0;i < variables->len;i++)
        if (!strcmp(name, (char *)(variables->data[i])))
            return i + 1;
    return -1;
}

void gen_branch(Node *node){
    gen(node->lhs);
    gen(node->rhs);
    printf("\tpop rdi\n");
    printf("\tpop rax\n");
    printf("\tcmp rdi, rax\n");
    if (node->ty == TK_EQUAL)
        printf("\tje .L%d\n", l_pos++);
    if (node->ty == TK_NEQUAL)
        printf("\tjne .L%d\n", l_pos++);
    printf("\tjmp .L%d\n", ((int)if_component_number->data[if_pos] * 2 + 1) - l_pos);
    printf("\n.L%d:\n", l_pos - 1);
}

void gen_lval(Node *node){
    if (node->ty == TK_IDENT){
        printf("\tmov rax, rbp\n");
        printf("\tsub rax, %d\n", get_variable(node->name) * 8); //Location variable address
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
        printf("\tpop rax\n"); //pop variable address
        printf("\tmov rax, [rax]\n"); //move variable value to rax
        printf("\tpush rax\n");
        return;
    }

    if (node->ty == '='){
        gen_lval(node->lhs);
        gen(node->rhs);

        printf("\tpop rdi\n"); //pop value
        printf("\tpop rax\n"); //pop variable address
        printf("\tmov [rax], rdi\n"); //move value to variable address
        printf("\tpush rdi\n"); //push value
        return;
    }

    if (node->ty == TK_EQUAL || node->ty == TK_NEQUAL){
        gen(node->lhs);
        gen(node->rhs);

        // printf("\tpop rdi\n");
        // printf("\tpop rax\n");
        // printf("\tcmp rdi, rax\n");
        // printf("\tsete al\n");
        // printf("\tmovzb rax, al\n");
        return;
    }

    if (node->ty == TK_IF){
        gen_branch(node->lhs);
        return;
    }

    if (node->ty == TK_RETURN){
        gen(node->lhs);
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