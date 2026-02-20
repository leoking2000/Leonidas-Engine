#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <LEO/LeoEngine.h>


static void run()
{
	leo::WINInitialization();
	leo::Window window(1600, 900, "Leonidas Engine", leo::WIN_FLAG_VSYNC | leo::WIN_FLAG_ESC_CLOSE);
	window.Create();

	leo::GraphicsInitialization();

	glm::vec2 center = window.HalfSize();

	leo::f32 offset = 0.0f;
	leo::f32 speed = 400.0f;

	while (!window.ShouldClose())
	{
		window.BeginFrame();
		window.SetTitle(std::format("FPS: {}", int(window.FPS())).c_str());

		if (center.x + offset > 2*center.x || center.x + offset < 0)
			speed *= -1.0f;

		offset += speed * window.DeltaTime();

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		window.DrawCircle(center.x + offset, center.y, 50, LEO_DARKBLUE);

		window.EndFrame();
	}

	window.Destroy();
	leo::WINTerminate();
}


int main(void) 
{
	run();

	return 0;
}