#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "Node.h"
#include "Vector3.h"

class Triangle
{
private:
    // IMPORTANT ALGORITHM : to check if a Node has intersected with a triangle, do the following :
    // 1) project point p onto plane formed by triangle.
    // 2) calculate barycentric coordinates of this point
    // 3) if 0 < s1, s2, s3 < 1 is FALSE, then the point is not even in the "cyclinder" around the triangle.
    // 4) if 0 < s1, s2, s3 < 1 is TRUE, then do normal plane intersection with (p - x1).n > 0 or < 0 to see on which side we are on.
    // 5) if dot product < 0, then move node towards surface

public:
    Node* n1;
    Node* n2;
    Node* n3;

    Vector3 normal;

    Triangle(Node* p1, Node* p2, Node* p3);
    void draw();

    Vector3 getNormal();
};

#endif