#pragma once

#include "../../Hazel/Render/VertexArray.h"

namespace Hazel {



	

/*���㻺������VBO������������������EBO���洢��ʵ�ʵ����ݣ�layoutָ����������β��ֵ�
	 VertexArray�����Ƕ��������������Լ�layout������*/
	
	 /*VertexArray���������OpenGL״̬��һ���������������˰󶨵����ϵ�VBO��EBO���Լ����㲼����Ϣ�������Ա������Ƕ���Щ�������Ͳ��ֵ����á�
	ͨ����VertexArray���󣬿��Խ������������ص���Ϣ������VBO��EBO�Ͳ��֣�������һ���ط��������ڽ��л���ʱ�Ͳ���Ҫ�ٴ�ָ����Щ��Ϣ���Ӷ�����˴���������ȺͿ�ά���ԡ�*/


	class OpenGLVertexArray : public VertexArray
	{
	public:
		OpenGLVertexArray();
		virtual ~OpenGLVertexArray();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) override;  //���Ӷ�VBO������
		virtual void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) override;  //���ö�EBO������

		virtual const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const { return m_VertexBuffers; }
		virtual const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const { return m_IndexBuffer; }
	private:
		uint32_t m_RendererID;  //���ɾ����ָʾVAO�ľ��������gl����
		std::vector<std::shared_ptr<VertexBuffer>> m_VertexBuffers;  //��¼VBO  Ŀǰ��û��ʹ���������¼
		std::shared_ptr<IndexBuffer> m_IndexBuffer;  //��¼EBO
	};

}