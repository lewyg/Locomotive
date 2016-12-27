#version 330 core

layout (location = 0) in vec3 position; // The position variable has attribute position 0

out vec3 ourColor; // Output a color to the fragment shader

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view *  model * vec4(position, 1.0f);
    ourColor = vec3(1.0, 1.0, 1.0); // Set ourColor to the input color we got from the vertex data
}
