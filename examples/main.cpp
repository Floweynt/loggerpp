#include <logging.h>
#include <logger_ref.h>
#include <logger_ostream.h>
int main()
{
    using namespace logging;
    logger::get_instance().add_transport<cout_transporter>(logger::TRACE, true);
    logger &l = logger::get_instance();

    logger_ostream os{"COUT", logger::INFO};
    std::cout.rdbuf(os.rdbuf());




    std::cout << "Test" << "Test" << std::flush;

}