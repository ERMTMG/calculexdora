#pragma once

#include "syntax_tree.hpp"
#include <memory>
#include <ostream>
#include <string>

namespace clex {

class EvalError {
  protected:
    std::string m_message;
    std::unique_ptr<Expression> m_problem;
  public:  
    EvalError(std::string&& message, std::unique_ptr<Expression>&& problem_expr) noexcept;
    const std::string& what() const noexcept;
    const Expression& problem_expr() const noexcept;
    virtual void print_to(std::ostream& out) const noexcept;
};

class UndefinedVariable : public EvalError {
  public:
    UndefinedVariable(std::unique_ptr<Expression>&& var_expr) noexcept;
    virtual void print_to(std::ostream& out) const noexcept override;
};

class DivideByZeroError : public EvalError {
  public:
    DivideByZeroError(std::unique_ptr<Expression>&& division) noexcept;
    virtual void print_to(std::ostream& out) const noexcept override;
};

class ComplexResultError : public EvalError {
  public:
    ComplexResultError(std::unique_ptr<Expression>&& expr) noexcept;
    virtual void print_to(std::ostream& out) const noexcept override;
};

}

