#ifndef STATEIDTEXTBLOCKATTRIBUTES_H
#define STATEIDTEXTBLOCKATTRIBUTES_H


#include <QList>
#include <QPointF>
#include <QString>
#include <QColor>
#include "iattribute.h"

#include "scdatamodel_global.h"
#include <QMetaType>


/**
 * \class StateIDTextBlockAttributes
 *
 * \ingroup DataModel
 * \ingroup Attributes
 *
 */



class SCDATAMODELSHARED_EXPORT  FontFamily :  public IAttribute
{
public:
    FontFamily(QObject * parent, QString key, QString initValue);
    FontFamily(const FontFamily& fontFamilyAttribute);
    FontFamily();

    ~FontFamily();

    FontFamily& operator=( const FontFamily& fontFamilyAttribute);

    virtual QString asString();

    virtual void    setValue(const QString value)  ;

private:

    QString _family;
};



class SCDATAMODELSHARED_EXPORT  FontSize :  public IAttribute
{
public:
    FontSize(QObject * parent, QString key, int initValue);
    FontSize(const FontSize & fontSizeAttribute );
    FontSize();

    ~FontSize();
    FontSize& operator=(const  FontSize& fontSizeAttribute);

    virtual QString asString();
    virtual int asInt();
    virtual void    setValue(const QString size)  ;
    void            setValue(const int v);

private:

    int _size;
};


class SCDATAMODELSHARED_EXPORT  FontColor :  public IAttribute
{
public:
    FontColor(QObject * parent, QString key, QColor initValue);
    FontColor(const FontColor& fontColorAttribute );
    FontColor();

    ~FontColor();
    FontColor& operator=( const FontColor& fontColorAttribute);

    virtual QString asString();
    QColor asQColor();

    /* html font color attribute
    color_name 	Specifies the text color with a color name (like "red")
    hex_number 	Specifies the text color with a hex code (like "#ff0000")
    rgb_number 	Specifies the text color with an rgb code (like "rgb(255,0,0)")
    */

    virtual void    setValue(const QString color)  ;
    void            setValue(const QColor color);

private:

    QColor _color;
};

class SCDATAMODELSHARED_EXPORT  FontBold :  public IAttribute
{
public:
    FontBold(QObject * parent, QString key, bool initValue);
    FontBold(const FontBold & fontBoldAttribute );
    FontBold();

    ~FontBold();
    FontBold& operator=(const FontBold& fontBoldAttribute);

    virtual QString asString();
    bool asBool();

    virtual void    setValue(const QString bold)  ;
            void    setValue(const bool bold)  ;

private:

    bool _bold;
};


class SCDATAMODELSHARED_EXPORT  FontUnderline :  public IAttribute
{
public:
    FontUnderline(QObject * parent, QString key, bool initValue);
    FontUnderline(const FontUnderline & fontUnderlineAttribute );
    FontUnderline();
    ~FontUnderline();
    FontUnderline& operator=( const FontUnderline& fontUnderlineAttribute);

    virtual QString asString();
            bool asBool();


    virtual void    setValue(const QString underline)  ;
            void    setValue(const bool underline)  ;

private:

    bool _underline;
};

class  SCDATAMODELSHARED_EXPORT TextBlockAttributes: public IAttributeContainer
{
public:

    TextBlockAttributes(QObject * parent, QString key);
    TextBlockAttributes( const TextBlockAttributes& ta);
    TextBlockAttributes();
    ~TextBlockAttributes();

    virtual void setAttributes(const IAttributeContainer& sourceAttrList);




private: // methods

    void initCommon();
};


Q_DECLARE_METATYPE(FontFamily);
Q_DECLARE_METATYPE(FontSize);
Q_DECLARE_METATYPE(FontColor);
Q_DECLARE_METATYPE(FontBold);
Q_DECLARE_METATYPE(FontUnderline);
Q_DECLARE_METATYPE(TextBlockAttributes);

#endif // STATEIDTEXTBLOCKATTRIBUTES_H
