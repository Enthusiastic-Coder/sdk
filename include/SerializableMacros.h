#define SERIALIZE_WRITE_BEGIN(versionMajor,versionMinor) \
    void Write(IWriteStream* pIStream) const \
{\
    pIStream->Write(int(versionMajor)); \
    pIStream->Write(int(versionMinor));

#define SERIALIZE_WRITE_ENTRY(var) \
    pIStream->Write(var);

#define SERIALIZE_WRITE_ENTRY_PAIR(var) \
    pIStream->Write(var.first); \
    pIStream->Write(var.second);

#define SERIALIZE_WRITE_SUPER(base) \
    base::Write(pIStream);

#define SERIALIZE_WRITE_PTR_VECTOR(col ) \
    pIStream->Write(col.size()); \
    for (auto &it : col) \
    it->Write(pIStream);

#define SERIALIZE_WRITE_VECTOR(col ) \
    pIStream->Write(col.size()); \
    for (auto &it : col) \
    pIStream->Write(it);


#define SERIALIZE_WRITE_RAW_VECTOR( col ) \
    pIStream->Write(col.size()); \
    for (auto *it : col) \
    it->Write(pIStream);


#define SERIALIZE_WRITE_THIS_PTR_VECTOR() \
    pIStream->Write(size()); \
    for (auto &it : *this) \
    it->Write(pIStream);

#define SERIALIZE_WRITE_VECTOR_PAIR(col ) \
    pIStream->Write(col.size()); \
    for (auto &it : col) \
{ \
    pIStream->Write(it.first); \
    pIStream->Write(it.second); \
    }

#define SERIALIZE_WRITE_VECTOR_VECTOR_PAIR(col ) \
    pIStream->Write(col.size()); \
    for (auto &it : col) \
{ \
    pIStream->Write(it.size()); \
    for (auto &it2 : it) \
{ \
    pIStream->Write(it2.first); \
    pIStream->Write(it2.second); \
    } \
    }

#define SERIALIZE_WRITE_OBJECT(obj ) \
    obj.Write(pIStream);

#define SERIALIZE_WRITE_MAP(map ) \
    pIStream->Write(map.size()); \
    for (auto&it : map)\
{\
    pIStream->Write(it.first); \
    pIStream->Write(it.second); \
    }\

#define SERIALIZE_WRITE_END() \
    }


#define SERIALIZE_READ_BEGIN(versionMajorRequired,versionMinorRequired) \
    void Read(IReadStream* pIStream) \
{\
{\
    int versionMajor(0); \
    int versionMinor(0); \
    pIStream->Read(versionMajor); \
    pIStream->Read(versionMinor); \
    (int(versionMajorRequired) == versionMajor); \
    (int(versionMinorRequired) == versionMinor); \
    }

#define SERIALIZE_READ_ENTRY(var) \
    pIStream->Read(var);

#define SERIALIZE_READ_ENTRY_PAIR(var) \
    pIStream->Read(var.first); \
    pIStream->Read(var.second);

#define SERIALIZE_READ_SUPER(base) \
    base::Read(pIStream);

#define SERIALIZE_READ_OBJ_VECTOR( col ) \
{ \
    size_t count(0); \
    pIStream->Read( count ); \
    col.resize( count ); \
    for(int i=0; i < count; ++i) \
        col[i] = new std::remove_reference<decltype( *col[0])>::type; \
    for( auto &it : col ) \
{ \
    it->Read( pIStream );\
    }\
    }

#define SERIALIZE_READ_PTR_VECTOR( col ) \
{ \
    size_t count(0); \
    pIStream->Read( count ); \
    col.resize( count ); \
    for( auto &it : col ) \
{ \
    it = std::unique_ptr<std::remove_reference<decltype( *col.begin()->get() )>::type>(\
    new std::remove_reference<decltype( *col.begin()->get() )>::type );\
    it->Read( pIStream );\
    }\
    }

#define SERIALIZE_READ_RAW_VECTOR( col ) \
{ \
    size_t count(0); \
    pIStream->Read( count ); \
    col.resize( count ); \
    for( size_t i=0; i < count; ++i) \
{ \
    col[i] = new std::remove_reference<decltype( **col.begin() )>::type;\
    col[i]->Read( pIStream);\
    } \
    }

#define SERIALIZE_READ_THIS_PTR_VECTOR() \
{ \
    size_t count(0); \
    pIStream->Read( count ); \
    resize( count ); \
    for( auto &it : *this ) \
{ \
    it = std::unique_ptr<std::remove_reference<decltype( *begin()->get() )>::type>(\
    new std::remove_reference<decltype( *begin()->get() )>::type );\
    it->Read( pIStream );\
    }\
    }

#define SERIALIZE_READ_THIS_RAW_VECTOR() \
{ \
    size_t count(0); \
    pIStream->Read( count ); \
    resize( count ); \
    for( size_t i=0; i < count; ++i) \
{ \
    (*this)[i] = new std::remove_reference<decltype( **begin() )>::type;\
    (*this)[i]->Read( pIStream);\
    } \
    }

#define SERIALIZE_READ_VECTOR( col ) \
{ \
    size_t count(0); \
    pIStream->Read( count ); \
    col.resize( count ); \
    for( auto &it : col ) \
    pIStream->Read(it );\
    }

#define SERIALIZE_READ_VECTOR_PAIR( col ) \
{ \
    size_t count(0); \
    pIStream->Read( count ); \
    col.resize( count ); \
    for( auto &it : col ) \
{ \
    pIStream->Read(it.first );\
    pIStream->Read(it.second );\
    }\
    }

#define SERIALIZE_READ_VECTOR_VECTOR_PAIR( col ) \
{ \
    size_t count(0); \
    pIStream->Read( count ); \
    col.resize( count ); \
    for( auto &it : col ) \
{ \
    pIStream->Read( count ); \
    it.resize( count ); \
    for( auto &it2 : it ) \
{ \
    pIStream->Read(it2.first );\
    pIStream->Read(it2.second );\
    }\
    }\
    }

#define SERIALIZE_READ_OBJECT(obj ) \
    obj.Read( pIStream );

#define SERIALIZE_READ_MAP(obj ) \
{\
    size_t size(0);\
    pIStream->Read( size );\
    if( size ) \
{\
    for( size_t i=0; i < size; i++ )\
{\
    std::remove_const<decltype(obj.begin()->first)>::type  u;\
    pIStream->Read( u );\
    std::remove_const<decltype(obj.begin()->second)>::type v;\
    pIStream->Read( v ); \
    obj[u]=v;\
    }\
    }\
    }

#define SERIALIZE_READ_END() \
    }


#define	SERIALIZE_WRITE_CLASS(StreamClass )\
    void Save( std::string sFilename )\
{\
    std::ofstream outFile(sFilename, std::ios::ate | std::ios::binary );\
    if( !outFile.is_open() )\
    return;\
    StreamClass outStoreFile( outFile );\
    Write(&outStoreFile);\
    }

#define SERIALIZE_READ_CLASS(StreamClass) \
    bool Load( std::string sFilename)\
{\
    std::ifstream inFile(sFilename, std::ios::binary );\
    if( !inFile.is_open() )\
    return false;\
    StreamClass inStoreFile( inFile );\
    Read(&inStoreFile);\
    return true;\
    }
