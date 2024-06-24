/****************************************************************************
** Meta object code from reading C++ file 'ins_encodeverify.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.12)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../include/ins_encodeverify.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ins_encodeverify.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.12. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_ins_encodeVerify_t {
    QByteArrayData data[10];
    char stringdata0[145];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ins_encodeVerify_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ins_encodeVerify_t qt_meta_stringdata_ins_encodeVerify = {
    {
QT_MOC_LITERAL(0, 0, 16), // "ins_encodeVerify"
QT_MOC_LITERAL(1, 17, 13), // "ABstatechange"
QT_MOC_LITERAL(2, 31, 0), // ""
QT_MOC_LITERAL(3, 32, 14), // "ABstatechange2"
QT_MOC_LITERAL(4, 47, 24), // "on_checkBox_stateChanged"
QT_MOC_LITERAL(5, 72, 4), // "arg1"
QT_MOC_LITERAL(6, 77, 14), // "ReadEncodePose"
QT_MOC_LITERAL(7, 92, 14), // "switchLanguage"
QT_MOC_LITERAL(8, 107, 7), // "english"
QT_MOC_LITERAL(9, 115, 29) // "on_ModbusClientBtn_11_clicked"

    },
    "ins_encodeVerify\0ABstatechange\0\0"
    "ABstatechange2\0on_checkBox_stateChanged\0"
    "arg1\0ReadEncodePose\0switchLanguage\0"
    "english\0on_ModbusClientBtn_11_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ins_encodeVerify[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   44,    2, 0x06 /* Public */,
       3,    0,   45,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    1,   46,    2, 0x0a /* Public */,
       6,    0,   49,    2, 0x0a /* Public */,
       7,    1,   50,    2, 0x0a /* Public */,
       9,    0,   53,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    5,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,    8,
    QMetaType::Void,

       0        // eod
};

void ins_encodeVerify::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ins_encodeVerify *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->ABstatechange(); break;
        case 1: _t->ABstatechange2(); break;
        case 2: _t->on_checkBox_stateChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->ReadEncodePose(); break;
        case 4: _t->switchLanguage((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 5: _t->on_ModbusClientBtn_11_clicked(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (ins_encodeVerify::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ins_encodeVerify::ABstatechange)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (ins_encodeVerify::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ins_encodeVerify::ABstatechange2)) {
                *result = 1;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject ins_encodeVerify::staticMetaObject = { {
    &QDialog::staticMetaObject,
    qt_meta_stringdata_ins_encodeVerify.data,
    qt_meta_data_ins_encodeVerify,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *ins_encodeVerify::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ins_encodeVerify::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ins_encodeVerify.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int ins_encodeVerify::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void ins_encodeVerify::ABstatechange()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void ins_encodeVerify::ABstatechange2()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
