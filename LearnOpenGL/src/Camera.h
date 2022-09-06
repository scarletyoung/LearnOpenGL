#pragma once

#include <glm/glm.hpp>

enum class CameraMovement
{
  Forward, Backward, Left, Right
};

class Camera
{
public:
  Camera(const glm::vec3& position = glm::vec3(0.0f, 0.0f, 0.0f),
         const glm::vec3& up = glm::vec3(0.0f, 1.0f, 0.0f));

  void OnUpdate(float deltaTime);
  void OnMouseMovement(float xOffset, float yOffset);
  void OnKeyPress(const CameraMovement& movement, float deltaTime);
  void OnMouseScroll(float yOffset);
  void OnResize(float width, float height);

  void SetAspectRatio(float aspectRatio);
  
  glm::vec3 GetPosition() const;

  glm::mat4 GetViewMatrix() const;
  glm::mat4 GetProjectionMatrix() const;
private:
  void UpdateViewMatrix();
  void UpdateProjectionMatrix();
private:
  glm::vec3 mPosition;
  glm::vec3 mFront;
  glm::vec3 mUp;
  glm::vec3 mRight;
  float mFov;
  float mAspectRatio;

  float mYaw;
  float mPitch;

  float mNear;
  float mFar;

  float mPanSpeed;
  float mRotationSpeed;

  glm::mat4 mViewMatrix;
  glm::mat4 mProjectionMatrix;
};