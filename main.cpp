#include <iostream>
#include <memory>
#include <windows.h>
#include "SDL.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h"
#include "Elements.h"
#include "Filesystem.h"
#include "EventManager.h"
#include "logger.h"
#include "date.h"
#include "config.h"
#include "app.h"
#include "Debug.h"

int main(int argc, char *argv[])
{
	Logger logger(App::APP_NAME + ".error.log");
	try
	{
		Config config(App::CONFIG_FILENAME);
		FileTagManager app(&logger, &config);
		app.initSDL();
		app.initResize();
		app.initElements();
		::ShowWindow(::GetConsoleWindow(), SW_MINIMIZE);
		SDL_StartTextInput();
		while (app.loop())
		{
			SDL_Delay(1);
		}
		SDL_StopTextInput();
		app.quitSDL();
		if (logger.getErrorCount() > 0)
		{
			logger.writeLog();
		}
		return 0;
	}
	catch (const std::exception &e)
	{
		logger.addLog(typeid(e).name() + std::string(" ") + (std::string)e.what());
		logger.writeLog();
		throw e;
	}
}