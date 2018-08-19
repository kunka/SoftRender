//
// Created by Administrator on 2018/4/1 0001.
//

#ifndef GL_TEXTURE_H
#define GL_TEXTURE_H

#include "CustomDraw.h"

TEST_NODE_BEGIN(Texture)

    protected:
        unsigned int texture;

        unsigned int VAOTR;
        unsigned int VBOTR;
        unsigned int EBOTR;
        unsigned int VAOBL;
        unsigned int VBOBL;
        unsigned int EBOBL;
        unsigned int VAOBR;
        unsigned int VBOBR;
        unsigned int EBOBR;

TEST_NODE_END(Texture)

#endif //GL_TEXTURE_H
