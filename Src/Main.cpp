/**
* @file Main.cpp
*/
#include "TitleScene.h"
#include"GLFWEW.h"
#include<Windows.h>
#include<iostream>
int main() {
	GLFWEW::Window& window = GLFWEW::Window::Instance();
	window.Init(1280, 720, u8"�A�N�V�����Q�[��");
	SceneStack& sceneStack = SceneStack::Instance();
	sceneStack.push(std::make_shared<TitleScene>());

	while (!window.ShouoldClose()) {
		const float deltaTime = window.DeltaTime();
		window.UpdateTimer();

		//ESC�L�[�������ꂽ��I���E�B���h�E�\��
		if (window.IsKeyPressed(GLFW_KEY_ESCAPE)) {
			if (MessageBox(nullptr, "�Q�[�����I�����܂����H", "�I��", MB_OKCANCEL) == IDOK) {
				break;
			}
		}
		sceneStack.Update(deltaTime);

		//�o�b�t�@���폜����
		glClearColor(0.8f, 0.2f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//GL�R���e�L�X�g�̃p�����[�^�ݒ�
		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);

		sceneStack.Render();
		window.SwapBuffers();
	}
}