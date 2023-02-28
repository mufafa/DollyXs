#ifndef ELEMENTBUILDER_H
#define ELEMENTBUILDER_H

#include <QString>
#include <QStringList>



class QDomDocument;
class QDomElement;

class ElementBuilder
{
    QDomDocument *f_Document = NULL;
public:

    ~ElementBuilder();
    ElementBuilder(QDomDocument* document);
    QDomElement create(const QString& node );
    QDomElement create(QString node, QString key, QString value);
    QDomElement create(QString node, QStringList keys, QStringList values);
};

#endif // ELEMENTBUILDER_H
