#include <iostream>
#include <string>
#include <vector>
#include "tokens.hpp"
#include "parser.hpp"
#include "symbol_table.hpp"

namespace clex {
    std::vector<Token> tokenize(const std::string& input);
}

int main() {
    clex::SymbolTable symbols; 
    std::string input_line;

    std::cout << "==========================================================================\n";
    std::cout << "=   CALCULADORA LEX + C++                                                =\n";
    std::cout << "=   Escribe 'Hecho por: Raúl Gabaldón Sánchez y Eduardo Rodríguez Hoces  =\n";
    std::cout << "==========================================================================\n";

    while (true) {
        std::cout << "\nIntroduce la sentencia > ";
        
        if (!std::getline(std::cin, input_line)) {
            break;
        }

        if (input_line.empty()) continue; //SI ES VACÍA SE VUELVE A REPETIR

        try {
            //SENTENCIA A TOKENS
            std::vector<clex::Token> tokens = clex::tokenize(input_line);
            
            //PARSEAR
            clex::Parser parser(std::move(tokens));
            auto statement = parser.parse_next_statement();

            //EJECUTAR / EVALUAR
            if(statement.is_expression()) {
                
                clex::Expression expr = statement.move_as_expression();
                double result = expr.evaluate(symbols);
                std::cout << "Resultado: " << result << "\n";
            } else {
                clex::Assignment assign = statement.move_as_assignment();
                assign.execute(symbols);
                std::cout << "Variable '" << *assign.get_var().get_ident() << "' guardada correctamente.\n";
            }

        } catch (const std::exception& e) {

            std::cerr << "ERROR: " << e.what() << "\n";
        }
    }

    return 0;
}