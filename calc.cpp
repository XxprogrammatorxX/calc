#include <iostream>
#include <vector>
#include <string>
#include <cmath>
//#include <stdexcept>
#include "token.h"
#include "variable.h"
#include "grammar.h"

using namespace std;

int main () try {
    Symbol_table var_table;
    var_table.define_name ("pi", 3.141592653589793, true);
    var_table.define_name ("e",  2.718281828459045, true);
    var_table.define_name ("c", 3e8, true); 
    inv(var_table);
    Token_stream ts;
    calculate(ts, var_table);
}   

catch (exception& e) {
    cerr << "exception: " << e.what() << endl;
    return 1;
}

catch (Exit){
    cerr << "bye!" << endl;
    return 1;
}

catch (...) {
    cerr << "Oops, unknown exception" << endl;
    return 2;
}
