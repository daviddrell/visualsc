#ifndef TRANSISTIONATTRIBUTES_H
#define TRANSISTIONATTRIBUTES_H

#include <QList>
#include <QPointF>
#include <QString>
#include "iattribute.h"

#include "scdatamodel_global.h"
#include <QMetaType>



class  SCDATAMODELSHARED_EXPORT TransitionAttributes: public IAttributeContainer
{
public:

    TransitionAttributes(QObject * parent, QString key);
    TransitionAttributes( const TransitionAttributes& ta);
    TransitionAttributes();
    ~TransitionAttributes();

    virtual AttributeMajorType getMajorType();


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

        virtual int getType();

    private:

        QList<QPointF>  _pathPoints;
    };



    class SCDATAMODELSHARED_EXPORT  TransitionStringAttribute :  public IAttribute
    {
    public:
        TransitionStringAttribute(QObject * parent, QString key, QString initValue);
        TransitionStringAttribute(const TransitionStringAttribute & stringAttribute );
        TransitionStringAttribute();
        ~TransitionStringAttribute();

        TransitionStringAttribute& operator=( QString& str);

        virtual QString asString();

        virtual void    setValue(const QString value)  ;

        virtual int     getType();

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

        virtual int     getType();

    private:

        QPointF _position;
    };


private: // methods

    void initCommon();
};
Q_DECLARE_METATYPE(TransitionAttributes);
Q_DECLARE_METATYPE( TransitionAttributes::TransitionPathAttribute);
Q_DECLARE_METATYPE(TransitionAttributes::TransitionStringAttribute);
Q_DECLARE_METATYPE(TransitionAttributes::TransitionPositionAttribute);

#endif // TRANSISTIONATTRIBUTES_H
