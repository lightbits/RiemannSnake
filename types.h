#ifndef TYPES_H
#define TYPES_H
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> // for glm::ortho and glm::perspective
#include <glm/gtc/type_ptr.hpp> // for value_ptr(matrix)

typedef glm::mat2 mat2;
typedef glm::mat3 mat3;
typedef glm::mat4 mat4;
typedef glm::vec2 vec2;
typedef glm::vec3 vec3;
typedef glm::vec4 vec4;
typedef std::string string;
typedef unsigned int uint32;
typedef unsigned short uint16;
typedef unsigned char uint8;

#endif