#include "configurations.hpp"
#include "file_io.hpp"
#include "./logging/logger.hpp"
#include "types.hpp"

namespace log_interpreter
{
	using namespace types;

	struct logline_part
	{
		string source,
		       timestamp,
		       source_subsystem;
	};

	class interpreter
	{
		file_io::syslog_file        syslog_io;
		std::vector< logline_part > loglines;
	public:
		void initialize();
		void parse();
	public: //The following functions shall be removed
		void debug_print_loglines();
		void debug_print_logline_part();
	};
}
