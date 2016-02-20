#ifndef _IMAGE_LOGGER
#define _IMAGE_LOGGER

#include <windows.h>
#include <stdio.h>
#include <iostream>

HANDLE* console_handle;
FILE *log_file = fopen("log.txt", "a");

void set_console(HANDLE* consoleHandle);
void write_log(const char* format, ...);
void write_to_file(char* output);
void write_to_console(char* output);
void close_log();


BOOL WINAPI ConsoleHandler(DWORD dwCtrlType){
	LRESULT Result = 0;

	switch(dwCtrlType)
	{
		case CTRL_SHUTDOWN_EVENT:
		case CTRL_LOGOFF_EVENT:
		case CTRL_CLOSE_EVENT:
		{
			write_to_console("closing...");
			write_to_file("closing...");
			close_log();
		}
	}

	return Result;
}

void set_console(HANDLE* consoleHandle)
{
	console_handle = consoleHandle; 
	BOOL handlerSet = SetConsoleCtrlHandler( (PHANDLER_ROUTINE) ConsoleHandler, TRUE );

	if(!handlerSet)
		write_to_console("Error: Couldn't set the handler log file will not record properly");
}

void write_log(const char* format, ...){
	va_list args;
	va_start(args, format);
	
	int strLength = strlen(format);
	char* outputBuffer = (char*) malloc(strLength);
	
	strcpy(outputBuffer, format);
	
	int formatted = vsprintf(outputBuffer, format, args);

	if(formatted)
	{
		write_to_console(outputBuffer);
		write_to_file(outputBuffer);
	}
	else
	{
		write_to_console("Error formatting text");
		write_to_file("Error formatting text");
	}
	
	va_end(args);

}

void write_to_file(char* output)
{
	if(log_file == NULL)
	{
		write_to_console("log.txt does not exist, could not write to log");
		return;
	}
	fprintf(log_file, "%s\n", output);
}

void write_to_console(char* output)
{
	std::cout << output << std::endl;
}

void close_log()
{
	fclose(log_file); 
}

#endif