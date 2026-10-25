/**
 * @file logger.inl
 * @author Andoni Zalaya (andoni.zalaya.c@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2026-10-21
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#include <format>
#include <iostream>

template <typename... Args>
void Logger::Log(LogLevel level, const std::string& format_string, Args... args) const
{
#if !defined(DEBUG)
    if (level != LogLevel::eDEBUG) {
#endif
    std::cout << '[' 
              << LOG_LEVEL_TO_STRING.at(level) 
              << "]: "
              << std::vformat(format_string, std::make_format_args(args...)) 
              << std::endl;
#if !defined(DEBUG)
    }
#endif
}