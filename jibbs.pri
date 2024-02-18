

INCLUDEPATH += $${PWD}/include

CONFIG(release, debug|release) {
    CONFIGURATION = "Release"
}

CONFIG(debug, debug|release) {
    CONFIGURATION = "Debug"
}

win32{
DESTDIR = $$OUT_PWD/$$CONFIGURATION
OBJECTS_DIR = $$OUT_PWD/.obj
MOC_DIR = $$OUT_PWD/.moc
RCC_DIR = $$OUT_PWD/.rcc
UI_DIR = $$OUT_PWD/.ui


    DEPLOY_DIR = $$DESTDIR
}

android{
    DEPLOY_DIR = /assets
}

isEmpty(TARGET_EXT) {
    win32 {
        TARGET_CUSTOM_EXT = .exe
    }   
}


CONFIG(release, debug|release) {
    contains(CONFIG, "qt"){

        win32 {
            DEPLOY_COMMAND = $(QTDIR)/bin/windeployqt
        }

        win32 {

        DEPLOY_TARGET =  $$shell_quote($$shell_path($${DESTDIR}/$${TARGET}$${TARGET_CUSTOM_EXT}))

        #message(  $${DEPLOY_COMMAND} $${DEPLOY_TARGET} )

        #  # Uncomment the following line to help debug the deploy command when running qmake
        #  warning($${DEPLOY_COMMAND} $${DEPLOY_TARGET})

            QMAKE_POST_LINK = $${DEPLOY_COMMAND} --qmldir $(QTDIR)\qml $${DEPLOY_TARGET}
        }
    }
}

SOURCES += \
    $$PWD/source/math/QVRHdgTable.cpp \
    $$PWD/source/math/qhdgtable.cpp \
    $$PWD/source/mesh/MeshData.cpp \
    $$PWD/source/mesh/MultSphereMeshObjBuilder.cpp \
    $$PWD/source/mesh/MultipleInstanceObjBuilder.cpp \
    $$PWD/source/mesh/ObjLoader.cpp \
    $$PWD/source/mesh/ObjectLites.cpp \
    $$PWD/source/mesh/meshloader.cpp \
    $$PWD/source/utilities/QmlClipboardAdapter.cpp \
    $$PWD/source/utilities/RandomStringGenerator.cpp \
    $$PWD/source/utilities/permissions.cpp
