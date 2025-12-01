#pragma once
#include "tokens.hpp"
#include <ostream>
#include <string>
#include <vector>

namespace clex {

class ParserError {
  protected:  
    std::string m_message;
  public:
    ParserError(std::string&& message) noexcept;
    const std::string& what() const noexcept;
    virtual void print_to(std::ostream& out) const noexcept;
};

class ExpectedToken : public ParserError {
  public:
    ExpectedToken(std::vector<Token>&& expected_tokens, Token actual_token) noexcept;
    virtual void print_to(std::ostream& out) const noexcept override;
};

class ExpectedOperator : public ExpectedToken {
  public:
    ExpectedOperator(Token actual_token) noexcept;
    virtual void print_to(std::ostream& out) const noexcept override;  
};

class MismatchedParentheses : public ParserError {
  public:
    MismatchedParentheses(Token paren_token, Token nearby_token) noexcept;
    virtual void print_to(std::ostream& out) const noexcept override;
};



}