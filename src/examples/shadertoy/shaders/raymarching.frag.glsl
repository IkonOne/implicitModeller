#version 330 core

uniform vec2 u_resolution;
uniform mat4 u_mvp;
uniform float u_time;
uniform vec2 u_mouse;

#define MAX_STEPS 100
#define MAX_DIST 100.0
#define SURF_DIST 0.01

float getDist(vec3 p) {
    vec4 s = vec4(0, 1, 6, 1);
    float theta = 2*u_time;
    float decay = 1.0/(1+mod(u_time, 8.0)/8.0);
    s.y += abs(cos(1.5*theta)*decay)*2;
    float sphereDist = length(p-s.xyz)-s.w;

    float planeDist = p.y+0.0;

    float d = min(sphereDist, planeDist);
    return d;
}

vec3 getNormal(vec3 p) {
    float d = getDist(p);
    vec2 e = vec2(0.1, 0);
    vec3 n = vec3(
        getDist(p-e.xyy),
        getDist(p-e.yxy),
        getDist(p-e.yyx)
    );

    return normalize(d-n);
}


float rayMarch(in vec3 ro, in vec3 rd) {
    float dO = 0;
    for (int i = 0; i < MAX_STEPS; i++) {
        vec3 p = ro + rd*dO;
        float dS = getDist(p);
        dO += dS;
        if (dO > MAX_DIST || dS < SURF_DIST) break;
    }

    return dO;
}

float getLight(vec3 p) {
    vec3 lightPos = vec3(-2, 5, 4);
    // float theta = 0.4*u_time;
    // lightPos.xz += vec2(sin(theta), cos(theta))*2.0;
    vec3 l = normalize(lightPos-p);
    vec3 n = getNormal(p);

    float dif = clamp(dot(n, l), 0.0, 1.0);

    float d = rayMarch(p+n*SURF_DIST*2, l);
    if (d < length(l-p)) dif *= 0.8;

    return dif;
}

void main() {
    vec2 uv = (gl_FragCoord.xy-0.5*u_resolution) / u_resolution.y;
    vec3 color = vec3(0);

    vec3 ro = vec3(0.0, 1.0, 0.0);
    vec3 rd = normalize(vec3(uv.x, uv.y, 1.0));

    vec2 mouse = (u_mouse-0.5*u_resolution) / u_resolution.y;
    vec3 rotZ = vec3(-sin(mouse.x), 0.0, cos(mouse.x));
    vec3 rotX = vec3(0.0, sin(mouse.y)*1.3, cos(mouse.y)*1.3);
    ro -= 1.5 * (rotZ + rotX);

    float d = rayMarch(ro, rd);

    vec3 p = ro + rd * d;

    // color = vec3(d/6.0);
    // color = getNormal(p);

    float dif = getLight(p);
    color = vec3(dif, dif*0.2, 0.4);

    gl_FragColor = vec4(color, 1.0);
}