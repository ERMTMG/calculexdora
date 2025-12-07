#include "parser_errors.hpp"
#include "tokens.hpp"
#include <cstdlib>
#include <ostream>
#include <sstream>
#include <utility>
#include <vector>

namespace clex {

ParserError::ParserError(std::string&& message, Token problem_token) noexcept : m_message(std::move(message)), m_problem_token(problem_token) {}; 

const std::string& ParserError::what() const noexcept {
    return m_message;
}

void ParserError::print_to(std::ostream& out) const noexcept {
    out << "<ERROR DE SINTAXIS> " << this->what();
}

Token ParserError::problem_token() const noexcept {
    return m_problem_token;
}

ExpectedToken::ExpectedToken(std::vector<TokenType>&& expected_tokens, Token actual_token) noexcept : ParserError("", actual_token) {
    std::stringstream msg;
    if(expected_tokens.empty()) {
        std::cerr << "Se ha construido una excepción inválida (esto no debería pasar)";
        std::abort();
    }
    if(expected_tokens.size() < 2) {
        msg << "Se esperaba un token de tipo " << expected_tokens[0] << ", ";
    } else {
        msg << "Se esperaba un token de uno de los siguientes tipos: ";
        for(TokenType& token_type : expected_tokens) {
            msg << token_type << ", ";
        }
    }
    msg << "pero se obtuvo " << actual_token << '\n';
    m_message = msg.str();
}

void ExpectedToken::print_to(std::ostream& out) const noexcept {
    out << "<TOKEN INVÁLIDO> " << this->what();
}

ExpectedOperator::ExpectedOperator(Token actual_token) noexcept : 
    ExpectedToken(
        {
            TokenType::OP_PLUS,
            TokenType::OP_MINUS,
            TokenType::OP_ASTERISK,
            TokenType::OP_SLASH,
            TokenType::OP_CARET,
        }, 
        actual_token
    ) {};

void ExpectedOperator::print_to(std::ostream& out) const noexcept {
    out << "<OPERADOR ESPERADO> " << this->what();
}

MismatchedParentheses::MismatchedParentheses(Token paren_token, Token nearby_token) noexcept : ParserError("", nearby_token) {
    std::stringstream msg{"Paréntesis desparejo "};
    msg << paren_token << " cerca del token " << nearby_token << '\n';
    m_message = msg.str();
}

void MismatchedParentheses::print_to(std::ostream& out) const noexcept {
    out << "<PARÉNTESIS DESPAREJO> " << this->what();
}

}