#ifndef LAB6
#define LAB6


#include <cstdlib>
#include <string>
#include <vector>
#include <iostream>
#include <list>
#include <map>
#include <algorithm>
#include <fstream>

class Rule {
    private:
        char left;
        std::string right;
    public:
        Rule(char l, std::string r) : left(l), right(r) {};
        char get_l() const {return left;};
        std::string get_r() const {return right;};
        void set(char l) {left = l;};
        void set(std::string r) {right = r;};
};

enum Relation { 
    None=' ', Before='<', Together='=', After='>', Dual='d' 
};

enum Help_e {
    ALPABET_LEN=17
};

class Symbol {

    public:
        char symbol;
        Relation rel;
        std::string str_value;
        int int_value;
        Symbol() : symbol(EOF), rel(None), str_value(""), int_value(0) {};
        Symbol(char _symbol) : symbol(_symbol), rel(None), str_value(""), int_value(0) {};
        void out() {
            std::cout << int_value << " " << str_value << " " << symbol << std::endl;
        }

};

class Scanner {
    private:
        std::string native;
        std::ifstream file_read;
        char read_s; // Считанный символ
        static bool is_o_digit(char symbol);
    public:
        Scanner(int argc, char ** argv);
        ~Scanner();
        void get();
        Symbol get_lexem(); // return lexem 
        void run(std::list<Symbol> &inp); // out binary classes as lexems in out file 

};

struct triad {
    int index;
    char op;
    std::string operand1;
    std::string operand2;
    bool is_visible;
};

#endif