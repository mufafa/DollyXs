#ifndef SAXON_H
#define SAXON_H

#include "itransformer.h"

#include <QProcess>


class Saxon : public ITransformer
{
    Q_OBJECT
    QProcess* cmd;
    QString SAXON_JAR;

public:
    explicit Saxon(QString outputLogFilePath, QObject *parent = nullptr);
    ~Saxon();



private slots:
    void handleReadStandardError();
    void handleReadStandardOutput();

private:
    void copySaxonJarFile();

    // ITransformer interface
public:
    void transform(QString XMLFile, QString XSLStyleSheetFile) override;
};

#endif // SAXON_H
