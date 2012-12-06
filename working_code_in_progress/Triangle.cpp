#include "Triangle.h"
#include "Arrow.h"

// OpenGL imports
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>

void Triangle::draw()
{
    // set color to white
    glColor3f(1.0, 0.0, 1.0);

    glBegin(GL_TRIANGLES);
        glVertex3f(p1.x, p1.y, p1.z);
        glVertex3f(p2.x, p2.y, p2.z);
        glVertex3f(p3.x, p3.y, p3.z);
    glEnd();

    // draw normal vector
    Vector3 center = (p1 + p2 + p3) / 3.0;
    Arrow normalVector(center, center + normal);
    normalVector.draw();
}

// bool Triangle::isInsideTriangleVerticalSpace(Node* p)
// {
//     // project point on triangle plane
//     Vector3 pointOnPlane = projectToTrianglePlane(p);

//     // calculate barycentric coordinates
//     float s1 = calculateTriangleArea(pointOnPlane, node2->getPosition(), node3->getPosition()) / area;
//     float s2 = calculateTriangleArea(pointOnPlane, node1->getPosition(), node2->getPosition()) / area;
//     float s3 = calculateTriangleArea(pointOnPlane, node1->getPosition(), node3->getPosition()) / area;

//     return (0 < s1 && s1 < 1) && (0 < s2 && s2 < 1) && (0 < s3 && s3 < 1);
// }

// Vector3 Triangle::projectToTrianglePlane(Node* p)
// {
//     Vector3 vectorFromPlaneToP = p->getPosition() - node1->getPosition();
//     float distance = vectorFromPlaneToP.dot(normal);
//     Vector3 offsetToPlane = normal * distance;
//     Vector3 pointOnPlane = p->getPosition() - offsetToPlane;

//     return pointOnPlane;
// }

// float Triangle::calculateTriangleArea(Vector3 p1, Vector3 p2, Vector3 p3)
// {
//     return ((p2 - p1).cross(p3 - p1)).length() * 0.5;
// }

// Note that the order of the nodes is VERY important, because the triangle's
// normal vector will be calculated by the following algorithm.
// Give the triangles in clock-wise order for simplicity
// (p2 - p1).cross(p3 - p1).normalize();
Triangle::Triangle(Vector3 point1, Vector3 point2, Vector3 point3) :
    p1(point1),
    p2(point2),
    p3(point3),
    normal((p2 - p1).cross(p3 - p1)),
    area(normal.length() * 0.5)
{}

// FIXME : doesn't work
// void Triangle::testIntersection(Node* p)
// {
//     if(isInsideTriangleVerticalSpace(p))
//     {
//         std::cout << "inside triangle vertical space" << std::endl;
//         Vector3 vectorFromPlaneToP = p->getPosition() - node1->getPosition();

//         // TODO : Potential problem here if there is a closed surface made of triangles
//         // are just at surface, or behind it
//         if(vectorFromPlaneToP.dot(normal) <= 0.0)
//         {
//             std::cout << "you are on the wrong side of the object" << std::endl;
//             // move the node towards surface (actually, a little bit further
//             // from surface for precision reasons)
//             Vector3 pointProjectedToPlane = projectToTrianglePlane(p);
//             p->translate(pointProjectedToPlane - p->getPosition() + 0.1);
//             p->setMoveable(false);
//         }
//     }
// }