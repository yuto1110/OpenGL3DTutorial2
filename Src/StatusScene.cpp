#include"StatusScene.h"
#include"GLFWEW.h"
void StatusScene::ProcessInput() {
	GLFWEW::Window&window = GLFWEW::Window::Instance();
	if (window.GetGamePad().buttonDown&GamePad::START) {
		SceneStack::Instance().pop();
	}
}