#include "parser.hpp"
#include "parser_errors.hpp"
#include "syntax_tree.hpp"
#include "token_list.hpp"
#include "tokens.hpp"
#include <memory>
#include <utility>
#include <vector>


namespace clex {
    
Parser::Parser(TokenList&& tokens) noexcept : m_tokens(tokens) {};

Parser::Parser(std::vector<Token>&& tokens) noexcept : m_tokens(std::move(tokens)) {};

Token Parser::expect_operand_token() {
    Token tok = m_tokens.next();
    switch(tok.type()) {
      case TokenType::NUMBER:
      case TokenType::IDENTIFIER: 
        return tok;
      default:
        throw ExpectedToken({TokenType::IDENTIFIER, TokenType::NUMBER}, tok);
    }
}

std::unique_ptr<Expression> Parser::parse_expression_recursive(int minimal_binding_power) {
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
      case TokenType::PAREN_L: {
        lhs = this->parse_expression_recursive(0); // reseteamos el binding power por los paréntesis
        Token after_paren = m_tokens.next();
        if(after_paren.type() != TokenType::PAREN_R) {
            throw ExpectedToken({TokenType::PAREN_R}, after_paren);
        }
        break;
      }
      default: throw ExpectedToken({TokenType::IDENTIFIER, TokenType::NUMBER, TokenType::PAREN_L}, first_tok);
    }

    int current_binding_power = minimal_binding_power;

    while(true) { // bucle infinito para seguir mirando por la derecha, que term
        Token operator_tok = m_tokens.peek();
        switch(operator_tok.type()) {
          case TokenType::END_OF_FILE:
          case TokenType::NEWLINE: 
          case TokenType::PAREN_R: {
            return lhs; // hemos llegado al final de la expresión, no hace falta operador
          } 
          default: {
            if(!operator_tok.is_operator_token()) {
                throw ExpectedOperator(operator_tok);
            }
          }
        }

        current_binding_power = *operator_tok.get_binding_power();
        if(current_binding_power <= minimal_binding_power) { // no seguimos haciendo binding hacia la derecha, terminamos el bucle y retornamos
            return lhs;
        }

        m_tokens.next(); // nos saltamos el token que ya sabemos que es un operador

        std::unique_ptr<Expression> rhs = parse_expression_recursive(current_binding_power); // hacemos recursión para comprobar si a la derecha hay una expresion compleja

        lhs = std::make_unique<Expression>( // y recogemos todo lo que hemos hecho en lhs para seguir con la próxima iteración del bucle
            Expression::bin_op(
                std::move(operator_tok), 
                std::move(lhs), 
                std::move(rhs)
            )
        );
    }
}

std::unique_ptr<Expression> Parser::parse_expression() {
    return parse_expression_recursive(-1);
}

}