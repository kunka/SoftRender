//
// Created by huangkun on 27/03/2018.
//

#include "Render.h"
#include <glad/glad.h>

void Render::clear() {
    glDepthMask(true);
    glClearColor(_clearColor.r, _clearColor.g, _clearColor.b, _clearColor.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDepthMask(false);
}
