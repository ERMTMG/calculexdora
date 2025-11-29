#include "tokens.hpp"
#include "token_list.hpp"
#include "parser.hpp"
#include <iostream>
#include <utility>
#include <vector>

int main(int, char**) {
    std::vector<clex::Token> toks = {
        clex::Token::number("3.14"),
        clex::Token(clex::TokenType::OP_MINUS),
        clex::Token::identifier("asafdsg")
    };
    std::cout << "Lista de tokens inicial: ";
    for(const clex::Token& tok : toks) {
        std::cout << tok << ' ';
    }
    std::cout << '\n';
    clex::TokenList toks_list{std::move(toks)}; 
    clex::Parser parser = clex::Parser(std::move(toks_list));
    auto expr = parser.parse_expression();

    if(expr != nullptr) {
        std::cout << "Resultado: " << *expr << '\n';
    }
}