#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;

enum CAM_MOVEMENT {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

class Camera
{
private:
    vec3 camPos, camFront, camDirection, camRight, camUp, up;
    mat4 view;
    float moveSpeed = 2.5f;
    float pitch, yaw, mouseSense;
    float fov;

    void UpdateCameraFront()
    {
        vec3 direction;
        direction.x = cos(radians(yaw)) * cos(radians(pitch));
        direction.y = sin(radians(pitch));
        direction.z = sin(radians(yaw)) * cos(radians(pitch));

        camFront = normalize(direction);
    }

public:
    Camera(vec3 camP, vec3 camF, vec3 up, float moveSpeed, float pitch, float yaw, float mouseSense, float fov)
    {
        this->camPos = camP;
        this->camFront = camF;
        this->up = up;
        this->moveSpeed = moveSpeed;

        this->pitch = pitch;
        this->yaw = yaw;
        this->mouseSense = mouseSense;
        this->fov = fov;

        //this->camTarget = camT;
        //this->camDirection = normalize(this->camPos - this->camTarget);
        //this->camRight = normalize(cross(up, this->camDirection));
        //this->camUp = cross(this->camDirection, this->camRight);
    };

    // Getters 
    mat4 getView() const
    {
        // GLM can create its own custom view matrix! Might not be best for FPS style controls/customizable view matrices.
        return lookAt(camPos, camPos + camFront, up);
    };

    float getFOV() const
    {
        return this->fov;
    }

    vec3 getPos() const
    {
        return this->camPos;
    }

    void setYaw(float yaw) {
        this->yaw = yaw;
    };

    float getYaw() {
        return this->yaw;
    };

    void CamPosTranslate(CAM_MOVEMENT input, float dt)
    {
        // We make this variable to ensure that as we look up/down the speed does not decrease!
        vec3 xzPlane = normalize(vec3(cos(radians(yaw)), 0.0f, sin(radians(yaw))));
        float camSpeed = moveSpeed * dt;
        if(input == FORWARD)
            camPos += camSpeed * xzPlane;
        if(input == BACKWARD)
            camPos -= camSpeed * xzPlane;
        if(input == LEFT)
            camPos -= normalize(cross(xzPlane, up)) * camSpeed;
        if(input == RIGHT)
            camPos += normalize(cross(xzPlane, up)) * camSpeed;

        camPos.y = 0.0f;
    }

    void MouseProcess(float xoff, float yoff, bool constrain)
    {
        //cout << yaw << " | " << pitch << endl;

        xoff *= this->mouseSense;
        yoff *= this->mouseSense;

        yaw += xoff;
        pitch += yoff;

        // Constrain the pitch value to not cause LookAt flip!
        if(constrain) {
            if(pitch > 89.0f)
                pitch = 89.0f;
            else if(pitch < -89.0f)
                pitch = -89.0f;
        }

        this->UpdateCameraFront();
    }

    void ScrollProcess(float yoffset)
    {
        fov -= (float)yoffset;
        if (fov < 1.0f)
            fov = 1.0f;
        if (fov > 45.0f)
            fov = 45.0f; 
    }
};

#endif