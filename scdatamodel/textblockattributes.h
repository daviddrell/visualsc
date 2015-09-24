#ifndef STATEIDTEXTBLOCKATTRIBUTES_H
#define STATEIDTEXTBLOCKATTRIBUTES_H


#include <QList>
#include <QPointF>
#include <QString>
#include <QColor>
#include "iattribute.h"

#include "scdatamodel_global.h"
#include <QMetaType>




/** \class TextAttribute
    \brief Not really an attribute, but the text itself which fills the textblock
  *
  */
class SCDATAMODELSHARED_EXPORT  TextAttribute :  public IAttribute
{
    Q_OBJECT

public:
    TextAttribute(QObject * parent, QString key, QString initValue);
    TextAttribute(const TextAttribute& textAttribute);
    TextAttribute();

    ~TextAttribute();

    TextAttribute& operator=( const TextAttribute& textAttribute);

    virtual QString asString();

    virtual void    setValue(const QString value)  ;

private:

    QString _text;
};


class SCDATAMODELSHARED_EXPORT  FontFamilyAttribute :  public IAttribute
{
    Q_OBJECT

public:
    FontFamilyAttribute(QObject * parent, QString key, QString initValue);
    FontFamilyAttribute(const FontFamilyAttribute& fontFamilyAttribute);
    FontFamilyAttribute();

    ~FontFamilyAttribute();

    FontFamilyAttribute& operator=( const FontFamilyAttribute& fontFamilyAttribute);

    virtual QString asString();

    virtual void    setValue(const QString value)  ;

signals:
    void changed(FontFamilyAttribute*);

private:

    QString _family;
};



class SCDATAMODELSHARED_EXPORT  FontSizeAttribute :  public IAttribute
{
    Q_OBJECT

public:
    FontSizeAttribute(QObject * parent, QString key, int initValue);
    FontSizeAttribute(QObject * parent, QString key, QString initValue);
    FontSizeAttribute(const FontSizeAttribute & fontSizeAttribute );
    FontSizeAttribute();

    ~FontSizeAttribute();
    FontSizeAttribute& operator=(const  FontSizeAttribute& fontSizeAttribute);

    virtual QString asString();
    virtual int asInt();
    virtual void    setValue(const QString size)  ;
    void            setValue(const int v);

signals:
    void changed(FontSizeAttribute*);

private:

    int _size;
};


class SCDATAMODELSHARED_EXPORT  FontColorAttribute :  public IAttribute
{
    Q_OBJECT

public:
    FontColorAttribute(QObject * parent, QString key, QColor initValue);
    FontColorAttribute(const FontColorAttribute& fontColorAttribute );
    FontColorAttribute();

    ~FontColorAttribute();
    FontColorAttribute& operator=( const FontColorAttribute& fontColorAttribute);

    virtual QString asString();
    QColor asQColor();

    /* html font color attribute
    color_name 	Specifies the text color with a color name (like "red")
    hex_number 	Specifies the text color with a hex code (like "#ff0000")
    rgb_number 	Specifies the text color with an rgb code (like "rgb(255,0,0)")
    */

    virtual void setValue(const QString color)  ;
    void setValue(const QColor color);

private:

    QColor _color;
};

class SCDATAMODELSHARED_EXPORT  FontBoldAttribute :  public IAttribute
{
    Q_OBJECT

public:
    FontBoldAttribute(QObject * parent, QString key, bool initValue);
    FontBoldAttribute(QObject * parent, QString key, QString initValue);
    FontBoldAttribute(const FontBoldAttribute & fontBoldAttribute );
    FontBoldAttribute();

    ~FontBoldAttribute();
    FontBoldAttribute& operator=(const FontBoldAttribute& fontBoldAttribute);

    virtual QString asString();
    bool asBool();

    virtual void setValue(const QString bold)  ;
            void setValue(const bool bold)  ;

signals:
            void changed(FontBoldAttribute*);

private:

    bool _bold;
};


class SCDATAMODELSHARED_EXPORT  FontUnderlineAttribute :  public IAttribute
{
    Q_OBJECT

public:
    FontUnderlineAttribute(QObject * parent, QString key, bool initValue);
    FontUnderlineAttribute(QObject * parent, QString key, QString initValue);
    FontUnderlineAttribute(const FontUnderlineAttribute & fontUnderlineAttribute );
    FontUnderlineAttribute();
    ~FontUnderlineAttribute();
    FontUnderlineAttribute& operator=( const FontUnderlineAttribute& fontUnderlineAttribute);

    virtual QString asString();
            bool asBool();


    virtual void    setValue(const QString underline)  ;
            void    setValue(const bool underline)  ;

private:

    bool _underline;
};



/**
 * \class StateIDTextBlockAttributes
 *
 * \ingroup DataModel
 * \ingroup Attributes
 *
 */


class  SCDATAMODELSHARED_EXPORT TextBlockAttributes: public IAttributeContainer
{
    Q_OBJECT

public:

    TextBlockAttributes(QObject * parent, QString key);
    TextBlockAttributes( const TextBlockAttributes& ta);
    TextBlockAttributes();
    ~TextBlockAttributes();

    TextBlockAttributes& operator=( TextBlockAttributes& sa );


    virtual void setAttributes(const IAttributeContainer& sourceAttrList);



private: // methods

    void initCommon();
};


Q_DECLARE_METATYPE(FontFamilyAttribute);
Q_DECLARE_METATYPE(FontSizeAttribute);
Q_DECLARE_METATYPE(FontColorAttribute);
Q_DECLARE_METATYPE(FontBoldAttribute);
Q_DECLARE_METATYPE(FontUnderlineAttribute);
Q_DECLARE_METATYPE(TextBlockAttributes);

#endif // STATEIDTEXTBLOCKATTRIBUTES_H
