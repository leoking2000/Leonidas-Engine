#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <LEO/LeoEngine.h>


static void run()
{
	leo::Window window(1600, 800, "Leonidas Engine", leo::WIN_FLAG_VSYNC | leo::WIN_FLAG_ESC_CLOSE);
	leo::GraphicsInitialization();

	leo::ShaderProgram shader(RESOURCES_PATH"Shaders/TestProject/Shader");

	float vertexs[] = {
		// pos       
		 0.25f,  0.25f,
		 0.25f, -0.25f,
		-0.25f, -0.25f,
	};

	leo::VertexBuffer vertexBuffer(vertexs, sizeof(vertexs));

	leo::ElementType arr[1] = { leo::ElementType::FLOAT2 };
	leo::Layout<1> layout(arr);

	leo::VertexArray vertexArray;
	vertexArray.AddBuffer(std::move(vertexBuffer), layout);

	// index buffer
	uint32_t indices[] = {
		0, 1, 2
	};

	leo::IndexBuffer indexBuffer(indices, 3);

	leo::Mesh object(vertexArray, indexBuffer, 1);

	leo::f32 offset = 0.0f;
	leo::f32 speed = 1.0f;

	while (!window.ShouldClose())
	{
		window.BeginFrame();
		window.SetTitle(std::format("FPS: {}", int(window.FPS())).c_str());

		if (offset > 0.5f || offset < -0.5f)
			speed *= -1.0f;

		offset += speed * window.DeltaTime();

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		shader.Bind();

		shader.SetUniform("offset", offset);
		object.Draw();

		shader.UnBind();


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