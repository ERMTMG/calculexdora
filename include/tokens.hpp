#pragma once
#include <iostream>
#include <optional>
#include <string>

namespace clex {

enum class TokenType {
    ERROR_TOKEN = -1,
    END_OF_FILE,
    NUMBER,      // 0123456789
    IDENTIFIER,  // nombre de variable
    OP_PLUS,     // "+"
    OP_MINUS,    // "-"
    OP_ASTERISK, // "*"
    OP_SLASH,    // "/"
    OP_CARET,    // "^"
    ASSIGN       // "="  
};

class Token {
  private:
  std::string m_info;
    TokenType m_type;
    Token(TokenType type, const std::string& info) noexcept;
  public:
    Token();
    Token(const Token&) = default;
    Token(Token&&) = default;
    Token& operator=(const Token&) = default;
    Token& operator=(Token&&) = default;

    Token(TokenType type);
    static Token number(const std::string& num) noexcept;
    static Token identifier(const std::string& str) noexcept;
    std::optional<double> get_num() const noexcept;
    std::optional<std::string> get_ident() const noexcept;
    TokenType type() const noexcept;
    bool operator==(const Token& rhs) const noexcept;
    bool operator!=(const Token& rhs) const noexcept;
    friend std::ostream& operator<<(std::ostream& out, const Token& tok) noexcept;
};

std::ostream& operator<<(std::ostream& out, const Token& tok) noexcept;

}
