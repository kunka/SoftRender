//
// Created by huangkun on 04/04/2018.
//

#include "ModelLoad.h"

TEST_NODE_IMP_BEGIN

    ModelLoad::ModelLoad() {
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

        const char *light_vert = R"(
#version 330 core
layout (location = 0) in vec3 a_position;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(a_position, 1.0);
}
)";

        const char *light_frag = R"(
#version 330 core
out vec4 FragColor;
uniform vec3 lightColor;

void main()
{
    FragColor = vec4(lightColor, 1.0f);
}
)";

        const char *frag = R"(
#version 330 core
out vec4 FragColor;
in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

struct PointLight {
       vec3 position;
       float constant;
       float linear;
       float quadratic;
       vec3 ambient;
       vec3 diffuse;
       vec3 specular;
   };
#define NR_POINT_LIGHTS 2
uniform PointLight pointLights[NR_POINT_LIGHTS];


struct SpotLight {
vec3 position;
vec3 direction;
float cutOff;
float outerCutOff;

vec3 ambient;
vec3 diffuse;
vec3 specular;

float constant;
float linear;
float quadratic;
};
#define NR_SPOT_LIGHTS 1
uniform SpotLight spotLights[NR_SPOT_LIGHTS];


uniform vec3 viewPos;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

vec3 calcPointLight(PointLight light, vec3 normal, vec3 FragPos, vec3 viewDir);
vec3 calcSpotLight(SpotLight light, vec3 norm, vec3 FragPos, vec3 viewDir);

void main()
{
//    FragColor = vec4(vec3(texture(texture_diffuse1, TexCoords)), 1.0);

    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 result;

    // phase 2: Point lights
    for(int i = 0; i < NR_POINT_LIGHTS; i++)
        result += calcPointLight(pointLights[i], norm, FragPos, viewDir);

    // phase 3: Spot light
    for(int i = 0; i < NR_SPOT_LIGHTS; i++)
        result += calcSpotLight(spotLights[i], norm, FragPos, viewDir);

    FragColor = vec4(result, 1.0);
}

vec3 calcPointLight(PointLight light, vec3 norm, vec3 FragPos, vec3 viewDir)
{
    float distance = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    vec3 ambient = light.ambient * vec3(texture(texture_diffuse1, TexCoords));

    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(texture_diffuse1, TexCoords));

    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = light.specular * spec * vec3(texture(texture_specular1, TexCoords));

    return attenuation*(diffuse + ambient + specular);
}


vec3 calcSpotLight(SpotLight light, vec3 norm, vec3 FragPos, vec3 viewDir)
{
     // check if lighting is inside the spotlight cone
    vec3 lightDir = normalize(light.position - FragPos);
    float theta = dot(lightDir, normalize(-light.direction));

    if(theta > light.outerCutOff)
    {
        // soft edges
        float intensity = 1.0;
        if(theta < light.cutOff)
        {
            float epsilon = light.outerCutOff - light.cutOff ;
            intensity = clamp((light.outerCutOff - theta) / epsilon, 0.0, 1.0);
        }

        float distance = length(light.position - FragPos);
        float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

        vec3 ambient = light.ambient * vec3(texture(texture_diffuse1, TexCoords));

        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = light.diffuse * diff * vec3(texture(texture_diffuse1, TexCoords));

        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
        vec3 specular = light.specular * spec * vec3(texture(texture_specular1, TexCoords));

        return attenuation*(diffuse + specular) * intensity + ambient;
    }
    else
    {
        // use ambient light
        vec3 ambient = light.ambient * vec3(texture(texture_diffuse1, TexCoords));
        return ambient;
    }
}

)";
        shader.loadStr(vert, frag);
        lightShader.loadStr(light_vert, light_frag);

        float vertices[] = {
                // positions          // normals           // texture coords
                -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
                0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
                0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
                0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
                -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
                -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

                -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
                0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
                0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
                -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
                -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

                -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
                -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
                -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
                -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
                -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
                -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

                0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
                0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
                0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
                0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
                0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
                0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

                -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
                0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
                0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
                0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
                -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
                -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,

                -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
                0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
                0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
                0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
                -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
                -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f
        };
        // point light obj
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glGenVertexArrays(1, &lightVAO);
        glBindVertexArray(lightVAO);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) 0);
        glEnableVertexAttribArray(0);

        glBindVertexArray(0);

        modelObject = new Model();
        modelObject->load("../res/nanosuit/nanosuit.obj");
    }

    void ModelLoad::draw(const mat4 &transform) {
        glEnable(GL_DEPTH_TEST);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        view = glm::lookAt(cameraPos, cameraPos + cameraDir, cameraUp);

        static glm::vec3 pointLightPositions[] = {
                glm::vec3(-3.0f, 5.0f, 2.0f),
                glm::vec3(3.0f, 5.0f, 2.0f),
        };
        static glm::vec3 pointLightColors[] = {
                glm::vec3(1.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 1.0f, 0.0f),
        };

        glBindVertexArray(lightVAO);
        for (int i = 0; i < 2; i++) {
            // light obj
            vec3 lightColor = pointLightColors[i];
            vec3 lightPos = pointLightPositions[i];
            lightShader.use();
            model = glm::mat4();
            model = glm::translate(model, lightPos);
            model = glm::translate(model, vec3(0, 0, -10));
            model = glm::scale(model, glm::vec3(0.2, 0.2f, 0.2f));
            lightShader.setMat4("projection", projection);
            lightShader.setMat4("model", model);
            lightShader.setMat4("view", view);
            lightShader.setVec3("lightColor", lightColor);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }


        shader.use();

        std::string name;
        for (int i = 0; i < 2; i++) {
            name = formatString("pointLights[%d].position", i);
            shader.setVec3(name, pointLightPositions[i]);
            name = formatString("pointLights[%d].ambient", i);
            shader.setVec3(name, vec3(0.5f, 0.5f, 0.5f) * pointLightColors[i]);
            name = formatString("pointLights[%d].diffuse", i);
            shader.setVec3(name, vec3(1.0f, 1.0f, 1.0f) * pointLightColors[i]);
            name = formatString("pointLights[%d].specular", i);
            shader.setVec3(name, vec3(1.0f, 1.0f, 1.0f) * pointLightColors[i]);
            name = formatString("pointLights[%d].constant", i);
            shader.setFloat(name, 1.0f);
            name = formatString("pointLights[%d].linear", i);
            shader.setFloat(name, 0.09f);
            name = formatString("pointLights[%d].quadratic", i);
            shader.setFloat(name, 0.032f);
        }
        shader.setVec3("spotLights[0].ambient", vec3(0.5f, 0.5f, 0.5f));
        shader.setVec3("spotLights[0].diffuse", vec3(1.0f, 1.0f, 1.0f)); // darken the light a bit to fit the scene
        shader.setVec3("spotLights[0].specular", vec3(1.0f, 1.0f, 1.0f));
        shader.setFloat("spotLights[0].constant", 1.0f);
        shader.setFloat("spotLights[0].linear", 0.09f);
        shader.setFloat("spotLights[0].quadratic", 0.032f);
        shader.setFloat("spotLights[0].cutOff", glm::cos(glm::radians(9.0f)));
        shader.setFloat("spotLights[0].outerCutOff", glm::cos(glm::radians(12.0f)));
        shader.setVec3("spotLights[0].position", cameraPos);
        shader.setVec3("spotLights[0].direction", cameraDir);
        shader.setVec3("viewPos", cameraPos);


        shader.setMat4("projection", projection);
        shader.setMat4("view", view);
        shader.setVec3("viewPos", cameraPos);
        model = glm::mat4();
        model = glm::translate(model, vec3(0, -10, -10));
        shader.setMat4("model", model);
        modelObject->draw(shader);

        glDisable(GL_DEPTH_TEST);
    }

    ModelLoad::~ModelLoad() {
        glDeleteVertexArrays(1, &lightVAO);
        glDeleteBuffers(1, &VBO);
        delete modelObject;
    }
TEST_NODE_IMP_END