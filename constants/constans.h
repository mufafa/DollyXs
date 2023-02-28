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
 * file : constans.h
 * date : 7.02.2023
 */
#pragma once

#include <QtCore>
#include <QString>



namespace Constants {
      Q_NAMESPACE
      enum Code {
	    Author,
	    LongName,

	    MacDebugTargetDir,
	    MacIdHeaderDir,
	    MacProjectDir,
	    MacReleaseTargetDir,
	    MacSdkDir,
	    MacSourceDir,
	    MacSpecificDebugXcconfigFile,
	    MacSpecificReleaseXcconfigFile,
	    MacSpecificXcconfigFile,
	    MacTargetType, //in intel based imac xcode should implement ONLY_ACTIVE_ARCH = YES;

	    PluginType,
	    PrefixId,
	    ProductType,
	    ShortName,

	    WinDebugIntermediateDir,
	    WinDebugIntermediateDirX64,
	    WinDebugTargetDir,
	    WinDebugTargetDirX64,
	    WinIdHeaderDir,
	    WinProjectDir,
	    WinReleaseIntermediateDir,
	    WinReleaseIntermediateDirX64,
	    WinReleaseTargetDir,
	    WinReleaseTargetDirX64,
	    WinSdkDir,
	    WinSourceDir,
	    WinSpecificCppRspFile,
	    WinSpecificOdfrcRspFile,
      };
      Q_ENUM_NS(Code)

      //in order to use matching xsl files 'code names'
      //eg  MacDebugTargetDir -> mac-debug-target-dir
      inline QString GetCodeXmlName(const QString& code) {
	    QString result = "";
	    for (int index = 0; index < code.length(); ++index) {
		  if (!code[index].isUpper() ){
		       result.append(code[index]);
		     } else {
			if (index != 0) result.append("-");
			result.append(code[index].toLower());
		     }
	      }
	    return result;
	 }
      //returns QString from enum valus name
      //eg  MacDebugTargetDir
      inline  QString GetCodeStringValue(Constants::Code value) {
	    QMetaEnum e =  QMetaEnum::fromType<Constants::Code>();
	    return e.key(value);
	 }

      //return QStringList from all code keys from enum
      inline QStringList GetAllCodeKeysString() {
	    QStringList keys;
	    QMetaEnum e =  QMetaEnum::fromType<Constants::Code>();
	    for (int i = 0; i < e.keyCount(); i++)
	       {
		  keys.append(e.key(i));
	       }
	    return keys;
	 }


      //https://gist.github.com/andrey-str/1795f40ee73e6e57b9a7
      inline void revealFile(QWidget* parent, const QString &pathToReveal) {

	    // See http://stackoverflow.com/questions/3490336/how-to-reveal-in-finder-or-show-in-explorer-with-qt
	    // for details

	    // Mac, Windows support folder or file.
	#if defined(Q_OS_WIN)
	    const QString explorer = Environment::systemEnvironment().searchInPath(QLatin1String("explorer.exe"));
	    if (explorer.isEmpty()) {
		QMessageBox::warning(parent,
				     tr("Launching Windows Explorer failed"),
				     tr("Could not find explorer.exe in path to launch Windows Explorer."));
		return;
	    }
	    QString param;
	    if (!QFileInfo(pathIn).isDir())
		param = QLatin1String("/select,");
	    param += QDir::toNativeSeparators(pathIn);
	    QString command = explorer + " " + param;
	    QString command = explorer + " " + param;
	    QProcess::startDetached(command);

	#elif defined(Q_OS_MAC)
	    Q_UNUSED(parent)
	    QStringList scriptArgs;
	    scriptArgs << QLatin1String("-e")
		    << QString::fromLatin1("tell application \"Finder\" to reveal POSIX file \"%1\"")
		    .arg(pathToReveal);
	    QProcess::execute(QLatin1String("/usr/bin/osascript"), scriptArgs);
	    scriptArgs.clear();
	    scriptArgs << QLatin1String("-e")
		    << QLatin1String("tell application \"Finder\" to activate");
	    QProcess::execute("/usr/bin/osascript", scriptArgs);
	#else
	    // we cannot select a file here, because no file browser really supports it...
	    const QFileInfo fileInfo(pathIn);
	    const QString folder = fileInfo.absoluteFilePath();
	    const QString app = Utils::UnixUtils::fileBrowser(Core::ICore::instance()->settings());
	    QProcess browserProc;
	    const QString browserArgs = Utils::UnixUtils::substituteFileBrowserParameters(app, folder);
	    if (debug)
		qDebug() <<  browserArgs;
	    bool success = browserProc.startDetached(browserArgs);
	    const QString error = QString::fromLocal8Bit(browserProc.readAllStandardError());
	    success = success && error.isEmpty();
	    if (!success)
		showGraphicalShellError(parent, app, error);
	#endif

	}


      inline void ChangeFileContent(const QString& fileName, const QString& old = "", const QString& replace="") {

	    QFileInfo myinfo(fileName);
	    auto destDir = myinfo.path();

	    //backup source prjsettings file
	    QString tmpFile = destDir + "/tmp.txt";
	    bool rn = QFile::rename(fileName, tmpFile);
	    if (!rn) {
		  qDebug() << "tmp prj file not copied";
		  return;
	       }

	    QFile source(destDir + "/tmp.txt");



	    source.open(QIODevice::ReadOnly);
	    QFile destination(fileName);
	    destination.open(QIODevice::WriteOnly);
	    QByteArray buffer;
	    int chunksize = 2000;
	    while(!(buffer = source.read(chunksize)).isEmpty()){
		buffer.replace(old.toUtf8(), replace.toUtf8()); //string
		destination.write(buffer);
	    }
	    destination.close();
	    source.close();
	    QFile::remove(tmpFile);
	 }



   }
