/****************************************************************************
** Meta object code from reading C++ file 'scxmlreader.h'
**
** Created: Sat Feb 26 12:37:07 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../scdatamodel/scxmlreader.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'scxmlreader.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_SCXMLReader[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      12,       // signalCount

 // signals: signature, parameters, type, tag, flags
      28,   13,   12,   12, 0x05,
      51,   12,   12,   12, 0x05,
      83,   12,   12,   12, 0x05,
     103,   12,   12,   12, 0x05,
     123,   12,   12,   12, 0x05,
     149,   12,   12,   12, 0x05,
     175,   12,   12,   12, 0x05,
     218,   12,   12,   12, 0x05,
     247,   12,   12,   12, 0x05,
     281,  276,   12,   12, 0x05,
     314,   12,   12,   12, 0x05,
     329,   12,   12,   12, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_SCXMLReader[] = {
    "SCXMLReader\0\0result,message\0"
    "done(bool,QStringList)\0"
    "makeANewState(StateAttributes*)\0"
    "enterStateElement()\0leaveStateElement()\0"
    "enterTransistionElement()\0"
    "leaveTransistionElement()\0"
    "makeANewTransistion(TransitionAttributes*)\0"
    "enterTransitionPathElement()\0"
    "leaveTransitionPathElement()\0path\0"
    "makeANewTransistionPath(QString)\0"
    "leaveElement()\0enterElement()\0"
};

const QMetaObject SCXMLReader::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_SCXMLReader,
      qt_meta_data_SCXMLReader, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &SCXMLReader::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *SCXMLReader::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *SCXMLReader::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SCXMLReader))
        return static_cast<void*>(const_cast< SCXMLReader*>(this));
    return QThread::qt_metacast(_clname);
}

int SCXMLReader::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: done((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< QStringList(*)>(_a[2]))); break;
        case 1: makeANewState((*reinterpret_cast< StateAttributes*(*)>(_a[1]))); break;
        case 2: enterStateElement(); break;
        case 3: leaveStateElement(); break;
        case 4: enterTransistionElement(); break;
        case 5: leaveTransistionElement(); break;
        case 6: makeANewTransistion((*reinterpret_cast< TransitionAttributes*(*)>(_a[1]))); break;
        case 7: enterTransitionPathElement(); break;
        case 8: leaveTransitionPathElement(); break;
        case 9: makeANewTransistionPath((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 10: leaveElement(); break;
        case 11: enterElement(); break;
        default: ;
        }
        _id -= 12;
    }
    return _id;
}

// SIGNAL 0
void SCXMLReader::done(bool _t1, QStringList _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void SCXMLReader::makeANewState(StateAttributes * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void SCXMLReader::enterStateElement()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}

// SIGNAL 3
void SCXMLReader::leaveStateElement()
{
    QMetaObject::activate(this, &staticMetaObject, 3, 0);
}

// SIGNAL 4
void SCXMLReader::enterTransistionElement()
{
    QMetaObject::activate(this, &staticMetaObject, 4, 0);
}

// SIGNAL 5
void SCXMLReader::leaveTransistionElement()
{
    QMetaObject::activate(this, &staticMetaObject, 5, 0);
}

// SIGNAL 6
void SCXMLReader::makeANewTransistion(TransitionAttributes * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void SCXMLReader::enterTransitionPathElement()
{
    QMetaObject::activate(this, &staticMetaObject, 7, 0);
}

// SIGNAL 8
void SCXMLReader::leaveTransitionPathElement()
{
    QMetaObject::activate(this, &staticMetaObject, 8, 0);
}

// SIGNAL 9
void SCXMLReader::makeANewTransistionPath(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 9, _a);
}

// SIGNAL 10
void SCXMLReader::leaveElement()
{
    QMetaObject::activate(this, &staticMetaObject, 10, 0);
}

// SIGNAL 11
void SCXMLReader::enterElement()
{
    QMetaObject::activate(this, &staticMetaObject, 11, 0);
}
QT_END_MOC_NAMESPACE
