/**
*@file Scene.cpp
*/
#include "Scene.h"
#include <iostream>
/**
*�R���X�g���N�^
*/
Scene::Scene(const char*name) :name(name) {

}

/**
*�f�X�g���N�^
*/
Scene::~Scene() {
	Finalize();
}

/**
*�V�[����������Ԃɂ���
*/
void Scene::Play() {
	isActive = true;
}

/**
*�V�[�����~��Ԃɂ���
*/
void Scene::Stop() {
	isActive = false;
}

/**
*�V�[����\������
*/
void Scene::Show() {
	isVisible = true;
}

/**
*�V�[�����\���ɂ���
*/
void Scene::Hide() {
	isVisible = false;
}

/**
*�V�[�����擾����
*/
const std::string&Scene::Name()const {
	return name;
}

/**
*�V�[���̊�����Ԃ𒲂ׂ�
*/
bool Scene::IsActive() const {
	return isActive;
}

/**
*�V�[���̕\����Ԃ𒲂ׂ�
*/
bool Scene::IsVisible()const {
	return isVisible;
}