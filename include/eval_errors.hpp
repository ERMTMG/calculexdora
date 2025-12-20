/**
 * @file eval_errors.hpp
 * @brief Definición de las clases de error producidas durante la evaluación de expresiones.
 *
 * Este archivo define una jerarquía de clases que representan los distintos
 * errores que pueden ocurrir durante la evaluación semántica de expresiones
 * y asignaciones. 
 *
 * @author Eduardo Rodríguez, Raúl Gabaldón
 * @date 2025-12
 */
#pragma once

#include "syntax_tree.hpp"
#include <memory>
#include <ostream>
#include <string>

namespace clex {

/**
 * @brief Clase base para errores de evaluación.
 *
 * `EvalError` es usado para errores genéricos que no encajan con la descripción de 
 * ninguna subclase definida.
 */
class EvalError {
  protected:
    std::string m_message; /**< Mensaje descriptivo del error. */
    std::unique_ptr<Expression> m_problem; /**< Expresión que ha provocado el error. */

  public:  
    /**
     * @brief Constructor.
     *
     * @param message Mensaje descriptivo del error.
     * @param problem_expr Expresión que ha provocado el error.
     */
    EvalError(std::string&& message, std::unique_ptr<Expression>&& problem_expr) noexcept;

    /**
     * @brief Obtiene el mensaje de error.
     *
     * @return Referencia constante al mensaje.
     */
    const std::string& what() const noexcept;

    /**
     * @brief Obtiene la expresión problemática asociada al error.
     *
     * @return Referencia constante a la expresión que provocó el error.
     */
    const Expression& problem_expr() const noexcept;

    /**
     * @brief Imprime el error en un flujo de salida como `std::cerr`.
     *
     * @param out Flujo de salida.
     */
    virtual void print_to(std::ostream& out) const noexcept;
};

/**
 * @brief Error producido por el uso de una variable no definida.
 *
 * Este error se lanza cuando, durante la evaluación, se intenta acceder
 * al valor de un identificador que no se encuentra en la tabla de símbolos.
 */
class UndefinedVariable : public EvalError {
  public:
    /**
     * @brief Constructor.
     *
     * @param var_expr Expresión que representa la variable no definida. Siempre será una expresión de tipo `OperandExpression` con un identificador como token.
     */
    UndefinedVariable(std::unique_ptr<Expression>&& var_expr) noexcept;

    /**
     * @brief Sobrecarga de `EvalError::print_to()`
     */
    virtual void print_to(std::ostream& out) const noexcept override;
};

/**
 * @brief Error producido por una división entre cero.
 *
 * Este error se lanza cuando una expresión de división intenta evaluar
 * un denominador cuyo valor es cero. Debido a la imprecisión de números de punto
 * flotante, puede que en algunos casos de evaluación se obtengan falsos negativos, donde una
 * expresión debería ser cero pero acaba siendo un número muy pequeño y se permite la división
 * por ella, por ejemplo `1/sin(pi)`.
 */
class DivideByZeroError : public EvalError {
  public:
    /**
     * @brief Constructor.
     *
     * @param division Expresión de división que provocó el error.
     */
    DivideByZeroError(std::unique_ptr<Expression>&& division) noexcept;

    /**
     * @brief Sobrecarga de `EvalError::print_to()`
     */
    virtual void print_to(std::ostream& out) const noexcept override;
};

/**
 * @brief Error producido por un resultado complejo no soportado.
 *
 * Este error se lanza cuando la evaluación de una expresión produciría
 * un resultado complejo, el cual no está soportado por el sistema. Representa,
 * en general, un error de dominio.
 */
class ComplexResultError : public EvalError {
  public:
    /**
     * @brief Constructor.
     *
     * @param expr Expresión cuya evaluación produciría un resultado complejo.
     */
    ComplexResultError(std::unique_ptr<Expression>&& expr) noexcept;

    /**
     * @brief Sobrecarga de `EvalError::print_to()`
     */
    virtual void print_to(std::ostream& out) const noexcept override;
};

} // namespace clex
