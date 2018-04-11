//
// Created by huangkun on 2018/4/11.
//

#include "BlinnPhongLighting.h"
#include "stb_image.h"

TEST_NODE_IMP_BEGIN

    BlinnPhongLighting::BlinnPhongLighting() {
        const char *vert = R"(
#version 330 core
layout (location = 0) in vec3 a_position;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec2 a_tex;

// declare an interface block; see 'Advanced GLSL' for what these are.
out VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(a_position, 1.0);
    vs_out.FragPos = vec3(model * vec4(a_position, 1.0));
    vs_out.Normal = mat3(transpose(inverse(model))) * a_normal; // should cal on CPU
//    vs_out.Normal = a_normal;
    vs_out.TexCoords = a_tex;
}
)";

        const char *frag = R"(
#version 330 core
in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} fs_in;

struct Material {
sampler2D diffuse;
float shininess;
};
uniform Material material;

struct Light {
vec3 position;
vec3 ambient;
vec3 diffuse;
vec3 specular;
};
uniform Light light;

uniform vec3 viewPos;

out vec4 FragColor;

void main()
{
    vec3 color = texture(material.diffuse, fs_in.TexCoords).rgb;
    vec3 ambient = light.ambient * color;

    vec3 norm = normalize(fs_in.Normal);
    vec3 lightDir = normalize(light.position - fs_in.FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * color;

    vec3 viewDir = normalize(viewPos - fs_in.FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec;

    FragColor = vec4(color, 1.0);//vec4(diffuse + ambient + specular, 1.0);
}
)";

        shader.loadStr(vert, frag);
        float vertices[] = {
                // positions         // normals         // texcoords
                10.0f, -0.5f, 10.0f, 0.0f, 1.0f, 0.0f, 10.0f, 0.0f,
                -10.0f, -0.5f, 10.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
                -10.0f, -0.5f, -10.0f, 0.0f, 1.0f, 0.0f, 0.0f, 10.0f,

                10.0f, -0.5f, 10.0f, 0.0f, 1.0f, 0.0f, 10.0f, 0.0f,
                -10.0f, -0.5f, -10.0f, 0.0f, 1.0f, 0.0f, 0.0f, 10.0f,
                10.0f, -0.5f, -10.0f, 0.0f, 1.0f, 0.0f, 10.0f, 10.0f
        };

        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) 0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (6 * sizeof(float)));
        glEnableVertexAttribArray(2);

        // texture
        stbi_set_flip_vertically_on_load(true); // flipY

        int width, height, nrChannels;
        unsigned char *data = stbi_load("../res/wood.png", &width, &height, &nrChannels, 0);
        if (!data) {
            log("Failed to load texture");
            return;
        } else {
            log("Texture width = %d, height = %d", width, height);
        }

        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        // set the texture wrapping/filtering options (on the currently bound texture object)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        stbi_image_free(data);

        shader.use();
        shader.setMat4("projection", projection);
        shader.setInt("material.diffuse", 0);
        shader.setFloat("material.shininess", 32.0f);
        shader.setVec3("light.ambient", vec3(0.05f, 0.05f, 0.05f));
        shader.setVec3("light.diffuse", vec3(1.0f, 1.0f, 1.0f));
        shader.setVec3("light.specular", vec3(0.3f, 0.3f, 0.3f));
        shader.setVec3("light.position", vec3(0, 1, 0));

        glEnable(GL_DEPTH_TEST);
    }

    void BlinnPhongLighting::draw(const mat4 &transform) {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        view = glm::lookAt(cameraPos, cameraPos + cameraDir, cameraUp);
        shader.use();
        shader.setMat4("view", view);

        // floor
        model = glm::mat4();
        shader.setMat4("model", model);
        shader.setVec3("viewPos", cameraPos);

        glBindVertexArray(VAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }

    BlinnPhongLighting::~BlinnPhongLighting() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    }
TEST_NODE_IMP_END