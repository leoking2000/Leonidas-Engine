#include <LEO/LeoEngine.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>


class TestLayer : public leo::Layer
{
public:
	virtual void OnCreate() override
	{
		leo::GetDefaultLogChannel().SetLoggingLevel(leo::VERBOSE);

		shader = leo::ShaderProgram(RESOURCES_PATH"TestProject/basicTex");
		cube = leo::Mesh::GenerateCube();

		leo::ImageData image = leo::ReadImageData(RESOURCES_PATH"TestProject/brick1.jpg");

		texture = leo::Texture(image.width, image.height, leo::TextureFormat::RGBA8UB, image.data.get());


		leo::Color c = leo::SKYBLUE;
		glClearColor(c.r / 255.0f, c.g / 255.0f, c.b / 255.0f, c.a / 255.0f);
	}

	virtual void OnEvent(leo::Event& e)
	{
		LEOLOGVERBOSE("{}", e.ToString());
	}

	virtual void OnUpdate(leo::f32 dt) override
	{
		leo::Window& window = leo::Application::Get().GetWindow();
		glm::uvec2 size = window.Size();

		// Do the Rendering here for now since Renderer does to exits
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);

		glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(-1.5f, -1.0f, 0.0f));
		glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f));
		leo::f32 aspect = (float)size.x / (float)size.y;
		glm::mat4 proj = glm::perspective(1.0472f, aspect, 0.1f, 100.0f);
		
		shader.Bind();
		texture.Bind(1);

		shader.SetUniform("mv_matrix", view * model);
		shader.SetUniform("proj_matrix", proj);
		shader.SetUniform("samp", 1);

		cube.Draw();

		shader.UnBind();
	}

private:
	leo::ShaderProgram shader;
	leo::Mesh cube;
	leo::Texture texture;
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