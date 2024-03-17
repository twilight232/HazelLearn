#pragma once

#include "../../Hazel/Render/VertexArray.h"

namespace Hazel {



	

/*顶点缓冲区（VBO），和索引缓冲区（EBO）存储了实际的数据，layout指明数据是如何布局的
	 VertexArray更像是对这两个缓冲区以及layout的引用*/
	
	 /*VertexArray对象更像是OpenGL状态的一个容器，它包含了绑定到其上的VBO和EBO，以及顶点布局信息。它可以被看作是对这些缓冲区和布局的引用。
	通过绑定VertexArray对象，可以将所有与绘制相关的信息（包括VBO、EBO和布局）集中在一个地方，这样在进行绘制时就不需要再次指定这些信息，从而提高了代码的清晰度和可维护性。*/


	class OpenGLVertexArray : public VertexArray
	{
	public:
		OpenGLVertexArray();
		virtual ~OpenGLVertexArray();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) override;  //增加对VBO的引用
		virtual void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) override;  //设置对EBO的引用

		virtual const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const { return m_VertexBuffers; }
		virtual const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const { return m_IndexBuffer; }
	private:
		uint32_t m_RendererID;  //生成句柄，指示VAO的句柄，面向gl函数
		std::vector<std::shared_ptr<VertexBuffer>> m_VertexBuffers;  //记录VBO  目前还没有使用这个来记录
		std::shared_ptr<IndexBuffer> m_IndexBuffer;  //记录EBO
	};

}