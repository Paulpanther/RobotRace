precision mediump float;

uniform vec2 u_resolution;
varying vec2 v_position;

void main() {
    vec2 pos = v_position;

    vec2 origin = vec2(0.0, 0.0);
    float width = .1;

    float d = distance(pos, origin);
    float streched_width = d * 1. / width;
    float streched_pattern = streched_width * 3.0;
    float repeat = mod(streched_pattern, 3.);
    float color = floor(repeat) / 2.;
    gl_FragColor = vec4(vec3(color), 1.0);
}
