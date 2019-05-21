#include "TitleScene.h"
#include"MainGameScene.h"
#include"GLFWEW.h"

void TitleScene::ProcessInput() {
	GLFWEW::Window&window = GLFWEW::Window::Instance();
	if (window.GetGamePad().buttonDown&GamePad::START) {
		SceneStack::Instance().Replace(std::make_shared<MainGameScene>());
	}
}