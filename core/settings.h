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
 * file : settings.h
 * date : 7.02.2023
 */

#pragma once
#include <QObject>
#include <QString>
#include <QHash>
#include <QMetaEnum>
#include "constants/constans.h"

class Settings : public QObject {
   Q_OBJECT
   typedef QHash<Constants::Code, QVariant> CodeVariables;
   CodeVariables m_codes;
public:
    static Settings* instance();
    static QString getConfigPath();
    static QString getDataPath();
    static QString getCachePath();

    void load();
    void save();

#pragma mark AppCode
    QString sdkRootFolder;
    QString sdkXsFolder;
    //
    bool isAddPlatformArchPreference;
    bool isChangeSDKVariablePreference;
    bool isSkipWinProjectOnMac;

#pragma mark MacTab
    bool    macSharedXcConfigCheck;

 #pragma mark WinTab
    //FIXME : will be implemented

 #pragma mark FeaturesTab
    bool    generateDialog;
    bool    generatePanel;
    bool    generateScriptingProvider;
    bool    generateSelectionSuite;
    bool    generateFacade;
    bool    generatePreflight;

 #pragma mark input_files
    QString dollyMainFile;
    //the main file that saxon will use to convert xml
    QString platformInputFile;

signals:

private:
    explicit Settings();
    Q_DISABLE_COPY(Settings)
    QString m_name;

public:
    void setValue(Constants::Code code, QString value);
    QString getValue(Constants::Code code);

   };
