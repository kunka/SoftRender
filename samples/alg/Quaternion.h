//
// Created by huangkun on 2018/5/9.
//

#ifndef GL_QUATERNION_H
#define GL_QUATERNION_H


#include "CustomDraw.h"

TEST_NODE_BEGIN(Quaternion)

        virtual void fixedUpdate(float delta) override;


        void lerp(vec3 &var, vec3 from, vec3 to, float dur);
        void slerp(vec3 &var, vec3 from, vec3 to, float dur);
        void slerpQ(vec3 &var, vec3 from, vec3 to, float dur);
        void slerpT(vec3 &var, vec3 from, vec3 to, float dur);

        std::vector<std::function<void(float)>> tickers;
        float elapsed1;
        float elapsed2;
        float elapsed3;
        float elapsed4;
        std::vector<float> vertices;
        vec3 pA;
        vec3 pB;
        vec3 p1;
        vec3 p2;
        vec3 p3;
        vec3 p4;
TEST_NODE_END(Quaternion)


#endif //GL_QUATERNION_H
