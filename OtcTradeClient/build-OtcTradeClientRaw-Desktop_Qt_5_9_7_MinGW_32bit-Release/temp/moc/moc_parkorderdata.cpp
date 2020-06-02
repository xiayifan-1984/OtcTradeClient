/****************************************************************************
** Meta object code from reading C++ file 'parkorderdata.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../OtcTradeClientRaw/guiRaw/form/parkorderdata.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'parkorderdata.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_CParkOrderData_t {
    QByteArrayData data[8];
    char stringdata0[109];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CParkOrderData_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CParkOrderData_t qt_meta_stringdata_CParkOrderData = {
    {
QT_MOC_LITERAL(0, 0, 14), // "CParkOrderData"
QT_MOC_LITERAL(1, 15, 24), // "onDoubleClickedTableView"
QT_MOC_LITERAL(2, 40, 0), // ""
QT_MOC_LITERAL(3, 41, 16), // "onParkOrderEvent"
QT_MOC_LITERAL(4, 58, 15), // "ParkOrderEvent*"
QT_MOC_LITERAL(5, 74, 6), // "pEvent"
QT_MOC_LITERAL(6, 81, 21), // "on_combox_indexChange"
QT_MOC_LITERAL(7, 103, 5) // "index"

    },
    "CParkOrderData\0onDoubleClickedTableView\0"
    "\0onParkOrderEvent\0ParkOrderEvent*\0"
    "pEvent\0on_combox_indexChange\0index"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CParkOrderData[] = {

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
    QMetaType::Void, QMetaType::Int,    7,

       0        // eod
};

void CParkOrderData::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CParkOrderData *_t = static_cast<CParkOrderData *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->onDoubleClickedTableView((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 1: _t->onParkOrderEvent((*reinterpret_cast< ParkOrderEvent*(*)>(_a[1]))); break;
        case 2: _t->on_combox_indexChange((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject CParkOrderData::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_CParkOrderData.data,
      qt_meta_data_CParkOrderData,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *CParkOrderData::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CParkOrderData::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CParkOrderData.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int CParkOrderData::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
struct qt_meta_stringdata_ParkOrderTable_t {
    QByteArrayData data[1];
    char stringdata0[15];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ParkOrderTable_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ParkOrderTable_t qt_meta_stringdata_ParkOrderTable = {
    {
QT_MOC_LITERAL(0, 0, 14) // "ParkOrderTable"

    },
    "ParkOrderTable"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ParkOrderTable[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

void ParkOrderTable::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObject ParkOrderTable::staticMetaObject = {
    { &QTableView::staticMetaObject, qt_meta_stringdata_ParkOrderTable.data,
      qt_meta_data_ParkOrderTable,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *ParkOrderTable::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ParkOrderTable::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ParkOrderTable.stringdata0))
        return static_cast<void*>(this);
    return QTableView::qt_metacast(_clname);
}

int ParkOrderTable::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QTableView::qt_metacall(_c, _id, _a);
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
