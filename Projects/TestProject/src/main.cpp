#include <LEO/LeoEngine.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>


class TestLayer : public leo::Layer
{
public:

	virtual void OnUpdate(leo::f32 dt) override
	{
		leo::Window& window = leo::Application::Get().GetWindow();
		window.SetTitle(std::format("FPS: {}", int(window.FPS())).c_str());

		glm::vec2 center = window.HalfSize();

		offset += speed * window.DeltaTime();

		if (center.x + offset > 2 * center.x)
		{
			speed *= -1.0f;
			offset = center.x;
		}
		else if (center.x + offset < 0)
		{
			offset = -center.x;
			speed *= -1.0f;
		}

		// Do the Rendering here for now since Renderer does to exits
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		window.DrawCircle(center.x + offset, center.y, 50, LEO_DARKBLUE);
	}

private:
	leo::f32 offset = 0.0f;
	leo::f32 speed = 800.0f;
};



int main(void) 
{
	leo::Application app({ 1600, 900, "Leonidas Engine", leo::WIN_FLAG_VSYNC | leo::WIN_FLAG_ESC_CLOSE });

	app.GetLayerStack().PushLayer<TestLayer>();

	app.Run();

	return 0;
}