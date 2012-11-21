#ifndef MESH_VIEWER_HH
#define MESH_VIEWER_HH

#include <string>
#include <vector>

#include "../gl/TrackballViewer.h"
#include "../gl/shader.h"
#include "../gl/Mesh3D.h"
#include "../gl/Light3D.h"
#include "../utils/StopWatch.h"
#include "Cloth.h"

class ClothViewer : public TrackballViewer
{

public:

    ClothViewer(const char* _title, int _width, int _height);
    void loadMesh(const std::string& filenameOBJ, const std::string& filenameMTL = std::string());

protected:

    virtual void init();
    virtual void keyboard(int key, int x, int y);
    virtual void reshape(int w, int h);
    virtual void idle();
    virtual void draw_scene(DrawMode _draw_mode);

private:

    void drawCloth();
    void drawCartoon();

protected:

    // mesh object
    Mesh3D m_mesh;

    // cartoon shader
    Shader m_cartoonShader;

    StopWatch watch;
    bool isWatchOn;
    float currentTime;

    Cloth m_cloth;
};

#endif // MESH_VIEWER_HH defined