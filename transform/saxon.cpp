#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QProcess>
#include "saxon.h"




Saxon::Saxon(QString outputLogFilePath, QObject *parent) : ITransformer(outputLogFilePath,parent), cmd(new QProcess(this))
{
    connect( cmd, SIGNAL(readyReadStandardError()), this, SLOT(handleReadStandardError()) );
    connect( cmd, SIGNAL(readyReadStandardOutput()), this, SLOT(handleReadStandardOutput()));
    copySaxonJarFile();
}

Saxon::~Saxon()
{
    delete cmd;
}

void Saxon::handleReadStandardError()
{
    QByteArray data = cmd->readAllStandardError();
    if(!QString(data).isEmpty()) {
        outputLogFile.write("ERROR: " + data + "-");
        qDebug() << "ERROR: " + data;
    }
}

void Saxon::handleReadStandardOutput()
{
    QByteArray data = cmd->readAllStandardOutput();
    if(!QString(data).isEmpty()) {
        outputLogFile.write(data);
    }
}


void Saxon::transform(QString XMLFile, QString XSLStyleSheetFile)
{
       bool result = true;
       QStringList arguments;

       arguments << "-jar" << SAXON_JAR << XMLFile << XSLStyleSheetFile;
       cmd->start("java", arguments, QIODevice::ReadWrite);
       cmd->waitForFinished();

       if ( cmd->state() == QProcess::NotRunning ) {
	   qDebug() << "The process is not running.It exits";

       }
       else if ( cmd->state() == QProcess::Starting ) {
	   qDebug() << "The process is started, but the program has not yet been invoked.";
       }
       else if ( cmd->state() == QProcess::Running ) {
	   qDebug() << "The process is running and is ready for reading and writing.";
       }

       if (!cmd->waitForStarted())  {
	   qDebug() << "Not yet started";
       }

       if ( cmd->exitStatus() != QProcess::NormalExit) {
	     result = false;
	  }

      cmd->close();


      ///bunu silelim sadece delay için
//      {
//	 QTime dieTime = QTime::currentTime().addSecs(5);
//	     while( QTime::currentTime() < dieTime )
//	     {
//		 QCoreApplication::processEvents( QEventLoop::AllEvents, 100 );
//	     }
//      }

      emit taskFinished(result);

}


void Saxon::copySaxonJarFile()
{
//    will save saxon file into application bundle resource folder
#ifdef __APPLE__
    QString appBundleMacOsPath = QCoreApplication::applicationDirPath();
    appBundleMacOsPath.append("/../Resources/jars");
    QDir jarDir(QDir(appBundleMacOsPath).absolutePath());
    if(!jarDir.exists()) {
        QDir().mkdir(jarDir.path());
    }
    QString saxonJarFile(":/Saxon/Resources/saxon/saxon.jar");
    QString targetFile(jarDir.path() + "/saxon.jar");
    if(!QFile(targetFile).exists()) {
        if(!QFile::copy(saxonJarFile, targetFile)){
            qDebug() << saxonJarFile << " kopyalanamadı";
        }
    }
    SAXON_JAR = targetFile;
#elif
    ///should be implemented for windows
#endif

}
