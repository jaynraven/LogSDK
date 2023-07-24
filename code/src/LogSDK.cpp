#include "LogSDK.h"
#include "log4cpp/Category.hh"
#include "log4cpp/RollingFileAppender.hh"
#include "log4cpp/PropertyConfigurator.hh"
#include "log4cpp/PatternLayout.hh"
#include "io.h"
#include "version/version.h"
#pragma comment(lib, "ws2_32.lib") 

#define LOG_BUFFER_LEN_MAX (64 * 1024)
#define LOG_FILE_SIZE_MAX  (10 * 1024 * 1024)

bool GetFileFolde(const std::string& file, std::string &folder)
{
	size_t pos = file.rfind('/');
	if (pos != file.npos)
	{
		folder = file.substr(0, pos);
		return true;
	}
	else
	{
		pos = file.rfind('\\');
		if (pos != file.npos)
		{
			folder = file.substr(0, pos);
			return true;
		}
	}
	return false;
}

bool InitLog(const char* config_file, const char* log_file, const char* catalog)
{
	try
	{
		std::string log_folder;
		if (!GetFileFolde(log_file, log_folder))
		{
			printf("Log file path is not correct\n");
			return false;
		}
		if (access(log_folder.c_str(), 0) != 0)
		{
			std::string mkdir = "mkdir \"" + log_folder + "\"";
			if (0 != system(mkdir.c_str()))
			{
				printf("Log folder create fail\n");
				return false;
			}
		}

		if (0 == access(config_file, 0))
		{
			log4cpp::PropertyConfigurator::configure(config_file);
		}
		else
		{
			printf("Cound not find config file\n");
		}
		
		log4cpp::Category& root = log4cpp::Category::getInstance(catalog);
		log4cpp::RollingFileAppender *appender = new log4cpp::RollingFileAppender(catalog, log_file);
		appender->setMaximumFileSize(LOG_FILE_SIZE_MAX);

		log4cpp::PatternLayout *patternLayout = new log4cpp::PatternLayout();
		patternLayout->setConversionPattern("%d{%Y-%m-%d %H:%M:%S.%l} [%t] [%p] %m %n");
		appender->setLayout(patternLayout);

		root.addAppender(appender);

		root.info("LogSDK version: %s", SDK_VERSION);
		return true;
	}
	catch(const std::exception& e)
	{
		printf("InitLog exception: %s", e.what());
		return false;
	}
}

bool Log(const char* catalog, LogLevel level, const char* format, ...)
{
	try
	{
		int res = false;
		char buffer[LOG_BUFFER_LEN_MAX] = {0};
		va_list args;
		va_start (args, format);
		vsnprintf (buffer, LOG_BUFFER_LEN_MAX - 1, format, args);
		va_end (args);

		if (log4cpp::Category::exists(catalog))
		{
			switch (level)
			{
			case LEVEL_INFO:
				if (log4cpp::Category::getInstance(catalog).isInfoEnabled())
				{
					// log4cpp::Category::getInstance(catalog).info(format, args);
					log4cpp::Category::getInstance(catalog).info("%s", buffer);
					res = true;
				}
				break;
			case LEVEL_ERROR:
				if (log4cpp::Category::getInstance(catalog).isErrorEnabled())
				{
					// log4cpp::Category::getInstance(catalog).error(format, args);
					log4cpp::Category::getInstance(catalog).error("%s", buffer);
					res = true;
				}
				break;
			case LEVEL_WARN:
				if (log4cpp::Category::getInstance(catalog).isWarnEnabled())
				{
					// log4cpp::Category::getInstance(catalog).warn(format, args);
					log4cpp::Category::getInstance(catalog).warn("%s", buffer);
					res = true;
				}
				break;
			case LEVEL_DEBUG:
				if (log4cpp::Category::getInstance(catalog).isDebugEnabled())
				{
					// log4cpp::Category::getInstance(catalog).debug(format, args);
					log4cpp::Category::getInstance(catalog).debug("%s", buffer);
					res = true;
				}
				break;
			default:
				{
					printf("Unknown log level: %u", level);
					res = false;
				}
					break;
			}
		}
		
		return res;
	}
	catch(const std::exception& e)
	{
		printf("Log exception: %s", e.what());\
		return false;
	}
}