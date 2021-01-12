#ifndef VARIABLE_H
#define VARIABLE_H 1 

#include <string>
#include <vector>
#include "token.h"

using namespace std;




const string prompt = "> ";
const string result = "= ";
const string declkey = "let";
const string constkey = "const";


struct Variable {
    string name;
    double value;
    bool con;
    Variable (string n, double v, bool c):
        name{ n }, value{ v },  con {c} { }
};


struct Symbol_table {
	vector <Variable> var_table;
	double get_value (string s);
	double set_value (string s, double d);
	bool is_declared(string s);
	double define_name(string var, double val, bool c);
	void constant();
};


#endif