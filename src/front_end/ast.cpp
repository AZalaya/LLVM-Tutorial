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

#include "ast.hpp"

#include "definitions.hpp"
#include "logger.hpp"
#include "llvm_manager.hpp"

#include <cstring>
#include <llvm/IR/Value.h>

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

    NumericType NumberNode::getNumericType() const
    {
        return mNumericType;
    }

    void NumberNode::Print() const 
    {
        Logger::GetLogger().Log(LogLevel::eDEBUG, "NumberNode: {} ({})", mValue, NUMERIC_TO_STRING.at(mNumericType));
    }

    llvm::Value* NumberNode::CodeGeneration() const 
    {
        return LLVMManager::GetLLVMManager().CodeGeneration(*this);
    }

    // IdentifierNode implementation ===================================

    IdentifierNode::IdentifierNode(const std::string& name)
        : mName(name) 
    {}

    void IdentifierNode::Print() const 
    {
        Logger::GetLogger().Log(LogLevel::eDEBUG, "IdentifierNode: {}", mName);
    }

    const std::string& IdentifierNode::getName() const
    {
        return mName;
    }

    llvm::Value* IdentifierNode::CodeGeneration() const 
    {
        return LLVMManager::GetLLVMManager().CodeGeneration(*this);
    }

    // BinaryOpNode implementation ===================================

    BinaryOpNode::BinaryOpNode(BinaryOperators op, std::unique_ptr<ASTNode> left, std::unique_ptr<ASTNode> right)
        : mOp(op), mLeft(std::move(left)), mRight(std::move(right)) 
    {}

    BinaryOperators BinaryOpNode::getOperator() const
    {
        return mOp;
    }

    const std::unique_ptr<ASTNode>& BinaryOpNode::getLHS() const
    {
        return mLeft;
    }

    const std::unique_ptr<ASTNode>& BinaryOpNode::getRHS() const
    {
        return mRight;
    }

    void BinaryOpNode::Print() const 
    {
        Logger::GetLogger().Log(LogLevel::eDEBUG, "BinaryOpNode: {}", BINARY_OPERATOR_TO_STRING.at(mOp));
        
        if (mLeft) {
            mLeft->Print();
        }
        if (mRight) {
            mRight->Print();
        }
    }

    llvm::Value* BinaryOpNode::CodeGeneration() const 
    {
        return LLVMManager::GetLLVMManager().CodeGeneration(*this);
    }

    // IfStatementNode implementation ===================================

    IfStatementNode::IfStatementNode(std::unique_ptr<ASTNode> condition, std::vector<std::unique_ptr<ASTNode>> then_branch, std::vector<std::unique_ptr<ASTNode>> else_branch)
        : mCondition(std::move(condition)), mThenBranch(std::move(then_branch)), mElseBranch(std::move(else_branch)) 
    {}

    void IfStatementNode::Print() const 
    {
        Logger::GetLogger().Log(LogLevel::eDEBUG, "IfStatementNode: ");

        if (mCondition) {
            Logger::GetLogger().Log(LogLevel::eDEBUG, "Condition: ");
            mCondition->Print();
        }
        
        Logger::GetLogger().Log(LogLevel::eDEBUG, "Then branch: ");
        for (const std::unique_ptr<ASTNode>& node : mThenBranch) {
            if (node) {
                node->Print();
            }
        }
        
        Logger::GetLogger().Log(LogLevel::eDEBUG, "Else branch: ");
        for (const std::unique_ptr<ASTNode>& node : mElseBranch) {
            if (node) {
                node->Print();
            }
        }
    }

    const std::unique_ptr<ASTNode>& IfStatementNode::getCondition() const 
    {
        return mCondition;
    }

    const std::vector<std::unique_ptr<ASTNode>>& IfStatementNode::getThenBranch() const 
    {
        return mThenBranch;
    }

    const std::vector<std::unique_ptr<ASTNode>>& IfStatementNode::getElseBranch() const 
    {
        return mElseBranch;
    }

    llvm::Value* IfStatementNode::CodeGeneration() const 
    {
        return LLVMManager::GetLLVMManager().CodeGeneration(*this);
    }

    // FunctionCallNode implementation ===================================

    FunctionCallNode::FunctionCallNode(const std::string& name, std::vector<std::unique_ptr<ASTNode>> arguments)
        : mName(name), mArguments(std::move(arguments)) 
    {}

    void FunctionCallNode::Print() const 
    {
        Logger::GetLogger().Log(LogLevel::eDEBUG, "FunctionCallNode: {}", mName);

        for (const std::unique_ptr<ASTNode>& argument : mArguments) {
            if (argument) {
                argument->Print();
            }
        }
    }

    const std::string& FunctionCallNode::getName() const 
    {
        return mName;
    }

    const std::vector<std::unique_ptr<ASTNode>>& FunctionCallNode::getArguments() const 
    {
        return mArguments;
    }  

    llvm::Value* FunctionCallNode::CodeGeneration() const 
    {
        return LLVMManager::GetLLVMManager().CodeGeneration(*this);
    }

    // FunctionDeclarationNode implementation ===================================

    FunctionDeclarationNode::FunctionDeclarationNode(const std::string& name, std::vector<std::string> arguments)
        : mName(name), mArguments(std::move(arguments)) 
    {}

    void FunctionDeclarationNode::Print() const 
    {
        Logger::GetLogger().Log(LogLevel::eDEBUG, "FunctionDeclarationNode: {}", mName);

        for (const std::string& argument : mArguments) {
            Logger::GetLogger().Log(LogLevel::eDEBUG, "Argument: : {}", argument);
        }
    }

    const std::string& FunctionDeclarationNode::getName() const 
    {
        return mName;
    }

    const std::vector<std::string>& FunctionDeclarationNode::getArguments() const 
    {
        return mArguments;
    }

    llvm::Value* FunctionDeclarationNode::CodeGeneration() const 
    {
        return LLVMManager::GetLLVMManager().CodeGeneration(*this);
    }

    // FunctionDefinitionNode implementation ===================================

    FunctionDefinitionNode::FunctionDefinitionNode(std::unique_ptr<FunctionDeclarationNode> declaration, std::vector<std::unique_ptr<ASTNode>> body)
        : mDeclaration(std::move(declaration)), mBody(std::move(body)) 
    {}

    void FunctionDefinitionNode::Print() const 
    {
        Logger::GetLogger().Log(LogLevel::eDEBUG, "FunctionDefinitionNode: ");

        if (mDeclaration) {
            mDeclaration->Print();
        }
        for (const std::unique_ptr<AST::ASTNode>& node : mBody) {
            if (node) {
                node->Print();
            }
        }
    }

    const std::unique_ptr<FunctionDeclarationNode>& FunctionDefinitionNode::getDeclaration() const 
    {
        return mDeclaration;
    }

    const std::vector<std::unique_ptr<ASTNode>>& FunctionDefinitionNode::getBody() const 
    {
        return mBody;
    }

    llvm::Value* FunctionDefinitionNode::CodeGeneration() const 
    {
        return LLVMManager::GetLLVMManager().CodeGeneration(*this);
    }

} // namespace AST
} // namespace FrontEnd