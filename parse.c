#include "cc.h"

Vector *main_code;
Vector *sub_code;
int l_num = 1;
Vector *if_component_number;
int brackets;

static Node *new_node(int op, Node *lhs, Node *rhs){
    Node *node = (Node*)malloc(sizeof(Node));
    node->ty = op;
    node->lhs = lhs;
    node->rhs = rhs;
    return node;
}

static Node *new_node_num(int val){
     Node *node = (Node*)malloc(sizeof(Node));
     node->ty = TK_NUM;
     node->val = val;
     return node;
 }

 static Node *new_node_ident(char *name){
     Node *node = (Node*)malloc(sizeof(Node));
     node->ty = TK_IDENT;
     node->name = name;
    return node;
 }

static Node* statement();
static void compound_statement();
static Node* assign();
static Node* expr();
static Node* mul();
static Node* term();

int pos = 0;
Vector *tokens;

void program(Vector *t){
    tokens = t;
    Node *lhs = statement();
    vec_push(main_code, lhs);
    if (((Token*)(tokens->data[pos]))->ty == TK_EOF)
        return;
    program(t);
}

static Node* statement(){
    if(((Token*)(tokens->data[pos]))->ty == TK_IF){
        pos++;
        if_component_number->data[if_component_number->len - 1]++;
        if (((Token*)(tokens->data[pos]))->ty == '('){
            pos++;
            Node *lhs = assign();
            if (((Token*)(tokens->data[pos]))->ty == ')'){
                pos++;
                l_num += 2;
                if (((Token*)(tokens->data[pos]))->ty == '{'){
                    pos++;
                    compound_statement();
                    brackets++;
                }
                else{
                    vec_push(main_code, assign());
                }
            }
            return new_node(TK_IF, lhs, NULL);
        }
    }
    if(((Token*)(tokens->data[pos]))->ty == TK_RETURN){
        pos++;
        Node* lhs = assign();
        return new_node(TK_RETURN, lhs, NULL);
    }
    return assign();
}

static void compound_statement(){
    Node *lhs = statement();
    vec_push(main_code, lhs);
    if (((Token*)(tokens->data[pos++]))->ty == '}'){
        brackets--;
        if (brackets == 0){
            vec_push(if_component_number, 0);
        }
        return;
    }
    compound_statement();
}

static Node* assign(){
    Node* lhs = expr();
    if (((Token*)(tokens->data[pos]))->ty == ';'){
        pos++;
        return lhs;
    }
    if (((Token*)(tokens->data[pos]))->ty == '=' ||
    ((Token*)(tokens->data[pos]))->ty == TK_EQUAL ||
    ((Token*)(tokens->data[pos]))->ty == TK_NEQUAL){
        int ty = ((Token *)(tokens->data[pos]))->ty;
        pos++;
        return  new_node(ty, lhs, assign());
    }
    return lhs;
}

static Node* expr(){
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

static Node* mul(){
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

static Node* term(){
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