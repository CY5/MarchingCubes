#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec4 NormalWorldSpace;

out vec2 v_TexCoord;
out vec3 v_lightDirectionCamSpace;
out vec3 v_normalCamSpace;
out vec3 v_EyeDir;

uniform mat4 u_MVP;
uniform mat4 u_Model;
uniform mat4 u_View;
//uniform mat4 u_Proj;

void main()
{
   vec3 lightPositionWorldspace = vec3(-40.0,-30.0,-120.0);
   gl_Position = u_MVP * position;
   //gl_Position = u_MVP * position;
   
   // Vector that goes from the vertex to the camera, in camera space.
   // In camera space, the camera is at the origin (1.0, 1.0, 50.0)
   vec3 vertPosCamSpace = (u_View * u_Model * position).xyz;
   vec3 eyeDirCamSpace  = vec3(1.0, 1.0, 50.0) - vertPosCamSpace;

   // Vector that goes from the vertex to the light, in camera space. M is ommited because it's identity.
   vec3 lightPositionCamSpace = ( u_View  * vec4(lightPositionWorldspace,1.0)).xyz;
   v_lightDirectionCamSpace = lightPositionCamSpace + eyeDirCamSpace;

   // Normal of the the vertex, in camera space
   v_normalCamSpace = ( u_View * u_Model * NormalWorldSpace).xyz;

   v_EyeDir = normalize(eyeDirCamSpace);

   v_TexCoord = texCoord;
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;
uniform sampler2D u_Texture;

in vec2 v_TexCoord;
in vec3 v_lightDirectionCamSpace;
in vec3 v_normalCamSpace;
in vec3 v_EyeDir;

void main()
{
   vec4 lightColor = normalize(vec4(0.8,0.4,0.5,1.0));
   vec4 texColor = normalize(texture(u_Texture, v_TexCoord));
   vec3 n = normalize(v_normalCamSpace);
   vec3 l = normalize(v_lightDirectionCamSpace);
   float cosTheta = clamp(dot(n,l),0.0, 1.0);
   float vdl= dot(l, l);
   vec3 R = reflect(-l,n);
   float cosAlpha = clamp( dot( v_EyeDir,R ), 0,1 );
   float LightPower = 10.0;
   vec4 mA = vec4(0.1,0.12,0.1,1.0) * texColor;
   color = mA+
            (texColor * lightColor * LightPower * cosTheta)/(vdl*vdl)+
            // Specular : reflective highlight, like a mirror
            (texColor * lightColor * LightPower * pow(cosAlpha,5.0) )/ (vdl*vdl);
   //color = vec4(0.0,0.0,1.0,1.0);
};