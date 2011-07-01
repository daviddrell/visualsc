#include "iattribute.h"
#include "textblockattributes.h"
#include <QStringList>

//************ FontFamily **************

FontFamily::FontFamily(QObject * parent, QString key, QString initValue) :
        IAttribute(parent,key),
        _family(initValue)
{
}


FontFamily::FontFamily() : IAttribute()
{}


FontFamily::FontFamily(const FontFamily& fontFamilyAttribute):
        IAttribute(fontFamilyAttribute)
{
    _family = fontFamilyAttribute._family;
}

FontFamily& FontFamily::operator=(const FontFamily& fontFamilyAttribute)
{
    IAttribute::operator =(fontFamilyAttribute);

    _family = fontFamilyAttribute._family;
    return *this;
}


FontFamily::~FontFamily()
{}

QString FontFamily::asString()
{
    return _family;
}

void FontFamily::setValue(const QString value)
{
    if ( _family != value)
    {
        _family = value;
        emit changed(this);
    }
}


//************ FontSize **************


FontSize::FontSize(QObject * parent, QString key, int initValue) :
        IAttribute(parent,key),
        _size(initValue)
{
}


FontSize::FontSize(const  FontSize& fontSizeAttribute) : IAttribute(fontSizeAttribute)
{

    _size = fontSizeAttribute._size;

}

FontSize& FontSize::operator=(const  FontSize& fontSizeAttribute)
{
    IAttribute::operator =(fontSizeAttribute);

    _size = fontSizeAttribute._size;

    return *this;
}


FontSize::FontSize() : IAttribute()
{}


FontSize::~FontSize()
{}

int FontSize::asInt()
{
    return _size;
}

QString FontSize::asString()
{
    QString sizeStr = QString::number(_size) ;
    return sizeStr;
}

void  FontSize::setValue(const QString v)
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

void  FontSize::setValue(const int s)
{
    if ( _size != s)
    {
        _size  = s;
        emit changed(this);
    }

}





//************ FontColor **************



FontColor::FontColor(QObject * parent, QString key, QColor initValue) :
        IAttribute(parent,key),
        _color(initValue)
{
}

FontColor::FontColor(const FontColor& fontColorAttribute): IAttribute(fontColorAttribute)
{
    _color = fontColorAttribute._color;
}

FontColor& FontColor::operator=( const FontColor& fontColorAttribute)
{
    IAttribute::operator =(fontColorAttribute);

    _color = fontColorAttribute._color;
    return *this;
}


FontColor::FontColor() : IAttribute()
{}


FontColor::~FontColor()
{}

QColor FontColor::asQColor()
{
    return _color;
}

QString FontColor::asString()
{
    int r=0,g=0,b=0,a=0;
    _color.getRgb(&r,&g,&b,&a);
    QString color  = QString("#") + QString::number(r,(int)16).toUpper();
    color.append( QString::number(g,(int)16).toUpper()) ;
    color.append( QString::number(b,(int)16).toUpper()) ;
    return color;
}

void  FontColor::setValue(const QString c)
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


void  FontColor::setValue(const QColor color)
{
    if ( color != color )
    {
       _color  = color;
       emit changed(this);
    }
}



//************ FontBold **************

FontBold::FontBold(QObject * parent, QString key, bool initValue): IAttribute(parent,key)
{
    _bold = initValue;
}

FontBold::FontBold(const FontBold & fontBoldAttribute): IAttribute(fontBoldAttribute)
{
    _bold = fontBoldAttribute._bold;
}

FontBold& FontBold::operator =(const FontBold & fontBoldAttribute)
{
    IAttribute::operator =(fontBoldAttribute);

    _bold = fontBoldAttribute._bold;
    return *this;
}

FontBold::FontBold(): IAttribute()
{
}

FontBold::~FontBold()
{
}

QString FontBold::asString()
{
    return (_bold ? QString("true") : QString("false"));
}


bool FontBold::asBool()
{
    return _bold ;
}

void FontBold::setValue(const QString bold)
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


void FontBold::setValue(const bool bold)
{

    if ( bold != _bold)
    {
        _bold = bold;
        emit changed(this);
    }
}


//************ FontUnderline **************



FontUnderline::FontUnderline(QObject * parent, QString key, bool initValue): IAttribute(parent,key)
{
    _underline = initValue;
}

FontUnderline::FontUnderline(const FontUnderline & fontUnderlineAttribute) : IAttribute(fontUnderlineAttribute)
{
    _underline = fontUnderlineAttribute._underline;
}

FontUnderline& FontUnderline::operator =(const FontUnderline & fontUnderlineAttribute)
{
    IAttribute::operator =(fontUnderlineAttribute);

    _underline = fontUnderlineAttribute._underline;

    return *this;
}


FontUnderline::FontUnderline() : IAttribute()
{
}

FontUnderline::~FontUnderline()
{}

QString FontUnderline::asString()
{
    return (_underline ? QString("true") : QString("false"));
}


bool FontUnderline::asBool()
{
    return _underline ;
}

void FontUnderline::setValue(const QString underline)
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


void FontUnderline::setValue(const bool underline)
{

    if ( underline != _underline)
    {
        _underline = underline;
        emit changed(this);
    }
}

