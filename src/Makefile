CC = g++
AR = ar
RM = rm

CDEBUG = -g
LDEBUG = -g

INC_DIR = ../include
OBJ_DIR = ../obj
LIB_DIR = ../lib

all: $(LIB_DIR)/libCFileMgr.a

SRC = \
CFileMgr.cpp \
CFileMgrDetails.cpp \
CFileMgrDir.cpp \
CFileMgrFile.cpp \
CFileMgrFilmstrip.cpp \
CFileMgrIcons.cpp \
\
CDirFiles.cpp \
CDirFilesMgr.cpp \
CDirView.cpp \
CFileIcon.cpp \
CFileNotify.cpp \
CImageThumbnail.cpp \
CEncode64.cpp \
CMime.cpp \

OBJS = $(patsubst %.cpp,$(OBJ_DIR)/%.o,$(SRC))

CPPFLAGS = \
-std=c++14 \
-I. \
-I$(INC_DIR) \
-I../../CRenderer/include \
-I../../CConfig/include \
-I../../CCommand/include \
-I../../CImageLib/include \
-I../../CFont/include \
-I../../CMath/include \
-I../../CFile/include \
-I../../CFileUtil/include \
-I../../CStrUtil/include \
-I../../COS/include \
-I../../CUtil/include \

clean:
	$(RM) -f $(OBJ_DIR)/*.o
	$(RM) -f $(LIB_DIR)/libCFileMgr.a

$(OBJS): $(OBJ_DIR)/%.o: %.cpp
	$(CC) -c $< -o $(OBJ_DIR)/$*.o $(CPPFLAGS)

.SUFFIXES: .cpp

$(LIB_DIR)/libCFileMgr.a: $(OBJS)
	$(AR) crv $(LIB_DIR)/libCFileMgr.a $(OBJS)
