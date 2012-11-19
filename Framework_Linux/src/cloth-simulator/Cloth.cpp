#include<vector>
#include "MassElement.cpp"
#include "SpringConstraint.cpp"

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
