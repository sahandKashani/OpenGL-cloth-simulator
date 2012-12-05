#include "Sphere.h"

// OpenGL imports
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>

Sphere::Sphere(Vector3 c, float r) :
    center(c),
    radius(r)
{}

Vector3 Sphere::getCenter()
{
    return center;
}

float Sphere::getRadius()
{
    return radius;
}

void Sphere::draw()
{
    // glColor3f(1.0, 1.0, 1.0);

    glPushMatrix();
        glTranslatef(center.x, center.y, center.z);
        glutSolidSphere(radius - 0.1, 20, 20);
    glPopMatrix();
}