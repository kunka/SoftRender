//
// Created by huangkun on 2018/4/27.
//

#include "GeometryShader2.h"

TEST_NODE_IMP_BEGIN

    GeometryShader2::GeometryShader2() {
        const char *vert = R"(
#version 330 core
layout (location = 0) in vec3 a_position;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec2 a_texCoords;
out vec2 TexCoords;
out vec3 FragPos;
out vec3 Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(a_position, 1.0);
    Normal = a_normal;
    TexCoords = a_texCoords;
    FragPos = vec3(model * vec4(a_position, 1.0));
}
)";

        const char *normal_vert = R"(
#version 330 core
layout (location = 0) in vec3 a_position;
layout (location = 1) in vec3 a_normal;
out vec3 Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(a_position, 1.0);
    mat3 normalMatrix = mat3(transpose(inverse(view * model)));
    Normal = normalize(vec3(projection * vec4(normalMatrix * a_normal, 1.0)));
}
)";

        const char *gem = R"(
#version 330 core
layout (triangles) in;
layout (line_strip, max_vertices = 2) out;
//layout (triangle_strip, max_vertices = 3) out;
in vec3 Normal[];

void main()
{
    vec4 p1 = gl_in[0].gl_Position;
    vec4 p2 = gl_in[1].gl_Position;
    vec4 p3 = gl_in[2].gl_Position;
    gl_Position = p1;
    EmitVertex();
    gl_Position = p1 + vec4(Normal[0] * 0.4, 0);
    EmitVertex();

    EndPrimitive();
}
)";

        const char *normal_frag = R"(
#version 330 core
out vec4 FragColor;

void main()
{
    FragColor = vec4(1.0, 1.0, 0.0, 1.0);
}
)";

        const char *frag = R"(
#version 330 core
out vec4 FragColor;
in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

void main()
{
    FragColor = texture(texture_diffuse1, TexCoords);
}
)";
        shader.loadStr(vert, frag);
        normalShader.loadStr(vert, normal_frag, gem);
    }

    void GeometryShader2::draw(const mat4 &transform) {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        view = glm::lookAt(cameraPos, cameraPos + cameraDir, cameraUp);

        shader.use();
        shader.setMat4("projection", projection);
        shader.setMat4("view", view);
        model = glm::mat4();
        model = glm::translate(model, vec3(0, -8, -12));
        shader.setMat4("model", model);
        modelObject->draw(shader);


        normalShader.use();
        normalShader.setMat4("projection", projection);
        normalShader.setMat4("view", view);
        model = glm::mat4();
        model = glm::translate(model, vec3(0, -8, -12));
        normalShader.setMat4("model", model);
        modelObject->draw(normalShader);
    }

    GeometryShader2::~GeometryShader2() {
    }

TEST_NODE_IMP_END