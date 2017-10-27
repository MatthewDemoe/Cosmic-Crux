#version 420

struct PointLight
{
	vec4 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float specularExponent;
	float constantAttenuation;
	float linearAttenuation;
	float quadraticAttenuation;
};

uniform PointLight pointLight;

uniform sampler2D uTex;

in vec3 position;
in vec2 texCoord;
in vec3 normal;

out vec4 outColor;

vec3 calculatePointLight(PointLight light, vec3 norm, vec4 textureColor);

void main()
{
	//account for rasterizer interpolating 
	vec3 norm = normalize(normal);

	vec4 textureColor = texture(uTex, texCoord);

	outColor.rgb += calculatePointLight(pointLight, norm, textureColor);

	outColor.a = textureColor.a;
}

vec3 calculatePointLight(PointLight light, vec3 norm, vec4 textureColor)
{
	vec3 lightVec = light.position.xyz - position;
	float dist = length(lightVec);
	vec3 lightDir = lightVec / dist;


	//The light contributes to this surface

	//Calculate the attenuation(falloff)
	float attenuation = 1.0 / (light.constantAttenuation + (light.linearAttenuation * dist) + (light.quadraticAttenuation * dist * dist));

	vec3 ambient = attenuation * light.ambient;

	//Calculate the diffuse contribution
	float NdotL = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = light.diffuse * NdotL * attenuation * textureColor.rgb;

	vec3 reflectDir = reflect(-lightDir, norm);
	float VdotR = max(dot(normalize(-position), reflectDir), 0.0);

	vec3 specular = light.specular * pow(VdotR, light.specularExponent) * attenuation;

	return ambient + diffuse + specular;
}
