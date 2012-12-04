#include "Node.h"

// OpenGL imports
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>

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
        Vector3 acceleration = force / mass;
        Vector3 temp = position;
        position = position + (position - oldPosition) + acceleration * duration;
        oldPosition = temp;
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