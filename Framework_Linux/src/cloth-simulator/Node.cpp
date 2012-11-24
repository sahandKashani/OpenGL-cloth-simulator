#include "Node.h"
#include "../math/Vector3.h"
#include "../gl/gl.h"

#define AIR_DAMPENING 0.01
#define MASS 1.0

Node::Node()
{}

Node::Node(Vector3 p_position, bool p_movable) :
    position(p_position),
    old_position(p_position),
    velocity(Vector3(0, 0, 0)),
    acceleration(Vector3(0, 0, 0)),
    normal(Vector3(0, 0, 0)),
    movable(p_movable),
    mass(MASS)
{}

void Node::move(float duration)
{
    if (movable) {
        Vector3 temp_old_position = position;
        position = position + (position - old_position) * (1.0 - AIR_DAMPENING) + acceleration * duration;
        old_position = temp_old_position;
        acceleration = Vector3(0, 0, 0);
    }
}

void Node::translateByOffset(Vector3 offset)
{
    if (movable) {
        position += offset;
    }
}

Vector3 Node::getPosition()
{
    return position;
}

void Node::draw()
{
    Vector3 positionWorld = m_transformationMatrix * position;

    // position.print("positionObject");
    positionWorld.print("positionWorld");

    glColor3f(1.0, 0.0, 0.0);
    glPushMatrix();
        glTranslated(positionWorld.x, positionWorld.y, positionWorld.y);
        glutSolidSphere(1,10,10);
    glPopMatrix();

}