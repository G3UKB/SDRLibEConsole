/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic startup code for a JUCE application.

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "MainComponent.h"
#include "Common/extern.h"
#include "Common/gui_cache.h"
#include "Properties/properties.h"
#include "Properties/prop_cache.h"
#include "RadioInterface/radio_interface.h"
#include "RadioWindows/radio_component.h"
#include "RadioWindows/radio_window.cpp"

//==============================================================================
class JuceConsoleApplication  : public JUCEApplication
{
public:
    //==============================================================================
    JuceConsoleApplication() {}

    const String getApplicationName() override       { return ProjectInfo::projectName; }
    const String getApplicationVersion() override    { return ProjectInfo::versionString; }
    bool moreThanOneInstanceAllowed() override       { return false; }

    //==============================================================================
    void initialise (const String& commandLine) override
    {
        // Application initialisation
		
		// Restore all properties
		PropCache::getInstance()->restore_all();

		// Create main window
        mainWindow.reset (new MainWindow (getApplicationName()));
	
		// Make wisdom file if not already made
		c_server_make_wisdom("E:/Projects/SDRLibEConsole/trunk/src/cpp/wisdom");
		
		// Initialise server
		if (!c_server_init()) {
			printf("Failed to initialise server!");
			return;
		}
		c_server_set_num_rx(3);
		if (!RadioInterface::getInstance()->ri_set_default_audio()) return;
		if (!RadioInterface::getInstance()->ri_radio_discover()) return;
		if (!RadioInterface::getInstance()->ri_server_start()) return;

		MainComponent* c = (MainComponent*)GUICache::getInstance()->getMainInst();
		c->start_ui();
		//MainComponent* c = mainWindow->get_component();
		//mainWindow->get_component()->start_ui();

		// Start radio-1 instance
		RadioWindow *w1 = new RadioWindow("radio-1");
		w1->get_component()->start_ui();
		RadioWindow *w2 = new RadioWindow("radio-2");
		w2->get_component()->start_ui();
		RadioWindow *w3 = new RadioWindow("radio-3");
		w3->get_component()->start_ui();
    }

    void shutdown() override
    {
        // Add your application's shutdown code here..

        mainWindow = nullptr; // (deletes our window)
    }

    //==============================================================================
    void systemRequestedQuit() override
    {
        // This is called when the app is being asked to quit: you can ignore this
        // request and let the app carry on running, or call quit() to allow the app to close.
        quit();
    }

    void anotherInstanceStarted (const String& commandLine) override
    {
        // When another instance of the app is launched while this one is running,
        // this method is invoked, and the commandLine parameter tells you what
        // the other instance's command-line arguments were.
    }

    //==============================================================================
    /*
        This class implements the desktop window that contains an instance of
        our MainComponent class.
    */
    class MainWindow    : public DocumentWindow
    {
    public:
        MainWindow (String name)  : DocumentWindow (name,
                                                    Desktop::getInstance().getDefaultLookAndFeel()
                                                                          .findColour (ResizableWindow::backgroundColourId),
                                                    DocumentWindow::allButtons)
        {
            setUsingNativeTitleBar (true);
			c = new MainComponent();
            setContentOwned (new MainComponent(), true);

           #if JUCE_IOS || JUCE_ANDROID
            setFullScreen (true);
           #else
            setResizable (true, true);
            centreWithSize (getWidth(), getHeight());
           #endif

            setVisible (true);
        }

        void closeButtonPressed() override
        {
            // This is called when the user tries to close this window. Here, we'll just
            // ask the app to quit when this happens, but you can change this to do
            // whatever you need.
            JUCEApplication::getInstance()->systemRequestedQuit();
        }

        /* Note: Be careful if you override any DocumentWindow methods - the base
           class uses a lot of them, so by overriding you might break its functionality.
           It's best to do all your work in your content component instead, but if
           you really have to override any DocumentWindow methods, make sure your
           subclass also calls the superclass's method.
        */
		MainComponent *get_component() {
			return c;
		}

    private:
		MainComponent *c;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainWindow)
    };

private:
    std::unique_ptr<MainWindow> mainWindow;
};

//==============================================================================
// This macro generates the main() routine that launches the app.
START_JUCE_APPLICATION (JuceConsoleApplication)
