#include "Node.h"

// OpenGL imports
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "ClothSimulator.h"
#include "Arrow.h"
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

Vector3 Node::getForce()
{
    return force;
}

void Node::setPosition(Vector3 pos)
{
    position = pos;
}

void Node::setMass(float m)
{
    mass = m;
}

void Node::setForce(Vector3 f)
{
    force = f;
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
    originalForce = force;
}

Vector3 Node::getPosition()
{
    return position;
}

void Node::resetToOriginalForce()
{
    force = originalForce;
}

void Node::draw()
{
    glColor3f(1.0, 1.0, 1.0);

    // push matrix so we can come back to the "origin" (on element (0.0, 0.0, 0.0))
    // for each node to draw.
    glPushMatrix();
        glTranslatef(position.x, position.y, position.z);
        glutSolidSphere(0.15, 10, 10);
    glPopMatrix();
    // back at "origin" (on element (0.0, 0.0, 0.0)) again.

    Arrow appliedForce(position, position + force);
    appliedForce.draw();
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