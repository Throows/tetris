#include "Application.hpp"

int main()
{
    spdlog::info("Demarrage de l'application");
    std::srand(std::time(nullptr));

    Application app;
    return app.Run();
}