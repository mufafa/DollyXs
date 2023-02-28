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
 * file : application.cpp
 * date : 7.02.2023
 */
#include "application.h"


#include <QStyleFactory>

Application::Application(int &argc, char **argv) : QApplication(argc, argv){

//      this->setStyle(QStyleFactory::create("Fusion"));
      this->connect(this, &Application::lastWindowClosed, this, &Application::quit);
  };
