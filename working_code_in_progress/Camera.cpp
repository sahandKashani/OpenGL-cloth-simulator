#include "Camera.h"

// initialize camera with the following properties:
// position = (0.0, 0.0, 0.0)
// viewDirection = (0.0, 0.0, -1.0)
// upDirection = (0.0, 1.0, 0.0)
// rightDirection = (1.0, 0.0, 0.0)
// yaw = 0.0
// pitch = 0.0
// roll = 0.0
Camera::Camera() :
    position(Vector3(0.0, 0.0, 0.0)),
    viewDirection(Vector3(0.0, 0.0, -1.0)),
    upDirection(Vector3(0.0, 1.0, 0.0)),
    rightDirection(Vector3(1.0, 0.0, 0.0)),
    yaw(0.0),
    pitch(0.0),
    roll(0.0)
{}

void Camera::setViewDirection(Vector3 direction)
{
    viewDirection = direction;
}

void Camera::setUpDirection(Vector3 direction)
{
    upDirection = direction;
}

void Camera::setPosition(Vector3 pos)
{
    position = pos;
}

Vector3 Camera::getPosition()
{
    return position;
}

Vector3 Camera::getViewDirection()
{
    return viewDirection;
}

Vector3 Camera::getUpDirection()
{
    return upDirection;
}

Vector3 Camera::getRightDirection()
{
    return rightDirection;
}

float Camera::getYaw()
{
    return yaw;
}

float Camera::getPitch()
{
    return pitch;
}

float Camera::getRoll()
{
    return roll;
}

void Camera::translate(Vector3 direction)
{
    position += direction;
}

Matrix4 Camera::getRotationMatrixAroundArbitraryAxisThroughOrigin(float angleInRadians, Vector3 rotationAxisDirection)
{
    // application of Rodriguez' rotation formula (taken from Wikipedia not from
    // the course, since the course slides were using a left-handed coordinate system,
    // but we want to use a right-handed one.)

    // normalize direction
    Vector3 unitRotationAxisDirection = rotationAxisDirection.normalize();

    float u_x = unitRotationAxisDirection.x;
    float u_y = unitRotationAxisDirection.y;
    float u_z = unitRotationAxisDirection.z;

    float cosa = cos(angleInRadians);
    float sina = sin(angleInRadians);

    // first column of final rotation matrix
    Vector3 col1 = Vector3(cosa + u_x * u_x * (1.0 - cosa)      ,
                           u_y * u_x * (1.0 - cosa) + u_z * sina,
                           u_z * u_x * (1.0 - cosa) - u_y * sina);

    // second column of final rotation matrix
    Vector3 col2 = Vector3(u_x * u_y * (1.0 - cosa) - u_z * sina,
                           cosa + u_y * u_y * (1.0 - cosa)      ,
                           u_z * u_y * (1.0 - cosa) + u_x * sina);

    // third column of final rotation matrix
    Vector3 col3 = Vector3(u_x * u_z * (1.0 - cosa) + u_y * sina,
                           u_y * u_z * (1.0 - cosa) - u_x * sina,
                           cosa + u_z * u_z * (1.0 - cosa)      );

    Matrix4 rotationMatrix = Matrix4(col1, col2, col3);
    return rotationMatrix;
}

void Camera::rotateAroundXAxisObject(float angleInRadians)
{
    pitch += angleInRadians;

    Vector3 oldPosition = position;

    // go to (0.0, 0.0, 0.0)
    translate(Vector3(-oldPosition.x, -oldPosition.y, -oldPosition.z));

    Vector3 rotationAxis = upDirection.cross(viewDirection);
    Matrix4 rotationMatrix = getRotationMatrixAroundArbitraryAxisThroughOrigin(angleInRadians, rotationAxis);

    // position does not change
    viewDirection = rotationMatrix * viewDirection;
    upDirection = rotationMatrix * upDirection;
    rightDirection = rotationMatrix * rightDirection;

    // go back to the position the camera was previously in
    translate(Vector3(oldPosition.x, oldPosition.y, oldPosition.z));
}

void Camera::rotateAroundYAxisObject(float angleInRadians)
{
    yaw += angleInRadians;

    Vector3 oldPosition = position;

    // go to (0.0, 0.0, 0.0)
    translate(Vector3(-oldPosition.x, -oldPosition.y, -oldPosition.z));

    Vector3 rotationAxis = upDirection;
    Matrix4 rotationMatrix = getRotationMatrixAroundArbitraryAxisThroughOrigin(angleInRadians, rotationAxis);

    // position does not change
    viewDirection = rotationMatrix * viewDirection;
    upDirection = rotationMatrix * upDirection;
    rightDirection = rotationMatrix * rightDirection;

    // go back to the position the camera was previously in
    translate(Vector3(oldPosition.x, oldPosition.y, oldPosition.z));
}

void Camera::rotateAroundZAxisObject(float angleInRadians)
{
    roll += angleInRadians;

    Vector3 oldPosition = position;

    // go to (0.0, 0.0, 0.0)
    translate(Vector3(-oldPosition.x, -oldPosition.y, -oldPosition.z));

    Vector3 rotationAxis = viewDirection;
    Matrix4 rotationMatrix = getRotationMatrixAroundArbitraryAxisThroughOrigin(angleInRadians, rotationAxis);

    // position does not change
    viewDirection = rotationMatrix * viewDirection;
    upDirection = rotationMatrix * upDirection;
    rightDirection = rotationMatrix * rightDirection;

    // go back to the position the camera was previously in
    translate(Vector3(oldPosition.x, oldPosition.y, oldPosition.z));
}

void Camera::rotateAroundXAxisWorld(float angleInRadians)
{
    Vector3 rotationAxis = Vector3(1.0, 0.0, 0.0);
    Matrix4 rotationMatrix = getRotationMatrixAroundArbitraryAxisThroughOrigin(angleInRadians, rotationAxis);

    position = rotationMatrix * position;
    upDirection = rotationMatrix * upDirection;
    viewDirection = rotationMatrix * viewDirection;
    rightDirection = rotationMatrix * rightDirection;
}

void Camera::rotateAroundYAxisWorld(float angleInRadians)
{
    Vector3 rotationAxis = Vector3(0.0, 1.0, 0.0);
    Matrix4 rotationMatrix = getRotationMatrixAroundArbitraryAxisThroughOrigin(angleInRadians, rotationAxis);

    position = rotationMatrix * position;
    viewDirection = rotationMatrix * viewDirection;
    upDirection = rotationMatrix * upDirection;
    rightDirection = rotationMatrix * rightDirection;
}

void Camera::rotateAroundZAxisWorld(float angleInRadians)
{
    Vector3 rotationAxis = Vector3(0.0, 0.0, 1.0);
    Matrix4 rotationMatrix = getRotationMatrixAroundArbitraryAxisThroughOrigin(angleInRadians, rotationAxis);

    position = rotationMatrix * position;
    viewDirection = rotationMatrix * viewDirection;
    upDirection = rotationMatrix * upDirection;
    rightDirection = rotationMatrix * rightDirection;
}

void Camera::showCameraStatus()
{
    std::cout << "camera status:" << std::endl;
    std::cout << "  camera position      : " << position.toString() << std::endl;
    std::cout << "  camera view direction: " << viewDirection.toString() << std::endl;
    std::cout << "  camera up direction  : " << upDirection.toString() << std::endl;
    std::cout << "  yaw                  : " << yaw << " rad" << std::endl;
    std::cout << "  pitch                : " << pitch << " rad" << std::endl;
    std::cout << "  roll                 : " << roll << " rad" << std::endl;

    std::cout << std::endl;
}