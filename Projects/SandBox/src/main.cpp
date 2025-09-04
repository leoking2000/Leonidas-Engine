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
	s.player_pos = glm::vec2(LEO::WIN::Width() / 2.0f, LEO::WIN::Height() / 2.0f);
	s.player_radius = LEO::WIN::Width() * 0.01f;
}

void GameUpdate(GameState& s)
{
#if 0
	ImGui::Begin("Game Debug");

	ImGui::Text("FPS: %i", LEO::WIN::CurrentFPS());

	ImGui::DragFloat2("Player Pos:", glm::value_ptr(s.player_pos));
	ImGui::DragFloat("Player Radius:", &s.player_radius);

	if (ImGui::Button("Rand"))
	{
		s.player_pos = LEO::UTL::RandFloat2(100.0f, 900.0f);
		s.player_radius = LEO::UTL::RandFloat(20.0f, 100.0f);
	}

	ImGui::End();
#endif

	if (LEO::WIN::KeyDown(LEO::WIN::ESCAPE))
	{
		s.game_over = true;
	}

	if (LEO::WIN::KeyDown(LEO::WIN::W))
	{
		s.player_pos.y -= s.player_radius * 0.1f;
	}
	if (LEO::WIN::KeyDown(LEO::WIN::S))
	{
		s.player_pos.y += s.player_radius * 0.1f;
	}

	if (LEO::WIN::KeyDown(LEO::WIN::D))
	{
		s.player_pos.x += s.player_radius * 0.1f;
	}
	if (LEO::WIN::KeyDown(LEO::WIN::A))
	{
		s.player_pos.x -= s.player_radius * 0.1f;
	}

	if (LEO::WIN::MouseButtonPressed(LEO::WIN::MOUSE_BUTTON_LEFT))
	{
		s.player_pos = LEO::WIN::MousePosition();
	}
}

void GameDraw(GameState& s)
{
	LEO::WIN::RenderCircle(s.player_pos, s.player_radius, LEO_DARKGRAY);
	LEO::WIN::RenderTriangle({ 100.0f, 100.0f }, { 100.0f, 200.0f }, { 200.0f, 200.0f }, LEO_FORESTGREEN);
}


int main(void)
{
	LEO::LOG::GetDefaultLogChannel().SetLoggingLevel(LEO::LOG::Level::DEBUG);

	LEO::WIN::CreateWindow(1600, 900, "Leonidas Engine", LEO::WIN::FLAG_RESIZABLE);
	LEO::WIN::SetClearColor(LEO_BLACK);
	LEO::WIN::SetFPSTarget(60);

#if 0
	i32 year = -2025;
	year = glm::abs(year);
	LEO::UTL::Timer timer;
	LEOLOGVERBOSE("The year is {}", year);
	LEOLOGDEBUG("Leonidas Engine {}!!!", year);
	LEOLOGDEBUG("Log time: {}ms", timer.ElapsedMillis());

	LEOCHECKF(year == 2025, "wrong year!!!").LEOWATCH(year);
	LEOASSERT(year == 2025, "wrong year!!!").LEOWATCH(year);
#endif

	GameState s;
	GameInit(s);

	while (!LEO::WIN::ShouldClose() && !s.game_over)
	{
		LEO::WIN::StartFrame();
		
		GameUpdate(s);
		GameDraw(s);

		LEO::WIN::EndFrame();
	}


	LEO::WIN::DestroyWindow();

	return 0;
}


