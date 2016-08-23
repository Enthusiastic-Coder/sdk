INCLUDEPATH += $(SDK_ASSIMP)/include

LIBS += -L$(SDK_ASSIMP)/$$CONFIGURATION/$$TARGET_ARCH_ABI
LIBS += -lAssimp32

win32 {
    depends.files = $(SDK_ASSIMP)/$$CONFIGURATION/$$TARGET_ARCH_ABI/Assimp32.dll
    depends.path = $$DEPLOY_DIR
    INSTALLS += depends
}

android {
    ANDROID_EXTRA_LIBS = $$SDK_ASSIMP/$$CONFIGURATION/$$TARGET_ARCH_ABI/libAssimp32.so
}
