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
            clex::Token::identifier("2"),
        },
        {
            clex::Token::number("2"),
            clex::Token(clex::TokenType::OP_PLUS),
            clex::Token::identifier("2"),
            clex::Token(clex::TokenType::OP_PLUS),
            clex::Token::identifier("2"),
            clex::Token(clex::TokenType::OP_PLUS),
            clex::Token::identifier("2"),
            clex::Token(clex::TokenType::OP_PLUS),
            clex::Token::identifier("2"),
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
    };
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
    auto expr = parser.parse_expression();

    if(expr != nullptr) {
        std::cout << "Resultado: " << *expr << '\n';
    } else {
        std::cout << "¡Expresión resultante nula!\n";
    }
}