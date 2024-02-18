#pragma once

#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/fmt/ostr.h"



namespace Hazel {

	class HAZEL_API Log
	{
	public:
		static void Init();

		//设置两种日志类，一种由引擎核心来使用，另一种由客户端使用
		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;



	};




}


//Core log macros
#define HZ_CORE_ERROR(...)		::Hazel::Log::GetCoreLogger()->error(__VA_ARGS__)   //定义HZ_CORE_ERROR这个宏来代指后面的那个函数
#define HZ_CORE_WARN(...)		::Hazel::Log::GetCoreLogger()->warn(__VA_ARGS__) 
#define HZ_CORE_INFO(...)		::Hazel::Log::GetCoreLogger()->info(__VA_ARGS__) 
#define HZ_CORE_TRACE(...)		::Hazel::Log::GetCoreLogger()->trace(__VA_ARGS__) 
#define HZ_CORE_FATAL(...)		::Hazel::Log::GetCoreLogger()->fatal(__VA_ARGS__) 

//Client log macros

#define HZ_ERROR(...)			::Hazel::Log::GetClientLogger()->error(__VA_ARGS__)   //定义HZ_CORE_ERROR这个宏来代指后面的那个函数
#define HZ_WARN(...)			::Hazel::Log::GetClientLogger()->warn(__VA_ARGS__) 
#define HZ_INFO(...)			::Hazel::Log::GetClientLogger()->info(__VA_ARGS__) 
#define HZ_TRACE(...)			::Hazel::Log::GetClientLogger()->trace(__VA_ARGS__) 
#define HZ_FATAL(...)			::Hazel::Log::GetClientLogger()->fatal(__VA_ARGS__) 


//if dist build
#define HZ_CORE_INFO