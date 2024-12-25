#include <iostream>
#include <cctype>
#include <cstdlib>
#include <cstring>
#include "lab6.h"
#include <string>

static void MyError(std::string msg) {
    std::cout << "Scanner error: " << msg << std::endl;
    exit(1);
}

Scanner::Scanner(int argc, char ** argv) {
    if (argc > 1) {
        file_read.open(argv[1], std::ios_base::in);
        file_read >> read_s;
        if (!file_read.is_open()) {
            MyError("can't open file");
        }
        native = "=;+-*/()";
    }
    else {
        MyError("Few arguments - need input file and output file");
    }   
}

Scanner::~Scanner() {
    if (file_read.is_open()) file_read.close();
}

bool Scanner::is_o_digit(char symbol) {
    return isdigit(symbol) && symbol != '8' && symbol != '9';
}

void Scanner::get() {
    file_read >> read_s;
}

Symbol Scanner::get_lexem() {
    Symbol ret_symb;
    while (isspace(read_s)) get();
    if (file_read.eof()) {
        ret_symb.symbol = '!';
    }
    else if (isalpha(read_s) || read_s == '_') {
        std::string value_I = "";
        value_I += read_s;
        get();
        while (isalpha(read_s) || is_o_digit(read_s) || read_s == '_') {
            value_I += read_s;
            get();
        }
        ret_symb.symbol = 'I';
        ret_symb.str_value = value_I;
    }
    else if (is_o_digit(read_s)) {
        int temp_val = 0;
        std::string temp;
        while (is_o_digit(read_s)) {
            temp += read_s;
            get();
        }
        temp_val = std::stoi(temp, (std::size_t *)0, 8);
        ret_symb.symbol = 'C'; 
        ret_symb.int_value = temp_val;
    }
    else if (strchr(native.data(), read_s)) {
        ret_symb.symbol = read_s;
        get();
    }
    else
        MyError("Unknown symbol");
    return ret_symb;
}

void Scanner::run(std::list<Symbol> &inp) {
    Symbol out_s;
    do {
        out_s = get_lexem();
        inp.push_back(out_s);
    }
    while (out_s.symbol != '!');
    file_read.close();
}