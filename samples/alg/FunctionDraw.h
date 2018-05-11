//
// Created by huangkun on 2018/5/4.
//

#ifndef GL_FUNCTIONDRAW_H
#define GL_FUNCTIONDRAW_H


#include "CustomDraw.h"

TEST_NODE_BEGIN(FunctionDraw)

        virtual void fixedUpdate(float delta) override;

        void setCamera(const glm::vec3 &position, const glm::vec3 &rotation);

        void linear(float &var, float from, float to, float dur);

        std::vector<std::function<void(float)>> tickers;
        float elapsed;
TEST_NODE_END(FunctionDraw)

#endif //GL_FUNCTIONDRAW_H
