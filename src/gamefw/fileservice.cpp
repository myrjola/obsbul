#include "../common.h"

#include <fstream>

#include <FreeImagePlus.h>
#include <physfs.h>

#include "fileservice.h"

#include "gamefw.h"

using namespace gamefw;

FileService::FileService()
{
    FreeImage_Initialise( 0 );
    if (!PHYSFS_isInit())
        PHYSFS_init(NULL);
    PHYSFS_setSaneConfig("config", PROJECT_NAME, "ob", 0, 1);

    m_dirseparator.assign(PHYSFS_getDirSeparator());

    // Find the project directory
    string basedir(PHYSFS_getBaseDir());

    string project_name(PROJECT_NAME);

    int project_path_pos = basedir.find(project_name);
    if (project_path_pos == basedir.npos) {
        DLOG(FATAL) << "Project " << project_name <<
        " root directory not found from " << basedir;
    }
    basedir.erase(project_path_pos + project_name.length());

    PHYSFS_mount(basedir.c_str(), NULL, 0); // Mount to root.

    m_entity_factory = new EntityFactory();
}

FileService::~FileService()
{
    delete m_entity_factory;
    PHYSFS_deinit();
    FreeImage_DeInitialise();
}

const char* FileNotFoundException::what() const throw()
{
    return "File not found.";
}

const char* FileService::fileToBuffer(const string& filename ) const
{
    const string realpath(getRealPath(filename));

	// Open file at end.
    ifstream file( realpath.c_str(), ios::in | ios::ate );

    if ( file.is_open() ) {
        uint size = (uint) file.tellg(); // Get file size.
        // The +1 is for '\0'.
        char* buffer = new char[size + 1];
        file.seekg( 0, ios::beg ); // Back to start.
		char* c = buffer;
        // Read the file to buffer.
		while (file.good()) {
            file.get(*c);
			c++;
		}

        file.close();

        *(c - 1) = '\0'; // Delete EOF.

        DLOG( INFO ) << filename << " loaded into buffer.";

        return buffer;
    }

    DLOG( ERROR ) << filename << " not found.";
    throw FileNotFoundException();
}

GLuint FileService::makeTexture( const string& name ) const
{
    fipImage* image = readImage( name );

    assert( image->isValid() );

    int width = image->getWidth();
    int height = image->getHeight();
    void* pixels = (void*) image->accessPixels();
    GLuint texture;

    glGenTextures( 1, &texture );
    glBindTexture( GL_TEXTURE_2D, texture );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexImage2D(
        GL_TEXTURE_2D, 0,           // target, level
        GL_RGB8,                    // internal format
        width, height, 0,           // width, height, border
        GL_BGR, GL_UNSIGNED_BYTE,   // external format, type
        pixels                      // pixels
    );

    image->clear();
    delete image;

    return texture;

}

fipImage* FileService::readImage( const string& name ) const
{
    string filename = "assets/images/" + name + ".png";

    string realpath(getRealPath(filename));

    fipImage* image = new fipImage;

    if ( !image->load( realpath.c_str() ) ) {
        assert(false); // Shouldn't fail.
    }
    image->convertTo24Bits();

    DLOG( INFO ) << filename << " loaded to texture.";
    return image;
}

const string FileService::getRealPath(const string& path) const
{
    if (!PHYSFS_exists(path.c_str())) {
        DLOG( ERROR ) << path << " not found.";
        throw FileNotFoundException();
    }
    string realpath(PHYSFS_getRealDir(path.c_str()));
    realpath += m_dirseparator + path;
    return realpath;
}

Entity FileService::createEntity(const string& name) const
{
    string path = "assets/entities/" + name + ".xml";
    string realpath(getRealPath(path));
    return m_entity_factory->createEntity(realpath);
}

const std::string gamefw::FileService::getDirSeparator() const
{
    return m_dirseparator;
}
