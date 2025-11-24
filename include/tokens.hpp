#pragma once
#include <iostream>
#include <optional>
#include <string>
#include <variant>

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
    TokenType m_type;
    std::variant<std::monostate, double, std::string> m_info;
    Token(TokenType type, std::variant<std::monostate, double, std::string>&& info) noexcept;
  public:
    Token();
    Token(const Token&) = default;
    Token(Token&&) = default;
    Token& operator=(const Token&) = default;
    Token& operator=(Token&&) = default;

    Token(TokenType type);
    static Token from_number(double num) noexcept;
    static Token from_ident(const std::string& str) noexcept;
    std::optional<double> get_num() const noexcept;
    std::optional<std::string> get_ident() const noexcept;
    TokenType type() const noexcept;
    std::string to_string() const noexcept;
    friend std::ostream& operator<<(std::ostream& out, const Token& tok) noexcept;
};

std::ostream& operator<<(std::ostream& out, const Token& tok) noexcept;

}
