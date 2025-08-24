#include <LEO/LeoEngine.h>
#include <imgui/imgui.h>

struct GameState
{
	glm::vec2 player_pos;
	f32       player_radius;
};

void GameInit(GameState& s)
{
	s.player_pos = glm::vec2(LEO::WIN::Width() / 2.0f, LEO::WIN::Height() / 2.0f);
	s.player_radius = LEO::WIN::Width() * 0.01f;
}

void GameUpdate(GameState& s)
{
	ImGui::Begin("Game Debug");
	ImGui::DragFloat2("Player Pos:", glm::value_ptr(s.player_pos));
	ImGui::DragFloat("Player Radius:", &s.player_radius);
	ImGui::End();
}

void GameDraw(GameState& s)
{
	LEO::WIN::RenderCircle(s.player_pos, s.player_radius, LEO_DARKGRAY);
	LEO::WIN::RenderTriangle({ 100.0f, 100.0f }, { 100.0f, 200.0f }, { 200.0f, 200.0f }, LEO_FORESTGREEN);
}


int main(void)
{
	LEO::LOG::Initialization();
	LEO::LOG::GetDefaultLogChannel().SetLoggingLevel(LEO::LOG::Level::DEBUG);

	LEO::WIN::Initialization(1600, 900, "Leonidas Engine", LEO::WIN::FLAG_VSYNC);
	LEO::WIN::SetClearColor(LEO_BLACK);

	i32 year = -2025;
	year = glm::abs(year);
	
	LEO::UTL::Timer timer;
	LEOLOGVERBOSE("The year is {}", year);
	LEOLOGDEBUG("Leonidas Engine {}!!!", year);
	LEOLOGDEBUG("Log time: {}ms", timer.ElapsedMillis());

	LEOCHECKF(year == 2025, "wrong year!!!").LEOWATCH(year);
	LEOASSERT(year == 2025, "wrong year!!!").LEOWATCH(year);

	GameState s;
	GameInit(s);

	while (!LEO::WIN::ShouldClose())
	{
		LEO::WIN::StartFrame();
		
		GameUpdate(s);
		GameDraw(s);

		LEO::WIN::EndFrame();
	}


	LEO::WIN::Terminate();
	LEO::LOG::Terminate();

	return 0;
}


