
CONFIG(release, debug|release) {
    CONFIGURATION = "Release"
}

CONFIG(debug, debug|release) {
    CONFIGURATION = "Debug"
}

win32{
    TARGET_ARCH_ABI = Win32
}

android{
    TARGET_ARCH_ABI = $${ANDROID_TARGET_ARCH}
    DEFINES += IS_QT_ANDROID
}


DESTDIR = $$_PRO_FILE_PWD_/$${CONFIGURATION}/$${TARGET_ARCH_ABI}
MOC_DIR = $$_PRO_FILE_PWD_/bin/$${TARGET_ARCH_ABI}/$${CONFIGURATION}

OBJECTS_DIR = $$MOC_DIR
UI_DIR= $$MOC_DIR
RCC_DIR += $$MOC_DIR



win32{
    DEPLOY_DIR = $$DESTDIR
}

android{
    DEPLOY_DIR = /assets
}

isEmpty(TARGET_EXT) {
    win32 {
        TARGET_CUSTOM_EXT = .exe
    }
    macx {
        TARGET_CUSTOM_EXT = .app
    }
} #else {
   # TARGET_CUSTOM_EXT = $${TARGET_EXT}
#}

contains(CONFIG, "qt"){

    win32 {
        DEPLOY_COMMAND = windeployqt
    }
    macx {
        DEPLOY_COMMAND = macdeployqt
    }

    win32 {

    DEPLOY_TARGET =  $$shell_quote($$shell_path($${DESTDIR}/$${TARGET}$${TARGET_CUSTOM_EXT}))

    #message(  $${DEPLOY_COMMAND} $${DEPLOY_TARGET} )

    #  # Uncomment the following line to help debug the deploy command when running qmake
    #  warning($${DEPLOY_COMMAND} $${DEPLOY_TARGET})

        QMAKE_POST_LINK = $${DEPLOY_COMMAND} $${DEPLOY_TARGET}
    }
}
