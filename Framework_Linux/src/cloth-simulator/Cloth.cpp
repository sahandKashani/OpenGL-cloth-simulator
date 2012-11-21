// #include <vector>
// #include "../math/Vector3.h"
// #include "Node.h"
// #include "SpringConstraint.h"
// #include "../gl/GlutViewer.h"

// class Cloth {

// private:

//     int cloth_width;
//     int cloth_height;
//     int number_of_nodes_on_width;
//     int number_of_nodes_on_height;

//     std::vector<Node> nodes;
//     std::vector<SpringConstraint> springConstraints;

//     Node* getNode(int x, int y)
//     {
//         return &nodes[y * number_of_nodes_on_width + x];
//     }

//     void makeSpringConstraint(Node* node_1, Node* node_2)
//     {
//         springConstraints.push_back(SpringConstraint(node_1, node_2));
//     }

//     void createStructuralSpringConstraints()
//     {
//         for(int x = 0; x < (number_of_nodes_on_width - 1); x += 1) {
//             for(int y = 0; y < (number_of_nodes_on_height - 1); y += 1) {

//                 Node* centerNode = getNode(x, y);
//                 makeSpringConstraint(centerNode, getNode(x + 1, y));
//                 makeSpringConstraint(centerNode, getNode(x, y + 1));
//             }
//         }
//     }

//     void createShearSpringConstraints()
//     {
//         for(int x = 0; x < (number_of_nodes_on_width - 1); x += 1) {
//             for(int y = 0; y < (number_of_nodes_on_height - 1); y += 1) {

//                 makeSpringConstraint(getNode(x, y), getNode(x + 1, y + 1));
//                 makeSpringConstraint(getNode(x + 1, y), getNode(x, y + 1));
//             }
//         }
//     }

//     void createBendingSpringConstraints()
//     {
//         for(int x = 0; x < number_of_nodes_on_width - 2; x += 1) {
//             for(int y = 0; y < number_of_nodes_on_height - 2; y += 1) {

//                 Node* centerNode = getNode(x, y);

//                 // springs directed towards right and bottom (structural bending)
//                 makeSpringConstraint(centerNode, getNode(x + 2, y));
//                 makeSpringConstraint(centerNode, getNode(x, y + 2));

//                 // springs directed diagonally (shear bending)
//                 makeSpringConstraint(centerNode, getNode(x + 2, y + 2));
//                 makeSpringConstraint(getNode(x + 2, y), getNode(x, y + 2));
//             }
//         }
//     }

//     void addNodes()
//     {
//         int total_number_of_nodes = number_of_nodes_on_width * number_of_nodes_on_height;
//         nodes.resize(total_number_of_nodes);

//         for(int x = 0; x < number_of_nodes_on_width; x += 1) {
//            for(int y = 0; y < number_of_nodes_on_height; y += 1) {

//                float x_position = cloth_width * ((float) x) / ((float) number_of_nodes_on_width);
//                float y_position = cloth_height * ((float) y) / ((float) number_of_nodes_on_height);
//                Vector3 node_position = Vector3(x_position, y_position, 5.0);

//                nodes[y * number_of_nodes_on_width + x] = Node(node_position, true, MASS);
//            }
//        }
//     }

// public:

//     Cloth(float p_cloth_width = 50.0, float p_cloth_height = 50.0, int p_number_of_nodes_on_width = 20, int p_number_of_nodes_on_height = 20) :
//         cloth_width(p_cloth_width),
//         cloth_height(p_cloth_height),
//         number_of_nodes_on_width(p_number_of_nodes_on_width),
//         number_of_nodes_on_height(p_number_of_nodes_on_height)
//     {
//        addNodes();
//        createStructuralSpringConstraints();
//        createShearSpringConstraints();
//        createBendingSpringConstraints();
//     }

//     void draw()
//     {
//         for(std::vector<Node>::iterator node_iterator = nodes.begin(); node_iterator != nodes.end(); ++node_iterator) {
//             Vector3 position = node_iterator->getPosition();

//             std::cout << "position = (" << position.x << ", " << position.y << ", " << position.z << ")" << std::endl;

//             // built-in (glut library) function , draw you a sphere.
//             glutSolidSphere(90, 5, 5);
//         }
//     }
// };
