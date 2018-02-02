#include <QFileDialog>
#include <QWindow>
#include <glbinding/Binding.h>
#include <glbinding/Meta.h>

#include <common\log.h>
#include <decaf-sdl\decafsdl_opengl.h>
#include <decaf-sdl\decafsdl_vulkan.h>
#include <SDL_syswm.h>

#include <decaf-sdl\config.h>
#include "mainwindow.h"

auto gCliLog = gLog;

MainWindow::MainWindow(QWidget *parent)
   : QMainWindow(parent)
{
   ui.setupUi(this);
}


static std::string
getPathBasename(const std::string &path)
{
   auto pos = path.find_last_of("/\\");

   if (!pos) {
      return path;
   } else {
      return path.substr(pos + 1);
   }
}

bool MainWindow::run(const std::string gamePath)
{
    auto shouldQuit = false;

    if (!initializeSDL())
    {
        return false;
    }

    //auto debugUiRenderer = mGraphicsDriver->getDecafDebugUiRenderer();
    //decaf::setDebugUiRenderer(debugUiRenderer);

    // Set input provider
    decaf::setInputDriver(this);
    decaf::addEventListener(this);
    //openInputDevices();

    // Set sound driver
    //decaf::setSoundDriver(mSoundDriver);

    decaf::setClipboardTextCallbacks(
        [](void *context) -> const char * {
        return SDL_GetClipboardText();
    },
        [](void *context, const char *text) {
        SDL_SetClipboardText(text);
    });

    // Initialise emulator
    if (!decaf::initialise(gamePath)) {
        return false;
    }

    // Start emulator
    decaf::start();
    while (!shouldQuit && !decaf::hasExited()) {
        QCoreApplication::processEvents();

        /*if (mVpad0Controller) {
            SDL_GameControllerUpdate();
        }*/

        SDL_Event event;

        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_WINDOWEVENT:
                if (event.window.event == SDL_WINDOWEVENT_CLOSE) {
                    shouldQuit = true;
                }

                break;
            case SDL_MOUSEBUTTONDOWN:
                //decaf::injectMouseButtonInput(translateMouseButton(event.button.button), decaf::input::MouseAction::Press);
                break;
            case SDL_MOUSEBUTTONUP:
                //decaf::injectMouseButtonInput(translateMouseButton(event.button.button), decaf::input::MouseAction::Release);
                break;
            case SDL_MOUSEWHEEL:
                decaf::injectScrollInput(static_cast<float>(event.wheel.x), static_cast<float>(event.wheel.y));
                break;
            case SDL_MOUSEMOTION:
                decaf::injectMousePos(static_cast<float>(event.motion.x), static_cast<float>(event.motion.y));
                break;
            case SDL_KEYDOWN:
                //decaf::injectKeyInput(translateKeyCode(event.key.keysym), decaf::input::KeyboardAction::Press);
                break;
            case SDL_KEYUP:
                if (event.key.keysym.sym == SDLK_TAB) {
                    //mToggleDRC = !mToggleDRC;
                }

                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    shouldQuit = true;
                }

                //decaf::injectKeyInput(translateKeyCode(event.key.keysym), decaf::input::KeyboardAction::Release);
                break;
            case SDL_TEXTINPUT:
                decaf::injectTextInput(event.text.text);
                break;
            case SDL_QUIT:
                shouldQuit = true;
                break;
            }
        }

        //Viewport tvViewport, drcViewport;
        //calculateScreenViewports(tvViewport, drcViewport);
        //mGraphicsDriver->renderFrame(tvViewport, drcViewport);
    }

    // Shut down decaf
    decaf::shutdown();

    return true;
}

bool MainWindow::initializeSDL()
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER) != 0) {
        gLog->error("Failed to initialize SDL: {}", SDL_GetError());
        return false;
    }

    if (config::display::backend == "opengl") {
        mainsdl = new DecafSDLOpenGL();
        drcsdl = new DecafSDLOpenGL();
    }
    else if (config::display::backend == "vulkan") {
        mainsdl = new DecafSDLVulkan();
        drcsdl = new DecafSDLVulkan();
    }

    if (!mainsdl->initialise(1280, 720)) {
        gLog->error("Failed to create main graphics window");
        return false;
    }

    if (!drcsdl->initialise(854, 480)) {
        gLog->error("Failed to create gamepad graphics window");
        return false;
    }

    SDL_SysWMinfo systemInfo;
    SDL_VERSION(&systemInfo.version);
    SDL_GetWindowWMInfo(mainsdl->getWindow(), &systemInfo);
    auto mainhandle = systemInfo.info.win.window;
    SDL_GetWindowWMInfo(drcsdl->getWindow(), &systemInfo);
    auto drchandle = systemInfo.info.win.window;

    ui.dockWidgetContents->createWindowContainer(QWindow::fromWinId((WId)mainhandle));
    ui.dockWidgetDRCContents->createWindowContainer(QWindow::fromWinId((WId)drchandle));

    return true;
}

void MainWindow::actOpen()
{
   auto gamePath = QFileDialog::getOpenFileName(this, tr("Open File"), QString(), tr("Wii U Executable (*.rpx)"));

   if (gamePath.isEmpty()) {
      return;
   }

   // Initialise libdecaf logger
   auto logFile = getPathBasename(gamePath.toStdString());
   decaf::initialiseLogging(logFile);

   // Start emulator
   run(gamePath.toStdString());
}

void MainWindow::actExit()
{
    close();
}

// VPAD
vpad::Type
MainWindow::getControllerType(vpad::Channel channel)
{
   return vpad::Type::DRC;
}

ButtonStatus
MainWindow::getButtonStatus(vpad::Channel channel, vpad::Core button)
{
   return ButtonStatus::ButtonReleased;
}

float
MainWindow::getAxisValue(vpad::Channel channel, vpad::CoreAxis axis)
{
   return 0.0f;
}

bool
MainWindow::getTouchPosition(vpad::Channel channel, vpad::TouchPosition &position)
{
   return false;
}

// WPAD
wpad::Type
MainWindow::getControllerType(wpad::Channel channel)
{
   return wpad::Type::Disconnected;
}

ButtonStatus
MainWindow::getButtonStatus(wpad::Channel channel, wpad::Core button)
{
   return ButtonStatus::ButtonReleased;
}

ButtonStatus
MainWindow::getButtonStatus(wpad::Channel channel, wpad::Classic button)
{
   return ButtonStatus::ButtonReleased;
}

ButtonStatus
MainWindow::getButtonStatus(wpad::Channel channel, wpad::Nunchuck button)
{
   return ButtonStatus::ButtonReleased;
}

ButtonStatus
MainWindow::getButtonStatus(wpad::Channel channel, wpad::Pro button)
{
   return ButtonStatus::ButtonReleased;
}

float
MainWindow::getAxisValue(wpad::Channel channel, wpad::NunchuckAxis axis)
{
   return 0.0f;
}

float
MainWindow::getAxisValue(wpad::Channel channel, wpad::ProAxis axis)
{
   return 0.0f;
}

// Events
void
MainWindow::onGameLoaded(const decaf::GameInfo &info)
{
}
