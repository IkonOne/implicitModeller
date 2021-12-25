#version 330 core

uniform vec2 u_resolution;
uniform mat4 u_mvp;
uniform float u_time;

float circle(in vec2 _st, in float _radius) {
    return length(_st)-_radius;
}

void main() {
    vec2 st = gl_FragCoord.xy / u_resolution - vec2(0.5);
    vec3 color = vec3(0.0);

    // float d = pow(distance(st, vec2(0.4)), distance(st, vec2(0.6)));
    // float val = step(d, 0.1);
    float val = circle(st, 0.2);
    val = fract(-val);
    color = vec3(val, val*0.2, 0.1);

    // vec2 bl = step(vec2(0.1), st);
    // color = vec3(bl.x * bl.y);

    // vec2 tr = step(vec2(0.1), 1-st);
    // color *= vec3(tr.x * tr.y);

    gl_FragColor = vec4(color, 1.0);
}
