#ifndef SHADER_H
#define SHADER_H

#include "glm/glm.hpp"

#include <string>
 
class Shader {
public:
    unsigned int id;
  
    Shader(const char* vertexPath, const char* fragmentPath);
    void use();
    void setBool(const std::string &name, bool value) const;  
    void setInt(const std::string &name, int value) const;   
    void setFloat(const std::string &name, float value) const;
    void setVec4(const std::string &name, glm::vec4 value) const;
    void setMat4(const std::string &name, glm::mat4 value) const;
};
  
#endif
