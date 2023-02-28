#ifndef ITRANSFORMER_H
#define ITRANSFORMER_H

#include <QHash>
#include <QObject>
#include <QFile>
class ITransformer : public QObject
{
    Q_OBJECT
protected:
    QFile outputLogFile;
public:
    explicit ITransformer(QString outputLogFilePath, QObject *parent = nullptr);
    virtual void transform(QString XMLFile, QString XSLStyleSheetFile) = 0;
    virtual ~ITransformer();
signals:
    void taskFinished(bool);


};

#endif // ITRANSFORMER_H
