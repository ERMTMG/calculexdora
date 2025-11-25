#pragma once
#include <iostream>
#include <optional>
#include <variant>
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
    ASSIGN,      // "="  
    PAREN_L,     // "("
    PAREN_R,     // ")"
};

class Token {
  public:
    using TokenVariant = std::variant<std::monostate, std::string, double, int>;
  private:
    TokenVariant m_data;
    TokenType m_type;
    Token(TokenType type, TokenVariant&& info) noexcept;
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
    std::optional<int> get_binding_power() const noexcept;
    TokenType type() const noexcept;
    bool is_operator_token() const noexcept;
    bool is_operand_token() const noexcept;
    bool operator==(const Token& rhs) const noexcept;
    bool operator!=(const Token& rhs) const noexcept;
    friend std::ostream& operator<<(std::ostream& out, const Token& tok) noexcept;
};

std::ostream& operator<<(std::ostream& out, const Token& tok) noexcept;

}
