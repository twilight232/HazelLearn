#pragma once

#ifdef HZ_PLATFORM_WINDOWS

		#ifdef  HZ_BUILD_DLL
			#define HAZEL_API __declspec(dllexport)   //��Ӧ����dll����ʱ����������Ҫ�����Ĵ���
		#else
			#define HAZEL_API __declspec(dllimport)   //û��HZ_BUILD_DLL��ǣ������ǿͻ��˴�����ʹ��dll���룬��ѡ����
		#endif //  HZ_BUILD_DLL

#else
#error Hazel only support Windows!



#endif // HZ_PLATFORM_WINDOWS


