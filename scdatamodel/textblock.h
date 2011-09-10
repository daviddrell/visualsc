#ifndef TEXTBLOCK_H
#define TEXTBLOCK_H

#include "textblockattributes.h"
#include <QXmlStreamWriter>
#include "scitem.h"

/**

  \class   TextBlock
  \ingroup DataModel
  \brief   The class represents a text-block in a state-chart Data-Model

  A TextBlock only has attributes and text.

*/

class SCDATAMODELSHARED_EXPORT TextBlock : public SCItem
{
    Q_OBJECT

public:
    TextBlock();
    virtual IAttributeContainer * getAttributes(); // reimplemented from base SCItem
    TextBlockAttributes attributes;
    QString getText();
    void    setText(QString);
    void writeSCVXML(QXmlStreamWriter & sw);

signals:
     void textChanged();

private:

    QString _text;
};


Q_DECLARE_METATYPE (TextBlock*)


#endif // TEXTBLOCK_H
