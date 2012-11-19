#include "../math/Vector3.h"

class SpringConstraint {

private:
    float distanceAtRest;
    MassElement* m1;
    MassElement* m2;

public:
    SpringConstraint(MassElement p_m1, MassElement p_m2) :
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
}