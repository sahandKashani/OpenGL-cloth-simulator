#include<vector>
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

    Vector3 getPosition() {
        return Vector3(position);
    }
};

class SpringConstraint {

private:
    float distanceAtRest;
    MassElement* m1;
    MassElement* m2;

public:
    SpringConstraint(MassElement* p_m1, MassElement* p_m2) :
        m1(p_m1), m2(p_m2)
    {
        Vector3 vectorBetweenM1AndM2 = m2->getPosition() - m1->getPosition();
        float currentDistanceBetweenM1AndM2 = vectorBetweenM1AndM2.length();
        float sizeRatioDifference = distanceAtRest / currentDistanceBetweenM1AndM2;
        Vector3 totalCorrectionVector = vectorBetweenM1AndM2 * (1.0 - sizeRatioDifference);
        Vector3 correctionVectorPerMassElement = totalCorrectionVector / 2.0;

        m1->translateByOffset(correctionVectorPerMassElement);
        m2->translateByOffset(-correctionVectorPerMassElement);
    }
};

class Cloth {
private:
    int numberOfMassElementsOnWidth;
    int numberOfMassElementsOnHeight;

    std::vector<MassElement> massElements;
    std::vector<SpringConstraint> springConstraints;

public:
    Cloth(float p_width, float p_height, int p_numberOfMassElementsOnWidth, int p_numberOfMassElementsOnHeight) :
        numberOfMassElementsOnWidth(p_numberOfMassElementsOnWidth),
        numberOfMassElementsOnHeight(p_numberOfMassElementsOnHeight)
    {
//
//        int totalNumberOfMassElements = numberOfMassElementsOnWidth * numberOfMassElementsOnHeight;
//        massElements.resize(totalNumberOfMassElements);
//
//        for(int x = 0; x < numberOfMassElementsOnWidth; x += 1) {
//            for(int y = 0; y < numberOfMassElementsOnHeight; y += 1) {
//
//                float x_position = ((float) x) / ((float) numberOfMassElementsOnWidth);
//                float y_position = ((float) y) / ((float) numberOfMassElementsOnHeight);
//                Vector3 massElementPosition = Vector3(x_position, y_position, 0);
//
//                massElements[y * numberOfMassElementsOnWidth + x] = MassElement();
//            }
//        }
    }
};
