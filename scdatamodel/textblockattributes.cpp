#include "iattribute.h"
#include "textblockattributes.h"
#include <QStringList>
#include "positionattribute.h"
#include "sizeattribute.h"
#include <QDebug>


//************ Text **************

TextAttribute::TextAttribute(QObject * parent, QString key, QString initValue) :
        IAttribute(parent,key),
        _text(initValue)
{
}


TextAttribute::TextAttribute() : IAttribute()
{}


TextAttribute::TextAttribute(const TextAttribute& textAttribute):
        IAttribute(textAttribute)
{
    _text = textAttribute._text;
}


TextAttribute& TextAttribute::operator=(const TextAttribute& textAttribute)
{
    IAttribute::operator =(textAttribute);

    _text = textAttribute._text;
    return *this;
}


TextAttribute::~TextAttribute()
{}

QString TextAttribute::asString()
{
    return _text;
}

void TextAttribute::setValue(const QString value)
{
    if ( _text != value)
    {
        _text = value;
        emit changed(this);
    }
}

//************ FontFamily **************

FontFamilyAttribute::FontFamilyAttribute(QObject * parent, QString key, QString initValue) :
        IAttribute(parent,key),
        _family(initValue)
{
}


FontFamilyAttribute::FontFamilyAttribute() : IAttribute()
{}


FontFamilyAttribute::FontFamilyAttribute(const FontFamilyAttribute& fontFamilyAttribute):
        IAttribute(fontFamilyAttribute)
{
    _family = fontFamilyAttribute._family;
}


FontFamilyAttribute& FontFamilyAttribute::operator=(const FontFamilyAttribute& fontFamilyAttribute)
{
    IAttribute::operator =(fontFamilyAttribute);

    _family = fontFamilyAttribute._family;
    return *this;
}


FontFamilyAttribute::~FontFamilyAttribute()
{}

QString FontFamilyAttribute::asString()
{
    return _family;
}

void FontFamilyAttribute::setValue(const QString value)
{
    if ( _family != value)
    {
        _family = value;
        emit changed(this);
    }
}


//************ FontSize **************


FontSizeAttribute::FontSizeAttribute(QObject * parent, QString key, int initValue) :
        IAttribute(parent,key),
        _size(initValue)
{
}


FontSizeAttribute::FontSizeAttribute(QObject * parent, QString key, QString initValue) :
        IAttribute(parent,key),
        _size()
{
    setValue(initValue);
}

FontSizeAttribute::FontSizeAttribute(const  FontSizeAttribute& fontSizeAttribute) : IAttribute(fontSizeAttribute)
{

    _size = fontSizeAttribute._size;

}

FontSizeAttribute& FontSizeAttribute::operator=(const  FontSizeAttribute& fontSizeAttribute)
{
    IAttribute::operator =(fontSizeAttribute);

    _size = fontSizeAttribute._size;

    return *this;
}


FontSizeAttribute::FontSizeAttribute() : IAttribute()
{}


FontSizeAttribute::~FontSizeAttribute()
{}

int FontSizeAttribute::asInt()
{
    return _size;
}

QString FontSizeAttribute::asString()
{
    QString sizeStr = QString::number(_size) ;
    return sizeStr;
}

void  FontSizeAttribute::setValue(const QString v)
{

    bool ok = false;
    int sz = v.toInt(&ok, 10);


    if ( ! ok )
    {
        emit error(QString("FontSize::setValue cound not convert [" + v + +"] to a size"));
    }
    else if ( sz != _size)
    {
        _size  = sz;
        emit changed(this);
    }

}

void  FontSizeAttribute::setValue(const int s)
{
    if ( _size != s)
    {
        _size  = s;
        emit changed(this);
    }

}





//************ FontColor **************



FontColorAttribute::FontColorAttribute(QObject * parent, QString key, QColor initValue) :
        IAttribute(parent,key),
        _color(initValue)
{
}

FontColorAttribute::FontColorAttribute(const FontColorAttribute& fontColorAttribute): IAttribute(fontColorAttribute)
{
    _color = fontColorAttribute._color;
}

FontColorAttribute& FontColorAttribute::operator=( const FontColorAttribute& fontColorAttribute)
{
    IAttribute::operator =(fontColorAttribute);

    _color = fontColorAttribute._color;
    return *this;
}


FontColorAttribute::FontColorAttribute() : IAttribute()
{}


FontColorAttribute::~FontColorAttribute()
{}

QColor FontColorAttribute::asQColor()
{
    return _color;
}

QString FontColorAttribute::asString()
{
    int r=0,g=0,b=0,a=0;
    _color.getRgb(&r,&g,&b,&a);
    QString color  = QString("rgb(") + QString::number(r,(int)10).toUpper();
    color.append(",");
    color.append( QString::number(g,(int)10).toUpper()) ;
    color.append(",");
    color.append( QString::number(b,(int)10).toUpper()) ;
    color.append(")");
    return color;
}

void  FontColorAttribute::setValue(const QString c)
{
    bool ok = true;
    QString color = c;
    QColor tempColor;

    if ( color.contains("rgb("))
    {
        // "rgb(244,24,240)"
        color.remove(0,4);
        color.remove(")");
        QStringList list = color.split(",");
        if ( list.count() != 3)
        {
            ok = false;
        }
        else
        {
            int r=0,g=0,b=0;
            r = list[0].toInt(&ok, 10);
            g = list[1].toInt(&ok, 10);
            b = list[2].toInt(&ok, 10);
            if ( ok )
                tempColor = QColor(r,g,b);


        }
    }
    else
    {
        tempColor = QColor (color);
        ok = tempColor.isValid();
    }


    if ( ! ok )
    {
        emit error(QString("FontSize::setValue cound not convert [" + c  +"] to a color"));
    }
    else if ( tempColor != _color)
    {
        _color = tempColor;
        emit changed(this);
    }
}


void  FontColorAttribute::setValue(const QColor color)
{
    if ( color != color )
    {
       _color  = color;
       emit changed(this);
    }
}



//************ FontBold **************

FontBoldAttribute::FontBoldAttribute(QObject * parent, QString key, bool initValue): IAttribute(parent,key)
{
    _bold = initValue;
}

FontBoldAttribute::FontBoldAttribute(QObject * parent, QString key, QString initValue): IAttribute(parent,key)
{
    setValue(initValue);
}

FontBoldAttribute::FontBoldAttribute(const FontBoldAttribute & fontBoldAttribute): IAttribute(fontBoldAttribute)
{
    _bold = fontBoldAttribute._bold;
}

FontBoldAttribute& FontBoldAttribute::operator =(const FontBoldAttribute & fontBoldAttribute)
{
    IAttribute::operator =(fontBoldAttribute);

    _bold = fontBoldAttribute._bold;
    return *this;
}

FontBoldAttribute::FontBoldAttribute(): IAttribute()
{
}

FontBoldAttribute::~FontBoldAttribute()
{
}

QString FontBoldAttribute::asString()
{
    return (_bold ? QString("true") : QString("false"));
}


bool FontBoldAttribute::asBool()
{
    return _bold ;
}

void FontBoldAttribute::setValue(const QString bold)
{
    bool tempVal=false;

    if ( bold.toLower() == "true"  )
        tempVal = true;

    if ( tempVal != _bold)
    {
        _bold = tempVal;
        emit changed(this);
    }
}


void FontBoldAttribute::setValue(const bool bold)
{

    if ( bold != _bold)
    {
        _bold = bold;
        qDebug() << "FontBoldAttribute setValue to: " << bold<<" and emitting changed";
        emit changed(this);
    }
}


//************ FontUnderline **************



FontUnderlineAttribute::FontUnderlineAttribute(QObject * parent, QString key, bool initValue): IAttribute(parent,key)
{
    _underline = initValue;
}


FontUnderlineAttribute::FontUnderlineAttribute(QObject * parent, QString key, QString initValue): IAttribute(parent,key)
{
    setValue(initValue);
}

FontUnderlineAttribute::FontUnderlineAttribute(const FontUnderlineAttribute & fontUnderlineAttribute) : IAttribute(fontUnderlineAttribute)
{
    _underline = fontUnderlineAttribute._underline;
}

FontUnderlineAttribute& FontUnderlineAttribute::operator =(const FontUnderlineAttribute & fontUnderlineAttribute)
{
    IAttribute::operator =(fontUnderlineAttribute);

    _underline = fontUnderlineAttribute._underline;

    return *this;
}


FontUnderlineAttribute::FontUnderlineAttribute() : IAttribute()
{
}

FontUnderlineAttribute::~FontUnderlineAttribute()
{}

QString FontUnderlineAttribute::asString()
{
    return (_underline ? QString("true") : QString("false"));
}


bool FontUnderlineAttribute::asBool()
{
    return _underline ;
}

void FontUnderlineAttribute::setValue(const QString underline)
{
    bool tempVal=false;

    if ( underline.toLower() == "true"  )
        tempVal = true;

    if ( tempVal != _underline)
    {
        _underline = tempVal;
        emit changed(this);
    }
}


void FontUnderlineAttribute::setValue(const bool underline)
{

    if ( underline != _underline)
    {
        _underline = underline;
        emit changed(this);
    }
}






TextBlockAttributes::TextBlockAttributes() :
        IAttributeContainer()
{
}

TextBlockAttributes::TextBlockAttributes(QObject * parent, QString key) :
        IAttributeContainer(parent, key)
{
}

TextBlockAttributes::TextBlockAttributes(const TextBlockAttributes& sa): IAttributeContainer(sa)
{
    setAttributes(sa);
}

TextBlockAttributes& TextBlockAttributes::operator=( TextBlockAttributes& sa )
{
    setAttributes(sa);
    return *this;
}


TextBlockAttributes::~TextBlockAttributes()
{ }


/**
 * @brief TextBlockAttributes::setAttributes
 * @param sourceAttrList
 */
void TextBlockAttributes::setAttributes(const IAttributeContainer& sourceAttrList)
{
    // if the dest container has a matching key,
    // update the value to be the source value.
    // else, add the source to the container
    //
    // remove from the dest list any attributes that are not in the souce list

    QMapIterator<QString,IAttribute*> i(sourceAttrList);
    while (i.hasNext())
    {
        QString key  = i.next().key();
        IAttribute* sourceAttr = sourceAttrList.value(key)  ;
        IAttribute* destAttr = this->value( key ) ;
        if ( destAttr )
        {
            destAttr->setValue( sourceAttr->asString());
        }
        else
        {
            IAttribute* newAttr=NULL;

            PositionAttribute *ps=NULL;
            SizeAttribute *sz=NULL;
            FontFamilyAttribute *ff=NULL;
            FontSizeAttribute *fs=NULL;
            FontColorAttribute *fc=NULL;
            FontBoldAttribute *fb=NULL;
            FontUnderlineAttribute *fu=NULL;

            if ( ( ps = dynamic_cast<PositionAttribute *>(sourceAttr) ) != NULL )
            {
                PositionAttribute * newPs = new PositionAttribute (*ps);
                addItem(newPs);
                newAttr = newPs;
            }
            else if ( ( sz = dynamic_cast<SizeAttribute *>(sourceAttr) ) != NULL )
            {
                SizeAttribute * newSz = new SizeAttribute (*sz);
                addItem(newSz);
                newAttr = newSz;
            }
            else if ( (  ff = dynamic_cast<FontFamilyAttribute *>(sourceAttr) ) != NULL )
            {
                FontFamilyAttribute * f = new FontFamilyAttribute (*ff);
                addItem(f);
                newAttr = f;
            }
            else if ( (  fs = dynamic_cast<FontSizeAttribute *>(sourceAttr) ) != NULL )
            {
                FontSizeAttribute * f = new FontSizeAttribute (*fs);
                addItem(f);
                newAttr = f;
            }
            else if ( (  fc = dynamic_cast<FontColorAttribute *>(sourceAttr) ) != NULL )
            {
                FontColorAttribute * f = new FontColorAttribute (*fc);
                addItem(f);
                newAttr = f;
            }
            else if ( (  fb = dynamic_cast<FontBoldAttribute *>(sourceAttr) ) != NULL )
            {
                FontBoldAttribute * f = new FontBoldAttribute (*fb);
                addItem(f);
                newAttr = f;
            }
            else if ( (  fu = dynamic_cast<FontUnderlineAttribute *>(sourceAttr) ) != NULL )
            {
                FontUnderlineAttribute * f = new FontUnderlineAttribute (*fu);
                addItem(f);
                newAttr = f;
            }

            if ( newAttr ) emit attributeAdded(newAttr);
        }
    }

    // now delete local attributes that are not contained in the source list

   QMapIterator<QString,IAttribute*> j(sourceAttrList);

    while (j.hasNext())
    {
        QString key  = j.next().key();

        if ( !sourceAttrList.contains(key) )
        {
            IAttribute* attr = this->value(key);
            this->remove(key);
            emit attributeDeleted(attr);
            delete attr;
        }
    }
}





