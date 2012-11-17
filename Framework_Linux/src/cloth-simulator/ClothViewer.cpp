//=============================================================================
// //   Exercise code for the lecture //
//=============================================================================
//=============================================================================
// //  CLASS ClothViewer - IMPLEMENTATION //
//=============================================================================

//== INCLUDES =================================================================

#include "ClothViewer.h"
#include "../utils/Mesh3DReader.h"

//== IMPLEMENTATION ==========================================================


ClothViewer::
ClothViewer(const char* _title, int _width, int _height)
    : TrackballViewer(_title, _width, _height) {
    init();
}


//-----------------------------------------------------------------------------


void
ClothViewer::
init() {
    // initialize parent
    TrackballViewer::init();

    // set camera to look at world coordinate center
    set_scene_pos(Vector3(0.0, 0.0, 0.0), 2.0);

    // load shaders
    m_cartoonShader.create("cartoon.vs", "cartoon.fs");
    m_depthShader.create("depth.vs", "depth.fs");
    m_edgeShader.create("edge.vs", "edge.fs");
    m_blendingShader.create("blending.vs", "blending.fs");

    // setup 1D color texture with 4 colors
    float  tex[12] = {
        0.2, 0.2, 0.2,
        0.4, 0.4, 0.4,
        0.6, 0.6, 0.6,
        0.8, 0.8, 0.8
    };

    m_cartoonShadingTexture.create(4, 1, GL_RGB, GL_RGB, GL_FLOAT, tex, GL_NEAREST);
}



//-----------------------------------------------------------------------------


void
ClothViewer::
reshape(int _w, int _h) {
    TrackballViewer::reshape(_w, _h);

    // resize framebuffer and textures
    m_fbo.create(_w, _h, true);

    m_cartoonOutputTexture.create(_w, _h, GL_RGB, GL_RGB, GL_UNSIGNED_BYTE);
    // try GL_RGB4, GL_RGB8, GL_RGB10, GL_RGB16 to see effect of edge precision
    m_depthTexture.create(_w, _h, GL_RGB16, GL_RGB, GL_UNSIGNED_BYTE);
    m_edgeTexture.create(_w, _h, GL_RGB, GL_RGB, GL_UNSIGNED_BYTE);

    // attach textures to frame buffer
    m_fbo.attachTexture(GL_COLOR_ATTACHMENT0_EXT, m_cartoonOutputTexture.getID());
    m_fbo.attachTexture(GL_COLOR_ATTACHMENT1_EXT, m_depthTexture.getID());
    m_fbo.attachTexture(GL_COLOR_ATTACHMENT2_EXT, m_edgeTexture.getID());

}


//-----------------------------------------------------------------------------

void
ClothViewer::
loadMesh(const std::string& filenameOBJ, const std::string& filenameMTL) {
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



//-----------------------------------------------------------------------------


void
ClothViewer::
keyboard(int key, int x, int y) {
    switch (key) {
        case 'h':
            printf("Help:\n");
            printf("'h'\t-\thelp\n");
            break;

        default:
            TrackballViewer::special(key, x, y);
            break;
    }

    glutPostRedisplay();
}

//-----------------------------------------------------------------------------


void
ClothViewer::
draw_scene(DrawMode _draw_mode) {
    // draw cartoon shading
    m_fbo.bind(GL_COLOR_ATTACHMENT0_EXT);
    drawCartoon();
    m_fbo.unbind();

    // draw depth image
    m_fbo.bind(GL_COLOR_ATTACHMENT1_EXT);
    drawDepth();
    m_fbo.unbind();

    // calculate edges on depth image
    m_fbo.bind(GL_COLOR_ATTACHMENT2_EXT);
    drawEdge();
    m_fbo.unbind();

    // blend edges and cartoon shading
    blendCartoonAndEdge();

}


//-----------------------------------------------------------------------------
void
ClothViewer::
drawCartoon() {

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

    if (m_mesh.hasUvTextureCoord()) {
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    }

    m_cartoonShadingTexture.setLayer(0);
    m_cartoonShadingTexture.bind();
    m_cartoonShader.setIntUniform("texture", m_cartoonShadingTexture.getLayer());

    glVertexPointer(3, GL_DOUBLE, 0, m_mesh.getVertexPointer());
    glNormalPointer(GL_DOUBLE, 0, m_mesh.getNormalPointer());

    if (m_mesh.hasUvTextureCoord()) {
        glTexCoordPointer(2, GL_DOUBLE, 0, m_mesh.getUvTextureCoordPointer());
    }

    for (unsigned int i = 0; i < m_mesh.getNumberOfParts(); i++) {
        bool hasTexture = m_mesh.hasUvTextureCoord() && m_mesh.getMaterial(i).hasDiffuseTexture();
        m_cartoonShader.setIntUniform("useTexture", hasTexture);
        m_cartoonShader.setVector3Uniform("diffuseColor",
                                          m_mesh.getMaterial(i).m_diffuseColor.x,
                                          m_mesh.getMaterial(i).m_diffuseColor.y,
                                          m_mesh.getMaterial(i).m_diffuseColor.z);

        if (hasTexture) {
            m_mesh.getMaterial(i).m_diffuseTexture.bind();
            m_cartoonShader.setIntUniform("textureDiffuse", m_mesh.getMaterial(i).m_diffuseTexture.getLayer());
        }

        glDrawElements(GL_TRIANGLES, m_mesh.getNumberOfFaces(i) * 3, GL_UNSIGNED_INT, m_mesh.getVertexIndicesPointer(i));

        if (hasTexture) {
            m_mesh.getMaterial(i).m_diffuseTexture.unbind();
        }
    }

    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);

    if (m_mesh.hasUvTextureCoord()) {
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    }

    m_cartoonShadingTexture.unbind();
    m_cartoonShader.unbind();
}

//-----------------------------------------------------------------------------
void
ClothViewer::
drawDepth() {

    // clear screen
    glEnable(GL_DEPTH_TEST);
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_depthShader.bind();

    // set parameters
    m_depthShader.setMatrix4x4Uniform("worldcamera", m_camera.getTransformation().Inverse());
    m_depthShader.setMatrix4x4Uniform("projection", m_camera.getProjectionMatrix());
    m_depthShader.setMatrix4x4Uniform("modelworld", m_mesh.getTransformation());
    m_depthShader.setFloatUniform("near", m_camera.getNearPlane());
    m_depthShader.setFloatUniform("far", m_camera.getFarPlane());

    glEnableClientState(GL_VERTEX_ARRAY);

    glVertexPointer(3, GL_DOUBLE, 0, m_mesh.getVertexPointer());

    for (unsigned int i = 0; i < m_mesh.getNumberOfParts(); i++) {
        glDrawElements(GL_TRIANGLES, m_mesh.getNumberOfFaces(i) * 3, GL_UNSIGNED_INT, m_mesh.getVertexIndicesPointer(i));
    }

    glDisableClientState(GL_VERTEX_ARRAY);

    m_depthShader.unbind();

}


//-----------------------------------------------------------------------------

void
ClothViewer::
renderFullScreenQuad() {
    // render full screen quad (note that vertex coordinates are already in opengl coordinates, so no transformation required!)
    glBegin(GL_QUADS);
    glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 0.0f, 0.0f);
    glVertex2f(-1.0f, -1.0f);
    glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 1.0f, 0.0f);
    glVertex2f(1.0f, -1.0f);
    glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 1.0f, 1.0f);
    glVertex2f(1.0f, 1.0f);
    glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 0.0f, 1.0f);
    glVertex2f(-1.0f, 1.0f);
    glEnd();
}


//-----------------------------------------------------------------------------
void
ClothViewer::
drawEdge() {

    // clear screen
    glDisable(GL_DEPTH_TEST);

    m_edgeShader.bind();
    m_depthTexture.setLayer(0);
    m_depthTexture.bind();
    m_edgeShader.setIntUniform("texture", m_depthTexture.getLayer());
    m_edgeShader.setFloatUniform("dx", 1.0 / width_);
    m_edgeShader.setFloatUniform("dy", 1.0 / height_);

    // render a quad over full image
    renderFullScreenQuad();

    m_edgeShader.unbind();

}

//-----------------------------------------------------------------------------
void
ClothViewer::
blendCartoonAndEdge() {

    // clear screen
    glDisable(GL_DEPTH_TEST);

    m_blendingShader.bind();
    m_cartoonOutputTexture.setLayer(0);
    m_cartoonOutputTexture.bind();
    m_blendingShader.setIntUniform("texture1", m_cartoonOutputTexture.getLayer());
    m_edgeTexture.setLayer(1);
    m_edgeTexture.bind();
    m_blendingShader.setIntUniform("texture2", m_edgeTexture.getLayer());


    // render a quad over full image
    renderFullScreenQuad();

    m_blendingShader.unbind();
}


//=============================================================================
