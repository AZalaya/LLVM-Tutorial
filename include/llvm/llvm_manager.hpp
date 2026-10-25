/**
 * @file llvm_manager.hpp
 * @author Andoni Zalaya (andoni.zalaya.c@gmail.com)
 * @brief Header file for the LLVM Manager class, responsible for for creating the LLVM IR code from the parsed AST Nodes.
 * @version 0.1
 * @date 2026-10-20
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#ifndef LLVM_MANAGER_HPP
#define LLVM_MANAGER_HPP

#include "ast.hpp"

#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Module.h>

#include <llvm/Passes/PassBuilder.h>
#include <llvm/IR/PassManager.h>
#include <llvm/Analysis/LoopAnalysisManager.h>
#include <llvm/Analysis/CGSCCPassManager.h>
#include <llvm/IR/PassInstrumentation.h>
#include <llvm/Passes/StandardInstrumentations.h>

#include <memory>
#include <string>
#include <unordered_map>

class LLVMManager
{
private:
    /**
     * @brief Construct a new LLVMManager object.
     * 
     */
    LLVMManager();

    /**
     * @brief Destroy the LLVMManager object.
     * 
     */
    ~LLVMManager() = default;

public:
    /**
     * @brief Avoid copying the LLVM Manager from the copy constructor.
     * 
     */
    LLVMManager(const LLVMManager&) = delete;

    /**
     * @brief Avoid copying the LLVM Manager from the assignment operator.
     * 
     * @return LLVMManager& 
     */
    LLVMManager& operator=(const LLVMManager&) = delete;

public:
    /**
     * @brief Get the LLVM Manager instance.
     * 
     * @return LLVMManager& Singleton instance of LLVM Manager.
     */
    static LLVMManager& GetLLVMManager();

    /**
     * @brief Generate the LLVM IR code representation of the given number node.
     * 
     * @param node A number node form the AST.
     * @return llvm::Value* LLVM IR value.
     */
    llvm::Value* CodeGeneration(const FrontEnd::AST::NumberNode& node);

    /**
     * @brief Generate the LLVM IR code representation of the given identifier node.
     * 
     * @param node An identifier node form the AST.
     * @return llvm::Value* LLVM IR value.
     */
    llvm::Value* CodeGeneration(const FrontEnd::AST::IdentifierNode& node);

    /**
     * @brief Generate the LLVM IR code representation of the given binary operation node.
     * 
     * @param node A binary operation node form the AST.
     * @return llvm::Value* LLVM IR value.
     */
    llvm::Value* CodeGeneration(const FrontEnd::AST::BinaryOpNode& node);

    /**
     * @brief Generate the LLVM IR code representation of the given if statement node.
     * 
     * @param node A if statement node form the AST.
     * @return llvm::Value* LLVM IR value.
     */
    llvm::Value* CodeGeneration(const FrontEnd::AST::IfStatementNode& node);

    /**
     * @brief Generate the LLVM IR code representation of the given function call node.
     * 
     * @param node A function call node form the AST.
     * @return llvm::Value* LLVM IR value.
     */
    llvm::Value* CodeGeneration(const FrontEnd::AST::FunctionCallNode& node);

    /**
     * @brief Generate the LLVM IR code representation of the given function declaration node.
     * 
     * @param node A function declaration node form the AST.
     * @return llvm::Value* LLVM IR value.
     */
    llvm::Value* CodeGeneration(const FrontEnd::AST::FunctionDeclarationNode& node);

    /**
     * @brief Generate the LLVM IR code representation of the given function definition node.
     * 
     * @param node A function definition node form the AST.
     * @return llvm::Value* LLVM IR value.
     */
    llvm::Value* CodeGeneration(const FrontEnd::AST::FunctionDefinitionNode& node);

private:
    std::unique_ptr<llvm::LLVMContext> mContext;
    std::unique_ptr<llvm::IRBuilder<>> mBuilder;
    std::unique_ptr<llvm::Module> mModule;

    std::unique_ptr<llvm::FunctionPassManager> mFunctionPassManager;
    std::unique_ptr<llvm::LoopAnalysisManager> mLoopAnalysisManager;
    std::unique_ptr<llvm::FunctionAnalysisManager> mFunctionAnalysisManager;
    std::unique_ptr<llvm::CGSCCAnalysisManager> mCGSCCAnalysisManager;
    std::unique_ptr<llvm::ModuleAnalysisManager> mModuleAnalysisManager;
    std::unique_ptr<llvm::PassInstrumentationCallbacks> mPassInstrumentationCallbacks;
    std::unique_ptr<llvm::StandardInstrumentations> mStandardInstrumentations;
    std::unique_ptr<llvm::PassBuilder> mPassBuilder;

    std::unordered_map<std::string, llvm::Value*> mReferencedValues;

};

#endif // LLVM_MANAGER_HPP