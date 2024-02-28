#pragma once

#ifdef HZ_PLATFORM_WINDOWS

		#ifdef  HZ_BUILD_DLL
			#define HAZEL_API __declspec(dllexport)   //对应编译dll代码时，代表是需要导出的代码
		#else
			#define HAZEL_API __declspec(dllimport)   //没有HZ_BUILD_DLL标记，代表是客户端代码在使用dll代码，即选择导入
		#endif //  HZ_BUILD_DLL

#else
#error Hazel only support Windows!
#endif // HZ_PLATFORM_WINDOWS




#ifdef HZ_ENABLE_ASSERTS
#define HZ_ASSERT(x, ...) { if(!(x)) { HZ_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#define HZ_CORE_ASSERT(x, ...) { if(!(x)) { HZ_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
#define HZ_ASSERT(x, ...)
#define HZ_CORE_ASSERT(x, ...)
#endif



#define BIT(x) (1 << x)

#define HZ_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)



