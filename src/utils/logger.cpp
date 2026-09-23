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
#include <iostream>

Logger& Logger::GetLogger()
{
    static Logger logger;

    return logger;
}

void Logger::Log(LogLevel level, const std::string& message)
{
    switch (level) 
    {
    case LogLevel::eINFO:
        std::cout << "[INFO] ";
        break;

    case LogLevel::eERROR:
        std::cout << "[ERROR] ";
        break;

    default:
        std::cout << "[UNKNOWN] ";
    }

    std::cout << message << std::endl;
}