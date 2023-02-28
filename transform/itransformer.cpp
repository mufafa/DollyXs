#include "itransformer.h"
#include <QtDebug>
ITransformer::ITransformer(QString outputLogFilePath, QObject *parent) : QObject(parent), outputLogFile(outputLogFilePath)
{
    outputLogFile.open(QIODevice::ReadWrite | QIODevice::Text);
}

ITransformer::~ITransformer()
{
    outputLogFile.close();
}
