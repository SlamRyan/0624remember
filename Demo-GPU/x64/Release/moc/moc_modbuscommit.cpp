/****************************************************************************
** Meta object code from reading C++ file 'modbuscommit.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.12)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../include/modbuscommit.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QVector>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'modbuscommit.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.12. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_ModbusCommit_t {
    QByteArrayData data[23];
    char stringdata0[404];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ModbusCommit_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ModbusCommit_t qt_meta_stringdata_ModbusCommit = {
    {
QT_MOC_LITERAL(0, 0, 12), // "ModbusCommit"
QT_MOC_LITERAL(1, 13, 26), // "modbus_state_change_signal"
QT_MOC_LITERAL(2, 40, 0), // ""
QT_MOC_LITERAL(3, 41, 20), // "QModbusDevice::State"
QT_MOC_LITERAL(4, 62, 5), // "state"
QT_MOC_LITERAL(5, 68, 28), // "modbus_error_occurred_signal"
QT_MOC_LITERAL(6, 97, 20), // "QModbusDevice::Error"
QT_MOC_LITERAL(7, 118, 23), // "modbus_read_over_signal"
QT_MOC_LITERAL(8, 142, 29), // "QModbusDataUnit::RegisterType"
QT_MOC_LITERAL(9, 172, 4), // "type"
QT_MOC_LITERAL(10, 177, 5), // "isErr"
QT_MOC_LITERAL(11, 183, 16), // "QVector<quint16>"
QT_MOC_LITERAL(12, 200, 8), // "readData"
QT_MOC_LITERAL(13, 209, 16), // "modbus_write_res"
QT_MOC_LITERAL(14, 226, 17), // "state_change_slot"
QT_MOC_LITERAL(15, 244, 19), // "error_occurred_slot"
QT_MOC_LITERAL(16, 264, 5), // "error"
QT_MOC_LITERAL(17, 270, 20), // "readready_coils_slot"
QT_MOC_LITERAL(18, 291, 22), // "readready_holding_slot"
QT_MOC_LITERAL(19, 314, 20), // "readready_input_slot"
QT_MOC_LITERAL(20, 335, 23), // "readready_discrete_slot"
QT_MOC_LITERAL(21, 359, 20), // "writeready_coil_slot"
QT_MOC_LITERAL(22, 380, 23) // "writeready_holding_slot"

    },
    "ModbusCommit\0modbus_state_change_signal\0"
    "\0QModbusDevice::State\0state\0"
    "modbus_error_occurred_signal\0"
    "QModbusDevice::Error\0modbus_read_over_signal\0"
    "QModbusDataUnit::RegisterType\0type\0"
    "isErr\0QVector<quint16>\0readData\0"
    "modbus_write_res\0state_change_slot\0"
    "error_occurred_slot\0error\0"
    "readready_coils_slot\0readready_holding_slot\0"
    "readready_input_slot\0readready_discrete_slot\0"
    "writeready_coil_slot\0writeready_holding_slot"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ModbusCommit[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   74,    2, 0x06 /* Public */,
       5,    1,   77,    2, 0x06 /* Public */,
       7,    3,   80,    2, 0x06 /* Public */,
      13,    2,   87,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      14,    1,   92,    2, 0x08 /* Private */,
      15,    1,   95,    2, 0x08 /* Private */,
      17,    0,   98,    2, 0x08 /* Private */,
      18,    0,   99,    2, 0x08 /* Private */,
      19,    0,  100,    2, 0x08 /* Private */,
      20,    0,  101,    2, 0x08 /* Private */,
      21,    0,  102,    2, 0x08 /* Private */,
      22,    0,  103,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 6,    4,
    QMetaType::Void, 0x80000000 | 8, QMetaType::Bool, 0x80000000 | 11,    9,   10,   12,
    QMetaType::Void, 0x80000000 | 8, QMetaType::Bool,    9,   10,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 6,   16,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void ModbusCommit::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ModbusCommit *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->modbus_state_change_signal((*reinterpret_cast< QModbusDevice::State(*)>(_a[1]))); break;
        case 1: _t->modbus_error_occurred_signal((*reinterpret_cast< QModbusDevice::Error(*)>(_a[1]))); break;
        case 2: _t->modbus_read_over_signal((*reinterpret_cast< QModbusDataUnit::RegisterType(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2])),(*reinterpret_cast< QVector<quint16>(*)>(_a[3]))); break;
        case 3: _t->modbus_write_res((*reinterpret_cast< QModbusDataUnit::RegisterType(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 4: _t->state_change_slot((*reinterpret_cast< QModbusDevice::State(*)>(_a[1]))); break;
        case 5: _t->error_occurred_slot((*reinterpret_cast< QModbusDevice::Error(*)>(_a[1]))); break;
        case 6: _t->readready_coils_slot(); break;
        case 7: _t->readready_holding_slot(); break;
        case 8: _t->readready_input_slot(); break;
        case 9: _t->readready_discrete_slot(); break;
        case 10: _t->writeready_coil_slot(); break;
        case 11: _t->writeready_holding_slot(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 2:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QModbusDataUnit::RegisterType >(); break;
            case 2:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QVector<quint16> >(); break;
            }
            break;
        case 3:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QModbusDataUnit::RegisterType >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (ModbusCommit::*)(QModbusDevice::State );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ModbusCommit::modbus_state_change_signal)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (ModbusCommit::*)(QModbusDevice::Error );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ModbusCommit::modbus_error_occurred_signal)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (ModbusCommit::*)(QModbusDataUnit::RegisterType , bool , QVector<quint16> );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ModbusCommit::modbus_read_over_signal)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (ModbusCommit::*)(QModbusDataUnit::RegisterType , bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ModbusCommit::modbus_write_res)) {
                *result = 3;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject ModbusCommit::staticMetaObject = { {
    &QWidget::staticMetaObject,
    qt_meta_stringdata_ModbusCommit.data,
    qt_meta_data_ModbusCommit,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *ModbusCommit::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ModbusCommit::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ModbusCommit.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int ModbusCommit::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    }
    return _id;
}

// SIGNAL 0
void ModbusCommit::modbus_state_change_signal(QModbusDevice::State _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void ModbusCommit::modbus_error_occurred_signal(QModbusDevice::Error _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void ModbusCommit::modbus_read_over_signal(QModbusDataUnit::RegisterType _t1, bool _t2, QVector<quint16> _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void ModbusCommit::modbus_write_res(QModbusDataUnit::RegisterType _t1, bool _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
