QT += network
QT -= gui

CONFIG += c++11 console
CONFIG -= app_bundle

TEMPLATE = app
# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        main.cpp \
        server.cpp

HEADERS += \
    server.h

OBJECTS_DIR = generated_files
MOC_DIR = generated_files

unix {

    QMAKE_MKDIR = mkdir -p
    CONF_FILE = log.txt
    DOWNLOAD_FOLDER = downloads

    CONFIG(debug, debug|release) {
        message(DEBUG building started)

#        DEFINES += QT_NO_DEBUG_OUTPUT

        cmd_mkdir1.commands = $(MKDIR) $$DOWNLOAD_FOLDER

        cmd_copyfile1.commands = $(COPY_FILE) $$PWD/$$CONF_FILE $$OUT_PWD/$$CONF_FILE

        first.depends = cmd_mkdir1\
                        $(first)\
                        cmd_copyfile1

        export(first.depends)
        export(cmd_mkdir1.commands)
        export(cmd_copyfile1.commands)

        QMAKE_EXTRA_TARGETS += cmd_mkdir1 \
                               first \
                               cmd_copyfile1
        QMAKE

    }


    CONFIG(release, debug|release) {
        message(Release building started)

        DEFINES += QT_NO_DEBUG_OUTPUT

        cmd_mkdir1.commands = $(MKDIR) $$DOWNLOAD_FOLDER

        cmd_copyfile1.commands = $(COPY_FILE) $$PWD/$$CONF_FILE $$OUT_PWD/$$CONF_FILE

        first.depends = cmd_mkdir1\
                        $(first)\
                        cmd_copyfile1

        export(first.depends)
        export(cmd_mkdir1.commands)
        export(cmd_copyfile1.commands)

        QMAKE_EXTRA_TARGETS += cmd_mkdir1 \
                               first \
                               cmd_copyfile1
        QMAKE

        QMAKE_POST_LINK += make clean

    }
}
