

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

SOURCES += \
    $$PWD/source/action/Action.cpp \
    $$PWD/source/action/ActionGroup.cpp \
    $$PWD/source/action/Menu.cpp \
    $$PWD/source/maptiles/MapTileEntries.cpp \
    $$PWD/source/maptiles/MapTileEntry.cpp \
    $$PWD/source/maptiles/Flat3DSpere.cpp \
    $$PWD/source/maptiles/FlatSphere.cpp \
    $$PWD/source/maptiles/WorldTileRenderer.cpp \
    $$PWD/source/math/QVRHdgTable.cpp \
    $$PWD/source/math/qhdgtable.cpp \
    $$PWD/source/mesh/AssimpMeshManager.cpp \
    $$PWD/source/mesh/MeshData.cpp \
    $$PWD/source/mesh/MultSphereMeshObjBuilder.cpp \
    $$PWD/source/mesh/MultipleInstanceObjBuilder.cpp \
    $$PWD/source/mesh/ObjLoader.cpp \
    $$PWD/source/mesh/ObjectLites.cpp \
    $$PWD/source/mesh/SceneLoader.cpp \
    $$PWD/source/mesh/SubScene.cpp \
    $$PWD/source/mesh/meshloader.cpp \
    $$PWD/source/opengl/OpenGLFontMeshBuilder.cpp \
    $$PWD/source/opengl/OpenGLFontRenderer.cpp \
    $$PWD/source/opengl/OpenGLFontTexture.cpp \
    $$PWD/source/opengl/OpenGLPipeline.cpp \
    $$PWD/source/opengl/OpenGLRenderer.cpp \
    $$PWD/source/opengl/OpenGLShaderProgram.cpp \
    $$PWD/source/opengl/QtTextureManager.cpp \
    $$PWD/source/opengl/SkyBox.cpp \
    $$PWD/source/opengl/SkyDome.cpp \
    $$PWD/source/utilities/QmlClipboardAdapter.cpp \
    $$PWD/source/utilities/RandomStringGenerator.cpp \
    $$PWD/source/utilities/TextureCacheManager.cpp \
    $$PWD/source/utilities/permissions.cpp

HEADERS += \
    $$PWD/include/jibbs/action/Action.h \
    $$PWD/include/jibbs/action/ActionGroup.h \
    $$PWD/include/jibbs/action/ActionValue.h \
    $$PWD/include/jibbs/action/Menu.h \
    $$PWD/include/jibbs/boundary/BoundaryHelperT.h \
    $$PWD/include/jibbs/boundary/BoundingBox.h \
    $$PWD/include/jibbs/boundary/HeightData.h \
    $$PWD/include/jibbs/gps/GPSBoundary.h \
    $$PWD/include/jibbs/gps/GPSLocation.h \
    $$PWD/include/jibbs/gps/GPSModelTransform.h \
    $$PWD/include/jibbs/gps/GPSTileContainer.h \
    $$PWD/include/jibbs/maptiles/MapTileEntries.h \
    $$PWD/include/jibbs/maptiles/MapTileEntry.h \
    $$PWD/include/jibbs/maptiles/Flat3DSpere.h \
    $$PWD/include/jibbs/maptiles/FlatSphere.h \
    $$PWD/include/jibbs/maptiles/TileHelpers.h \
    $$PWD/include/jibbs/maptiles/WorldTileRenderer.h \
    $$PWD/include/jibbs/math/BezierEquation.h \
    $$PWD/include/jibbs/math/Frustum.h \
    $$PWD/include/jibbs/math/MassChannel.h \
    $$PWD/include/jibbs/math/MathSupport.h \
    $$PWD/include/jibbs/math/MeshHelper.h \
    $$PWD/include/jibbs/math/Plane.h \
    $$PWD/include/jibbs/math/QVRHdgTable.h \
    $$PWD/include/jibbs/math/Quarternion.h \
    $$PWD/include/jibbs/math/SinTable.h \
    $$PWD/include/jibbs/math/degreesRadians.h \
    $$PWD/include/jibbs/math/euler.h \
    $$PWD/include/jibbs/math/qhdgtable.h \
    $$PWD/include/jibbs/matrix/Matrix3x3.h \
    $$PWD/include/jibbs/matrix/Matrix4x4.h \
    $$PWD/include/jibbs/matrix/MatrixArrayCoords.h \
    $$PWD/include/jibbs/mesh/AssimpMeshManager.h \
    $$PWD/include/jibbs/mesh/MeshData.h \
    $$PWD/include/jibbs/mesh/MultSphereMeshObjBuilder.h \
    $$PWD/include/jibbs/mesh/MultipleInstanceObjBuilder.h \
    $$PWD/include/jibbs/mesh/ObjLoader.h \
    $$PWD/include/jibbs/mesh/ObjectLites.h \
    $$PWD/include/jibbs/mesh/SceneLoader.h \
    $$PWD/include/jibbs/mesh/SubScene.h \
    $$PWD/include/jibbs/mesh/meshloader.h \
    $$PWD/include/jibbs/mesh/pivotData.h \
    $$PWD/include/jibbs/mesh/vertexData.h \
    $$PWD/include/jibbs/opengl/OpenGLFontMeshBuilder.h \
    $$PWD/include/jibbs/opengl/OpenGLFontRenderer.h \
    $$PWD/include/jibbs/opengl/OpenGLFontTexture.h \
    $$PWD/include/jibbs/opengl/OpenGLPipeline.h \
    $$PWD/include/jibbs/opengl/OpenGLRenderer.h \
    $$PWD/include/jibbs/opengl/OpenGLShaderProgram.h \
    $$PWD/include/jibbs/opengl/QtTextureManager.h \
    $$PWD/include/jibbs/opengl/SkyBox.h \
    $$PWD/include/jibbs/opengl/SkyDome.h \
    $$PWD/include/jibbs/serializable/OGLTexture2D.h \
    $$PWD/include/jibbs/serializable/Serializable.h \
    $$PWD/include/jibbs/serializable/SerializableMacros.h \
    $$PWD/include/jibbs/serializable/SerializableStdStream.h \
    $$PWD/include/jibbs/utilities/QmlClipboardAdapter.h \
    $$PWD/include/jibbs/utilities/RandomStringGenerator.h \
    $$PWD/include/jibbs/utilities/TextureCacheManager.h \
    $$PWD/include/jibbs/utilities/permissions.h \
    $$PWD/include/jibbs/utilities/stdafx.h \
    $$PWD/include/jibbs/vector/vector2.h \
    $$PWD/include/jibbs/vector/vector3.h \
    $$PWD/include/jibbs/vector/vector4.h \
    $$PWD/include/jibbs/win/OGLWtlWndImpl.h \
    $$PWD/include/jibbs/win/Win32JoystickController.h \
    $$PWD/include/jibbs/win/openglhwnd.h
