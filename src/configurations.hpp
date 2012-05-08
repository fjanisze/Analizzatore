/*******************************
 *
 * Generic configurations
 *
********************************/

#define SYSLOG_FILE_FORMAT "SYSLOG*.LOG"

#define INVALID_FILE_HANDLER -1

#define MAX_LOG_LINE_SIZE 1024
#define MIN_LOG_LINE_SIZE 40
#define INITIAL_LINE_COUNTER 100000
#define THRESHOLD_INC_FACTOR 1.05  //Allow to reduce the amount fo reallocations when loading the log files
#define SYSLOG_HEADER_SIZE 30 //This is not an accurate value, but at least 30 characters more are present at the beginning of every syslog line


/*******************************
 *
 * Logging
 *
********************************/

#define LOGGING_LEVEL_1


/*******************************
 *
 * For statistic purpose
 *
********************************/

#define FETCH_STATISTICS 1 //1 - Collect, 0 - Nope
