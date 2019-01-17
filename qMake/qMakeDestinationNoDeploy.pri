
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

DESTDIR = $$OUT_PWD/$$CONFIGURATION
OBJECTS_DIR = $$OUT_PWD/.obj
MOC_DIR = $$OUT_PWD/.moc
RCC_DIR = $$OUT_PWD/.rcc
UI_DIR = $$OUT_PWD/.ui


win32{
    DEPLOY_DIR = $$DESTDIR
}

android{
    DEPLOY_DIR = /assets
}

