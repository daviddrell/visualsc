#ifndef TEXTBLOCK_H
#define TEXTBLOCK_H

#include "textblockattributes.h"
#include <QXmlStreamWriter>
#include "scitem.h"

/**

  \class   SCTextBlock
  \ingroup DataModel
  \brief   The class represents a text-block in a state-chart Data-Model

  A SCTextBlock only has attributes and text.

*/

class SCDATAMODELSHARED_EXPORT SCTextBlock : public SCItem
{
    Q_OBJECT

public:
    SCTextBlock();
    virtual IAttributeContainer * getAttributes(); // reimplemented from base SCItem
    TextBlockAttributes attributes;
    QString getText();
    void    setText(QString);
    void writeSCVXML(QXmlStreamWriter & sw);

public slots:
    void handleAttributeChanged(IAttribute *attr);

signals:
     void textChanged();

private:

    QString _text;
};


Q_DECLARE_METATYPE (SCTextBlock*)


#endif // TEXTBLOCK_H
