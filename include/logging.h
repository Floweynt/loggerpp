#ifndef __LOGGING_LOGGER_H__
#define __LOGGING_LOGGER_H__
#include <string>
#include <utility>
#include <vector>
#include <iostream>
#include <fstream>
#include <memory>
#include <stacktrace.h>

namespace logging
{
    class transporter
	{
	public:
		virtual void log(const std::string&) = 0;
		virtual ~transporter() = default;
	};

	class cout_transporter : public transporter
	{
	public:
        cout_transporter() = default;
		inline void log(const std::string& str) override { std::cout << str << std::flush; }
		~cout_transporter() override = default;
	};

	class fs_transporter : public transporter
	{
		std::ofstream file;
	public:
		inline explicit fs_transporter(const std::string& s) : file(std::ofstream{s}) { }
		inline void log(const std::string& str) override { file << str << std::flush; }
		~fs_transporter() override = default;
	};

    class logger
	{
	public:
		enum level
		{
			TRACE = 0,
			DEBUG,
			INFO,
			WARN,
			ERROR,
			FATAL
		};

		struct transport
		{
			std::shared_ptr<transporter> tp;
			level l;
			bool is_colored;

			inline transport(std::shared_ptr<transporter> a, level b, bool c) : tp(std::move(a)), l(b), is_colored(c) {};
		};

        logger& operator= (const logger&) = delete;
        logger& operator= (logger&&) = delete;

        static logger& get_instance()
        {
            if(!instance)
                instance = new logger;
            return *instance;
        }

	private:
		std::vector<transport> transports;
        static inline logger* instance = nullptr;
	protected:
		logger() = default;
	public:
		logger(const logger&) = delete;
		logger(logger&&) = default;

        inline logger& add_transport(std::shared_ptr<transporter> t, level l, bool c = false) { transports.emplace_back(t, l, c); return *this; }
		
		template<typename T, typename... Args>
		logger& add_transport(level l, bool c, Args&&... args) { return add_transport(std::make_shared<T>(std::forward<Args>(args)...), l, c); }

		template<typename T>
		logger& add_transport(level l, bool c) { return add_transport(std::make_shared<T>(), l, c); }

        // anon logging

        inline logger& log(level l, const std::string& buf) { return nlog(l, "ANON", buf); }
        inline logger& log(level l, const std::exception& e) { return nlog(l, "ANON", "Exception thrown with what() = ", e); }
        inline logger& log(level l, const stacktrace::stack_aware_exception& e) { return nlog(l, "ANON", "", e); }
        inline logger& log(level l, const std::string& name, const std::exception& e) { return nlog(l, "ANON",name + e.what()); }
        inline logger& log(level l, const std::string& str, const stacktrace::stack_aware_exception& e) { return nlog(l, "ANON", str, e); }

        inline logger& trace(const std::string& s) { return nlog(TRACE, "ANON", s); }
        inline logger& trace(const std::exception& e) { return nlog(TRACE, "ANON", e); }
        inline logger& trace(const stacktrace::stack_aware_exception& e) { return nlog(TRACE, "ANON", e); }
        inline logger& trace(const std::string& str, const std::exception& e) { return nlog(TRACE, "ANON", str, e); }
        inline logger& trace(const std::string& str, const stacktrace::stack_aware_exception& e) { return nlog(TRACE, "ANON", str, e); }

        inline logger& debug(const std::string& s) { return nlog(DEBUG, "ANON", s); }
        inline logger& debug(const std::exception& e) { return nlog(DEBUG, "ANON", e); }
        inline logger& debug(const stacktrace::stack_aware_exception& e) { return nlog(DEBUG, "ANON", e); }
        inline logger& debug(const std::string& str, const std::exception& e) { return nlog(DEBUG, "ANON", str, e); }
        inline logger& debug(const std::string& str, const stacktrace::stack_aware_exception& e) { return nlog(DEBUG, "ANON", str, e); }

        inline logger& info(const std::string& s) { return nlog(INFO, "ANON", s); }
        inline logger& info(const std::exception& e) { return nlog(INFO, "ANON", e); }
        inline logger& info(const stacktrace::stack_aware_exception& e) { return nlog(INFO, "ANON", e); }
        inline logger& info(const std::string& str, const std::exception& e) { return nlog(INFO, str, "ANON", e); }
        inline logger& info(const std::string& str, const stacktrace::stack_aware_exception& e) { return nlog(INFO, "ANON", str, e); }

        inline logger& warn(const std::string& s) { return nlog(WARN, "ANON", s); }
        inline logger& warn(const std::exception& e) { return nlog(WARN, "ANON", e); }
        inline logger& warn(const stacktrace::stack_aware_exception& e) { return nlog(WARN, "ANON", e); }
        inline logger& warn(const std::string& str, const std::exception& e) { return nlog(WARN, "ANON", str, e); }
        inline logger& warn(const std::string& str, const stacktrace::stack_aware_exception& e) { return nlog(WARN, "ANON", str, e); }

        inline logger& error(const std::string& s) { return nlog(ERROR, "ANON", s); }
        inline logger& error(const std::exception& e) { return nlog(ERROR, "ANON", e); }
        inline logger& error(const stacktrace::stack_aware_exception& e) { return nlog(ERROR, "ANON", e); }
        inline logger& error(const std::string& str, const std::exception& e) { return nlog(ERROR, "ANON", str, e); }
        inline logger& error(const std::string& str, const stacktrace::stack_aware_exception& e) { return nlog(ERROR, "ANON", str, e); }

        inline void fatal(const std::string& s, int o = -1) { nlog(FATAL, "ANON", s); std::exit(o); }
        inline void fatal(const std::exception& e, int o = -1) { nlog(FATAL, "ANON", e); std::exit(o); }
        inline void fatal(const stacktrace::stack_aware_exception& e, int o = -1) { nlog(FATAL, "ANON", e); std::exit(o); }
        inline void fatal(const std::string& str, const std::exception& e, int o = -1) { nlog(FATAL, "ANON", str, e); std::exit(o); }
        inline void fatal(const std::string& str, const stacktrace::stack_aware_exception& e, int o = -1) { nlog(FATAL, "ANON", str, e); std::exit(o); }

        // named logging

        logger& nlog(level l, const std::string& name, const std::string& buf);
        inline logger& nlog(level l, const std::string& name, const std::exception& e) { return nlog(l, name, "Exception thrown with what() = ", e); }
        inline logger& nlog(level l, const std::string& name, const stacktrace::stack_aware_exception& e) { return nlog(l, name, "", e); }
        inline logger& nlog(level l, const std::string& name, const std::string& str, const std::exception& e) { return nlog(l, name, str + e.what()); }
        logger& nlog(level l, const std::string& name, const std::string& str, const stacktrace::stack_aware_exception& e);

        inline logger& ntrace(const std::string& name, const std::string& s) { return nlog(TRACE, name, s); }
        inline logger& ntrace(const std::string& name, const std::exception& e) { return nlog(TRACE, name, e); }
        inline logger& ntrace(const std::string& name, const stacktrace::stack_aware_exception& e) { return log(TRACE, name, e); }
        inline logger& ntrace(const std::string& name, const std::string& str, const std::exception& e) { return nlog(TRACE, name, str, e); }
        inline logger& ntrace(const std::string& name, const std::string& str, const stacktrace::stack_aware_exception& e) { return nlog(TRACE, name, str, e); }

        inline logger& ndebug(const std::string& name, const std::string& s) { return nlog(DEBUG, name, s); }
        inline logger& ndebug(const std::string& name, const std::exception& e) { return nlog(DEBUG, name, e); }
        inline logger& ndebug(const std::string& name, const stacktrace::stack_aware_exception& e) { return nlog(DEBUG, name, e); }
        inline logger& ndebug(const std::string& name, const std::string& str, const std::exception& e) { return nlog(DEBUG, name, str, e); }
        inline logger& ndebug(const std::string& name, const std::string& str, const stacktrace::stack_aware_exception& e) { return nlog(DEBUG, name, str, e); }

        inline logger& ninfo(const std::string& name, const std::string& s) { return nlog(INFO, name, s); }
        inline logger& ninfo(const std::string& name, const std::exception& e) { return nlog(INFO, name, e); }
        inline logger& ninfo(const std::string& name, const stacktrace::stack_aware_exception& e) { return nlog(INFO, name, e); }
        inline logger& ninfo(const std::string& name, const std::string& str, const std::exception& e) { return nlog(INFO, name, str, e); }
        inline logger& ninfo(const std::string& name, const std::string& str, const stacktrace::stack_aware_exception& e) { return nlog(INFO, name, str, e); }

        inline logger& nwarn(const std::string& name, const std::string& s) { return nlog(WARN, name, s); }
        inline logger& nwarn(const std::string& name, const std::exception& e) { return nlog(WARN, name, e); }
        inline logger& nwarn(const std::string& name, const stacktrace::stack_aware_exception& e) { return nlog(WARN, name, e); }
        inline logger& nwarn(const std::string& name, const std::string& str, const std::exception& e) { return nlog(WARN, name, str, e); }
        inline logger& nwarn(const std::string& name, const std::string& str, const stacktrace::stack_aware_exception& e) { return nlog(WARN, name, str, e); }

        inline logger& nerror(const std::string& name, const std::string& s) { return nlog(ERROR, name, s); }
        inline logger& nerror(const std::string& name, const std::exception& e) { return nlog(ERROR, name, e); }
        inline logger& nerror(const std::string& name, const stacktrace::stack_aware_exception& e) { return nlog(ERROR, name, e); }
        inline logger& nerror(const std::string& name, const std::string& str, const std::exception& e) { return nlog(ERROR, name, str, e); }
        inline logger& nerror(const std::string& name, const std::string& str, const stacktrace::stack_aware_exception& e) { return nlog(ERROR, name, str, e); }

        inline void nfatal(const std::string& name, const std::string& s, int o = -1) { nlog(FATAL, name, s); std::exit(o); }
        inline void nfatal(const std::string& name, const std::exception& e, int o = -1) { nlog(FATAL, name, e); std::exit(o); }
        inline void nfatal(const std::string& name, const stacktrace::stack_aware_exception& e, int o = -1) { nlog(FATAL, name, e); std::exit(o); }
        inline void nfatal(const std::string& name, const std::string& str, const std::exception& e, int o = -1) { nlog(FATAL, name, str, e); std::exit(o); }
        inline void nfatal(const std::string& name, const std::string& str, const stacktrace::stack_aware_exception& e, int o = -1) { nlog(FATAL, name, str, e); std::exit(o); }
	};

    int str_to_level(const std::string& str);
    std::string level_to_str(logger::level l);
}

#endif
