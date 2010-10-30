#include "objfile.h"

#include <fstream>
#include <stdexcept>

namespace gamefw {

ObjFile::objFile(string path)
:
m_path(path),
m_num_positions(0),
m_num_normals(0),
m_num_texcoords(0),
m_num_triangles(0),
m_num_materials(0)
{
    ifstream file(path);
    
    assert(file.is_open());

    parseFile(&file);

    file.close();
}

void ObjFile::parseFile(ifstream* file)
{
    streamsize LARGE_NUMBER = std::numeric_limits<std::streamsize>::max();
    uint line_number = 1;
    uint current_mtl = 0; // Default material.
    string definition;
    GLfloat v1, v2, v3;
    while (!file->eof()) {
        file >> definition;
        if (definition.at(0) == 'v') { // Vertex varyings.
            file >> v1 >> v2;
            if (definition == "vt") { // Texcoord
                addTexCoord(v1, v2);
            } else {
                file >> v3;
            }
            // Bail out if incorrect format.
            if (file->bad()) {
                DLOG(ERROR) << "Incorrect format in " <<
                    m_path << ":" << line_number;
                throw CorruptObjFileException();
            }
                
            if (definition == "v") { // Position.
                addPosition(v1, v2, v3);

            } else if (definition == "vn") { // Normal
                addNormal(v1, v2, v3);

            } else {
                DLOG(ERROR) << "Unknown definition in " <<
                    m_path << ":" << line_number;
                throw CorruptObjFileException();
            }
            
        } else if (definition == "mtllib") {
            string mtllib_name;
            file >> mtllib_name; // Next word.
            loadMaterials(mtllib_name);
            
        } else if (definition == "usemtl") {
            string mtl_name;
            file >> mtl_name;
            try {
                current_mtl = mtl_table.at(mtl_name);
            } catch (std::out_of_range) {
                DLOG(ERROR) << "Unknown material in " << 
                    m_path << ":" << line_number;
                throw CorruptObjFileException();
            }
            
        } else if (definition == "f") { // Triangle.
            // position, normal, texcoord.
            GLuint p1, n1, t1;
            GLuint p2, n2, t2;
            GLuint p3, n3, t3;
            
            char slash;
            // Recognize only %d/%d/%d.
            file >> p1 >> slash >> n1 >> slash >> t1;
            file >> p2 >> slash >> n2 >> slash >> t2;
            file >> p3 >> slash >> n3 >> slash >> t3;
            
            addTriangle(p1, n1, t1, p2, n2, t2,
                        p3, n3, t3, current_mtl);
        }

        // Next line.
        line_number++;
        file->ignore(LARGE_NUMBER, '\n');
    }
}

const char* CorruptObjFileException::what() const
{
    return "Corrupt obj-file.";
}

void ObjFile::addPosition(GLfloat x, GLfloat y, GLfloat z)
{
    m_positions.push_back(x);
    m_positions.push_back(y);
    m_positions.push_back(z);
    m_num_positions++;
}

void ObjFile::addNormal(GLfloat x, GLfloat y, GLfloat z)
{
    m_normals.push_back(x);
    m_normals.push_back(y);
    m_normals.push_back(z);
    m_num_normals++;
}

void ObjFile::addTexcoord(GLfloat u, GLfloat v)
{
    m_texcoords.push_back(u);
    m_texcoords.push_back(v);
    m_num_texcoords++;
}

void ObjFile::addTriangle(GLuint p1, GLuint n1, GLuint t1,
                          GLuint p2, GLuint n2, GLuint t2,
                          GLuint p3, GLuint n3, GLuint t3,
                          GLuint mtl_index)
{
    GLuint pindices[] = {p1, p2, p3};
    GLuint nindices[] = {n1, n2, n3};
    GLuint tindices[] = {t1, t2, t3};
    t_obj_triangle triangle = {pindices, nindices, tindices, mtl_index};
}

void ObjFile::loadMaterials(string arg1)
{

}

/* glmReadMTL: read a wavefront material library file
 *
 * model - properly initialized GLMmodel structure
 * name  - name of the material library
 */
static GLvoid
glmReadMTL(t_obj_model* model, char* name)
{
    FILE* file;
    char* dir;
    char* filename;
    char* t_filename;
    char    buf[128];
    GLuint nummaterials, i;
    
    dir = __glmDirName(model->pathname);
    filename = (char*)malloc(sizeof(char) * (strlen(dir) + strlen(name) + 1));
	strcpy(filename, dir);
    strcat(filename, name);
    
    file = fopen(filename, "r");
    if (!file) {
        __glmFatalError( "glmReadMTL() failed: can't open material file \"%s\".",
			 filename);
    }
    free(filename);
    
    /* count the number of materials in the file */
    nummaterials = 1;
    while(fscanf(file, "%s", buf) != EOF) {
        switch(buf[0]) {
        case '#':               /* comment */
            /* eat up rest of line */
            fgets(buf, sizeof(buf), file);
            break;
        case 'n':               /* newmtl */
	    if(strncmp(buf, "newmtl", 6) != 0)
		__glmFatalError("glmReadMTL: Got \"%s\" instead of \"newmtl\" in file \"%s\"", buf, filename);
            fgets(buf, sizeof(buf), file);
            nummaterials++;
            sscanf(buf, "%s %s", buf, buf);
            break;
        default:
            /* eat up rest of line */
            fgets(buf, sizeof(buf), file);
            break;
        }
    }
    
    rewind(file);
    
    model->materials = (t_obj_mtl*)malloc(sizeof(t_obj_mtl) * nummaterials);
    model->nummaterials = nummaterials;
    
    /* set the default material */
    for (i = 0; i < nummaterials; i++) {
        model->materials[i].name = NULL;
        model->materials[i].shininess = 65.0;
        model->materials[i].diffuse[0] = 0.8;
        model->materials[i].diffuse[1] = 0.8;
        model->materials[i].diffuse[2] = 0.8;
        model->materials[i].diffuse[3] = 1.0;
        model->materials[i].ambient[0] = 0.2;
        model->materials[i].ambient[1] = 0.2;
        model->materials[i].ambient[2] = 0.2;
        model->materials[i].ambient[3] = 1.0;
        model->materials[i].specular[0] = 0.0;
        model->materials[i].specular[1] = 0.0;
        model->materials[i].specular[2] = 0.0;
        model->materials[i].specular[3] = 1.0;
        model->materials[i].map_diffuse = -1;
    }
    model->materials[0].name = __glmStrdup("default");
    
    /* now, read in the data */
    nummaterials = 0;
    while(fscanf(file, "%s", buf) != EOF) {
        switch(buf[0]) {
        case '#':               /* comment */
            /* eat up rest of line */
            fgets(buf, sizeof(buf), file);
            break;
        case 'n':               /* newmtl */
#if 0
            __glmWarning("name=%s; Ns=%g; Ka=%g,%g,%g; Kd=%g,%g,%g; Ks=%g,%g,%g",
			 model->materials[nummaterials].name,
			 model->materials[nummaterials].shininess/128.0*GLM_MAX_SHININESS,
			 model->materials[nummaterials].ambient[0],
			 model->materials[nummaterials].ambient[1],
			 model->materials[nummaterials].ambient[2],
			 model->materials[nummaterials].diffuse[0],
			 model->materials[nummaterials].diffuse[1],
			 model->materials[nummaterials].diffuse[2],
			 model->materials[nummaterials].specular[0],
			 model->materials[nummaterials].specular[1],
			 model->materials[nummaterials].specular[2]);
#endif
	    if(strncmp(buf, "newmtl", 6) != 0)
		__glmFatalError("glmReadMTL: Got \"%s\" instead of \"newmtl\" in file \"%s\"", buf, filename);
            fgets(buf, sizeof(buf), file);
            sscanf(buf, "%s %s", buf, buf);
            nummaterials++;
            model->materials[nummaterials].name = __glmStrdup(buf);
            break;
        case 'N':
            switch(buf[1]) {
            case 's':
                fscanf(file, "%f", &model->materials[nummaterials].shininess);
                /* wavefront shininess is from [0, 1000], so scale for OpenGL */
                model->materials[nummaterials].shininess /= GLM_MAX_SHININESS;
                model->materials[nummaterials].shininess *= 128.0;
                break;
            case 'i':
                /* Refraction index.  Values range from 1 upwards. A value
                   of 1 will cause no refraction. A higher value implies
                   refraction. */
                __glmWarning("refraction index ignored");
                fgets(buf, sizeof(buf), file);
                break;
            default:
		__glmWarning("glmReadMTL: Command \"%s\" ignored", buf);
                fgets(buf, sizeof(buf), file);
                break;
            }
	    break;
        case 'K':
            switch(buf[1]) {
            case 'd':
                fscanf(file, "%f %f %f",
		       &model->materials[nummaterials].diffuse[0],
		       &model->materials[nummaterials].diffuse[1],
		       &model->materials[nummaterials].diffuse[2]);
                break;
            case 's':
                fscanf(file, "%f %f %f",
		       &model->materials[nummaterials].specular[0],
		       &model->materials[nummaterials].specular[1],
		       &model->materials[nummaterials].specular[2]);
                break;
            case 'a':
                fscanf(file, "%f %f %f",
		       &model->materials[nummaterials].ambient[0],
		       &model->materials[nummaterials].ambient[1],
		       &model->materials[nummaterials].ambient[2]);
                break;
            default:
		__glmWarning("glmReadMTL: Command \"%s\" ignored", buf);
                /* eat up rest of line */
                fgets(buf, sizeof(buf), file);
                break;
            }
            break;
        case 'd':
            /* d = Dissolve factor (pseudo-transparency).
               Values are from 0-1. 0 is completely transparent, 1 is opaque. */
	{
	    float alpha;
	    fscanf(file, "%f", &alpha);
	    model->materials[nummaterials].diffuse[3] = alpha;
	}
	break;
        case 'i':
	    if(strncmp(buf, "illum", 5) != 0)
		__glmFatalError("glmReadMTL: Got \"%s\" instead of \"illum\" in file \"%s\"", buf, filename);
            /* illum = (0, 1, or 2) 0 to disable lighting, 1 for
               ambient & diffuse only (specular color set to black), 2
               for full lighting. I've also seen values of 3 and 4 for
               'illum'... when there's a 3 there, there's often a
               'sharpness' attribute, but I didn't find any
               explanation. And I think the 4 illum value is supposed
               to denote two-sided polygons, but I kinda get the
               impression that some people just make stuff up and add
               whatever they want to these files, so there could be
               anything in there ;). */
	    {
		int illum;
		fscanf(file, "%d", &illum);
		if(illum != 2)	/* illum=2 is standard lighting */
		    __glmWarning("illum material ignored: illum %d", illum);
	    }
	    break;
        case 'm':
            /* texture map */
            filename = malloc(FILENAME_MAX);
            fgets(filename, FILENAME_MAX, file);
            t_filename = __glmStrStrip((char*)filename);
            free(filename);
            if(strncmp(buf, "map_Kd", 6) == 0) {
                model->materials[nummaterials].map_diffuse = glmFindOrAddTexture(model, t_filename);
                free(t_filename);
            } else {
                __glmWarning("map %s %s ignored",buf,t_filename);
                free(t_filename);
                fgets(buf, sizeof(buf), file);
            }
            break;
        case 'r':
            /* reflection type and filename (?) */
	    fgets(buf, sizeof(buf), file);
            __glmWarning("reflection type ignored: r%s",buf);
	    break;
	default:
	    /* eat up rest of line */
	    fgets(buf, sizeof(buf), file);
	    break;
        }
    }
    free(dir);
    fclose(file);
}

/* glmFirstPass: first pass at a Wavefront OBJ file that gets all the
 * statistics of the model (such as #vertices, #normals, etc)
 *
 * model - properly initialized GLMmodel structure
 * file  - (fopen'd) file descriptor 
 */
static GLvoid
glmFirstPass(t_obj_model* model, FILE* file)
{
    GLuint  numvertices;        /* number of vertices in model */
    GLuint  numnormals;         /* number of normals in model */
    GLuint  numtexcoords;       /* number of texcoords in model */
    GLuint  numtriangles;       /* number of triangles in model */
    t_obj_group* group;            /* current group */
    unsigned    v, n, t;
    char        buf[128];
    
    /* make a default group */
    group = glmAddGroup(model, "default");
    
    numvertices = numnormals = numtexcoords = numtriangles = 0;
    while(fscanf(file, "%s", buf) != EOF) {
        switch(buf[0]) {
        case '#':               /* comment */
            /* eat up rest of line */
            fgets(buf, sizeof(buf), file);
            break;
        case 'v':               /* v, vn, vt */
            switch(buf[1]) {
            case '\0':          /* vertex */
                /* eat up rest of line */
                fgets(buf, sizeof(buf), file);
                numvertices++;
                break;
            case 'n':           /* normal */
                /* eat up rest of line */
                fgets(buf, sizeof(buf), file);
                numnormals++;
                break;
            case 't':           /* texcoord */
                /* eat up rest of line */
                fgets(buf, sizeof(buf), file);
                numtexcoords++;
                break;
            default:
                __glmFatalError("glmFirstPass(): Unknown token \"%s\".", buf);
                break;
            }
            break;
	case 'm':
	    if(strncmp(buf, "mtllib", 6) != 0)
		__glmFatalError("glmReadOBJ: Got \"%s\" instead of \"mtllib\"", buf);
	    fgets(buf, sizeof(buf), file);
	    sscanf(buf, "%s %s", buf, buf);
	    model->mtllibname = __glmStrStrip((char*)buf);
	    glmReadMTL(model, model->mtllibname);
	    break;
	case 'u':
	    if(strncmp(buf, "usemtl", 6) != 0)
		__glmFatalError("glmReadOBJ: Got \"%s\" instead of \"usemtl\"", buf);
	    /* eat up rest of line */
	    fgets(buf, sizeof(buf), file);
	    break;
	case 'g':               /* group */
	    /* eat up rest of line */
	    fgets(buf, sizeof(buf), file);
	    buf[strlen(buf)-1] = '\0';  /* nuke '\n' */
#endif
	    group = glmAddGroup(model, buf);
	    break;
	case 'f':               /* face */
	    v = n = t = 0;
	    fscanf(file, "%s", buf);
	    /* can be one of %d, %d//%d, %d/%d, %d/%d/%d %d//%d */
                if (strstr(buf, "//")) {
                    /* v//n */
                    sscanf(buf, "%d//%d", &v, &n);
                    fscanf(file, "%d//%d", &v, &n);
                    fscanf(file, "%d//%d", &v, &n);
                    numtriangles++;
                    group->numtriangles++;
                    while(fscanf(file, "%d//%d", &v, &n) > 0) {
                        numtriangles++;
                        group->numtriangles++;
                    }
                } else if (sscanf(buf, "%d/%d/%d", &v, &t, &n) == 3) {
                    /* v/t/n */
                    fscanf(file, "%d/%d/%d", &v, &t, &n);
                    fscanf(file, "%d/%d/%d", &v, &t, &n);
                    numtriangles++;
                    group->numtriangles++;
                    while(fscanf(file, "%d/%d/%d", &v, &t, &n) > 0) {
                        numtriangles++;
                        group->numtriangles++;
                    }
                } else if (sscanf(buf, "%d/%d", &v, &t) == 2) {
                    /* v/t */
                    fscanf(file, "%d/%d", &v, &t);
                    fscanf(file, "%d/%d", &v, &t);
                    numtriangles++;
                    group->numtriangles++;
                    while(fscanf(file, "%d/%d", &v, &t) > 0) {
                        numtriangles++;
                        group->numtriangles++;
                    }
                } else {
                    /* v */
                    fscanf(file, "%d", &v);
                    fscanf(file, "%d", &v);
                    numtriangles++;
                    group->numtriangles++;
                    while(fscanf(file, "%d", &v) > 0) {
                        numtriangles++;
                        group->numtriangles++;
                    }
                }
                break;
                
	default:
	    /* eat up rest of line */
	    fgets(buf, sizeof(buf), file);
	    break;
        }
    }
  
    /* set the stats in the model structure */
    model->numvertices  = numvertices;
    model->numnormals   = numnormals;
    model->numtexcoords = numtexcoords;
    model->numtriangles = numtriangles;
  
    /* allocate memory for the triangles in each group */
    group = model->groups;
    while(group) {
	group->triangles = (GLuint*)malloc(sizeof(GLuint) * group->numtriangles);
	group->numtriangles = 0;
	group = group->next;
    }
}

/* glmSecondPass: second pass at a Wavefront OBJ file that gets all
 * the data.
 *
 * model - properly initialized GLMmodel structure
 * file  - (fopen'd) file descriptor 
 */
static GLvoid
glmSecondPass(t_obj_model* model, FILE* file)
{
    GLuint  numvertices;        /* number of vertices in model */
    GLuint  numnormals;         /* number of normals in model */k
    GLuint  numtexcoords;       /* number of texcoords in model */
    GLuint  numtriangles;       /* number of triangles in model */
    GLfloat*    vertices;           /* array of vertices  */
    GLfloat*    normals;            /* array of normals */
    GLfloat*    texcoords;          /* array of texture coordinates */
    t_obj_group* group;            /* current group pointer */
    GLuint  material;           /* current material */
    unsigned int v, n, t;
    char        buf[128];
    
    /* set the pointer shortcuts */
    vertices       = model->vertices;
    normals    = model->normals;
    texcoords    = model->texcoords;
    group      = model->groups;
    
    /* on the second pass through the file, read all the data into the
    allocated arrays */
    numvertices = numnormals = numtexcoords = 1;
    numtriangles = 0;
    material = 0;
    while(fscanf(file, "%s", buf) != EOF) {
        switch(buf[0]) {
        case '#':               /* comment */
            /* eat up rest of line */
            fgets(buf, sizeof(buf), file);
            break;
        case 'v':               /* v, vn, vt */
            switch(buf[1]) {
            case '\0':          /* vertex */
                fscanf(file, "%f %f %f", 
                    &vertices[3 * numvertices + 0], 
                    &vertices[3 * numvertices + 1], 
                    &vertices[3 * numvertices + 2]);
                numvertices++;
                break;
            case 'n':           /* normal */
                fscanf(file, "%f %f %f", 
                    &normals[3 * numnormals + 0],
                    &normals[3 * numnormals + 1], 
                    &normals[3 * numnormals + 2]);
                numnormals++;
                break;
            case 't':           /* texcoord */
                fscanf(file, "%f %f", 
                    &texcoords[2 * numtexcoords + 0],
                    &texcoords[2 * numtexcoords + 1]);
                numtexcoords++;
                break;
            }
            break;
            case 'u':
                fgets(buf, sizeof(buf), file);
                sscanf(buf, "%s %s", buf, buf);
                material = glmFindMaterial(model, buf);
                if(!group->material && group->numtriangles)
                    group->material = material;
                break;
            case 'g':               /* group */
                /* eat up rest of line */
                fgets(buf, sizeof(buf), file);
                buf[strlen(buf)-1] = '\0';  /* nuke '\n' */
                group = glmFindGroup(model, buf);
                break;
            case 'f':               /* face */
                v = n = t = 0;
		T(numtriangles).findex = -1;
                if(group->material == 0)
                    group->material = material;
                T(numtriangles).material = material;
                fscanf(file, "%s", buf);
                /* can be one of %d, %d//%d, %d/%d, %d/%d/%d %d//%d */
                if (strstr(buf, "//")) {
                    /* v//n */
                    sscanf(buf, "%u//%u", &v, &n);
                    T(numtriangles).vindices[0] = v;
                    T(numtriangles).tindices[0] = -1;
                    T(numtriangles).nindices[0] = n;
                    fscanf(file, "%u//%u", &v, &n);
                    T(numtriangles).vindices[1] = v;
                    T(numtriangles).tindices[1] = -1;
                    T(numtriangles).nindices[1] = n;
                    fscanf(file, "%u//%u", &v, &n);
                    T(numtriangles).vindices[2] = v;
		    T(numtriangles).tindices[2] = -1;
                    T(numtriangles).nindices[2] = n;
                    group->triangles[group->numtriangles++] = numtriangles;
                    numtriangles++;
                    while(fscanf(file, "%u//%u", &v, &n) > 0) {
                        T(numtriangles).material = material;
                        T(numtriangles).vindices[0] = T(numtriangles-1).vindices[0];
                        T(numtriangles).tindices[0] = T(numtriangles-1).tindices[0];
                        T(numtriangles).nindices[0] = T(numtriangles-1).nindices[0];
                        T(numtriangles).vindices[1] = T(numtriangles-1).vindices[2];
                        T(numtriangles).tindices[1] = T(numtriangles-1).tindices[2];
                        T(numtriangles).nindices[1] = T(numtriangles-1).nindices[2];
                        T(numtriangles).vindices[2] = v;
                        T(numtriangles).tindices[2] = -1;
                        T(numtriangles).nindices[2] = n;
                        group->triangles[group->numtriangles++] = numtriangles;
                        numtriangles++;
                    }
                } else if (sscanf(buf, "%u/%u/%u", &v, &t, &n) == 3) {
                    /* v/t/n */
                    T(numtriangles).vindices[0] = v;
                    T(numtriangles).tindices[0] = t;
                    T(numtriangles).nindices[0] = n;
                    fscanf(file, "%u/%u/%u", &v, &t, &n);
                    T(numtriangles).vindices[1] = v;
                    T(numtriangles).tindices[1] = t;
                    T(numtriangles).nindices[1] = n;
                    fscanf(file, "%u/%u/%u", &v, &t, &n);
                    T(numtriangles).vindices[2] = v;
                    T(numtriangles).tindices[2] = t;
                    T(numtriangles).nindices[2] = n;
                    group->triangles[group->numtriangles++] = numtriangles;
                    numtriangles++;
                    while(fscanf(file, "%u/%u/%u", &v, &t, &n) > 0) {
                        T(numtriangles).material = material;
                        T(numtriangles).vindices[0] = T(numtriangles-1).vindices[0];
                        T(numtriangles).tindices[0] = T(numtriangles-1).tindices[0];
                        T(numtriangles).nindices[0] = T(numtriangles-1).nindices[0];
                        T(numtriangles).vindices[1] = T(numtriangles-1).vindices[2];
                        T(numtriangles).tindices[1] = T(numtriangles-1).tindices[2];
                        T(numtriangles).nindices[1] = T(numtriangles-1).nindices[2];
                        T(numtriangles).vindices[2] = v;
                        T(numtriangles).tindices[2] = t;
                        T(numtriangles).nindices[2] = n;
                        group->triangles[group->numtriangles++] = numtriangles;
                        numtriangles++;
                    }
                } else if (sscanf(buf, "%u/%u", &v, &t) == 2) {
                    /* v/t */
                    T(numtriangles).vindices[0] = v;
                    T(numtriangles).tindices[0] = t;
		    T(numtriangles).nindices[0] = -1;
		    fscanf(file, "%u/%u", &v, &t);
                    T(numtriangles).vindices[1] = v;
                    T(numtriangles).tindices[1] = t;
		    T(numtriangles).nindices[1] = -1;
                    fscanf(file, "%u/%u", &v, &t);
                    T(numtriangles).vindices[2] = v;
                    T(numtriangles).tindices[2] = t;
		    T(numtriangles).nindices[2] = -1;
                    group->triangles[group->numtriangles++] = numtriangles;
                    numtriangles++;
                    while(fscanf(file, "%u/%u", &v, &t) > 0) {
                        T(numtriangles).material = material;
                        T(numtriangles).vindices[0] = T(numtriangles-1).vindices[0];
                        T(numtriangles).tindices[0] = T(numtriangles-1).tindices[0];
                        T(numtriangles).nindices[0] = T(numtriangles-1).nindices[0];
                        T(numtriangles).vindices[1] = T(numtriangles-1).vindices[2];
                        T(numtriangles).tindices[1] = T(numtriangles-1).tindices[2];
                        T(numtriangles).nindices[1] = T(numtriangles-1).nindices[2];
                        T(numtriangles).vindices[2] = v;
                        T(numtriangles).tindices[2] = t;
                        T(numtriangles).nindices[2] = -1;
                        group->triangles[group->numtriangles++] = numtriangles;
                        numtriangles++;
                    }
                } else {
                    /* v */
                    sscanf(buf, "%u", &v);
                    T(numtriangles).vindices[0] = v;
                    T(numtriangles).tindices[0] = -1;
		    T(numtriangles).nindices[0] = -1;
		    fscanf(file, "%u", &v);
                    T(numtriangles).vindices[1] = v;
                    T(numtriangles).tindices[1] = -1;
		    T(numtriangles).nindices[1] = -1;
                    fscanf(file, "%u", &v);
                    T(numtriangles).vindices[2] = v;
                    T(numtriangles).tindices[2] = -1;
		    T(numtriangles).nindices[2] = -1;
                    group->triangles[group->numtriangles++] = numtriangles;
                    numtriangles++;
                    while(fscanf(file, "%u", &v) > 0) {
                        T(numtriangles).material = material;
                        T(numtriangles).vindices[0] = T(numtriangles-1).vindices[0];
                        T(numtriangles).tindices[0] = T(numtriangles-1).tindices[0];
                        T(numtriangles).nindices[0] = T(numtriangles-1).nindices[0];
                        T(numtriangles).vindices[1] = T(numtriangles-1).vindices[2];
                        T(numtriangles).tindices[1] = T(numtriangles-1).tindices[2];
                        T(numtriangles).nindices[1] = T(numtriangles-1).nindices[2];
                        T(numtriangles).vindices[2] = v;
                        T(numtriangles).tindices[2] = -1;
                        T(numtriangles).nindices[2] = -1;
                        group->triangles[group->numtriangles++] = numtriangles;
                        numtriangles++;
                    }
                }
                break;
                
            default:
                /* eat up rest of line */
                fgets(buf, sizeof(buf), file);
                break;
    }
  }
}


/* glmDelete: Deletes a GLMmodel structure.
 *
 * model - initialized GLMmodel structure
 */
GLvoid
glmDelete(t_obj_model* model)
{
    t_obj_group* group;
    GLuint i;
    
    assert(model);
    
    if (model->pathname)     free(model->pathname);
    if (model->mtllibname) free(model->mtllibname);
    if (model->vertices)     free(model->vertices);
    if (model->normals)  free(model->normals);
    if (model->texcoords)  free(model->texcoords);
    if (model->facetnorms) free(model->facetnorms);
    if (model->triangles)  free(model->triangles);
    if (model->materials) {
        for (i = 0; i < model->nummaterials; i++)
	    {
		free(model->materials[i].name);
	    }
        free(model->materials);
    }
    if (model->textures) {
        for (i = 0; i < model->numtextures; i++) {
            free(model->textures[i].name);
            glDeleteTextures(1,&model->textures[i].id);
        }
        free(model->textures);
    }
    while(model->groups) {
        group = model->groups;
        model->groups = model->groups->next;
        free(group->name);
        free(group->triangles);
        free(group);
    }
    
    free(model);
}

/* glmReadOBJ: Reads a model description from a Wavefront .OBJ file.
 * Returns a pointer to the created object which should be free'd with
 * glmDelete().
 *
 * filename - name of the file containing the Wavefront .OBJ format data.  
 */
t_obj_model*
glmReadOBJ(const char* filename)
{
    t_obj_model* model;
    FILE*   file;
    int i, j;

    /* open the file */
    file = fopen(filename, "r");
    if (!file) {
        __glmFatalError( "glmReadOBJ() failed: can't open data file \"%s\".",
			 filename);
    }

    /* allocate a new model */
    model = (t_obj_model*)malloc(sizeof(t_obj_model));
    model->pathname    = __glmStrdup(filename);
    model->mtllibname    = NULL;
    model->numvertices   = 0;
    model->vertices    = NULL;
    model->numnormals    = 0;
    model->normals     = NULL;
    model->numtexcoords  = 0;
    model->texcoords       = NULL;
    model->numfacetnorms = 0;
    model->facetnorms    = NULL;
    model->numtriangles  = 0;
    model->triangles       = NULL;
    model->nummaterials  = 0;
    model->materials       = NULL;
    model->numtextures  = 0;
    model->textures       = NULL;
    model->numgroups       = 0;
    model->groups      = NULL;
    model->position[0]   = 0.0;
    model->position[1]   = 0.0;
    model->position[2]   = 0.0;
    
    /* make a first pass through the file to get a count of the number
       of vertices, normals, texcoords & triangles */
    glmFirstPass(model, file);
    
    /* allocate memory */
    model->vertices = (GLfloat*)malloc(sizeof(GLfloat) *
				       3 * (model->numvertices + 1));
    model->triangles = (t_obj_triangle*)malloc(sizeof(t_obj_triangle) *
					    model->numtriangles);
    if (model->numnormals) {
        model->normals = (GLfloat*)malloc(sizeof(GLfloat) *
					  3 * (model->numnormals + 1));
    }
    if (model->numtexcoords) {
        model->texcoords = (GLfloat*)malloc(sizeof(GLfloat) *
					    2 * (model->numtexcoords + 1));
    }
    
    /* rewind to beginning of file and read in the data this pass */
    rewind(file);
    
    glmSecondPass(model, file);

    /* verify the indices */
    for (i = 0; i < model->numtriangles; i++) {
	if (T(i).findex != -1)
	    if (T(i).findex <= 0 || T(i).findex > model->numfacetnorms)
		__glmFatalError("facet index for triangle %d out of bounds (%d > %d)\n", i, T(i).findex, model->numfacetnorms);
	for (j=0; j<3; j++) {
	    if (T(i).nindices[j] != -1)
		if (T(i).nindices[j] <= 0 || T(i).nindices[j] > model->numnormals)
		    __glmFatalError("normal index for triangle %d out of bounds (%d > %d)\n", i, T(i).nindices[j], model->numnormals);
	    if (T(i).vindices[j] != -1)
		if (T(i).vindices[j] <= 0 || T(i).vindices[j] > model->numvertices)
		    __glmFatalError("vertex index for triangle %d out of bounds (%d > %d)\n", i, T(i).vindices[j], model->numvertices);
	}
    }

    /* close the file */
    fclose(file);
    
    return model;
}

}