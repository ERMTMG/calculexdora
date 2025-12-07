#pragma once
#include "symbol_table.hpp"
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

    Expression clone() const noexcept;
    double evaluate(const SymbolTable& symbol_table) const;

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

    Expression clone() const noexcept;
    double evaluate(const SymbolTable& symbols) const;

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

    Expression clone() const noexcept;
    double evaluate(const SymbolTable& symbols) const;

    friend std::ostream& operator<<(std::ostream& out, const Expression& expr);
};

std::ostream& operator<<(std::ostream& out, const Expression& expr);

class Assignment {
  private:
    Token m_variable_lhs;
    std::unique_ptr<Expression> m_rhs;
  public:
    Assignment(Token&& variable_lhs, std::unique_ptr<Expression>&& rhs);

    const Token& get_var() const noexcept;
    const std::unique_ptr<Expression>& get_value() const noexcept;
    
    void execute(SymbolTable& symbols) const;

    friend std::ostream& operator<<(std::ostream& out, const Assignment& assign);
};

std::ostream& operator<<(std::ostream& out, const Assignment& assign);

class Statement {
  private:
    std::variant<Expression, Assignment> m_data;
    Statement(Expression&& expr) noexcept;
    Statement(Assignment&& assign) noexcept;
    Statement() = delete;
  public:  
    static Statement expression(Expression&& expr) noexcept;
    static Statement assignment(Assignment&& assign) noexcept;
    bool is_expression() const noexcept;
    inline bool is_assignment() const noexcept { return !is_expression(); }
    Expression&& move_as_expression();
    Assignment&& move_as_assignment();
    const Expression& ref_as_expression() const;
    const Assignment& ref_as_assignment() const;
};
}