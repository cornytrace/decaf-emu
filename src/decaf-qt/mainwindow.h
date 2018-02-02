#pragma once

#include "libdecaf/decaf.h"
#include "decaf-sdl/decafsdl_graphics.h"
#include <QtWidgets/QMainWindow>
#include <QThread>
#include "ui_mainwindow.h"

using namespace decaf::input;

class MainWindow : public QMainWindow, public decaf::InputDriver, public decaf::EventListener
{
   Q_OBJECT

public:
   MainWindow(QWidget *parent = Q_NULLPTR);

protected slots:
   void actOpen();
   void actExit();

   // VPAD
   virtual vpad::Type
   getControllerType(vpad::Channel channel) override;

   virtual ButtonStatus
   getButtonStatus(vpad::Channel channel, vpad::Core button) override;

   virtual float
   getAxisValue(vpad::Channel channel, vpad::CoreAxis axis) override;

   virtual bool
   getTouchPosition(vpad::Channel channel, vpad::TouchPosition &position) override;

   // WPAD
   virtual wpad::Type
   getControllerType(wpad::Channel channel) override;

   virtual ButtonStatus
   getButtonStatus(wpad::Channel channel, wpad::Core button) override;

   virtual ButtonStatus
   getButtonStatus(wpad::Channel channel, wpad::Classic button) override;

   virtual ButtonStatus
   getButtonStatus(wpad::Channel channel, wpad::Nunchuck button) override;

   virtual ButtonStatus
   getButtonStatus(wpad::Channel channel, wpad::Pro button) override;

   virtual float
   getAxisValue(wpad::Channel channel, wpad::NunchuckAxis axis) override;

   virtual float
   getAxisValue(wpad::Channel channel, wpad::ProAxis axis) override;

   // Events
   virtual void
   onGameLoaded(const decaf::GameInfo &info) override;

private:
   bool run(const std::string gamePath);
   bool initializeSDL();
   Ui::MainWindowClass ui;
   DecafSDLGraphics *mainsdl;
   DecafSDLGraphics *drcsdl;
};
