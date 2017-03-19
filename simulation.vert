#version 400
in      vec4 a_Vertex;
in      vec3 a_initVelocity;
in      vec3 a_centerMass;
in      vec3 a_angularVelocity;

out     vec4 v_Color;

uniform mat4  u_proj;
uniform mat4 u_model;
uniform mat4  u_view;
uniform float u_time;

void main(void)
{
    //move exploded vertices
    vec3 t         = vec3(u_time*u_time,u_time,1);
    mat3 transMat;

    transMat[0] = vec3(0,-9.8,0);
    transMat[1] = a_initVelocity;
    transMat[2] = a_Vertex.xyz;

    vec4 position = u_model * vec4(transMat * t,1);

    //hacky air drag atan increasing and counded above so it doesn't make particles move backwards
    position.xz = vec2(0.2*atan(u_time)*a_initVelocity.x + a_Vertex.x,0.2*atan(u_time)*a_initVelocity.z + a_Vertex.z);

    if(position.y <= 0.0){
        position.y =0;
        v_Color     = vec4(1,0,0,1);
    }else{
        v_Color     = vec4(1.0);
    }

    gl_Position = u_proj * u_view * position;



}
