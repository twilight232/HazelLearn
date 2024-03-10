#pragma once

namespace Hazel {

	enum class RendererAPI
	{
		None = 0, OpenGL = 1
	};

	class Renderer
	{
	public:
		inline static RendererAPI GetAPI() { return s_RendererAPI; } //¾²Ì¬º¯Êý
	private: 
		static RendererAPI s_RendererAPI;  //¾²Ì¬±äÁ¿  
	};


}