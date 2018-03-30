//
//  shaders.h
//  gl
//
//  Created by huangkun on 15/11/2017.
//  Copyright © 2017 kk. All rights reserved.
//

#ifndef shaders_h
#define shaders_h

const static char *shader_position_color_size_vert = R"(
#version 330 core
layout (location = 0) in vec3 a_position;
layout (location = 1) in vec4 a_color;
float a_size;

void main()
{
//    gl_Position = MVPMatrix * a_position;
    gl_Position =  a_position;
    gl_PointSize = a_size;
    outColor = vec4(a_color);
}
)";

const static char *shader_position_color_size_frag = R"(
#version 330 core
in vec4 outColor;
out vec4 FragColor;

void main()
{
    FragColor = outColor;
}
)";

const static char *shader_position_color_vert = R"(
#version 330 core
layout (location = 0) in vec3 a_position;
layout (location = 1) in vec3 a_color;
out vec4 outColor;

void main()
{
//    gl_Position = MVPMatrix * a_position;
    gl_Position = vec4(a_position, 1.0);
    outColor = vec4(a_color, 1.0);
}
)";

const static char *shader_position_color_frag = R"(
#version 330 core
in vec4 outColor;
out vec4 FragColor;

void main()
{
    FragColor = outColor;
}
)";

#endif /* shaders_h */
