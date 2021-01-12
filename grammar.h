#ifndef GRAMMAR_H
#define GRAMMAR_H 1 

/*

Грамматика для ввода :

Вычисления: 
statement (declaration (инструкция(объявление(константа | переменная) | выражение(терм(вторичное выражение(первичное выражение ))))) | выход | приглашение)

Инструкция: 
Константное объявление 
Неконстантное объявление 
Выражение 

Константное объявление: 

“const” имя “=” Выражение 

Неконстантное объявление: 

“let” имя “=” выражение 

Выражение: 

Терм ( (“+” | ”-“) Терм)* 

Терм: 

Сэконд( (“*” | “/” | ”%”) Сэконд)* 


Сэконд: 

Первичное выражение (“^”) 


Первичное выражение: 
Число 
“(“ Выражение “)” 
 

*/ 

#include "variable.h"
#include "token.h"
#include <string>
#include <vector>
#include <cmath>


using namespace std;

void inv (Symbol_table& var_table);

void clean_up_mess (Token_stream& ts);

double expression (Token_stream& ts, Symbol_table& var_table);

double primary (Token_stream& ts, Symbol_table& var_table) ;

double secondary(Token_stream& ts, Symbol_table& var_table);

double term (Token_stream& ts, Symbol_table& var_table);

double declaration (Token_stream& ts, Symbol_table& var_table, bool c);

double statement (Token_stream& ts, Symbol_table& var_table);

void calculate (Token_stream& ts, Symbol_table& var_table);

unsigned long long factorial(int n);

void validate(string& name);

void from_f (string name, Symbol_table& var_table);

#endif