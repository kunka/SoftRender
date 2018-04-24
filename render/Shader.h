//
//  Shader.h
//  gl
//
//  Created by huangkun on 12/09/2017.
//  Copyright © 2017 kk. All rights reserved.
//

#ifndef Shader_h
#define Shader_h

#include <glm/glm.hpp>
#include <glad/glad.h> // include glad to get all the required OpenGL headers
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include "Log.h"

//extern const char *shader_positionColor_vert;
//extern const char *shader_positionColor_frag;

#define CHECK_GL_ERROR_DEBUG() \
    do { \
        GLenum __error = glGetError(); \
        if(__error) { \
            log("OpenGL error 0x%04X in %s %s %d\n", __error, __FILE__, __FUNCTION__, __LINE__); \
        } \
    } while (false)


class Shader {
public:
    // the program ID
    unsigned int ID;

    // constructor reads and builds the shader
    Shader();

    void loadStr(const GLchar *vertexStr, const GLchar *fragmentStr);

    void loadFile(const GLchar *vertexPath, const GLchar *fragmentPath);

    // use/activate the shader
    void use();

    // utility uniform functions
    void setBool(const std::string &name, bool value) const;

    void setInt(const std::string &name, int value) const;

    void setFloat(const std::string &name, float value) const;

    void setVec3(const std::string &name, const glm::vec3 &vec3) const;

    void setVec4(const std::string &name, const glm::vec4 &vec4) const;

    void setMat4(const std::string &name, const glm::mat4 &mat4) const;

private:
    // utility function for checking shader compilation/linking errors.
    // ------------------------------------------------------------------------
    void checkCompileErrors(unsigned int shader, std::string type);
};

#endif /* Shader_h */
