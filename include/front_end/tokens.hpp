/**
 * @file tokens.hpp
 * @author Andoni Zalaya (andoni.zalaya.c@gmail.com)
 * @brief  Header file for the TokenType enum and Token struct used in the lexer.
 * @version 0.1
 * @date 2026-09-13
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#ifndef TOKENS_HPP
#define TOKENS_HPP

#include "definitions.hpp"

#include <string>

namespace FrontEnd {

    struct Token 
    {
        TokenType mType;
        std::string mValue;

        /**
         * @brief Construct a new Token object
         * 
         * @param token_type 
         * @param value 
         */
        Token(TokenType token_type, const std::string& value);

        /**
         * @brief Returns a string representation of the token.
         * 
         * @return std::string 
         */
        std::string ToString() const;
    };

}

#endif // TOKENS_HPP