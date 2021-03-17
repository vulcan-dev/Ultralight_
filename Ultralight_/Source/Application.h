#pragma once

#include <AppCore/App.h>
#include <AppCore/Window.h>
#include <AppCore/Overlay.h>
#include <AppCore/JSHelpers.h>
#include <AppCore/Platform.h>
#include <Ultralight/Ultralight.h>
#include <cstdint>
#include <string>
#include <spdlog/spdlog.h>
#include <vector>
#include <memory>

using namespace ultralight;

class Application : public ultralight::WindowListener, public ultralight::LoadListener, public ultralight::ViewListener {
public:    
    /// <summary>
    /// Initializes a new instance of the <see cref="Application"/> class.
    /// </summary>
    /// <param name="title">The title.</param>
    /// <param name="width">The width.</param>
    /// <param name="height">The height.</param>
    Application(const std::string title = "Application", const uint16_t& width = 440, const uint16_t& height = 340);
 
    /// <summary>
    /// Runs this instance.
    /// </summary>
    void Run();

    virtual ~Application();


    /// <summary>
    /// Initializes the overlay.
    /// </summary>
    /// <param name="window">The window.</param>
    /// <param name="overlay">The overlay.</param>
    /// <param name="width">The width.</param>
    /// <param name="height">The height.</param>
    /// <param name="x">The x.</param>
    /// <param name="y">The y.</param>
    /// <param name="url">The URL.</param>
    /// <returns></returns>
    int16_t InitializeOverlay(RefPtr<ultralight::Window>& window, RefPtr<ultralight::Overlay> overlay,
        const uint32_t width, const uint32_t height, 
        const uint32_t x, const uint32_t y, 
        const String& url);

    /// <summary>
    /// Gets the window.
    /// </summary>
    /// <returns></returns>
    RefPtr<ultralight::Window> GetWindow() { return this->window_; }
private:

    /// <summary>
    /// Initializes this instance.
    /// </summary>
    /// <returns></returns>
    int16_t Initialize();

    /// <summary>
    /// Initializes the window.
    /// </summary>
    /// <returns></returns>
    int16_t InitializeWindow();

    /// <summary>
    /// Initializes the platform.
    /// </summary>
    void InitializePlatform();

    /// <summary>
    /// Gets the top URL.
    /// </summary>
    /// <returns></returns>
    inline String8 GetTopURL() {
        return overlays_.at(0).get()->view().get().url().utf8().data();
    }

private:
    /* Overrides */
/// <summary>
/// Raises the Close event.
/// </summary>
/// <param name="window">The window.</param>
    inline virtual void OnClose(ultralight::Window* window) override {
        app_->Quit();
    }

    /// <summary>
    /// Called when [resize].
    /// </summary>
    /// <param name="window">The window.</param>
    /// <param name="width">The width.</param>
    /// <param name="height">The height.</param>
    inline virtual void OnResize(ultralight::Window* window, uint32_t width, uint32_t height) override {
        if (!this->overlays_.empty()) {
            for (size_t i = 0; i < overlays_.size(); i++) {
                overlays_.at(i).get()->Resize(width, height);
            }
        }
    }


    /// <summary>
    /// Called when [DOM ready].
    /// </summary>
    /// <param name="caller">The caller.</param>
    /// <param name="frame_id">The frame identifier.</param>
    /// <param name="is_main_frame">if set to <c>true</c> [is main frame].</param>
    /// <param name="url">The URL.</param>
    virtual void OnDOMReady(ultralight::View* caller, uint64_t frame_id, bool is_main_frame, const ultralight::String& url) override;

private:
	/// <summary>
	/// Gets the message.
	/// </summary>
	/// <param name="thisObj">The this object.</param>
	/// <param name="args">The arguments.</param>
	/// <returns></returns>
	inline const JSValue Application::GetMessage(const JSObject& thisObj, const JSArgs& args) const {
		return JSValue("Test");
	}

	/// <summary>
	/// Adds the counter.
	/// </summary>
	/// <param name="thisObj">The this object.</param>
	/// <param name="args">The arguments.</param>
	/// <returns></returns>
	inline JSValue Application::AddCounter(const JSObject& thisObj, const JSArgs& args) {
		this->counter_++;
		spdlog::debug("AddCounter Button pressed: Counter = {0}", this->counter_);

		return JSValue(this->counter_);
	}


    /// <summary>
    /// Loads the addon counter.
    /// </summary>
    /// <param name="thisObj">The this object.</param>
    /// <param name="args">The arguments.</param>
    /// <returns></returns>
    JSValue LoadAddonCounter(const JSObject& thisObj, const JSArgs& args);

private:
	ultralight::RefPtr<ultralight::App> app_;
	ultralight::RefPtr<ultralight::Window> window_;
	std::vector<RefPtr<Overlay>> overlays_;
	RefPtr<Overlay> overlay_test;

	int counter_ = 0;
	uint16_t width_, height_ = 0;
	std::string title_ = "";

	String8 ui_menu_url = "file:///Assets/addons/counter/counter.html";
};