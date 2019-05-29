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
	//info�s��ǂݍ���
	int line = 1;//�ǂݍ��ޔԍ�(�G���[�\���p)
	int spacing[2];//1�s�ڂ̓ǂݍ��݃`�F�b�N�p
	int ret = fscanf(fp.get(),
		"info face=\"%*[^\"]\"size=%*d bold=%*d charset=%*s unicode=%*d"
		"stretchH=%*d smooth=%*d aa%*d padding=%*d,%*d,%*d,%*d spacing=%*d,%d%*[^\n]",
		&spacing[0], &spacing[1]);
	if (ret < 2) {
		std::cerr << "[�G���[]" << __func__ << ":" << filename << "�̓ǂݍ��݂Ɏ��s(" << line << "�s��).\n";
		return false;
	}
	++line;
	//common�s��ǂݍ���
	float scaleH;
	ret = fscanf(fp.get(),
		"common lineHeight=&f base=%f scaleW=&d scaleH%f pages=%*d packed=%*d%*[^\n]",
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
		ret = fscanf(fp.get(), "page id=%d file=\"%255[^\"]\"", &id, tex);
		if (ret < 2) {
			break;
		}
		tex[sizeof(tex) / sizeof(tex[0]) - 1] = '\0';//0�I�[��ۏ؂���
		if (texNameList.size() <= static_cast<size_t>(id)) {
			texNameList.resize(id + 1);
		}
		texNameList[id] = std::string("Res/") + tex;
		++line;
	}
	if (texNameList.empty()) {
		std::cerr << "[�G���[]" << __func__ << ":" << filename << "�̓ǂݍ��݂Ɏ��s(" << line << "�s��).\n";
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
	//char�s��ǂݍ���
	return true;
}
