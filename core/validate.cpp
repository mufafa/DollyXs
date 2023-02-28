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
 * file : validate.cpp
 * date : 9.02.2023
 */
#include "validate.h"
#include "constants/fileutility.h"
#include "core/settings.h"

#include <QRegularExpression>

const static QRegularExpression WHITE_SPACE("\\s");
const static QRegularExpression MAC_PATH_SEPERATOR("\\\\");
const static QRegularExpression HEX_DECIMAL("^0x[0-9a-fA-F]+");

using namespace Constants;

Validate::Validate(QObject *parent)
   : QObject{parent}
   {

   }

QString Validate::validateHelper(bool isCheck, QString str)
   {
      return isCheck ? ("<br>" + str + "<br>") : "";
   }


QString Validate::checkSelections()
   {

      std::function<QString(void)> error = []() -> QString {
	   QString bold("<b style=\"color:red\">");
	   bold.append("Error :");
	   bold.append("</b>");
	   return bold;
	 };

      std::function<QString(const QString& str)> italic = [](const QString& str) -> QString {
	   QString italic = QString("<i style=\"color:LightSlateGray\">%1</i>").arg(str);
	   return italic;
	 };

      auto m_settings = Settings::instance();


      QString errorString = "";
      int shortNameLength = m_settings->getValue(ShortName).length();

      QString str = QString("%1 Attribute '%2' needs to be between 1-23 characters.").arg(error(), italic("short name"));
      str = str + "<br>The short name is used in source and target file names.<br>";
      str = str + "A shortname of > 23 characters will not build properly on the Mac!";

      errorString = errorString + validateHelper((shortNameLength > 23 || shortNameLength < 1), str);
      errorString = errorString + validateHelper(m_settings->getValue(ShortName).contains(WHITE_SPACE), "Error: The short name must not contain whitespace. It's used in code!");

      str = QString("%1 attribute 'long-name' must not contain whitespace.").arg(error());
      errorString = errorString + validateHelper(m_settings->getValue(LongName).contains(WHITE_SPACE), str);

      str = QString("%1 attribute '%2' must be a hexadecimal. Check if you have entered your assigned ID correctly.").arg(error(), italic("prefix-id"));
      errorString = errorString + validateHelper(!m_settings->getValue(PrefixId).contains(HEX_DECIMAL), str);

      str = QString("%1 attribute '%2' of element 'Code' uses the wrong path separator. ").arg(error(), italic("mac-debug-target-dir"));
      str = str + "Please use '/' path separator.";
      errorString = errorString + validateHelper(m_settings->getValue(MacDebugTargetDir).contains(MAC_PATH_SEPERATOR), str);

      str = QString("%1 attribute '%2' of element 'Code' uses the wrong path separator. ").arg(error(), italic("mac-release-target-dir"));
      str = str + "Please use '/' path separator.";
      errorString = errorString + validateHelper(m_settings->getValue(MacReleaseTargetDir).contains(MAC_PATH_SEPERATOR), str);

      str = QString("%1 attribute '%2' of element 'Code' uses the wrong path separator. ").arg(error(), italic("mac-id-header-dir"));
      str = str + "Please use '/' path separator.";
      errorString = errorString + validateHelper(m_settings->getValue(MacIdHeaderDir).contains(MAC_PATH_SEPERATOR), str);


      str = QString("%1 attribute '%2' of element 'Code' uses the wrong path separator. ").arg(error(), italic("mac-sdk-dir"));
      str = str + "Please use '/' path separator.";
      errorString = errorString + validateHelper(m_settings->getValue(MacSdkDir).contains(MAC_PATH_SEPERATOR), str);

      str = QString("%1 attribute '%2' of element 'Code' uses the wrong path separator. ").arg(error(), italic("mac-source-dir"));
      str = str + "Please use '/' path separator.";
      errorString = errorString + validateHelper(m_settings->getValue(MacSourceDir).contains(MAC_PATH_SEPERATOR), str);

      str = QString("%1 attribute '%2' should have value. ").arg(error(), italic("mac-project-dir"));
      str = str + "Please select a project folder";
      errorString = errorString + validateHelper(m_settings->getValue(MacProjectDir).isEmpty(), str);

      str = QString("%1 attribute '%2' of element 'Code' uses the wrong path separator. ").arg(error(), italic("mac-project-dir"));
      str = str + "Please use '/' path separator.";
      errorString = errorString + validateHelper(m_settings->getValue(MacProjectDir).contains(MAC_PATH_SEPERATOR), str);



      if (FileUtility::GetOSString() == "mac") {
	    //win project
	  } else {
	       //windows os
      }
      if (m_settings->macSharedXcConfigCheck) {
	      errorString = errorString + validateHelper(m_settings->getValue(MacSpecificXcconfigFile) == "", "Please specify an XCConfig file or uncheck the shared XCConfig file checkbox!");
	      errorString = errorString + validateHelper(m_settings->getValue(MacSpecificDebugXcconfigFile) == "", "Please specify an DebugXCConfig file or uncheck the shared XCConfig file checkbox!");
	      errorString = errorString + validateHelper(m_settings->getValue(MacSpecificReleaseXcconfigFile) == "", "Please specify an ReleaseXCConfig file or uncheck the shared XCConfig file checkbox!");
      }
      //TODO : shared config checker seçili olmadığı ve main, debug, release boş olduğunda hata vermesi gerekiyor

      return errorString;
   }

QString Validate::checkFolders()
   {

      std::function<QString(const QString& str)> info= [](const QString& str) -> QString {
	   QString info = QString("<b><i style=\"color:Aqua\">%1</i></b>").arg(str);
	   return info;
	 };

      auto m_settings = Settings::instance();
      QString sdkFolder = m_settings->sdkRootFolder;
      QString errorString = "";

      QString str = QString("%1 Indesign SDK Folder not exists").arg(info("Error : "));
      errorString += errorString + validateHelper(m_settings->sdkRootFolder.isEmpty(), str);

      QString xsFolder = sdkFolder + "/devtools/dolly/xs";
      str = QString("%1 xsl transform files should be in '%2'").arg(info("Error : "), info("&lt;SDK&gt;/devtools/dolly/xs"));
      errorString += validateHelper(!QDir(xsFolder).exists(), str);

      return errorString;
   }

