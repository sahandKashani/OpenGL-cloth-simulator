#include "Floor.h"
#include "DrawingSettings.h"

// OpenGL imports
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>

// provide corners in counter-clockwise order, and normal will be facing upwards
Floor::Floor(Vector3 tl, Vector3 bl, Vector3 br, Vector3 tr) :
    topLeft(tl),
    bottomLeft(bl),
    bottomRight(br),
    topRight(tr),
    normal((topLeft - topRight).cross(bottomRight - topRight))
{}

void Floor::draw()
{
    DrawingSettings* drawingSettings = DrawingSettings::getInstance();

    if(drawingSettings->isDrawFloorEnabled())
    {
        glPushAttrib(GL_POLYGON_BIT); // save mesh settings
            glPushAttrib(GL_CURRENT_BIT); // save color

                Vector3 color = drawingSettings->getFloorColor();
                glColor3f(color.x, color.y, color.z);
                glBegin(GL_QUADS);
                    glVertex3f(topLeft.x, topLeft.y, topLeft.z);
                    glVertex3f(topRight.x, topRight.y, topRight.z);
                    glVertex3f(bottomRight.x, bottomRight.y, bottomRight.z);
                    glVertex3f(bottomLeft.x, bottomLeft.y, bottomLeft.z);
                glEnd();

            glPopAttrib(); // GL_CURRENT_BIT
        glPopAttrib(); // GL_POLYGON_BIT
    }
}

void Floor::handleNodeIntersection(Node* node)
{

}