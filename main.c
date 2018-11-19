#include "cc.h"

int main(int argc, char **argv){
    if (argc != 2){
        fprintf(stderr, "引数の個数が正しくありません\n");
        return 1;
    }
    
    variables = new_vector();
    code = new_vector();
    program(tokenize(argv[1]));

    printf(".intel_syntax noprefix\n");
    printf(".global _main\n");
    printf("_main:\n");

    printf("\tpush rbp\n");
    printf("\tmov rbp, rsp\n");
    printf("\tsub rsp, %d\n", variables->len * 8);

    for (int i = 0;i < code->len;i++){
        gen((Node *)(code->data[i]));
        printf("\tpop rax\n");
    }

    printf("\tmov rsp, rbp\n");
    printf("\tpop rbp\n");
    printf("\tret\n");
    return 0;
}