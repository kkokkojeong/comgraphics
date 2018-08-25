/****************************************************************************
** Meta object code from reading C++ file 'dv_client.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../dv_client.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'dv_client.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_DV_Client[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      11,   10,   10,   10, 0x08,
      40,   10,   10,   10, 0x08,
      68,   10,   10,   10, 0x08,
      94,   10,   10,   10, 0x08,
     121,   10,   10,   10, 0x08,
     148,   10,   10,   10, 0x08,
     177,   10,   10,   10, 0x08,
     213,  208,   10,   10, 0x08,
     246,  208,   10,   10, 0x08,
     276,  208,   10,   10, 0x08,
     306,  208,   10,   10, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_DV_Client[] = {
    "DV_Client\0\0on_actionConnect_triggered()\0"
    "on_actionSeogwi_triggered()\0"
    "on_actionJeju_triggered()\0"
    "on_actionAbout_triggered()\0"
    "on_actionClose_triggered()\0"
    "on_actionRequest_triggered()\0"
    "on_actionRendering_triggered()\0arg1\0"
    "on_actionTranslate_toggled(bool)\0"
    "on_actionRotate_toggled(bool)\0"
    "on_actionZoomIn_toggled(bool)\0"
    "on_actionZoomOut_toggled(bool)\0"
};

void DV_Client::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        DV_Client *_t = static_cast<DV_Client *>(_o);
        switch (_id) {
        case 0: _t->on_actionConnect_triggered(); break;
        case 1: _t->on_actionSeogwi_triggered(); break;
        case 2: _t->on_actionJeju_triggered(); break;
        case 3: _t->on_actionAbout_triggered(); break;
        case 4: _t->on_actionClose_triggered(); break;
        case 5: _t->on_actionRequest_triggered(); break;
        case 6: _t->on_actionRendering_triggered(); break;
        case 7: _t->on_actionTranslate_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 8: _t->on_actionRotate_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 9: _t->on_actionZoomIn_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 10: _t->on_actionZoomOut_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData DV_Client::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject DV_Client::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_DV_Client,
      qt_meta_data_DV_Client, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DV_Client::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DV_Client::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DV_Client::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DV_Client))
        return static_cast<void*>(const_cast< DV_Client*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int DV_Client::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
