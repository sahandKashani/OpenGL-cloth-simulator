#include "Node.h"

// OpenGL imports
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "ClothSimulator.h"
#include "Arrow.h"
#include "DrawingSettings.h"
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
        // verlet integration
        Vector3 acceleration = force / mass;
        Vector3 temp = position;
        position = position + (position - oldPosition) + acceleration * duration;
        oldPosition = temp;

        // TODO : Runge-Kutta 4 intergration (RK4)
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

Vector3 Node::getOldPosition()
{
    return oldPosition;
}

void Node::resetToOriginalForce()
{
    force = originalForce;
}

void Node::draw()
{
    DrawingSettings* drawingSettings = DrawingSettings::getInstance();

    if(drawingSettings->isDrawNodesEnabled())
    {
        glPushAttrib(GL_POLYGON_BIT); // save mesh settings
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            glPushAttrib(GL_CURRENT_BIT); // save color

                Vector3 color = drawingSettings->getNodesColor();
                glColor3f(color.x, color.y, color.z);
                // push matrix so we can come back to the "origin" (on element (0.0, 0.0, 0.0))
                // for each node to draw.
                glPushMatrix();
                    glTranslatef(position.x, position.y, position.z);
                    glutSolidSphere(0.1, 10, 10);
                glPopMatrix();
                // back at "origin" (on element (0.0, 0.0, 0.0)) again.

            glPopAttrib(); // GL_CURRENT_BIT
        glPopAttrib(); // GL_POLYGON_BIT
    }

    // force applied to the node
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

void Node::handleNodeIntersection(Node* node)
{

}