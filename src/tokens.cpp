#include "tokens.hpp"
#include <optional>
#include <ostream>
#include <stdexcept>
#include <string>

namespace clex {

std::ostream& operator<<(std::ostream& out, TokenType token_type) noexcept {
    switch(token_type) {
      case TokenType::ERROR_TOKEN: {
        return out << "<Error token>";
      }
      case TokenType::END_OF_FILE: {
        return out << "<EOF>";
      }
      case TokenType::NUMBER: {
        return out << "Number";
      }
      case TokenType::IDENTIFIER: {
        return out << "Identifier";
      }
      case TokenType::OP_PLUS: {
        return out << "Plus ('+')";
      }
      case TokenType::OP_MINUS: {
        return out << "Minus ('-')";
      }
      case TokenType::OP_ASTERISK: {
        return out << "Asterisk ('*')";
      }
      case TokenType::OP_SLASH: {
        return out << "Slash ('/')";
      }
      case TokenType::OP_CARET: {
        return out << "Caret ('^')";
      }
      case TokenType::ASSIGN: {
        return out << "Assign ('=')";
      }
      case TokenType::PAREN_L: {
        return out << "Left Parenthesis ('(')";
      }
      case TokenType::PAREN_R: {
        return out << "Right Parenthesis (')')";
      }
      default: {
        return out << "<Invalid token type>";
      }
    }
}

Token::Token(TokenType type, Token::TokenVariant&& data) noexcept :
    m_data(data), m_type(type) {};

Token::Token() : m_data(), m_type(TokenType::ERROR_TOKEN) {};

Token::Token(TokenType type) : m_data(), m_type(type) {
    if(type == TokenType::NUMBER || type == TokenType::IDENTIFIER) {
        throw std::invalid_argument("No token info provided for number/identifier token. Use Token::from_number() or Token::from_ident() instead");
    }
}

Token Token::identifier(const std::string &str) noexcept {
    return Token {
        TokenType::IDENTIFIER,
        TokenVariant(str)
    };
}

Token Token::number(const std::string& num) noexcept {
    return Token {
        TokenType::NUMBER,
        TokenVariant(std::stod(num))
    };
}

TokenType Token::type() const noexcept { return m_type; }

std::optional<std::string> Token::get_ident() const noexcept {
    if(m_type == TokenType::IDENTIFIER) {
        return std::get<std::string>(m_data);
    } else {
        return {};
    }
}

std::optional<double> Token::get_num() const noexcept {
    if(m_type == TokenType::NUMBER) {
        return std::get<double>(m_data);
    } else {
        return {};
    }
}

std::optional<int> Token::get_binary_binding_power() const noexcept {
    switch(m_type) {
      case TokenType::OP_PLUS:
      case TokenType::OP_MINUS: {
          return 1;
      }
      case TokenType::OP_ASTERISK:
      case TokenType::OP_SLASH: {
          return 2;
      }
      case TokenType::OP_CARET: {
          return 3;
      }
      default: return {};
    }
}

std::optional<int> Token::get_unary_binding_power() const noexcept {
    switch(m_type) {
      case TokenType::OP_PLUS:
      case TokenType::OP_MINUS: {
          return 5;
      }
      default: return {};
    }
}

bool Token::operator==(const Token& rhs) const noexcept {
    if(this->m_type != rhs.m_type) {
        return false;
    } else {
        switch(m_type) {
          case TokenType::IDENTIFIER: {
            return std::get<std::string>(m_data) == std::get<std::string>(rhs.m_data);
          }
          case TokenType::NUMBER: {
            return std::get<double>(m_data) == std::get<double>(rhs.m_data);
          }
          default: {
            return true;
          }
        }
    }
}

bool Token::operator!=(const Token& rhs) const noexcept {
    return !(*this == rhs);
}

bool Token::is_unary_operator_token() const noexcept {
    return (
        m_type == TokenType::OP_MINUS
     || m_type == TokenType::OP_PLUS
    );
}

bool Token::is_binary_operator_token() const noexcept {
    return (
        m_type == TokenType::OP_PLUS 
     || m_type == TokenType::OP_MINUS
     || m_type == TokenType::OP_ASTERISK
     || m_type == TokenType::OP_SLASH
     || m_type == TokenType::OP_CARET
    );
}

bool Token::is_right_associative() const noexcept {
    return (
        m_type == TokenType::OP_CARET
    );
}

bool Token::is_operator_token() const noexcept {
    return is_unary_operator_token() || is_binary_operator_token();
}

bool Token::is_operand_token() const noexcept {
    return (m_type == TokenType::NUMBER || m_type == TokenType::IDENTIFIER);
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
        return out << "<Number " << std::get<double>(tok.m_data) << '>';
      }
      case TokenType::IDENTIFIER: {
        return out << "<Identifier " << std::get<std::string>(tok.m_data) << '>';
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
      case TokenType::ASSIGN: {
        return out << "<Assign>";
      }
      case TokenType::PAREN_L: {
        return out << "<Left Parenthesis>";
      }
      case TokenType::PAREN_R: {
        return out << "<Right Parenthesis>";
      }
      default: {
        return out << "<Invalid token type>";
      }
    }
}

} // namespace clex
