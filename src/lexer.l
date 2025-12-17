%{
#include "tokens.hpp"
#include <string>
#include <iostream>
#include <vector>

using namespace clex;

#define YY_DECL clex::Token yylex()
%}

%option noyywrap

DIGIT    [0-9]
NUMBER   {DIGIT}+(\.{DIGIT}+)?
ID       [a-zA-Z][a-zA-Z0-9]*
WS       [ \t\r]+

%%

{WS}        { /* Ignorar espacios */ }
"+"         { return Token(TokenType::OP_PLUS); }
"-"         { return Token(TokenType::OP_MINUS); }
"*"         { return Token(TokenType::OP_ASTERISK); }
"/"         { return Token(TokenType::OP_SLASH); }
"^"         { return Token(TokenType::OP_CARET); }
"="         { return Token(TokenType::ASSIGN); }
"("         { return Token(TokenType::PAREN_L); }
")"         { return Token(TokenType::PAREN_R); }
"sqrt"      { return Token(TokenType::OP_FUNC_SQRT); }
"log"       { return Token(TokenType::OP_FUNC_LOG); }
"sin"       { return Token(TokenType::OP_FUNC_SIN); }
"cos"       { return Token(TokenType::OP_FUNC_COS); }
"tan"       { return Token(TokenType::OP_FUNC_TAN); }
"asin"      { return Token(TokenType::OP_FUNC_ARCSIN); }
"acos"      { return Token(TokenType::OP_FUNC_ARCCOS); }
"atan"      { return Token(TokenType::OP_FUNC_ARCTAN); }
{NUMBER}    { return Token::number(std::string(yytext)); }
{ID}        { return Token::identifier(std::string(yytext)); }
.           { std::cerr << "Error: " << yytext << std::endl; return Token(); }
<<EOF>>     { return Token(TokenType::END_OF_FILE); }

%%

typedef struct yy_buffer_state *YY_BUFFER_STATE;
extern YY_BUFFER_STATE yy_scan_string(const char *str);
extern void yy_delete_buffer(YY_BUFFER_STATE buffer);

namespace clex {
    std::vector<Token> tokenize(const std::string& input) {
        std::vector<Token> tokens;
        YY_BUFFER_STATE buffer = yy_scan_string(input.c_str());
        
        while(true) {
            Token tok = yylex();
            if (tok.type() == TokenType::END_OF_FILE) break;
            if (tok.type() != TokenType::ERROR_TOKEN) {
                tokens.push_back(std::move(tok));
            }
        }
        yy_delete_buffer(buffer);
        return tokens;
    }
}


