/****************************************************************************
** Meta object code from reading C++ file 'twapinputbox.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../OtcTradeClientRaw/guiRaw/form/twapinputbox.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'twapinputbox.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_CTwapInputBox_t {
    QByteArrayData data[10];
    char stringdata0[138];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CTwapInputBox_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CTwapInputBox_t qt_meta_stringdata_CTwapInputBox = {
    {
QT_MOC_LITERAL(0, 0, 13), // "CTwapInputBox"
QT_MOC_LITERAL(1, 14, 14), // "onClickOpenChk"
QT_MOC_LITERAL(2, 29, 0), // ""
QT_MOC_LITERAL(3, 30, 15), // "onClickCloseChk"
QT_MOC_LITERAL(4, 46, 17), // "onClickCloseToday"
QT_MOC_LITERAL(5, 64, 14), // "onClickAutoChk"
QT_MOC_LITERAL(6, 79, 13), // "onClickBuyChk"
QT_MOC_LITERAL(7, 93, 14), // "onClickSellChk"
QT_MOC_LITERAL(8, 108, 17), // "onCodePressReturn"
QT_MOC_LITERAL(9, 126, 11) // "onClickTwap"

    },
    "CTwapInputBox\0onClickOpenChk\0\0"
    "onClickCloseChk\0onClickCloseToday\0"
    "onClickAutoChk\0onClickBuyChk\0"
    "onClickSellChk\0onCodePressReturn\0"
    "onClickTwap"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CTwapInputBox[] = {

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
       4,    0,   56,    2, 0x08 /* Private */,
       5,    0,   57,    2, 0x08 /* Private */,
       6,    0,   58,    2, 0x08 /* Private */,
       7,    0,   59,    2, 0x08 /* Private */,
       8,    0,   60,    2, 0x08 /* Private */,
       9,    0,   61,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void CTwapInputBox::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CTwapInputBox *_t = static_cast<CTwapInputBox *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->onClickOpenChk(); break;
        case 1: _t->onClickCloseChk(); break;
        case 2: _t->onClickCloseToday(); break;
        case 3: _t->onClickAutoChk(); break;
        case 4: _t->onClickBuyChk(); break;
        case 5: _t->onClickSellChk(); break;
        case 6: _t->onCodePressReturn(); break;
        case 7: _t->onClickTwap(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject CTwapInputBox::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_CTwapInputBox.data,
      qt_meta_data_CTwapInputBox,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *CTwapInputBox::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CTwapInputBox::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CTwapInputBox.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int CTwapInputBox::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
