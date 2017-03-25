TEMPLATE = app

TARGET = CQFileMgrTest

DEPENDPATH += .

QT += widgets opengl

QMAKE_CXXFLAGS += -std=c++11

MOC_DIR = .moc

SOURCES += \
main.cpp \
\
CQFileMgrTest.cpp \
\
CQImageCanvas.cpp \
CQPaintTools.cpp \
CQWinWidget.cpp \
CQZoomCursor.cpp \

HEADERS += \
CQFileMgrTest.h \
\
CQImageCanvas.h \
CQPaintTools.h \
CQWinWidget.h \
CQZoomCursor.h \
CValueHistory.h \

DESTDIR     = ../bin
OBJECTS_DIR = ../obj
LIB_DIR     = ../lib

INCLUDEPATH += \
. \
../qinclude \
../include \
../../CQUtil/include \
../../CImageLib/include \
../../CFont/include \
../../CFile/include \
../../CFileUtil/include \
../../CMath/include \
../../CStrUtil/include \
../../CUtil/include \
../../COS/include \

unix:LIBS += \
-L$$LIB_DIR \
-L../../CQUtil/lib \
-L../../CRenderer/lib \
-L../../CConfig/lib \
-L../../CCommand/lib \
-L../../CImageLib/lib \
-L../../CFont/lib \
-L../../CFile/lib \
-L../../CFileUtil/lib \
-L../../CMath/lib \
-L../../CStrUtil/lib \
-L../../CRegExp/lib \
-L../../CUtil/lib \
-L../../COS/lib \
-lCQFileMgr \
-lCFileMgr \
-lCQRenderer \
-lCRenderer \
-lCQUtil \
-lCCommand \
-lCConfig \
-lCImageLib \
-lCFont \
-lCFile \
-lCFileUtil \
-lCMath \
-lCStrUtil \
-lCRegExp \
-lCUtil \
-lCOS \
-lpng -ljpeg -ltre -lfreetype -lcurses
