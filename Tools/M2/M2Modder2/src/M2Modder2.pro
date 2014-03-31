TEMPLATE = app
TARGET = M2Modder2
QT += core \
    gui
HEADERS += Dialogs/RibbonDialog.h \
    Tools/modelhelpstructs.h \
    Dialogs/TransparencyDialog.h \
    Tools/modelenums.h \
    Dialogs/TextureDialog.h \
    Tools/modelheaders.h \
    Dialogs/ResizeDialog.h \
    Tools/modeleditor.h \
    m2modder2.h
SOURCES += Dialogs/RibbonDialog.cpp \
    Dialogs/TransparencyDialog.cpp \
    Dialogs/TextureDialog.cpp \
    Dialogs/ResizeDialog.cpp \
    Tools/modeleditor.cpp \
    main.cpp \
    m2modder2.cpp
FORMS += m2modder2.ui
RESOURCES += 
