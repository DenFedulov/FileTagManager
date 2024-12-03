#include <iostream>
#include <windows.h>
#include "SDL.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h"
#include "logger.h"
#include "config.h"
#include "CommonObjects.h"
#include "App.h"
#include "SDL_Helpers.h"
#include "AppDB.h"
#pragma comment(lib,"user32.lib") 

int main(int argc, char *argv[])
{
	CommonObjects comm;
	Logger logger(G_App::APP_NAME + ".error.log");
	comm.logger = &logger;
	try
	{
		initSDL(&logger, &comm);
		Config config(G_App::CONFIG_FILENAME);
		comm.config = &config;
		AppDB db(G_App::DB_FILENAME);
		comm.db = &db;
		FileTagManager app(&comm);
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
	}
	return 0;
}