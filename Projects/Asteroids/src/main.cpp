#include <LEO/LeoEngine.h>
#include "components.h"
#include "Asteroids.h"

int main()
{
	LEO::CreateWindow(1600, 900, "Asteroids");

	LEO::SetClearColor(LEO_BLACK);
	bool game_over = false;

	Game game;

	while (!LEO::ShouldCloseWindow() && !game_over)
	{
		LEO::StartFrame();

		game.UpdateGame();
		game.RenderGame();

		if (LEO::KeyDown(LEO::KEY_ESCAPE)) { game_over = true; }

		std::string title = std::format("Leonidas Engine FPS:{}", LEO::CurrentFPS());
		LEO::SetWinTitle(title);

		LEO::EndFrame();
	}

	LEO::DestroyWindow();

	return 0;
}