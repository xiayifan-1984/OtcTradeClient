/****************************************************************************
** Meta object code from reading C++ file 'tradeSingleData.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../OtcTradeClientRaw/guiRaw/sdi/tradeSingleData.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'tradeSingleData.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_CTradeSingleData_t {
    QByteArrayData data[5];
    char stringdata0[58];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CTradeSingleData_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CTradeSingleData_t qt_meta_stringdata_CTradeSingleData = {
    {
QT_MOC_LITERAL(0, 0, 16), // "CTradeSingleData"
QT_MOC_LITERAL(1, 17, 15), // "onTransactEvent"
QT_MOC_LITERAL(2, 33, 0), // ""
QT_MOC_LITERAL(3, 34, 21), // "TransactionEventArgs*"
QT_MOC_LITERAL(4, 56, 1) // "e"

    },
    "CTradeSingleData\0onTransactEvent\0\0"
    "TransactionEventArgs*\0e"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CTradeSingleData[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   19,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3,    4,

       0        // eod
};

void CTradeSingleData::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CTradeSingleData *_t = static_cast<CTradeSingleData *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->onTransactEvent((*reinterpret_cast< TransactionEventArgs*(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject CTradeSingleData::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_CTradeSingleData.data,
      qt_meta_data_CTradeSingleData,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *CTradeSingleData::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CTradeSingleData::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CTradeSingleData.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int CTradeSingleData::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 1;
    }
    return _id;
}
struct qt_meta_stringdata_CTradeTotalData_t {
    QByteArrayData data[5];
    char stringdata0[57];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CTradeTotalData_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CTradeTotalData_t qt_meta_stringdata_CTradeTotalData = {
    {
QT_MOC_LITERAL(0, 0, 15), // "CTradeTotalData"
QT_MOC_LITERAL(1, 16, 15), // "onTransactEvent"
QT_MOC_LITERAL(2, 32, 0), // ""
QT_MOC_LITERAL(3, 33, 21), // "TransactionEventArgs*"
QT_MOC_LITERAL(4, 55, 1) // "e"

    },
    "CTradeTotalData\0onTransactEvent\0\0"
    "TransactionEventArgs*\0e"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CTradeTotalData[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   19,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3,    4,

       0        // eod
};

void CTradeTotalData::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CTradeTotalData *_t = static_cast<CTradeTotalData *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->onTransactEvent((*reinterpret_cast< TransactionEventArgs*(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject CTradeTotalData::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_CTradeTotalData.data,
      qt_meta_data_CTradeTotalData,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *CTradeTotalData::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CTradeTotalData::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CTradeTotalData.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int CTradeTotalData::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 1;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
