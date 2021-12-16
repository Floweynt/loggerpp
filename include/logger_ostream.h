//
// Created by FloweyTF on 12/16/2021.
//

#ifndef __LOGGING_LOGGER_OSTREAM_H__
#define __LOGGING_LOGGER_OSTREAM_H__
#include <ostream>
#include <streambuf>
#include <utility>
#include <logging.h>
#include <logger_ref.h>

namespace logging
{
    class logger_buf : public std::streambuf
    {
        logger::level level;
        logger_ref ref;
        std::string buf;
    public:
        inline explicit logger_buf(logger_ref ref, logger::level l) : std::streambuf(), ref(std::move(ref)), level(l) {}
        inline std::streamsize xsputn(const char* str, std::streamsize n) override
        {
            buf += std::string{str, static_cast<size_t>(n)};
            return n;
        }

        inline int sync() override
        {
            ref.log(level, buf);
            buf.clear();
        }
    };

    class logger_ostream : public std::ostream
    {
        logger_buf buf;
    public:
        inline logger_ostream(const std::string& name, logger::level level) : std::ostream(&buf), buf(logger_ref{name}, level) {}
    };
}

#endif //__LOGGING_LOGGER_OSTREAM_H__
