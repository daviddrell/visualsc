#include "transitionattributes.h"

#include <QListIterator>
#include <QStringList>


//////////////////////////////////////////
//
//
//                TransitionAttributes container
//
//////////////////////////////////////////


TransitionAttributes::TransitionAttributes():
        IAttributeContainer()
{
    initCommon();
}

TransitionAttributes::TransitionAttributes(QObject * parent, QString key):
        IAttributeContainer(parent, key)
{
    initCommon();

}


TransitionAttributes::TransitionAttributes(const TransitionAttributes& ta):
        IAttributeContainer( ta)
{
    initCommon();

}

TransitionAttributes::~TransitionAttributes()
{

}



void TransitionAttributes::setAttributes(const IAttributeContainer& sourceAttrList)
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

            TransitionAttributes::TransitionPathAttribute *pt;
            TransitionAttributes::TransitionPositionAttribute *ps;
            TransitionAttributes::TransitionStringAttribute *str;

            if ( ( pt = dynamic_cast<TransitionAttributes::TransitionPathAttribute *>(sourceAttr)) != NULL)
            {
                TransitionAttributes::TransitionPathAttribute * newPt = new TransitionAttributes::TransitionPathAttribute (*pt);
                addItem(newPt);
                newAttr= newPt;
            }
            else if ( (  ps = dynamic_cast<TransitionAttributes::TransitionPositionAttribute *>(sourceAttr) ) != NULL )
            {
                TransitionAttributes::TransitionPositionAttribute * newPs = new TransitionAttributes::TransitionPositionAttribute (*ps);
                addItem(newPs);
                newAttr = newPs;
            }
            else if ( ( str = dynamic_cast<TransitionAttributes::TransitionStringAttribute *>(sourceAttr) ) != NULL )
            {
                TransitionAttributes::TransitionStringAttribute * newStr = new TransitionAttributes::TransitionStringAttribute (*str);
                addItem(newStr);
                newAttr = newStr;
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




TransitionAttributes::AttributeMajorType TransitionAttributes::getMajorType()
{
    return IAttributeContainer::aMajorType_Transition ;
}


void TransitionAttributes::initCommon()
{
    qRegisterMetaType<TransitionAttributes>("TransitionAttributes");

    qRegisterMetaType<TransitionStringAttribute>("TransitionAttributes::TransitionPathAttribute");

    qRegisterMetaType<TransitionStringAttribute>("TransitionAttributes::TransitionStringAttribute");

    qRegisterMetaType<TransitionPositionAttribute>("TransitionAttributes::TransitionPositionAttribute");

}

//////////////////////////////////////////
//
//
//                Path Attribute
//
//////////////////////////////////////////



TransitionAttributes::TransitionPathAttribute::TransitionPathAttribute():
        IAttribute(),
        _pathPoints()
{
}

TransitionAttributes::TransitionPathAttribute::TransitionPathAttribute(QObject * parent, QString key, QList<QPointF>& initValue):
        IAttribute(parent,key),
        _pathPoints(initValue)
{
}

TransitionAttributes::TransitionPathAttribute::TransitionPathAttribute(const TransitionPathAttribute & pathAttribute ) :
        IAttribute(pathAttribute)
{
}


TransitionAttributes::TransitionPathAttribute::~TransitionPathAttribute()
{
}

TransitionAttributes::TransitionPathAttribute& TransitionAttributes::TransitionPathAttribute::operator=( QList<QPointF>& path )
{
    _pathPoints.clear();

    QListIterator<QPointF> i(path);
    while (i.hasNext())
    {
        _pathPoints.append(i.next());
    }

    return *this;
}

QString TransitionAttributes::TransitionPathAttribute::asString()
{
    //    <path d="m 100 100  300 100  200 300 "

    QString string = "m ";
    QListIterator<QPointF> i(_pathPoints);
    while (i.hasNext())
    {
        string.append(QString::number( i.next().x() ));
        string = " ";
        string.append(QString::number( i.next().y() ));
        string = " ";
    }
    return string;
}

QList<QPointF> TransitionAttributes::TransitionPathAttribute::asQPointFList()
{
    return QList<QPointF>(_pathPoints);
}

void TransitionAttributes::TransitionPathAttribute::setValue(const QString data)
{
    _pathPoints.clear();

    QStringList sl = data.split(" ");

    for (int i=0; i < sl.length(); i++ )
    {
        if ( sl[i].trimmed().isEmpty())
            sl.removeAt(i);
    }

    for (int i=0; i < sl.length(); )
    {
        if ( sl[i] == "m" ) i++;
        if ( sl[i] == "L" ) i++;

        if (  sl.length() > (i+1))
        {
            qreal x = sl[i].toDouble();
            qreal y = sl[i+1].toDouble();
            _pathPoints.append( QPointF(x,y));
            i += 2;
        }
        else
            break;
    }
}

void TransitionAttributes::TransitionPathAttribute::setValue(QList<QPointF>& path )
{
    _pathPoints = path;
}

int TransitionAttributes::TransitionPathAttribute::getType()
{
    return (int)QVariant::UserType + (int)IAttribute::aType_Path;
}




//////////////////////////////////////////
//
//
//                String Attribute
//
//////////////////////////////////////////



TransitionAttributes::TransitionStringAttribute::TransitionStringAttribute( ):
        IAttribute(),
        _string()
{
}

TransitionAttributes::TransitionStringAttribute::TransitionStringAttribute(QObject * parent, QString key, QString initValue):
        IAttribute(parent,key),
        _string(initValue)
{
}

TransitionAttributes::TransitionStringAttribute::TransitionStringAttribute(const TransitionStringAttribute & stringAttribute ) :
        IAttribute(stringAttribute)
{
}

TransitionAttributes::TransitionStringAttribute::~TransitionStringAttribute()
{
}

TransitionAttributes::TransitionStringAttribute& TransitionAttributes::TransitionStringAttribute::operator=( QString& str)
                                                                                                           {
    _string = str;
    return *this;
}

QString TransitionAttributes::TransitionStringAttribute::asString()
{
    return _string;
}



void TransitionAttributes::TransitionStringAttribute::setValue(QString str )
{
    _string = str;
}

int TransitionAttributes::TransitionStringAttribute::getType()
{
    return (int)QVariant::String;
}




//////////////////////////////////////////
//
//
//                Position
//
//////////////////////////////////////////


TransitionAttributes::TransitionPositionAttribute::TransitionPositionAttribute( ) :
        IAttribute(),
        _position()
{
}

TransitionAttributes::TransitionPositionAttribute::TransitionPositionAttribute(QObject * parent, QString key, QPointF initValue) :
        IAttribute(parent,key),
        _position(initValue)
{
}

TransitionAttributes::TransitionPositionAttribute::TransitionPositionAttribute(const TransitionStringAttribute & stringAttribute ) :
        IAttribute(stringAttribute)
{
}

TransitionAttributes::TransitionPositionAttribute::~TransitionPositionAttribute( )
{
}

TransitionAttributes::TransitionPositionAttribute& TransitionAttributes::TransitionPositionAttribute::operator=( QString& str)
{
    this->setValue(str);
    return *this;
}


QString TransitionAttributes::TransitionPositionAttribute::asString()
{
    return (QString::number(_position.x()) + QString(",") +  QString::number(_position.y()));
}


QPointF TransitionAttributes::TransitionPositionAttribute::asQPointF()
{
    return _position;
}


void TransitionAttributes::TransitionPositionAttribute::setValue(const int w,const int h)
{
    _position.setX(w);
    _position.setY(h);
}


void TransitionAttributes::TransitionPositionAttribute::setValue(const QPointF s)
{
    _position = s;
}


void TransitionAttributes::TransitionPositionAttribute::setValue(const QString position)
{
    QStringList sl = position.split(",");
    if ( sl.length() == 2)
    {
        _position.setX( sl[0].toInt() );
        _position.setY( sl[1].toInt() );
    }
}

int TransitionAttributes::TransitionPositionAttribute::getType()
{
    return (int)QVariant::PointF;
}


