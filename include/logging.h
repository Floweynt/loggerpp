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

        inline logger& nlog(level l, const std::string& name, const std::string& buf)
        {
            using namespace std::literals::string_literals;
            std::string out_color = "["s + name + "]: "s + buf + '\n';
            std::string out_nocolor = out_color;
            switch(l)
            {
                case logger::TRACE:
                    out_color = "[\x1b[90mTRACE\x1b[0m] " + out_color;
                    break;
                case logger::DEBUG:
                    out_color = "[\x1b[90mDEBUG\x1b[0m] " + out_color;
                    break;
                case logger::INFO:
                    out_color = "[\x1b[90mINFO\x1b[0m] " + out_color;
                    break;
                case logger::WARN:
                    out_color = "[\x1b[33mWARN\x1b[0m] " + out_color;
                    break;
                case logger::ERROR:
                    out_color = "[\x1b[31mERROR\x1b[0m] " + out_color;
                    break;
                case logger::FATAL:
                    out_color = "[\x1b[31mFATAL\x1b[0m] " + out_color;
                    break;
            }
            switch(l)
            {
                case logger::TRACE:
                    out_nocolor = "[TRACE]: " + out_nocolor;
                    break;
                case logger::DEBUG:
                    out_nocolor = "[DEBUG]: " + out_nocolor;
                    break;
                case logger::INFO:
                    out_nocolor = "[INFO]: " + out_nocolor;
                    break;
                case logger::WARN:
                    out_nocolor = "[WARN]: " + out_nocolor;
                    break;
                case logger::ERROR:
                    out_nocolor = "[ERROR]: " + out_nocolor;
                    break;
                case logger::FATAL:
                    out_nocolor = "[FATAL]: " + out_nocolor;
                    break;
            }

            for(auto& i : transports)
            {
                if(i.l <= l)
                {
                    if(i.is_colored)
                        i.tp->log(out_color);
                    else
                        i.tp->log(out_nocolor);
                }
            }
            return *this;
        }
        inline logger& nlog(level l, const std::string& name, const std::exception& e) { return nlog(l, name, "Exception thrown with what() = ", e); }
        inline logger& nlog(level l, const std::string& name, const stacktrace::stack_aware_exception& e) { return nlog(l, name, "", e); }
        inline logger& nlog(level l, const std::string& name, const std::string& str, const std::exception& e) { return nlog(l, name, str + e.what()); }
        inline logger& nlog(level l, const std::string& name, const std::string& str, const stacktrace::stack_aware_exception& e)
        {
            std::ostringstream ss;
            if(!str.empty())
                ss << str << '\n' << stacktrace::stacktrace << e;
            else
                ss << stacktrace::stacktrace << e;
            return nlog(l, name, ss.str());
        }

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

    inline int str_to_level(const std::string& str)
    {
		if(str.size() < 4 || str.size() > 5)
			return -1;
		
		if(str.size() == 4)
		{
			if(*((const uint32_t*) str.c_str()) == *((const uint32_t*)"INFO"))
				return logger::INFO;
			else if(*((const uint32_t*) str.c_str()) == *((const uint32_t*)"WARN"))
				return logger::WARN;
			return -1;
		}
		else if(str.size() == 5)
		{
			if(*((const uint32_t*) str.c_str()) == *((const uint32_t*)"DEBU"))
				return str.c_str()[4] == 'G' ? logger::DEBUG : -1;
			else if(*((const uint32_t*) str.c_str()) == *((const uint32_t*)"ERRO"))
				return str.c_str()[4] == 'R' ? logger::ERROR : -1;
			else if(*((const uint32_t*) str.c_str()) == *((const uint32_t*)"FATA"))
				return str.c_str()[4] == 'L' ? logger::FATAL : -1;
			return -1;
		}

		return false;
	}
	
    template<typename T>
    std::string level_to_str(logger::level l)
    {
        return "";
    }
}

#endif
