#include "Arrow.h"

// OpenGL imports
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>

Arrow::Arrow(Vector3 basePoint, Vector3 endPoint) :
    base(basePoint),
    end(endPoint)
{}

void Arrow::draw()
{
    glColor3f(0.0, 1.0, 0.0);
    glBegin(GL_LINES);
        glVertex3f(base.x, base.y, base.z);
        glVertex3f(end.x, end.y, end.z);
    glEnd();

    glPushMatrix();
        glTranslatef(end.x, end.y, end.z);
        glutSolidSphere(0.1, 10, 10);
    glPopMatrix();
}