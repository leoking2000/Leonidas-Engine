#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <LEO/LeoEngine.h>

static void init(leo::Window& window)
{
	leo::Random rng(2025);

	LEOLOGINFO("Hello!");

	LEOLOGINFO("Random Numbers!!!");

	LEOLOGINFO("Random ints {} {} {} {} {}", rng.Int(-2, 4), rng.Int(-2, 4), rng.Int(-2, 4), rng.Int(-2, 4), rng.Int(-2, 4));
	LEOLOGINFO("Random Uints {} {} {} {} {}", rng.Int(2, 4), rng.Int(2, 4), rng.Int(2, 4), rng.Int(2, 4), rng.Int(2, 4));

	leo::f32 foo = 0.0f;
}

static void display(leo::Window& window)
{
	glClearColor(0.2f, 0.2f, 0.9f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

static void run()
{
	leo::Window window(1600, 800, "Leonidas Engine", leo::WIN_FLAG_VSYNC | leo::WIN_FLAG_ESC_CLOSE);

	init(window);

	while (!window.ShouldClose())
	{
		window.BeginFrame();

		display(window);

		window.SetTitle(std::format("FPS: {}", int(window.FPS())).c_str());

		window.EndFrame();
	}
}

int main(void) 
{
	leo::WINInitialization();
	
	run();

	leo::WINTerminate();

	return 0;
}