INCLUDEPATH += $$(SDK_SDL2MIXER)

win32 {
    LIBS += -L$$(SDK_SDL2MIXER)/VisualC/win32/$$CONFIGURATION
}

LIBS += -lSDL2_mixer

win32 {

    sdl2mixer.files = $$(SDK_SDL2MIXER)/VisualC/Win32/$$CONFIGURATION/*.dll
    sdl2mixer.files += $$(SDK_SDL2MIXER)/VisualC/Win32/$$CONFIGURATION/*.txt
    sdl2mixer.path = $$DEPLOY_DIR
    INSTALLS += sdl2mixer

}
