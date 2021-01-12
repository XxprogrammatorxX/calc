#include <fstream>
#include <string>
#include <iostream>
#include "grammar.h"


void clean_up_mess (Token_stream& ts) {
    ts.ignore (print);
}

double primary (Token_stream& ts, Symbol_table& var_table) {  
    Token t = ts.get();
    switch (t.kind) {
        case '(': {
            double d = expression(ts, var_table);
            t = ts.get();
            if (t.kind != ')')
                throw runtime_error{"'(' expected"};
            return d;
        }
        case '{': {
            double d = expression(ts, var_table);
            t = ts.get();
            if (t.kind != '}')
                throw runtime_error{"'{' expected"};
            return d;
        }

        case '-':
            return -primary(ts, var_table);
        case '+':
            return +primary(ts, var_table);

        case number:
            return t.value;
        case name:
            return var_table.get_value(t.name);

        default:
            throw runtime_error{"primary expected"};
    }
}

double secondary(Token_stream& ts, Symbol_table& var_table){
    double left = primary(ts, var_table);
    Token t = ts.get();
    switch (t.kind){
            case '^':{
                double d = primary(ts, var_table);
                return pow (left, d);
            }
            case '!':{
                Token sect = ts.get();
                if (sect.kind == '!') 
                    throw runtime_error{"'!!' not definded"};
                    ts.putback(sect);
                if (left>20)
                    throw runtime_error{"factorial greater than 20 cant be calculated"};
                if (int (left) == left)
                    return factorial(left);
                else
                    throw runtime_error {"not integer value for factorial"};
            }
            default :{
                ts.putback(t);
                return left;
            }
    }
}


double term (Token_stream& ts, Symbol_table& var_table) {
    double left = secondary(ts, var_table);
    while (true){
        Token t = ts.get();

        switch (t.kind) {
            case '*':
                left *= secondary(ts, var_table);
                break;

            case '/': {
                double d = secondary(ts, var_table);
                if (d == 0) throw runtime_error{"divide by zero"};
                left /= d;
                break;
            }
            case '%': {
                double d = secondary(ts, var_table);
                if (d==0) throw runtime_error{"divide by zero "};
                left = fmod(left, d);
                //t = ts.get();
                break;
            }

            default:
                ts.putback(t);
                return left;
        }
    }
}


double expression (Token_stream& ts, Symbol_table& var_table) {
    double left = term(ts, var_table);
    while (true) {

        Token t = ts.get();

        switch (t.kind) {
            case '+':{
                Token t2 = ts.get();
                if (t2.kind == '+'  || t2.kind == '-'){
                    throw runtime_error{"too much + or -"};
                }
                ts.putback(t2);
                left += term(ts, var_table);
                break;
            }

            case '-':
                left -= term(ts, var_table);
                break;

            default:
                ts.putback(t);
                return left;
        }
    }
}

double declaration (Token_stream& ts, Symbol_table& var_table, bool c) {    
    Token t = ts.get();
        if (t.kind != name)
            throw runtime_error{"name expected in declaration"};

    string var = t.name;     

    if (var_table.is_declared(var))//проверка объявлена переменная или нет
        throw runtime_error{" declared twice"};

    t = ts.get();
        if (t.kind != '=')
            throw runtime_error{"'=' missing in declaration of "};  //проверка ошибок объявления 

    double d = expression(ts, var_table);
    var_table.define_name(var, d, c);
    return d;
}


double statement (Token_stream& ts, Symbol_table& var_table) {    //работа с переменными
    Token t = ts.get();
    switch (t.kind) {
        case let:
            return declaration(ts, var_table, false);
        case cnst:
            return declaration(ts, var_table, true);
        case from:{
            string s;
            s = ts.getname();
            from_f(s, var_table);
            ts.putback(Token { print });
            //break;
        }
        case name:{
            Token t2 = ts.get();
            if (t2.kind == '=')     
                return var_table.set_value(t.name, expression(ts, var_table));
            
            ts.putback(t2);
            ts.putback(t);

            return expression(ts, var_table);

        }
        default:
            ts.putback(t);
            return expression(ts, var_table);
    }
}


void calculate (Token_stream& ts, Symbol_table& var_table) {             //основной цикл
    
    
    while (true) try {
        cout << prompt;
        Token t = ts.get();
        while (t.kind == print)
            t = ts.get();
        if (t.kind == help)
            inv(var_table);
        else {
            while (t.kind == print)
                t = ts.get();
            ts.putback(t);
            double d = statement(ts, var_table);
            cout << '\r' <<  result << d << endl;
        }
    }
    catch (Bad_input) {
        return;
    }
    catch (runtime_error& e) {
        cerr << e.what() << endl;
        clean_up_mess(ts);
    }
}


void inv (Symbol_table& var_table) {
    cout << "\n\n\n";
    cout << "\t\t\t***calc***\n" 
    << "Operands: +, -, *, /, % \n"
    << "You can use pow by '^'(supports fractional and negative),\nfactorial by '!' and parenthesis '(' ')'\n"
    << "To define a variable enter 'let <name> = <expression>'\n"
    << "For using files enter 'from <*.*>' \n'> primary expected' means, that file ended\n"
    << "\n\t !!! File MUST have ';' at the end !!!\n\n"
    << "To define a constant enter 'const <name> = <expression>'\n"
    << "To assign a value to a variable '<name> = expression'\n"
    << "Defined const: \n";
    var_table.constant();
    cout << "For exit print quit, for print answer enter ';' \n"
    << "to output an instruction print 'help'\n\n\n" << endl; 
}

unsigned long long factorial(int n){
    if (n<0){
        throw runtime_error("value less than zero");
    }
    if (n==0){
        return 1;
    }
    if (n>0){
        unsigned long long rez=1;
        for (int i=1; i<n+1; i++){
            rez*=i;
        }
        return rez;
    }
    return 1;
}

void validate(string& ifn){
    if (ifn[ifn.length() - 1] == ';')
        ifn = string (ifn.begin(),  ifn.begin() + ifn.length() - 2);
    if (ifn[ifn.length() - 1] == '"')
        ifn = string (ifn.begin(),  ifn.begin() + ifn.length() - 2);
    
    if (ifn[0] == '"')
        ifn = string (ifn.begin() + 1, ifn.begin() + ifn.length() + 1);
}

void from_f (string ifn, Symbol_table& var_table){
    validate(ifn);
    //cout << "_" << ifn;

    ifstream ifs {ifn};
    if (!ifs) 
        throw runtime_error {"cant open_" + ifn + "_file\n"};

    Token_stream tts { ifs };
    calculate(tts, var_table);
}