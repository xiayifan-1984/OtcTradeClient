/****************************************************************************
** Meta object code from reading C++ file 'selectcommodity.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../OtcTradeClientRaw/guiRaw/dialog/selectcommodity.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'selectcommodity.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_SelectCommodity_t {
    QByteArrayData data[13];
    char stringdata0[177];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SelectCommodity_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SelectCommodity_t qt_meta_stringdata_SelectCommodity = {
    {
QT_MOC_LITERAL(0, 0, 15), // "SelectCommodity"
QT_MOC_LITERAL(1, 16, 4), // "onOK"
QT_MOC_LITERAL(2, 21, 0), // ""
QT_MOC_LITERAL(3, 22, 8), // "onCancel"
QT_MOC_LITERAL(4, 31, 17), // "onCodetextChanged"
QT_MOC_LITERAL(5, 49, 4), // "text"
QT_MOC_LITERAL(6, 54, 21), // "on_combox_indexChange"
QT_MOC_LITERAL(7, 76, 5), // "index"
QT_MOC_LITERAL(8, 82, 26), // "on_listwidget_selectChange"
QT_MOC_LITERAL(9, 109, 24), // "onlistcodeDoubleCliecked"
QT_MOC_LITERAL(10, 134, 16), // "QListWidgetItem*"
QT_MOC_LITERAL(11, 151, 11), // "oneditkeyup"
QT_MOC_LITERAL(12, 163, 13) // "oneditkeydown"

    },
    "SelectCommodity\0onOK\0\0onCancel\0"
    "onCodetextChanged\0text\0on_combox_indexChange\0"
    "index\0on_listwidget_selectChange\0"
    "onlistcodeDoubleCliecked\0QListWidgetItem*\0"
    "oneditkeyup\0oneditkeydown"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SelectCommodity[] = {

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
       1,    0,   54,    2, 0x08 /* Private */,
       3,    0,   55,    2, 0x08 /* Private */,
       4,    1,   56,    2, 0x08 /* Private */,
       6,    1,   59,    2, 0x08 /* Private */,
       8,    0,   62,    2, 0x08 /* Private */,
       9,    1,   63,    2, 0x08 /* Private */,
      11,    0,   66,    2, 0x08 /* Private */,
      12,    0,   67,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void, QMetaType::Int,    7,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 10,    2,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void SelectCommodity::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        SelectCommodity *_t = static_cast<SelectCommodity *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->onOK(); break;
        case 1: _t->onCancel(); break;
        case 2: _t->onCodetextChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: _t->on_combox_indexChange((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->on_listwidget_selectChange(); break;
        case 5: _t->onlistcodeDoubleCliecked((*reinterpret_cast< QListWidgetItem*(*)>(_a[1]))); break;
        case 6: _t->oneditkeyup(); break;
        case 7: _t->oneditkeydown(); break;
        default: ;
        }
    }
}

const QMetaObject SelectCommodity::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_SelectCommodity.data,
      qt_meta_data_SelectCommodity,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *SelectCommodity::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SelectCommodity::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_SelectCommodity.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int SelectCommodity::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
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
