#include "mainwindow.h"
#include "constants/fileutility.h"
#include "widget/overlayprogress.h"
#include "core/validate.h"
#include "ui_mainwindow.h"

#include <QFileInfo>
#include <QTemporaryFile>
#include <QFileDialog>
#include <QException>
#include <QMessageBox>
#include <QDirIterator>
#include <QStandardPaths>
#include <QSettings>
#include <QButtonGroup>
#include <QDebug>


#include "transform/saxon.h"

#include "core/settings.h"
#include "xml/inputfile.h"
#include "widget/alert.h"
#include "widget/preferences.h"

#include <filemodel/ifilemodel.h>


using namespace Constants;

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_settings = Settings::instance();
    prefs = new Preferences(this);
    createUi();
    applySettings();
}

#pragma mark NewVersion
void MainWindow::createUi()
   {
      if(m_settings->isSkipWinProjectOnMac) {
	    //win tab will remove due to not implamented
	    ui->winTab->setEnabled(false);
	    ui->winTab->setVisible(false);
	    ui->tabWidget->removeTab(3); //removes wintab
	 }

      //shorname not longer than 23 char and no space
      QRegularExpression shortNameRx("[a-z-A-Z]{1,23}");
      QValidator *shortNameValidator = new QRegularExpressionValidator(shortNameRx, this);
      ui->shortNameTF->setValidator(shortNameValidator);

      QRegularExpression prefixrx("^0x[0-9a-fA-F]+");
      QValidator *prefixValidator = new QRegularExpressionValidator(prefixrx, this);
      ui->prefixIDTF->setValidator(prefixValidator);

      //longname not contain white space
      QRegularExpression longNameRx("[a-z-A-Z]{1,}");
      QValidator *longNameValidator = new QRegularExpressionValidator(longNameRx, this);
      ui->longNameTF->setValidator(longNameValidator);

      createConnections();
      createMenuActions();
   }


void MainWindow::applySettings()
   {

      setSdkRootFolder(m_settings->sdkRootFolder);
      ui->indesignSdkFolder->setText(m_settings->sdkRootFolder);

      ui->longNameTF->setText(m_settings->getValue(LongName));
      ui->shortNameTF->setText(m_settings->getValue(ShortName));
      ui->prefixIDTF->setText(m_settings->getValue(PrefixId));
      ui->authorTF->setText(m_settings->getValue(Author));

      ui->macProjectDir->setText(m_settings->getValue(MacProjectDir));
      ui->macSourceDir->setText(m_settings->getValue(MacSourceDir));
      ui->macHeaderDir->setText(m_settings->getValue(MacIdHeaderDir));
      ui->macSDKDir->setText(m_settings->getValue(MacSdkDir));
      ui->macDebugTargetDir->setText(m_settings->getValue(MacDebugTargetDir));
      ui->macReleaseTargetDir->setText(m_settings->getValue(MacReleaseTargetDir));

      ui->macTargetTypeCombo->setCurrentText(m_settings->getValue(MacTargetType));

      ui->macSharedConfigCheck->setChecked(m_settings->macSharedXcConfigCheck);

      QString macXCConfigFile = m_settings->getValue(MacSpecificXcconfigFile);
      QString macSpecificDebugXcconfigFile = m_settings->getValue(MacSpecificDebugXcconfigFile);
      QString macSpecificReleaseXcconfigFile = m_settings->getValue(MacSpecificReleaseXcconfigFile);

      //default xcconfig files
      if (macXCConfigFile.isEmpty()) {
	    ui->macMainXCConfigDir->setText("_shared_build_settings/plugin.sdk.xcconfig");
	 } else {
	 ui->macMainXCConfigDir->setText(macXCConfigFile);
	 }
      if (macSpecificDebugXcconfigFile.isEmpty()) {
	    ui->macDebugXCConfigDir->setText("_shared_build_settings/cocoa_ui_x64_debug.xcconfig");
	 } else {
	    ui->macDebugXCConfigDir->setText(macSpecificDebugXcconfigFile);

	 }
      if (macSpecificReleaseXcconfigFile.isEmpty()) {
		  ui->macReleaseXCConfigDir->setText("_shared_build_settings/cocoa_ui_x64_release.xcconfig");
	 } else {
	    ui->macReleaseXCConfigDir->setText(macSpecificReleaseXcconfigFile);
	 }

      ui->productTypeCombo->setCurrentText(m_settings->getValue(ProductType));
      ui->plugInTypeCombo->setCurrentText(m_settings->getValue(PluginType));


      ui->generateDialogCheck->setChecked(m_settings->generateDialog);
      ui->generatePanelCheck->setChecked(m_settings->generatePanel);
      ui->generateScriptingSupportCheck->setChecked(m_settings->generateScriptingProvider);
      ui->generateSelectionSuiteCheck->setChecked(m_settings->generateSelectionSuite);
      ui->generateFacadeCheck->setChecked(m_settings->generateFacade);
      ui->generatePreflightCheck->setChecked(m_settings->generatePreflight);

   }

void MainWindow::saveSettings()
   {
      m_settings->sdkRootFolder = ui->indesignSdkFolder->text();
      m_settings->sdkXsFolder = ui->xsFolder->text();

      m_settings->setValue(LongName, ui->longNameTF->text());
      m_settings->setValue(ShortName, ui->shortNameTF->text());
      m_settings->setValue(Author, ui->authorTF->text());
      m_settings->setValue(PrefixId, ui->prefixIDTF->text());

      m_settings->setValue(MacProjectDir, ui->macProjectDir->text());
      m_settings->setValue(MacSourceDir, ui->macSourceDir->text());
      m_settings->setValue(MacIdHeaderDir, ui->macHeaderDir->text());
      m_settings->setValue(MacSdkDir, ui->macSDKDir->text());
      m_settings->setValue(MacDebugTargetDir, ui->macDebugTargetDir->text());
      m_settings->setValue(MacReleaseTargetDir, ui->macReleaseTargetDir->text());

      m_settings->setValue(MacTargetType, ui->macTargetTypeCombo->currentText());

      m_settings->macSharedXcConfigCheck = ui->macSharedConfigCheck->isChecked();

      m_settings->setValue(MacSpecificXcconfigFile, ui->macMainXCConfigDir->text());
      m_settings->setValue(MacSpecificDebugXcconfigFile, ui->macDebugXCConfigDir->text());
      m_settings->setValue(MacSpecificReleaseXcconfigFile, ui->macReleaseXCConfigDir->text());

      m_settings->setValue(ProductType, ui->productTypeCombo->currentText());
      m_settings->setValue(PluginType, ui->plugInTypeCombo->currentText());

      m_settings->generateDialog = ui->generateDialogCheck->isChecked();
      m_settings->generatePanel = ui->generatePanelCheck->isChecked();
      m_settings->generateScriptingProvider = ui->generateScriptingSupportCheck->isChecked();
      m_settings->generateSelectionSuite = ui->generateSelectionSuiteCheck->isChecked();
      m_settings->generateFacade = ui->generateFacadeCheck->isChecked();
      m_settings->generatePreflight = ui->generatePreflightCheck->isChecked();

      m_settings->save();
   }

void MainWindow::createMenuActions()
   {
      connect(ui->actionLoadDefault, &QAction::triggered, this, [&](){
	       this->loadDefaultSettings();
	 });
      connect(ui->actionSaveDefaults, &QAction::triggered, this, [&](){
	       this->saveSettings();
	 });

      connect(ui->actionShowPreferences, &QAction::triggered, this, [&](){
	 prefs->exec();
	 });
#if defined(Q_OS_MAC)
      //??
#endif
   }

void MainWindow::createConnections()
   {

      connect(ui->cancelButton, &QPushButton::clicked, this, &QApplication::quit);
      connect(ui->okButton, &QPushButton::released, this, [&](){
	    //TODO: xs folder ve diğer dosyaları bakmak gerekiyor
	    saveSettings();
	    Validate v;
	    Alert *alert = nullptr;
	    QString folderErr = v.checkFolders();
	    if(!folderErr.isEmpty()) {
		  alert = new Alert("Folder Error", folderErr, this);
		  alert->exec();
		  return;
	       }
	    QString selectionErr = v.checkSelections();
	    if (!selectionErr.isEmpty()) {
		  alert = new Alert("Configure Error", selectionErr, this);
		  alert->exec();
		  return;
	    }


	    InputFile inputFile;
	    inputFile.createDynamicMainFile();
	    inputFile.createInputFile();

	    transform();

	 });

      ///shortname will be added end of source/header path
      connect(ui->shortNameTF, &QLineEdit::textEdited, this, &MainWindow::shortNameChanged);

      connect(ui->indesignSdkFolderChooser, &QToolButton::clicked,this, [&](){
	    auto directory = QFileDialog::getExistingDirectory(this, "Choose Dir", m_settings->sdkRootFolder);
	    if (!directory.isEmpty()) {
		  ui->indesignSdkFolder->setText(directory);
		  setSdkRootFolder(directory);
	       }
      });

      //plugin tab
      connect(ui->longNameTF, &QLineEdit::textEdited, this, [&](const QString){
	    m_settings->setValue(LongName, ui->longNameTF->text());
	 });
      connect(ui->shortNameTF, &QLineEdit::textEdited, this, [&](const QString){
	    m_settings->setValue(ShortName, ui->shortNameTF->text());
	 });
      connect(ui->authorTF, &QLineEdit::textEdited, this, [&](const QString){
	    m_settings->setValue(Constants::Author, ui->authorTF->text());
	 });
      connect(ui->prefixIDTF, &QLineEdit::textEdited, this, [&](const QString){
	    m_settings->setValue(Constants::PrefixId, ui->prefixIDTF->text());
	 });

      //mac tab buttons
      QButtonGroup *macButtonGroup = new QButtonGroup(this);
      macButtonGroup->addButton(ui->macProjectDirChooser);
      macButtonGroup->addButton(ui->macSourceDirChooser);
      macButtonGroup->addButton(ui->macHeaderDirChooser);
      macButtonGroup->addButton(ui->macSDKDirChooser);
      macButtonGroup->addButton(ui->macDebugTargetDirChooser);
      macButtonGroup->addButton(ui->macReleaseTargetDirChooser);

      connect(macButtonGroup, &QButtonGroup::buttonClicked, this, [&](QAbstractButton *button){
	    auto buttonName = button->objectName();
	    buttonName.truncate(buttonName.length()-QString("Chooser").length());
	    auto *targetLabel = this->findChild<QLineEdit*>(buttonName);

	   QString preselected;
	   bool isRelative = false;

	   if(buttonName == "macProjectDir") {
	       preselected = m_settings->getValue(MacProjectDir);
	    } else if (buttonName == "macSourceDir") {

		 preselected = FileUtility::GetAbsolutePathFromRelativePath(m_settings->getValue(MacSourceDir), m_settings->getValue(MacProjectDir));
		 isRelative = true;
	    } else if (buttonName == "macHeaderDir") {
		 preselected = FileUtility::GetAbsolutePathFromRelativePath(m_settings->getValue(MacIdHeaderDir), m_settings->getValue(MacProjectDir));
		 isRelative = true;
	    } else if (buttonName == "macSDKDir") {
		 preselected = FileUtility::GetAbsolutePathFromRelativePath(m_settings->getValue(MacSdkDir), m_settings->getValue(MacProjectDir));
		 isRelative = true;
	    } else if (buttonName == "macDebugTargetDir") {
		 preselected = FileUtility::GetAbsolutePathFromRelativePath(m_settings->getValue(MacDebugTargetDir), m_settings->getValue(MacProjectDir));
		 isRelative = true;
	    } else if (buttonName == "macReleaseTargetDir") {
		 preselected = FileUtility::GetAbsolutePathFromRelativePath(m_settings->getValue(MacReleaseTargetDir), m_settings->getValue(MacProjectDir));
		 isRelative = true;
	    }
	   else {
	      preselected = m_settings->sdkRootFolder;
	   }
	    qDebug() << preselected;
	    auto directory = QFileDialog::getExistingDirectory(this, "Choose Dir", preselected);
	    if (!directory.isEmpty()) {
		  QString dir = isRelative ? FileUtility::GetRelativePath(m_settings->getValue(MacProjectDir), directory) : directory;
		  targetLabel->setText(dir);
		  targetLabel->setFocus();
	       }


	    m_settings->setValue(MacProjectDir, ui->macProjectDir->text());
	    m_settings->setValue(MacSourceDir, ui->macSourceDir->text());
	    m_settings->setValue(MacIdHeaderDir, ui->macHeaderDir->text());
	    m_settings->setValue(MacSdkDir, ui->macSDKDir->text());
	    m_settings->setValue(MacDebugTargetDir, ui->macDebugTargetDir->text());
	    m_settings->setValue(MacReleaseTargetDir, ui->macReleaseTargetDir->text());
	 });

      //architecture
      connect(ui->macTargetTypeCombo, &QComboBox::currentIndexChanged, this, [&](int){
	    m_settings->setValue(MacTargetType, ui->macTargetTypeCombo->currentText());
	 });


      //macsharedconfigcheck
      connect(ui->macSharedConfigCheck, &QCheckBox::stateChanged, this, [&](int value){
	    auto group_box_enable = [&](bool en) {
		ui->sharedXCConfigGroupBox->setEnabled(en);
	    };
	    if(value == 2) {
		group_box_enable(true);
	    } else {
		group_box_enable(false);
	    }
	    m_settings->macSharedXcConfigCheck = value;
	 });

      //macsharedconfigfiles
      QButtonGroup *macXcConfigGroup = new QButtonGroup(this);
      macXcConfigGroup->addButton(ui->macDebugXCConfigDirChooser);
      macXcConfigGroup->addButton(ui->macReleaseXCConfigDirChooser);
      macXcConfigGroup->addButton(ui->macMainXCConfigDirChooser);
      connect(macXcConfigGroup, &QButtonGroup::buttonClicked, this, [&](QAbstractButton *button){

	    auto buttonName = button->objectName();
	    buttonName.truncate(buttonName.length()-QString("Chooser").length());
	    auto *targetLabel = this->findChild<QLineEdit*>(buttonName);

	    QString preselected;
	    if (buttonName == "macMainXCConfigDir") {
		     preselected = FileUtility::GetAbsolutePathFromRelativePath(m_settings->getValue(MacSpecificXcconfigFile), m_settings->getValue(MacProjectDir));
	    } else if (buttonName == "macDebugXCConfigDir") {
		     preselected = FileUtility::GetAbsolutePathFromRelativePath(m_settings->getValue(MacSpecificDebugXcconfigFile), m_settings->getValue(MacProjectDir));
	    } else  if (buttonName == "macReleaseXCConfigDir") {
		  preselected = FileUtility::GetAbsolutePathFromRelativePath(m_settings->getValue(MacSpecificReleaseXcconfigFile), m_settings->getValue(MacProjectDir));
	    }

	    QString configFile = QFileDialog::getOpenFileName(this, "Choose File", preselected, tr("Config (*.xcconfig)"));
	    if (!configFile.isEmpty()) {
		  QString relativePath = FileUtility::GetRelativePath(m_settings->getValue(MacProjectDir), configFile);
		  targetLabel->setText(relativePath);
		  targetLabel->setFocus();
	       }

	    m_settings->setValue(MacSpecificXcconfigFile, ui->macMainXCConfigDir->text());
	    m_settings->setValue(MacSpecificDebugXcconfigFile, ui->macDebugXCConfigDir->text());
	    m_settings->setValue(MacSpecificReleaseXcconfigFile, ui->macReleaseXCConfigDir->text());
      });
      ///check correct plugin configuration
      connect(ui->plugInTypeCombo, &QComboBox::currentIndexChanged, this, &MainWindow::onPlugInTypeComboCurrentIndexChanged);
      connect(ui->productTypeCombo, &QComboBox::currentIndexChanged, this, &MainWindow::onProductTypeComboCurrentIndexChanged);

      //generate check boxes
      connect(ui->generateDialogCheck, &QCheckBox::clicked, this, [&](bool v){
	    m_settings->generateDialog = v;
	 });
      connect(ui->generatePanelCheck, &QCheckBox::clicked, this, [&](bool v){
	    m_settings->generatePanel = v;
	 });
      connect(ui->generateScriptingSupportCheck, &QCheckBox::clicked, this, [&](bool v){
	    m_settings->generateScriptingProvider = v;
	 });
      connect(ui->generateSelectionSuiteCheck, &QCheckBox::clicked, this, [&](bool v){
	    m_settings->generateSelectionSuite = v;
	 });
      connect(ui->generateFacadeCheck, &QCheckBox::clicked, this, [&](bool v){
	    m_settings->generateFacade = v;
	 });
      connect(ui->generatePreflightCheck, &QCheckBox::clicked, this, [&](bool v){
	    m_settings->generatePreflight = v;
	 });

   }



void MainWindow::loadDefaultSettings()
   {
      if (m_settings->sdkRootFolder.isEmpty()) {
	 m_settings->setValue(MacProjectDir, "");
       }
      //
      m_settings->setValue(Author, "Author");
      m_settings->setValue(LongName, "Dolly");
      m_settings->setValue(ShortName, "dollytest");
      m_settings->setValue(PrefixId, "0xe9400");

      m_settings->setValue(MacDebugTargetDir, "../debug_cocoa64/sdk");
      m_settings->setValue(MacIdHeaderDir, "../../../source/sdksamples/dollytest");
      m_settings->setValue(MacReleaseTargetDir, "../release_cocoa64/sdk");
      m_settings->setValue(MacSdkDir, "../../..");
      m_settings->setValue(MacSourceDir, "../../../source/sdksamples/dollytest");
      m_settings->setValue(MacTargetType, "x86_64");
      m_settings->setValue(ProductType, "kInDesignProduct");
      m_settings->setValue(PluginType, "kUIPlugin");

      m_settings->setValue(MacSpecificXcconfigFile, "_shared_build_settings/plugin.sdk.xcconfig");
      m_settings->setValue(MacSpecificDebugXcconfigFile, "_shared_build_settings/cocoa_ui_x64_debug.xcconfig");
      m_settings->setValue(MacSpecificReleaseXcconfigFile, "_shared_build_settings/cocoa_ui_x64_release.xcconfig");

      m_settings->macSharedXcConfigCheck = true;


      m_settings->setValue(WinDebugIntermediateDir, "..\\objD");
      m_settings->setValue(WinDebugIntermediateDirX64, "..\\objDx64");
      m_settings->setValue(WinDebugTargetDir, "..\\debug\\sdk");
      m_settings->setValue(WinDebugTargetDirX64, "..\\debugx64\\sdk");
      m_settings->setValue(WinIdHeaderDir, "..\\..\\..\\source\\sdksamples\\dollytest");
      m_settings->setValue(WinProjectDir, "");
      m_settings->setValue(WinReleaseIntermediateDir, "..\\objR");
      m_settings->setValue(WinReleaseIntermediateDirX64, "..\\objRx64");
      m_settings->setValue(WinReleaseTargetDir, "..\\release\\sdk");
      m_settings->setValue(WinReleaseTargetDirX64, "..\\releasex64\\sdk");
      m_settings->setValue(WinSdkDir, "..\\..\\..");
      m_settings->setValue(WinSourceDir, "..\\..\\..\\source\\sdksamples\\dollytest");

      m_settings->generateDialog = true;
      m_settings->generatePanel = true;
      m_settings->generateFacade = false;
      m_settings->generatePreflight = false;
      m_settings->generateScriptingProvider = false;
      m_settings->generateSelectionSuite = false;

      applySettings();
   }

///sdkrootfolder
void MainWindow::setSdkRootFolder(QString path)
   {


      RootFolder root(path, this);
      if (!root.checkDirExists() || path == "") {
	    ui->indesignSdkFolder->setText("");
	    shiftInitTabs(0);
	    return;
	 }
      if (!root.checkXSFolder()) {
	    ui->xsFolder->setText("Transform files should be in <SDK>/devtools/dolly/xs");
	    shiftInitTabs(0);
	    return;
	 }

      //sdk folder preflerden geliyorsa ilk taba geç
      shiftInitTabs(1);

      ui->indesignSdkFolder->setText(root.getRootPath());
      ui->xsFolder->setText(root.getXsPath());
      m_settings->sdkRootFolder = path;

      ///change prj folder
      QString platform = FileUtility::GetOSString();
      QString projectFolder = path + "/build/" +  platform + "/prj";
      ui->macProjectDir->setText(projectFolder);
      m_settings->setValue(Constants::MacProjectDir, projectFolder);

      m_settings->save();
   }

void MainWindow::transform()
   {
      QString dataPath = m_settings->getDataPath();
      QString tmpOutputFile(dataPath + "/outputfile.txt");


      auto inputFile = m_settings->platformInputFile;
      auto dollyMainFile = m_settings->dollyMainFile;


      m_saxon = new Saxon(tmpOutputFile);
      QThread *thread = new QThread(this);
      m_saxon->moveToThread(thread);

      connect(m_saxon, &Saxon::taskFinished, this, [&](bool result) {
	    QString msg = result ? "Process finished successfully" : "Process not finished";
	    Alert alert("Finish", msg);
	    alert.exec();

	    //add platform arch | change sdk root to absolute
	    proceedPreferences();


	    if (result) {
		  openOutputFolder();
	       }
	 });

      connect(m_saxon, &Saxon::taskFinished, thread, &QThread::quit);
      connect(m_saxon, &Saxon::taskFinished, m_saxon, &QObject::deleteLater);
      connect(thread, &QThread::finished, thread, &QObject::deleteLater);

      m_progress = new OverlayProgress(centralWidget(), 250);
      m_progress->resize(centralWidget()->size());

      connect(thread, &QThread::started, this, [&]() {
	    m_progress->show(true);
	 });
      connect(m_saxon, &Saxon::taskFinished, this, [&](){
	    m_progress->show(false);
	    delete m_progress;

	 });


      connect( thread, &QThread::started, m_saxon, [=](){
	    m_saxon->transform(inputFile,dollyMainFile);
	 });

      thread->start();



   }

void MainWindow::openOutputFolder()
   {
      QString projOutput = FileUtility::GetOSString() == "mac" ?
      m_settings->getValue(Constants::MacProjectDir) +"/" + m_settings->getValue(Constants::LongName) + ".xcodeproj" :
      m_settings->getValue(Constants::WinProjectDir) +"/" + m_settings->getValue(Constants::LongName) + ".vcxproj";
      revealFile(this, projOutput);
   }

void MainWindow::proceedPreferences()
   {
      QString xcodeProjectFile = m_settings->getValue(Constants::MacProjectDir) +
	       "/" + m_settings->getValue(Constants::LongName) + ".xcodeproj";
      QString xcodeSettingFile = xcodeProjectFile + "/project.pbxproj";
      if (!QFile::exists(xcodeSettingFile)) {
	    Alert alert("Project not fount", xcodeProjectFile, this);
	    alert.exec();
	    return;
      };

	 //change $SDK_ROOT variable to absolute path
      if(m_settings->isChangeSDKVariablePreference) {
	     ChangeFileContent(xcodeSettingFile, "$ID_SDK_ROOT", m_settings->sdkRootFolder);
      }

      if(m_settings->isAddPlatformArchPreference) {
	    //in intel based imac "ONLY_ACTIVE_ARCH" should be YES. default is NO
	    QString findStr = "INFOPLIST_PREPROCESS = YES;";
	    QString replaceStr = "INFOPLIST_PREPROCESS = YES;\r\nONLY_ACTIVE_ARCH = YES;";
	    ChangeFileContent(xcodeSettingFile, findStr, replaceStr);
      }
   }



void MainWindow::onProductTypeComboCurrentIndexChanged(int index)
{
    if (index == 2 || index == 4) {
	ui->plugInTypeCombo->setCurrentIndex(1);
	ui->plugInTypeCombo->setEnabled(false);
    } else {
	ui->plugInTypeCombo->setEnabled(true);
    }
    m_settings->setValue(ProductType, ui->productTypeCombo->currentText());
}

void MainWindow::onPlugInTypeComboCurrentIndexChanged(int index)
{
      //Can not create dialog, panel, menu for kInDesignServerProduct
    auto combo_enable = [&](bool en) {
	ui->generateDialogCheck->setEnabled(en);
	ui->generatePanelCheck->setEnabled(en);
	 //clear status
	if (!en) {
	      ui->generateDialogCheck->setChecked(en);
	      ui->generatePanelCheck->setChecked(en);
	   }
    };

    if(index == 1) {
	combo_enable(false);
    } else {
	combo_enable(true);
    }
    m_settings->setValue(PluginType, ui->plugInTypeCombo->currentText());

}

void MainWindow::shiftInitTabs(int index)
{
    auto tabWidget = ui->tabWidget;
    QWidget* shiftTab = tabWidget->widget(index);
    tabWidget->setCurrentWidget(shiftTab);
   }

void MainWindow::shortNameChanged(QString name)
{
    QString np("../../../source/sdksamples/");
    np.append(name);
    ui->macSourceDir->setText(np);
    ui->macHeaderDir->setText(np);
}


#pragma mark delete
MainWindow::~MainWindow()
{
    delete ui;
}


