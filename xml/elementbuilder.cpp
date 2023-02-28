#include "elementbuilder.h"

#include <QDomElement>



ElementBuilder::ElementBuilder(QDomDocument *document) :f_Document(document)
   {}


ElementBuilder::~ElementBuilder()
   {
//      delete f_Document;
   }


QDomElement ElementBuilder::create(const QString &node)
   {
      return this->create(node, QStringList(), QStringList());
   }

QDomElement ElementBuilder::create(QString node, QString key, QString value)
   {
      return this->create(node, QStringList(key), QStringList(value));
   }


QDomElement  ElementBuilder::create(QString node, QStringList keys, QStringList values)
   {
      QDomElement element = this->f_Document->createElement(node);
      if(!keys.isEmpty()  && !keys.isEmpty()) {
	  for (int b = 0; b < keys.length() ; b++ ) {
	      element.setAttribute(keys[b], values[b]);
	  }
      }
      return element;
   }
