#version 400
in      vec4 a_Vertex;
out     vec4 v_Color;
uniform mat4 u_proj;
//uniform mat4 u_model;
uniform mat4 u_view;


void main(void)
{
    //mat4 LookAt(float eyex, float eyey, float eyez, float atx, float aty, float atz,float upx, float upy, float upz);

    gl_Position = u_proj * u_view * a_Vertex;
    v_Color     = vec4(1.0);
}
