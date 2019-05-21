/**
* @file Main.cpp
*/
#include "TitleScene.h"
#include"GLFWEW.h"
int main() {
	GLFWEW::Window& window = GLFWEW::Window::Instance();
	window.Init(1280, 720, u8"アクションゲーム");
	SceneStack& sceneStack = SceneStack::Instance();
	sceneStack.push(std::make_shared<TitleScene>());

	while (!window.ShouoldClose()) {
		const float deltaTime = window.DeltaTime();
		window.UpdateTimer();
		sceneStack.Update(deltaTime);
		sceneStack.Render();
		window.SwapBuffers();
	}
}