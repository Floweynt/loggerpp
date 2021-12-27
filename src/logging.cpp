#include <logging.h>

namespace logging
{
    inline logger& logger::nlog(logger::level l, const std::string& name, const std::string& buf)
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

    logger& logger::nlog(logger::level l, const std::string& name, const std::string& str, const stacktrace::stacktrace_exception& e)
    {
        std::ostringstream ss;
        if(!str.empty())
            ss << str << '\n' << stacktrace::stacktrace << e;
        else
            ss << stacktrace::stacktrace << e;
        return nlog(l, name, ss.str());
    }

    int str_to_level(const std::string& str)
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

    std::string level_to_str(logger::level l)
    {
        switch (l) {
            case logger::TRACE:
                return "TRACE";
            case logger::DEBUG:
                return "DEBUG";
            case logger::INFO:
                return "INFO";
            case logger::WARN:
                return "WARN";
            case logger::ERROR:
                return "ERROR";
            case logger::FATAL:
                return "FATAL";
        };
    }
}