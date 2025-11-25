#pragma once
#include "tokens.hpp"
#include <memory>
#include <ostream>
#include <utility>
#include <variant>

namespace clex {

class Expression;

enum class ExpressionType : bool {
    OPERAND,
    BIN_OP,
};

class OperandExpression {
  private:
    Token m_tok;
  public:
    OperandExpression(Token&& tok);
    const Token& get_token() const noexcept;

    friend class Expression;
    friend std::ostream& operator<<(std::ostream& out, const OperandExpression& expr);
};

std::ostream& operator<<(std::ostream& out, const OperandExpression& expr);
    
class BinOpExpression {
  private:
    Token m_operator;
    std::unique_ptr<Expression> m_lhs;
    std::unique_ptr<Expression> m_rhs;
  public:
    BinOpExpression(Token&& oper, std::unique_ptr<Expression>&& lhs, std::unique_ptr<Expression>&& rhs);
    const Token& get_operator() const noexcept;
    std::pair<const Expression&, const Expression&> get_operands() const noexcept;
    friend class Expression;
    friend std::ostream& operator<<(std::ostream& out, const BinOpExpression& expr);
};

std::ostream& operator<<(std::ostream& out, const BinOpExpression& expr);

class Expression {
  private:
    std::variant<BinOpExpression, OperandExpression> m_data;
    ExpressionType m_type;  
    Expression(OperandExpression&& operand) noexcept;
    Expression(BinOpExpression&& bin_op) noexcept;
    Expression() = delete;
  public: 
    static Expression operand(Token&& tok);
    static Expression bin_op(Token&& oper, std::unique_ptr<Expression>&& lhs, std::unique_ptr<Expression>&& rhs);
    
    ExpressionType type() const noexcept;
    const Token& get_token() const noexcept;
    const OperandExpression& as_operand() const;
    const BinOpExpression& as_bin_op() const;

    friend std::ostream& operator<<(std::ostream& out, const Expression& expr);
};

std::ostream& operator<<(std::ostream& out, const Expression& expr);

}