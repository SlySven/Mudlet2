/****************************************************************************
** Meta object code from reading C++ file 'TTextEdit.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.1.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/TTextEdit.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'TTextEdit.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.1.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_TTextEdit_t {
    QByteArrayData data[7];
    char stringdata[133];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_TTextEdit_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_TTextEdit_t qt_meta_stringdata_TTextEdit = {
    {
QT_MOC_LITERAL(0, 0, 9),
QT_MOC_LITERAL(1, 10, 21),
QT_MOC_LITERAL(2, 32, 0),
QT_MOC_LITERAL(3, 33, 29),
QT_MOC_LITERAL(4, 63, 19),
QT_MOC_LITERAL(5, 83, 14),
QT_MOC_LITERAL(6, 98, 33)
    },
    "TTextEdit\0slot_toggleTimeStamps\0\0"
    "slot_copySelectionToClipboard\0"
    "slot_scrollBarMoved\0slot_popupMenu\0"
    "slot_copySelectionToClipboardHTML\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_TTextEdit[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   39,    2, 0x0a,
       3,    0,   40,    2, 0x0a,
       4,    1,   41,    2, 0x0a,
       5,    0,   44,    2, 0x0a,
       6,    0,   45,    2, 0x0a,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void TTextEdit::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        TTextEdit *_t = static_cast<TTextEdit *>(_o);
        switch (_id) {
        case 0: _t->slot_toggleTimeStamps(); break;
        case 1: _t->slot_copySelectionToClipboard(); break;
        case 2: _t->slot_scrollBarMoved((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->slot_popupMenu(); break;
        case 4: _t->slot_copySelectionToClipboardHTML(); break;
        default: ;
        }
    }
}

const QMetaObject TTextEdit::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_TTextEdit.data,
      qt_meta_data_TTextEdit,  qt_static_metacall, 0, 0}
};


const QMetaObject *TTextEdit::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TTextEdit::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_TTextEdit.stringdata))
        return static_cast<void*>(const_cast< TTextEdit*>(this));
    return QWidget::qt_metacast(_clname);
}

int TTextEdit::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
