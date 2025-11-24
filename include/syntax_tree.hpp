#pragma once
#include "tokens.hpp"
#include <memory>
#include <ostream>

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
    //...
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
    //...
    friend std::ostream& operator<<(std::ostream& out, const BinOpExpression& expr);
};

std::ostream& operator<<(std::ostream& out, const BinOpExpression& expr);

class Expression {
  private:
    union {
        OperandExpression m_operand;
        BinOpExpression m_bin_op;
    };
    ExpressionType m_type;  
    Expression(OperandExpression&& operand) noexcept;
    Expression(BinOpExpression&& bin_op) noexcept;
    Expression() = delete;
  public: 
    ~Expression();
    
    static Expression operand(Token&& tok);
    static Expression bin_op(Token&& oper, std::unique_ptr<Expression>&& lhs, std::unique_ptr<Expression>&& rhs);
    
    friend std::ostream& operator<<(std::ostream& out, const Expression& expr);
};

std::ostream& operator<<(std::ostream& out, const Expression& expr);

}