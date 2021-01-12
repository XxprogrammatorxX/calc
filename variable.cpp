#include "variable.h"


double Symbol_table::get_value (string s) {
    for (int i = 0; i < var_table.size(); ++i)
        if (var_table[i].name == s)
            return var_table[i].value;

    throw runtime_error{"get: undefined name " + s};
}

double Symbol_table::set_value (string s, double d) {
    for (int i = 0; i < var_table.size(); ++i) {
        if (var_table[i].name == s) {
            if (var_table[i].con){
                throw runtime_error{"its const variable"};
            }
            var_table[i].value = d;
            return d;
        }
    }
    throw runtime_error{"set: undefined name " + s};
}

bool Symbol_table::is_declared (string s){
    for (int i = 0; i < var_table.size(); ++i)
        if (var_table[i].name == s) return true;
    return false;  
}

double Symbol_table::define_name (string var, double val, bool c) {
    if (is_declared(var))
        throw runtime_error{var + " declared twice"};

    var_table.push_back (Variable{ var, val, c });

    return val;
}

void Symbol_table::constant (){
    for (int i=0; i<var_table.size(); i++){
        if (var_table[i].con)
            cout << "\t" << var_table[i].name << " = " << var_table[i].value << '\n';
    }
}