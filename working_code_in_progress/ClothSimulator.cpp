#include "ClothSimulator.h"
#include "Keyboard.h"

// OpenGL imports
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>

ClothSimulator* ClothSimulator::instance = 0;

ClothSimulator* ClothSimulator::getInstance()
{
    if(instance == 0)
    {
        instance = new ClothSimulator();
    }

    return instance;
}

ClothSimulator::ClothSimulator()
{}

void ClothSimulator::simulate()
{
    Keyboard::getInstance()->applyNormalKeyboardActions();
    scene->simulate();
}

void ClothSimulator::draw()
{
    scene->draw();
}

void ClothSimulator::createScene()
{
    Keyboard* keyboard = Keyboard::getInstance();

    // TODO : Enable later
    // keyboard->showHelp();
    keyboard->resetKeyboardStatus();

    scene = new BatmanScene();
}

Scene* ClothSimulator::getScene()
{
    return scene;
}