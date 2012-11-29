#ifndef _LOG_H_
#define _LOG_H_

#ifdef DEBUG
	#define LOG_DEBUG(x) log_message(x)
#else
	#define LOG_DEBUG(x)
#endif

void log_message(const char *);

#endif
