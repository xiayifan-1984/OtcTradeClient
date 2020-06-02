/****************************************************************************
** Meta object code from reading C++ file 'SmartParkBox.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../OtcTradeClientRaw/guiRaw/sdi/SmartParkBox.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'SmartParkBox.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_CSmartParkBox_t {
    QByteArrayData data[12];
    char stringdata0[194];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CSmartParkBox_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CSmartParkBox_t qt_meta_stringdata_CSmartParkBox = {
    {
QT_MOC_LITERAL(0, 0, 13), // "CSmartParkBox"
QT_MOC_LITERAL(1, 14, 12), // "onQuoteClick"
QT_MOC_LITERAL(2, 27, 0), // ""
QT_MOC_LITERAL(3, 28, 3), // "idx"
QT_MOC_LITERAL(4, 32, 17), // "onCodePressReturn"
QT_MOC_LITERAL(5, 50, 12), // "onOrderEvent"
QT_MOC_LITERAL(6, 63, 15), // "OrderEventArgs*"
QT_MOC_LITERAL(7, 79, 28), // "on_bgGroup_direction_toggled"
QT_MOC_LITERAL(8, 108, 25), // "on_bgGroup_offset_toggled"
QT_MOC_LITERAL(9, 134, 19), // "on_inputbtn_clicked"
QT_MOC_LITERAL(10, 154, 20), // "on_cancelbtn_clicked"
QT_MOC_LITERAL(11, 175, 18) // "on_parkbtn_clicked"

    },
    "CSmartParkBox\0onQuoteClick\0\0idx\0"
    "onCodePressReturn\0onOrderEvent\0"
    "OrderEventArgs*\0on_bgGroup_direction_toggled\0"
    "on_bgGroup_offset_toggled\0on_inputbtn_clicked\0"
    "on_cancelbtn_clicked\0on_parkbtn_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CSmartParkBox[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   54,    2, 0x08 /* Private */,
       4,    0,   57,    2, 0x08 /* Private */,
       5,    1,   58,    2, 0x08 /* Private */,
       7,    2,   61,    2, 0x08 /* Private */,
       8,    2,   66,    2, 0x08 /* Private */,
       9,    0,   71,    2, 0x08 /* Private */,
      10,    0,   72,    2, 0x08 /* Private */,
      11,    0,   73,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 6,    2,
    QMetaType::Void, QMetaType::Int, QMetaType::Bool,    2,    2,
    QMetaType::Void, QMetaType::Int, QMetaType::Bool,    2,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void CSmartParkBox::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CSmartParkBox *_t = static_cast<CSmartParkBox *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->onQuoteClick((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->onCodePressReturn(); break;
        case 2: _t->onOrderEvent((*reinterpret_cast< OrderEventArgs*(*)>(_a[1]))); break;
        case 3: _t->on_bgGroup_direction_toggled((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 4: _t->on_bgGroup_offset_toggled((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 5: _t->on_inputbtn_clicked(); break;
        case 6: _t->on_cancelbtn_clicked(); break;
        case 7: _t->on_parkbtn_clicked(); break;
        default: ;
        }
    }
}

const QMetaObject CSmartParkBox::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_CSmartParkBox.data,
      qt_meta_data_CSmartParkBox,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *CSmartParkBox::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CSmartParkBox::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CSmartParkBox.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int CSmartParkBox::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 8;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
