/****************************************************************************
** Meta object code from reading C++ file 'scgraphicsview.h'
**
** Created: Sun Feb 20 14:27:04 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../scgraphicsview/scgraphicsview.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'scgraphicsview.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_SCGraphicsView[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      25,   16,   15,   15, 0x08,
      50,   15,   15,   15, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_SCGraphicsView[] = {
    "SCGraphicsView\0\0newState\0"
    "handleNewState(SCState*)\0"
    "handleNewTransition(SCTransition*)\0"
};

const QMetaObject SCGraphicsView::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_SCGraphicsView,
      qt_meta_data_SCGraphicsView, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &SCGraphicsView::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *SCGraphicsView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *SCGraphicsView::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SCGraphicsView))
        return static_cast<void*>(const_cast< SCGraphicsView*>(this));
    return QWidget::qt_metacast(_clname);
}

int SCGraphicsView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: handleNewState((*reinterpret_cast< SCState*(*)>(_a[1]))); break;
        case 1: handleNewTransition((*reinterpret_cast< SCTransition*(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 2;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
