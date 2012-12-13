#include "Scene.h"
#include "DrawingSettings.h"

// OpenGL imports
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>

Scene::Scene()
{}

float Scene::getNearPlane()
{
    return nearPlane;
}

float Scene::getFarPlane()
{
    return farPlane;
}

Camera* Scene::getCamera()
{
    return camera;
}

void Scene::drawWorldAxis()
{
    if(DrawingSettings::getInstance()->isDrawWorldAxisEnabled())
    {
        glPushAttrib(GL_POLYGON_BIT ); // save mesh settings
            glPushAttrib(GL_CURRENT_BIT); // save color

                glColor3f(1.0, 0.0, 0.0);
                // x-axis in red
                glBegin(GL_LINES);
                    glVertex3f(0.0, 0.0, 0.0);
                    glVertex3f(1.0, 0.0, 0.0);
                glEnd();
                glPushMatrix();
                    glRotatef(90, 0.0, 1.0, 0.0);
                    glTranslatef(0.0, 0.0, 1.0);
                    glutSolidCone(0.15, 0.30, 10, 10);
                glPopMatrix();

                // y-axis in green
                glColor3f(0.0, 1.0, 0.0);
                glBegin(GL_LINES);
                    glVertex3f(0.0, 0.0, 0.0);
                    glVertex3f(0.0, 1.0, 0.0);
                glEnd();
                glPushMatrix();
                    glRotatef(-90, 1.0, 0.0, 0.0);
                    glTranslatef(0.0, 0.0, 1.0);
                    glutSolidCone(0.15, 0.30, 10, 10);
                glPopMatrix();

                // z-axis in blue
                glColor3f(0.0, 0.0, 1.0);
                glBegin(GL_LINES);
                    glVertex3f(0.0, 0.0, 0.0);
                    glVertex3f(0.0, 0.0, 1.0);
                glEnd();
                glPushMatrix();
                    glTranslatef(0.0, 0.0, 1.0);
                    glutSolidCone(0.15, 0.30, 10, 10);
                glPopMatrix();

            glPopAttrib(); // GL_CURRENT_BIT
        glPopAttrib(); // GL_POLYGON_BIT
    }
}