//=============================================================================
//
//   Exercise code for the lecture
//
//=============================================================================
//=============================================================================
//
//  CLASS MeshViewer
//
//=============================================================================


#ifndef MESH_VIEWER_HH
#define MESH_VIEWER_HH


//== INCLUDES =================================================================
#include <string>
#include <vector>

#include "../gl/TrackballViewer.h"
#include "../gl/shader.h"
#include "../gl/Mesh3D.h"
#include "../gl/Light3D.h"
#include "../gl/texture.h"
#include "../gl/fbo.h"
#include "../utils/StopWatch.h"

//== CLASS DEFINITION =========================================================



/*
 ClothViewer.
*/
class ClothViewer : public TrackballViewer {
    public:

        ClothViewer(const char* _title, int _width, int _height);


        void loadMesh(const std::string& filenameOBJ, const std::string& filenameMTL = std::string());

    protected:

        // overloaded GUI function
        virtual void init();
        virtual void keyboard(int key, int x, int y);
        virtual void reshape(int w, int h);
        virtual void idle();
        virtual void draw_scene(DrawMode _draw_mode);

    private:
        void drawCartoon();
        void drawDepth();
        void drawEdge();
        void blendCartoonAndEdge();
        void drawBall();
        void renderFullScreenQuad();

    protected:

        // frame buffer object for render2texture
        FrameBufferObject m_fbo;

        // mesh object
        Mesh3D m_mesh;

        // cartoon shader
        Shader m_cartoonShader;

        // depth shader
        Shader m_depthShader;

        // edge shader
        Shader m_edgeShader;

        // blending shader
        Shader m_blendingShader;

        // cartoon shading texture
        Texture m_cartoonShadingTexture;

        // cartoon output texture
        Texture m_cartoonOutputTexture;

        // depth texture
        Texture m_depthTexture;

        // edge texture
        Texture m_edgeTexture;

        StopWatch watch;
        bool isWatchOn;
        float currentTime;


};


//=============================================================================
#endif // MESH_VIEWER_HH defined
//=============================================================================
