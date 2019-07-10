/**
*@file Terrain.h
*/
#ifndef TERRAIN_H_INCLUDED
#define TERRAIN_H_INCLUDED
#include"Mesh.h"
#include<glm/glm.hpp>
#include<string>
#include<vector>

namespace Terrain {
	/**
	*�����}�b�v
	*
	*1.LoadFromFile()�ŉ摜�t�@�C�����獂������ǂݍ���
	*.CreateMesh()�œǂݍ��񂾍�����񂩂�n�`���b�V�����쐬����
	*.����n�_�̍����𒲂ׂ�ɂ�Height()���g��
	*/
	class HeightMap {
	public:
		HeightMap() = default;
		~HeightMap() = default;

		bool LoadFromFile(const char*path, float scale, float baseLevel);
		float Height(const glm::vec3&pos)const;
		const glm::ivec2&Size()const;
		bool CreateMesh(Mesh::Buffer&meshBuffer,
			const char*meshName, const char*texName = nullptr)const;

	private:
		std::string name;///<���ƂɂȂ����t�@�C����
		glm::ivec2 size = glm::ivec2(0);///<�͂��ƃ}�b�v�̑傫��
		std::vector<float>heights;///<�����f�[�^

		glm::vec3 CalcNormal(int x, int z)const;

	};

	
}//namespace Terrain
#endif // !TERRAIN_H_INCLUDED
