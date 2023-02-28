#ifndef FILEUTILITY_H
#define FILEUTILITY_H

#include <QDir>
#include <QException>
#include <QFileDialog>
#include <QString>


namespace FileUtility {


static inline QString GetOSString() {
    QString tmpString = QSysInfo::productType();
    return  tmpString.contains("(?i).*win.*") ? "win" : "mac";
}

static inline QString GetAbsolutePathFromRelativePath(QString relativePath, QString basePath) {
    QDir AbsolutePath(basePath);
    QString RelativeToAbsolutePath = AbsolutePath.cleanPath(AbsolutePath.absoluteFilePath(relativePath));
    return  RelativeToAbsolutePath;
}


static inline QString GetFileFilter() {
    QString filter;
    QString ostype = GetOSString();
    if(ostype == "mac") {
        filter = "xcconfig (*.xcconfig);;XCConfig (*.xcconfig)";
    } else if (ostype == "win") {
        filter = "All files (*.*);;rsp (*.rsp)";
    }
    return filter;
}

static inline QString GetRelativePath(const QString& basePath, const QString& path) {
      QDir dir(basePath);
      QString relativePath = dir.relativeFilePath(path);     // s is "images/file.jpg"
      return relativePath;
   }

static inline QString GetFile(QString filePath, QString baseAbsolutePath) {

    QFileInfo path(filePath);

    QString absoluteFilePath = filePath;
    try {
        //relative ise absolute bul geri gönder
        if(path.isRelative()) {
            absoluteFilePath = GetAbsolutePathFromRelativePath(filePath, baseAbsolutePath);
        }

        QString filter = GetFileFilter();


        QFileDialog dlg(NULL, ("Select file"));
        dlg.selectFile(QFileInfo(absoluteFilePath).canonicalPath());

        dlg.setFileMode(QFileDialog::AnyFile);
        QString configFile = dlg.getOpenFileName(NULL, "Choose File", absoluteFilePath, filter);

        return configFile;

    }  catch (QException exception) { qDebug() << exception.what(); }
    return "";
}

static inline QString GetDirectory(QString filePath, QString baseAbsolutePath) {
    QFileInfo path(filePath);
    QString absoluteFilePath = filePath;

    try {
        //relative ise absolute bul geri gönder
        if(path.isRelative()) {
            absoluteFilePath = GetAbsolutePathFromRelativePath(filePath, baseAbsolutePath);
        }
        auto directory = QFileDialog::getExistingDirectory(NULL, "Choose Dir", absoluteFilePath);
        return directory;

    }  catch (QException exception) { qDebug() << exception.what(); }
    return "";
}

}



#endif // FILEUTILITY_H
