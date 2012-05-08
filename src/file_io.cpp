#include "file_io.hpp"

namespace file_io
{

	syslog_file::syslog_file()
	{
		if( log_lines.capacity() < INITIAL_LINE_COUNTER )
		{
			LOG("syslog_file::syslog_file(): Increasing 'vector' capacity to ", INITIAL_LINE_COUNTER, " from ", log_lines.capacity());
			log_lines.reserve(INITIAL_LINE_COUNTER);
		}
	}

	ptr_stream syslog_file::open(string_cref filename)
	{
		LOG("syslog_file::open(): Opening file \"",filename.c_str(),"\"...");	
		ptr_stream r_handler( new std::fstream );
		r_handler->open(filename.c_str(), std::ios_base::in | std::ios_base::binary );
		if( r_handler->is_open() )
		{
			LOG("syslog_file::open(): OK!");	
		}
		else
		{
			LOG_ERR("syslog_file::open(): Unable to open!!");
		}
		return std::move( r_handler );
	}

	bool syslog_file::close(ptr_stream&& stream)
	{
		return ( stream->close() , ! stream->is_open() );
	}

	int syslog_file::find_syslog_files()
	{
		LOG("syslog_file::find_syslog_files(): Seeking for syslog files!");
		//Use windows specific code
		WIN32_FIND_DATA fileData;
		HANDLE hFind = INVALID_HANDLE_VALUE;
		
		hFind = FindFirstFile( (LPCSTR)SYSLOG_FILE_FORMAT, &fileData );
		if( hFind != INVALID_HANDLE_VALUE )
		{
			do{
				syslog_files.push_back( (char*)fileData.cFileName );
			}while( FindNextFile( hFind,&fileData) );
			FindClose(hFind);
		}
		LOG("syslog_file::find_syslog_files(): I found ", syslog_files.size(), " syslog files!");	
		return syslog_files.size();
	}

	void syslog_file::calibrate_logline_container(unsigned threshold)
	{
		if( log_lines.capacity() < threshold )
		{
			LOG("syslog_file::calibrate_logline_container(): Increasing threshold to ", threshold, " from ", log_lines.capacity());
			log_lines.reserve(threshold);
		}
	}

	/*
	 * Make snapshots and syslogs compatible for the next parsing phase
	 */

	inline
	char* syslog_file::normalize_syslog(char* line)
	{
		char* return_ptr = strchr( line + SYSLOG_HEADER_SIZE ,']');
		if( return_ptr == nullptr ) //not found
			return line;
		return return_ptr + 6; 
	}

	/*
	 * Read and store the information from a log file
	 *
	 */
	long syslog_file::read_log(string_cref logname)
	{
		LOG("syslog_file::read_log(): Reading ",logname.c_str());
		long amount_of_lines = 0, ignored_line = 0;
		ptr_stream stream = open(logname);
		if(stream->is_open())
		{
			LOG("syslog_file::read_log(): The log is properly opened and ready to be readed!");
			//Read the log
			char* line_buffer, *line;
			try{
				line_buffer = new char[MAX_LOG_LINE_SIZE];
			}catch(std::bad_alloc& xa)
			{
				LOG_ERR("syslog_file::read_log(): Allocation failure! ");
				stream->close();
				return -1;
			}
		
			short line_lenght = 0;
			do{
				stream->getline(line_buffer, MAX_LOG_LINE_SIZE);
				line = normalize_syslog(line_buffer);
				
				if( ( line_lenght = strlen(line) ) <= MIN_LOG_LINE_SIZE ) //This cannot be a valid line!
				{
					++ignored_line;
				}
				else
				{
					log_lines.push_back(line);
					++amount_of_lines;
				}
				//Collect statistics
				if( FETCH_STATISTICS )
				{
					logline_stats.max_lenght = std::max(logline_stats.max_lenght, line_lenght);
					logline_stats.min_lenght = std::min(logline_stats.max_lenght, line_lenght);
				}
			}while(!stream->eof());

			logline_stats.logline_available = amount_of_lines;
			logline_stats.logline_ignored = ignored_line;

			delete[] line_buffer;
			LOG("syslog_file::read_log(): number of lines available from ", logname.c_str()," is ", amount_of_lines, "; Ignored lines :", ignored_line );
			stream->close();
		}
		return amount_of_lines;
	}

	/*
	 * This way of calculating the memory consumption is not 100% accurate
	 */
	unsigned syslog_file::calculate_memory_consumption()
	{
		unsigned consumption = sizeof(logline_container);
		for( auto it : log_lines )
		{
			consumption += it.size() * sizeof(char) + sizeof(string);
		}
		if( log_lines.size() < log_lines.capacity() )
		{
			consumption += (log_lines.capacity() - log_lines.size()) * sizeof(string);
		}
		return consumption;
	}

	long syslog_file::read_all_log()
	{
		LOG("syslog_file::read_all_log(): Trying to read all the logs");
		if( syslog_files.empty() )
		{
			find_syslog_files();
		}
		unsigned container_threshold = 0, biggest_log = 0,amount_of_lines, total_amount_of_lines;
		for(auto it : syslog_files )
		{
			amount_of_lines = read_log(it);
			if( amount_of_lines > biggest_log )
			{
				biggest_log = amount_of_lines * THRESHOLD_INC_FACTOR;
				calibrate_logline_container( biggest_log * syslog_files.size() );
			}
			total_amount_of_lines += amount_of_lines;
		}
		LOG("syslog_file::read_all_log(): After ", syslog_files.size(), " log file has been readed, ", total_amount_of_lines, " lines are ready to be analysed!");
		//Print statistics
		if( FETCH_STATISTICS )
		{
			LOG("syslog_file::read_all_log(): Max line lenght is ", logline_stats.max_lenght, " Min is ", logline_stats.min_lenght );
			unsigned mem_consumption = calculate_memory_consumption();
			LOG("syslog_file::read_all_log(): Memory consumption for log line storing : ", mem_consumption," bytes ( ", mem_consumption/(1024*1024) ," MiB )");
		}
		return total_amount_of_lines;
	}

	const std::vector< string >& syslog_file::get_logfile_names()
	{
		return syslog_files;
	}

	logline_container& syslog_file::get_loglines()
	{
		return log_lines;
	}
}

