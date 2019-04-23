/**
*@file Scene.cpp
*/
#include "Scene.h"
#include <iostream>
/**
*コンストラクタ
*/
Scene::Scene(const char*name) :name(name) {
	std::cout << "Scene コンストラクタ:" << name << "\n";

}

/**
*デストラクタ
*/
Scene::~Scene() {
	Finalize();
	std::cout << "Scene デストラクタ:" << name << "\n";
}

/**
*シーンを活動状態にする
*/
void Scene::Play() {
	isActive = true;
	std::cout << "Scene Play:" << name << "\n";
}

/**
*シーンを停止状態にする
*/
void Scene::Stop() {
	isActive = false;
	std::cout << "Scene Stop:" << name << "\n";
}

/**
*シーンを表示する
*/
void Scene::Show() {
	isVisible = true;
	std::cout << "Scene Show:" << name << "\n";
}

/**
*シーンを非表示にする
*/
void Scene::Hide() {
	isVisible = false;
	std::cout << "Scene Hide:" << name << "\n";
}

/**
*シーンを取得する
*/
const std::string&Scene::Name()const {
	return name;
}

/**
*シーンの活動状態を調べる
*/
bool Scene::IsActive() const {
	return isActive;
}

/**
*シーンの表示状態を調べる
*/
bool Scene::IsVisible()const {
	return isVisible;
}