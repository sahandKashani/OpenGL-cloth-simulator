#include "Sphere.h"

// OpenGL imports
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>

Sphere::Sphere(Vector3 c, float r) :
    center(c),
    radius(r),
    sticky(false)
{}

Sphere::Sphere(Vector3 c, float r, bool stick) :
    center(c),
    radius(r),
    sticky(stick)
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

        // draw the sphere a little smaller for collision problems
        glutSolidSphere(radius - 0.1, 20, 20);
    glPopMatrix();
}

bool Sphere::willHitSphere(Node* node)
{
    return (node->getPosition() - center).length() < radius;
}

void Sphere::handleNodeIntersection(Node* node)
{
    if(willHitSphere(node))
    {
        Vector3 currentPositionToCenter = node->getPosition() - center;
        float length = currentPositionToCenter.length();

        node->translate(currentPositionToCenter.normalize() * (radius - length));

        Vector3 t1Pos = node->getPosition();
        Vector3 sphereNormalNormalized = (-1) * (t1Pos - center).normalize();
        Vector3 normalForceDirectionNormalized = node->getForce().dot(sphereNormalNormalized) * sphereNormalNormalized;
        Vector3 tangentForceDirectionNormalized = node->getForce() - normalForceDirectionNormalized;
        Vector3 tangentForce = tangentForceDirectionNormalized;

        // only keep the tangent force now, since the normal force is absorbed by the sphere
        node->setForce(tangentForce);

        if(sticky)
        {
            // sticky sphere
            node->setMoveable(false);
        }
    }
    else
    {
        // no longer in collision with the sphere, so put the original force back
        node->resetToOriginalForce();
    }
}