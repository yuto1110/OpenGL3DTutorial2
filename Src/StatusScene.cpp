#include"StatusScene.h"
#include"GLFWEW.h"

/**
*�V�[��������������
*/
bool StatusScene::Initialize() {
	spriteRenderer.Init(1000, "Res/Sprite.vert", "Res/Sprite.frag");
	sprites.reserve(100);
	Sprite spr(Texture::Image2D::Create("Res/rock.tga"));
	spr.Scale(glm::vec2(2));
	sprites.push_back(spr);
	return true;
}
void StatusScene::ProcessInput() {
	GLFWEW::Window&window = GLFWEW::Window::Instance();
	if (window.GetGamePad().buttonDown&GamePad::START) {
		SceneStack::Instance().pop();
	}
}
/**
*�V�[�����X�V����
*/
void StatusScene::Update(float deltaTime) {
	spriteRenderer.BeginUpdate();
	for (const Sprite&e : sprites) {
		spriteRenderer.AddVertices(e);
	}
	spriteRenderer.EndUpdate();
}
/**
*�V�[����`�悷��
*/

void StatusScene::Render()
{
	const GLFWEW::Window& window = GLFWEW::Window::Instance();
	const glm::vec2 screenSize(window.Width(), window.Height());
	spriteRenderer.Draw(screenSize);
}