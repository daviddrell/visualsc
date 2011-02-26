/****************************************************************************
** Meta object code from reading C++ file 'scdatamodel.h'
**
** Created: Mon Feb 21 11:43:44 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../scdatamodel/scdatamodel.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'scdatamodel.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_SCDataModel[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: signature, parameters, type, tag, flags
      28,   13,   12,   12, 0x05,
      69,   60,   12,   12, 0x05,
     108,   94,   12,   12, 0x05,

 // slots: signature, parameters, type, tag, flags
     143,   13,   12,   12, 0x08,
     178,   12,   12,   12, 0x08,
     216,   12,   12,   12, 0x08,
     286,   12,   12,   12, 0x08,
     320,  317,   12,   12, 0x08,
     397,   12,   12,   12, 0x08,
     415,   12,   12,   12, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_SCDataModel[] = {
    "SCDataModel\0\0sucess,message\0"
    "openCompleted(bool,QStringList)\0"
    "newState\0newStateSignal(SCState*)\0"
    "newTransition\0newTransitionSignal(SCTransition*)\0"
    "handleReaderDone(bool,QStringList)\0"
    "handleMakeANewState(StateAttributes*)\0"
    "handleMakeANewTransition(TransitionAttributes::TransitionAttributes*)\0"
    "handleLeaveTransitionElement()\0tp\0"
    "handleMakeANewTransitionPath(TransitionAttributes::TransitionPathAttri"
    "bute*)\0"
    "handleTransitUp()\0handleTransitDown()\0"
};

const QMetaObject SCDataModel::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_SCDataModel,
      qt_meta_data_SCDataModel, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &SCDataModel::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *SCDataModel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *SCDataModel::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SCDataModel))
        return static_cast<void*>(const_cast< SCDataModel*>(this));
    return QObject::qt_metacast(_clname);
}

int SCDataModel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: openCompleted((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< QStringList(*)>(_a[2]))); break;
        case 1: newStateSignal((*reinterpret_cast< SCState*(*)>(_a[1]))); break;
        case 2: newTransitionSignal((*reinterpret_cast< SCTransition*(*)>(_a[1]))); break;
        case 3: handleReaderDone((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< QStringList(*)>(_a[2]))); break;
        case 4: handleMakeANewState((*reinterpret_cast< StateAttributes*(*)>(_a[1]))); break;
        case 5: handleMakeANewTransition((*reinterpret_cast< TransitionAttributes::TransitionAttributes*(*)>(_a[1]))); break;
        case 6: handleLeaveTransitionElement(); break;
        case 7: handleMakeANewTransitionPath((*reinterpret_cast< TransitionAttributes::TransitionPathAttribute*(*)>(_a[1]))); break;
        case 8: handleTransitUp(); break;
        case 9: handleTransitDown(); break;
        default: ;
        }
        _id -= 10;
    }
    return _id;
}

// SIGNAL 0
void SCDataModel::openCompleted(bool _t1, QStringList _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void SCDataModel::newStateSignal(SCState * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void SCDataModel::newTransitionSignal(SCTransition * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE
