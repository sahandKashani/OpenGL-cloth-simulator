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

        // draw the sphere a little smaller for collision problems
        glutSolidSphere(radius - 0.1, 20, 20);
    glPopMatrix();
}

void Sphere::handleNodeIntersection(Node* node)
{
    Vector3 currentPositionToCenter = node->getPosition() - center;
    float length = currentPositionToCenter.length();

    if(length < radius)
    {
        node->translate(currentPositionToCenter.normalize() * (radius - length));

        Vector3 t1Pos = node->getPosition();
        Vector3 sphereNormalNormalized = (-1) * (t1Pos - center).normalize();
        Vector3 normalForceDirectionNormalized = node->getForce().dot(sphereNormalNormalized) * sphereNormalNormalized;
        Vector3 tangentForceDirectionNormalized = node->getForce() - normalForceDirectionNormalized;
        Vector3 tangentForce = tangentForceDirectionNormalized;

        node->setForce(tangentForce);
    }

//         Vector3 sphereCenter = sphereIterator->getCenter();
//         float sphereRadius = sphereIterator->getRadius();

//         float a = direction.dot(direction);
//         float b = 2 * (oldPosition.dot(direction) - sphereCenter.dot(direction));
//         float c = sphereCenter.dot(sphereCenter) + oldPosition.dot(oldPosition) - (2 * sphereCenter.dot(oldPosition)) - (sphereRadius * sphereRadius);

//         float discriminant = (b * b) - (4 * a * c);

//         // no intersection with a sphere
//         if(discriminant <= 0)
//         {
//             oldPosition = position;
//             position = newPosition;
//         }
//         else if (discriminant > 0)
//         {
//             float t1 = (-b - sqrt(discriminant)) / (2 * a);
//             float t2 = (-b + sqrt(discriminant)) / (2 * a);

//             // oldPosition + t * direction = newPosition;
//             // t * direction = newPosition - oldPosition;
//             // t * direction.x = newPosition.x - oldPosition.x;
//             // t = (newPosition.x - oldPosition.x) / direction.x;
//             float tNewPosition = (newPosition.x - oldPosition.x) / direction.x;

//             if(tNewPosition < t1)
//             {
//                 // trajectory of node will eventually hit the sphere, but
//                 // will not reach it in this iteration, so we can move it.
//                 oldPosition = position;
//                 position = newPosition;
//             }
//             else
//             {
//                 Vector3 t1Pos = oldPosition + t1 * direction;
//                 Vector3 sphereNormalNormalized = (-1) * (t1Pos - sphereCenter).normalize();
//                 Vector3 normalForceDirectionNormalized = force.dot(sphereNormalNormalized) * sphereNormalNormalized;
//                 Vector3 tangentForceDirectionNormalized = force - normalForceDirectionNormalized;

//                 Vector3 tangentForce = tangentForceDirectionNormalized;

//                 std::cout << "tangentForce = " << tangentForce.toString() << std::endl;
//                 force = tangentForce;
//                 position = t1Pos;
//             }
//         }
}