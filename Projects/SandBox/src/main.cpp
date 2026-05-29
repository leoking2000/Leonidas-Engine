#include <LEO/LeoEngine.h>
#include "SandBox.h"

int main(int argc, char** argv)
{
	leo::WindowsParameters win_params;
	win_params.width = 1600;
	win_params.height = 900;
	win_params.title = "Sandbox";
	win_params.init_flags = leo::WIN_FLAG_ESC_CLOSE;

	leo::Application app(win_params);

	app.GetLayerStack().PushLayer<SandBoxLayer>();
	app.Run();

	return 0;
}


