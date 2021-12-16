#include <logging.h>
#include <logger_ref.h>

int main()
{
    using namespace logging;
    logger::get_instance().add_transport<cout_transporter>(logger::TRACE, true);
    logger& l = logger::get_instance();

    logger_ref logger("foo");
    logger.info("UwU");
}