#include "token_list.hpp"
#include "tokens.hpp"
#include <algorithm>
#include <vector>

namespace clex {

TokenList::TokenList(std::vector<Token>&& tokens) : m_tokens(std::move(tokens)) {
    if(m_tokens.back().type() != TokenType::END_OF_FILE) {
        m_tokens.emplace_back(TokenType::END_OF_FILE);
    }
    std::reverse(m_tokens.begin(), m_tokens.end());
};

bool TokenList::at_end() const noexcept {
    return m_tokens.back().type() == TokenType::END_OF_FILE;
}

const Token& TokenList::peek() const noexcept {
    return m_tokens.back();
}

Token TokenList::next() noexcept {
    if(!at_end()) {
        Token tok = std::move(m_tokens.back());
        m_tokens.pop_back();
        return tok;
    } else {
        return m_tokens.back();
    }
}

}