#include "SpringConstraint.h"
#include "Node.h"
#include "../math/Vector3.h"

SpringConstraint::SpringConstraint(Node* n1, Node* n2) :
    node_1(n1),
    node_2(n2)
{
    Vector3 vector_between_nodes = node_2->getPosition() - node_1->getPosition();
    distance_at_rest = vector_between_nodes.length();
}

void SpringConstraint::satisfyConstraint()
{
    Vector3 vector_between_nodes = node_2->getPosition() - node_1->getPosition();
    float current_distance_between_nodes = vector_between_nodes.length();
    float distance_ratio_difference = distance_at_rest / current_distance_between_nodes;
    Vector3 total_correction_vector = vector_between_nodes * (1.0 - distance_ratio_difference);
    Vector3 correction_per_node_vector = total_correction_vector / 2.0;

    node_1->translateByOffset(correction_per_node_vector);
    node_2->translateByOffset(-1 * correction_per_node_vector);
}