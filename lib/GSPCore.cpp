#include <GSPCore.hpp>

#include <GWindow/GWindow.hpp>

bool GSPCore::Init()
{
    return GWindow::Init();
}

void GSPCore::Poll()
{
    GWindow::Poll();
}