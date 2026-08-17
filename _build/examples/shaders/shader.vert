#version 110

uniform mat4 mWorldViewProj;
uniform mat4 uMat;

varying vec2 vTexCoord;

void main()
{
    gl_Position = mWorldViewProj * gl_Vertex;
    vTexCoord = (uMat * vec4(gl_MultiTexCoord0.xy, 0.0, 1.0)).xy;
}
