#include "log_interpreter.hpp"

namespace log_interpreter
{
	void interpreter::initialize()
	{
		syslog_io.read_all_log();
	}

	/*
	 * Parse logs
	 */
	void interpreter::parse()
	{
		auto logs = syslog_io.get_loglines();
		if(logs.empty())
		{
			LOG("interpreter::parse(): No logs to parse, quitting");
		}

		logline_part parsed_line;
		for( auto line : logs )
		{
			parsed_line.source = line.substr( 0 , 8 );
			parsed_line.timestamp = line.substr( 10 , 21 );
			parsed_line.source_subsystem = line.substr( 39, 5 ); 	
			loglines.push_back( parsed_line );
		}
	}

	void interpreter::debug_print_loglines()
	{
		for(auto it : syslog_io.get_loglines() )
		{
			std::cout<<it<<std::endl;
		}
	}

	void interpreter::debug_print_logline_part()
	{
		for(auto it : loglines )
		{
			std::cout<<"SOURCE:"<<it.source<<", TIMESTAMP:"<<it.timestamp<<", SUBSYSTEM:"<<it.source_subsystem<<std::endl;
		}
	}
}

int main()
{
	log_interpreter::interpreter log;
	log.initialize();
	log.parse();
	log.debug_print_logline_part();	
	system("pause");
}
