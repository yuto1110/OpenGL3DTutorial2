#define _CRT_SECURE_NO_WARNINGS
#include "Font.h"
#include <memory>
#include<iostream>
#include<stdio.h>
/**
*フォント描画オブジェクトを初期化する
*/
bool FontRenderer::Init(size_t maxCharacter) {
	return spriteRenderer.Init(maxCharacter, "Res/Sprite.vert", "Res/Sprite.frag");
}

/**
*フォントファイルを読み込む
*/
bool FontRenderer::LoadFromFile(const char*filename) {
	std::unique_ptr<FILE, decltype(&fclose)>fp(fopen(filename, "r"), &fclose);
	if (!fp) {
		std::cerr << "[エラー]" << __func__ << ":" << filename << "を開けません.\n";
		return false;
	}
	//info行を読み込む
	int line = 1;//読み込む番号(エラー表示用)
	int spacing[2];//1行目の読み込みチェック用
	int ret = fscanf(fp.get(),
		"info face=\"%*[^\"]\"size=%*d bold=%*d charset=%*s unicode=%*d"
		"stretchH=%*d smooth=%*d aa%*d padding=%*d,%*d,%*d,%*d spacing=%*d,%d%*[^\n]",
		&spacing[0], &spacing[1]);
	if (ret < 2) {
		std::cerr << "[エラー]" << __func__ << ":" << filename << "の読み込みに失敗(" << line << "行目).\n";
		return false;
	}
	++line;
	//common行を読み込む
	float scaleH;
	ret = fscanf(fp.get(),
		"common lineHeight=&f base=%f scaleW=&d scaleH%f pages=%*d packed=%*d%*[^\n]",
		&lineHeight, &base, &scaleH);
	if (ret < 3) {
		std::cerr << "[エラー]" << __func__ << ":" << filename << "の読み込み失敗(" << line << "行目).\n";
		return false;
	}
	//page行の読み込み
	std::vector<std::string> texNameList;
	texNameList.reserve(16);
	for (;;) {
		int id;
		char tex[256];
		ret = fscanf(fp.get(), "page id=%d file=\"%255[^\"]\"", &id, tex);
		if (ret < 2) {
			break;
		}
		tex[sizeof(tex) / sizeof(tex[0]) - 1] = '\0';//0終端を保証する
		if (texNameList.size() <= static_cast<size_t>(id)) {
			texNameList.resize(id + 1);
		}
		texNameList[id] = std::string("Res/") + tex;
		++line;
	}
	if (texNameList.empty()) {
		std::cerr << "[エラー]" << __func__ << ":" << filename << "の読み込みに失敗(" << line << "行目).\n";
		return false;
	}

	//chars行を読み込む
	int charCount;//char行の数
	ret = fscanf(fp.get(), "chars count=%d", &charCount);
	if (ret < 1) {
		std::cerr << "[エラー]" << __func__ << ":" << filename << "行の読み込みに失敗(" << line << "行目).\n";
		return false;
	}
	++line;
	//char行を読み込む
	return true;
}
