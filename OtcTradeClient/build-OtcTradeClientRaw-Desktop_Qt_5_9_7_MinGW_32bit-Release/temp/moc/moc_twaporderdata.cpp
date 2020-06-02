/****************************************************************************
** Meta object code from reading C++ file 'twaporderdata.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../OtcTradeClientRaw/guiRaw/form/twaporderdata.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'twaporderdata.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_CTwapOrderData_t {
    QByteArrayData data[10];
    char stringdata0[140];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CTwapOrderData_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CTwapOrderData_t qt_meta_stringdata_CTwapOrderData = {
    {
QT_MOC_LITERAL(0, 0, 14), // "CTwapOrderData"
QT_MOC_LITERAL(1, 15, 24), // "onDoubleClickedTableView"
QT_MOC_LITERAL(2, 40, 0), // ""
QT_MOC_LITERAL(3, 41, 12), // "onOrderEvent"
QT_MOC_LITERAL(4, 54, 15), // "TwapOrderEvent*"
QT_MOC_LITERAL(5, 70, 6), // "pEvent"
QT_MOC_LITERAL(6, 77, 18), // "onInsertOrderEvent"
QT_MOC_LITERAL(7, 96, 15), // "OrderEventArgs*"
QT_MOC_LITERAL(8, 112, 21), // "on_combox_indexChange"
QT_MOC_LITERAL(9, 134, 5) // "index"

    },
    "CTwapOrderData\0onDoubleClickedTableView\0"
    "\0onOrderEvent\0TwapOrderEvent*\0pEvent\0"
    "onInsertOrderEvent\0OrderEventArgs*\0"
    "on_combox_indexChange\0index"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CTwapOrderData[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   34,    2, 0x08 /* Private */,
       3,    1,   37,    2, 0x08 /* Private */,
       6,    1,   40,    2, 0x08 /* Private */,
       8,    1,   43,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::QModelIndex,    2,
    QMetaType::Void, 0x80000000 | 4,    5,
    QMetaType::Void, 0x80000000 | 7,    5,
    QMetaType::Void, QMetaType::Int,    9,

       0        // eod
};

void CTwapOrderData::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CTwapOrderData *_t = static_cast<CTwapOrderData *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->onDoubleClickedTableView((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 1: _t->onOrderEvent((*reinterpret_cast< TwapOrderEvent*(*)>(_a[1]))); break;
        case 2: _t->onInsertOrderEvent((*reinterpret_cast< OrderEventArgs*(*)>(_a[1]))); break;
        case 3: _t->on_combox_indexChange((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject CTwapOrderData::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_CTwapOrderData.data,
      qt_meta_data_CTwapOrderData,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *CTwapOrderData::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CTwapOrderData::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CTwapOrderData.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int CTwapOrderData::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}
struct qt_meta_stringdata_TwapOrderTable_t {
    QByteArrayData data[1];
    char stringdata0[15];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_TwapOrderTable_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_TwapOrderTable_t qt_meta_stringdata_TwapOrderTable = {
    {
QT_MOC_LITERAL(0, 0, 14) // "TwapOrderTable"

    },
    "TwapOrderTable"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_TwapOrderTable[] = {

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

void TwapOrderTable::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObject TwapOrderTable::staticMetaObject = {
    { &QTableView::staticMetaObject, qt_meta_stringdata_TwapOrderTable.data,
      qt_meta_data_TwapOrderTable,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *TwapOrderTable::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TwapOrderTable::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_TwapOrderTable.stringdata0))
        return static_cast<void*>(this);
    return QTableView::qt_metacast(_clname);
}

int TwapOrderTable::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QTableView::qt_metacall(_c, _id, _a);
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
