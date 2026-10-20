/**
 * @file main.cpp
 * @author Andoni Zalaya (andoni.zalaya.c@gmail.com)
 * @brief Main entry point for the CKal compiler application.
 * @version 0.1
 * @date 2026-09-13
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#include "definitions.hpp"
#include "logger.hpp"
#include "lexer.hpp"
#include "parser.hpp"

#include <llvm/IR/Value.h>
#include <llvm/Support/raw_ostream.h>

#include <fstream>

int main(int argc, char** argv) 
{
    if (argc < 3) {
        Logger::GetLogger().Log(LogLevel::eERROR, "Usage: " + std::string(argv[0]) + " <source_file> <output_file>");
        return 1;
    }

    std::string source_file = argv[1];
    std::ifstream file(source_file);

    if (!file.is_open()) {
        Logger::GetLogger().Log(LogLevel::eERROR, "Error: Could not open the source file");
        return 1;
    }

    std::string source_code((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();

    // Tokenize the source code
    std::vector<FrontEnd::Token> tokens = FrontEnd::Lexer::Lexer::Tokenize(source_code);

#if defined(DEBUG)
    // Print the tokens
    for (const FrontEnd::Token& token : tokens) {
        Logger::GetLogger().Log(LogLevel::eDEBUG, token.ToString());
    }
#endif

    std::vector<std::unique_ptr<FrontEnd::AST::ASTNode>> ast_nodes = 
        FrontEnd::Parser(tokens).Parse();

#if defined(DEBUG)
    // Print the AST nodes
    for (const std::unique_ptr<FrontEnd::AST::ASTNode>& node : ast_nodes) {
        Logger::GetLogger().Log(LogLevel::eDEBUG, node->ToString());
    }
#endif

    // Store the resulting IR to the output file
    std::error_code err;
    std::string output_file = argv[2];
    llvm::raw_fd_ostream llvm_ostream(output_file, err);

    if (err) {
        Logger::GetLogger().Log(LogLevel::eERROR, "An error occurred trying to open the output file");
        return 1;
    }

    for (const std::unique_ptr<FrontEnd::AST::ASTNode>& node : ast_nodes) {
        node->CodeGeneration()->print(llvm_ostream);
    }

    llvm_ostream.close();

    return 0;
}