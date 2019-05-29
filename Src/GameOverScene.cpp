#include"GameOverScene.h"
#include"TitleScene.h"
#include "GLFWEW.h"

bool GameOverScene::Initialize() {
	spriteRenderer.Init(1000, "Res/Sprite.vert", "Res/Sprite.frag");
	sprites.reserve(100);
	Sprite spr(Texture::Image2D::Create("Res/House.tga"));
	spr.Scale(glm::vec2(2));
	sprites.push_back(spr);
	return true;
}
void GameOverScene::ProcessInput() {
	GLFWEW::Window&window = GLFWEW::Window::Instance();
	if (window.GetGamePad().buttonDown&GamePad::START) {
		SceneStack::Instance().Replace(std::make_shared<TitleScene>());
	}
}
/**
*シーンを更新する
*/
void GameOverScene::Update(float deltaTime) {
	spriteRenderer.BeginUpdate();
	for (const Sprite&e : sprites) {
		spriteRenderer.AddVertices(e);
	}
	spriteRenderer.EndUpdate();
}
/**
*シーンを描画する
*/

void GameOverScene::Render()
{
	const GLFWEW::Window& window = GLFWEW::Window::Instance();
	const glm::vec2 screenSize(window.Width(), window.Height());
	spriteRenderer.Draw(screenSize);
}