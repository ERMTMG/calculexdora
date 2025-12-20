/**
 * @file tokens.hpp
 * @brief Definición de las clases necesarias para tokens.
 *
 * @author Eduardo Rodríguez, Raúl Gabaldón
 * @date 2025-12
 */
#pragma once
#include <iostream>
#include <optional>
#include <ostream>
#include <variant>
#include <string>

namespace clex {

/**
 * @brief Tipo enumerado que describe todos los tipos de tokens (números, 
 * identificadores, operadores, funciones,...)
 *
 * No confundir con la clase `Token`, que tiene información sobre tokens completos con información adicional.
 */
enum class TokenType {
    ERROR_TOKEN = -1, // Token erróneo
    END_OF_FILE,    // Token especial para el fin de archivo
    NEWLINE,        // Línea nueva
    NUMBER,         // Números
    IDENTIFIER,     // Identificadores (nombres de variables)
    OP_PLUS,        // Operador "+"
    OP_MINUS,       // Operador "-"
    OP_ASTERISK,    // Operador "*"
    OP_SLASH,       // Operador "/"
    OP_CARET,       // Operador "^"
    OP_FUNC_SQRT,   // Función "sqrt"
    OP_FUNC_LOG,    // Función "log"
    OP_FUNC_SIN,    // Función "sin"
    OP_FUNC_COS,    // Función "cos"
    OP_FUNC_TAN,    // Función "tan"
    OP_FUNC_ARCSIN, // Función "arcsin"
    OP_FUNC_ARCCOS, // Función "arccos"
    OP_FUNC_ARCTAN, // Función "arctan"
    ASSIGN,         // Operador de asignación "="  
    PAREN_L,        // Paréntesis "("
    PAREN_R,        // Paréntesis ")"
};

/**
 * @brief Operador de inserción en flujo de salida (para uso con 
 * `std::cout` y similares)
 *
 * Convierte el tipo de token a una cadena dependiente del tipo en sí (sin información adicional) y la imprime.
 * 
 * @param out El flujo de salida.
 * @param token_type El tipo de token a imprimir
 * @return Referencia a `out` después de la operación
 */
std::ostream& operator<<(std::ostream& out, TokenType token_type) noexcept;

/**
 * @brief Clase para los tokens como tal, que define información adicional aparte del tipo.
 *
 * Aparte del tipo de token, contiene información como el valor para tokens numéricos y el nombre de
 * identificador para tokens de identificador.
 */
class Token {
  public:
    using TokenVariant = std::variant<std::monostate, std::string, double>;
  private:
    TokenVariant m_data;
    TokenType m_type;
    Token(TokenType type, TokenVariant&& info) noexcept;
  public:
    /**
    * @brief Constructor por defecto, construye un token de error.
    */
    Token();
    ;// Constructores y asignaciones de copia y movimiento, todos por defecto.
    Token(const Token&) = default;
    Token(Token&&) = default;
    Token& operator=(const Token&) = default;
    Token& operator=(Token&&) = default;

    /**
    * @brief Constructor a partir de un `TokenType`, usado para construir Tokens sin contexto adicional.
    *
    * Este constructor debe ser usado para tokens de operadores, símbolos, etc..., no números ni identificadores.
    *
    * @param type El tipo de token.
    * @exception Lanza `std::invalid_argument` si `type == TokenType::NUMBER` ó `type == TokenType::IDENTIFIER`. Para estos tipos de token, debe usarse `Token::number` ó `Token::identifier.`
    */
    Token(TokenType type);
    /**
    * @brief Construye y devuelve un token numérico a partir de un `std::string` que contiene un número.
    *
    * Esta función es la que debe ser usada para tokens de tipo `TokenType::NUMBER`.
    *
    * @param num Cadena que contiene un número para almacenar en el token, como `"2"`, `"5.43"`, etc...
    * @return El token numérico construido.
    * @pre `num` es analizable por la función de la librería estándar `std::stod()`. 
    */
    static Token number(const std::string& num) noexcept;

    /**
    * @brief Construye y devuelve un token de identificador a partir de un `std::string` que contiene un nombre.
    *
    * Esta función es la que debe ser usada para tokens de tipo `TokenType::IDENTIFIER`.
    *
    * @param str Cadena que contiene el nombre del identificador.
    * @return El token de identificador construido.
    */
    static Token identifier(const std::string& str) noexcept;

    /**
    * @brief Obtiene el número almacenado en un token de tipo `TokenType::NUMBER`
    *
    * Este método devuelve un `std::optional` vacío si el token sobre el que se llama no tiene ningún valor numérico almacenado.
    *
    * @return un `std::optional<double>` que contiene el valor numérico del token si su tipo es `TokenType::NUMBER`, y vacío si no.
    */
    std::optional<double> get_num() const noexcept;

    /**
    * @brief Obtiene el nombre de identificador almacenado en un token de tipo `TokenType::IDENTIFIER`
    *
    * Este método devuelve un `std::optional` vacío si el token sobre el que se llama no tiene ningún valor de nombre almacenado.
    *
    * @return un `std::optional<std::string>` que contiene el valor de nombre del token si su tipo es `TokenType::IDENTIFIER`, y vacío si no.
    */
    std::optional<std::string> get_ident() const noexcept;

    /**
    * @brief Obtiene el *binding power* correspondiente a un token de operador binario
    *
    * Este método devuelve un `std::optional` vacío si el token sobre el que se llama no actúa como operador binario.
    * El *binding power* es usado por el analizador sintáctico a la hora de construir árboles de expresión, y es análogo a la 
    * precedencia de los operadores. Por ejemplo, el _binding power_ de un token de tipo `TokenType::OP_ASTERISK` es mayor que el
    * de un token de tipo `TokenType::OP_PLUS`.
    *
    * @return un `std::optional<int>` que contiene el _binding power_ del token como operador binario si su tipo lo soporta, y vacío si no.
    */
    std::optional<int> get_binary_binding_power() const noexcept;

    /**
    * @brief Obtiene el *binding power* correspondiente a un token de operador unario o función
    *
    * Este método devuelve un `std::optional` vacío si el token sobre el que se llama no actúa como operador unario o función.
    * El *binding power* es usado por el analizador sintáctico a la hora de construir árboles de expresión, y es análogo a la 
    * precedencia de los operadores. Por ejemplo, el _binding power_ de un token de tipo `TokenType::OP_MINUS` es mayor que el
    * de un token de tipo `TokenType::OP_FUNC_LOG`.
    *
    * @return un `std::optional<int>` que contiene el _binding power_ del token como operador unario si su tipo lo soporta, y vacío si no.
    */
    std::optional<int> get_unary_binding_power() const noexcept;

    /**
    * @brief Devuelve el tipo del token.
    * .
    * @return El tipo del token sobre el que se llama, como un enumerado `TokenType`.
    */
    TokenType type() const noexcept;

    /**
    * @brief Comprueba si el token actúa como operador unario o función.
    *
    * Los tokens que actúan como operadores unarios son los de tipo (`TokenType::`) `OP_PLUS`, `OP_MINUS`, `OP_FUNC_SQRT`,
    * `OP_FUNC_LOG`, `OP_FUNC_SIN`, `OP_FUNC_COS`, `OP_FUNC_TAN`, `OP_FUNC_ARCSIN`, `OP_FUNC_ARCCOS` y `OP_FUNC_ARCTAN`.
    * .
    * @return `true` si el token sobre el que se llama es de uno de los tipos mencionados anteriormente, `false` si no.
    */
    bool is_unary_operator_token() const noexcept;

    /**
    * @brief Comprueba si el token actúa como operador binario.
    *
    * Los tokens que actúan como operadores binarios son los de tipo (`TokenType::`) `OP_PLUS`, `OP_MINUS`, `OP_ASTERISK`,
    * `OP_SLASH` y `OP_CARET`.
    * .
    * @return `true` si el token sobre el que se llama es de uno de los tipos mencionados anteriormente, `false` si no.
    */
    bool is_binary_operator_token() const noexcept;

    /**
    * @brief Comprueba si el token actúa como operador unario o binario.
    *
    * Este método es equivalente a llamar `is_unary_operator_token() || is_binary_operator_token()`.
    * .
    * @return `true` si el token sobre el que se llama es un operador, `false` si no.
    */
    bool is_operator_token() const noexcept;

    /**
    * @brief Comprueba si el token actúa como operando.
    *
    * Los tokens que actúan como operandos son los de tipo `TokenType::NUMBER` y `TokenType::IDENTIFIER`.
    * .
    * @return `true` si el token sobre el que se llama actúa como operando, `false` si no.
    */
    bool is_operand_token() const noexcept;

    /**
    * @brief Comprueba si el token es un operador binario asociativo hacia la derecha.
    *
    * Por ahora, solo existe un operador binario asociativo hacia la derecha, los tokens de tipo `TokenType::OP_CARET`.
    * Los operadores asociativos a la derecha son aquellos que en análisis sintáctico se agrupan a la derecha, si los paréntesis
    * no especifican lo contrario. Por ejemplo, `2 ^ 2 ^ 2 ^ 2` se analiza como `2 ^ (2 ^ (2 ^ 2))` y no como `((2 ^ 2) ^ 2) ^ 2`.
    * .
    * @return `true` si el token sobre el que se llama es un operador asociativo a la derecha, `false` si no.
    */
    bool is_right_associative() const noexcept;
    
    /**
    * @brief Operador de comparación de igualdad entre tokens
    *
    * @param rhs El token con el que `*this` se va a comparar.
    * @return `true` si `*this` tiene un estado equivalente a `rhs`, `false` en caso contrario.
    */
    bool operator==(const Token& rhs) const noexcept;

    /**
    * @brief Operador de comparación de desigualdad entre tokens
    *
    * @param rhs El token con el que `*this` se va a comparar.
    * @return `true` si `*this` **no** tiene un estado equivalente a `rhs`, `false` en caso contrario.
    */
    bool operator!=(const Token& rhs) const noexcept;
    friend std::ostream& operator<<(std::ostream& out, const Token& tok) noexcept;
};
/**

 * @brief Operador de inserción en flujo de salida (para uso con 
 * `std::cout` y similares)
 *
 * Convierte el token a una cadena (con el tipo de token e información adicional si la tiene) y la imprime.
 * 
 * @param out El flujo de salida.
 * @param token_type El token a imprimir
 * @return Referencia a `out` después de la operación
 */
std::ostream& operator<<(std::ostream& out, const Token& tok) noexcept;

}
