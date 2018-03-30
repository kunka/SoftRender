const char* pos_color_frag = R"(

precision lowp float;

varying vec4 v_fragmentColor;

void main()
{
    gl_FragColor = v_fragmentColor;
}
)";
