######################################################################
# Automatically generated by qmake (2.00a) Wed Jun 22 16:57:14 2005
######################################################################

TEMPLATE = lib
CONFIG += qt warn_on
QT -= gui
LIBS += -LC:/workspace/personal/djon/libs \
        -lz
DEPENDPATH += .
INCLUDEPATH += . \
     C:/Qt/2010.04/qt/src/3rdparty/zlib

# Input
HEADERS += *.h

SOURCES += *.c *.cpp

unix {
  OBJECTS_DIR=.obj
  MOC_DIR=.moc
}

# UNIX installation

isEmpty(PREFIX): PREFIX=/usr/local

unix {
  headers.path=$$PREFIX/include/quazip
  headers.files=$$HEADERS
  target.path=$$PREFIX/lib
  INSTALLS += headers target
}