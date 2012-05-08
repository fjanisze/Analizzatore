#include "log.hpp"

#ifndef LOGGER_HPP
#define LOGGER_HPP

namespace
{
	logging::logger< logging::file_log_policy > log_hinst(&logging::logger_hinst, "execution.log");
}

#endif


#ifdef LOGGING_LEVEL_1
#define LOG log_hinst.print
#define LOG_ERR log_hinst.print_error
#define LOG_WARN log_hinst.print_warning
#else
#define LOG(...) 
#define LOG_ERR(...)
#define LOG_WARN(...)
#endif
