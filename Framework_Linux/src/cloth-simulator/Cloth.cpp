#include<vector>

#include "../math/Vector3.h"

#define AIR_DAMPENING 0.01
#define MASS 1.01

class Node {

private:

    Vector3 position;
    Vector3 old_position;
    Vector3 velocity;
    Vector3 acceleration;
    Vector3 normal;
    bool movable;
    float mass;

public:

    Node()
    {}

    Node(Vector3 p_position, bool p_movable, float p_mass) :
        position(p_position),
        old_position(p_position),
        velocity(Vector3(0, 0, 0)),
        acceleration(Vector3(0, 0, 0)),
        normal(Vector3(0, 0, 0)),
        movable(p_movable),
        mass(p_mass)
    {}

    void move(float duration)
    {
        if (movable) {
            Vector3 temp_old_position = position;
            position = position + (position - old_position) * (1.0 - AIR_DAMPENING) + acceleration * duration;
            old_position = temp_old_position;
            acceleration = Vector3(0, 0, 0);
        }
    }

    void translateByOffset(Vector3 offset)
    {
        if (movable) {
            position += offset;
        }
    }

    Vector3 getPosition()
    {
        return position;
    }
};

class SpringConstraint {

private:

    float distance_at_rest;
    Node* node_1;
    Node* node_2;

public:

    SpringConstraint(Node* n1, Node* n2) :
        node_1(n1), node_2(n2)
    {
        Vector3 vector_between_nodes = node_2->getPosition() - node_1->getPosition();
        float current_distance_between_nodes = vector_between_nodes.length();
        float distance_ratio_difference = distance_at_rest / current_distance_between_nodes;
        Vector3 total_correction_vector = vector_between_nodes * (1.0 - distance_ratio_difference);
        Vector3 correction_per_node_vector = total_correction_vector / 2.0;

        node_1->translateByOffset(correction_per_node_vector);
        node_2->translateByOffset(-1 * correction_per_node_vector);
    }
};

class Cloth {

private:

    int cloth_width;
    int cloth_height;
    int number_of_nodes_on_width;
    int number_of_nodes_on_height;

    std::vector<Node> nodes;
    std::vector<SpringConstraint> springConstraints;

    Node* getNode(int x, int y)
    {
        return &nodes[y * number_of_nodes_on_width + x];
    }

    void makeSpringConstraint(Node* node_1, Node* node_2)
    {
        springConstraints.push_back(SpringConstraint(node_1, node_2));
    }

    void createStructuralSpringConstraints()
    {
        for(int x = 0; x < (number_of_nodes_on_width - 1); x += 1) {
            for(int y = 0; y < (number_of_nodes_on_height - 1); y += 1) {

                Node* centerNode = getNode(x, y);
                makeSpringConstraint(centerNode, getNode(x + 1, y));
                makeSpringConstraint(centerNode, getNode(x, y + 1));
            }
        }
    }

    void createShearSpringConstraints()
    {
        for(int x = 0; x < (number_of_nodes_on_width - 1); x += 1) {
            for(int y = 0; y < (number_of_nodes_on_height - 1); y += 1) {

                makeSpringConstraint(getNode(x, y), getNode(x + 1, y + 1));
                makeSpringConstraint(getNode(x + 1, y), getNode(x, y + 1));
            }
        }
    }

    void createBendingSpringConstraints()
    {
        for(int x = 0; x < number_of_nodes_on_width - 2; x += 1) {
            for(int y = 0; y < number_of_nodes_on_height - 2; y += 1) {

                Node* centerNode = getNode(x, y);

                // springs directed towards right and bottom (structural bending)
                makeSpringConstraint(centerNode, getNode(x + 2, y));
                makeSpringConstraint(centerNode, getNode(x, y + 2));

                // springs directed diagonally (shear bending)
                makeSpringConstraint(centerNode, getNode(x + 2, y + 2));
                makeSpringConstraint(getNode(x + 2, y), getNode(x, y + 2));
            }
        }
    }

    void addNodes()
    {
        int total_number_of_nodes = number_of_nodes_on_width * number_of_nodes_on_height;
        nodes.resize(total_number_of_nodes);

        for(int x = 0; x < number_of_nodes_on_width; x += 1) {
           for(int y = 0; y < number_of_nodes_on_height; y += 1) {

               float x_position = cloth_width * ((float) x) / ((float) number_of_nodes_on_width);
               float y_position = cloth_height * ((float) y) / ((float) number_of_nodes_on_height);
               Vector3 node_position = Vector3(x_position, y_position, 0);

               nodes[y * number_of_nodes_on_width + x] = Node(node_position, true, MASS);
           }
       }
    }

public:

    Cloth(float p_cloth_width, float p_cloth_height, int p_number_of_nodes_on_width, int p_number_of_nodes_on_height) :
        cloth_width(p_cloth_width),
        cloth_height(p_cloth_height),
        number_of_nodes_on_width(p_number_of_nodes_on_width),
        number_of_nodes_on_height(p_number_of_nodes_on_height)
    {
       addNodes();
       createStructuralSpringConstraints();
       createShearSpringConstraints();
       createBendingSpringConstraints();
    }
};
