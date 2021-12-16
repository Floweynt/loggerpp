#ifndef __LOGGING_LOGGER_REF_H__
#define  __LOGGING_LOGGER_REF_H__
#include <logging.h>

#include <utility>

namespace logging
{
    class logger_ref
    {
        std::string name = "ANON";
    public:
        logger_ref() = default;
        logger_ref(const logger_ref&) = default;
        logger_ref(logger_ref&&) = default;
        inline explicit logger_ref(std::string name) : name(std::move(name)) {}

        inline logger& log(logger::level l, const std::string& buf) { return logger::get_instance().nlog(l, name, buf); }
        inline logger& log(logger::level l, const std::exception& e) { return logger::get_instance().nlog(l, name, e); }
        inline logger& log(logger::level l, const stacktrace::stack_aware_exception& e) { return logger::get_instance().nlog(l, name, e); }
        inline logger& log(logger::level l, const std::string& str, const std::exception& e) { return logger::get_instance().nlog(l, name, str, e); }
        inline logger& log(logger::level l, const std::string& str, const stacktrace::stack_aware_exception& e) { return logger::get_instance().nlog(l, name, str, e); }

        inline logger& trace(const std::string& s) { return log(logger::TRACE, s); }
        inline logger& trace(const std::exception& e) { return log(logger::TRACE, e); }
        inline logger& trace(const stacktrace::stack_aware_exception& e) { return log(logger::TRACE, e); }
        inline logger& trace(const std::string& str, const std::exception& e) { return log(logger::TRACE, str, e); }
        inline logger& trace(const std::string& str, const stacktrace::stack_aware_exception& e) { return log(logger::TRACE, str, e); }

        inline logger& debug(const std::string& s) { return log(logger::DEBUG, s); }
        inline logger& debug(const std::exception& e) { return log(logger::DEBUG, e); }
        inline logger& debug(const stacktrace::stack_aware_exception& e) { return log(logger::DEBUG, e); }
        inline logger& debug(const std::string& str, const std::exception& e) { return log(logger::DEBUG, str, e); }
        inline logger& debug(const std::string& str, const stacktrace::stack_aware_exception& e) { return log(logger::DEBUG, str, e); }

        inline logger& info(const std::string& s) { return log(logger::INFO, s); }
        inline logger& info(const std::exception& e) { return log(logger::INFO, e); }
        inline logger& info(const stacktrace::stack_aware_exception& e) { return log(logger::INFO, e); }
        inline logger& info(const std::string& str, const std::exception& e) { return log(logger::INFO, str, e); }
        inline logger& info(const std::string& str, const stacktrace::stack_aware_exception& e) { return log(logger::INFO, str, e); }

        inline logger& warn(const std::string& s) { return log(logger::WARN, s); }
        inline logger& warn(const std::exception& e) { return log(logger::WARN, e); }
        inline logger& warn(const stacktrace::stack_aware_exception& e) { return log(logger::WARN, e); }
        inline logger& warn(const std::string& str, const std::exception& e) { return log(logger::WARN, str, e); }
        inline logger& warn(const std::string& str, const stacktrace::stack_aware_exception& e) { return log(logger::WARN, str, e); }

        inline logger& error(const std::string& s) { return log(logger::ERROR, s); }
        inline logger& error(const std::exception& e) { return log(logger::ERROR, e); }
        inline logger& error(const stacktrace::stack_aware_exception& e) { return log(logger::ERROR, e); }
        inline logger& error(const std::string& str, const std::exception& e) { return log(logger::ERROR, str, e); }
        inline logger& error(const std::string& str, const stacktrace::stack_aware_exception& e) { return log(logger::ERROR, str, e); }

        inline void fatal(const std::string& s, int o = -1) { log(logger::FATAL, s); std::exit(o); }
        inline void fatal(const std::exception& e, int o = -1) { log(logger::FATAL, e); std::exit(o); }
        inline void fatal(const stacktrace::stack_aware_exception& e, int o = -1) { log(logger::FATAL, e); std::exit(o); }
        inline void fatal(const std::string& str, const std::exception& e, int o = -1) { log(logger::FATAL, str, e); std::exit(o); }
        inline void fatal(const std::string& str, const stacktrace::stack_aware_exception& e, int o = -1) { log(logger::FATAL, str, e); std::exit(o); }
    };
}

#endif //__LOGGING_LOGGER_REF_H__
