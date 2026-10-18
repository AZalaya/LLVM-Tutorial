#include "logger.hpp"

namespace FrontEnd {
namespace AST {

template <typename T>
T NumberNode::getValue() const
{
    if (mNumericType == NumericType::eINT8) {
        if (!std::is_same<T, char>::value) {
            Logger::GetLogger().Log(LogLevel::eWARNING, "Type mismatch: Stored value is INT8, but requested different type. Attempting type cast.");
        }

        char result;
        std::memcpy(&result, &mValue, sizeof(result));
        return static_cast<T>(result);
    }
    if (mNumericType == NumericType::eINT16) {
        if (!std::is_same<T, short>::value) {
            Logger::GetLogger().Log(LogLevel::eWARNING, "Type mismatch: Stored value is INT16, but requested different type. Attempting type cast.");
        }
        
        short result;
        std::memcpy(&result, &mValue, sizeof(result));
        return static_cast<T>(result);
    }
    if (mNumericType == NumericType::eINT32) {
        if (!std::is_same<T, int>::value) {
            Logger::GetLogger().Log(LogLevel::eWARNING, "Type mismatch: Stored value is INT32, but requested different type. Attempting type cast.");
        }
        
        int result;
        std::memcpy(&result, &mValue, sizeof(result));
        return static_cast<T>(result);
    }
    if (mNumericType == NumericType::eINT64) {
        if (!std::is_same<T, long>::value) {
            Logger::GetLogger().Log(LogLevel::eWARNING, "Type mismatch: Stored value is INT64, but requested different type. Attempting type cast.");
        }
        
        long result;
        std::memcpy(&result, &mValue, sizeof(result));
        return static_cast<T>(result);
    }
    if (mNumericType == NumericType::eUINT8) {
        if (!std::is_same<T, unsigned char>::value) {
            Logger::GetLogger().Log(LogLevel::eWARNING, "Type mismatch: Stored value is UINT8, but requested different type. Attempting type cast.");
        }
        
        unsigned char result;
        std::memcpy(&result, &mValue, sizeof(result));
        return static_cast<T>(result);
    }
    if (mNumericType == NumericType::eUINT16) {
        if (!std::is_same<T, unsigned short>::value) {
            Logger::GetLogger().Log(LogLevel::eWARNING, "Type mismatch: Stored value is UINT16, but requested different type. Attempting type cast.");
        }
        
        unsigned short result;
        std::memcpy(&result, &mValue, sizeof(result));
        return static_cast<T>(result);
    }
    if (mNumericType == NumericType::eUINT32) {
        if (!std::is_same<T, unsigned int>::value) {
            Logger::GetLogger().Log(LogLevel::eWARNING, "Type mismatch: Stored value is UINT32, but requested different type. Attempting type cast.");
        }
        
        unsigned int result;
        std::memcpy(&result, &mValue, sizeof(result));
        return static_cast<T>(result);
    }
    if (mNumericType == NumericType::eUINT64) {
        if (!std::is_same<T, unsigned long>::value) {
            Logger::GetLogger().Log(LogLevel::eWARNING, "Type mismatch: Stored value is UINT64, but requested different type. Attempting type cast.");
        }
        
        unsigned long result;
        std::memcpy(&result, &mValue, sizeof(result));
        return static_cast<T>(result);
    }
    if (mNumericType == NumericType::eFLOAT32) {
        if (!std::is_same<T, float>::value) {
            Logger::GetLogger().Log(LogLevel::eWARNING, "Type mismatch: Stored value is FLOAT32, but requested different type. Attempting type cast.");
        }
        
        float result;
        std::memcpy(&result, &mValue, sizeof(result));
        return static_cast<T>(result);
    }
    if (mNumericType == NumericType::eFLOAT64) {
        if (!std::is_same<T, double>::value) {
            Logger::GetLogger().Log(LogLevel::eWARNING, "Type mismatch: Stored value is FLOAT64, but requested different type. Attempting type cast.");
        }
        
        double result;
        std::memcpy(&result, &mValue, sizeof(result));
        return static_cast<T>(result);
    }
    
    Logger::GetLogger().Log(LogLevel::eERROR, "Stored value has unknown numeric type.");
    return T(0); // Return default value for unknown type            
}

} // namespace AST
} // namespace FrontEnd