/****************************************************************************
** Meta object code from reading C++ file 'dataorder.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../OtcTradeClientRaw/guiRaw/form/dataorder.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'dataorder.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MultiOrderWidget_t {
    QByteArrayData data[7];
    char stringdata0[102];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MultiOrderWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MultiOrderWidget_t qt_meta_stringdata_MultiOrderWidget = {
    {
QT_MOC_LITERAL(0, 0, 16), // "MultiOrderWidget"
QT_MOC_LITERAL(1, 17, 21), // "on_combox_indexChange"
QT_MOC_LITERAL(2, 39, 0), // ""
QT_MOC_LITERAL(3, 40, 5), // "index"
QT_MOC_LITERAL(4, 46, 12), // "onOrderEvent"
QT_MOC_LITERAL(5, 59, 15), // "OrderEventArgs*"
QT_MOC_LITERAL(6, 75, 26) // "on_tableview_doubleClicked"

    },
    "MultiOrderWidget\0on_combox_indexChange\0"
    "\0index\0onOrderEvent\0OrderEventArgs*\0"
    "on_tableview_doubleClicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MultiOrderWidget[] = {

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
       4,    1,   32,    2, 0x08 /* Private */,
       6,    1,   35,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, 0x80000000 | 5,    2,
    QMetaType::Void, QMetaType::QModelIndex,    2,

       0        // eod
};

void MultiOrderWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MultiOrderWidget *_t = static_cast<MultiOrderWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->on_combox_indexChange((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->onOrderEvent((*reinterpret_cast< OrderEventArgs*(*)>(_a[1]))); break;
        case 2: _t->on_tableview_doubleClicked((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject MultiOrderWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_MultiOrderWidget.data,
      qt_meta_data_MultiOrderWidget,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *MultiOrderWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MultiOrderWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MultiOrderWidget.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int MultiOrderWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
QT_WARNING_POP
QT_END_MOC_NAMESPACE
