#include "hzpch.h"
#include "Log.h"





namespace Hazel {
	std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
	std::shared_ptr<spdlog::logger> Log::s_ClientLogger;


	void Log::Init()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");  //%^是颜色起始，%$是颜色结束，%T是时间戳，%n是日志名称，%v是日志实际文本内容
		s_CoreLogger = spdlog::stdout_color_mt("HAZEL"); //stdout_color_mt这个函数定义被拆分到了"spdlog/sinks/stdout_color_sinks.h"，github上的issue栏看到的
		s_CoreLogger->set_level(spdlog::level::trace);
		
		
		s_ClientLogger = spdlog::stdout_color_mt("APP");
		s_ClientLogger->set_level(spdlog::level::trace);

	}

	





}




