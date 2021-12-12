#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 2) uniform sampler2D texSampler;
layout(location = 0) in vec3 fragNormal;
layout(location = 1) in vec2 fragTexCoord;
layout(location = 2) in vec4 position;

layout(location = 0) out vec4 outColor;

layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
} ubo;

layout(binding = 1) uniform LightUBO {
    vec3 pos;
    vec4 lightColor;
    vec4 ambientLightColor;
} light;


void main()
{
	vec3 posfix = vec3(position.x, position.z, position.y);

	vec3 normalWorldSpace = normalize(mat3(ubo.view) * fragNormal);

	vec3 directionToLight = light.pos - position.xyz;
	float attenuation = 1.0 / dot(directionToLight, directionToLight); // distance squared


	float dist = 1 - distance(position.xyz, light.pos) * light.lightColor.w;

	vec3 lightColor = light.lightColor.xyz * dist;
	vec3 ambientLight = light.ambientLightColor.xyz * light.ambientLightColor.w;
	vec3 diffuseLight = lightColor * max(dot(normalWorldSpace, normalize(directionToLight)), 0);
	
	
    vec4 baseColor = texture(texSampler, fragTexCoord);
    outColor.x = max(lightColor.x * baseColor.x, ambientLight.x * baseColor.x);
	outColor.y = max(lightColor.y * baseColor.y, ambientLight.y * baseColor.y);
	outColor.z = max(lightColor.z * baseColor.z, ambientLight.z * baseColor.z);
	outColor.w = baseColor.w;
	

}
