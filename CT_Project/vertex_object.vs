#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 FragPos;  
out vec3 Normal;

void main()
{
    gl_Position = projection*view*model*vec4(aPos,1.0f);
    
    FragPos = vec3(model * vec4(aPos, 1.0));// 点的位置
    //Normal = aNormal;//因为面要进行转动，所以这样的话， 随着面的转动，法向量不会转， 效果也会很怪异

    //Normal = vec3(projection*view*model * vec4(aNormal, 1.0));//这样 暗部 才会有透视的情况，才会看起来正常一点
    Normal = mat3(transpose(inverse(model))) * aNormal;
}
 