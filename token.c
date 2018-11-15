#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
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

        if ('a' <= *p && *p <= 'z'){
            token = (Token *)malloc(sizeof(Token));
            token->ty = TK_IDENT;
            token->input = p;
            vec_push(tokens, token);
            p++;
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