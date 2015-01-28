#include "Triangle.h"
#include "Arrow.h"
#include "DrawingSettings.h"

// OpenGL imports
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>

void Triangle::draw()
{
    DrawingSettings* drawingSettings = DrawingSettings::getInstance();

    if(drawingSettings->isDrawTrianglesEnabled())
    {
        glPushAttrib(GL_POLYGON_BIT); // save mesh settings
            glPushAttrib(GL_CURRENT_BIT); // save color
                glPushAttrib(GL_LIGHTING_BIT);

                    glEnable(GL_LIGHTING);
                    glEnable(GL_LIGHT0);
                    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

                    Vector3 p1 = n1->getPosition();
                    Vector3 p1_normal = n1->getNormal();

                    Vector3 p2 = n2->getPosition();
                    Vector3 p2_normal = n2->getNormal();

                    Vector3 p3 = n3->getPosition();
                    Vector3 p3_normal = n3->getNormal();

                    Vector3 color = drawingSettings->getTriangleColor();

                    GLfloat mat_amb_diff[] = {color.x, color.y, color.z, 1.0};
                    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, mat_amb_diff);

                    glBegin(GL_TRIANGLES);
                        glNormal3f(p1_normal.x, p1_normal.y, p1_normal.z);
                        glVertex3f(p1.x, p1.y, p1.z);

                        glNormal3f(p2_normal.x, p2_normal.y, p2_normal.z);
                        glVertex3f(p2.x, p2.y, p2.z);

                        glNormal3f(p3_normal.x, p3_normal.y, p3_normal.z);
                        glVertex3f(p3.x, p3.y, p3.z);
                    glEnd();

                glPopAttrib(); // GL_LIGHTING_BIT
            glPopAttrib(); // GL_CURRENT_BIT
        glPopAttrib(); // GL_POLYGON_BIT

        // draw normal vector
        Vector3 center = (p1 + p2 + p3) / 3.0;
        Arrow normalVector(center, center + normal);
        normalVector.draw();
    }
}

// declare points in counter-clockwise direction for all triangles to have the
// same outer surface
Triangle::Triangle(Node* p1, Node* p2, Node* p3) :
    n1(p1),
    n2(p2),
    n3(p3),
    normal((p2->getPosition() - p1->getPosition()).cross(p3->getPosition() - p1->getPosition()).normalize())
{}

Vector3 Triangle::getNormal()
{
    return normal;
}