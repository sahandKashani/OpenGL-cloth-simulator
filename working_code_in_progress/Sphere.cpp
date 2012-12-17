#include "Sphere.h"
#include "DrawingSettings.h"

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

void Sphere::setCenter(Vector3 c)
{
    center = c;
}

void Sphere::translate(Vector3 direction)
{
    center += direction;
}

void Sphere::draw()
{
    DrawingSettings* drawingSettings = DrawingSettings::getInstance();

    if(drawingSettings->isDrawSpheresEnabled())
    {
        glPushAttrib(GL_POLYGON_BIT); // save mesh settings
            glPushAttrib(GL_CURRENT_BIT); // save color
                glPushAttrib(GL_LIGHTING_BIT);

                    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

                    glEnable(GL_LIGHTING);
                    glEnable(GL_LIGHT0);

                    Vector3 color = drawingSettings->getSphereColor();

                    GLfloat mat_amb_diff[] = {color.x, color.y, color.z, 1.0};
                    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, mat_amb_diff);

                    glPushMatrix();
                        glTranslatef(center.x, center.y, center.z);

                        // draw the sphere a little smaller for collision problems
                        glutSolidSphere(radius - 0.2, 20, 20);
                    glPopMatrix();

                glPopAttrib(); // GL_LIGHTING_BIT
            glPopAttrib(); // GL_CURRENT_BIT
        glPopAttrib(); // GL_POLYGON_BIT
    }
}

bool Sphere::willHitSphere(Node* node)
{
    return (node->getPosition() - center).length() < radius;
}

void Sphere::handleNodeIntersection(Node* node, bool isClothSelfIntersectionSphere)
{
    if(willHitSphere(node))
    {
        Vector3 currentPositionToCenter = node->getPosition() - center;
        float length = currentPositionToCenter.length();

        node->translate(currentPositionToCenter.normalize() * (radius - length));

        if(!isClothSelfIntersectionSphere)
        {
            // decompose forces applied to the node into the tangent force
            Vector3 t1Pos = node->getPosition();
            Vector3 sphereNormalNormalized = (-1) * (t1Pos - center).normalize();
            Vector3 normalForceDirectionNormalized = node->getForce().dot(sphereNormalNormalized) * sphereNormalNormalized;
            Vector3 tangentForceDirectionNormalized = node->getForce() - normalForceDirectionNormalized;
            Vector3 tangentForce = tangentForceDirectionNormalized;

            // only keep the tangent force now, since the normal force is absorbed by the sphere
            node->setForce(tangentForce);
        }
    }
    else
    {
        if(!isClothSelfIntersectionSphere)
        {
            // no longer in collision with the sphere, so put the original force back
            node->resetToOriginalForce();
        }
    }
}