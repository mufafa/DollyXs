#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class Settings;
class Saxon;
class OverlayProgress;
class Preferences;

QT_BEGIN_NAMESPACE

namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();



private:
    Ui::MainWindow *ui;

private:
    Settings* m_settings;
    Saxon* m_saxon;
    OverlayProgress* m_progress;
    Preferences* prefs;


private:
    void createUi();
    void applySettings();
    void saveSettings();
    void createConnections();
    void createMenuActions();
    void SetupDirs(QString rootFolder);
    void shiftInitTabs(int index);


    //! xs folderını kontrol et yoksa dur
    void setSdkRootFolder(QString path);
    void transform();
    void openOutputFolder();
    void proceedPreferences();


private slots:
    void loadDefaultSettings();
    void onPlugInTypeComboCurrentIndexChanged(int index);
    void onProductTypeComboCurrentIndexChanged(int index);
    void shortNameChanged(QString name);
};
#endif // MAINWINDOW_H
