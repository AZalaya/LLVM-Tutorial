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

#include "front_end/tokens.hpp"

 namespace FrontEnd {

    Token::Token(TokenType t, const std::string& v) 
    : mType(t)
    , mValue(v) 
    {}

    std::string Token::ToString() const
    {
        switch (mType) {
            case TokenType::eEOF: return "EOF";
            case TokenType::eDEFINITION: return "DEFINITION";
            case TokenType::eEXTERN: return "EXTERN";
            case TokenType::eIDENTIFIER: return "IDENTIFIER(" + mValue + ")";
            case TokenType::eNUMBER: return "NUMBER(" + mValue + ")";
            default: return "UNKNOWN(" + mValue + ")";
        }
    }

}