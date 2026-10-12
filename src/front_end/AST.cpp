/**
 * @file AST.cpp
 * @author Andoni Zalaya (andoni.zalaya.c@gmail.com)
 * @brief Implementation file for the Abstract Syntax Tree (AST) classes, which represent the structure of the parsed code.
 * @version 0.1
 * @date 2026-09-13
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#include "AST.hpp"

#include "logger.hpp"

#include <cstring>
#include <type_traits>

namespace FrontEnd {
namespace AST {

    // NUMberNode implementation ===================================

    NumberNode::NumberNode(const std::string& value)
    : mValue(0)
    , mNumericType(NumericType::eUNKNOWN)
    {
        std::size_t type = 0;

        if (value.back() == 'u' || value.back() == 'U') {
            type |= UNSIGNED;
        }
        else if (std::size_t size = value.size();
                 size > 1 && (value.at(size - 2) == 'u' || value.at(size - 2) == 'U')) {
            type |= UNSIGNED;
        }
        else {
            type |= SIGNED;
        }

        std::size_t point = value.find('.');

        if (point != std::string::npos) {
            if (type & UNSIGNED) {
                Logger::GetLogger().Log(LogLevel::eWARNING, "Unsigned floating-point numbers are not standard. Defaulting to signed.");

                type = (type & ~UNSIGNED) | SIGNED;
            }

            if (value.back() == 'f' || value.back() == 'F') {
                type |=  FLOATING_POINT | SIZE_32;
                float tempValue = std::stof(value);
                std::memcpy(&mValue, &tempValue, sizeof(tempValue));
            } 
            else if (value.back() == 'd' || value.back() == 'D') {
                type |= FLOATING_POINT | SIZE_64;
                double tempValue = std::stod(value);
                std::memcpy(&mValue, &tempValue, sizeof(tempValue));
            } 
            else {
                type |= FLOATING_POINT | SIZE_64; // Default to FLOAT64 for floating-point values
                double tempValue = std::stod(value);
                std::memcpy(&mValue, &tempValue, sizeof(tempValue));
            }
        } 
        else if (value.back() == 'f' || value.back() == 'F') {
            type |=  FLOATING_POINT | SIZE_32;
            float tempValue = std::stof(value);
            std::memcpy(&mValue, &tempValue, sizeof(tempValue));
        } 
        else if (value.back() == 'd' || value.back() == 'D') {
            type |= FLOATING_POINT | SIZE_64;
            double tempValue = std::stod(value);
            std::memcpy(&mValue, &tempValue, sizeof(tempValue));
        } 
        else if (value.back() == 'l' || value.back() == 'L') {
            type |= INTEGER | SIZE_64;
            long tempValue = std::stol(value);
            std::memcpy(&mValue, &tempValue, sizeof(tempValue));
        } 
        else if (value.back() == 'i' || value.back() == 'I') {
            type |= INTEGER | SIZE_32;
            int tempValue = std::stoi(value);
            std::memcpy(&mValue, &tempValue, sizeof(tempValue));
        } 
        else if (value.back() == 's' || value.back() == 'S') {
            type |= INTEGER | SIZE_16;
            short tempValue = static_cast<short>(std::stoi(value));
            std::memcpy(&mValue, &tempValue, sizeof(tempValue));
        } 
        else if (value.back() == 'c' || value.back() == 'C') {
            type |= INTEGER | SIZE_8;
            char tempValue = static_cast<char>(std::stoi(value));
            std::memcpy(&mValue, &tempValue, sizeof(tempValue));
        } 
        else {
            type |= INTEGER | SIZE_32; // Default to INT32 for integer values
            int tempValue = std::stoi(value);
            std::memcpy(&mValue, &tempValue, sizeof(tempValue));
        }

        mNumericType = static_cast<NumericType>(type);
    }

    std::string NumberNode::ToString() const 
    {
        return "NumberNode: " + std::to_string(mValue) + " (" + std::string(NUMERIC_TO_STRING.at(mNumericType)) + ")\n";
    }

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

    // VariableNode implementation ===================================

    VariableNode::VariableNode(const std::string& name)
        : mName(name) 
    {}

    std::string VariableNode::ToString() const 
    {
        return "VariableNode: " + mName + '\n';
    }

    // BinaryOpNode implementation ===================================

    BinaryOpNode::BinaryOpNode(BinaryOperators op, std::unique_ptr<ASTNode> left, std::unique_ptr<ASTNode> right)
        : mOp(op), mLeft(std::move(left)), mRight(std::move(right)) 
    {}

    std::string BinaryOpNode::ToString() const 
    {
        std::string result = "BinaryOpNode: " + std::string(BINARY_OPERATOR_TO_STRING.at(mOp)) + '\n';
        if (mLeft) {
            result += mLeft->ToString();
        }
        if (mRight) {
            result += mRight->ToString();
        }
        
        return result;
    }

    // IfStatementNode implementation ===================================

    IfStatementNode::IfStatementNode(std::unique_ptr<ASTNode> condition, std::vector<std::unique_ptr<ASTNode>> then_branch, std::vector<std::unique_ptr<ASTNode>> else_branch)
        : mCondition(std::move(condition)), mThenBranch(std::move(then_branch)), mElseBranch(std::move(else_branch)) 
    {}

    std::string IfStatementNode::ToString() const 
    {
        std::string result = "IfStatementNode: \n";
        if (mCondition) {
            result += "Condition: \n" + mCondition->ToString();
        }
        result += "Then Branch: \n";
        for (const std::unique_ptr<ASTNode>& node : mThenBranch) {
            if (node) {
                result += node->ToString();
            }
        }
        result += "Else Branch: \n";
        for (const std::unique_ptr<ASTNode>& node : mElseBranch) {
            if (node) {
                result += node->ToString();
            }
        }
        
        return result;
    }

    // FunctionCallNode implementation ===================================

    FunctionCallNode::FunctionCallNode(const std::string& name, std::vector<std::unique_ptr<ASTNode>> arguments)
        : mName(name), mArguments(std::move(arguments)) 
    {}

    std::string FunctionCallNode::ToString() const 
    {
        std::string result = "FunctionCallNode: " + mName + '\n';
        for (const std::unique_ptr<ASTNode>& arg : mArguments) {
            if (arg) {
                result += arg->ToString();
            }
        }
        
        return result;
    }

    // FunctionDeclarationNode implementation ===================================

    FunctionDeclarationNode::FunctionDeclarationNode(const std::string& name, std::vector<std::string> arguments)
        : mName(name), mArguments(std::move(arguments)) 
    {}

    std::string FunctionDeclarationNode::ToString() const 
    {
        std::string result = "FunctionDeclarationNode: " + mName + '\n';
        for (const std::string& arg : mArguments) {
            result += "Argument: " + arg + '\n';
        }
        
        return result;
    }

    // FunctionDefinitionNode implementation ===================================

    FunctionDefinitionNode::FunctionDefinitionNode(std::unique_ptr<FunctionDeclarationNode> declaration, std::vector<std::unique_ptr<ASTNode>> body)
        : mDeclaration(std::move(declaration)), mBody(std::move(body)) 
    {}

    std::string FunctionDefinitionNode::ToString() const 
    {
        std::string result = "FunctionDefinitionNode: \n";
        if (mDeclaration) {
            result += mDeclaration->ToString();
        }
        for (const std::unique_ptr<AST::ASTNode>& node : mBody) {
            if (node) {
                result += node->ToString();
            }
        }

        return result;
    }

} // namespace AST
} // namespace FrontEnd