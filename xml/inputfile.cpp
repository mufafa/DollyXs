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
 * file : inputfile.cpp
 * date : 14.02.2023
 */
#include "inputfile.h"
#include "constants/fileutility.h"
#include "core/settings.h"
#include "elementbuilder.h"


#include <QDomDocument>
#include <QDir>
#include <QFileInfo>

using namespace Constants;

static const char* PROJ_FILES[] {
#ifdef WIN_TAB
      "vcxproj-file",
      "vcxproj-filters-file",
      "DebugWin32-vsprops-file",
      "ReleaseWin32-vsprops-file",
      "DebugX64-vsprops-file",
      "ReleaseX64-vsprops-file",
      "rsp-cpp-file",
      "rsp-odfrc-file",
#endif
      "xcodeproj-file",
      "xcconfig-plugin-file",
      "xcconfig-plugin-debug-file",
      "xcconfig-plugin-release-file",
      "id-h-file",
      "id-cpp-file",
      "nostrip-file",
      "triggerresourcedeps-file",
      "factorylist-file",
      "rc-file",
      "fr-file",
      "enus-fr-file",
      "jajp-fr-file",
   };

InputFile::InputFile(QObject *parent)
   : QObject{parent}
   {
   }

InputFile::~InputFile()
   {
   }


bool InputFile::createDynamicMainFile()
   {
      auto m_setting = Settings::instance();

      auto dataPath = m_setting->getDataPath();
      if (!QDir(dataPath).exists()) {
	    QDir().mkdir(dataPath);
	 }
      auto dollyDynamicMainFile = dataPath + "/" + "dollyxs-dynamic-main.xsl";


      QDomDocument document;
      QDomElement styleSheetElement = document.createElement("xsl:stylesheet");
      styleSheetElement.setAttribute("xmlns:xsl", "http://www.w3.org/1999/XSL/Transform");
      styleSheetElement.setAttribute("version", "1.1");
      document.appendChild(styleSheetElement);


      QDomElement preserveSpaceElem = document.createElement("xsl:preserve-space");
      preserveSpaceElem.setAttribute("elements", "text");

      QDomElement outputElem = document.createElement("xsl:output");
      outputElem.setAttribute("method", "text");
      outputElem.setAttribute("indent", "no");

      QDomElement templateElem = document.createElement("xsl:template");
      templateElem.setAttribute("match", "/");


      QDomElement applyTemplatesElem = document.createElement("xsl:apply-templates");
      styleSheetElement.appendChild(preserveSpaceElem);
      styleSheetElement.appendChild(templateElem);
      templateElem.appendChild(applyTemplatesElem);

      QDir xsFolder = m_setting->sdkXsFolder;
      QStringList fileList = xsFolder.entryList();
      for(auto &xslFile : fileList) {
	  if(!isInvalidName(xslFile)) {
	      QDomElement includeElem = document.createElement("xsl:include");
	      includeElem.setAttribute("href", xsFolder.absolutePath() + "/" + xslFile);
	      styleSheetElement.appendChild(includeElem);
	  }
      }
      styleSheetElement.appendChild(outputElem);
      this->SaveXMLDocument(document,dollyDynamicMainFile);

      m_setting->dollyMainFile = dollyDynamicMainFile;
      return true;


   }

bool InputFile::isInvalidName(QString xslFile)
   {
      return (xslFile.isEmpty() || xslFile.length() == 0 ||
	      xslFile.startsWith("dollyxs-dynamic-main") ||
	      xslFile.startsWith("global-variables") ||
	      !xslFile.endsWith(".xsl") );
   }

void InputFile::createInputFile()
   {

      auto m_setting = Settings::instance();
      QString platformInputFile = FileUtility::GetOSString() + "-input.xml";


      QDomDocument document;
      ElementBuilder elementBuilder(&document);
      QDomElement codeElement = elementBuilder.create("code");
      document.appendChild(codeElement);

      addEscapes(codeElement);

      for (auto file : PROJ_FILES) {
	    codeElement.appendChild(elementBuilder.create(file));
      }

      bool isOnlyModelPlugin = m_setting->getValue(PluginType) == "kModelPlugIn" ? true : false;

      if(!isOnlyModelPlugin) {
		  codeElement.appendChild(elementBuilder.create("generate-menu"));
		  codeElement.appendChild(elementBuilder.create("impl-item", { "impl-id", "impl-name", "impl-file" },
								 { "k" + m_setting->getValue(ShortName) + "ActionComponentImpl", m_setting->getValue(ShortName) + "ActionComponent", "actioncomponent-impl-file" }));


	    if(m_setting->generatePanel) {
		  codeElement.appendChild(elementBuilder.create("generate-panel"));
	       }


	    if(m_setting->generateDialog) {
		codeElement.appendChild(elementBuilder.create("generate-dialog"));
		codeElement.appendChild(elementBuilder.create("impl-item", { "impl-id", "impl-name", "impl-file" },  { "k" + m_setting->getValue(ShortName) + "DialogControllerImpl", m_setting->getValue(ShortName) + "DialogController", "dialogcontroller-impl-file" }));
		codeElement.appendChild(elementBuilder.create("impl-item", { "impl-id", "impl-name", "impl-file" },  { "k" + m_setting->getValue(ShortName) + "DialogObserverImpl", m_setting->getValue(ShortName) + "DialogObserver", "dialogobserver-impl-file" }));

	    }
	 }

      if(m_setting->generateScriptingProvider) {
	    codeElement.appendChild(elementBuilder.create("generate-scriptprovider"));
		    codeElement.appendChild(elementBuilder.create("scriptingdefs-h-file"));
		    codeElement.appendChild(elementBuilder.create("impl-item",  { "impl-id", "impl-name", "impl-file" },  { "k" + m_setting->getValue(ShortName) + "ScriptProviderImpl", m_setting->getValue(ShortName) + "ScriptProvider", "scriptprovider-impl-file" }));
		    QDomElement bossItemElement = elementBuilder.create("boss-item",  { "boss-id", "boss-parent" }, { "k" + m_setting->getValue(ShortName) + "ScriptProviderBoss", "kBaseScriptProviderBoss" });
		    codeElement.appendChild(elementBuilder.create("boss-impl-pair", { "interface-id", "impl-id" }, { "IID_ISCRIPTPROVIDER", "k" + m_setting->getValue(ShortName) + "ScriptProviderImpl" }));
		    codeElement.appendChild(bossItemElement);
	 }


      if(m_setting->generateFacade) {

	    codeElement.appendChild(elementBuilder.create("generate-facade"));
		    codeElement.appendChild(elementBuilder.create("facade-h-file", { "header-file" },  { "I" + m_setting->getValue(ShortName) + "Facade.h" }));
		    codeElement.appendChild(elementBuilder.create("impl-item", { "impl-id", "impl-name", "impl-file" }, { "k" + m_setting->getValue(ShortName) + "FacadeImpl", m_setting->getValue(ShortName) + "Facade", "facade-impl-file" }));
		    QDomElement interfaceItemElement = elementBuilder.create("interface-item",  { "interface-id" }, { "IID_I" + m_setting->getValue(ShortName).toUpper() + "FACADE" });
		    codeElement.appendChild(interfaceItemElement);
		    QDomElement bossAddInElement = elementBuilder.create("boss-addin", QStringList{ "boss-id" },  QStringList{ "kUtilsBoss" });
		    bossAddInElement.appendChild(elementBuilder.create("boss-impl-pair", { "interface-id", "impl-id" }, { "IID_I" + m_setting->getValue(ShortName).toUpper() + "FACADE", "k" + m_setting->getValue(ShortName) + "FacadeImpl" }));
		    codeElement.appendChild(bossAddInElement);
	 }

      if(m_setting->generateSelectionSuite) {
	    codeElement.appendChild(elementBuilder.create("generate-selectionsuite"));
		    codeElement.appendChild(elementBuilder.create("selectionsuite-h-file",  { "header-file" },  { "I" + m_setting->getValue(ShortName) + "Suite.h" }));
		    codeElement.appendChild(elementBuilder.create("selectionsuite-csb-h-file",  { "header-file" },  { m_setting->getValue(ShortName) + "SuiteCSB.h" }));
		    codeElement.appendChild(elementBuilder.create("impl-item",  { "impl-id", "impl-name", "impl-file" },  { "k" + m_setting->getValue(ShortName) + "SuiteASBImpl", m_setting->getValue(ShortName) + "SuiteASB", "selectionsuite-asb-impl-file" }));
		    codeElement.appendChild(elementBuilder.create("impl-item",  { "impl-name", "impl-file" },  { m_setting->getValue(ShortName) + "SuiteCSB", "selectionsuite-csb-impl-file" }));
		    codeElement.appendChild(elementBuilder.create("impl-item",  { "impl-id", "impl-name", "impl-file" },  { "k" + m_setting->getValue(ShortName) + "SuiteLayoutCSBImpl", m_setting->getValue(ShortName) + "SuiteLayoutCSB", "selectionsuite-layoutcsb-impl-file" }));
		    codeElement.appendChild(elementBuilder.create("impl-item",  { "impl-id", "impl-name", "impl-file" },  { "k" + m_setting->getValue(ShortName) + "SuiteTextCSBImpl", m_setting->getValue(ShortName) + "SuiteTextCSB", "selectionsuite-textcsb-impl-file" }));
		    codeElement.appendChild(elementBuilder.create("impl-item",  { "impl-id", "impl-name", "impl-file" },  { "k" + m_setting->getValue(ShortName) + "SuiteDefaultsCSBImpl", m_setting->getValue(ShortName) + "SuiteDefaultsCSB", "selectionsuite-defaultscsb-impl-file" }));
		    QDomElement interFaceItemElement = elementBuilder.create("interface-item",  { "interface-id" },  { "IID_I" + m_setting->getValue(ShortName).toUpper() + "SUITE" });
		    codeElement.appendChild(interFaceItemElement);
		    QDomElement integratorSuiteBoss = elementBuilder.create("boss-addin",  "boss-id",  "kIntegratorSuiteBoss");
		    integratorSuiteBoss.appendChild(elementBuilder.create("boss-impl-pair",  { "interface-id", "impl-id" },  { "IID_I" + m_setting->getValue(ShortName).toUpper() + "SUITE", "k" + m_setting->getValue(ShortName) + "SuiteASBImpl" }));
		    codeElement.appendChild(integratorSuiteBoss);
		    QDomElement textSuiteBoss = elementBuilder.create("boss-addin", "boss-id", "kTextSuiteBoss");
		    textSuiteBoss.appendChild(elementBuilder.create("boss-impl-pair",  { "interface-id", "impl-id" },  { "IID_I" + m_setting->getValue(ShortName).toUpper() + "SUITE", "k" + m_setting->getValue(ShortName) + "SuiteTextCSBImpl" }));
		    codeElement.appendChild(textSuiteBoss);
		    QDomElement layoutSuiteBoss = elementBuilder.create("boss-addin",  "boss-id", "kLayoutSuiteBoss");
		    layoutSuiteBoss.appendChild(elementBuilder.create("boss-impl-pair",  { "interface-id", "impl-id" },  { "IID_I" + m_setting->getValue(ShortName).toUpper() + "SUITE", "k" + m_setting->getValue(ShortName) + "SuiteLayoutCSBImpl" }));
		    codeElement.appendChild(layoutSuiteBoss);
		    QDomElement applicationDefaultSuiteBoss = elementBuilder.create("boss-addin", "boss-id", "kApplicationDefaultSuiteBoss");
		    applicationDefaultSuiteBoss.appendChild(elementBuilder.create("boss-impl-pair",  { "interface-id", "impl-id" },  { "IID_I" + m_setting->getValue(ShortName).toUpper() + "SUITE", "k" + m_setting->getValue(ShortName) + "SuiteDefaultsCSBImpl" }));
		    codeElement.appendChild(applicationDefaultSuiteBoss);
		    QDomElement documentDefaultSuiteBoss = elementBuilder.create("boss-addin", "boss-id", "kDocumentDefaultSuiteBoss");
		    documentDefaultSuiteBoss.appendChild(elementBuilder.create("boss-impl-pair",  { "interface-id", "impl-id" },  { "IID_I" + m_setting->getValue(ShortName).toUpper() + "SUITE", "k" + m_setting->getValue(ShortName) + "SuiteDefaultsCSBImpl" }));
		    codeElement.appendChild(documentDefaultSuiteBoss );
	 }

      if(m_setting->generatePreflight) {
	      codeElement.appendChild(elementBuilder.create("generate-preflight"));
	      codeElement.appendChild(elementBuilder.create("preflight-const-h-file", { "header-file" }, { m_setting->getValue(ShortName) + "PreflightConsts.h" }));
	      codeElement.appendChild(elementBuilder.create("preflight-ruletable-h-file", { "header-file" }, { m_setting->getValue(ShortName) + "RuleTable.h" }));
	      codeElement.appendChild(elementBuilder.create("impl-item", { "impl-id", "impl-name", "impl-file" }, { "k" + m_setting->getValue(ShortName) + "PreflightRuleServiceImpl", m_setting->getValue(ShortName) + "PreflightRuleService", "preflight-ruleservice-impl-file" }));
	      codeElement.appendChild(elementBuilder.create("impl-item", { "impl-id", "impl-name", "impl-file" }, { "k" + m_setting->getValue(ShortName) + "PreflightRuleUIImpl", m_setting->getValue(ShortName) + "PreflightRuleUI", "preflight-ruleui-impl-file" }));
	      codeElement.appendChild(elementBuilder.create("impl-item", { "impl-id", "impl-name", "impl-file" }, { "k" + m_setting->getValue(ShortName) + "PreflightRuleVisitorImpl", m_setting->getValue(ShortName) + "PreflightRuleVisitor", "preflight-rulevisitor-impl-file" }));
	      QDomElement preflightRuleBoss = elementBuilder.create("boss-item", { "boss-id", "boss-parent" }, { "k" + m_setting->getValue(ShortName) + "PreflightRuleBoss", "kPreflightRuleBaseBoss" });
	      preflightRuleBoss.appendChild(elementBuilder.create("boss-impl-pair", { "interface-id", "impl-id" }, { "IID_IPREFLIGHTRULEVISITOR", "k" + m_setting->getValue(ShortName) + "PreflightRuleVisitorImpl" }));
	      preflightRuleBoss.appendChild(elementBuilder.create("boss-impl-pair", { "interface-id", "impl-id" }, { "IID_IPREFLIGHTRULEUI", "k" + m_setting->getValue(ShortName) + "PreflightRuleUIImpl" }));
	      preflightRuleBoss.appendChild(elementBuilder.create("boss-impl-pair", { "interface-id", "impl-id" }, { "IID_IPREFLIGHTRULEDATA", "kPreflightRuleDataImpl" }));
	      preflightRuleBoss.appendChild(elementBuilder.create("boss-impl-pair", { "interface-id", "impl-id" }, { "IID_IPREFLIGHTRULEUTILITIES", "kPreflightRuleUtilitiesImpl" }));
	      preflightRuleBoss.appendChild(elementBuilder.create("boss-impl-pair", { "interface-id", "impl-id" }, { "IID_IPMPERSIST", "kPMPersistImpl" }));
	      codeElement.appendChild(preflightRuleBoss);
	      QDomElement preflightRuleSPBoss = elementBuilder.create("boss-item", { "boss-id", "boss-parent" }, { "k" + m_setting->getValue(ShortName) + "PreflightRuleSPBoss", "kInvalidClass" });
	      preflightRuleSPBoss.appendChild(elementBuilder.create("boss-impl-pair", { "interface-id", "impl-id" }, { "IID_IK2SERVICEPROVIDER", "kPreflightRuleSPImpl" }));
	      preflightRuleSPBoss.appendChild(elementBuilder.create("boss-impl-pair", { "interface-id", "impl-id" }, { "IID_IPREFLIGHTRULESERVICE", "k" + m_setting->getValue(ShortName) + "PreflightRuleServiceImpl" }));
	      codeElement.appendChild(preflightRuleSPBoss);
	 }

      auto dataPath = m_setting->getDataPath();
      if (!QDir(dataPath).exists()) {
	    QDir().mkdir(dataPath);
	 }

      auto inputFile = dataPath + "/" + platformInputFile;

      this->SaveXMLDocument(document,inputFile);
      m_setting->platformInputFile = inputFile;


   }

void InputFile::SaveXMLDocument(const QDomDocument &document, QString targetFile)
   {
      QFile file(targetFile);
      if( !file.open( QIODevice::WriteOnly | QIODevice::Text ) )
      {
	 qDebug( "Failed to open file for writing." );
	 return;
      }
      QTextStream stream( &file);
      stream << document.toString(5);
      file.close();
   }

void InputFile::addEscapes(QDomElement &element)
   {
      auto m_settings = Settings::instance();
      QString osType = FileUtility::GetOSString();
      QString projectFolder = osType == "mac" ?
	       m_settings->getValue(MacProjectDir) : m_settings->getValue(WinProjectDir);
      QStringList keySet = GetAllCodeKeysString();

      for (int index = 0; index < keySet.length(); ++index) {
	    QString seperator = QDir::separator();
	    auto v = static_cast<Code>(index);
	    QString value = m_settings->getValue(v);
	    auto key = keySet[index];
	    auto keyName = GetCodeXmlName(key);
	    if (value.isEmpty()) continue;

	    element.setAttribute(keyName, value);

	    bool osTypeDirMatches = QRegularExpression("^" + osType + ".*-dir$").match(keyName).hasMatch();
	    bool projectDirMatches = QRegularExpression(".*project-dir$").match(keyName).hasMatch();


	    QString fixedValue = value;
	    if(osTypeDirMatches && !projectDirMatches) {
			    QString absolutePath;
			    QString pathWithoutOsType = keyName.mid(osType.length()+1);

			    QFileInfo file(value);
			    if(file.isAbsolute()) {
				absolutePath = value;
			    } else {
				QString absolutePathFromRelative = projectFolder + seperator;
				absolutePathFromRelative =  absolutePathFromRelative + value;
				QFileInfo file1(absolutePathFromRelative);
				//canonicalFilePath file dahil path getiriyor
				absolutePath = file1.absoluteFilePath();
			    }
			    fixedValue = absolutePath;
			    element.setAttribute(pathWithoutOsType+"-absolute", absolutePath);
			}

	    QString fixedDir = fixedValue.replace("\\ ", "\\\\ ");
	    QString escapedKey = keyName + "-escaped";
	    element.setAttribute(escapedKey, fixedDir);
	 }
   }
