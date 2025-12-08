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

Expression Parser::parse_expression_recursive(int minimal_binding_power) {
    Token first_tok = m_tokens.next();
    Expression lhs = [&]() -> Expression { // Tengo que usar una lambda aquí porque no existe el constructor por defecto de Expression
        switch(first_tok.type()) {
          case TokenType::NUMBER:
          case TokenType::IDENTIFIER: {
            return Expression::operand(std::move(first_tok));
            break;
          }
          case TokenType::PAREN_L: {
            Expression tmp = this->parse_expression_recursive(0); // reseteamos el binding power por los paréntesis
            Token after_paren = m_tokens.next();
            if(after_paren.type() != TokenType::PAREN_R) {
                throw MismatchedParentheses(first_tok, after_paren);
            }
            return tmp;
          }
          default: throw ExpectedToken({TokenType::IDENTIFIER, TokenType::NUMBER, TokenType::PAREN_L}, first_tok);
        }
    }();
    
    int current_binding_power;

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
        if(
            (current_binding_power < minimal_binding_power && operator_tok.type() == TokenType::OP_CARET) // El operador ^ es asociativo a la derecha así que requiere un binding power estrictamente menor
            || (current_binding_power <= minimal_binding_power && operator_tok.type() != TokenType::OP_CARET) // El resto de operadores son asociativos a la izquierda, retornan conque sea menor o igual
        ) { 
            // En estos casos, no seguimos haciendo binding hacia la derecha, terminamos el bucle y retornamos
            return lhs;
        }

        m_tokens.next(); // nos saltamos el token que ya sabemos que es un operador

        Expression rhs = parse_expression_recursive(current_binding_power); // hacemos recursión para comprobar si a la derecha hay una expresion compleja

        lhs = Expression::bin_op(
                std::move(operator_tok), 
                std::make_unique<Expression>(std::move(lhs)), 
                std::make_unique<Expression>(std::move(rhs))
        );
    }
}

Expression Parser::parse_expression() {
    return parse_expression_recursive(-1);
}

Assignment Parser::parse_assignment(Token&& consumed_var_token) {
    if(m_tokens.peek().type() != TokenType::ASSIGN) {
        throw ExpectedToken({TokenType::ASSIGN}, m_tokens.peek().type());
    }
    m_tokens.next();
    return Assignment {
        std::move(consumed_var_token),
        std::make_unique<Expression>(
            parse_expression()
        )
    };
}

Statement Parser::parse_next_statement() {
    if (m_tokens.peek().type() != TokenType::IDENTIFIER) {
        return Statement::expression(parse_expression());
    } else {
        Token first_tok = m_tokens.next();
        if(m_tokens.peek().type() != TokenType::ASSIGN) {
            m_tokens.give_back(std::move(first_tok));
            return Statement::expression(parse_expression());
        } else {
            return Statement::assignment(parse_assignment(std::move(first_tok)));
        }
    }
}

}