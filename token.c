#include "cc.h"

Vector *tokens;

void tokenize(char *p){
    Token *token;
    while(*p){
        if (isspace(*p)){
            p++;
            continue;
        }

        if (*p == '+' || *p == '-' ||
         *p == '*' || *p == '/' ||
         *p == '(' || *p == ')' ||
         *p == '=' || *p == ';'){
            token = (Token *)malloc(sizeof(Token));
            token->ty = *p;
            token->input = p;
            vec_push(tokens, token);
            p++;
            continue;
        }

        if (isdigit(*p)) {
            token = (Token *)malloc(sizeof(Token));
            token->ty = TK_NUM;
            token->input = p;
            token->val = strtol(p, &p, 10);
            vec_push(tokens, token);
            continue;
        }

        if (isalpha(*p) || *p == '_'){
            token = (Token *)malloc(sizeof(Token));
            token->ty = TK_IDENT;
            int name_len = 1;
            while(isalnum(*(p + name_len)) ||  *(p + name_len) == '_')
                name_len++;
            token->name = strndup(p, name_len);
            vec_push(tokens, token);
            if (get_variable(token->name) < 0)
                vec_push(variables, token->name);
            p += name_len;
            continue;
        }

        fprintf(stderr, "トークナイズできません: %s\n", p);
        exit(1);
    }

    token = (Token *)malloc(sizeof(Token));
    token->ty = TK_EOF;
    token->input = p;
    vec_push(tokens, token);
}