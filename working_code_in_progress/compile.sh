#! /bin/sh

clear; g++ -o simulation main.cpp ClothSimulator.cpp Node.cpp Camera.cpp Constraint.cpp StructuralConstraint.cpp ShearConstraint.cpp Arrow.cpp Sphere.cpp Triangle.cpp Cloth.cpp Floor.cpp Scene.cpp BatmanScene.cpp Keyboard.cpp DrawingSettings.cpp -lglut -lGLU -lGL; ./simulation;