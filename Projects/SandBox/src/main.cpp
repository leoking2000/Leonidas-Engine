#include <LEO/LeoEngine.h>
#include <imgui/imgui.h>

// make it run on the web

int main(void)
{
	LEO::LOG::Initialization();
	LEO::LOG::GetDefaultLogChannel().SetLoggingLevel(LEO::LOG::Level::DEBUG);

	LEO::WIN::Initialization(1600, 900, "Leonidas Engine", LEO::WIN::FLAG_VSYNC);
	LEO::WIN::SetClearColor(LEO_FORESTGREEN);

	i32 year = -2025;
	year = glm::abs(year);
	
	LEO::UTL::Timer timer;
	LEOLOGVERBOSE("The year is {}", year);
	LEOLOGDEBUG("Leonidas Engine {}!!!", year);
	LEOLOGDEBUG("Log time: {}ms", timer.ElapsedMillis());

	LEOCHECKF(year == 2025, "wrong year!!!").LEOWATCH(year);
	LEOASSERT(year == 2025, "wrong year!!!").LEOWATCH(year);

	while (!LEO::WIN::ShouldClose())
	{
		LEO::WIN::StartFrame();
		
		
		ImGui::Begin("hello");
		ImGui::Text("ImGui is working with raylib!!!!");
		ImGui::End();

		LEO::WIN::EndFrame();
	}


	LEO::WIN::Terminate();
	LEO::LOG::Terminate();

	return 0;
}


