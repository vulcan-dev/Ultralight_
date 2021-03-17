#include "Application.h"
#include <iostream>


Application::Application(const std::string title, const uint16_t& width, const uint16_t& height) : title_(title), width_(width), height_(height), counter_(0) {
    this->InitializePlatform();
    this->Initialize();
}

int16_t Application::Initialize() {
#ifdef _DEBUG
    AllocConsole();
    spdlog::set_level(spdlog::level::debug);
#endif

    try {
        this->app_ = App::Create();
    } catch (std::exception e) {
        spdlog::error("App::Create : ", e.what());
        exit(EXIT_FAILURE);
    }

    this->InitializeWindow();

    return 0;
}

int16_t Application::InitializeWindow() {
    try {
        /* Check if title is empty */
        if (this->title_.empty()) {
            this->title_ = "Application";
            spdlog::warn("Title \"\" is invalid. Reverting to defaults \"Application\"");
        }

        this->window_ = Window::Create(this->app_->main_monitor(), this->width_, this->height_, false, ultralight::kWindowFlags_Titled | ultralight::kWindowFlags_Resizable);

        this->window_->SetTitle(this->title_.c_str());
        this->window_->set_listener(this);
    }
    catch (std::exception e) {
        spdlog::error("Window::Create : ", e.what());
        exit(EXIT_FAILURE);
    }

    return 0;
}

int16_t Application::InitializeOverlay(RefPtr<Window>& window, RefPtr<Overlay> overlay,
    const uint32_t width, const uint32_t height,
    const uint32_t x, const uint32_t y,
    const String& url) {
    try {
        auto c = overlay;
        overlay = Overlay::Create(*window, width, height, x, y);
        overlay->view()->set_load_listener(this);
        overlay->view()->LoadURL(url);

        overlays_.push_back(overlay);

        spdlog::debug("[Vector Size {0}] Loaded URL: {1}", overlays_.size(), url.utf8().data());
    }
    catch (std::exception e) {
        spdlog::error("Overlay::Create : ", e.what());
        exit(EXIT_FAILURE);
    }

    return 0;
}

void Application::InitializePlatform() {
    Platform::instance().set_font_loader(GetPlatformFontLoader());
    Platform::instance().set_logger(GetDefaultLogger("Logs/Ultralight.log"));
    Platform::instance().set_file_system(GetPlatformFileSystem("."));
}

void Application::Run() {
    this->app_->Run();
}

void Application::OnDOMReady(View* caller, uint64_t frame_id, bool is_main_frame, const String& url) {
    Ref< JSContext > context = caller->LockJSContext();
    SetJSContext(context.get());
    JSObject global = JSGlobalObject();

    global["LoadAddonCounter"] = BindJSCallbackWithRetval(&Application::LoadAddonCounter);
}

JSValue Application::LoadAddonCounter(const JSObject& thisObj, const JSArgs& args) {
    spdlog::debug("Unloaded URL: {0}", this->GetTopURL().data());
    this->overlays_.pop_back();

    this->InitializeOverlay(this->window_, overlay_test, this->window_.get()->height(), this->window_.get()->width(), 0, 0, ui_menu_url);

    return JSValue();
}

Application::~Application() {
    this->window_ = nullptr;
    this->app_ = nullptr;

    while (!this->overlays_.empty()) {
        this->overlays_.pop_back();
    }
}