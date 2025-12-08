#include "eval_errors.hpp"
#include "syntax_tree.hpp"
#include <ostream>
#include <sstream>
#include <string>
#include <utility>

namespace clex {

EvalError::EvalError(std::string&& message, std::unique_ptr<Expression>&& problem_expr) noexcept :
  m_message(std::move(message)), m_problem(std::move(problem_expr)) {};

const std::string& EvalError::what() const noexcept {
    return m_message;
}

const Expression& EvalError::problem_expr() const noexcept {
    return *m_problem;
}

void EvalError::print_to(std::ostream& out) const noexcept {
    out << "<ERROR DE EVALUACIÓN> " << this->what() << '\n';
}

UndefinedVariable::UndefinedVariable(std::unique_ptr<Expression>&& var_expr) noexcept : EvalError("", std::move(var_expr)) {
    std::stringstream msg; 
    msg << "No se ha definido la variable `"
        << *m_problem->as_operand().get_token().get_ident() << '`'; // Se garantiza que var_expr sea una expresión que contenga un solo operando identificador
    m_message = msg.str();
}

void UndefinedVariable::print_to(std::ostream& out) const noexcept {
    out << "<VARIABLE NO DEFINIDA> " << this->what() << '\n';
}

DivideByZeroError::DivideByZeroError(std::unique_ptr<Expression>&& division) noexcept : EvalError("", std::move(division)) {
    std::stringstream msg;
    msg << "División por cero en la expresión "
        << *m_problem;
    m_message = msg.str();
}

void DivideByZeroError::print_to(std::ostream& out) const noexcept {
    out << "<DIVISIÓN POR CERO> " << this->what() << '\n';
}

ComplexResultError::ComplexResultError(std::unique_ptr<Expression>&& expr) noexcept : EvalError("", std::move(expr)) {
    std::stringstream msg;
    msg << "El resultado de la expresión "
        << *m_problem << " no es un número real";
    m_message = msg.str();
}

void ComplexResultError::print_to(std::ostream& out) const noexcept {
    out << "<RESULTADO COMPLEJO> " << this->what() << '\n';
}

}