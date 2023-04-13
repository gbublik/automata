uniform sampler2D SourceTexture;
uniform vec2 shift;
uniform vec2 resolution;
uniform vec2 size;
varying vec4 color;

vec4 currentColor;

vec2 position;
float deltax;
float deltay;



void main(void) {
    //resolution *= zoom;
    position = gl_FragCoord.xy / resolution.xy + (shift / resolution.xy);
    deltax = 1.0/resolution.x;
    deltay = 1.0/resolution.y;

    gl_FragColor = texture2D(SourceTexture, position);
}