/**
 * @file definitions.hpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2026-09-15
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#ifndef DEFINITIONS_HPP
#define DEFINITIONS_HPP

#include <string>
#include <unordered_map>

namespace FrontEnd {

    enum class TokenType {
        eEOF,

        // Commands
        eDEFINITION,
        eEXTERN,

        // Primary
        eIDENTIFIER,
        eNUMBER,

        eUNKNOWN
    };

    static constexpr std::string EOF_STRING = "EOF";
    static constexpr std::string DEFINITION_STRING = "DEFINITION";
    static constexpr std::string EXTERN_STRING = "EXTERN";
    static constexpr std::string IDENTIFIER_STRING = "IDENTIFIER";
    static constexpr std::string NUMBER_STRING = "NUMBER";
    static constexpr std::string UNKNOWN_STRING = "UNKNOWN";

    static const std::unordered_map<TokenType, std::string> TOKEN_TO_STRING = {
        { TokenType::eEOF, EOF_STRING },
        { TokenType::eDEFINITION, DEFINITION_STRING },
        { TokenType::eEXTERN, EXTERN_STRING },
        { TokenType::eIDENTIFIER, IDENTIFIER_STRING },
        { TokenType::eNUMBER, NUMBER_STRING },
        { TokenType::eUNKNOWN, UNKNOWN_STRING }
    };

} // namespace FrontEnd

#endif // DEFINITIONS_HPP