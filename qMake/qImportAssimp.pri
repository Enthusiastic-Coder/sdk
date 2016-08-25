INCLUDEPATH += $(SDK_ASSIMP)/include

LIBS += -L$(SDK_ASSIMP)/$$CONFIGURATION/$$TARGET_ARCH_ABI
LIBS += -lAssimp32

win32 {
    assimp.files = $(SDK_ASSIMP)/$$CONFIGURATION/$$TARGET_ARCH_ABI/Assimp32.dll
    assimp.path = $$DEPLOY_DIR
    INSTALLS += assimp
}

android {
    ANDROID_EXTRA_LIBS = $$SDK_ASSIMP/$$CONFIGURATION/$$TARGET_ARCH_ABI/libAssimp32.so
}
