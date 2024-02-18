

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
