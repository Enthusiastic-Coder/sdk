INCLUDEPATH += $$(SDK_SDL2TTF)

win32 {
    LIBS += -L$$(SDK_SDL2TTF)/VisualC/Win32/$$CONFIGURATION
}

LIBS += -lSDL2_ttf

win32 {

    sdl2ttf.files = $$(SDK_SDL2TTF)/VisualC/Win32/$$CONFIGURATION/*.dll
    sdl2ttf.path = $$DEPLOY_DIR
    INSTALLS += sdl2ttf
}

