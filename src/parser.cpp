#include "parser.hpp"
#include "syntax_tree.hpp"
#include "token_list.hpp"
#include "tokens.hpp"
#include <memory>
#include <utility>
#include <vector>


namespace clex {
    
Parser::Parser(TokenList&& tokens) noexcept : m_tokens(tokens) {};

Parser::Parser(std::vector<Token>&& tokens) noexcept : m_tokens(std::move(tokens)) {};

std::unique_ptr<Expression> Parser::parse_expression() {
    Token first_tok = m_tokens.next();
    std::unique_ptr<Expression> lhs;
    switch(first_tok.type()) {
      case TokenType::NUMBER:
      case TokenType::IDENTIFIER: {
        lhs = std::make_unique<Expression>(
            Expression::operand(std::move(first_tok))
        );
        break;
      }
      default:
        throw "bad starting token"; // TODO: añadir errores como dios manda
    }

    Token operator_tok = m_tokens.peek();
    switch(operator_tok.type()) {
      case TokenType::END_OF_FILE: {
        return lhs; // Reached the end, no operation needed
      } 
      default: {
        if(!operator_tok.is_operator_token()) {
            throw "expected operator";
        } else {
            m_tokens.next(); // nos saltamos el token que ya sabemos que es un operador
        }
      }
    }

    Token rhs_tok = m_tokens.next();
    std::unique_ptr<Expression> rhs;
    switch(rhs_tok.type()) {
      case TokenType::NUMBER:
      case TokenType::IDENTIFIER: {
        rhs = std::make_unique<Expression>(
            Expression::operand(std::move(rhs_tok))
        );
        break;
      }
      default:
        throw "bad rhs token";
    }

    return std::make_unique<Expression>(
        Expression::bin_op(
            std::move(operator_tok), 
            std::move(lhs), 
            std::move(rhs)
        )
    );
}

}