#include "Node.h"

// OpenGL imports
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "ClothSimulator.h"
#include <vector>

Node::Node() :
    position(Vector3(0.0, 0.0, 0.0)),
    oldPosition(Vector3(0.0, 0.0, 0.0)),
    moveable(true),
    force(Vector3(0.0, 0.0, 0.0)),
    mass(1.0)
{}

Node::Node(Vector3 pos) :
    position(pos),
    oldPosition(pos),
    moveable(true),
    force(Vector3(0.0, 0.0, 0.0)),
    mass(1.0)
{}

void Node::setPosition(Vector3 pos)
{
    position = pos;
}

void Node::setMass(float m)
{
    mass = m;
}

void Node::intersectWithSpheres(Vector3 newPosition)
{
    ClothSimulator* clothSimulator = ClothSimulator::getInstance();
    Vector3 direction = (newPosition - oldPosition).normalize();

    for(std::vector<Sphere>::iterator sphereIterator = clothSimulator->spheres.begin();
        sphereIterator != clothSimulator->spheres.end();
        ++sphereIterator)
    {
        Vector3 sphereCenter = sphereIterator->getCenter();
        float sphereRadius = sphereIterator->getRadius();

        float a = direction.dot(direction);
        float b = 2 * (oldPosition.dot(direction) - sphereCenter.dot(direction));
        float c = sphereCenter.dot(sphereCenter) + oldPosition.dot(oldPosition) - (2 * sphereCenter.dot(oldPosition)) - (sphereRadius * sphereRadius);

        float discriminant = (b * b) - (4 * a * c);

        // no intersection with a sphere
        if(discriminant < 0)
        {
            Vector3 temp = position;
            position = newPosition;
            oldPosition = temp;
        }
        else if (discriminant > 0)
        {
            float t1 = (-b - sqrt(discriminant)) / (2 * a);
            float t2 = (-b + sqrt(discriminant)) / (2 * a);

            // oldPosition + t * direction = newPosition;
            // t * direction = newPosition - oldPosition;
            // t * direction.x = newPosition.x - oldPosition.x;
            // t = (newPosition.x - oldPosition.x) / direction.x;
            float tNewPosition = (newPosition.x - oldPosition.x) / direction.x;

            if(tNewPosition < t1)
            {
                // trajectory of node will eventually hit the sphere, but
                // will not reach it in this iteration, so we can move it.
                oldPosition = position;
                position = newPosition;
            }
            else if(t1 < tNewPosition && t2 < tNewPosition)
            {
                // will enter the sphere, so have to find it's appropriate
                // position on the surface
                // std::cout << "in between somewhere" << std::endl;
                // TODO
                // Vector3 temp = position;
                // position = oldPosition + t1 * direction;
                // oldPosition = temp;
                force = 0;
            }
            else if(t2 < tNewPosition)
            {
                // will completely pass the sphere, so have to find it's
                // appropriate position on the surface
                // std::cout << "got through somewhere" << std::endl;
                moveable = false;
            }
        }
        else
        {
            // node future position will be at the surface of the sphere,
            // so there is nothing to do.
        }
    }
}

void Node::intersectWithTriangles(Vector3 newPosition)
{
    ClothSimulator* clothSimulator = ClothSimulator::getInstance();
    Vector3 direction = (newPosition - oldPosition).normalize();

    for(std::vector<Triangle>::iterator triangleIterator = clothSimulator->triangles.begin();
        triangleIterator != clothSimulator->triangles.end();
        ++triangleIterator)
    {

    }
}

void Node::applyForces(float duration)
{
    if(moveable)
    {
        // TODO : integration
        Vector3 acceleration = force / mass;
        Vector3 temp = position;
        position = position + (position - oldPosition) + acceleration * duration;
        oldPosition = temp;

        // intersectWithSpheres(newPosition);
    }
}

void Node::addForce(Vector3 extraForce)
{
    force += extraForce;
}

Vector3 Node::getPosition()
{
    return position;
}

void Node::draw()
{
    // push matrix so we can come back to the "origin" (on element (0.0, 0.0, 0.0))
    // for each node to draw.
    glPushMatrix();
        glTranslatef(position.x, position.y, position.z);

        glutSolidSphere(0.15, 10, 10);
    glPopMatrix();
    // back at "origin" (on element (0.0, 0.0, 0.0)) again.
}

bool Node::isMoveable()
{
    return moveable;
}

void Node::setMoveable(bool isMovePossible)
{
    moveable = isMovePossible;
}

void Node::translate(Vector3 direction)
{
    position += direction;
}