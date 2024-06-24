/****************************************************************************
** Meta object code from reading C++ file 'ins_camlightpose.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.12)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../include/ins_camlightpose.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ins_camlightpose.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.12. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Ins_CamLightPose_t {
    QByteArrayData data[10];
    char stringdata0[131];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Ins_CamLightPose_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Ins_CamLightPose_t qt_meta_stringdata_Ins_CamLightPose = {
    {
QT_MOC_LITERAL(0, 0, 16), // "Ins_CamLightPose"
QT_MOC_LITERAL(1, 17, 13), // "setAngleValue"
QT_MOC_LITERAL(2, 31, 0), // ""
QT_MOC_LITERAL(3, 32, 5), // "angle"
QT_MOC_LITERAL(4, 38, 26), // "setupGraphicsViewWithImage"
QT_MOC_LITERAL(5, 65, 20), // "QGraphicsPixmapItem*"
QT_MOC_LITERAL(6, 86, 14), // "QGraphicsView*"
QT_MOC_LITERAL(7, 101, 4), // "view"
QT_MOC_LITERAL(8, 106, 9), // "imagePath"
QT_MOC_LITERAL(9, 116, 14) // "updateRotation"

    },
    "Ins_CamLightPose\0setAngleValue\0\0angle\0"
    "setupGraphicsViewWithImage\0"
    "QGraphicsPixmapItem*\0QGraphicsView*\0"
    "view\0imagePath\0updateRotation"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Ins_CamLightPose[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   29,    2, 0x0a /* Public */,
       4,    2,   32,    2, 0x0a /* Public */,
       9,    0,   37,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, QMetaType::Double,    3,
    0x80000000 | 5, 0x80000000 | 6, QMetaType::QString,    7,    8,
    QMetaType::Void,

       0        // eod
};

void Ins_CamLightPose::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<Ins_CamLightPose *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->setAngleValue((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 1: { QGraphicsPixmapItem* _r = _t->setupGraphicsViewWithImage((*reinterpret_cast< QGraphicsView*(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< QGraphicsPixmapItem**>(_a[0]) = std::move(_r); }  break;
        case 2: _t->updateRotation(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject Ins_CamLightPose::staticMetaObject = { {
    &QDialog::staticMetaObject,
    qt_meta_stringdata_Ins_CamLightPose.data,
    qt_meta_data_Ins_CamLightPose,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *Ins_CamLightPose::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Ins_CamLightPose::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Ins_CamLightPose.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int Ins_CamLightPose::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
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
