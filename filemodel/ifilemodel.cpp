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
 * file : ifilemodel.cpp
 * date : 6.02.2023
 */
#include "ifilemodel.h"

#include <QDir>


IFileModel::IFileModel(QString path, QObject *parent) : QObject(parent), m_Path(path)
   {

   }

QString IFileModel::getRelativePathByGivenPath(QString givenPath)
   {
      QDir dir(m_Path);
      QString s = dir.relativeFilePath(givenPath);
      return s;
   }

bool IFileModel::checkDirExists()
   {
      QDir dir(m_Path);
      return dir.exists();
   }


#pragma mark RootFolder

RootFolder::RootFolder(QString path, QObject *parent) : IFileModel(path, parent)
   {}

bool RootFolder::checkXSFolder()
   {
      auto xs = getXsPath();
      QDir xsFolder(xs);
      return xsFolder.exists();
   }

QString RootFolder::getRootPath()
   {
      return m_Path;
   }

QString RootFolder::getXsPath()
   {
      QDir xsFolder(m_Path + "/devtools/dolly/xs");
      return xsFolder.absolutePath();
   }

QString RootFolder::getRelativeSDKPath(QString path)
   {
      QDir root(m_Path);
      auto relativeDir = root.relativeFilePath(path);
      return relativeDir;
   }


