#include"MainGameScene.h"
#include"StatusScene.h"
#include"GameOverScene.h"
#include"GLFWEW.h"

void MainGameScene::ProcessInput() {
	if (!flag) {
		GLFWEW::Window&window = GLFWEW::Window::Instance();
		if (window.GetGamePad().buttonDown&GamePad::START) {
			flag = true;
			SceneStack::Instance().push(std::make_shared<StatusScene>());
		}
	}
	else {
		GLFWEW::Window&window = GLFWEW::Window::Instance();
		if (window.GetGamePad().buttonDown&GamePad::START) {
			SceneStack::Instance().Replace(std::make_shared<GameOverScene>());
		}
	}
}