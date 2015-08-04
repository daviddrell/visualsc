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

#ifndef TRANSISTIONATTRIBUTES_H
#define TRANSISTIONATTRIBUTES_H

#include <QList>
#include <QPointF>
#include <QString>
#include "iattribute.h"
#include "scdatamodel_global.h"
#include <QMetaType>


/**
 * \class TransitionAttributes
 *
 * \ingroup DataModel
 * \ingroup Attributes
 *
 */


class  SCDATAMODELSHARED_EXPORT TransitionAttributes: public IAttributeContainer
{
    Q_OBJECT
public:

    TransitionAttributes(QObject * parent, QString key);
    TransitionAttributes( const TransitionAttributes& ta);
    //TransitionAttributes(IAttributeContainer&);
    TransitionAttributes();
    ~TransitionAttributes();

    void addAttribute(const QString attrName);
    virtual void setAttributes(const IAttributeContainer& sourceAttrList);


private: // methods

    //void addAllAttributes(IAttributeContainer&);
    void initCommon();
};
    class SCDATAMODELSHARED_EXPORT  TransitionPathAttribute : public  IAttribute
    {
        //<path d="M 100 100 L 300 100 L 200 300 z" />

    public:
        TransitionPathAttribute(QObject * parent, QString key, QList<QPointF>& initPath);
        TransitionPathAttribute(const TransitionPathAttribute & pathAttributes );
        TransitionPathAttribute();
        ~TransitionPathAttribute();

        TransitionPathAttribute& operator=( QList<QPointF>& path );

        virtual QString         asString();
        virtual QList<QPointF>  asQPointFList();

        virtual void setValue(const QString value)  ;

        virtual void setValue(QList<QPointF>& path )  ;

    private:

        QList<QPointF>  _pathPoints;
    };



    class SCDATAMODELSHARED_EXPORT  TransitionStringAttribute :  public IAttribute
    {

        Q_OBJECT

    public:
        TransitionStringAttribute(QObject * parent, QString key, QString initValue);
        TransitionStringAttribute(const TransitionStringAttribute & stringAttribute );
        TransitionStringAttribute();
        ~TransitionStringAttribute();

        TransitionStringAttribute& operator=( QString& str);

        virtual QString asString();

        virtual void    setValue(const QString value)  ;

    signals:
        void changed(TransitionStringAttribute* tsa);

    private:

        QString _string;
    };


    class SCDATAMODELSHARED_EXPORT  TransitionPositionAttribute :  public IAttribute
    {
    public:
        TransitionPositionAttribute(QObject * parent, QString key, QPointF initValue);
        TransitionPositionAttribute(const TransitionStringAttribute & stringAttribute );
        TransitionPositionAttribute();
        ~TransitionPositionAttribute();
        TransitionPositionAttribute& operator=( QString& str);

        virtual QString asString();
        virtual QPointF asQPointF();
        virtual void    setValue(const QString position)  ;
        void            setValue(const int w,const int h);
        void            setValue(const QPointF s);

    private:

        QPointF _position;
    };



Q_DECLARE_METATYPE(TransitionAttributes);
Q_DECLARE_METATYPE(TransitionPathAttribute);
Q_DECLARE_METATYPE(TransitionStringAttribute);
Q_DECLARE_METATYPE(TransitionPositionAttribute);

#endif // TRANSISTIONATTRIBUTES_H
