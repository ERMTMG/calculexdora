#include "parser_errors.hpp"
#include "tokens.hpp"
#include <cstdlib>
#include <ostream>
#include <sstream>
#include <utility>
#include <vector>

namespace clex {

ParserError::ParserError(std::string&& message) noexcept : m_message(std::move(message)) {}; 

const std::string& ParserError::what() const noexcept {
    return m_message;
}

void ParserError::print_to(std::ostream& out) const noexcept {
    out << "<GENERIC PARSER ERROR> " << this->what();
}

ExpectedToken::ExpectedToken(std::vector<TokenType>&& expected_tokens, Token actual_token) noexcept : ParserError("") {
    std::stringstream msg;
    if(expected_tokens.empty()) {
        std::cerr << "Invalid exception constructed";
        std::abort();
    }
    if(expected_tokens.size() < 2) {
        msg << "Expected token " << expected_tokens[0] << ", ";
    } else {
        msg << "Expected one of ";
        for(TokenType& token_type : expected_tokens) {
            msg << token_type << ", ";
        }
    }
    msg << "got " << actual_token << '\n';
    m_message = msg.str();
}

void ExpectedToken::print_to(std::ostream& out) const noexcept {
    out << "<INVALID TOKEN> " << this->what();
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
    out << "<EXPECTED OPERATOR> " << this->what();
}

MismatchedParentheses::MismatchedParentheses(Token paren_token, Token nearby_token) noexcept : ParserError("") {
    std::stringstream msg{"Mismatched parenthesis "};
    msg << paren_token << " near token " << nearby_token << '\n';
    m_message = msg.str();
}

void MismatchedParentheses::print_to(std::ostream& out) const noexcept {
    out << "<MISMATCHED PARENTHESES> " << this->what();
}

}