#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>

inline float Clamp(float value, float bottom, float top)
{
  return std::max(std::min(value, top), bottom);
}

Camera::Camera(const glm::vec3& position, const glm::vec3& up)
  : mPosition(position), mUp(up)
{
  mYaw = -90.0f;
  mPitch = 0.0f;
  float yaw = glm::radians(mYaw);
  float pitch = glm::radians(mPitch);
  mFront = glm::vec3(glm::cos(yaw) * glm::cos(pitch), glm::sin(pitch), glm::sin(yaw) * glm::cos(pitch));
  mFront = glm::normalize(mFront);
  mRight = glm::normalize(glm::cross(mFront, mUp));
  mUp = glm::cross(mRight, mFront);
  mFov = 45.f;

  mNear = 0.1f;
  mFar = 1000.0f;

  mPanSpeed = 0.5f;
  mRotationSpeed = 0.1f;
  mAspectRatio = 800.0f / 600.0f;
  UpdateViewMatrix();
  UpdateProjectionMatrix();
}

void Camera::OnUpdate(float deltaTime)
{}

void Camera::OnMouseMovement(float xOffset, float yOffset)
{
  mYaw += xOffset * mRotationSpeed;
  mPitch += yOffset * mRotationSpeed;
  mPitch = Clamp(mPitch, -89.0f, 89.0f);
  float yaw = glm::radians(mYaw);
  float pitch = glm::radians(mPitch);
  mFront = glm::vec3(glm::cos(yaw) * glm::cos(pitch), glm::sin(pitch), glm::sin(yaw) * glm::cos(pitch));
  mFront = glm::normalize(mFront);
  mRight = glm::normalize(glm::cross(mFront, mUp));
  mUp = glm::cross(mRight, mFront);
  UpdateViewMatrix();
}

void Camera::OnKeyPress(const CameraMovement & movement, float deltaTime)
{
  if (movement == CameraMovement::Forward)
  {
    mPosition += mPanSpeed * mFront;
    UpdateViewMatrix();
  } else if (movement == CameraMovement::Backward)
  {
    mPosition -= mPanSpeed * mFront;
    UpdateViewMatrix();
  }
  if (movement == CameraMovement::Left)
  {
    mPosition -= glm::normalize(glm::cross(mFront, mUp)) * mPanSpeed;
    UpdateViewMatrix();
  } else if (movement == CameraMovement::Right)
  {
    mPosition += glm::normalize(glm::cross(mFront, mUp)) * mPanSpeed;
    UpdateViewMatrix();
  }
}

void Camera::OnMouseScroll(float yOffset)
{
  mFov -= (float)yOffset;
  mFov = Clamp(mFov, 1.0f, 80.f);
  UpdateProjectionMatrix();
}

void Camera::OnResize(float width, float height)
{
  mAspectRatio = width / height;
  UpdateProjectionMatrix();
}

void Camera::SetAspectRatio(float aspectRatio)
{
  mAspectRatio = aspectRatio;
}

glm::vec3 Camera::GetPosition() const
{
  return mPosition;
}

glm::vec3 Camera::GetFront() const
{
  return mFront;
}

glm::mat4 Camera::GetViewMatrix() const
{
  return mViewMatrix;
}

glm::mat4 Camera::GetProjectionMatrix() const
{
  return mProjectionMatrix;
}

void Camera::UpdateViewMatrix()
{
  mViewMatrix = glm::lookAt(mPosition, mPosition + mFront, mUp);
}

void Camera::UpdateProjectionMatrix()
{
  mProjectionMatrix = glm::perspective(mFov, mAspectRatio, mNear, mFar);
}
