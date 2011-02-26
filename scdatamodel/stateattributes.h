#ifndef STATEATTRIBUTES_H
#define STATEATTRIBUTES_H

#include <QString>
#include <QStringList>
#include <QPoint>

#include "iattribute.h"

#include "scdatamodel_global.h"
#include <QPoint>
#include <QString>
#include <QMetaType>


class  SCDATAMODELSHARED_EXPORT StateAttributes : public IAttributeContainer
{
public:
    StateAttributes();


    StateAttributes(QObject * parent, QString key);

    StateAttributes(const StateAttributes& sa);

    StateAttributes& operator=( StateAttributes& sa );


    virtual AttributeMajorType getMajorType();

    ~StateAttributes() ;

      //  NAME


    class StateName :  public IAttribute
    {
    public:

        StateName(QObject*  parent,QString key,QString n);
        StateName();
        ~StateName();

        StateName& operator=( StateName& sa );

        QString asString();
        void    setValue(const QString name);
        int getType() ;

    private :
            QString _name;
    };



    //  SIZE


    class StateSize: public IAttribute
    {
    public:

        StateSize();
        StateSize(QObject*  parent, QString key, QPoint s);
        StateSize(QObject*  parent, QString key, QString s);

        ~StateSize();

        StateSize& operator=( StateSize& sa );


        int getType() ;

        QString asString();
        QPointF asPointF() const;

        void    setValue(QString size);
        void    setValue(const int w,const int h);
        void    setValue(const QPointF s);
    private :
            QPointF _size;
    };



    // POSITION


    class StatePosition:  public IAttribute
    {
    public:
        StatePosition();

        StatePosition(QObject*  parent,QString key,QPointF p);

        StatePosition(QObject*  parent,QString key,QString s);

        ~StatePosition();

        StatePosition& operator=( StatePosition& sa );

        int getType();

        QString asString();
        QPointF asPointF() const;

        void    setValue(const QString position);
        void    setValue(int w, int h);
        void    setValue(const QPointF p);
    private :
            QPointF _position;

    };

    class StateString:  public IAttribute
    {
    public:
        StateString(QObject*  parent,QString key,QString s);
        StateString() ;
        ~StateString() ;

        StateString& operator=( StateString& sa );

        int getType();

        QString asString();


        void    setValue(const QString value);

        private :
            QString _value;

    };

};

Q_DECLARE_METATYPE(StateAttributes);
Q_DECLARE_METATYPE( StateAttributes::StateName);
Q_DECLARE_METATYPE( StateAttributes::StateSize);
Q_DECLARE_METATYPE(StateAttributes::StatePosition);
Q_DECLARE_METATYPE(StateAttributes::StateString);

#endif // STATEATTRIBUTES_H
