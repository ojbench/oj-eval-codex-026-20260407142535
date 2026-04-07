#include "calc.h"
#include <bits/stdc++.h>
using namespace std;

// A tiny recursive descent parser for +,-,*,/ and parentheses
// It builds the AST nodes defined in visitor.h and evaluates via calculator.

static string s; size_t iPos;

node* parseExpr();
node* parseTerm();
node* parseFactor();

void skipSpaces(){ while(iPos < s.size() && isspace((unsigned char)s[iPos])) ++iPos; }

node* parseNumber(){
    skipSpaces();
    size_t start = iPos;
    bool hasDot = false;
    if (iPos < s.size() && (s[iPos] == '+' || s[iPos] == '-')) ++iPos;
    while(iPos < s.size() && (isdigit((unsigned char)s[iPos]) || s[iPos]=='.')){
        if(s[iPos]=='.') hasDot = true;
        ++iPos;
    }
    string tok = s.substr(start, iPos-start);
    num_node *n = new num_node;
    if (hasDot) n->number = stod(tok); else n->number = (long long)stoll(tok);
    return n;
}

node* parseFactor(){
    skipSpaces();
    if (iPos < s.size() && s[iPos]=='('){
        ++iPos; node* e = parseExpr(); skipSpaces(); if(iPos < s.size() && s[iPos]==')') ++iPos; return e;
    }
    return parseNumber();
}

node* bindBin(node* l, node* r, char op){
    if (op=='+'){ auto *n = new add_node; n->lnode=l; n->rnode=r; return n; }
    if (op=='-'){ auto *n = new sub_node; n->lnode=l; n->rnode=r; return n; }
    if (op=='*'){ auto *n = new mul_node; n->lnode=l; n->rnode=r; return n; }
    auto *n = new div_node; n->lnode=l; n->rnode=r; return n;
}

node* parseTerm(){
    node* l = parseFactor();
    while(true){
        skipSpaces();
        if (iPos < s.size() && (s[iPos]=='*' || s[iPos]=='/')){
            char op = s[iPos++];
            node* r = parseFactor();
            l = bindBin(l, r, op);
        } else break;
    }
    return l;
}

node* parseExpr(){
    node* l = parseTerm();
    while(true){
        skipSpaces();
        if (iPos < s.size() && (s[iPos]=='+' || s[iPos]=='-')){
            char op = s[iPos++];
            node* r = parseTerm();
            l = bindBin(l, r, op);
        } else break;
    }
    return l;
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    if(!getline(cin, s)) return 0;
    iPos = 0;
    node* root = parseExpr();
    calculator calc;
    auto res = calc.visit(root);
    if (auto pd = any_cast<double>(&res)){
        cout.setf(ios::fixed); cout<<setprecision(10)<<*pd<<"\n";
    } else if (auto pi = any_cast<long long>(&res)){
        cout<<*pi<<"\n";
    }
    return 0;
}
