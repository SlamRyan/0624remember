/****************************************************************************
** Meta object code from reading C++ file 'ins_titlebar.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.12)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../include/ins_titlebar.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ins_titlebar.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.12. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_INS_TitleBar_t {
    QByteArrayData data[8];
    char stringdata0[119];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_INS_TitleBar_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_INS_TitleBar_t qt_meta_stringdata_INS_TitleBar = {
    {
QT_MOC_LITERAL(0, 0, 12), // "INS_TitleBar"
QT_MOC_LITERAL(1, 13, 15), // "minimizeClicked"
QT_MOC_LITERAL(2, 29, 0), // ""
QT_MOC_LITERAL(3, 30, 24), // "maximizeOrRestoreClicked"
QT_MOC_LITERAL(4, 55, 12), // "closeClicked"
QT_MOC_LITERAL(5, 68, 14), // "minimizeWindow"
QT_MOC_LITERAL(6, 83, 23), // "maximizeOrRestoreWindow"
QT_MOC_LITERAL(7, 107, 11) // "closeWindow"

    },
    "INS_TitleBar\0minimizeClicked\0\0"
    "maximizeOrRestoreClicked\0closeClicked\0"
    "minimizeWindow\0maximizeOrRestoreWindow\0"
    "closeWindow"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_INS_TitleBar[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   44,    2, 0x06 /* Public */,
       3,    0,   45,    2, 0x06 /* Public */,
       4,    0,   46,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    0,   47,    2, 0x08 /* Private */,
       6,    0,   48,    2, 0x08 /* Private */,
       7,    0,   49,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void INS_TitleBar::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<INS_TitleBar *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->minimizeClicked(); break;
        case 1: _t->maximizeOrRestoreClicked(); break;
        case 2: _t->closeClicked(); break;
        case 3: _t->minimizeWindow(); break;
        case 4: _t->maximizeOrRestoreWindow(); break;
        case 5: _t->closeWindow(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (INS_TitleBar::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&INS_TitleBar::minimizeClicked)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (INS_TitleBar::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&INS_TitleBar::maximizeOrRestoreClicked)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (INS_TitleBar::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&INS_TitleBar::closeClicked)) {
                *result = 2;
                return;
            }
        }
    }
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject INS_TitleBar::staticMetaObject = { {
    &QWidget::staticMetaObject,
    qt_meta_stringdata_INS_TitleBar.data,
    qt_meta_data_INS_TitleBar,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *INS_TitleBar::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *INS_TitleBar::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_INS_TitleBar.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int INS_TitleBar::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
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
void INS_TitleBar::minimizeClicked()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void INS_TitleBar::maximizeOrRestoreClicked()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void INS_TitleBar::closeClicked()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
