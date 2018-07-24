/****************************************************************************
** Meta object code from reading C++ file 'platform.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.2.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../platform.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'platform.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.2.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_CPlatform_t {
    QByteArrayData data[10];
    char stringdata[84];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_CPlatform_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_CPlatform_t qt_meta_stringdata_CPlatform = {
    {
QT_MOC_LITERAL(0, 0, 9),
QT_MOC_LITERAL(1, 10, 9),
QT_MOC_LITERAL(2, 20, 0),
QT_MOC_LITERAL(3, 21, 8),
QT_MOC_LITERAL(4, 30, 9),
QT_MOC_LITERAL(5, 40, 9),
QT_MOC_LITERAL(6, 50, 16),
QT_MOC_LITERAL(7, 67, 4),
QT_MOC_LITERAL(8, 72, 6),
QT_MOC_LITERAL(9, 79, 3)
    },
    "CPlatform\0readDICOM\0\0fileList\0showImage\0"
    "nFrameIdx\0QTreeWidgetItem*\0item\0column\0"
    "run\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CPlatform[] = {

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
       1,    1,   34,    2, 0x0a,
       4,    1,   37,    2, 0x0a,
       4,    2,   40,    2, 0x0a,
       9,    0,   45,    2, 0x0a,

 // slots: parameters
    QMetaType::Void, QMetaType::QStringList,    3,
    QMetaType::Void, QMetaType::Int,    5,
    QMetaType::Void, 0x80000000 | 6, QMetaType::Int,    7,    8,
    QMetaType::Void,

       0        // eod
};

void CPlatform::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CPlatform *_t = static_cast<CPlatform *>(_o);
        switch (_id) {
        case 0: _t->readDICOM((*reinterpret_cast< QStringList(*)>(_a[1]))); break;
        case 1: _t->showImage((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->showImage((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 3: _t->run(); break;
        default: ;
        }
    }
}

const QMetaObject CPlatform::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_CPlatform.data,
      qt_meta_data_CPlatform,  qt_static_metacall, 0, 0}
};


const QMetaObject *CPlatform::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CPlatform::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CPlatform.stringdata))
        return static_cast<void*>(const_cast< CPlatform*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int CPlatform::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
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
struct qt_meta_stringdata_BorderlessMainWindow_t {
    QByteArrayData data[6];
    char stringdata[79];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_BorderlessMainWindow_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_BorderlessMainWindow_t qt_meta_stringdata_BorderlessMainWindow = {
    {
QT_MOC_LITERAL(0, 0, 20),
QT_MOC_LITERAL(1, 21, 14),
QT_MOC_LITERAL(2, 36, 0),
QT_MOC_LITERAL(3, 37, 13),
QT_MOC_LITERAL(4, 51, 14),
QT_MOC_LITERAL(5, 66, 11)
    },
    "BorderlessMainWindow\0slot_minimized\0"
    "\0slot_restored\0slot_maximized\0slot_closed\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_BorderlessMainWindow[] = {

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
       1,    0,   34,    2, 0x08,
       3,    0,   35,    2, 0x08,
       4,    0,   36,    2, 0x08,
       5,    0,   37,    2, 0x08,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void BorderlessMainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        BorderlessMainWindow *_t = static_cast<BorderlessMainWindow *>(_o);
        switch (_id) {
        case 0: _t->slot_minimized(); break;
        case 1: _t->slot_restored(); break;
        case 2: _t->slot_maximized(); break;
        case 3: _t->slot_closed(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject BorderlessMainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_BorderlessMainWindow.data,
      qt_meta_data_BorderlessMainWindow,  qt_static_metacall, 0, 0}
};


const QMetaObject *BorderlessMainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *BorderlessMainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_BorderlessMainWindow.stringdata))
        return static_cast<void*>(const_cast< BorderlessMainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int BorderlessMainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
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
QT_END_MOC_NAMESPACE
