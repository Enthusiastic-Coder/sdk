INCLUDEPATH += $$(SDK_SDL2)/include

win32 {
    LIBS += -L$$(SDK_SDL2)/VisualC/win32/$$CONFIGURATION
}

LIBS += -lSDL2main -lSDL2

win32 {
    sdl2.files += $$(SDK_SDL2)/VisualC/Win32/$$CONFIGURATION/SDL2.dll
    sdl2.path = $$DEPLOY_DIR
    INSTALLS += sdl2
}

