/**
 * @file definitions.hpp
 * @author Andoni Zalaya (andoni.zalaya.c@gmail.com)
 * @brief Common definitions used throughout the project.
 * @version 0.1
 * @date 2026-09-15
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#ifndef DEFINITIONS_HPP
#define DEFINITIONS_HPP

#include <string_view>
#include <unordered_map>

enum class LogLevel
{
    eINFO,
    eDEBUG,
    eWARNING,
    eERROR
};

static constexpr std::string_view INFO_STRING = "INFO";
static constexpr std::string_view DEBUG_STRING = "DEBUG";
static constexpr std::string_view WARNING_STRING = "WARNING";
static constexpr std::string_view ERROR_STRING = "ERROR";

static const std::unordered_map<LogLevel, std::string_view> LOG_LEVEL_TO_STRING = {
    { LogLevel::eINFO, INFO_STRING },
    { LogLevel::eDEBUG, DEBUG_STRING },
    { LogLevel::eWARNING, WARNING_STRING },
    { LogLevel::eERROR, ERROR_STRING }
};

namespace FrontEnd {

    enum class TokenType {
        eEOF,

        // Commands
        eDEFINITION,
        eEXTERN,
        eIF,
        eELSE,

        // Primary
        eIDENTIFIER,
        eNUMBER,

        // Braces
        ePARENTHESIS_START,
        ePARENTHESIS_FINISH,
        eCURLY_BRACE_START,
        eCURLY_BRACE_FINISH,
        eSQUARE_BRACKET_START,
        eSQUARE_BRACKET_FINISH,

        // Operators
        eLESS_THAN,
        eSUM,
        eMINUS,
        eMULTIPLY,

        // Separators
        eEOL,
        eCOMMA,

        eUNKNOWN
    };

    size_t constexpr SIZE_8  = 0b0000;
    size_t constexpr SIZE_16 = 0b0001;
    size_t constexpr SIZE_32 = 0b0010;
    size_t constexpr SIZE_64 = 0b0011;

    size_t constexpr INTEGER        = 0b0000;
    size_t constexpr FLOATING_POINT = 0b0100;

    size_t constexpr SIGNED   = 0b0000;
    size_t constexpr UNSIGNED = 0b1000;

    enum class NumericType {
        eINT8 = SIGNED | INTEGER | SIZE_8,
        eINT16 = SIGNED | INTEGER | SIZE_16,
        eINT32 = SIGNED | INTEGER | SIZE_32,
        eINT64 = SIGNED | INTEGER | SIZE_64,
        eUINT8 = UNSIGNED | INTEGER | SIZE_8,
        eUINT16 = UNSIGNED | INTEGER | SIZE_16,
        eUINT32 = UNSIGNED | INTEGER | SIZE_32,
        eUINT64 = UNSIGNED | INTEGER | SIZE_64,
        eFLOAT32 = SIGNED | FLOATING_POINT | SIZE_32,
        eFLOAT64 = SIGNED | FLOATING_POINT | SIZE_64,
        eUNKNOWN = -1
    };

    enum class BinaryOperators {
        eLESS_THAN = 1,
        eSUM,
        eMINUS,
        eMULTIPLY,

        eUNKNOWN = -1
    };

    static constexpr std::string_view EOF_STRING = "EOF";
    static constexpr std::string_view DEFINITION_STRING = "DEFINITION";
    static constexpr std::string_view EXTERN_STRING = "EXTERN";
    static constexpr std::string_view IF_STRING = "IF";
    static constexpr std::string_view ELSE_STRING = "ELSE";
    static constexpr std::string_view IDENTIFIER_STRING = "IDENTIFIER";
    static constexpr std::string_view NUMBER_STRING = "NUMBER";
    static constexpr std::string_view PARENTHESIS_START_STRING = "PARENTHESIS_START";
    static constexpr std::string_view PARENTHESIS_FINISH_STRING = "PARENTHESIS_FINISH";
    static constexpr std::string_view CURLY_BRACE_START_STRING = "CURLY_BRACE_START";
    static constexpr std::string_view CURLY_BRACE_FINISH_STRING = "CURLY_BRACE_FINISH";
    static constexpr std::string_view SQUARE_BRACKET_START_STRING = "SQUARE_BRACKET_START";
    static constexpr std::string_view SQUARE_BRACKET_FINISH_STRING = "SQUARE_BRACKET_FINISH";
    static constexpr std::string_view LESS_THAN_STRING = "LESS_THAN";
    static constexpr std::string_view SUM_STRING = "SUM";
    static constexpr std::string_view MINUS_STRING = "MINUS";
    static constexpr std::string_view MULTIPLY_STRING = "MULTIPLY";
    static constexpr std::string_view EOL_STRING = "EOL";
    static constexpr std::string_view COMMA_STRING = "COMMA";
    static constexpr std::string_view UNKNOWN_STRING = "UNKNOWN";

    static const std::unordered_map<TokenType, std::string_view> TOKEN_TO_STRING = {
        { TokenType::eEOF, EOF_STRING },
        { TokenType::eDEFINITION, DEFINITION_STRING },
        { TokenType::eEXTERN, EXTERN_STRING },
        { TokenType::eIF, IF_STRING },
        { TokenType::eELSE, ELSE_STRING },
        { TokenType::eIDENTIFIER, IDENTIFIER_STRING },
        { TokenType::eNUMBER, NUMBER_STRING },
        { TokenType::ePARENTHESIS_START, PARENTHESIS_START_STRING },
        { TokenType::ePARENTHESIS_FINISH, PARENTHESIS_FINISH_STRING },
        { TokenType::eCURLY_BRACE_START, CURLY_BRACE_START_STRING },
        { TokenType::eCURLY_BRACE_FINISH, CURLY_BRACE_FINISH_STRING },
        { TokenType::eSQUARE_BRACKET_START, SQUARE_BRACKET_START_STRING },
        { TokenType::eSQUARE_BRACKET_FINISH, SQUARE_BRACKET_FINISH_STRING },
        { TokenType::eLESS_THAN, LESS_THAN_STRING },
        { TokenType::eSUM, SUM_STRING },
        { TokenType::eMINUS, MINUS_STRING },
        { TokenType::eMULTIPLY, MULTIPLY_STRING },
        { TokenType::eEOL, EOL_STRING },
        { TokenType::eCOMMA, COMMA_STRING },
        { TokenType::eUNKNOWN, UNKNOWN_STRING }
    };

    static const std::unordered_map<BinaryOperators, std::string_view> BINARY_OPERATOR_TO_STRING = {
        { BinaryOperators::eLESS_THAN, LESS_THAN_STRING },
        { BinaryOperators::eSUM, SUM_STRING },
        { BinaryOperators::eMINUS, MINUS_STRING },
        { BinaryOperators::eMULTIPLY, MULTIPLY_STRING }
    };

    static const std::unordered_map<TokenType, BinaryOperators> TOKEN_TO_OPERATOR = {
        { TokenType::eLESS_THAN, BinaryOperators::eLESS_THAN },
        { TokenType::eSUM, BinaryOperators::eSUM },
        { TokenType::eMINUS, BinaryOperators::eMINUS },
        { TokenType::eMULTIPLY, BinaryOperators::eMULTIPLY },
    };

    static constexpr std::string_view INT8_STRING = "INT8";
    static constexpr std::string_view INT16_STRING = "INT16";
    static constexpr std::string_view INT32_STRING = "INT32";
    static constexpr std::string_view INT64_STRING = "INT64";
    static constexpr std::string_view UINT8_STRING = "UINT8";
    static constexpr std::string_view UINT16_STRING = "UINT16";
    static constexpr std::string_view UINT32_STRING = "UINT32";
    static constexpr std::string_view UINT64_STRING = "UINT64";
    static constexpr std::string_view FLOAT32_STRING = "FLOAT32";
    static constexpr std::string_view FLOAT64_STRING = "FLOAT64";

    static const std::unordered_map<NumericType, std::string_view> NUMERIC_TO_STRING = {
        { NumericType::eINT8, INT8_STRING },
        { NumericType::eINT16, INT16_STRING },
        { NumericType::eINT32, INT32_STRING },
        { NumericType::eINT64, INT64_STRING },
        { NumericType::eUINT8, UINT8_STRING },
        { NumericType::eUINT16, UINT16_STRING },
        { NumericType::eUINT32, UINT32_STRING },
        { NumericType::eUINT64, UINT64_STRING },
        { NumericType::eFLOAT32, FLOAT32_STRING },
        { NumericType::eFLOAT64, FLOAT64_STRING },
        { NumericType::eUNKNOWN, UNKNOWN_STRING }
    };

} // namespace FrontEnd

#endif // DEFINITIONS_HPP