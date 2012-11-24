#ifndef CLOTH_HH
#define CLOTH_HH

#include <vector>
#include "Node.h"
#include "SpringConstraint.h"

class Cloth {

private:

    int cloth_width;
    int cloth_height;
    int number_of_nodes_on_width;
    int number_of_nodes_on_height;

    std::vector<Node> nodes;
    std::vector<SpringConstraint> springConstraints;

    Node* getNode(int x, int y);

    void makeSpringConstraint(Node* node_1, Node* node_2);

    void createStructuralSpringConstraints();

    void createShearSpringConstraints();

    void createBendingSpringConstraints();

    void addNodes();

public:

    Cloth();

    Cloth(float p_cloth_width, float p_cloth_height, int p_number_of_nodes_on_width, int p_number_of_nodes_on_height);

    void draw();

};

#endif