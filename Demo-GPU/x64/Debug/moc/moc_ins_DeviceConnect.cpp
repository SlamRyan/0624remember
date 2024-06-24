/****************************************************************************
** Meta object code from reading C++ file 'ins_DeviceConnect.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.12)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../include/ins_DeviceConnect.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ins_DeviceConnect.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.12. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_ins_DeviceConnect_t {
    QByteArrayData data[16];
    char stringdata0[261];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ins_DeviceConnect_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ins_DeviceConnect_t qt_meta_stringdata_ins_DeviceConnect = {
    {
QT_MOC_LITERAL(0, 0, 17), // "ins_DeviceConnect"
QT_MOC_LITERAL(1, 18, 14), // "handleSelected"
QT_MOC_LITERAL(2, 33, 0), // ""
QT_MOC_LITERAL(3, 34, 11), // "lightHandle"
QT_MOC_LITERAL(4, 46, 12), // "cameraHandle"
QT_MOC_LITERAL(5, 59, 12), // "systemHandle"
QT_MOC_LITERAL(6, 72, 18), // "systemHandleSelect"
QT_MOC_LITERAL(7, 91, 15), // "DeviceInfoIndex"
QT_MOC_LITERAL(8, 107, 8), // "CamIndex"
QT_MOC_LITERAL(9, 116, 10), // "Lightindex"
QT_MOC_LITERAL(10, 127, 22), // "onDeviceConnectClicked"
QT_MOC_LITERAL(11, 150, 19), // "onDeviceTestClicked"
QT_MOC_LITERAL(12, 170, 20), // "onDeviceCloseClicked"
QT_MOC_LITERAL(13, 191, 22), // "onConnectButtonClicked"
QT_MOC_LITERAL(14, 214, 21), // "onCombinButtonClicked"
QT_MOC_LITERAL(15, 236, 24) // "onSystemListItemSelected"

    },
    "ins_DeviceConnect\0handleSelected\0\0"
    "lightHandle\0cameraHandle\0systemHandle\0"
    "systemHandleSelect\0DeviceInfoIndex\0"
    "CamIndex\0Lightindex\0onDeviceConnectClicked\0"
    "onDeviceTestClicked\0onDeviceCloseClicked\0"
    "onConnectButtonClicked\0onCombinButtonClicked\0"
    "onSystemListItemSelected"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ins_DeviceConnect[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    3,   59,    2, 0x06 /* Public */,
       6,    1,   66,    2, 0x06 /* Public */,
       7,    2,   69,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      10,    0,   74,    2, 0x0a /* Public */,
      11,    0,   75,    2, 0x0a /* Public */,
      12,    0,   76,    2, 0x0a /* Public */,
      13,    0,   77,    2, 0x0a /* Public */,
      14,    0,   78,    2, 0x0a /* Public */,
      15,    0,   79,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::VoidStar, QMetaType::VoidStar, QMetaType::VoidStar,    3,    4,    5,
    QMetaType::Void, QMetaType::VoidStar,    5,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    8,    9,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void ins_DeviceConnect::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ins_DeviceConnect *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->handleSelected((*reinterpret_cast< void*(*)>(_a[1])),(*reinterpret_cast< void*(*)>(_a[2])),(*reinterpret_cast< void*(*)>(_a[3]))); break;
        case 1: _t->systemHandleSelect((*reinterpret_cast< void*(*)>(_a[1]))); break;
        case 2: _t->DeviceInfoIndex((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 3: _t->onDeviceConnectClicked(); break;
        case 4: _t->onDeviceTestClicked(); break;
        case 5: _t->onDeviceCloseClicked(); break;
        case 6: _t->onConnectButtonClicked(); break;
        case 7: _t->onCombinButtonClicked(); break;
        case 8: _t->onSystemListItemSelected(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (ins_DeviceConnect::*)(void * , void * , void * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ins_DeviceConnect::handleSelected)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (ins_DeviceConnect::*)(void * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ins_DeviceConnect::systemHandleSelect)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (ins_DeviceConnect::*)(int , int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ins_DeviceConnect::DeviceInfoIndex)) {
                *result = 2;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject ins_DeviceConnect::staticMetaObject = { {
    &QDialog::staticMetaObject,
    qt_meta_stringdata_ins_DeviceConnect.data,
    qt_meta_data_ins_DeviceConnect,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *ins_DeviceConnect::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ins_DeviceConnect::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ins_DeviceConnect.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int ins_DeviceConnect::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void ins_DeviceConnect::handleSelected(void * _t1, void * _t2, void * _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void ins_DeviceConnect::systemHandleSelect(void * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void ins_DeviceConnect::DeviceInfoIndex(int _t1, int _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
