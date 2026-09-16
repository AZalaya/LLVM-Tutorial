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
        eUFLOAT32 = UNSIGNED | FLOATING_POINT | SIZE_32,
        eUFLOAT64 = UNSIGNED | FLOATING_POINT | SIZE_64,
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

    static constexpr std::string INT8_STRING = "INT8";
    static constexpr std::string INT16_STRING = "INT16";
    static constexpr std::string INT32_STRING = "INT32";
    static constexpr std::string INT64_STRING = "INT64";
    static constexpr std::string UINT8_STRING = "UINT8";
    static constexpr std::string UINT16_STRING = "UINT16";
    static constexpr std::string UINT32_STRING = "UINT32";
    static constexpr std::string UINT64_STRING = "UINT64";
    static constexpr std::string FLOAT32_STRING = "FLOAT32";
    static constexpr std::string FLOAT64_STRING = "FLOAT64";
    static constexpr std::string UFLOAT32_STRING = "UFLOAT32";
    static constexpr std::string UFLOAT64_STRING = "UFLOAT64";

    static const std::unordered_map<NumericType, std::string> NUMERIC_TO_STRING = {
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
        { NumericType::eUFLOAT32, UFLOAT32_STRING },
        { NumericType::eUFLOAT64, UFLOAT64_STRING },
        { NumericType::eUNKNOWN, UNKNOWN_STRING }
    };

} // namespace FrontEnd

#endif // DEFINITIONS_HPP