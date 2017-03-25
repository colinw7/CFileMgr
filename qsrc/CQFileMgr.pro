TEMPLATE = lib

TARGET = CQFileMgr

DEPENDPATH += .

QT += widgets

CONFIG += staticlib
CONFIG += debug

QMAKE_CXXFLAGS += -std=c++11

MOC_DIR = .moc

# Input
SOURCES += \
CQFileMgr.cpp \
CQFileMgrDetails.cpp \
CQFileMgrFilmstripIcons.cpp \
CQFileMgrFilmstripImage.cpp \
CQFileMgrIcons.cpp \
CQFileMgrInfo.cpp \
CQFileMgrToolbar.cpp \
\
CQDirBar.cpp \
CQDirPlaces.cpp \
CQDirView.cpp \
CQPixelRendererCanvas.cpp \
CQPixelRendererScrolledCanvas.cpp \

HEADERS += \
../qinclude/CQFileMgr.h \
../qinclude/CQDirBar.h \
../qinclude/CQDirPlaces.h \
../qinclude/CQDirView.h \
\
CQFileMgrDetails.h \
CQFileMgrFilmstripIcons.h \
CQFileMgrFilmstripImage.h \
CQFileMgrIcons.h \
CQFileMgrInfo.h \
CQFileMgrToolbar.h \
\
CQPixelRendererCanvas.h \
CQPixelRendererScrolledCanvas.h \
CQPixelRendererSubCanvas.h \

OBJECTS_DIR = ../obj

DESTDIR = ../lib

INCLUDEPATH += \
. \
../qinclude \
../include \
../../CQUtil/include \
../../CConfig/include \
../../CRenderer/qinclude \
../../CRenderer/include \
../../CImageLib/include \
../../CFont/include \
../../CFile/include \
../../CMath/include \
../../CStrUtil/include \
../../COS/include \
../../CUtil/include \
