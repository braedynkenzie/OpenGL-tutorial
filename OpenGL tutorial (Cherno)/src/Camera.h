#pragma once

// CREDIT TO https://learnopengl.com/book/learnopengl_book.pdf for the base Camera class source code

#ifndef CAMERA_H
#define CAMERA_H

#include <GL\glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 8.0f;
const float ZOOM = 45.0f;
const float SENSITIVITY = 0.4f;
const glm::vec3 FRONT = glm::vec3(0.0f, 0.0f, -1.0f);

// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera
{
public:
    // Camera Attributes
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    // Euler Angles
    float Yaw;
    float Pitch;
    // Camera options
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;

    // Constructor with vectors
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), float zoom = ZOOM, glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) 
        : Front(FRONT), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY)
    {
        Position = position;
        WorldUp = up;
        Yaw = yaw;
        Pitch = pitch;
        Zoom = zoom;
        updateCameraVectors();
    }
    // Constructor with scalar values
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
    {
        Position = glm::vec3(posX, posY, posZ);
        WorldUp = glm::vec3(upX, upY, upZ);
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }

    // Returns the view matrix calculated using Euler Angles and the LookAt Matrix
    glm::mat4 GetViewMatrix()
    {
        return glm::lookAt(Position, Position + Front, Up);
    }

    // Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void ProcessKeyboard(Camera_Movement direction, float deltaTime)
    {
        float velocity = MovementSpeed * deltaTime;
        if (direction == FORWARD)
            Position += Front * velocity;
        if (direction == BACKWARD)
            Position -= Front * velocity;
        if (direction == LEFT)
            Position -= Right * velocity;
        if (direction == RIGHT)
            Position += Right * velocity;
    }

    // Processes keyboard input for first person, 'walking' style camera
    void ProcessKeyboardForWalkingView(Camera_Movement direction, float deltaTime, float yPos)
    {
        float velocity = MovementSpeed * deltaTime;
        if (direction == FORWARD)
            Position += 2.0f * (Front * velocity);
        if (direction == BACKWARD)
            Position -= (Front * velocity);
        if (direction == LEFT)
            Position -= 2.0f * (Right * velocity);
        if (direction == RIGHT)
            Position += 2.0f * (Right * velocity);

        // Limit y min/max positions
        Position.y = yPos;
    }
    
    // Processes keyboard input for a map view style camera
    void ProcessKeyboardForMapView(Camera_Movement direction, float deltaTime, float yPosBottomLimit, float yPosUpperLimit)
    {
        float velocity = MovementSpeed * deltaTime;
        if (direction == FORWARD)
            Position += (Up * velocity) + (Front * velocity);
        if (direction == BACKWARD)
            Position -= (Up * velocity) + (Front * velocity);
        if (direction == LEFT)
            Position -= Right * velocity;
        if (direction == RIGHT)
            Position += Right * velocity;

        // Limit y min/max positions
        if (Position.y > yPosUpperLimit)
            Position.y = yPosUpperLimit;
        if (Position.y < yPosBottomLimit)
            Position.y = yPosBottomLimit;
    }

    // Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void ProcessMouseMovement(float xOffset, float yOffset, GLboolean constrainPitch = true)
    {
        xOffset *= MouseSensitivity;
        yOffset *= MouseSensitivity;

        Yaw += xOffset;
        Pitch += yOffset;

        // Make sure that when pitch is out of bounds, screen doesn't get flipped
        if (constrainPitch)
        {
            if (Pitch > 89.0f)
                Pitch = 89.0f;
            if (Pitch < -89.0f)
                Pitch = -89.0f;
        }

        // Update Front, Right and Up Vectors using the updated Euler angles
        updateCameraVectors();
    }

    // Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void ProcessMouseScroll(float yOffset)
    {
        float fov_max = 135.0f;
        float fov_min = 0.5f;
        if (Zoom >= fov_min && Zoom <= fov_max)
            Zoom -= yOffset;
        if (Zoom <= fov_min)
            Zoom = fov_min;
        if (Zoom >= fov_max)
            Zoom = fov_max;
    }

private:
    // Calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors()
    {
        // Calculate the new Front vector
        glm::vec3 front;
        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.y = sin(glm::radians(Pitch));
        front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        Front = glm::normalize(front);
        // Also re-calculate the Right and Up vector
        Right = glm::normalize(glm::cross(Front, WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        Up = glm::normalize(glm::cross(Right, Front));
    }
};
#endif