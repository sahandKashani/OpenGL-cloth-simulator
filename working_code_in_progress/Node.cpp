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

void Node::applyForces(float duration)
{
    if(moveable)
    {
        // TODO : integration
        // Vector3 acceleration = force / mass;
        // Vector3 temp = position;
        // position = position + (position - oldPosition) + acceleration * duration;
        // oldPosition = temp;

        ClothSimulator* clothSimulator = ClothSimulator::getInstance();

        Vector3 acceleration = force / mass;
        Vector3 newPosition = position + (position - oldPosition) + acceleration * duration;
        Vector3 direction = newPosition - oldPosition;

        for(std::vector<Sphere>::iterator sphereIterator = clothSimulator->spheres.begin();
            sphereIterator != clothSimulator->spheres.end();
            ++sphereIterator)
        {
            std::cout << sphereIterator->getCenter().toString() << std::endl;
        }

        // Resolving the problem algebraically, we get the following coefficients of a quadratic equation in t:
        // double a=ray.direction.dot(ray.direction);
        // double b=2*(ray.point.dot(ray.direction) - m_center.dot(ray.direction));
        // double c=m_center.dot(m_center)+ray.point.dot(ray.point) - 2.0*m_center.dot(ray.point) - m_radius*m_radius;

        // //Check if there is a solution:
        // double discriminant = b*b - 4.0*a*c;
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