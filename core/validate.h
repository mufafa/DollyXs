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
 * file : validate.h
 * date : 9.02.2023
 */
#pragma once

#include <QObject>

class Settings;
class Validate : public QObject
   {
   Q_OBJECT
public:
   explicit Validate(QObject *parent = nullptr);
   QString checkSelections();
   QString checkFolders();
signals:

private:
   QString validateHelper(bool isCheck, QString str);
   };

