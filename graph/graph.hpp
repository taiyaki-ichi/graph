#pragma once

namespace graph
{

	//�L���O���t�̂Ƃ�
	struct directed {};
	
	//���_�ɏ�����������Ƃ�
	template<typename T>
	struct vertexProperty {};

	//�ӂɏ�����������Ƃ�
	template<typename T>
	struct edgeProperty {};


	//�O���t�{��
	//�אڃ��X�g�ŊǗ�����\��
	template<typename... Args>
	class graph{};

}