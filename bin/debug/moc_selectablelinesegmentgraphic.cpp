/****************************************************************************
** Meta object code from reading C++ file 'selectablelinesegmentgraphic.h'
**
** Created: Sat Feb 26 12:37:16 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../scgraphicsview/selectablelinesegmentgraphic.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'selectablelinesegmentgraphic.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_SelectableLineSegmentGraphic[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      30,   29,   29,   29, 0x05,
      41,   29,   29,   29, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_SelectableLineSegmentGraphic[] = {
    "SelectableLineSegmentGraphic\0\0selected()\0"
    "unselected()\0"
};

const QMetaObject SelectableLineSegmentGraphic::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_SelectableLineSegmentGraphic,
      qt_meta_data_SelectableLineSegmentGraphic, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &SelectableLineSegmentGraphic::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *SelectableLineSegmentGraphic::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *SelectableLineSegmentGraphic::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SelectableLineSegmentGraphic))
        return static_cast<void*>(const_cast< SelectableLineSegmentGraphic*>(this));
    if (!strcmp(_clname, "QGraphicsPolygonItem"))
        return static_cast< QGraphicsPolygonItem*>(const_cast< SelectableLineSegmentGraphic*>(this));
    return QObject::qt_metacast(_clname);
}

int SelectableLineSegmentGraphic::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: selected(); break;
        case 1: unselected(); break;
        default: ;
        }
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void SelectableLineSegmentGraphic::selected()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void SelectableLineSegmentGraphic::unselected()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}
QT_END_MOC_NAMESPACE
