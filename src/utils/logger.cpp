/**
 * @file logger.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2026-09-23
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#include "logger.hpp"
#include "definitions.hpp"
#include <iostream>

const Logger& Logger::GetLogger()
{
    static Logger logger;

    return logger;
}

void Logger::Log(LogLevel level, const std::string& message) const
{
#if !defined(DEBUG)
    if (level != LogLevel::eDEBUG) {
#endif
    std::cout << '[' 
              << LOG_LEVEL_TO_STRING.at(level) 
              << "]: "
              << message 
              << std::endl;
#if !defined(DEBUG)
    }
#endif
}