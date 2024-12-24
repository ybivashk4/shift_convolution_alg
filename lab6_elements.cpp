#include "lab6.h"
#include <stack>
#include <fstream>  
#include <iostream>
std::list<Symbol> input;
std::vector<Symbol> stack;
std::map<std::string, int> all_variables;



static const std::vector<Rule> rules = {
        {'S', "I=E;"},
        {'E', "E+T"},
        {'E', "E-T"},
        {'_', "!L!"},
        {'T', "T*M"},
        {'T', "T/M"},
        {'M', "(E)"},
        {'L', "LS"},
        {'M', "-M"},
        {'L', "S"},
        {'E', "T"},
        {'T', "M"},      
        {'M', "I"},
        {'M', "C"}
};

void set_var(std::string name, int val) {
    all_variables[name] = val;
}

int get_var(const std::string var){
   if (all_variables.find( var ) != all_variables.end()) {
        return all_variables[var];
   }
    throw "Invalid syntax\n";
    
}

void getAllVar() {
    std::cout << "\n\tALL results var is: \n";
    for (const auto & [key, value] : all_variables) {
        std::cout << key << " = " << value << std::endl;
    }
}


Relation find_rel(char x, char y) {

    char matrix[ALPABET_LEN][ALPABET_LEN] = {
          //LSETM=;+-*/()IC!
    /*L*/  " =           < =",
    /*S*/  "             > >",
    /*E*/  "      ===   =   ",
    /*T*/  "      >>>== >   ",
    /*M*/  "      >>>>> >   ",
    /*=*/  "  d<<   <  < << ",
    /*;*/  "             > >",
    /*+*/  "   d<   <  < << ",
    /*-*/  "   dd   <  < << ",
    /***/  "    =   <  < << ",
    /*/*/  "    =   <  < << ",
    /*(*/  "  d<<   <  < << ",          
    /*)*/  "      >>>>> >   ",
    /*I*/  "     =>>>>> >   ",
    /*C*/  "      >>>>> >   ",
    /*!*/  "d<           <  ",
    };

    std::string alpabet = "LSETM=;+-*/()IC!";
    int i = alpabet.find(x);
    int j = alpabet.find(y);
    return (i != std::string::npos && j != std::string::npos) ? static_cast<Relation>(matrix[i][j]) : None;
    
}

/* -1 = ERROR: rule not found */
int g(std::string rule) {

    bool is_find = false;
    int index = -1;
    int i = 0;
    for (auto &a : rules) {
        if (rule == a.get_r()) {
            index = i;
            break;
        }
        i++;
    }
    return index;
}

void run() {
    Symbol first_sym('!');
    stack.push_back(first_sym);
    while (input.size() > 0) {
        Symbol x = stack.back();
        Symbol y = input.front();
        // Поменять алгоритм - идти будем справа налево

        // Алгоритм сдвиг-свёртка
        Relation cur_rel = find_rel(x.symbol, y.symbol);
        std::cout << (char)cur_rel << " " << x.symbol << " " << y.symbol << std::endl;
        if (cur_rel == None) {
            // Ошибка
            throw "Syntax error";
        }
        else if (cur_rel == Before || cur_rel == Together || cur_rel == Dual) {
            // СДВИГ
            stack.push_back(y);
        }
        else {
            // свёртка
            Symbol saved_y = y;
            std::string rule("");
            std::vector<Symbol> symbol_rule;
            // Достаём правило, которое до > и после <
            while (cur_rel != Before) {
                std::string tmp(1, x.symbol);
                symbol_rule.insert(symbol_rule.begin(), x);
                rule = tmp + rule;
                y = x;
                stack.pop_back();
                x = stack.back();
                cur_rel = find_rel(x.symbol, y.symbol);
            }
            
            // Свёртка следующего по длине правила
            int n = g(rule);
            
            while (n == -1 && rule != "") {
                y.symbol = rule.front();
                rule.erase(0);
                symbol_rule.erase(symbol_rule.begin());
                if (rule == "") break;

                x.symbol = rule.front();
                cur_rel = find_rel(x.symbol, y.symbol);
                if (cur_rel == Dual) {
                    n = g(rule);
                }
            }
            
            // Такого правила нет
            if (n == -1) {
                throw "Invalid syntax";
            }

            // замена правила на его левую часть
            int m = g(rule);
            while (m != -1) {
                rule = rules[m].get_l();
                m = g(rule);
            }
            
            // Далее надо по номеру правила выполнить операции
            Symbol new_s(rule[0]);
            switch (++n) {
                case 1:
                    set_var(symbol_rule[0].str_value, symbol_rule[2].int_value);
                    stack.push_back(new_s);
                    input.push_front(input.front());
                    break;
                case 2:
                    new_s.int_value = symbol_rule[0].int_value + symbol_rule[2].int_value;
                    stack.push_back(new_s);
                    input.push_front(input.front());
                    break;
                case 3:
                    new_s.int_value = symbol_rule[0].int_value - symbol_rule[2].int_value;
                    stack.push_back(new_s);
                    input.push_front(input.front());
                    break;
                case 4:
                    std::cout << "parse complete!\n";
                    exit(0);
                    break;
                case 5:
                    new_s.int_value = symbol_rule[0].int_value * symbol_rule[2].int_value;
                    stack.push_back(new_s);
                    input.push_front(input.front());
                    break;
                case 6:
                    new_s.int_value = symbol_rule[0].int_value / symbol_rule[2].int_value;
                    stack.push_back(new_s);
                    input.push_front(input.front());
                    break;
                case 7:
                    new_s.int_value = symbol_rule[1].int_value;
                    stack.push_back(new_s);
                    input.push_front(input.front());
                    break;
                case 8:
                    stack.push_back(new_s);
                    input.push_front(input.front());
                    break;
                case 9:
                    new_s.int_value = -1 * symbol_rule[1].int_value;
                    stack.push_back(new_s);
                    input.push_front(input.front());
                    break;
                case 10:
                    stack.push_back(new_s);
                    input.push_front(input.front());
                    break;
                case 11:
                    new_s.int_value = symbol_rule[0].int_value;
                    stack.push_back(new_s);
                    input.push_front(input.front());
                    break;
                case 12:
                    new_s.int_value = symbol_rule[0].int_value;
                    stack.push_back(new_s);
                    input.push_front(input.front());
                    break;
                case 13:
                    new_s.int_value = get_var(symbol_rule[0].str_value);
                    stack.push_back(new_s);
                    input.push_front(input.front());
                    break;
                case 14:
                    new_s.int_value = symbol_rule[0].int_value;
                    stack.push_back(new_s);
                    input.push_front(input.front());                 
                    break;
                case -1:
                    throw "Syntax error";
                    break;
            }

        }
        input.pop_front();
    } 
}

int main (int argc, char ** argv) {

    Symbol first_symbol;
    first_symbol.symbol = '!';
    stack.push_back(first_symbol);
    Scanner my_scan(argc, argv);

    try {
        my_scan.run(input);
        run();
        getAllVar();
    }
    catch (std::string e) {
        std::cout << e;
    }
    catch (std::exception e) {
        std::cout << e.what();
    }
}


/*
stack.push_back(new_s);
input.push_front(input.front());

repalce

Symbol tmp = stack.back();
stack.pop_back();
stack.push_back(new_s);
input.push_front(input.front());
stack.push_back(new_s);
input.push_front(input.front());

*/