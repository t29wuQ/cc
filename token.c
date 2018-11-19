#include "cc.h"

Map *keywords;

static Token *add_token(Vector *v, int ty, char *input){
    Token *t = malloc(sizeof(Token));
    t->ty = ty;
    t->input = input;
    vec_push(v, t);
    return t;
}

static Vector *scan(char *p){
    Vector *tokens = new_vector();
    Token *token;
    while(*p){
        if (isspace(*p)){
            p++;
            continue;
        }

        if (strchr("+-*/=;()", *p)){
            add_token(tokens, *p, p);
            p++;
            continue;
        }

        if (isdigit(*p)) {
            token = add_token(tokens, TK_NUM, p);
            token->val = strtol(p, &p, 10);
            continue;
        }

        if (isalpha(*p) || *p == '_'){
            int len = 1;
            while(isalnum(*(p + len)) ||  *(p + len) == '_')
                len++;
            char *name = strndup(p, len);
            int ty = (intptr_t)map_get(keywords, name);
            if (!ty)
                ty = TK_IDENT;
            add_token(tokens, ty, name);
            if (get_variable(name) < 0)
                vec_push(variables, name);
            p += len;
            continue;
        }

        fprintf(stderr, "トークナイズできません: %s\n", p);
        exit(1);
    }

    add_token(tokens, TK_EOF, p);
    return tokens;
}

Vector *tokenize(char *p){
    keywords = new_map();
    map_put(keywords, "return", (void *)TK_RETURN);
    return scan(p);
}