#include "cc.h"

int main(int argc, char **argv){
    if (argc != 2){
        fprintf(stderr, "引数の個数が正しくありません\n");
        return 1;
    }
    
    variables = new_vector();
    main_code = new_vector();
    sub_code = new_vector();
    if_component_number = new_vector();
    vec_push(if_component_number, 0);    
    program(tokenize(argv[1]));

    printf(".intel_syntax noprefix\n");
    printf(".global _main\n");
    printf("_main:\n");
    printf("\tpush rbp\n");
    printf("\tmov rbp, rsp\n");
    printf("\tsub rsp, %d\n", variables->len * 8);

    for (int i = 0;i < main_code->len;i++){
        gen((Node *)(main_code->data[i]));
        printf("\tpop rax\n");
        if (((Node *)(main_code->data[i]))->ty == TK_RETURN){
            printf("\tjmp .Lend\n");
            break;
        }
    }

    int label_pos = 0;
    if (sub_code->len != 0){
         printf("\n.L%d:\n", label_pos++);
         for (int i = 0;i < sub_code->len;i++){
            Node *node = (Node *)(sub_code->data[i]);
            if (node == NULL){
                if (i != (sub_code->len - 1)){
                    printf("\n.L%d:\n", label_pos++);
                    continue;
                }
                break;
            }
            gen(node);
            if (node->ty == TK_RETURN){
                printf("\tpop rax\n");
                printf("\tjmp .Lend\n");
                if (i != (sub_code->len - 1)){
                    printf("\n.L%d:\n", label_pos++);
                    continue;
                }
                break;
            }
        }
    }
    printf("\n.Lend:\n");
    printf("\tmov rsp, rbp\n");
    printf("\tpop rbp\n");
    printf("\tret\n");
    return 0;
}