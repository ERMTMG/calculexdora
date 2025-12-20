/**
 * @file parser_errors.hpp
 * @brief Definición de las clases de error producidas durante el análisis sintáctico.
 *
 * Este archivo define una jerarquía de clases para representar errores
 * detectados por el analizador sintáctico.
 *
 * @author Eduardo Rodríguez, Raúl Gabaldón
 * @date 2025-12
 */
#pragma once

#include "tokens.hpp"
#include <ostream>
#include <string>
#include <vector>

namespace clex {

/**
 * @brief Clase base para errores de análisis sintáctico.
 *
 * `ParserError` es usado para errores genéricos que no encajan con la descripción de 
 * ninguna subclase definida.
 */
class ParserError {
  protected:  
    std::string m_message;     /**< Mensaje de error. */
    Token m_problem_token;     /**< Token relacionado con el error. */

  public:
    /**
     * @brief Constructor.
     *
     * @param message Mensaje descriptivo del error.
     * @param problem_token Token que ha causado (o está relacionado) con el error.
     */
    ParserError(std::string&& message, Token problem_token) noexcept;

    /**
     * @brief Obtiene el mensaje de error.
     *
     * @return Referencia constante al mensaje.
     */
    const std::string& what() const noexcept;

    /**
     * @brief Imprime el error en un flujo de salida como `std::cerr`.
     *
     * @param out Flujo de salida.
     */
    virtual void print_to(std::ostream& out) const noexcept;

    /**
     * @brief Obtiene el token problemático asociado al error.
     *
     * @return Una copia del token de error.
     */
    Token problem_token() const noexcept;
};

/**
 * @brief Error que indica que se esperaba uno o más tipos de token concretos.
 *
 * Este error se produce cuando el analizador encuentra un token distinto
 * de los tipos esperados en un determinado punto del análisis.
 */
class ExpectedToken : public ParserError {
  public:
    /**
     * @brief Constructor.
     *
     * @param expected_tokens Vector con los tipos de token esperados.
     * @param actual_token Token realmente encontrado.
     */
    ExpectedToken(std::vector<TokenType>&& expected_tokens, Token actual_token) noexcept;

    /**
     * @brief Sobrecarga de `ParserError::print_to()`
     */
    virtual void print_to(std::ostream& out) const noexcept override;
};

/**
 * @brief Error que indica que se esperaba un operador.
 *
 * Especialización de `ExpectedToken` para el caso en el que el analizador
 * esperaba encontrar un operador específicamente.
 */
class ExpectedOperator : public ExpectedToken {
  public:
    /**
     * @brief Constructor.
     *
     * @param actual_token Token realmente encontrado en lugar del operador.
     */
    ExpectedOperator(Token actual_token) noexcept;

    /**
     * @brief Sobrecarga de `ExpectedToken::print_to()`
     */
    virtual void print_to(std::ostream& out) const noexcept override;  
};

/**
 * @brief Error que indica paréntesis dispares.
 *
 * Este error se produce cuando el analizador detecta una discrepancia
 * entre paréntesis de apertura y cierre (por ejemplo, cuando no se cierra un
 * paréntesis anteriormente abierto).
 */
class MismatchedParentheses : public ParserError {
  public:
    /**
     * @brief Constructor.
     *
     * @param paren_token Token del paréntesis implicado en el error.
     * @param nearby_token Token cercano que ayuda a contextualizar el error.
     */
    MismatchedParentheses(Token paren_token, Token nearby_token) noexcept;

    /**
     * @brief Sobrecarga de `ParserError::print_to()`
     */
    virtual void print_to(std::ostream& out) const noexcept override;
};

} // namespace clex
