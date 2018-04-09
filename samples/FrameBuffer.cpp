//
// Created by huangkun on 09/04/2018.
//

#include "FrameBuffer.h"

TEST_NODE_IMP_BEGIN

    FrameBuffer::FrameBuffer() {
        const char *vert = R"(
#version 330 core
layout (location = 0) in vec3 a_position;
layout (location = 1) in vec2 a_texCoord;
out vec2 texCoord;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(a_position, 1.0);
    texCoord = a_texCoord;
}
)";

        const char *frame_vert = R"(
#version 330 core
layout (location = 0) in vec2 a_position;
layout (location = 1) in vec2 a_texCoord;
out vec2 texCoord;

void main()
{
    gl_Position = vec4(a_position.x, a_position.y, 0.0, 1.0);
    texCoord = a_texCoord;
}
)";

        const char *frag = R"(
#version 330 core
in vec2 texCoord;
out vec4 FragColor;

uniform sampler2D ourTexture;

void main()
{
    FragColor = texture(ourTexture, texCoord);
}
)";
        const char *frame_frag = R"(
#version 330 core
in vec2 texCoord;
out vec4 FragColor;

uniform sampler2D ourTexture;

const float offset = 1.0 / 300.0;

void main()
{
    // Origin
//    FragColor = texture(ourTexture, texCoord);

    // Inversion
//    FragColor = vec4(vec3(1.0 - texture(ourTexture, texCoord)), 1.0);

    // GrayScale
//    FragColor = texture(ourTexture, texCoord);
//    float average = 0.2126 * FragColor.r + 0.7152 * FragColor.g + 0.0722 * FragColor.b;
//    FragColor = vec4(average, average, average, 1.0);

    // Kernel Effects
      vec2 offsets[9] = vec2[](
           vec2(-offset, offset), // top-left
           vec2( 0.0f,    offset), // top-center
           vec2( offset, offset), // top-right
           vec2(-offset, 0.0f), // center-left
           vec2( 0.0f,    0.0f), // center-center
           vec2( offset, 0.0f), // center-right
           vec2(-offset, -offset), // bottom-left
           vec2( 0.0f, -offset), // bottom-center
           vec2( offset, -offset) // bottom-right
      );

//       float kernel[9] = float[](
//           -1, -1, -1,
//           -1, 9, -1,
//           -1, -1, -1
//        );

        // Blur
//       float kernel[9] = float[](
//            1.0 / 16, 2.0 / 16, 1.0 / 16,
//            2.0 / 16, 4.0 / 16, 2.0 / 16,
//            1.0 / 16, 2.0 / 16, 1.0 / 16
//        );

        // Edge detection
       float kernel[9] = float[](
            1.0 , 1.0, 1.0,
            1.0 , -8.0, 1.0,
            1.0 , 1.0, 1.0
        );

       vec3 sampleTex[9];
       for(int i = 0; i < 9; i++)
       {
           sampleTex[i] = vec3(texture(ourTexture, texCoord.st + offsets[i]));
       }
       vec3 col = vec3(0.0);
       for(int i = 0; i < 9; i++)
           col += sampleTex[i] * kernel[i];
       FragColor = vec4(col, 1.0);
}
)";

        shader.loadStr(vert, frag);
        frameShader.loadStr(frame_vert, frame_frag);

        shader.use();
        shader.setInt("ourTexture", 0);
        shader.setMat4("projection", projection);
        cameraPos = vec3(0.0f, 2.0f, 4.0f);
        cameraDir = vec3(0.0f, 0.0f, 0.0f) - cameraPos;

        frameShader.use();
        frameShader.setInt("ourTexture", 0);

        view = glm::lookAt(cameraPos, cameraPos + cameraDir, cameraUp);
        glm::quat quat = glm::quat_cast(view);
        vec3 angles = eulerAngles(quat);
        pitch = -degrees(angles.x);


        float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
                // positions   // texCoords
                -1.0f, 1.0f, 0.0f, 1.0f,
                -1.0f, -1.0f, 0.0f, 0.0f,
                1.0f, -1.0f, 1.0f, 0.0f,

                -1.0f, 1.0f, 0.0f, 1.0f,
                1.0f, -1.0f, 1.0f, 0.0f,
                1.0f, 1.0f, 1.0f, 1.0f
        };
        glGenVertexArrays(1, &quadVAO);
        glBindVertexArray(quadVAO);

        glGenBuffers(1, &quadVBO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *) 0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *) (2 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glGenFramebuffers(1, &FBO);
        glBindFramebuffer(GL_FRAMEBUFFER, FBO);

        auto &size = Director::getInstance()->getWinSize();
        // create a color attachment texture
        glGenTextures(1, &textureColorbuffer);
        glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, size.width, size.height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);
        // create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
        unsigned int rbo;
        glGenRenderbuffers(1, &rbo);
        glBindRenderbuffer(GL_RENDERBUFFER, rbo);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, size.width, size.height);
        // use a single renderbuffer object for both a depth AND stencil buffer.
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
        // now actually attach it
        // now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            log("ERROR::FRAMEBUFFER:: Framebuffer is not complete!");
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

    }

    void FrameBuffer::draw(const mat4 &transform) {
        glBindFramebuffer(GL_FRAMEBUFFER, FBO);
        glEnable(GL_DEPTH_TEST); // enable depth testing (is disabled for rendering screen-space quad)

        glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glBindVertexArray(VAO);
        glBindTexture(GL_TEXTURE_2D, texture);
        shader.use();
        // use WSAD to control
        view = glm::lookAt(cameraPos, cameraPos + cameraDir, cameraUp);
        shader.setMat4("view", view);

        // left behind
        model = glm::mat4();
        model = glm::translate(model, glm::vec3(-1.0f, 0.5f, 0.0f));
        shader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // right behind
        model = glm::mat4();
        model = glm::translate(model, glm::vec3(1.0f, 0.5f, 0.0f));
        shader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // x center
        model = glm::mat4();
        model = glm::translate(model, glm::vec3(0.0f, 0.5f, 1.0f));
        model = glm::scale(model, vec3(2, 1, 1));
        shader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // plane
        shader.setInt("ourTexture", 0);
        model = glm::mat4();
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::scale(model, vec3(8, 6, 1));
        shader.setMat4("model", model);
        glBindTexture(GL_TEXTURE_2D, texture2);
        glBindVertexArray(planeVAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glBindVertexArray(0);
        // now bind back to default framebuffer and draw a quad plane with the attached framebuffer color texture
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.
        // clear all relevant buffers
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        // set clear color to white (not really necessery actually, since we won't be able to see behind the quad anyways)
        glClear(GL_COLOR_BUFFER_BIT);

        frameShader.use();
        glBindVertexArray(quadVAO);
        glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
        // use the color attachment texture as the texture of the quad plane
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }

    FrameBuffer::~FrameBuffer() {
        glDeleteVertexArrays(1, &quadVAO);
        glDeleteBuffers(1, &quadVBO);

    }
TEST_NODE_IMP_END