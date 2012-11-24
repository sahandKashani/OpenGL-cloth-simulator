#include "Cloth.h"
#include "Node.h"
#include <iostream>

Node* Cloth::getNode(int x, int y)
{
    return &nodes[y * number_of_nodes_on_width + x];
}

void Cloth::makeSpringConstraint(Node* node_1, Node* node_2)
{
    springConstraints.push_back(SpringConstraint(node_1, node_2));
}

void Cloth::createStructuralSpringConstraints()
{
    for(int x = 0; x < (number_of_nodes_on_width - 1); x += 1) {
        for(int y = 0; y < (number_of_nodes_on_height - 1); y += 1) {

            Node* centerNode = getNode(x, y);
            makeSpringConstraint(centerNode, getNode(x + 1, y));
            makeSpringConstraint(centerNode, getNode(x, y + 1));
        }
    }
}

void Cloth::createShearSpringConstraints()
{
    for(int x = 0; x < (number_of_nodes_on_width - 1); x += 1) {
        for(int y = 0; y < (number_of_nodes_on_height - 1); y += 1) {

            makeSpringConstraint(getNode(x, y), getNode(x + 1, y + 1));
            makeSpringConstraint(getNode(x + 1, y), getNode(x, y + 1));
        }
    }
}

void Cloth::createBendingSpringConstraints()
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

void Cloth::addNodes()
{
    int total_number_of_nodes = number_of_nodes_on_width * number_of_nodes_on_height;
    nodes.resize(total_number_of_nodes);

    for(int x = 0; x < number_of_nodes_on_width; x += 1) {
       for(int y = 0; y < number_of_nodes_on_height; y += 1) {

           float x_position = cloth_width * ((float) x) / ((float) number_of_nodes_on_width);
           float y_position = cloth_height * ((float) y) / ((float) number_of_nodes_on_height);
           Vector3 node_position = Vector3(x_position, y_position, 0.0);

           nodes[y * number_of_nodes_on_width + x] = Node(node_position, true);
           node_position.print("added node");
       }
   }
}

Cloth::Cloth()
{}

Cloth::Cloth(float p_cloth_width = 50.0, float p_cloth_height = 50.0, int p_number_of_nodes_on_width = 20, int p_number_of_nodes_on_height = 20) :
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

void Cloth::draw()
{
    for(std::vector<Node>::iterator node_iterator = nodes.begin(); node_iterator != nodes.end(); ++node_iterator) {
        // node_iterator->draw();
    }
}