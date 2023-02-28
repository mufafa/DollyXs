QT       += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
CONFIG += c++11


OBJECTS_DIR=$$OUT_PWD/obj_files
MOC_DIR=$$OUT_PWD/moc_files
UI_DIR=$$OUT_PWD/ui_files
RCC_DIR = $$OUT_PWD/rcc_files

SOURCES += \
    filemodel/ifilemodel.cpp \
    widget/alert.cpp \
    widget/overlayprogress.cpp \
    core/application.cpp \
    core/settings.cpp \
    core/validate.cpp \
    transform/saxon.cpp \
    transform/itransformer.cpp \
    main.cpp \
    mainwindow.cpp \
    widget/preferences.cpp \
    xml/elementbuilder.cpp \
    xml/inputfile.cpp

HEADERS += \
    constants/fileutility.h \
    constants/constans.h \
    filemodel/ifilemodel.h \
    widget/alert.h \
    widget/overlayprogress.h \
    core/application.h \
    core/settings.h \
    core/validate.h \
    mainwindow.h \
    transform/itransformer.h \
    transform/saxon.h \
    widget/preferences.h \
    xml/elementbuilder.h \
    xml/inputfile.h

FORMS += \
    widget/alert.ui \
    mainwindow.ui \
    widget/preferences.ui
RESOURCES += \
    Resources.qrc


macx:{
    ICON = Resources/icons/dolly.icns
}







