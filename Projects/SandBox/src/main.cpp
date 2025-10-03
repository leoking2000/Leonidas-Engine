#include "SandBox.h"

int main(int argc, char** argv)
{
	LEO::WindowsParameters win_params;
	win_params.win_width = 1600;
	win_params.win_height = 900;
	win_params.win_title = "Sandbox";
	win_params.win_init_flags = LEO::WIN_FLAG_ESC_CLOSE;

	LEO::Application app(win_params);

	app.PushLayer<SandBoxLayer>();
	app.Run();

	return 0;
}


