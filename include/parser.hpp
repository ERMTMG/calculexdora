#pragma once
#include "tokens.hpp"
#include "syntax_tree.hpp"
#include "token_list.hpp"
#include <memory>
#include <vector>

namespace clex {

class Parser {
  private:  
    TokenList m_tokens;
  public:
    Parser(TokenList&& tokens) noexcept;
    Parser(std::vector<Token>&& tokens) noexcept;

    std::unique_ptr<Expression> parse_expression();
};

}