#version 330 core

uniform vec2 center;
uniform float radius;
uniform vec2 iRes;

out vec4 fragColor;

void main()
{
    vec2 pos = gl_FragCoord.xy - center;
    // Calculate the distance from the fragment to the center of the circle
    float dist = sqrt(pos.x*pos.x + pos.y*pos.y);
    if (dist < 100.0) 
        fragColor = vec4(1.0, 1.0, 0.0, 1.0);
    else
        fragColor = vec4(0.0);
}
