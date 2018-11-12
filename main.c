#include<stdio.h>
#include "cc.h"

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