#include "Application.hpp"

int main()
{
    spdlog::info("Demarrage de l'application");
    std::srand((unsigned int)::time(nullptr));

    Application app;
    return app.Run();
}