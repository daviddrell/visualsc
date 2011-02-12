#ifndef STATEATTRIBUTES_H
#define STATEATTRIBUTES_H

#include <QString>
#include <QStringList>
#include <QPoint>

#include "scdatamodel_global.h"



class  SCDATAMODELSHARED_EXPORT StateAttributes
{
public:
    StateAttributes();
    StateAttributes(StateAttributes & sa);



    class StateName
    {
    public:
        StateName():_name(){}
        StateName(QString n):_name(n){}

        QString asString(){return _name;}
        void    set(QString name){ _name = name;}
    private :
            QString _name;
    };

    class StateSize
    {
    public:
        StateSize(): _size(){}
        StateSize(QPoint s): _size(s){}

        QString asString()
        {
            return (QString::number(_size.x()) + QString(",") +  QString::number(_size.y()));
        }
        QPoint asPoint()
        {
            return (_size);
        }

        void    set(QString size)
        {
            QStringList sl = size.split(",");
            if ( sl.length() == 2)
            {
                _size.setX( sl[0].toInt() );
                _size.setY( sl[1].toInt() );
            }
        }
        void    set(int w, int h){_size.setX(w); _size.setY(h);}
        void    set(QPoint s){_size.setX(s.x()); _size.setY(s.y());}
    private :
            QPoint _size;
    };

    class StatePosition
    {
    public:
        StatePosition() : _position(){}
        StatePosition(QPoint p) : _position(p){}

        QString asString()
        {
            return (QString::number(_position.x()) + QString(",") +  QString::number(_position.y()));
        }
        QPoint asPoint()
        {
            return (_position);
        }

        void    set(QString position)
        {
            QStringList sl = position.split(",");
            if ( sl.length() == 2)
            {
                _position.setX( sl[0].toInt() );
                _position.setY( sl[1].toInt() );
            }
        }
        void    set(int w, int h){_position.setX(w); _position.setY(h);}
        void    set(QPoint p){_position.setX(p.x()); _position.setY(p.y());}
    private :
            QPoint _position;
    };

    StateName name;
    StateSize size;
    StatePosition position;
    bool          hasBeenSized;

};

#endif // STATEATTRIBUTES_H
