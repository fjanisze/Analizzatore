#include "configurations.hpp"
#include ".\memory\memory.hpp"
#include ".\logging\logger.hpp"
#include "types.hpp"
#include <utility>
#include <fstream>
#include <memory>
#include <list>
#include <vector>
#include <windows.h>

#ifndef FILE_IO_HPP
#define FILE_IO_HPP


namespace file_io
{
	using namespace types;
	
	typedef long file_handler;
	typedef std::shared_ptr< std::fstream > ptr_stream;

	struct log_stats
	{
		short max_lenght,
		      min_lenght;
		unsigned logline_available,
			 logline_ignored;
		log_stats() : max_lenght(0), 
		              min_lenght( std::numeric_limits<short>::max() ),
			      logline_available(0),
			      logline_ignored(0) {}
	};

	typedef std::vector< string > logline_container;

	class syslog_file
	{
		log_stats	      logline_stats;
		std::vector< string > syslog_files;
		logline_container     log_lines;

		ptr_stream	open(string_cref filename);
		bool            close(ptr_stream&& stream);

		void		calibrate_logline_container(unsigned threshold);
	
		int	     find_syslog_files();
		long	     read_log(string_cref logname);
		char*	     normalize_syslog(char* line);
	public:
		syslog_file();
		const std::vector< string >& get_logfile_names();
		unsigned 	calculate_memory_consumption();
		long		read_all_log();
		logline_container& get_loglines();
	};


}

#endif
