#include "Application.hpp"

#ifdef PLAT_WINDOWS
#include <Windows.h>
#elif (PLAT_DARWIN || PLAT_LINUX)
#include <unistd.h>
#endif

#if (WIN_START && PLAT_WINDOWS)
int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) 
{
	(void)hInstance, (void)hPrevInstance, (void)lpCmdLine, (void)nCmdShow;
    spdlog::info("Demarrage de l'application");
    std::srand((unsigned int)::time(nullptr));

    Application app;
    return app.Run();
}
#else

int main(int argc, char** argv)
{
	(void)argc, (void)argv;
    spdlog::info("Demarrage de l'application");
    std::srand((unsigned int)::time(nullptr));

#ifdef PLAT_DARWIN
	std::filesystem::path exePath = argv[0];
	std::filesystem::current_path(exePath.parent_path());
#endif

    Application app;
    return app.Run();
}
#endif