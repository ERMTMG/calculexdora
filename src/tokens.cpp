#include "tokens.hpp"
#include <optional>
#include <stdexcept>
#include <string>

namespace clex {

Token::Token(TokenType type, const std::string& info) noexcept :
    m_type(type), m_info(info) {};

Token::Token() : m_type(TokenType::ERROR_TOKEN), m_info() {};

Token::Token(TokenType type) : m_type(type), m_info() {
    if(type == TokenType::NUMBER || type == TokenType::IDENTIFIER) {
        throw new std::invalid_argument("No token info provided for number/identifier token. Use Token::from_number() or Token::from_ident() instead");
    }
}

Token Token::identifier(const std::string &str) noexcept {
    return Token {
        TokenType::IDENTIFIER,
        std::move(str)
    };
}

Token Token::number(const std::string&  num) noexcept {
    return Token {
        TokenType::NUMBER,
        num
    };
}

TokenType Token::type() const noexcept { return m_type; }
std::optional<std::string> Token::get_ident() const noexcept {
    if(m_type == TokenType::IDENTIFIER && !m_info.empty()) {
        return m_info;
    } else {
        return {};
    }
}

std::optional<double> Token::get_num() const noexcept {
    if(m_type == TokenType::NUMBER && !m_info.empty()) {
        try {
            return std::stod(m_info);
        } catch(const std::invalid_argument& e) {
            std::cerr << "ERROR: Token numérico con valor inválido\n"; // TODO: manejar este error mejor
            return {};
        }
    } else {
        return {};
    }
}

bool Token::operator==(const Token& rhs) const noexcept {
    if(this->m_type != rhs.m_type) {
        return false;
    } else if(
      this->m_type == TokenType::IDENTIFIER || this->m_type == TokenType::NUMBER 
      && this->m_info != rhs.m_info
    ) {
        return false;
    }
    return true;
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
        return out << "<Number " << tok.m_info << '>';
      }
      case TokenType::IDENTIFIER: {
        return out << "<Identifier " << tok.m_info << '>';
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
