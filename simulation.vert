#version 400
in      vec4 a_Vertex;
in      vec3 a_initVelocity;
in      vec3 a_centerMass;
in      vec3 a_angularVelocity;

out     vec4 v_Color;

uniform mat4  u_proj;
uniform mat4  u_model;
uniform mat4  u_view;
uniform float u_time;

void main(void)
{
    //move exploded vertices
    vec3 t         = vec3(u_time*u_time,u_time,1);
    mat3 transMat;

    transMat[0] = vec3(0,-9.8,0);
    transMat[1] = a_initVelocity;
    transMat[2] = a_centerMass.xyz;//vertex is moved in relation to the center of mass to preserve shape

    vec4 position = u_model * vec4(transMat * t,1);

    //compute offset verctor from center of mass to vertex
    vec3 vert_to_com = a_Vertex.xyz - a_centerMass.xyz;

    //hacky air drag atan increasing and bounded above so it doesn't make particles move backwards
    if(atan(u_time) - atan(u_time - 0.01) < 0.001){
        position.xz = vec2(0.2*atan(1000)*a_initVelocity.x + a_Vertex.x,0.2*atan(1000)*a_initVelocity.z + a_Vertex.z);
    }else{
        position.xz = vec2(0.2*atan(u_time)*a_initVelocity.x + a_Vertex.x,0.2*atan(u_time)*a_initVelocity.z + a_Vertex.z);
    }


    if(position.y <= 0.0){
        position.y =0;
        //v_Color     = vec4(1,0,0,1);
        v_Color     = vec4(1.0);
    }else{
        v_Color     = vec4(1.0);
    }

    //after getting the position of the center of mass we can get the actual
    //position of the vertex relative to the projectile's center of mass
    position.xyz += vert_to_com.xyz;

    gl_Position = u_proj * u_view * position;



}
