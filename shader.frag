uniform sampler2D SourceTexture;
uniform vec2 resolution;

float deltax;
float deltay;
vec2 position;
varying vec4 color;

float get(int x, int y) {
    vec4 p = texture2D(SourceTexture, position + vec2( deltax * x, deltay * y));
    return p.a;
}

void main(void) {
    position = gl_FragCoord.xy / resolution.xy;
    deltax = 1.0/resolution.x;
    deltay = 1.0/resolution.y;

    float sum = get(1, 1) + get(-1, 1) + get(-1, -1) + get(1, -1) + get(0, 1) + get(0, -1) + get(-1, 0) + get(1, 0);
    float center = get(0, 0);
    float koef=0.0;

    vec4 currentColor = vec4(1, 1, 1, 1);

    if(center == 1) // если единица - то живая, если ноль, то мёртвая
    {
        if(sum > 3.5 || sum < 1.5) koef = 0.0;
        else {
            koef = 1.0;

        }
    }
    else
    {
        if(sum > 2.5 && sum < 3.5) {
            koef = 1.0; // если Sum==3, то оживаем

        }
    }


    gl_FragColor = currentColor * koef;
}