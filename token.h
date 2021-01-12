#ifndef TOKEN_H
#define TOKEN_H 1

#include <string>
#include <vector>
#include <iostream>

using namespace std;


constexpr char print = ';';
constexpr char number = '8';
constexpr char name = 'a';
constexpr char let = 'L';
constexpr char cnst = 'c';
constexpr char help = 'h';
constexpr char from = 'f';
const string quit = "quit";


class Bad_input {};
class Exit {};

struct Token{                   
    char kind;
    double value;
    string name;

    Token (char ch):
        kind{ ch }, value{ 0 } { }

    Token (char ch, double val):
        kind{ ch }, value{ val } { }
    Token (char ch, string n):
        kind {ch}, name {n} {}
};

class Token_stream {                //свой поток ввода
    bool full{ false };
    Token buffer{ '\0' };
    bool full2 { false };
    Token buffer2{ '\0' };

    public:
        
        Token_stream () : in {cin} { }
        Token_stream(istream& s) : in {s} { }

    	Token get ();
        string getname();
        void putback (Token t);
		void ignore (char);
        char ignore_space();
    private:
        istream& in;
};

#endif