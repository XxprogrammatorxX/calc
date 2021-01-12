#include "token.h"
#include "variable.h"

void Token_stream::putback (Token t) { 
    if (full2)
        throw runtime_error{"putback() into a full buffer"};

    if (full){
        full2 = true;
        buffer2 = t;
        return;
    }

    buffer = t;
    full = true;
}

char Token_stream::ignore_space(){
    char c;
    while (in.get(c)){
        if (isspace(c)){
            if (c == '\n') return print;
        }
        else return c;
    }
    return print;
}

string Token_stream::getname(){
    string name;
    char ch = ' ';
    while (isspace(ch)) in.get(ch);
    if (ch == '"'){
        getline(in, name);
        return name;
    }
    getline(in, name);
    string s = "";
    s += ch;
    s += name;
    return s;
}

Token Token_stream::get () {

    if (full2){
        full2 = false;
        return buffer2;
    }


    if (full) {
        full = false;
        return buffer;
    } 

    char ch;
    in.get(ch);

    if (!in){
        throw Bad_input{};
    }

    if (isspace(ch)){
        if (ch  !=  ' ' && ch  !=  '\t') return Token { print };
        ch = ignore_space();
    }

    switch (ch) {
        case '+':
        case '-':
        case '*':
        case '/':
        case '%': 
        case '(':
        case ')':
        case ';':
        case '=':
        case '^':
        case '!':
        case '{':
        case '}':
            return Token{ ch };

        case '.':
        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9': {
            in.putback(ch);
            double val;
            in >> val;
            return Token{ number, val };
        }

        default:                        //проверка, а не переменная ли это
            if (isalpha(ch)) {
                string s;
                s += ch;
                while (in.get(ch) && (isalpha(ch) || isdigit(ch) || ch == '_'))
                    s += ch;
                in.putback(ch);
                if (s == quit)
                    throw Exit {};
                if (s == "help"){
                    return Token { help };
                }
                if (s == "from"){
                    return Token { from };
                }
                if (s == declkey) 
                    return Token{ let };
                if (s == constkey)
                    return Token{ cnst };
                return Token {name, s};
            }
    throw runtime_error{"Bad token"};
    }
}


void Token_stream::ignore (char c) {            //пропуск всего до следующей команды
    if (full && c == buffer.kind){
        full = false;
        return;
    }
    full2 = false;
    full = false;
    char ch;
    while (in.get(ch))
        if (ch == c || ch == '\n') {
            putback(print);
            return;
        }
}
