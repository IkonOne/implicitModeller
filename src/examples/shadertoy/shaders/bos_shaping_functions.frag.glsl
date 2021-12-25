#version 330 core

uniform vec2 u_resolution;
uniform mat4 u_mvp;
uniform float u_time;

#define PI 3.14159265359

void main() {
    vec2 st = gl_FragCoord.xy / u_resolution;

    float val;
    float x = st.x;
    // val = pow(x, 1);
    // val = step(0.5, x);
    // val = smoothstep(0.1, 0.9, x);
    // val = smoothstep(0.49,0.5,x) - smoothstep(0.5,0.51,x);
    // val = mod(x, 0.7);
    // val = fract(x);
    // val = fract(pow(2*x, 5));
    // val = floor(x*3);
    // val = sign(sin(x*40));
    // val = abs(sin(x*40));
    // val = clamp(x, 0.4, 0.6);
    // val = min(0.8, x);
    // val = max(sin(u_time), x);
    // val = sin(x*PI*15)*0.5 + 0.5;
    // val = sin(u_time)*0.5 + 0.5;
    val = smoothstep(sin(u_time)*0.25+0.25, cos(u_time)*0.25+0.75, x);

    vec3 color = vec3(val, val*0.5, 0.2);
    // vec3 color = vec3(1, 0.5, 0.2);
    gl_FragColor = vec4(color, 1.0);
}