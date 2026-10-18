#ifndef LLVM_MANAGER_HPP
#define LLVM_MANAGER_HPP

#include "ast.hpp"

#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Module.h"

#include <string>
#include <unordered_map>

class LLVMManager
{
private:
    LLVMManager();
    ~LLVMManager() = default;

public:
    LLVMManager(const LLVMManager&) = delete;
    LLVMManager& operator=(const LLVMManager&) = delete;

public:
    static LLVMManager& GetLLVMManager();

    llvm::Value* CodeGeneration(const FrontEnd::AST::NumberNode& node);
    llvm::Value* CodeGeneration(const FrontEnd::AST::VariableNode& node);
    llvm::Value* CodeGeneration(const FrontEnd::AST::BinaryOpNode& node);
    llvm::Value* CodeGeneration(const FrontEnd::AST::IfStatementNode& node);
    llvm::Value* CodeGeneration(const FrontEnd::AST::FunctionCallNode& node);
    llvm::Value* CodeGeneration(const FrontEnd::AST::FunctionDeclarationNode& node);
    llvm::Value* CodeGeneration(const FrontEnd::AST::FunctionDefinitionNode& node);

private:
    std::unique_ptr<llvm::LLVMContext> mContext;
    std::unique_ptr<llvm::IRBuilder<>> mBuilder;
    std::unique_ptr<llvm::Module> mModule;

    std::unordered_map<std::string, llvm::Value*> mReferencedValues;

};

#endif // LLVM_MANAGER_HPP