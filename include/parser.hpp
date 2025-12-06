#pragma once
#include "tokens.hpp"
#include "syntax_tree.hpp"
#include "token_list.hpp"
#include <vector>

namespace clex {

class Parser {
  private:  
    TokenList m_tokens;

    Expression parse_expression_recursive(int minimal_binding_power);
  public:
    Parser(TokenList&& tokens) noexcept;
    Parser(std::vector<Token>&& tokens) noexcept;

    Token expect_operand_token();
    Expression parse_expression();
    Assignment parse_assignment(Token&& consumed_var_token);
    Statement parse_next_statement();

};

}