/**
*@file Scene.cpp
*/
#include "Scene.h"
#include <iostream>
/**
*コンストラクタ
*/
Scene::Scene(const char*name) :name(name) {

}

/**
*デストラクタ
*/
Scene::~Scene() {
	Finalize();
}

/**
*シーンを活動状態にする
*/
void Scene::Play() {
	isActive = true;
}

/**
*シーンを停止状態にする
*/
void Scene::Stop() {
	isActive = false;
}

/**
*シーンを表示する
*/
void Scene::Show() {
	isVisible = true;
}

/**
*シーンを非表示にする
*/
void Scene::Hide() {
	isVisible = false;
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