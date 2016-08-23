INCLUDEPATH += $$(SDK_SDL2IMAGE)/include

win32 {
    LIBS += -L$$(SDK_SDL2IMAGE)/VisualC/$$CONFIGURATION
}

LIBS += -lSDL2_image

win32 {

    sdl2image.files += $$(SDK_SDL2IMAGE)/VisualC/$$CONFIGURATION/SDL2_image.dll
    sdl2image.files += $$(SDK_SDL2IMAGE)/VisualC/external/lib/x86/*.*
    sdl2image.path = $$DEPLOY_DIR
    INSTALLS += sdl2image
}

