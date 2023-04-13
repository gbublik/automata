#version 120

uniform sampler2D SourceTexture;
uniform vec2 resolution;

vec4 white = vec4(1, 1, 1, 1);
vec4 currentColor;

float deltax;
float deltay;
vec2 position;

bool[15] horizontalLine = bool[15](
0, 0, 0, 0, 0,
0, 1, 1, 1, 0,
0, 0, 0, 0, 0
);
bool[15] verticalLine = bool[15](
0, 0, 0,
0, 1, 0,
0, 1, 0,
0, 1, 0,
0, 0, 0
);
bool[16] square = bool[16](
0, 0, 0, 0,
0, 1, 1, 0,
0, 1, 1, 0,
0, 0, 0, 0
);
bool[30] horizontalBeehive = bool[30](
0, 0, 0, 0, 0, 0,
0, 0, 1, 1, 0, 0,
0, 1, 0, 0, 1, 0,
0, 0, 1, 1, 0, 0,
0, 0, 0, 0, 0, 0
);
bool[30] verticalBeehive = bool[30](
0, 0, 0, 0, 0,
0, 0, 1, 0, 0,
0, 1, 0, 1, 0,
0, 1, 0, 1, 0,
0, 0, 1, 0, 0,
0, 0, 0, 0, 0
);
bool[36] loaf1 = bool[36](
0, 0, 0, 0, 0, 0,
0, 0, 1, 0, 0, 0,
0, 1, 0, 1, 0, 0,
0, 1, 0, 0, 1, 0,
0, 0, 1, 1, 0, 0,
0, 0, 0, 0, 0, 0
);
bool[36] loaf2 = bool[36](
0, 0, 0, 0, 0, 0,
0, 0, 0, 1, 0, 0,
0, 0, 1, 0, 1, 0,
0, 1, 0, 0, 1, 0,
0, 0, 1, 1, 0, 0,
0, 0, 0, 0, 0, 0
);
bool[36] loaf3 = bool[36](
0, 0, 0, 0, 0, 0,
0, 0, 1, 1, 0, 0,
0, 1, 0, 0, 1, 0,
0, 0, 1, 0, 1, 0,
0, 0, 0, 1, 0, 0,
0, 0, 0, 0, 0, 0
);
bool[36] loaf4 = bool[36](
0, 0, 0, 0, 0, 0,
0, 0, 1, 1, 0, 0,
0, 1, 0, 0, 1, 0,
0, 1, 0, 1, 0, 0,
0, 0, 1, 0, 0, 0,
0, 0, 0, 0, 0, 0
);

bool[36] loaft = bool[36](
0, 0, 0, 0, 0, 0,
0, 0, 1, 1, 0, 0,
0, 1, 0, 0, 1, 0,
0, 1, 0, 1, 0, 0,
0, 0, 1, 0, 0, 0,
0, 0, 0, 0, 0, 0
);

bool get(int x, int y) {
    vec4 p = texture2D(SourceTexture, position + vec2(deltax * x, deltay * y));
    return p.a > 0.5;
}

bool[15] getMatix15(int x1, int x2, int y1, int y2) {
    bool[15] outputMatrix;
    int i = 0;
    for(int y = y1; y <= y2; y++) {
        for(int x = x1; x <= x2; x++) {
            outputMatrix[i] = get(x, y);
            i++;
        }
    }
    return outputMatrix;
}
bool[16] getMatix16(int x1, int x2, int y1, int y2) {
    bool[16] outputMatrix;
    int i = 0;
    for(int y = y1; y <= y2; y++) {
        for(int x = x1; x <= x2; x++) {
            outputMatrix[i] = get(x, y);
            i++;
        }
    }
    return outputMatrix;
}
bool[30] getMatix30(int x1, int x2, int y1, int y2) {
    bool[30] outputMatrix;
    int i = 0;
    for(int y = y1; y <= y2; y++) {
        for(int x = x1; x <= x2; x++) {
            outputMatrix[i] = get(x, y);
            i++;
        }
    }
    return outputMatrix;
}
bool[36] getMatix36(int x1, int x2, int y1, int y2) {
    bool[36] outputMatrix;
    int i = 0;
    for(int y = y1; y <= y2; y++) {
        for(int x = x1; x <= x2; x++) {
            outputMatrix[i] = get(x, y);
            i++;
        }
    }
    return outputMatrix;
}

bool isHLine() {
    if (getMatix15(-1, 3, -1, 1) == horizontalLine) {
        return true;
    }
    if (getMatix15(-2, 2, -1, 1) == horizontalLine) {
        return true;
    }
    if (getMatix15(-3, 1, -1, 1) == horizontalLine) {
        return true;
    }
    return false;
}
bool isVLine() {

    if (getMatix15(-1, 1, -1, 3) == verticalLine) {
        return true;
    }
    if (getMatix15(-1, 1, -2, 2) == verticalLine) {
        return true;
    }
    if (getMatix15(-1, 1, -3, 1) == verticalLine) {
        return true;
    }

    return false;
}
bool isSquare() {
    if (getMatix16(-1, 2, -1, 2) == square) {
        return true;
    }
    if (getMatix16(-2, 1, -1, 2) == square) {
        return true;
    }
    if (getMatix16(-1, 2, -2, 1) == square) {
        return true;
    }
    if (getMatix16(-2, 1, -2, 1) == square) {
        return true;
    }
    return false;
}
bool isBeehive() {
    if (getMatix30(-2, 3, -3, 1) == horizontalBeehive) {
        return true;
    }
    if (getMatix30(-3, 2, -3, 1) == horizontalBeehive) {
        return true;
    }
    if (getMatix30(-1, 4, -2, 2) == horizontalBeehive) {
        return true;
    }
    if (getMatix30(-4, 1, -2, 2) == horizontalBeehive) {
        return true;
    }
    if (getMatix30(-2, 3, -1, 3) == horizontalBeehive) {
        return true;
    }
    if (getMatix30(-3, 2, -1, 3) == horizontalBeehive) {
        return true;
    }

    if (getMatix30(-2, 2, -4, 1) == verticalBeehive) {
        return true;
    }
    if (getMatix30(-1, 3, -3, 2) == verticalBeehive) {
        return true;
    }
    if (getMatix30(-3, 1, -3, 2) == verticalBeehive) {
        return true;
    }
    if (getMatix30(-1, 3, -2, 3) == verticalBeehive) {
        return true;
    }
    if (getMatix30(-3, 1, -2, 3) == verticalBeehive) {
        return true;
    }
    if (getMatix30(-2, 2, -1, 4) == verticalBeehive) {
        return true;
    }

    return false;
}
bool isLoaf1() {
    if (getMatix36(-2, 3, -1, 4) == loaf1) {
        return true;
    }
    if (getMatix36(-1, 4, -2, 3) == loaf1) {
        return true;
    }
    if (getMatix36(-3, 2, -2, 3) == loaf1) {
        return true;
    }
    if (getMatix36(-1, 4, -3, 2) == loaf1) {
        return true;
    }
    if (getMatix36(-2, 3, -4, 1) == loaf1) {
        return true;
    }
    if (getMatix36(-3, 2, -4, 1) == loaf1) {
        return true;
    }
    if (getMatix36(-4, 1, -3, 2) == loaf1) {
        return true;
    }
    return false;
}
bool isLoaf2() {
    if (getMatix36(-3, 2, -4, 1) == loaf2) {
        return true;
    }
    if (getMatix36(-2, 3, -2, 3) == loaf2) {
        return true;
    }
    if (getMatix36(-4, 1, -2, 3) == loaf2) {
        return true;
    }
    if (getMatix36(-1, 4, -3, 2) == loaf2) {
        return true;
    }
    if (getMatix36(-4, 1, -3, 2) == loaf2) {
        return true;
    }
    if (getMatix36(-2, 3, -4, 1) == loaf2) {
        return true;
    }
    if (getMatix36(-3, 2, -1, 4) == loaf2) {
        return true;
    }
    return false;
}
bool isLoaf3() {
    if (getMatix36(-2, 3, -1, 4) == loaf3) {
        return true;
    }
    if (getMatix36(-3, 2, -1, 4) == loaf3) {
        return true;
    }
    if (getMatix36(-1, 4, -2, 3) == loaf3) {
        return true;
    }
    if (getMatix36(-4, 1, -2, 3) == loaf3) {
        return true;
    }
    if (getMatix36(-2, 3, -3, 2) == loaf3) {
        return true;
    }
    if (getMatix36(-4, 1, -3, 2) == loaf3) {
        return true;
    }
    if (getMatix36(-3, 2, -4, 1) == loaf3) {
        return true;
    }
    return false;
}
bool isLoaf4() {
    if (getMatix36(-2, 3, -1, 4) == loaf4) {
        return true;
    }
    if (getMatix36(-3, 2, -1, 4) == loaf4) {
        return true;
    }
    if (getMatix36(-1, 4, -2, 3) == loaf4) {
        return true;
    }
    if (getMatix36(-4, 1, -2, 3) == loaf4) {
        return true;
    }
    if (getMatix36(-1, 4, -3, 2) == loaf4) {
        return true;
    }
    if (getMatix36(-3, 2, -3, 2) == loaf4) {
        return true;
    }
    if (getMatix36(-2, 3, -4, 1) == loaf4) {
        return true;
    }
    return false;
}

void main() {
    //resolution *= zoom;
    position = gl_FragCoord.xy / resolution.xy;
    deltax = 1.0/resolution.x;
    deltay = 1.0/resolution.y;

    gl_FragColor = texture2D(SourceTexture, position);
    if (gl_FragColor.a > 0) {
        if (isHLine()) {
            gl_FragColor = vec4(1, 0, 0, 1);
        } else if (isVLine()) {
            gl_FragColor = vec4(1, 0.3, 0.2, 1);
        } else if (isSquare()) {
            gl_FragColor = vec4(0, 0, 1, 1);
        } else if (isBeehive()) {
            gl_FragColor = vec4(1, 1, 0, 1);
        } else if (isLoaf1()) {
            gl_FragColor = vec4(0, 1, 0, 1);
        } else if (isLoaf2()) {
            gl_FragColor = vec4(0.2, 1, 0.3, 1);
        } else if (isLoaf3()) {
            gl_FragColor = vec4(0.5, 1, 0.5, 1);
        } else if (isLoaf4()) {
            gl_FragColor = vec4(0.1, 1, 0.1, 1);
        }
    }
}