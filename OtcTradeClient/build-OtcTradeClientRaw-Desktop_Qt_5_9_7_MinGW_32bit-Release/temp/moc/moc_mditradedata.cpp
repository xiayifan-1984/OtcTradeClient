/****************************************************************************
** Meta object code from reading C++ file 'mditradedata.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../OtcTradeClientRaw/guiRaw/form/mditradedata.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mditradedata.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_CMDITradeData_t {
    QByteArrayData data[7];
    char stringdata0[83];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CMDITradeData_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CMDITradeData_t qt_meta_stringdata_CMDITradeData = {
    {
QT_MOC_LITERAL(0, 0, 13), // "CMDITradeData"
QT_MOC_LITERAL(1, 14, 21), // "on_combox_indexChange"
QT_MOC_LITERAL(2, 36, 0), // ""
QT_MOC_LITERAL(3, 37, 5), // "index"
QT_MOC_LITERAL(4, 43, 15), // "onTransactEvent"
QT_MOC_LITERAL(5, 59, 21), // "TransactionEventArgs*"
QT_MOC_LITERAL(6, 81, 1) // "e"

    },
    "CMDITradeData\0on_combox_indexChange\0"
    "\0index\0onTransactEvent\0TransactionEventArgs*\0"
    "e"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CMDITradeData[] = {

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
       4,    1,   27,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, 0x80000000 | 5,    6,

       0        // eod
};

void CMDITradeData::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CMDITradeData *_t = static_cast<CMDITradeData *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->on_combox_indexChange((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->onTransactEvent((*reinterpret_cast< TransactionEventArgs*(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject CMDITradeData::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_CMDITradeData.data,
      qt_meta_data_CMDITradeData,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *CMDITradeData::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CMDITradeData::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CMDITradeData.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int CMDITradeData::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
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
