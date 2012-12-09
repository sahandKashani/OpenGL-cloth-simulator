#include "Scene.h"

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