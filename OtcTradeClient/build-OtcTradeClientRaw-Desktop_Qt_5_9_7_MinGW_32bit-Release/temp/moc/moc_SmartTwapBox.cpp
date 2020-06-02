/****************************************************************************
** Meta object code from reading C++ file 'SmartTwapBox.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../OtcTradeClientRaw/guiRaw/sdi/SmartTwapBox.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'SmartTwapBox.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_CSmartTwapBox_t {
    QByteArrayData data[6];
    char stringdata0[108];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CSmartTwapBox_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CSmartTwapBox_t qt_meta_stringdata_CSmartTwapBox = {
    {
QT_MOC_LITERAL(0, 0, 13), // "CSmartTwapBox"
QT_MOC_LITERAL(1, 14, 17), // "onCodePressReturn"
QT_MOC_LITERAL(2, 32, 0), // ""
QT_MOC_LITERAL(3, 33, 19), // "on_inputbtn_clicked"
QT_MOC_LITERAL(4, 53, 28), // "on_bgGroup_direction_toggled"
QT_MOC_LITERAL(5, 82, 25) // "on_bgGroup_offset_toggled"

    },
    "CSmartTwapBox\0onCodePressReturn\0\0"
    "on_inputbtn_clicked\0on_bgGroup_direction_toggled\0"
    "on_bgGroup_offset_toggled"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CSmartTwapBox[] = {

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
       1,    0,   34,    2, 0x08 /* Private */,
       3,    0,   35,    2, 0x08 /* Private */,
       4,    2,   36,    2, 0x08 /* Private */,
       5,    2,   41,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::Bool,    2,    2,
    QMetaType::Void, QMetaType::Int, QMetaType::Bool,    2,    2,

       0        // eod
};

void CSmartTwapBox::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CSmartTwapBox *_t = static_cast<CSmartTwapBox *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->onCodePressReturn(); break;
        case 1: _t->on_inputbtn_clicked(); break;
        case 2: _t->on_bgGroup_direction_toggled((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 3: _t->on_bgGroup_offset_toggled((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        default: ;
        }
    }
}

const QMetaObject CSmartTwapBox::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_CSmartTwapBox.data,
      qt_meta_data_CSmartTwapBox,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *CSmartTwapBox::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CSmartTwapBox::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CSmartTwapBox.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int CSmartTwapBox::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
QT_WARNING_POP
QT_END_MOC_NAMESPACE
