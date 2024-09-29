#include <GWindow/GWindow.hpp>

GWindow::GWindow(const char* title, float width, float height) {
    window = GNativeWindow(title, width, height);
}

void GWindow::Poll() {
    GNativeWindow::Poll();
}