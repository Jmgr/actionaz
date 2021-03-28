/*
    Actiona
    Copyright (C) 2005 Jonathan Mercier-Ganady

    Actiona is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

    Actiona is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program. If not, see <http://www.gnu.org/licenses/>.

	Contact: jmgr@jmgr.info
*/

#include <QtGlobal>

#include "backend/backend.hpp"
#include "mainwindow.hpp"
#include "actiontools/actioninstance.hpp"
#include "actiontools/parameter.hpp"
#include "actiontools/subparameter.hpp"
#include "actiontools/actioninstancebuffer.hpp"
#include "global.hpp"
#include "progresssplashscreen.hpp"
#include "tools/languages.hpp"
#include "singleapplication/SingleApplication"

#ifdef ACT_PROFILE
#include "tools/highresolutiontimer.hpp"
#endif

#include <ctime>

#include <QDir>
#include <QDebug>
#include <QTextStream>
#include <QTextCodec>
#include <QElapsedTimer>
#include <QSettings>
#include <QCommandLineParser>

#ifdef Q_OS_UNIX
#undef signals
#include <libnotify/notify.h>
#define signals
#endif

static void cleanup()
{
#ifdef Q_OS_UNIX
	notify_uninit();
#endif
}

int main(int argc, char **argv)
{
#ifdef ACT_PROFILE
    Tools::HighResolutionTimer timer(QStringLiteral("Application run"));
#endif

    SingleApplication app(argc, argv);

	app.setQuitOnLastWindowClosed(false);

	app.setOrganizationName(QStringLiteral("Actiona"));
	app.setOrganizationDomain(QStringLiteral("actiona.tools"));
	app.setApplicationName(QStringLiteral("Actiona"));
	app.setApplicationVersion(Global::ACTIONA_VERSION.toString() + QStringLiteral(", script ") + Global::SCRIPT_VERSION.toString());

	qAddPostRoutine(cleanup);

    // Initialize the backend
    Backend::Instance::initialize();

	QCommandLineParser optionsParser;
	optionsParser.setApplicationDescription(QObject::tr("Emulates clics, key presses and other actions."));
	optionsParser.addHelpOption();
	optionsParser.addVersionOption();

	optionsParser.addOption({{QStringLiteral("p"), QStringLiteral("portable")}, QObject::tr("Starts in portable mode, storing the settings in the executable folder.")});

	optionsParser.parse(app.arguments());

	if(optionsParser.isSet(QStringLiteral("portable")))
	{
		QSettings::setPath(QSettings::IniFormat, QSettings::UserScope, QApplication::applicationDirPath() + QStringLiteral("/userSettings"));
		QSettings::setPath(QSettings::IniFormat, QSettings::SystemScope, QApplication::applicationDirPath() + QStringLiteral("/systemSettings"));
		QSettings::setDefaultFormat(QSettings::IniFormat);
	}

    QString locale = Tools::Languages::locale();

    Tools::Languages::installTranslator(QStringLiteral("qtbase"), locale);
    Tools::Languages::installTranslator(QStringLiteral("qtlocation"), locale);
    Tools::Languages::installTranslator(QStringLiteral("qtmultimedia"), locale);
    Tools::Languages::installTranslator(QStringLiteral("qtscript"), locale);
    Tools::Languages::installTranslator(QStringLiteral("qtxmlpatterns"), locale);
    Tools::Languages::installTranslator(QStringLiteral("tools"), locale);
    Tools::Languages::installTranslator(QStringLiteral("actiontools"), locale);
    Tools::Languages::installTranslator(QStringLiteral("executer"), locale);
    Tools::Languages::installTranslator(QStringLiteral("gui"), locale);

	optionsParser.addOptions(
	{
		{{QStringLiteral("s"), QStringLiteral("nosplash")}, QObject::tr("Disable the splash screen.")},
		{{QStringLiteral("t"), QStringLiteral("notrayicon")}, QObject::tr("Disable the tray icon.")},
		{{QStringLiteral("E"), QStringLiteral("noexecutionwindow")}, QObject::tr("Do not show the execution window.")},
		{{QStringLiteral("C"), QStringLiteral("noconsolewindow")}, QObject::tr("Do not show the console window.")},
		{{QStringLiteral("Q"), QStringLiteral("nocodeqt")}, QObject::tr("Do not include the Qt library into the code.")},
		{{QStringLiteral("e"), QStringLiteral("execute")}, QObject::tr("Execute the current script.")},
		{{QStringLiteral("x"), QStringLiteral("exitatend")}, QObject::tr("Close Actiona after execution - requires execute.")},
	});

	optionsParser.addPositionalArgument(QStringLiteral("script"), QObject::tr("The filepath of a script file to open/execute."), QStringLiteral("[script]"));

	optionsParser.process(app);

	if(optionsParser.isSet(QStringLiteral("exitatend")) && !optionsParser.isSet(QStringLiteral("execute")))
		optionsParser.showHelp(-1);

	QString startScript;
	const QStringList &positionalArguments = optionsParser.positionalArguments();
	if(positionalArguments.count() > 0)
	{
		startScript = positionalArguments.first();

		QFileInfo absoluteFileInfo(startScript);
		if(absoluteFileInfo.isFile() && absoluteFileInfo.isReadable())
		{
            if(app.sendMessage(absoluteFileInfo.absoluteFilePath().toUtf8()))
				return 0;
		}

		QFileInfo relativeFileInfo(QDir::current().filePath(startScript));
		if(relativeFileInfo.isFile() && relativeFileInfo.isReadable())
		{
            if(app.sendMessage(absoluteFileInfo.absoluteFilePath().toUtf8()))
				return 0;
		}
	 }

#ifdef Q_OS_UNIX
    notify_init("Actiona");
#endif

	app.addLibraryPath(QDir::currentPath() + QStringLiteral("/actions"));
	app.addLibraryPath(QDir::currentPath() + QStringLiteral("/plugins"));

	if(!optionsParser.isSet(QStringLiteral("nocodeqt")))
		app.addLibraryPath(QApplication::applicationDirPath() + QStringLiteral("/code"));

    //TODO: Move this in the constructor of these classes
    qRegisterMetaType<ActionTools::ActionInstance>("ActionInstance");
	qRegisterMetaType<ActionTools::ActionException::Exception>("Exception");
	qRegisterMetaType<ActionTools::Parameter>("Parameter");
	qRegisterMetaType<ActionTools::SubParameter>("SubParameter");
	qRegisterMetaType<ActionTools::ActionInstanceBuffer>("ActionInstanceBuffer");
	qRegisterMetaType<QVersionNumber>("Version");

	qRegisterMetaTypeStreamOperators<ActionTools::ActionInstance>("ActionInstance");
	qRegisterMetaTypeStreamOperators<ActionTools::Parameter>("Parameter");
	qRegisterMetaTypeStreamOperators<ActionTools::SubParameter>("SubParameter");
	qRegisterMetaTypeStreamOperators<ActionTools::ActionInstanceBuffer>("ActionInstanceBuffer");
	qRegisterMetaTypeStreamOperators<QVersionNumber>("Version");

	ProgressSplashScreen *splash = nullptr;
	if(!optionsParser.isSet(QStringLiteral("nosplash")) && !optionsParser.isSet(QStringLiteral("execute")))
	{
		splash = new ProgressSplashScreen(QPixmap(QStringLiteral(":/images/start.png")), Qt::WindowStaysOnTopHint);
		splash->setWindowOpacity(0);
		splash->show();
		app.processEvents();

		qreal splashScreenOpacity = 0;
		QElapsedTimer splashScreenFadeTime;

		splashScreenFadeTime.start();

		while(splashScreenOpacity < 1)
		{
			splashScreenOpacity = static_cast<qreal>(splashScreenFadeTime.elapsed()) * 0.003;

			if(splashScreenOpacity > 1)
				splashScreenOpacity = 1;

			splash->setWindowOpacity(splashScreenOpacity);
			splash->repaint();
		}
	}

	MainWindow mainWindow(optionsParser, splash, startScript, locale);

    QObject::connect(&app, &SingleApplication::receivedMessage, &mainWindow, &MainWindow::otherInstanceMessage);

	if(!optionsParser.isSet(QStringLiteral("execute")))
		mainWindow.show();

	return app.exec();
}
