#version 450
#pragma shader_stage(fragment)

layout(location = 0) out vec4 output_color;

void main() {
    output_color = vec4(
        0.788235294,
        0.31764705882,
        0.96078431372,
        1.0
    );
}
