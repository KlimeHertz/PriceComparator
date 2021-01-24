QT -= gui

QT += network

CONFIG += c++11 console
CONFIG -= app_bundle
QMAKE_CFLAGS += -std=c99

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
htmldonwloader.cpp \
    htmlparser.cpp \
main.cpp \
qgumboattribute.cpp \
qgumbodocument.cpp \
qgumbonode.cpp \
gumbo-parser/src/attribute.c \
gumbo-parser/src/char_ref.c \
gumbo-parser/src/error.c \
gumbo-parser/src/parser.c \
gumbo-parser/src/string_buffer.c \
gumbo-parser/src/string_piece.c \
gumbo-parser/src/tag.c \
gumbo-parser/src/tokenizer.c \
gumbo-parser/src/utf8.c \
gumbo-parser/src/util.c \
gumbo-parser/src/vector.c

HEADERS += \
htmldonwloader.h \
    htmlparser.h \
qgumboattribute.h \
qgumbodocument.h \
qgumbonode.h \
gumbo-parser/src/attribute.h \
gumbo-parser/src/char_ref.h \
gumbo-parser/src/error.h \
gumbo-parser/src/gumbo.h \
gumbo-parser/src/insertion_mode.h \
gumbo-parser/src/parser.h \
gumbo-parser/src/string_buffer.h \
gumbo-parser/src/string_piece.h \
gumbo-parser/src/token_type.h \
gumbo-parser/src/tokenizer_states.h \
gumbo-parser/src/tokenizer.h \
gumbo-parser/src/utf8.h \
gumbo-parser/src/util.h \
gumbo-parser/src/vector.h \
HtmlTag.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
