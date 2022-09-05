#pragma once

#include <string>
#include <unordered_map>
#include <glm/glm.hpp>

class Shader
{
public:
  Shader(const std::string& filePath);
  ~Shader();

  void Bind() const;
  void Unbind() const;

  void SetVec3(const std::string& name, float x, float y, float z) const;
  void SetVec4(const std::string& name, float x, float y, float z, float w) const;
  void SetInt(const std::string& name, int value) const;
  void SetMat4(const std::string& name, const glm::mat4& mat4) const;
private:
  int GetUniformLocation(const std::string& name) const;
  std::string ReadFile(const std::string& filepath);
  const std::unordered_map<uint32_t, std::string> Preprocess(const std::string& source);
  void Compile(const std::unordered_map<uint32_t, std::string>& shader_source);
private:
  uint32_t mRenderId;
  mutable std::unordered_map<std::string, int> mLocationCache;
};