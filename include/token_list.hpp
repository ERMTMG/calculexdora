#pragma once

#include "tokens.hpp"
#include <vector>

namespace clex {

class TokenList {
  private:  
    std::vector<Token> m_tokens;
  public:
    TokenList(std::vector<Token>&& tokens);
    TokenList(const TokenList&) = default;
    TokenList(TokenList&&) = default;
    TokenList& operator=(const TokenList&) = default;
    TokenList& operator=(TokenList&&) = default;

    bool at_end() const noexcept;
    const Token& peek() const noexcept;
    Token next() noexcept;
};

}