/****************************************************************************
** Meta object code from reading C++ file 'sctransition.h'
**
** Created: Sun Feb 13 07:25:25 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../scdatamodel/sctransition.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'sctransition.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_SCTransition[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      14,   13,   13,   13, 0x05,
      25,   13,   13,   13, 0x05,

 // slots: signature, parameters, type, tag, flags
      38,   13,   13,   13, 0x08,
      59,   13,   13,   13, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_SCTransition[] = {
    "SCTransition\0\0selected()\0unselected()\0"
    "handleLineSelected()\0handleLineUnSelected()\0"
};

const QMetaObject SCTransition::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_SCTransition,
      qt_meta_data_SCTransition, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &SCTransition::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *SCTransition::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *SCTransition::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SCTransition))
        return static_cast<void*>(const_cast< SCTransition*>(this));
    return QObject::qt_metacast(_clname);
}

int SCTransition::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: selected(); break;
        case 1: unselected(); break;
        case 2: handleLineSelected(); break;
        case 3: handleLineUnSelected(); break;
        default: ;
        }
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void SCTransition::selected()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void SCTransition::unselected()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}
QT_END_MOC_NAMESPACE
