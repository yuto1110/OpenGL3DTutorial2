#define _CRT_SECURE_NO_WARNINGS
#include "Font.h"
#include <memory>
#include<iostream>
#include<stdio.h>
/**
*�t�H���g�`��I�u�W�F�N�g������������
*/
bool FontRenderer::Init(size_t maxCharacter) {
	return spriteRenderer.Init(maxCharacter, "Res/Sprite.vert", "Res/Sprite.frag");
}

/**
*�t�H���g�t�@�C����ǂݍ���
*/
bool FontRenderer::LoadFromFile(const char*filename) {
	std::unique_ptr<FILE, decltype(&fclose)>fp(fopen(filename, "r"), &fclose);
	if (!fp) {
		std::cerr << "[�G���[]" << __func__ << ":" << filename << "���J���܂���.\n";
		return false;
	}
	// info�s��ǂݍ���.
	int line = 1; // �ǂݍ��ލs�ԍ�(�G���[�\���p).
	int spacing[2]; // �P�s�ڂ̓ǂݍ��݃`�G�b�N�p.
	int ret = fscanf(fp.get(),
		"info face=\"%*[^\"]\" size=%*d bold=%*d italic=%*d charset=%*s unicode=%*d"
		 " stretchH=%*d smooth=%*d aa=%*d padding=%*d,%*d,%*d,%*d spacing=%d,%d%*[^\n]",
		&spacing[0], &spacing[1]);
	if (ret < 2) {
		std::cerr << "[�G���[]" << __func__ << ": " << filename << "�̓ǂݍ��݂Ɏ��s(" <<
			line << "�s��).\n";
		return false;
		
	}
	++line;
	//common�s��ǂݍ���
	float scaleH;
	ret = fscanf(fp.get(),
		" common lineHeight=%f base=%f scaleW=%*d scaleH=%f pages=%*d packed=%*d%*[^\n]",
		&lineHeight, &base, &scaleH);
	if (ret < 3) {
		std::cerr << "[�G���[]" << __func__ << ":" << filename << "�̓ǂݍ��ݎ��s(" << line << "�s��).\n";
		return false;
	}
	//page�s�̓ǂݍ���
	std::vector<std::string> texNameList;
	texNameList.reserve(16);
	for (;;) {
		int id;
		char tex[256];
		ret = fscanf(fp.get(), " page id=%d file=\"%255[^\"]\"", &id, tex);
		if (ret < 2) {
			break;
			
		}
		tex[sizeof(tex) / sizeof(tex[0]) - 1] = '\0'; // 0�I�[��ۏ؂���.
		if (texNameList.size() <= static_cast<size_t>(id)) {
			texNameList.resize(id + 1);
			
		}
		texNameList[id] = std::string("Res/") + tex;
		++line;
		
	}
	if (texNameList.empty()) {
		std::cerr << "[�G���[]" << __func__ << ": " << filename << "�̓ǂݍ��݂Ɏ��s(" <<
			line << "�s��).\n";
		return false;
		
	}

	//chars�s��ǂݍ���
	int charCount;//char�s�̐�
	ret = fscanf(fp.get(), "chars count=%d", &charCount);
	if (ret < 1) {
		std::cerr << "[�G���[]" << __func__ << ":" << filename << "�s�̓ǂݍ��݂Ɏ��s(" << line << "�s��).\n";
		return false;
	}
	++line;
	// char�s��ǂݍ���.
	characterInfoList.clear();
	characterInfoList.resize(65536); // 16bit�ŕ\����͈͂��m��.
	for (int i = 0; i < charCount; ++i) {
		CharacterInfo info;
		ret = fscanf(fp.get(),
			" char id=%d x=%f y=%f width=%f height=%f xoffset=%f yoffset=%f xadvance=%f"
			 " page=%d chnl=%*d",
			&info.id, &info.uv.x, &info.uv.y, &info.size.x, &info.size.y,
			&info.offset.x, &info.offset.y, &info.xadvance, &info.page);
		if (ret < 9) {
			std::cerr << "[�G���[]" << __func__ << ": " << filename << "�̓ǂݍ��݂Ɏ��s(" <<
				line << "�s��).\n";
			return false;
			
		}
		
			    // �t�H���g�t�@�C���͍��オ���_�Ȃ̂ŁAOpenGL�̍��W�n(���������_)�ɕϊ�.
			info.uv.y = scaleH - info.uv.y - info.size.y;
		
			if (info.id < characterInfoList.size()) {
			characterInfoList[info.id] = info;
			
		}
		++line;
		
	}
	//�e�N�X�`����ǂݍ���
	textures.clear();
	textures.reserve(texNameList.size());
	for (const std::string&e : texNameList) {
		Texture::Image2DPtr tex = Texture::Image2D::Create(e.c_str());
		if (!tex) {
			return false;
		}
		textures.push_back(tex);
	}
	return true;
}

/**
*������̒ǉ����J�n����
*/
void FontRenderer::BeginUpdate() {
	spriteRenderer.BeginUpdate();
}

/**
*�������ǉ�����
*/
bool FontRenderer::AddString(const glm::vec2&position,const glm::vec4&color, const wchar_t*str) {
	glm::vec2 pos = position;
	glm::vec4 col = color;
	for (const wchar_t*itr = str; *itr; ++itr) {
		const CharacterInfo& info = characterInfoList[*itr];
		if (info.id >= 0 && info.size.x&&info.size.y) {
			//�X�v���C�g�̍��W�͉摜�̒��S���w�肷�邪�t�H���g�͍�����w�肷��
			//�����ł��̍���ł��������߂̕␳�l���v�Z����
			const float baseX = info.size.x*0.5f - info.offset.x;
			const float baseY = base - info.size.y * 0.5f - info.offset.y;
			const glm::vec3 spritePos = glm::vec3(pos + glm::vec2(baseX, baseY), 0);

			Sprite sprite(textures[info.page]);
			sprite.Position(spritePos);
			sprite.Rectangle({ info.uv,info.size });
			sprite.Color();
			if (!spriteRenderer.AddVertices(sprite)) {
				return false;
			}
		}
		pos.x += info.xadvance;//���̕\���ʒu�ֈړ�
	}
	return true;
}
/**
*������̒ǉ����I������
*/
void FontRenderer::EndUpdate() {
	spriteRenderer.EndUpdate();
}

/**
*�t�H���g��`�悷��
*/
void FontRenderer::Draw(const glm::vec2&screenSize) const {
	spriteRenderer.Draw(screenSize);
}
/**
*�s�̍������擾����
*/
float FontRenderer::LineHeight()const {
	return lineHeight;
}