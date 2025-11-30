#include "tokens.hpp"
#include <optional>
#include <stdexcept>
#include <string>

namespace clex {

Token::Token(TokenType type, Token::TokenVariant&& data) noexcept :
    m_data(data), m_type(type) {};

Token::Token() : m_data(), m_type(TokenType::ERROR_TOKEN) {};

Token::Token(TokenType type) : m_data(), m_type(type) {
    static const int PLUS_MINUS_BINDING_POWER = 1;
    static const int TIMES_DIVIDE_BINDING_POWER = 2;
    static const int EXPONENT_BINDING_POWER = 3;
    if(type == TokenType::NUMBER || type == TokenType::IDENTIFIER) {
        throw new std::invalid_argument("No token info provided for number/identifier token. Use Token::from_number() or Token::from_ident() instead");
    } else {
        switch (type) {
          case TokenType::OP_PLUS:
          case TokenType::OP_MINUS: {
            m_data = PLUS_MINUS_BINDING_POWER;
            break;
          }
          case TokenType::OP_ASTERISK:
          case TokenType::OP_SLASH: {
            m_data = TIMES_DIVIDE_BINDING_POWER;
            break;
          }
          case TokenType::OP_CARET: {
            m_data = EXPONENT_BINDING_POWER;
            break;
          }
          default: break;
        }
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

std::optional<int> Token::get_binding_power() const noexcept {
    if(is_operator_token()) {
        return std::get<int>(m_data);
    } else {
        return {};
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

bool Token::is_operator_token() const noexcept {
    return (
        m_type == TokenType::OP_PLUS 
     || m_type == TokenType::OP_MINUS
     || m_type == TokenType::OP_ASTERISK
     || m_type == TokenType::OP_SLASH
     || m_type == TokenType::OP_CARET
    );
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
