#ifndef general_debug_h
#define general_debug_h

#ifdef ERROR_HANDLING

	#include <stdio.h>
	#include <errno.h>
	#include <string.h>
	
	#define clean_errno \
		((errno == 0) ? "None" : strerror(errno))
	
	#define log_err(message, ...) \
		fprintf(stderr, "[Error] (%s, %d | %s) " \
		message "\n", __FILE__, __LINE__, clean_errno, ##__VA_ARGS__)
	
	#define log_warn(message, ...) \
		fprintf(stderr, "[Warning] (%s, %d | errno: %s) " \
		message "\n", __FILE__, __LINE__, clean_errno, ##__VA_ARGS__)

	#define log_info(message, ...) \
		fprintf(stderr, "[Info] (%s, %d) " \
		message "\n", __FILE__, __LINE__, ##__VA_ARGS__)
	
	#define handle_error(wrong, message, ...) \
		if((wrong)) {log_err(message, ##__VA_ARGS__); errno=0; goto error;}
	
	#define sentinel(message, ...) \
		{log_err(message, ##__VA_ARGS__); errno=0; goto error;}
	
	#define handle_mem_error(wrong) \
		handle_error(wrong, "Memory management error.")

#else

	#define clean_errno
	
	#define log_err(message, ...)
	
	#define log_warn(message, ...)
	
	#define log_info(message, ...)
	
	#define handle_error(wrong, message, ...)
	
	#define sentinel(message, ...)
	
	#define handle_mem_error(wrong)

#endif

#endif
