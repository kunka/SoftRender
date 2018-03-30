//
// Created by huangkun on 27/03/2018.
//

#ifndef GL_RENDER_H
#define GL_RENDER_H


#include <vector>
#include <glm/glm.hpp>

class Render {
public:
    void clear();

private:
    glm::vec4 _clearColor;
};

#endif //GL_RENDER_H
