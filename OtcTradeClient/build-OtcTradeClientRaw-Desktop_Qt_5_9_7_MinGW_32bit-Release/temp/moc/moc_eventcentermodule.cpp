/****************************************************************************
** Meta object code from reading C++ file 'eventcentermodule.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../OtcTradeClientRaw/module/eventcentermodule.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'eventcentermodule.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_QEventCenter_t {
    QByteArrayData data[14];
    char stringdata0[209];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QEventCenter_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QEventCenter_t qt_meta_stringdata_QEventCenter = {
    {
QT_MOC_LITERAL(0, 0, 12), // "QEventCenter"
QT_MOC_LITERAL(1, 13, 14), // "fireOrderEvent"
QT_MOC_LITERAL(2, 28, 0), // ""
QT_MOC_LITERAL(3, 29, 15), // "OrderEventArgs*"
QT_MOC_LITERAL(4, 45, 4), // "parg"
QT_MOC_LITERAL(5, 50, 17), // "fireTransactEvent"
QT_MOC_LITERAL(6, 68, 21), // "TransactionEventArgs*"
QT_MOC_LITERAL(7, 90, 18), // "fireParkOrderEvent"
QT_MOC_LITERAL(8, 109, 15), // "ParkOrderEvent*"
QT_MOC_LITERAL(9, 125, 5), // "event"
QT_MOC_LITERAL(10, 131, 22), // "fireTwapTwapOrderEvent"
QT_MOC_LITERAL(11, 154, 15), // "TwapOrderEvent*"
QT_MOC_LITERAL(12, 170, 18), // "fireSysNotifyEvent"
QT_MOC_LITERAL(13, 189, 19) // "SysNotifyEventArgs*"

    },
    "QEventCenter\0fireOrderEvent\0\0"
    "OrderEventArgs*\0parg\0fireTransactEvent\0"
    "TransactionEventArgs*\0fireParkOrderEvent\0"
    "ParkOrderEvent*\0event\0fireTwapTwapOrderEvent\0"
    "TwapOrderEvent*\0fireSysNotifyEvent\0"
    "SysNotifyEventArgs*"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QEventCenter[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   39,    2, 0x06 /* Public */,
       5,    1,   42,    2, 0x06 /* Public */,
       7,    1,   45,    2, 0x06 /* Public */,
      10,    1,   48,    2, 0x06 /* Public */,
      12,    1,   51,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 6,    4,
    QMetaType::Void, 0x80000000 | 8,    9,
    QMetaType::Void, 0x80000000 | 11,    9,
    QMetaType::Void, 0x80000000 | 13,    4,

       0        // eod
};

void QEventCenter::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        QEventCenter *_t = static_cast<QEventCenter *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->fireOrderEvent((*reinterpret_cast< OrderEventArgs*(*)>(_a[1]))); break;
        case 1: _t->fireTransactEvent((*reinterpret_cast< TransactionEventArgs*(*)>(_a[1]))); break;
        case 2: _t->fireParkOrderEvent((*reinterpret_cast< ParkOrderEvent*(*)>(_a[1]))); break;
        case 3: _t->fireTwapTwapOrderEvent((*reinterpret_cast< TwapOrderEvent*(*)>(_a[1]))); break;
        case 4: _t->fireSysNotifyEvent((*reinterpret_cast< SysNotifyEventArgs*(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (QEventCenter::*_t)(OrderEventArgs * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&QEventCenter::fireOrderEvent)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (QEventCenter::*_t)(TransactionEventArgs * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&QEventCenter::fireTransactEvent)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (QEventCenter::*_t)(ParkOrderEvent * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&QEventCenter::fireParkOrderEvent)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (QEventCenter::*_t)(TwapOrderEvent * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&QEventCenter::fireTwapTwapOrderEvent)) {
                *result = 3;
                return;
            }
        }
        {
            typedef void (QEventCenter::*_t)(SysNotifyEventArgs * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&QEventCenter::fireSysNotifyEvent)) {
                *result = 4;
                return;
            }
        }
    }
}

const QMetaObject QEventCenter::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_QEventCenter.data,
      qt_meta_data_QEventCenter,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *QEventCenter::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QEventCenter::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_QEventCenter.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int QEventCenter::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void QEventCenter::fireOrderEvent(OrderEventArgs * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void QEventCenter::fireTransactEvent(TransactionEventArgs * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void QEventCenter::fireParkOrderEvent(ParkOrderEvent * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void QEventCenter::fireTwapTwapOrderEvent(TwapOrderEvent * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void QEventCenter::fireSysNotifyEvent(SysNotifyEventArgs * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
