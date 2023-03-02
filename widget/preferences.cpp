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
 * file : preferences.cpp
 * date : 27.02.2023
 */
#include "preferences.h"
#include "ui_preferences.h"

#include <core/settings.h>

Preferences::Preferences(QWidget *parent) :
   QDialog(parent),
   ui(new Ui::Preferences)
   {
      setWindowFlag(Qt::Tool, true);
      ui->setupUi(this);
      m_settings = Settings::instance();
      initUI();
      createConnections();
   }


Preferences::~Preferences()
   {
      delete ui;
   }

void Preferences::initUI()
   {
      ui->onlyActiveArchCheck->setChecked(m_settings->isAddPlatformArchPreference);
      ui->sdkRootToAbsolute->setChecked(m_settings->isChangeSDKVariablePreference);
      ui->skipWinProject->setChecked(m_settings->isSkipWinProjectOnMac);
   }

void Preferences::createConnections()
   {

      connect(ui->okButton, &QPushButton::clicked, this, [&](){
	    this->close();
	 });

      connect(ui->initApplication, &QToolButton::clicked, this, [&](){
	 QDir(m_settings->getCachePath()).removeRecursively();
	 QDir(m_settings->getConfigPath()).removeRecursively();
	 QDir(m_settings->getConfigPath()).removeRecursively();
	 });

      connect(ui->onlyActiveArchCheck, &QCheckBox::clicked, this, [&](bool checked){
	    m_settings->isAddPlatformArchPreference = checked;
	 });
      connect(ui->sdkRootToAbsolute, &QCheckBox::clicked, this, [&](bool checked){
	    m_settings->isChangeSDKVariablePreference = checked;
	 });
      connect(ui->skipWinProject, &QCheckBox::clicked, this, [&](bool checked){
	    m_settings->isSkipWinProjectOnMac = checked;
	 });
   }
