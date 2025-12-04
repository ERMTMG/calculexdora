#include "syntax_tree.hpp"
#include "symbol_table.hpp"
#include "tokens.hpp"
#include <cmath>
#include <cstdlib>
#include <memory>
#include <ostream>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>
#include <variant>

namespace clex {

OperandExpression::OperandExpression(Token&& tok) : m_tok(tok) {
    if(!tok.is_operand_token()) {
        throw std::invalid_argument("Invalid token for operand");
    }
};

const Token& OperandExpression::get_token() const noexcept {
    return m_tok;
}

std::ostream& operator<<(std::ostream& out, const OperandExpression& expr) {
    return out << "<Operand " << expr.m_tok << '>';
}

BinOpExpression::BinOpExpression(Token&& oper, std::unique_ptr<Expression>&& lhs, std::unique_ptr<Expression>&& rhs) :
  m_operator(oper), m_lhs(std::move(lhs)), m_rhs(std::move(rhs)) {
    if(!oper.is_operator_token()) {
        throw std::invalid_argument("Invalid token for binary operation");
    }
    if(m_lhs == nullptr || m_rhs == nullptr) {
        throw std::invalid_argument(
            "Invalid expression pointer(s) for binary operation (" 
            + (m_lhs ? std::string{} : "lhs == nullptr,")
            + (m_rhs ? std::string{} : "rhs == nullptr") + ')'
        );
    }
}

const Token& BinOpExpression::get_operator() const noexcept {
    return m_operator;
}

std::pair<const Expression&, const Expression&> BinOpExpression::get_operands() const noexcept {
    return {*m_lhs, *m_rhs};
}

std::ostream& operator<<(std::ostream& out, const BinOpExpression& expr) {
    return out << "<Bin-op " << *expr.m_lhs << ' ' << expr.m_operator << ' ' << *expr.m_rhs << '>';
}

Expression::Expression(BinOpExpression&& bin_op) noexcept : m_data(std::move(bin_op)), m_type(ExpressionType::BIN_OP) {};

Expression::Expression(OperandExpression&& operand) noexcept : m_data(std::move(operand)), m_type(ExpressionType::OPERAND) {};

Expression Expression::bin_op(Token &&oper, std::unique_ptr<Expression> &&lhs, std::unique_ptr<Expression> &&rhs) {
    return Expression(
        BinOpExpression(
            std::move(oper),
            std::move(lhs),
            std::move(rhs)
        )
    );
}

Expression Expression::operand(Token &&tok) {
    return Expression(
        OperandExpression(
            std::move(tok)
        )
    );
}

ExpressionType Expression::type() const noexcept {
    return m_type;
}

const Token& Expression::get_token() const noexcept {
    auto visit_func = [](const auto& expr) -> const Token& {
        if constexpr(std::is_same_v<decltype(expr), BinOpExpression>) {
            return expr.m_operator;
        } else if constexpr(std::is_same_v<decltype(expr), OperandExpression>) {
            return expr.m_tok;
        } else {
            std::abort(); // no se puede llegar a esto, expr siempre será o BinOpExpression u OperandExpression
        }
    };
    return std::visit(visit_func, m_data);
}

const OperandExpression& Expression::as_operand() const {
    return std::get<OperandExpression>(m_data);
}

const BinOpExpression& Expression::as_bin_op() const {
    return std::get<BinOpExpression>(m_data);
}

std::ostream& operator<<(std::ostream& out, const Expression& expr) {
    auto visit_func = [&out](const auto& expr) -> std::ostream& {
        return out << expr;
    };
    return std::visit(visit_func, expr.m_data);
}

double OperandExpression::evaluate(const SymbolTable& symbols) const {
    if(m_tok.type() == TokenType::NUMBER) {
        return *m_tok.get_num();
    } else { // m_tok tiene que ser un identificador
        auto maybe_val = symbols.get(m_tok);
        if(maybe_val.has_value()) {
            return *maybe_val;
        } else {
            throw 1; // TODO: volver a añadir errores como dios manda.
        }
    }
}

double BinOpExpression::evaluate(const SymbolTable& symbols) const {
    if(m_lhs == nullptr || m_rhs == nullptr) {
        throw 2;
    }
    double lhs_value = m_lhs->evaluate(symbols);
    double rhs_value = m_rhs->evaluate(symbols);
    switch(m_operator.type()) {
      case TokenType::OP_PLUS: {
        return lhs_value + rhs_value;
      }
      case TokenType::OP_MINUS: {
        return lhs_value - rhs_value;
      }
      case TokenType::OP_ASTERISK: {
        return lhs_value * rhs_value;
      }
      case TokenType::OP_SLASH: {
        if(rhs_value == 0.0 || rhs_value == -0.0) {
            throw 3;
        }
        return lhs_value / rhs_value;
      }
      case TokenType::OP_CARET: {
        double result = std::pow(lhs_value, rhs_value);
        if(result != result) { // std::pow puede devolver NaN para valores complejos (como std::pow(-1.0, 0.5))
            throw 4;
        }
        return result;
      }
      default: __builtin_unreachable();
    }
}

double Expression::evaluate(const SymbolTable& symbols) const {
    auto visit_func = [&symbols](const auto& expr) -> double {
        return expr.evaluate(symbols);
    };
    return std::visit(visit_func, m_data);
}


}