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
    if(DrawingSettings::getInstance()->isDrawFloorEnabled())
    {
        glColor3f(1.0, 1.0, 1.0);
        glBegin(GL_QUADS);
            glVertex3f(topLeft.x, topLeft.y, topLeft.z);
            glVertex3f(topRight.x, topRight.y, topRight.z);
            glVertex3f(bottomRight.x, bottomRight.y, bottomRight.z);
            glVertex3f(bottomLeft.x, bottomLeft.y, bottomLeft.z);
        glEnd();
    }
}

void Floor::handleNodeIntersection(Node* node)
{

}