#include <jibbs/mesh/SceneLoader.h>
#include <jibbs/mesh/AssimpMeshManager.h>
#include <jibbs/math/MeshHelper.h>

#include <assimp/cfileio.h>

#include <vertexData.h>
#include <QImage>
#include <QFileInfo>
#include <QDebug>
#include <QDir>
#include <QOpenGLFunctions>
#include <QtConcurrent/QtConcurrent>
#include <QObject>
#include <QMutexLocker>

#include <stdlib.h>


namespace {
struct myAiFile : public aiFile
{
    QFile qFile;
};

struct myFileIO : public aiFileIO
{
    myAiFile* file = nullptr;
};

// aiFile callbacks
typedef size_t          (*aiFileWriteProc) (C_STRUCT aiFile*,   const char*, size_t, size_t);

size_t   custom_FileSizeProc  (C_STRUCT aiFile* io)
{
    myAiFile* myIO = (myAiFile*)io;
    size_t sz = myIO->qFile.size();
    return sz;
}

C_ENUM aiReturn custom_Seek(C_STRUCT aiFile* io, size_t offset, C_ENUM aiOrigin origin)
{
    myAiFile* myIO = (myAiFile*)io;

//    if( origin == aiOrigin::)
    if(!myIO->qFile.isOpen())
        if( !myIO->qFile.open(QIODevice::ReadOnly))
            return aiReturn_FAILURE;

    return myIO->qFile.seek(offset) ? aiReturn_SUCCESS: aiReturn_FAILURE;
}

size_t custom_TellProc(C_STRUCT aiFile* io)
{
    myAiFile* myIO = (myAiFile*)io;
    return myIO->qFile.pos();
}

size_t custom_Read  (C_STRUCT aiFile* io,   char* buffer, size_t pSize, size_t pCount)
{
    myAiFile* myIO = (myAiFile*)io;

    size_t readBytes = 0;
    int bytesRead = 0;

    do {
        bytesRead = myIO->qFile.read(buffer+readBytes, pSize*pCount-readBytes);
        readBytes += bytesRead;
    } while( bytesRead > 0 );

    return readBytes;
}


aiFile* custom_Open (C_STRUCT aiFileIO* io, const char* filename, const char* mode)
{
    myFileIO* myIO = (myFileIO*)io;

    if( myIO->file )
    {
        if(myIO->file->qFile.fileName() != filename)
        {
            myIO->file->qFile.close();
            myIO->file->qFile.setFileName(filename);
        }

        if(!myIO->file->qFile.isOpen())
            if( !myIO->file->qFile.open(QIODevice::ReadOnly))
                return nullptr;
    }

    return myIO->file;
}

void    custom_Close (C_STRUCT aiFileIO* io, C_STRUCT aiFile*)
{
    myFileIO* myIO = (myFileIO*)io;

    if( myIO->file)
        myIO->file->qFile.close();
}
}

sceneLoader::sceneLoader(AssimpMeshManager* manager) :_manager(manager)
{

}

sceneLoader::~sceneLoader()
{
    QObject::disconnect(_watcher,nullptr,nullptr,nullptr);
    _watcher->waitForFinished();

    _meshes.clear();

#ifdef Q_OS_WIN
    qDebug() << "aiScene Unloaded: " << _filename;
#endif
}

void sceneLoader::setExtractorHooks(std::function<void ()> extractLambda, std::function<void()> unextractorLambda)
{
    _extractor = extractLambda;
    _unextractor = unextractorLambda;
}

void sceneLoader::load(const QString &filename)
{
    _filename= filename;

    QObject::connect(_watcher, &QFutureWatcher<void>::finished, this, &sceneLoader::onLoaded);

    _loadFuture = QtConcurrent::run([filename, this] {

        QMutexLocker lock(_manager->mutex());

        if( _extractor != nullptr)
            _extractor();

        myAiFile aiFile;
        aiFile.ReadProc = &custom_Read;
        aiFile.FileSizeProc = &custom_FileSizeProc;
        aiFile.SeekProc = &custom_Seek;
        aiFile.TellProc = &custom_TellProc;

        myFileIO customIO;
        customIO.file = &aiFile;
        customIO.OpenProc = &custom_Open;
        customIO.CloseProc = &custom_Close;

        const aiScene* scene = aiImportFileEx(filename.toStdString().c_str(),
                                              aiProcessPreset_TargetRealtime_MaxQuality|aiProcess_FlipUVs, &customIO);

        if( _unextractor != nullptr)
            _unextractor();

        if (scene == nullptr)
        {
            qDebug() << "The file wasn't successfuly opened " << filename;
            qDebug() << "Error : " << aiGetErrorString();
            emit onError();
            return;
        }

        Vector3F minV = _BoundingBox.getMin();
        Vector3F maxV = _BoundingBox.getMax();
        recursiveProcess(scene->mRootNode, scene, minV, maxV);
        _BoundingBox.setLimits(minV, maxV);
        _CG = (maxV.y - minV.y) * 0.3f + minV.y;
        aiReleaseImport(scene);

        QMutexLocker lock2(&_isLoadedMutex);
        _isLoaded = true;
    });

    _watcher->setFuture(_loadFuture);
}

void sceneLoader::recursiveProcess(aiNode* node,const aiScene* scene, Vector3F& minV, Vector3F &maxV)
{
    //process
    for(unsigned int i=0;i<node->mNumMeshes;i++)
    {
        aiMesh* mesh=scene->mMeshes[node->mMeshes[i]];

        BoundingBox bb = processMesh(node->mName,mesh,scene);

        minV.x = std::min(minV.x, bb.getMin().x);
        minV.y = std::min(minV.y, bb.getMin().y);
        minV.z = std::min(minV.z, bb.getMin().z);

        maxV.x = std::max(maxV.x, bb.getMax().x);
        maxV.y = std::max(maxV.y, bb.getMax().y);
        maxV.z = std::max(maxV.z, bb.getMax().z);
    }

    //recursion
    for(unsigned int i=0;i<node->mNumChildren;i++)
    {
        recursiveProcess(node->mChildren[i],scene, minV, maxV);
    }
}

BoundingBox sceneLoader::processMesh(aiString name, aiMesh* m,const aiScene* scene)
{
    std::shared_ptr<std::vector<vertexData>> data( std::make_shared<std::vector<vertexData>>());
    std::shared_ptr<std::vector<unsigned short>> indices(std::make_shared<std::vector<unsigned short>>());
    aiColor4D col;
    aiMaterial* mat=scene->mMaterials[m->mMaterialIndex];
    aiGetMaterialColor(mat,AI_MATKEY_COLOR_DIFFUSE,&col);
    Vector3F defaultColor(col.r,col.g,col.b);

    std::vector<Vector3F> vecData;

    for(unsigned int i=0;i<m->mNumVertices;i++)
    {
            vertexData tmp;
            Vector3F tmpVec;

            //position
            tmpVec.x=m->mVertices[i].x;
            tmpVec.y=m->mVertices[i].y;
            tmpVec.z=m->mVertices[i].z;
            tmp.position=tmpVec;
            vecData.push_back(tmpVec);

            //tangent
            if(m->mNormals)
            {
                //normals
                tmpVec.x=m->mNormals[i].x;
                tmpVec.y=m->mNormals[i].y;
                tmpVec.z=m->mNormals[i].z;
            }else{
#ifdef Q_OS_WIN
                qDebug() << "ProcessMesh missing Normal : " << _filename << ":" << QString(name.C_Str());
#endif
                tmpVec.z=tmpVec.x=0;
                tmpVec.y=1.0;
            }
            tmp.normal=tmpVec;


            //tangent
            if(m->mTangents)
            {
                tmpVec.x=m->mTangents[i].x;
                tmpVec.y=m->mTangents[i].y;
                tmpVec.z=m->mTangents[i].z;
            }else{
                tmpVec.x=1.0;
                tmpVec.y=tmpVec.z=0;
            }
            tmp.tangent=tmpVec;


            //colors
            if(m->mColors[0])
            {
                //!= material color
                tmpVec.x=m->mColors[0][i].r;
                tmpVec.y=m->mColors[0][i].g;
                tmpVec.z=m->mColors[0][i].b;
            }else{
                tmpVec=defaultColor;
            }
            tmp.color=tmpVec;

            //color
            if(m->mTextureCoords[0])
            {
                tmpVec.x=m->mTextureCoords[0][i].x;
                tmpVec.y=m->mTextureCoords[0][i].y;
            }else{
                tmpVec.x=tmpVec.y=tmpVec.z=0.0;
            }
            tmp.U=tmpVec.x;
            tmp.V=tmpVec.y;
            data->push_back(tmp);
    }

    for(unsigned int i=0;i<m->mNumFaces;i++)
    {
        aiFace face=m->mFaces[i];
        for(unsigned int j=0;j<face.mNumIndices;j++) //0..2
        {
            indices->push_back(face.mIndices[j]);
        }
    }

    QString textureFilename;
    for(unsigned int i=0;i<mat->GetTextureCount(aiTextureType_DIFFUSE);i++)
    {
        aiString str;
        mat->GetTexture(aiTextureType_DIFFUSE,i,&str);
        textureFilename = QString(str.C_Str());
        break;
    }

    _meshes.push_back(std::make_shared<SubScene>(name.C_Str(), data, indices, textureFilename));

    return MeshHelper::calcBoundingBox( &*vecData.begin(), vecData.size());
}

const std::vector<std::shared_ptr<SubScene>>& sceneLoader::getMeshes() const
{
    return _meshes;
}

void sceneLoader::addPivot(const QString &name, std::function<void (OpenGLMatrixStack &, pivotData&)> handler)
{
    const auto& meshes = getMeshes();

    auto it = std::find_if(meshes.begin(), meshes.end(), [name](std::shared_ptr<SubScene> m) {
       return name == m->name().c_str();
    });

    if( it != meshes.end())
        const_cast<SubScene*>(it->get())->setPivotHandler(handler);
}

void sceneLoader::asyncIsLoaded(std::function<void(bool)> callback)
{
    QMutexLocker lock(&_isLoadedMutex);
    callback(_isLoaded);
}

bool sceneLoader::InFrustum(const Frustum &f) const
{
    return _BoundingBox.InFrustum(f);
}

float sceneLoader::getCG() const
{
    return _CG;
}

const BoundingBox &sceneLoader::getBoundingBox() const
{
    return _BoundingBox;
}
