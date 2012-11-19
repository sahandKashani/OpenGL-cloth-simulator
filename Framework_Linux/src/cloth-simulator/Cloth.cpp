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
    {}
}