#include <LEO/LeoEngine.h>
#include <imgui/imgui.h>

struct GameState
{
	glm::vec2 player_pos;
	f32       player_radius;
	bool      game_over = false;
};

void GameInit(GameState& s)
{
	s.player_pos = glm::vec2(LEO::WinWidth() / 2.0f, LEO::WinHeight() / 2.0f);
	s.player_radius = LEO::WinWidth() * 0.01f;
}

void GameUpdate(GameState& s)
{
#if 1
	ImGui::Begin("Game Debug");

	ImGui::Text("FPS: %i", LEO::CurrentFPS());

	ImGui::DragFloat2("Player Pos:", glm::value_ptr(s.player_pos));
	ImGui::DragFloat("Player Radius:", &s.player_radius);

	if (ImGui::Button("Rand"))
	{
		s.player_pos = LEO::RandFloat2(100.0f, 900.0f);
		s.player_radius = LEO::RandFloat(20.0f, 100.0f);
	}

	ImGui::End();
#endif

	if (LEO::KeyDown(LEO::KEY_ESCAPE))
	{
		s.game_over = true;
	}

	if (LEO::KeyDown(LEO::KEY_W))
	{
		s.player_pos.y -= s.player_radius * 0.1f;
	}
	if (LEO::KeyDown(LEO::KEY_S))
	{
		s.player_pos.y += s.player_radius * 0.1f;
	}

	if (LEO::KeyDown(LEO::KEY_D))
	{
		s.player_pos.x += s.player_radius * 0.1f;
	}
	if (LEO::KeyDown(LEO::KEY_A))
	{
		s.player_pos.x -= s.player_radius * 0.1f;
	}

	if (LEO::MouseButtonPressed(LEO::MOUSE_BUTTON_RIGHT))
	{
		s.player_pos = LEO::MousePosition();
	}
}

void GameDraw(GameState& s)
{
	LEO::RenderCircle(s.player_pos, s.player_radius, LEO_DARKGRAY);
	LEO::RenderTriangle({ 100.0f, 100.0f }, { 100.0f, 200.0f }, { 200.0f, 200.0f }, LEO_FORESTGREEN);
}


int main(void)
{
	LEO::GetDefaultLogChannel().SetLoggingLevel(LEO::LogLevel::DEBUG);

	LEO::CreateWindow(1600, 900, "Leonidas Engine", LEO::WIN_FLAG_RESIZABLE);
	LEO::SetClearColor(LEO_BLACK);
	LEO::SetFPSTarget(60);

#if 1
	i32 year = -2025;
	year = glm::abs(year);
	LEO::Timer timer;
	LEOLOGVERBOSE("The year is {}", year);
	LEOLOGDEBUG("Leonidas Engine {}!!!", year);
	LEOLOGDEBUG("Log time: {}ms", timer.ElapsedMillis());

	LEOCHECKF(year == 2025, "wrong year!!!").LEOWATCH(year);
	LEOASSERT(year == 2025, "wrong year!!!").LEOWATCH(year);
#endif

	GameState s;
	GameInit(s);

	while (!LEO::ShouldCloseWindow() && !s.game_over)
	{
		LEO::StartFrame();
		
		GameUpdate(s);
		GameDraw(s);

		LEO::EndFrame();
	}


	LEO::DestroyWindow();

	return 0;
}


