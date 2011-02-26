/****************************************************************************
** Meta object code from reading C++ file 'stateboxgraphic.h'
**
** Created: Sat Feb 26 13:08:35 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../scgraphicsview/stateboxgraphic.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'stateboxgraphic.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_StateBoxGraphic[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      22,   17,   16,   16, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_StateBoxGraphic[] = {
    "StateBoxGraphic\0\0attr\0"
    "handleAttributeChanged(IAttribute*)\0"
};

const QMetaObject StateBoxGraphic::staticMetaObject = {
    { &QGraphicsObject::staticMetaObject, qt_meta_stringdata_StateBoxGraphic,
      qt_meta_data_StateBoxGraphic, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &StateBoxGraphic::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *StateBoxGraphic::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *StateBoxGraphic::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_StateBoxGraphic))
        return static_cast<void*>(const_cast< StateBoxGraphic*>(this));
    return QGraphicsObject::qt_metacast(_clname);
}

int StateBoxGraphic::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGraphicsObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: handleAttributeChanged((*reinterpret_cast< IAttribute*(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 1;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
