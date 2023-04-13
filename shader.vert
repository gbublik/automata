#version 110
uniform vec2 resolution;
varying vec4 color;

void main () {
    gl_Position = gl_Vertex;
    color = gl_Color;
}