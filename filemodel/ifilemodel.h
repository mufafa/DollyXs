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
 * file : ifilemodel.h
 * date : 6.02.2023
 */
#pragma once

#include <QObject>

class IFileModel : public QObject
   {
   Q_OBJECT
public:
   explicit IFileModel(QString path, QObject *parent = nullptr);
   QString getRelativePathByGivenPath(QString givenPath);
   bool checkDirExists();
signals:

protected:
   QString m_Path;
};


class RootFolder : public IFileModel {
   public:
      explicit RootFolder(QString path, QObject *parent = nullptr);
   bool checkXSFolder();
   QString getRootPath();
   QString getXsPath();
   QString getRelativeSDKPath(QString path);
};


