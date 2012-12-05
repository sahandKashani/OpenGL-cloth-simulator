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

// TODO
// void Sphere::handleIntersection(Node* node)
// {
//     Vector3 centerToNode = node->getPosition() - center;
//     float centerToNodeLength = centerToNode.length();

//     if(centerToNodeLength < radius)
//     {
//         Vector3 translationToSurface = centerToNode.normalize() * (radius - centerToNodeLength);
//         node->translate(translationToSurface);
//     }
// }

void Sphere::draw()
{
    // glColor3f(1.0, 1.0, 1.0);

    glPushMatrix();
        glTranslatef(center.x, center.y, center.z);
        glutSolidSphere(radius - 0.1, 20, 20);
    glPopMatrix();
}