/****************************************************************************
** Meta object code from reading C++ file 'orderSingleData.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../OtcTradeClientRaw/guiRaw/sdi/orderSingleData.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'orderSingleData.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_CTwapSingleData_t {
    QByteArrayData data[8];
    char stringdata0[113];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CTwapSingleData_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CTwapSingleData_t qt_meta_stringdata_CTwapSingleData = {
    {
QT_MOC_LITERAL(0, 0, 15), // "CTwapSingleData"
QT_MOC_LITERAL(1, 16, 24), // "onDoubleClickedTableView"
QT_MOC_LITERAL(2, 41, 0), // ""
QT_MOC_LITERAL(3, 42, 12), // "onOrderEvent"
QT_MOC_LITERAL(4, 55, 15), // "TwapOrderEvent*"
QT_MOC_LITERAL(5, 71, 6), // "pEvent"
QT_MOC_LITERAL(6, 78, 18), // "onInsertOrderEvent"
QT_MOC_LITERAL(7, 97, 15) // "OrderEventArgs*"

    },
    "CTwapSingleData\0onDoubleClickedTableView\0"
    "\0onOrderEvent\0TwapOrderEvent*\0pEvent\0"
    "onInsertOrderEvent\0OrderEventArgs*"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CTwapSingleData[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   29,    2, 0x08 /* Private */,
       3,    1,   32,    2, 0x08 /* Private */,
       6,    1,   35,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::QModelIndex,    2,
    QMetaType::Void, 0x80000000 | 4,    5,
    QMetaType::Void, 0x80000000 | 7,    5,

       0        // eod
};

void CTwapSingleData::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CTwapSingleData *_t = static_cast<CTwapSingleData *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->onDoubleClickedTableView((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 1: _t->onOrderEvent((*reinterpret_cast< TwapOrderEvent*(*)>(_a[1]))); break;
        case 2: _t->onInsertOrderEvent((*reinterpret_cast< OrderEventArgs*(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject CTwapSingleData::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_CTwapSingleData.data,
      qt_meta_data_CTwapSingleData,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *CTwapSingleData::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CTwapSingleData::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CTwapSingleData.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int CTwapSingleData::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 3;
    }
    return _id;
}
struct qt_meta_stringdata_COrderSingleData_t {
    QByteArrayData data[5];
    char stringdata0[74];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_COrderSingleData_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_COrderSingleData_t qt_meta_stringdata_COrderSingleData = {
    {
QT_MOC_LITERAL(0, 0, 16), // "COrderSingleData"
QT_MOC_LITERAL(1, 17, 12), // "onOrderEvent"
QT_MOC_LITERAL(2, 30, 0), // ""
QT_MOC_LITERAL(3, 31, 15), // "OrderEventArgs*"
QT_MOC_LITERAL(4, 47, 26) // "on_tableview_doubleClicked"

    },
    "COrderSingleData\0onOrderEvent\0\0"
    "OrderEventArgs*\0on_tableview_doubleClicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_COrderSingleData[] = {

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
    QMetaType::Void, 0x80000000 | 3,    2,
    QMetaType::Void, QMetaType::QModelIndex,    2,

       0        // eod
};

void COrderSingleData::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        COrderSingleData *_t = static_cast<COrderSingleData *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->onOrderEvent((*reinterpret_cast< OrderEventArgs*(*)>(_a[1]))); break;
        case 1: _t->on_tableview_doubleClicked((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject COrderSingleData::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_COrderSingleData.data,
      qt_meta_data_COrderSingleData,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *COrderSingleData::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *COrderSingleData::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_COrderSingleData.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int COrderSingleData::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
struct qt_meta_stringdata_CParkSingleData_t {
    QByteArrayData data[6];
    char stringdata0[78];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CParkSingleData_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CParkSingleData_t qt_meta_stringdata_CParkSingleData = {
    {
QT_MOC_LITERAL(0, 0, 15), // "CParkSingleData"
QT_MOC_LITERAL(1, 16, 24), // "onDoubleClickedTableView"
QT_MOC_LITERAL(2, 41, 0), // ""
QT_MOC_LITERAL(3, 42, 12), // "onOrderEvent"
QT_MOC_LITERAL(4, 55, 15), // "ParkOrderEvent*"
QT_MOC_LITERAL(5, 71, 6) // "pEvent"

    },
    "CParkSingleData\0onDoubleClickedTableView\0"
    "\0onOrderEvent\0ParkOrderEvent*\0pEvent"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CParkSingleData[] = {

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
       3,    1,   27,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::QModelIndex,    2,
    QMetaType::Void, 0x80000000 | 4,    5,

       0        // eod
};

void CParkSingleData::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CParkSingleData *_t = static_cast<CParkSingleData *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->onDoubleClickedTableView((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 1: _t->onOrderEvent((*reinterpret_cast< ParkOrderEvent*(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject CParkSingleData::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_CParkSingleData.data,
      qt_meta_data_CParkSingleData,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *CParkSingleData::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CParkSingleData::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CParkSingleData.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int CParkSingleData::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
