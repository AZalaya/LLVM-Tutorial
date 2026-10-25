/**
 * @file llvm_manager.cpp
 * @author Andoni Zalaya (andoni.zalaya.c@gmail.com)
 * @brief Implementation file for the LLVM Manager class, responsible for for creating the LLVM IR code from the parsed AST Nodes.
 * @version 0.1
 * @date 2026-10-20
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#include "llvm_manager.hpp"

#include "logger.hpp"

#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Argument.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Value.h>
#include <llvm/IR/Verifier.h>

#include <llvm/Transforms/InstCombine/InstCombine.h>
#include <llvm/Transforms/Scalar/Reassociate.h>
#include "llvm/Transforms/Scalar/GVN.h"
#include <llvm/Transforms/Scalar/SimplifyCFG.h>

#include <string>
#include <vector>

LLVMManager::LLVMManager()
    : mContext(std::make_unique<llvm::LLVMContext>())
    , mBuilder(std::make_unique<llvm::IRBuilder<>>(*mContext))
    , mModule(std::make_unique<llvm::Module>("CKal_Module", *mContext))
    , mFunctionPassManager(std::make_unique<llvm::FunctionPassManager>())
    , mLoopAnalysisManager(std::make_unique<llvm::LoopAnalysisManager>())
    , mFunctionAnalysisManager(std::make_unique<llvm::FunctionAnalysisManager>())
    , mCGSCCAnalysisManager(std::make_unique<llvm::CGSCCAnalysisManager>())
    , mModuleAnalysisManager(std::make_unique<llvm::ModuleAnalysisManager>())
    , mPassInstrumentationCallbacks(std::make_unique<llvm::PassInstrumentationCallbacks>())
    , mStandardInstrumentations(std::make_unique<llvm::StandardInstrumentations>(*mContext, true))
    , mPassBuilder(std::make_unique<llvm::PassBuilder>())
{
    mStandardInstrumentations->registerCallbacks(*mPassInstrumentationCallbacks, mModuleAnalysisManager.get());

    // Standard cleanup optimizations
    mFunctionPassManager->addPass(llvm::InstCombinePass());
    mFunctionPassManager->addPass(llvm::ReassociatePass());
    mFunctionPassManager->addPass(llvm::GVNPass());
    mFunctionPassManager->addPass(llvm::SimplifyCFGPass());

    mPassBuilder->registerModuleAnalyses(*mModuleAnalysisManager);
    mPassBuilder->registerFunctionAnalyses(*mFunctionAnalysisManager);
    mPassBuilder->crossRegisterProxies(
        *mLoopAnalysisManager, 
        *mFunctionAnalysisManager, 
        *mCGSCCAnalysisManager, 
        *mModuleAnalysisManager
    );
}


LLVMManager& LLVMManager::GetLLVMManager()
{
    static LLVMManager llvm_manager;

    return llvm_manager;
}

llvm::Value* LLVMManager::CodeGeneration(const FrontEnd::AST::NumberNode& node)
{
    switch(node.getNumericType())
    {
    case FrontEnd::NumericType::eINT8:
        return llvm::ConstantInt::get(*mContext, llvm::APInt(8, node.getValue<char>(), true));

    case FrontEnd::NumericType::eINT16:
        return llvm::ConstantInt::get(*mContext, llvm::APInt(16, node.getValue<short>(), true));

    case FrontEnd::NumericType::eINT32:
        return llvm::ConstantInt::get(*mContext, llvm::APInt(32, node.getValue<int>(), true));

    case FrontEnd::NumericType::eINT64:
        return llvm::ConstantInt::get(*mContext, llvm::APInt(64, node.getValue<long>(), true));

    case FrontEnd::NumericType::eUINT8:
        return llvm::ConstantInt::get(*mContext, llvm::APInt(8, node.getValue<unsigned char>(), false));

    case FrontEnd::NumericType::eUINT16:
        return llvm::ConstantInt::get(*mContext, llvm::APInt(16, node.getValue<unsigned short>(), false));

    case FrontEnd::NumericType::eUINT32:
        return llvm::ConstantInt::get(*mContext, llvm::APInt(32, node.getValue<unsigned int>(), false));

    case FrontEnd::NumericType::eUINT64:
        return llvm::ConstantInt::get(*mContext, llvm::APInt(64, node.getValue<unsigned long>(), false));

    case FrontEnd::NumericType::eFLOAT32:
        return llvm::ConstantFP::get(*mContext, llvm::APFloat(node.getValue<float>()));

    case FrontEnd::NumericType::eFLOAT64:
        return llvm::ConstantFP::get(*mContext, llvm::APFloat(node.getValue<double>()));

    default:
        break;

    }

    Logger::GetLogger().Log(LogLevel::eERROR, "Unknown numeric type for code generation");
    
    return nullptr;
}

llvm::Value* LLVMManager::CodeGeneration(const FrontEnd::AST::IdentifierNode& node)
{
    std::unordered_map<std::string, llvm::Value*>::const_iterator it =
        mReferencedValues.find(node.getName());

    if (it == mReferencedValues.end()) {
        Logger::GetLogger().Log(LogLevel::eERROR, "Reference to undefined variable: {}", node.getName());
        return nullptr;
    }
    
    return it->second;
}

llvm::Value* LLVMManager::CodeGeneration(const FrontEnd::AST::BinaryOpNode& node)
{
    llvm::Value* lhs = node.getLHS()->CodeGeneration();
    llvm::Value* rhs = node.getRHS()->CodeGeneration();

    if (lhs == nullptr || rhs == nullptr) {
        Logger::GetLogger().Log(LogLevel::eERROR, "Failed to generate code for binary operation operands");
        return nullptr;
    }

    switch (node.getOperator()) 
    {
    case FrontEnd::BinaryOperators::eLESS_THAN:
        return mBuilder->CreateICmpSLT(lhs, rhs, "cmp_tmp");

    case FrontEnd::BinaryOperators::eSUM:
        return mBuilder->CreateAdd(lhs, rhs, "add_tmp");

    case FrontEnd::BinaryOperators::eMINUS:
        return mBuilder->CreateSub(lhs, rhs, "sub_tmp");

    case FrontEnd::BinaryOperators::eMULTIPLY:
        return mBuilder->CreateMul(lhs, rhs, "mul_tmp");

    default:
        break;

    }

    Logger::GetLogger().Log(LogLevel::eERROR, "Unknown binary operator for code generation");
    return nullptr;
}

llvm::Value* LLVMManager::CodeGeneration(const FrontEnd::AST::IfStatementNode& node)
{
    llvm::Value* condition = node.getCondition()->CodeGeneration();

    if (condition == nullptr) {
        Logger::GetLogger().Log(LogLevel::eERROR, "Failed to generate code for if statement condition");
        return nullptr;
    }

    llvm::Function* current_function = mBuilder->GetInsertBlock()->getParent();

    const std::vector<std::unique_ptr<FrontEnd::AST::ASTNode>>& then_statements = node.getThenBranch();
    const std::vector<std::unique_ptr<FrontEnd::AST::ASTNode>>& else_statements = node.getElseBranch();

    llvm::BasicBlock* then_block = llvm::BasicBlock::Create(*mContext, "then");
    llvm::BasicBlock* else_block = llvm::BasicBlock::Create(*mContext, "else");
    llvm::BasicBlock* end_block = llvm::BasicBlock::Create(*mContext, "if_cont");
 
    mBuilder->CreateCondBr(condition, then_block, else_block); 

    std::vector<llvm::Value*> then_values;
    std::vector<llvm::Value*> else_values;

    current_function->insert(current_function->end(), then_block);
    mBuilder->SetInsertPoint(then_block);

    for (const std::unique_ptr<FrontEnd::AST::ASTNode>& statement : then_statements) {
        llvm::Value* statement_value = statement->CodeGeneration();

        if (statement_value == nullptr) {
            Logger::GetLogger().Log(LogLevel::eERROR, "Failed to generate code for then branch statement");
            return nullptr;
        }

        then_values.emplace_back(statement_value);
    }

    mBuilder->CreateBr(end_block);

    then_block = mBuilder->GetInsertBlock();

    current_function->insert(current_function->end(), else_block);
    mBuilder->SetInsertPoint(else_block);

    for (const std::unique_ptr<FrontEnd::AST::ASTNode>& statement : else_statements) {
        llvm::Value* statement_value = statement->CodeGeneration();

        if (statement_value == nullptr) {
            Logger::GetLogger().Log(LogLevel::eERROR, "Failed to generate code for else branch statement");
            return nullptr;
        }
        
        else_values.emplace_back(statement_value);
    }

    mBuilder->CreateBr(end_block);

    else_block = mBuilder->GetInsertBlock();

    current_function->insert(current_function->end(), end_block);
    mBuilder->SetInsertPoint(end_block);

    llvm::PHINode* phi_node = mBuilder->CreatePHI(llvm::Type::getInt32Ty(*mContext), then_values.size() + else_values.size(), "if_result");

    phi_node->addIncoming(then_values.back(), then_block);
    phi_node->addIncoming(else_values.back(), else_block);

    return phi_node;
}

llvm::Value* LLVMManager::CodeGeneration(const FrontEnd::AST::FunctionCallNode& node)
{
    llvm::Function* function = mModule->getFunction(node.getName());

    if (function == nullptr) {
        Logger::GetLogger().Log(LogLevel::eERROR, "Call to undefined function: {}", node.getName());
        return nullptr;
    }

    const std::vector<std::unique_ptr<FrontEnd::AST::ASTNode>>& arguments = node.getArguments();

    if (function->arg_size() != arguments.size()) {
        Logger::GetLogger().Log(LogLevel::eERROR, "Function call argument count mismatch for function: {}", node.getName());
        return nullptr;
    }

    std::vector<llvm::Value*> llvm_arguments;

    for (const std::unique_ptr<FrontEnd::AST::ASTNode>& arg : arguments) {
        llvm::Value* llvmArg = arg->CodeGeneration();

        if (llvmArg == nullptr) {
            Logger::GetLogger().Log(LogLevel::eERROR, "Failed to generate code for function argument");
            return nullptr;
        }

        llvm_arguments.emplace_back(llvmArg);
    }

    return mBuilder->CreateCall(function, llvm_arguments, "call_tmp");
}

llvm::Value* LLVMManager::CodeGeneration(const FrontEnd::AST::FunctionDeclarationNode& node)
{
    const std::string& function_name = node.getName();
    const std::vector<std::string>& function_arguments = node.getArguments();

    std::vector<llvm::Type*> llvm_arguments(function_arguments.size(), llvm::Type::getInt32Ty(*mContext));

    llvm::FunctionType* function_type = llvm::FunctionType::get(llvm::Type::getInt32Ty(*mContext), llvm_arguments, false);

    llvm::Function* function = llvm::Function::Create(function_type, llvm::Function::ExternalLinkage, function_name, mModule.get());

    for (llvm::Argument& argument : function->args()) {
        argument.setName(function_arguments[argument.getArgNo()]);
    }

    return function;
}

llvm::Value* LLVMManager::CodeGeneration(const FrontEnd::AST::FunctionDefinitionNode& node)
{
    const std::string& function_name = node.getDeclaration()->getName();
    const std::unique_ptr<FrontEnd::AST::FunctionDeclarationNode>& function_declaration = node.getDeclaration();

    llvm::Function* function = mModule->getFunction(function_name);

    if (function == nullptr) {
        function = static_cast<llvm::Function*>(CodeGeneration(*function_declaration));
    }

    if (function == nullptr) {
        Logger::GetLogger().Log(LogLevel::eERROR, "Function definition for undefined function: {}", function_name);
        return nullptr;
    }

    if (!function->empty()) {
        Logger::GetLogger().Log(LogLevel::eERROR, "Function redefinition: {}", function_name);
        return nullptr;
    }

    llvm::BasicBlock* entry_block = llvm::BasicBlock::Create(*mContext, "entry_point", function);

    mBuilder->SetInsertPoint(entry_block);

    mReferencedValues.clear();
    for (llvm::Argument& argument : function->args()) {
        mReferencedValues[argument.getName().str()] = &argument;
    }

    std::vector<llvm::Value*> statements;
    for (const std::unique_ptr<FrontEnd::AST::ASTNode>& statement : node.getBody()) {
        llvm::Value* statement_value = statement->CodeGeneration();

        if (statement_value == nullptr) {
            Logger::GetLogger().Log(LogLevel::eERROR, "Failed to generate code for function body statement");
            function->eraseFromParent();
            return nullptr;
        }

        statements.emplace_back(statement_value);
    }

    mBuilder->CreateRet(statements.back());

    if (llvm::verifyFunction(*function, &llvm::errs())) {
        Logger::GetLogger().Log(LogLevel::eERROR, "Function verification failed: {}", function_name);
        function->eraseFromParent();
        return nullptr;
    }

    mFunctionPassManager->run(*function, *mFunctionAnalysisManager);

    return function;
}
