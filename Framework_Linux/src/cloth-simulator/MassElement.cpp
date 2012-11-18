#include "../math/Vector3.h"

#define TIME_INCREMENT 0.1
#define AIR_DAMPENING 0.01

class MassElement {

private:
    Vector3 position;
    Vector3 oldPosition;
    Vector3 velocity;
    Vector3 acceleration;
    Vector3 normal;
    bool canMove;
    float mass;

public:
    MassElement(Vector3 p_position, bool p_canMove, float p_mass) :
        position(p_position),
        oldPosition(p_position),
        velocity(Vector3(0, 0, 0)),
        acceleration(Vector3(0, 0, 0)),
        normal(Vector3(0, 0, 0)),
        canMove(p_canMove),
        mass(p_mass)
    {}

    void move() {
        if (canMove) {
            Vector3 temporaryOldPosition = position;
            position = position + (position - oldPosition) * (1.0 - AIR_DAMPENING) + acceleration * TIME_INCREMENT;
            oldPosition = temporaryOldPosition;
            acceleration = Vector3(0, 0, 0);
        }
    }

    void translateByOffset(Vector3 offset) {
        if (canMove) {
            position = position + offset;
        }
    }
};