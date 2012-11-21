#include "ClothViewer.h"
#include "../utils/Mesh3DReader.h"

ClothViewer::ClothViewer(const char* _title, int _width, int _height) : TrackballViewer(_title, _width, _height)
{
    init();
}

void ClothViewer::idle()
{
    if (isWatchOn) {
        float prevTime = currentTime;
        currentTime = watch.stop();

        float timeElapsed = currentTime - prevTime;

        // TODO : apply forces to the cloth
    }
}

void ClothViewer::init()
{
    // initialize parent
    TrackballViewer::init();

    // set camera to look at world coordinate center
    set_scene_pos(Vector3(0.0, 0.0, 0.0), 2.0);

    // load shaders
    m_cartoonShader.create("cartoon.vs", "cartoon.fs");
}

void ClothViewer::reshape(int _w, int _h)
{
    TrackballViewer::reshape(_w, _h);
}

void ClothViewer::loadMesh(const std::string& filenameOBJ, const std::string& filenameMTL)
{
    // load mesh from obj
    Mesh3DReader::read(filenameOBJ, m_mesh, filenameMTL);

    // calculate normals
    m_mesh.calculateVertexNormals();

    // get bounding box & reset scene camera accordingly
    Vector3 bbmin, bbmax;
    m_mesh.calculateBoundingBox(bbmin, bbmax);

    double radius = 0.5 * (bbmin - bbmax).length();
    Vector3 center = 0.5 * (bbmin + bbmax);

    set_scene_pos(center, radius);
}

void ClothViewer::keyboard(int key, int x, int y)
{
    switch (key) {
        case 'h':
            printf("Help:\n");
            printf("'h'\t-\thelp\n");
            break;

        case ' ':
            if (isWatchOn) {
                watch.stop();
                currentTime = 0.0;
            } else {
                watch.start();
            }

            isWatchOn = !isWatchOn;
            break;

        default:
            TrackballViewer::special(key, x, y);
            break;
    }

    glutPostRedisplay();
}

void ClothViewer::draw_scene(DrawMode _draw_mode)
{
    drawCartoon();
    drawCloth();
}

void ClothViewer::drawCloth()
{
    m_cloth.draw();
}

void ClothViewer::drawCartoon()
{
    // clear screen
    glEnable(GL_DEPTH_TEST);
    glClearColor(1, 1, 1, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_cartoonShader.bind();

    // set parameters
    m_cartoonShader.setMatrix4x4Uniform("worldcamera", m_camera.getTransformation().Inverse());
    m_cartoonShader.setMatrix4x4Uniform("projection", m_camera.getProjectionMatrix());
    m_cartoonShader.setMatrix4x4Uniform("modelworld", m_mesh.getTransformation());

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);

    glVertexPointer(3, GL_DOUBLE, 0, m_mesh.getVertexPointer());
    glNormalPointer(GL_DOUBLE, 0, m_mesh.getNormalPointer());

    for (unsigned int i = 0; i < m_mesh.getNumberOfParts(); i++) {
        m_cartoonShader.setVector3Uniform("diffuseColor",
                                          m_mesh.getMaterial(i).m_diffuseColor.x,
                                          m_mesh.getMaterial(i).m_diffuseColor.y,
                                          m_mesh.getMaterial(i).m_diffuseColor.z);

        glDrawElements(GL_TRIANGLES, m_mesh.getNumberOfFaces(i) * 3, GL_UNSIGNED_INT, m_mesh.getVertexIndicesPointer(i));
    }

    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);

    m_cartoonShader.unbind();
}