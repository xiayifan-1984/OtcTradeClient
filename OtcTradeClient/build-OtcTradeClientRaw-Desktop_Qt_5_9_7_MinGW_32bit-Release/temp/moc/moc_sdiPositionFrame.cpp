/****************************************************************************
** Meta object code from reading C++ file 'sdiPositionFrame.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../OtcTradeClientRaw/guiRaw/sdi/sdiPositionFrame.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'sdiPositionFrame.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_CSDIPositionFrame_t {
    QByteArrayData data[7];
    char stringdata0[84];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CSDIPositionFrame_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CSDIPositionFrame_t qt_meta_stringdata_CSDIPositionFrame = {
    {
QT_MOC_LITERAL(0, 0, 17), // "CSDIPositionFrame"
QT_MOC_LITERAL(1, 18, 15), // "onTransactEvent"
QT_MOC_LITERAL(2, 34, 0), // ""
QT_MOC_LITERAL(3, 35, 21), // "TransactionEventArgs*"
QT_MOC_LITERAL(4, 57, 1), // "e"
QT_MOC_LITERAL(5, 59, 18), // "onTabSelectChanged"
QT_MOC_LITERAL(6, 78, 5) // "index"

    },
    "CSDIPositionFrame\0onTransactEvent\0\0"
    "TransactionEventArgs*\0e\0onTabSelectChanged\0"
    "index"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CSDIPositionFrame[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   24,    2, 0x08 /* Private */,
       5,    1,   27,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, QMetaType::Int,    6,

       0        // eod
};

void CSDIPositionFrame::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CSDIPositionFrame *_t = static_cast<CSDIPositionFrame *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->onTransactEvent((*reinterpret_cast< TransactionEventArgs*(*)>(_a[1]))); break;
        case 1: _t->onTabSelectChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject CSDIPositionFrame::staticMetaObject = {
    { &QFrame::staticMetaObject, qt_meta_stringdata_CSDIPositionFrame.data,
      qt_meta_data_CSDIPositionFrame,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *CSDIPositionFrame::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CSDIPositionFrame::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CSDIPositionFrame.stringdata0))
        return static_cast<void*>(this);
    return QFrame::qt_metacast(_clname);
}

int CSDIPositionFrame::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QFrame::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
