#ifndef CAMERA_H
#define CAMERA_H

#include "Vector3.h"
#include "Matrix4.h"

class Camera
{
private:
    Vector3 position;
    Vector3 viewDirection;
    Vector3 upDirection;
    Vector3 rightDirection;

    // used for getters only, not for computation
    float yaw;
    float pitch;
    float roll;

    Matrix4 getRotationMatrixAroundArbitraryAxisThroughOrigin(float angleInRadians, Vector3 rotationAxisDirection);

public:
    Camera();

    Vector3 getPosition();
    Vector3 getViewDirection();
    Vector3 getUpDirection();
    Vector3 getRightDirection();

    float getYaw();
    float getPitch();
    float getRoll();

    void setPosition(Vector3 pos);
    void setViewDirection(Vector3 direction);
    void setUpDirection(Vector3 direction);

    // translation
    void translate(Vector3 direction);

    // rotation around world axis
    void rotateAroundXAxisWorld(float angleInRadians);
    void rotateAroundYAxisWorld(float angleInRadians);
    void rotateAroundZAxisWorld(float angleInRadians);

    // rotation around camera axis
    void rotateAroundXAxisObject(float angleInRadians);
    void rotateAroundYAxisObject(float angleInRadians);
    void rotateAroundZAxisObject(float angleInRadians);
};

#endif