/****************************************************************************
** Meta object code from reading C++ file 'dlgPackageExporter.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.1.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/dlgPackageExporter.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'dlgPackageExporter.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.1.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_dlgPackageExporter_t {
    QByteArrayData data[5];
    char stringdata[74];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_dlgPackageExporter_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_dlgPackageExporter_t qt_meta_stringdata_dlgPackageExporter = {
    {
QT_MOC_LITERAL(0, 0, 18),
QT_MOC_LITERAL(1, 19, 13),
QT_MOC_LITERAL(2, 33, 0),
QT_MOC_LITERAL(3, 34, 18),
QT_MOC_LITERAL(4, 53, 19)
    },
    "dlgPackageExporter\0slot_addFiles\0\0"
    "slot_browse_button\0slot_export_package\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_dlgPackageExporter[] = {

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
       1,    0,   29,    2, 0x0a,
       3,    0,   30,    2, 0x0a,
       4,    0,   31,    2, 0x0a,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void dlgPackageExporter::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        dlgPackageExporter *_t = static_cast<dlgPackageExporter *>(_o);
        switch (_id) {
        case 0: _t->slot_addFiles(); break;
        case 1: _t->slot_browse_button(); break;
        case 2: _t->slot_export_package(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject dlgPackageExporter::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_dlgPackageExporter.data,
      qt_meta_data_dlgPackageExporter,  qt_static_metacall, 0, 0}
};


const QMetaObject *dlgPackageExporter::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *dlgPackageExporter::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_dlgPackageExporter.stringdata))
        return static_cast<void*>(const_cast< dlgPackageExporter*>(this));
    return QDialog::qt_metacast(_clname);
}

int dlgPackageExporter::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
QT_END_MOC_NAMESPACE
