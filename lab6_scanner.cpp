#include <iostream>
#include <cctype>
#include <cstdlib>
#include <cstring>
#include "lab6.h"
#include <string>


Scanner::Scanner(int argc, char ** argv) {
    if (argc > 1) {
        file_read.open(argv[1], std::ios_base::in);
        file_read >> read_s;
        if (!file_read.is_open()) {
            throw "can't open file";
        }
        native = "=;+-*/()";
    }
    else {
        throw "Few arguments - need input file and output file";
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
        throw "Unknown symbol";
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

// // Выведем все записаные лексемы на экран
// void test_Scanner(int argc, char ** argv) {
//     std::ifstream is;
//     is.open(argv[2], std::ios::binary|std::ios::in);
//     if (is.is_open()) {
//         std::cout << "хайп " << argv[2];
//     }
//     is.seekg (0, is.end);
//     int length = is.tellg();
//     is.seekg (0, is.beg);
//     std::cout << "len is " << length << std::endl;
//     Symbol * buffer = new Symbol[length];
//     is.read((char *)buffer, length);
//     for (int i=0;i<length / sizeof(Symbol);i++) {
//         buffer[i].out();
//     }
//     is.close();
//     // delete[] buffer;
// }

// int main (int argc, char ** argv) {
//     int error = 0;
//     try {
//         Scanner main_scanner(argc, argv);
//         main_scanner.run();
//         // test_Scanner(argc, argv);
//     }
//     catch (std::string msg) {
//         std::cout << "You have an error\n" << msg << std::endl;
//         error = 1;
//     }
//     catch(std::exception e) {
//         std::cout << e.what() << std::endl;
//         error = 2;
//     }
//     return error;
// }