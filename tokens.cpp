#include "tokens.hpp"
#include <stdexcept>
#include <variant>

namespace clex {

Token::Token(TokenType type, std::variant<std::monostate, double, std::string>&& info) noexcept :
    m_type(type), m_info(info) {};

Token::Token() : m_type(TokenType::ERROR_TOKEN), m_info() {};
Token::Token(TokenType type) : m_type(type), m_info() {
    if(type == TokenType::NUMBER || type == TokenType::IDENTIFIER) {
        throw new std::invalid_argument("No token info provided for number/identifier token. Use Token::from_number() or Token::from_ident() instead");
    }
}

Token Token::from_ident(const std::string &str) noexcept {
    return Token{
        TokenType::IDENTIFIER,
        std::move(std::variant<std::monostate, double, std::string>{str})
    };
}

Token Token::from_number(double num) noexcept {
    return Token{
        TokenType::NUMBER,
        std::move(std::variant<std::monostate, double, std::string>{num})
    };
}

TokenType Token::type() const noexcept { return m_type; }
std::optional<std::string> Token::get_ident() const noexcept {
    if(std::holds_alternative<std::string>(m_info)) {
        return std::get<std::string>(m_info);
    } else {
        return {};
    }
}

std::optional<double> Token::get_num() const noexcept {
    if(std::holds_alternative<double>(m_info)) {
        return std::get<double>(m_info);
    } else {
        return {};
    }
}

std::ostream& operator<<(std::ostream& out, const Token& tok) noexcept {
    switch(tok.m_type) {
      case TokenType::ERROR_TOKEN: {
        return out << "<Error token>";
      }
      case TokenType::END_OF_FILE: {
        return out << "<EOF>";
      }
      case TokenType::NUMBER: {
        return out << "<Number " << std::get<double>(tok.m_info) << '>';
      }
      case TokenType::IDENTIFIER: {
        return out << "<Identifier " << std::get<std::string>(tok.m_info) << '>';
      }
      case TokenType::OP_PLUS: {
        return out << "<Plus>";
      }
      case TokenType::OP_MINUS: {
        return out << "<Minus>";
      }
      case TokenType::OP_ASTERISK: {
        return out << "<Asterisk>";
      }
      case TokenType::OP_SLASH: {
        return out << "<Slash>";
      }
      case TokenType::OP_CARET: {
        return out << "<Caret>";
      }
      default: {
        return out << "<Invalid token type>";
      }
    }
}

} // namespace clex
