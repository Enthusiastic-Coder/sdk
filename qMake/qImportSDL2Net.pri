INCLUDEPATH += $$(SDK_SDL2NET)

win32 {
    LIBS += -L$$(SDK_SDL2NET)/VisualC/win32/$$CONFIGURATION
}

LIBS += -lSDL2_net

win32 {

    sdl2net.files = $$(SDK_SDL2NET)/VisualC/Win32/$$CONFIGURATION/SDL2_net.dll
    sdl2net.path = $$DEPLOY_DIR
    INSTALLS += sdl2net

}
