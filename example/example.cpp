#include "LogSDKEx.h"

int main()
{
	if (!LOG_INIT)
	{
		printf("log init failed");
		return -1;
	}
	
	LOG_INFO("hello %s", "world");
	LOG_DEBUG("hello %s", "world");
	LOG_ERROR("hello %s", "world");
	LOG_WARN("hello %s", "world");

	InitLog("", "Logs\\test1.log", "test1");
	InitLog("", "Logs\\test2.log", "test2");

	LOG_INFO("test1", LEVEL_INFO, "test1 info");
	LOG_INFO("test2", LEVEL_INFO, "test2 info");

	// while (true)
	// {
	// 	LOG_INFO("test roll file");
	// }
	

	return 0;
}
