#include <LEO/LeoEngine.h>
#include "components.h"
#include "Asteroids.h"

int main()
{
	LEO::CreateWindow(1600, 900, "Asteroids", LEO::WIN_FLAG_ESC_CLOSE);

	LEO::SetClearColor(LEO_BLACK);
	Game game;

	while (!LEO::ShouldCloseWindow())
	{
		LEO::StartFrame();

		game.UpdateGame();
		game.RenderGame();

		LEO::EndFrame();
	}

	LEO::DestroyWindow();

	return 0;
}