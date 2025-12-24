#include <cctype>
#include <iostream>
#include <string>
#include <vector>
#include "tokens.hpp"
#include "parser.hpp"
#include "symbol_table.hpp"
#include "parser_errors.hpp"
#include "eval_errors.hpp"
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
        
        if(!std::getline(std::cin, input_line)) {
            break;
        }

        if(input_line.empty()) continue; //SI ES VACÍA SE VUELVE A REPETIR
        if(input_line == "exit" || input_line == "quit") {
            std::cout << "Saliendo...\n";
            break;
        }
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

        } catch (const clex::ParserError& e) {

            std::cerr << "ERROR DE SINTAXIS: " << e.what() << "\n";
            e.print_to(std::cerr);
            std::cerr << "\n";
        
        } catch (const clex::EvalError& e){
            std:: cerr << "ERROR DE EVALUACIÓN: ";
            e.print_to(std::cerr);
            std::cerr << "\n";
        } catch (const std::exception e){
            std::cerr << "ERROR: " << e.what() << "\n";
        }
    }

    return 0;
}