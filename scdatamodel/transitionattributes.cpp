/*
    visualsc - Visual State Chart Editing for SCXML State Engines
    Copyright (C) 2011  David W. Drell (david@davidwdrell.net) and
    Contributing Authors:


    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "transitionattributes.h"

#include <QListIterator>
#include <QStringList>
#include <QDebug>


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



void TransitionAttributes::addAttribute(const QString attrName)
{

    IAttribute* myAttr = this->value(attrName);

    if(!myAttr)
    {
        IAttribute* newAttr=NULL;

//        TransitionStringAttribute *str;
        TransitionStringAttribute * newStr = new TransitionStringAttribute();
        newStr->setValue(attrName);
        addItem(newStr);
        newAttr = newStr;


        qDebug() <<"newAttr created: "<< newAttr->asString();

        if ( newAttr ) emit attributeAdded(newAttr);
    }
    else
    {
        qDebug() << "Attribute "+ attrName+" already exists";
    }
}

void TransitionAttributes::setAttributes(const IAttributeContainer& sourceAttrList)
{
    QMapIterator<QString,IAttribute*> i(sourceAttrList);
    while(i.hasNext())
    {
        QString key = i.next().key();
        qDebug() << "key is: " << key;
        IAttribute* sourceAttr = sourceAttrList.value(key);
        IAttribute* myAttr = this->value(key);

        // check if this transitionattributes has the attribute
        // add the attribute to the transitionattributes and then set its value
        if(!myAttr)
        {
            IAttribute* newAttr=NULL;

            TransitionPathAttribute *pt;
            TransitionPositionAttribute *ps;
            TransitionStringAttribute *str;

            if ( ( pt = dynamic_cast<TransitionPathAttribute *>(sourceAttr)) != NULL)
            {
                TransitionPathAttribute * newPt = new TransitionPathAttribute (*pt);
                newPt->setValue(sourceAttr->asString());
                addItem(newPt);
                newAttr= newPt;
            }
            else if ( (  ps = dynamic_cast<TransitionPositionAttribute *>(sourceAttr) ) != NULL )
            {
                TransitionPositionAttribute * newPs = new TransitionPositionAttribute (*ps);
                newPs->setValue(sourceAttr->asString());
                addItem(newPs);
                newAttr = newPs;
            }
            else if ( ( str = dynamic_cast<TransitionStringAttribute *>(sourceAttr) ) != NULL )
            {
                TransitionStringAttribute * newStr = new TransitionStringAttribute (*str);
                newStr->setValue(sourceAttr->asString());
                addItem(newStr);
                newAttr = newStr;
            }

            qDebug() <<"newAttr created: "<< newAttr->asString();

            if ( newAttr ) emit attributeAdded(newAttr);
        }
        else
        {
            myAttr->setValue(sourceAttr->asString());
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

#ifdef USEOLD
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
        qDebug() << "key is: "<<key << " source: "<< sourceAttr->asString();


        if ( destAttr )
        {
            qDebug() << "setting destAttr from to source: " << destAttr->asString();
            //destAttr->setValue("YYEYYYYYYY");
           // if(key == "target")
                destAttr->setValue( sourceAttr->asString());
          //  else
             //   destAttr->setValue("AYY");
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
#endif
}



void TransitionAttributes::initCommon()
{
    qRegisterMetaType<TransitionAttributes>("TransitionAttributes");

    qRegisterMetaType<TransitionStringAttribute>("TransitionPathAttribute");

    qRegisterMetaType<TransitionStringAttribute>("TransitionStringAttribute");

    qRegisterMetaType<TransitionPositionAttribute>("TransitionPositionAttribute");

}

//////////////////////////////////////////
//
//
//                Path Attribute
//
//////////////////////////////////////////



TransitionPathAttribute::TransitionPathAttribute():
        IAttribute(),
        _pathPoints()
{
}

TransitionPathAttribute::TransitionPathAttribute(QObject * parent, QString key, QList<QPointF>& initValue):
        IAttribute(parent,key),
        _pathPoints(initValue)
{
}

TransitionPathAttribute::TransitionPathAttribute(const TransitionPathAttribute & pathAttribute ) :
        IAttribute(pathAttribute)
{
}


TransitionPathAttribute::~TransitionPathAttribute()
{
}

TransitionPathAttribute& TransitionPathAttribute::operator=( QList<QPointF>& path )
{
    _pathPoints.clear();

    QListIterator<QPointF> i(path);
    while (i.hasNext())
    {
        _pathPoints.append(i.next());
    }

    return *this;
}

QString TransitionPathAttribute::asString()
{
    //    <path d="M 100 100  L 300 100 L 200 300 "

    QString string = "M ";
    QListIterator<QPointF> i(_pathPoints);
    while (i.hasNext())
    {
        QPointF p = i.next();
        string.append(QString::number( p.x() ));
        string.append( " ");
        string.append(QString::number( p.y() ));
        string.append( " ");
        if ( i.hasNext())
            string.append("L ");
    }


    return string;
}

QList<QPointF> TransitionPathAttribute::asQPointFList()
{
    return QList<QPointF>(_pathPoints);
}

void TransitionPathAttribute::setValue(const QString inStr)
{
    qDebug() << "TransitionPathAttribute::setValue";
    _pathPoints.clear();

    QString data = inStr;

    data.replace('m','M');
    data.replace('l','L');

    QStringList sl = data.split(" ");

    for (int i=0; i < sl.length(); i++ )
    {
        if ( sl[i].trimmed().isEmpty())
            sl.removeAt(i);
    }

    /* at this point we should have three values, either:
     m 100 100
     or
     L 100 100
       */

    if ( sl.length() < 3) return;

    for (int i=0; i < sl.length(); )
    {
        if ( sl[i] == "M" ) i++;
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
    qDebug() << "leaving TPA Setvalue!";

}

void TransitionPathAttribute::setValue(QList<QPointF>& path )
{
    _pathPoints = path;

    emit changed(this);
}


//////////////////////////////////////////
//
//
//                String Attribute
//
//////////////////////////////////////////



TransitionStringAttribute::TransitionStringAttribute( ):
        IAttribute(),
        _string()
{
}

TransitionStringAttribute::TransitionStringAttribute(QObject * parent, QString key, QString initValue):
        IAttribute(parent,key),
        _string(initValue)
{
}

TransitionStringAttribute::TransitionStringAttribute(const TransitionStringAttribute & stringAttribute ) :
        IAttribute(stringAttribute)
{
}

TransitionStringAttribute::~TransitionStringAttribute()
{
}

TransitionStringAttribute& TransitionStringAttribute::operator=( QString& str)
{
    _string = str;
    return *this;
}

QString TransitionStringAttribute::asString()
{
    return _string;
}



void TransitionStringAttribute::setValue(QString str )
{
    _string = str;
    emit changed(this);

}



//////////////////////////////////////////
//
//
//                Position
//
//////////////////////////////////////////


TransitionPositionAttribute::TransitionPositionAttribute( ) :
        IAttribute(),
        _position()
{
}

TransitionPositionAttribute::TransitionPositionAttribute(QObject * parent, QString key, QPointF initValue) :
        IAttribute(parent,key),
        _position(initValue)
{
}

TransitionPositionAttribute::TransitionPositionAttribute(const TransitionStringAttribute & stringAttribute ) :
        IAttribute(stringAttribute)
{
}

TransitionPositionAttribute::~TransitionPositionAttribute( )
{
}

TransitionPositionAttribute& TransitionPositionAttribute::operator=( QString& str)
{
    this->setValue(str);
    return *this;
}


QString TransitionPositionAttribute::asString()
{
    return (QString::number(_position.x()) + QString(",") +  QString::number(_position.y()));
}


QPointF TransitionPositionAttribute::asQPointF()
{
    return _position;
}


void TransitionPositionAttribute::setValue(const int w,const int h)
{
    _position.setX(w);
    _position.setY(h);
    emit changed(this);

}


void TransitionPositionAttribute::setValue(const QPointF s)
{
    _position = s;
    emit changed(this);

}


void TransitionPositionAttribute::setValue(const QString position)
{
    QStringList sl = position.split(",");
    if ( sl.length() == 2)
    {
        _position.setX( sl[0].toDouble() );
        _position.setY( sl[1].toDouble() );
        emit changed(this);
    }
}




