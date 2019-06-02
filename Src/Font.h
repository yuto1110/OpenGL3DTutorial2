/**
@file Font.h
*/
#ifndef FONT_H_INCLUDED
#define FONT_H_INCLUDED
#include<GL\glew.h>
#include"Sprite.h"
#include<glm\glm.hpp>
#include<vector>
#include<string>

/**
*�r�b�g�}�b�v�t�H���g�`��N���X
*/
class FontRenderer {
public:
	FontRenderer() = default;
	~FontRenderer() = default;
	FontRenderer(const FontRenderer&) = delete;
	FontRenderer&operator=(const FontRenderer&) = default;

	bool Init(size_t maxChar);
	bool LoadFromFile(const char*path);
	void BeginUpdate();
	bool AddString(const glm::vec2&, const wchar_t*);
	void EndUpdate();
	void Draw(const glm::vec2&)const;
	float LineHeight()const;
private:
	SpriteRenderer spriteRenderer;
	std::vector<Texture::Image2DPtr>textures;
	float lineHeight = 0;
	float base = 0;
	//�������
	struct CharacterInfo {
		int id = -1;//�����R�[�h
		int page = 0;//�t�H���g�e�N�X�`���̔ԍ�
		glm::vec2 uv = glm::vec2(0);//�e�N�X�`�����W
		glm::vec2 size = glm::vec2(0);//�\���T�C�Y
		glm::vec2 offset = glm::vec2(0);//�\���ʒu�̕␳�ʒu
		float xadvance = 0;//���̕�����\������ʒu
	};
	std::vector<CharacterInfo>characterInfoList;//�������̃��X�g
};
#endif // !FONT_H_INCLUDED
