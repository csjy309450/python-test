/****************************************************************************
** Meta object code from reading C++ file 'CQplayerGUI.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "CQplayerGUI.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'CQplayerGUI.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CQplayerGUI[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      13,   12,   12,   12, 0x08,
      50,   41,   12,   12, 0x08,
      80,   41,   12,   12, 0x08,
     115,   12,   12,   12, 0x08,
     150,   12,   12,   12, 0x08,
     184,   12,   12,   12, 0x08,
     212,   12,   12,   12, 0x08,
     239,   12,   12,   12, 0x08,
     264,   12,   12,   12, 0x08,
     288,   12,   12,   12, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_CQplayerGUI[] = {
    "CQplayerGUI\0\0on_openFileButton_clicked()\0"
    "position\0on_sliderVlm_sliderMoved(int)\0"
    "on_sliderProgress_sliderMoved(int)\0"
    "on_sliderProgress_sliderReleased()\0"
    "on_sliderProgress_sliderPressed()\0"
    "on_backwardButton_clicked()\0"
    "on_forwardButton_clicked()\0"
    "on_pauseButton_clicked()\0"
    "on_stopButton_clicked()\0on_playButton_clicked()\0"
};

void CQplayerGUI::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CQplayerGUI *_t = static_cast<CQplayerGUI *>(_o);
        switch (_id) {
        case 0: _t->on_openFileButton_clicked(); break;
        case 1: _t->on_sliderVlm_sliderMoved((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->on_sliderProgress_sliderMoved((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->on_sliderProgress_sliderReleased(); break;
        case 4: _t->on_sliderProgress_sliderPressed(); break;
        case 5: _t->on_backwardButton_clicked(); break;
        case 6: _t->on_forwardButton_clicked(); break;
        case 7: _t->on_pauseButton_clicked(); break;
        case 8: _t->on_stopButton_clicked(); break;
        case 9: _t->on_playButton_clicked(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CQplayerGUI::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CQplayerGUI::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_CQplayerGUI,
      qt_meta_data_CQplayerGUI, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CQplayerGUI::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CQplayerGUI::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CQplayerGUI::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CQplayerGUI))
        return static_cast<void*>(const_cast< CQplayerGUI*>(this));
    return QDialog::qt_metacast(_clname);
}

int CQplayerGUI::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    }
    return _id;
}
QT_END_MOC_NAMESPACE