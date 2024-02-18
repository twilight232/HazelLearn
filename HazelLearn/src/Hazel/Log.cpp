#include "hzpch.h"
#include "Log.h"





namespace Hazel {
	std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
	std::shared_ptr<spdlog::logger> Log::s_ClientLogger;


	void Log::Init()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");  //%^����ɫ��ʼ��%$����ɫ������%T��ʱ�����%n����־���ƣ�%v����־ʵ���ı�����
		s_CoreLogger = spdlog::stdout_color_mt("HAZEL"); //stdout_color_mt����������屻��ֵ���"spdlog/sinks/stdout_color_sinks.h"��github�ϵ�issue��������
		s_CoreLogger->set_level(spdlog::level::trace);
		
		
		s_ClientLogger = spdlog::stdout_color_mt("APP");
		s_ClientLogger->set_level(spdlog::level::trace);

	}

	





}




