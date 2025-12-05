#include "parser_errors.hpp"
#include "symbol_table.hpp"
#include "tokens.hpp"
#include "parser.hpp"
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <utility>
#include <vector>

int main(int argc, char** argv) {
    std::vector<clex::Token> tests[] = {
        {
            clex::Token::number("2"),
            clex::Token(clex::TokenType::OP_PLUS),
            clex::Token::number("2"),
        },
        {
            clex::Token::number("2"),
            clex::Token(clex::TokenType::OP_PLUS),
            clex::Token::number("2"),
            clex::Token(clex::TokenType::OP_PLUS),
            clex::Token::number("2"),
            clex::Token(clex::TokenType::OP_PLUS),
            clex::Token::number("2"),
            clex::Token(clex::TokenType::OP_PLUS),
            clex::Token::number("2"),
        },
        {
            clex::Token::number("3.5"),
            clex::Token(clex::TokenType::OP_MINUS),
            clex::Token::identifier("abc"),
            clex::Token(clex::TokenType::OP_CARET),
            clex::Token::number("2"),
            clex::Token(clex::TokenType::OP_ASTERISK),
            clex::Token::identifier("xyz")
        },
        {
            clex::Token(clex::TokenType::OP_SLASH),
            clex::Token::identifier("abc"),
            clex::Token::number("2"),
        },
        {
            clex::Token::number("2"),
            clex::Token(clex::TokenType::OP_CARET),
            clex::Token(clex::TokenType::PAREN_L),
            clex::Token(clex::TokenType::PAREN_L),
            clex::Token::number("1"),
            clex::Token(clex::TokenType::OP_PLUS),
            clex::Token::number("1"),
            clex::Token(clex::TokenType::PAREN_R),
            clex::Token(clex::TokenType::OP_ASTERISK),
            clex::Token::number("3"),
            clex::Token(clex::TokenType::PAREN_R),
        },
    };

    clex::SymbolTable symbols;
    symbols.set(clex::Token::identifier("abc"), 1.0);
    symbols.set(clex::Token::identifier("xyz"), 2.0);

    size_t test_number = -1;
    if(argc > 1) {
        test_number = std::atoll(argv[1]);
    } else {
        std::cout << "Por favor, introduzca número de test (0-3): ";
        std::cin >> test_number;
    }

    std::vector<clex::Token>& toks = tests[test_number];
    std::cout << "Lista de tokens inicial: ";
    for(const clex::Token& tok : toks) {
        std::cout << tok << ' ';
    }
    std::cout << '\n';
    clex::Parser parser = clex::Parser(std::move(toks));
    try {
        auto expr = parser.parse_expression();
        if(expr != nullptr) {
            std::cout << "Resultado: " << *expr << '\n';
            double expr_val;
            try {
                expr_val = expr->evaluate(symbols);
            } catch(int i) {
                std::cout << "código de error " << i << '\n';
                exit(321534);
            }
            std::cout << "Tras evaluar la expresión, el valor calculado es: " << expr_val << '\n';
        } else {
            std::cout << "¡Expresión resultante nula!\n";
        }
    } catch(clex::ParserError& err) {
        std::cerr << "Se ha detectado un error sintáctico en la expresión:\n";
        err.print_to(std::cerr);
    }

}