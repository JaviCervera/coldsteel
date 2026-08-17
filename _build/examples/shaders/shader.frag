#version 110

uniform sampler2D tex;
uniform float uTime;
uniform vec4 mDiffuse;
uniform float mParam1;

varying vec2 vTexCoord;

void main()
{
    float pulse = 0.5 + 0.5 * sin(uTime * mParam1 * 2.0);
    vec4 texel = texture2D(tex, vTexCoord);
    gl_FragColor = texel * mDiffuse * pulse;
}
