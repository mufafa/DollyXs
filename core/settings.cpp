/*
 * This file is part of the XXX distribution (https://github.com/xxxx or http://xxx.github.io).
 * Copyright (c) 2023 Mustafa Oksuz.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 * file : settings.cpp
 * date : 7.02.2023
 */
#include "settings.h"
#include "constants/constans.h"

#include <QDir>
#include <QSettings>
#include <QApplication>
#include <QFont>
#include <QFontDatabase>
#include <QtCore/QStandardPaths>

using namespace Constants;

static Settings* s_settingsInstance = 0;
static const char* KEY = "DollyXs";
static QSettings* createQSettings(const QString& key);

QString Settings::getConfigPath()
{
    // * OSX: ~/Library/Preferences/DollyXs
    return QString("%1/%2").arg(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation),
				QCoreApplication::applicationName());
}


QString Settings::getDataPath()
{
      return QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
}

QString Settings::getCachePath()
{
   return QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
}

static void s_deleteSettings()
{
    delete s_settingsInstance;
}



Settings* Settings::instance()
{
    if (!s_settingsInstance) {
	s_settingsInstance = new Settings();
	s_settingsInstance->load();
	qAddPostRoutine(s_deleteSettings); // musti delete later olayı
    }

    return s_settingsInstance;
}

Settings::Settings() :
  isAddPlatformArchPreference(true),
  isChangeSDKVariablePreference(true),
  isSkipWinProjectOnMac(true)
{}

void Settings::setValue(Constants::Code code, QString value) {
      m_codes[code] = value;
   }

QString Settings::getValue(Constants::Code code) {
      return m_codes[code].toString();
   }

void Settings::load()
   {

      QSettings* settings = createQSettings(KEY);
      if (!settings)
	 return;


    QString key = QString::fromLatin1(KEY) + QLatin1String("/App/");
    sdkRootFolder = settings->value(key + "SdkRootFolder").toString();
    sdkXsFolder = settings->value(key + "SdkXsFolder").toString();
    isAddPlatformArchPreference = settings->value(key + "AddPlatformArch").toBool();
    isChangeSDKVariablePreference = settings->value(key + "ChangeSDKVariable").toBool();
//    isSkipWinProjectOnMac = settings->value(key + "SkipWinProjectOnMac").toBool();

    key = KEY + QLatin1String("/Common/");
    m_codes[LongName] = settings->value(key + GetCodeStringValue(LongName));
    m_codes[ShortName] = settings->value(key + GetCodeStringValue(ShortName));
    m_codes[PrefixId] = settings->value(key + GetCodeStringValue(PrefixId));
    m_codes[Author] = settings->value(key + GetCodeStringValue(Author));

    //featuresvalue
    m_codes[ProductType] = settings->value(key + GetCodeStringValue(ProductType));
    m_codes[PluginType] = settings->value(key + GetCodeStringValue(PluginType));

    //features check
    generateDialog   =	settings->value(key + "GenerateDialog").toBool();
    generatePanel = settings->value(key + "GeneratePanel").toBool();
    generateScriptingProvider  =	 settings->value(key + "GenerateScriptingSupport").toBool();
    generateSelectionSuite =   settings->value(key + "GenerateSelectionSuite").toBool();
    generateFacade   =	settings->value(key + "GenerateFacade").toBool();
    generatePreflight	=   settings->value(key + "GeneratePreflight").toBool();


    key = KEY + QLatin1String("/Mac/");
    m_codes[MacProjectDir] = settings->value(key + GetCodeStringValue(MacProjectDir));
    m_codes[MacSourceDir] = settings->value(key + GetCodeStringValue(MacSourceDir));
    m_codes[MacIdHeaderDir] = settings->value(key + GetCodeStringValue(MacIdHeaderDir));
    m_codes[MacSdkDir] = settings->value(key + GetCodeStringValue(MacSdkDir));
    m_codes[MacDebugTargetDir] = settings->value(key + GetCodeStringValue(MacDebugTargetDir));
    m_codes[MacReleaseTargetDir] = settings->value(key + GetCodeStringValue(MacReleaseTargetDir));

    macSharedXcConfigCheck = settings->value(key + "MacSharedXcConfigCheck").toBool();
    m_codes[MacTargetType] = settings->value(key + GetCodeStringValue(MacTargetType));
    m_codes[MacSpecificXcconfigFile] = settings->value(key + GetCodeStringValue(MacSpecificXcconfigFile));
    m_codes[MacSpecificDebugXcconfigFile] = settings->value(key + GetCodeStringValue(MacSpecificDebugXcconfigFile));
    m_codes[MacSpecificReleaseXcconfigFile] = settings->value(key + GetCodeStringValue(MacSpecificReleaseXcconfigFile));

    //windows
    key = KEY + QLatin1String("/Window/");
    m_codes[WinDebugIntermediateDir] = settings->value(key + GetCodeStringValue(WinDebugIntermediateDir));
    m_codes[WinDebugIntermediateDirX64] = settings->value(key + GetCodeStringValue(WinDebugIntermediateDirX64));
    m_codes[WinDebugTargetDir] = settings->value(key + GetCodeStringValue(WinDebugTargetDir));
    m_codes[WinDebugTargetDirX64] = settings->value(key + GetCodeStringValue(WinDebugTargetDirX64));
    m_codes[WinIdHeaderDir] = settings->value(key + GetCodeStringValue(WinIdHeaderDir));
    m_codes[WinProjectDir] = settings->value(key + GetCodeStringValue(WinProjectDir));
    m_codes[WinReleaseIntermediateDir] = settings->value(key + GetCodeStringValue(WinReleaseIntermediateDir));
    m_codes[WinReleaseIntermediateDirX64] = settings->value(key + GetCodeStringValue(WinReleaseIntermediateDirX64));
    m_codes[WinReleaseTargetDir] = settings->value(key + GetCodeStringValue(WinReleaseTargetDir));
    m_codes[WinReleaseTargetDirX64] = settings->value(key + GetCodeStringValue(WinReleaseTargetDirX64));
    m_codes[WinSdkDir] = settings->value(key + GetCodeStringValue(WinSdkDir));
    m_codes[WinSourceDir] = settings->value(key + GetCodeStringValue(WinSourceDir));
    m_codes[WinSpecificCppRspFile] = settings->value(key + GetCodeStringValue(WinSpecificCppRspFile));
    m_codes[WinSpecificOdfrcRspFile] = settings->value(key + GetCodeStringValue(WinSpecificOdfrcRspFile));

    delete settings;
}

void Settings::save()
{

    QSettings* settings = createQSettings(KEY);
    if (!settings)
	return;

   QString key = KEY + QLatin1String("/App/");
   settings->setValue(key + "SdkRootFolder", sdkRootFolder);
   settings->setValue(key + "SdkXsFolder", sdkXsFolder);
   settings->setValue(key + "AddPlatformArch", isAddPlatformArchPreference);
   settings->setValue(key + "ChangeSDKVariable", isChangeSDKVariablePreference);
   settings->setValue(key + "SkipWinProjectOnMac", isSkipWinProjectOnMac);

   key = KEY + QLatin1String("/Common/");
   settings->setValue(key + GetCodeStringValue(LongName),  m_codes[LongName].toString());
   settings->setValue(key + GetCodeStringValue(ShortName),  m_codes[ShortName].toString());
   settings->setValue(key + GetCodeStringValue(PrefixId),  m_codes[PrefixId].toString());
   settings->setValue(key + GetCodeStringValue(Author),  m_codes[Author].toString());

   //features
   settings->setValue(key + GetCodeStringValue(ProductType),  m_codes[ProductType].toString());
   settings->setValue(key + GetCodeStringValue(PluginType),  m_codes[PluginType].toString());
   //features check
   settings->setValue(key + "GenerateDialog", generateDialog);
   settings->setValue(key + "GeneratePanel", generatePanel);
   settings->setValue(key + "GenerateScriptingSupport", generateScriptingProvider);
   settings->setValue(key + "GenerateSelectionSuite", generateSelectionSuite);
   settings->setValue(key + "GenerateFacade", generateFacade);
   settings->setValue(key + "GeneratePreflight", generatePreflight);

   key = KEY + QLatin1String("/Mac/");
   //mac
   settings->setValue(key + GetCodeStringValue(MacProjectDir),  m_codes[MacProjectDir].toString());
   settings->setValue(key + GetCodeStringValue(MacSourceDir),  m_codes[MacSourceDir].toString());
   settings->setValue(key + GetCodeStringValue(MacIdHeaderDir),  m_codes[MacIdHeaderDir].toString());
   settings->setValue(key + GetCodeStringValue(MacSdkDir),  m_codes[MacSdkDir].toString());
   settings->setValue(key + GetCodeStringValue(MacDebugTargetDir),  m_codes[MacDebugTargetDir].toString());
   settings->setValue(key + GetCodeStringValue(MacReleaseTargetDir),  m_codes[MacReleaseTargetDir].toString());


   settings->setValue(key + GetCodeStringValue(MacTargetType),  m_codes[MacTargetType].toString());
   settings->setValue(key + "MacSharedXcConfigCheck", macSharedXcConfigCheck);
   settings->setValue(key + GetCodeStringValue(MacSpecificXcconfigFile),  m_codes[MacSpecificXcconfigFile].toString());
   settings->setValue(key + GetCodeStringValue(MacSpecificDebugXcconfigFile),  m_codes[MacSpecificDebugXcconfigFile].toString());
   settings->setValue(key + GetCodeStringValue(MacSpecificReleaseXcconfigFile),  m_codes[MacSpecificReleaseXcconfigFile].toString());

   //windows
   key = KEY + QLatin1String("/Window/");
   settings->setValue(key + GetCodeStringValue(WinDebugIntermediateDir),  m_codes[WinDebugIntermediateDir].toString());
   settings->setValue(key + GetCodeStringValue(WinDebugIntermediateDirX64),  m_codes[WinDebugIntermediateDirX64].toString());
   settings->setValue(key + GetCodeStringValue(WinDebugTargetDir),  m_codes[WinDebugTargetDir].toString());
   settings->setValue(key + GetCodeStringValue(WinDebugTargetDirX64),  m_codes[WinDebugTargetDirX64].toString());
   settings->setValue(key + GetCodeStringValue(WinIdHeaderDir),  m_codes[WinIdHeaderDir].toString());
   settings->setValue(key + GetCodeStringValue(WinProjectDir),  m_codes[WinProjectDir].toString());
   settings->setValue(key + GetCodeStringValue(WinReleaseIntermediateDir),  m_codes[WinReleaseIntermediateDir].toString());
   settings->setValue(key + GetCodeStringValue(WinReleaseIntermediateDirX64),  m_codes[WinDebugIntermediateDirX64].toString());
   settings->setValue(key + GetCodeStringValue(WinReleaseTargetDir),  m_codes[WinReleaseTargetDir].toString());
   settings->setValue(key + GetCodeStringValue(WinReleaseTargetDirX64),  m_codes[WinReleaseTargetDirX64].toString());
   settings->setValue(key + GetCodeStringValue(WinSdkDir),  m_codes[WinSdkDir].toString());
   settings->setValue(key + GetCodeStringValue(WinSourceDir),  m_codes[WinSourceDir].toString());
   settings->setValue(key + GetCodeStringValue(WinSpecificCppRspFile),  m_codes[WinSpecificCppRspFile].toString());
   settings->setValue(key + GetCodeStringValue(WinSpecificOdfrcRspFile),  m_codes[WinSpecificOdfrcRspFile].toString());

   delete settings;
   }


QSettings* createQSettings(const QString& KEY)
{
    QSettings* settings = new QSettings(Settings::getConfigPath() + "/" + KEY + ".ini", QSettings::IniFormat);
    return settings;
}
