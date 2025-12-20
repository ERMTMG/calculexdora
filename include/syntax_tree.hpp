/**
 * @file syntax_tree.hpp
 * @brief Definición de las clases necesarias para el árbol abstracto de sintaxis (AST) que resulta del análisis sintáctico.
 *
 * @author Eduardo Rodríguez, Raúl Gabaldón
 * @date 2025-12
 */
#pragma once
#include "symbol_table.hpp"
#include "tokens.hpp"
#include <cstdint>
#include <memory>
#include <ostream>
#include <utility>
#include <variant>

namespace clex {

class Expression;

/**
 * @brief Tipo enumerado que describe los tipos posibles de una expresión.
 *
 * No confundir con la clase `Expression`, que instancia los nodos de expresión en sí.
 */
enum class ExpressionType : uint8_t {
    OPERAND,   /**< Representa una expresión que consiste únicamente en un operando. */
    BIN_OP,    /**< Representa una expresión de operador binario. */
    UNARY_OP,  /**< Representa una expresión de operador unario o función. */
};

/**
 * @brief Expresión específica que encapsula un único operando.
 *
 * Un `OperandExpression` representa el caso base de una expresión, y
 * normalmente corresponde a un literal o a un identificador contenido
 * en un `Token`.
 */
class OperandExpression {
  private:
    Token m_tok; /**< Token que representa el operando. */
  public:
    /**
     * @brief Construye una expresión de operando a partir de un token.
     *
     * @param tok Token que contiene el valor o identificador del operando.
     * @exception Lanza `std::invalid_argument` si `tok` no es un token con tipo de operando.
     */
    OperandExpression(Token&& tok);

    /**
     * @brief Obtiene el token asociado a este operando.
     *
     * @return Referencia constante al token interno.
     */
    const Token& get_token() const noexcept;

    /**
     * @brief Crea una copia de esta expresión.
     *
     * Debido a que la clase `OperandExpression` representa un operando (una hoja del árbol de sintaxis), 
     * esta operación no tiene mucho coste justo para esta clase. Devuelve la expresión clonada como instancia 
     * de `Expression`, no de `OperandExpression`.
     * 
     * @return Una nueva instancia de `Expression` equivalente a ésta.
     */
    Expression clone() const noexcept;

    /**
     * @brief Evalúa la expresión utilizando una tabla de símbolos.
     *
     * Debido a tratarse de un solo operando, el único error de evaluación que puede lanzar la evaluación de este tipo
     * de expresiones es uno resultante de intentar acceder a una variable no definida.
     *
     * @param symbol_table Tabla de símbolos usada para la evaluación.
     * @return Resultado numérico de la evaluación.
     * @exception Lanza un `EvalError` si ha habido problemas en la evaluación de la expresión. Por ello, se recomienda encerrar llamadas a este método en un bloque `try ... catch`.
     */
    double evaluate(const SymbolTable& symbol_table) const;

    friend class Expression;
    friend std::ostream& operator<<(std::ostream& out, const OperandExpression& expr);
};

/**
 * @brief Operador de inserción en flujo de salida (para uso con 
 * `std::cout` y similares)
 *
 * Convierte el operando a una cadena con información sobre el token y la imprime.
 * 
 * @param out El flujo de salida.
 * @param expr La expresión a imprimir.
 * @return Referencia a `out` después de la operación.
 */
std::ostream& operator<<(std::ostream& out, const OperandExpression& expr);
    
/**
 * @brief Expresión que representa un operador binario.
 *
 * Un `BinOpExpression` contiene un token de operador y dos expresiones hijas, 
 * correspondientes a los operandos.
 */
class BinOpExpression {
  private:
    Token m_operator; /**< Token que representa el operador binario. */
    std::unique_ptr<Expression> m_lhs; /**< Puntero al operando izquierdo. */
    std::unique_ptr<Expression> m_rhs; /**< Puntero al operando derecho. */
  public:
    /**
     * @brief Construye una expresión binaria.
     *
     * @param oper Token que representa el operador.
     * @param lhs Puntero a la expresión del lado izquierdo.
     * @param rhs Puntero a la expresión del lado derecho.
     * @exception Lanza `std::invalid_argument` si o bien `oper` no es un token de tipo operador binario, o bien `lhs` ó `rhs` son punteros nulos.
     */
    BinOpExpression(Token&& oper,
                    std::unique_ptr<Expression>&& lhs,
                    std::unique_ptr<Expression>&& rhs);

    /**
     * @brief Obtiene el token del operador binario.
     *
     * @return Referencia constante al token del operador.
     */
    const Token& get_operator() const noexcept;

    /**
     * @brief Obtiene las expresiones operando como un `std::pair`.
     *
     * @return Un `std::pair` de referencias constantes al operando izquierdo y derecho.
     */
    std::pair<const Expression&, const Expression&> get_operands() const noexcept;

    /**
     * @brief Crea una copia profunda de esta expresión binaria.
     *
     * Debido a que la clase `BinOpExpression` representa una operación (un nodo intermedio del árbol de sintaxis), 
     * este método tiene un coste dependiente de la complejidad de la operación. Si el subárbol del árbol de sintaxis
     * cuya raíz es esta expresión tiene profundidad $h$, este método tiene complejidad en el peor caso de aproximadamente $O(2^h)$. 
     * Devuelve la expresión clonada como instancia de `Expression`, no de `BinOpExpression`.
     * 
     * @return Una nueva instancia de `Expression` equivalente a ésta.
     */
    Expression clone() const noexcept;

    /**
     * @brief Evalúa la expresión utilizando una tabla de símbolos.
     *
     * Debido a tratarse de una operación que puede tener cualquier otra subexpresión, no se puede garantizar qué tipo de errores de evaluación
     * puede lanzar este método, solo se asegura que serán de tipo `EvalError` o alguna de sus subclases.
     *
     * @param symbol_table Tabla de símbolos usada para la evaluación.
     * @return Resultado numérico de la evaluación.
     * @exception Lanza un `EvalError` si ha habido problemas en la evaluación de la expresión. Por ello, se recomienda encerrar llamadas a este método en un bloque `try ... catch`.
     */
    double evaluate(const SymbolTable& symbols) const;

    friend class Expression;
    friend std::ostream& operator<<(std::ostream& out, const BinOpExpression& expr);
};

/**
 * @brief Operador de inserción en flujo de salida (para uso con 
 * `std::cout` y similares)
 *
 * Convierte la operación binaria a una cadena con información sobre el operador y los operandos y la imprime.
 * 
 * @param out El flujo de salida.
 * @param expr La expresión a imprimir.
 * @return Referencia a `out` después de la operación.
 */
std::ostream& operator<<(std::ostream& out, const BinOpExpression& expr);

/**
 * @brief Expresión que representa un operador unario o función.
 *
 * Las funciones son internamente analizadas como operadores unarios. Por ejemplo, en `sqrt 5`, `sqrt` 
 * (el token de tipo `TokenType::OP_FUNC_SQRT`) es un operador unario que actúa sobre el operando `5`.
 * 
 * Un `UnaryOpExpression` contiene un operador y una única expresión
 * operando sobre la cual se aplica dicho operador.
 */
class UnaryOpExpression {
  private:
    Token m_operator; /**< Token que representa el operador unario o función. */
    std::unique_ptr<Expression> m_operand; /**< Expresión correspondiente al operando. */
  public:
    /**
     * @brief Construye una expresión unaria o función.
     *
     * @param oper Token que representa el operador.
     * @param operand Puntero a la expresión sobre la que actúa el operador.
     * @exception Lanza `std::invalid_argument` si o bien `oper` no es un token de operador unario o función, o bien `operand` es un puntero nulo.
     */
    UnaryOpExpression(Token&& oper, std::unique_ptr<Expression>&& operand);

    /**
     * @brief Obtiene el token del operador unario.
     *
     * @return Referencia constante al token del operador.
     */
    const Token& get_operator() const noexcept;

    /**
     * @brief Obtiene la expresión operando.
     *
     * @return Referencia constante a la expresión operando.
     */
    const Expression& get_operand() const noexcept;

    /**
     * @brief Crea una copia profunda de esta expresión binaria.
     *
     * Debido a que la clase `UnaryOpExpression` representa una operación (un nodo intermedio del árbol de sintaxis), 
     * este método tiene un coste dependiente de la complejidad de la operación. Si el subárbol del árbol de sintaxis
     * cuya raíz es esta expresión tiene profundidad $h$, este método tiene complejidad en el peor caso de aproximadamente $O(2^h)$. 
     * Devuelve la expresión clonada como instancia de `Expression`, no de `UnaryOpExpression`.
     * 
     * @return Una nueva instancia de `Expression` equivalente a ésta.
     */
    Expression clone() const noexcept;

    /**
     * @brief Evalúa la expresión utilizando una tabla de símbolos.
     *
     * Debido a tratarse de una operación que puede tener cualquier otra subexpresión, no se puede garantizar qué tipo de errores de evaluación
     * puede lanzar este método, solo se asegura que serán de tipo `EvalError` o alguna de sus subclases.
     *
     * @param symbol_table Tabla de símbolos usada para la evaluación.
     * @return Resultado numérico de la evaluación.
     * @exception Lanza un `EvalError` si ha habido problemas en la evaluación de la expresión. Por ello, se recomienda encerrar llamadas a este método en un bloque `try ... catch`.
     */
    double evaluate(const SymbolTable& symbols) const;

    friend class Expression;
    friend std::ostream& operator<<(std::ostream& out, const UnaryOpExpression& expr);
};

/**
 * @brief Operador de inserción en flujo de salida (para uso con 
 * `std::cout` y similares)
 *
 * Convierte la operación unaria o función a una cadena con información sobre el operador y el operando y la imprime.
 * 
 * @param out El flujo de salida.
 * @param expr La expresión a imprimir.
 * @return Referencia a `out` después de la operación.
 */
std::ostream& operator<<(std::ostream& out, const UnaryOpExpression& expr);

/**
 * @brief Representa una expresión genérica.
 *
 * Esta clase actúa como una variante que puede almacenar
 * cualquiera de los tipos concretos de expresión soportados 
 * (`OperandExpression`, `UnaryOpExpression` ó `BinaryOpExpression`).
 */
class Expression {
  private:
    std::variant<BinOpExpression, OperandExpression, UnaryOpExpression> m_data;
    ExpressionType m_type;  

    Expression(OperandExpression&& operand) noexcept;
    Expression(BinOpExpression&& bin_op) noexcept;
    Expression(UnaryOpExpression&& unary_op) noexcept;
    Expression() = delete;
  public: 
    /**
     * @brief Construye y devuelve una expresión de tipo operando, usando parámetros del constructor.
     *
     * @param tok Token que representa el operando.
     * @return Nueva expresión de tipo `ExpressionType::OPERAND`.
     * @pre Los argumentos pasados deben ser válidos para construir un `OperandExpression`.
     */
    static Expression operand(Token&& tok);

    /**
     * @brief Construye y devuelve una expresión de tipo operador binario.
     *
     * @param oper Token del operador.
     * @param lhs Puntero al operando izquierdo.
     * @param rhs Puntero al operando derecho.
     * @return Nueva expresión de tipo `ExpressionType::BIN_OP`.
     * @pre Los argumentos pasados deben ser válidos para construir un `BinOpExpression`.
     */
    static Expression bin_op(Token&& oper,
                             std::unique_ptr<Expression>&& lhs,
                             std::unique_ptr<Expression>&& rhs);

    /**
     * @brief Crea una expresión de operador unario o función.
     *
     * @param oper Token del operador.
     * @param operand Expresión operando.
     * @return Nueva expresión de tipo UNARY_OP.
     * @pre Los argumentos pasados deben ser válidos para construir un `UnaryOpExpression`.
     */
    static Expression unary_op(Token&& oper,
                               std::unique_ptr<Expression>&& operand);
    
    /**
     * @brief Obtiene el tipo de la expresión.
     *
     * @return Tipo de la expresión usando el enumerado `ExpressionType`.
     */
    ExpressionType type() const noexcept;

    /**
     * @brief Obtiene el token asociado a la expresión.
     *
     * Cualquier subtipo de expresión tiene un token específico asociado, pero
     * el token devuelto depende del tipo concreto de la expresión:
     * - Para expresiones de tipo `ExpressionType::OPERAND`, el token devuelto corresponde al operando (número o identificador)
     * - Para expresiones de tipo `ExpressionType::BIN_OP`, el token devuelto es el operador binario.
     * - Para expresiones de tipo `ExpressionType::UNARY_OP`, el token devuelto es el operador unario o función.
     *
     * @return Referencia constante al token correspondiente.
     */
    const Token& get_token() const noexcept;

    /**
     * @brief Accede a la expresión como operando.
     *
     * @return Referencia constante a la expresión como instancia de `OperandExpression`.
     * @pre El tipo de la expresión debe ser `ExpressionType::OPERAND`
     */
    const OperandExpression& as_operand() const;

    /**
     * @brief Accede a la expresión como operador binario.
     *
     * @return Referencia constante a la expresión como instancia de `BinOpExpression`.
     * @pre El tipo de la expresión debe ser `ExpressionType::BIN_OP`
     */
    const BinOpExpression& as_bin_op() const;

    /**
     * @brief Accede a la expresión como operador unario o función.
     *
     * @return Referencia constante a la expresión como instancia de `UnaryOpExpression`.
     * @pre El tipo de la expresión debe ser `ExpressionType::UNARY_OP`
     */
    const UnaryOpExpression& as_unary_op() const;

    /**
     * @brief Crea una copia profunda de esta expresión.
     *
     * Llama al método correspondiente al tipo verdadero de la expresión actual, por lo
     * que la complejidad de esta operación es aproximadamente $O(2^h)$ en el peor de 
     * los casos, siendo $h$ la altura del subárbol cuya raíz es esta expresión.
     * 
     * @return Una nueva instancia de `Expression` equivalente a ésta.
     */
    Expression clone() const noexcept;

    /**
     * @brief Evalúa la expresión utilizando una tabla de símbolos.
     *
     * Llama al método correspondiente al tipo verdadero de la expresión actual.
     * Debido a tratarse de una expresión que puede contener cualquier otra subexpresión, no se puede garantizar qué tipo de errores de evaluación
     * puede lanzar este método, solo se asegura que serán de tipo `EvalError` o alguna de sus subclases.
     *
     * @param symbol_table Tabla de símbolos usada para la evaluación.
     * @return Resultado numérico de la evaluación.
     * @exception Lanza un `EvalError` si ha habido problemas en la evaluación de la expresión. Por ello, se recomienda encerrar llamadas a este método en un bloque `try ... catch`.
     */
    double evaluate(const SymbolTable& symbols) const;

    friend std::ostream& operator<<(std::ostream& out, const Expression& expr);
};

/**
 * @brief Operador de inserción en flujo de salida (para uso con 
 * `std::cout` y similares)
 *
 * Llama al operador correspondiente al tipo verdadero de la expresión actual.
 * Convierte la expresión a una cadena con información dependiente del tipo de expresión y la imprime.
 * 
 * @param out El flujo de salida.
 * @param expr La expresión a imprimir.
 * @return Referencia a `out` después de la operación.
 */
std::ostream& operator<<(std::ostream& out, const Expression& expr);

/**
 * @brief Representa una asignación.
 *
 * Una asignación asocia el valor de una expresión al identificador
 * indicado por el token del lado izquierdo.
 */
class Assignment {
  private:
    Token m_variable_lhs; /**< Token del identificador asignado. */
    std::unique_ptr<Expression> m_rhs; /**< Puntero a la expresión del lado derecho. */
  public:
    /**
     * @brief Constructor
     *
     * @param variable_lhs Token del identificador del lado izquierdo.
     * @param rhs Expresión cuyo valor será asignado.
     * @exception Lanza `std::invalid_argument` si `variable_lhs` no es un token de tipo `TokenType::IDENTIFIER`
     */
    Assignment(Token&& variable_lhs, std::unique_ptr<Expression>&& rhs);


    /**
     * @brief Obtiene el token de la variable asignada.
     *
     * @return Referencia constante al token del lado izquierdo.
     */
    const Token& get_var() const noexcept;

    /**
     * @brief Obtiene la expresión asociada a la asignación.
     *
     * @return Referencia constante al puntero de la expresión.
     */
    const std::unique_ptr<Expression>& get_value() const noexcept;
    
    /**
     * @brief Ejecuta la asignación sobre una tabla de símbolos.
     *
     * Debido a tratarse de una expresión que puede contener cualquier otra subexpresión, no se puede garantizar qué tipo de errores de evaluación
     * puede lanzar este método, solo se asegura que serán de tipo `EvalError` o alguna de sus subclases.
     * La tabla de símbolos pasada comoo argumento será modificada si la expresión de la derecha de la asignación no
     * lanza error al ser evaluada.
     *
     * @param symbols La tabla de símbolos.
     */
    void execute(SymbolTable& symbols) const;

    friend std::ostream& operator<<(std::ostream& out, const Assignment& assign);
};

/**
 * @brief Operador de inserción en flujo de salida (para uso con 
 * `std::cout` y similares)
 *
 * Convierte la asignación a una cadena con información sobre la variable asignada y la expresión contenida y la imprime.
 * 
 * @param out El flujo de salida.
 * @param assign La asignación a imprimir.
 * @return Referencia a `out` después de la operación.
 */
std::ostream& operator<<(std::ostream& out, const Assignment& assign);

/**
 * @brief Representa una sentencia.
 *
 * Una sentencia puede contener o bien una `Expression`, que es evaluada y su resultado mostrado,
 * o bien un `Assignment` que es llevado a cabo modificando una tabla de símbolos.
 */
class Statement {
  private:
    std::variant<Expression, Assignment> m_data;

    Statement(Expression&& expr) noexcept;
    Statement(Assignment&& assign) noexcept;
    Statement() = delete;
  public:  
    /**
     * @brief Crea una sentencia a partir de una expresión.
     *
     * @param expr Expresión que compone la sentencia.
     * @return Nueva sentencia de tipo expresión.
     */
    static Statement expression(Expression&& expr) noexcept;

    /**
     * @brief Crea una sentencia a partir de una asignación.
     *
     * @param assign Asignación que compone la sentencia.
     * @return Nueva sentencia de tipo asignación.
     */
    static Statement assignment(Assignment&& assign) noexcept;

    /**
     * @brief Comprueba si la sentencia es una expresión.
     *
     * @return `true` si es una expresión, `false` en caso contrario.
     */
    bool is_expression() const noexcept;

    /**
     * @brief Comprueba si la sentencia es una asignación.
     *
     * @return `true` si es una asignación, `false` en caso contrario.
     */
    inline bool is_assignment() const noexcept { return !is_expression(); }

    /**
     * @brief Extrae la expresión, moviéndola fuera del objeto.
     *
     * El objeto `Statement` en el que se llama el método quedará en un estado indefinido tras la
     * llamada, por lo que no se puede utilizar otro método en el mismo objeto.
     *
     * @return La expresión contenida en la sentencia.
     * @pre El objeto debe contener una `Expression` (es decir, `is_expression()` debe devolver `true`)
     */
    Expression&& move_as_expression();

    /**
     * @brief Extrae la asignación moviéndola.
     *
     * El objeto `Statement` en el que se llama el método quedará en un estado indefinido tras la
     * llamada, por lo que no se puede utilizar otro método en el mismo objeto.
     *
     * @return Asignación contenida en la sentencia.
     * @pre El objeto debe contener un `Assignment` (es decir, `is_assignment()` debe devolver `true`)
     */
    Assignment&& move_as_assignment();

    /**
     * @brief Obtiene una referencia constante a la expresión contenida.
     *
     * @return Referencia constante a la expresión.
     * @pre El objeto debe contener una `Expression` (es decir, `is_expression()` debe devolver `true`)
     */
    const Expression& ref_as_expression() const;

    /**
     * @brief Obtiene una referencia constante a la asignación contenida.
     *
     * @return Referencia constante a la asignación.
     * @pre El objeto debe contener un `Assignment` (es decir, `is_assignment()` debe devolver `true`)
     */
    const Assignment& ref_as_assignment() const;
};

} // namespace clex
