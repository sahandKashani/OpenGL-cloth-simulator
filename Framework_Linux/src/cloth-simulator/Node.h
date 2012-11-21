#ifndef NODE_HH
#define NODE_HH

#include "../math/Vector3.h"
#include "../gl/Object3D.h"

class Node : public Object3D
{

private:

    Vector3 position;
    Vector3 old_position;
    Vector3 velocity;
    Vector3 acceleration;
    Vector3 normal;
    bool movable;
    float mass;

public:

    Node();

    Node(Vector3 p_position, bool p_movable);

    void move(float duration);

    void translateByOffset(Vector3 offset);

    Vector3 getPosition();

    void draw();
};

#endif