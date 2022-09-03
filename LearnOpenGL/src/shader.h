#pragma once

#include <string>
#include <unordered_map>

class Shader
{
public:
  Shader(const std::string& filePath);
  ~Shader();

  void Bind();
  void Unbind();

  void SetVec4(const std::string& name, float x, float y, float z, float w);
private:
  int GetUniformLocation(const std::string& name) const;
  std::string ReadFile(const std::string& filepath);
  const std::unordered_map<uint32_t, std::string> Preprocess(const std::string& source);
  void Compile(const std::unordered_map<uint32_t, std::string>& shader_source);
private:
  uint32_t mRenderId;
  mutable std::unordered_map<std::string, int> mLocationCache;
};