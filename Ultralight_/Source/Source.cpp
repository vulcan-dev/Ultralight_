#include "Application.h"

int main() {
    const uint16_t width = 800;
    const uint16_t height = 600;

    Application application("Ultralight", width, height);
    RefPtr<ultralight::Overlay> overlay_menu;
    application.InitializeOverlay(application.GetWindow(), overlay_menu, width, height, 0, 0, "file:///Assets/menu/ui_menu.html");
    application.Run();

    return 0;
}