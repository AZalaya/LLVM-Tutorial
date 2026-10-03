/**
 * @file tokens.cpp
 * @author Andoni Zalaya (andoni.zalaya.c@gmail.com)
 * @brief Implementation file for the TokenType enum and Token struct used in the lexer.
 * @version 0.1
 * @date 2026-09-13
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#include "tokens.hpp"

 namespace FrontEnd {

    Token::Token(TokenType token_type, const std::string& value) 
    : mType(token_type)
    , mValue(value) 
    {}

    std::string Token::ToString() const
    {
        return "Token(Type: " + std::string(TOKEN_TO_STRING.at(mType)) + ", Value: " + mValue + ")";
    }

}