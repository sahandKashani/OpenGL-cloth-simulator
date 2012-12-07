#include "Triangle.h"
#include "Arrow.h"

// OpenGL imports
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>

void Triangle::draw()
{
    // set color to white
    glColor3f(1.0, 0.0, 1.0);

    glBegin(GL_TRIANGLES);
        glVertex3f(p1.x, p1.y, p1.z);
        glVertex3f(p2.x, p2.y, p2.z);
        glVertex3f(p3.x, p3.y, p3.z);
    glEnd();

    // draw normal vector
    Vector3 center = (p1 + p2 + p3) / 3.0;
    Arrow normalVector(center, center + normal);
    normalVector.draw();
}

Triangle::Triangle(Vector3 point1, Vector3 point2, Vector3 point3) :
    p1(point1),
    p2(point2),
    p3(point3),
    normal((p2 - p1).cross(p3 - p1)),
    area(normal.length() * 0.5)
{}