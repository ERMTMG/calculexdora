#include "syntax_tree.hpp"
#include "tokens.hpp"
#include <memory>
#include <ostream>
#include <stdexcept>
#include <string>
#include <utility>

namespace clex {

OperandExpression::OperandExpression(Token&& tok) : m_tok(tok) {
    if(!tok.is_operand_token()) {
        throw new std::invalid_argument("Invalid token for operand");
    }
};

std::ostream& operator<<(std::ostream& out, const OperandExpression& expr) {
    return out << "<Operand " << expr.m_tok << '>';
}

BinOpExpression::BinOpExpression(Token&& oper, std::unique_ptr<Expression>&& lhs, std::unique_ptr<Expression>&& rhs) :
  m_operator(oper), m_lhs(std::move(lhs)), m_rhs(std::move(rhs)) {
    if(!oper.is_operator_token()) {
        throw new std::invalid_argument("Invalid token for binary operation");
    }
    if(m_lhs == nullptr || m_rhs == nullptr) {
        throw new std::invalid_argument(
            "Invalid expression pointer(s) for binary operation (" 
            + (m_lhs ? std::string{} : "lhs == nullptr,")
            + (m_rhs ? std::string{} : "rhs == nullptr") + ')'
        );
    }
}

std::ostream& operator<<(std::ostream& out, const BinOpExpression& expr) {
    return out << "<Bin-op " << *expr.m_lhs << ' ' << expr.m_operator << ' ' << *expr.m_rhs << '>';
}

}