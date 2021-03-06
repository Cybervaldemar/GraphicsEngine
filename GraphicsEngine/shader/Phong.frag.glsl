#version 410 core

#define MAX_LIGHTS 8

#define ALPHA_FACTOR 2.0

struct LightProperties
{
	vec4 diffuseColor;
	vec4 specularColor;
		
	vec3 direction;

	vec4 position;
	float constantAttenuation;
	float linearAttenuation;
	float quadraticAttenuation;
	
	vec3 spotDirection;
	float spotCosCutOff;
	float spotCosCutOffOuter;
};

struct MaterialProperties
{
	vec4 emissiveColor;
	vec4 ambientColor;
	vec4 diffuseColor;
	sampler2D diffuseTexture; 
	vec4 specularColor;
	sampler2D specularTexture; 
	float shininess;
	
	sampler2D normalMapTexture;
	
	vec4 reflectionColor;
	vec4 refractionColor;
	
	float transparency;
	
	samplerCube dynamicCubeMapTexture;
};

uniform	vec4 u_ambientLightColor;


uniform	LightProperties u_light[MAX_LIGHTS];

uniform	MaterialProperties u_material;

uniform	float u_eta;
uniform	float u_reflectanceNormalIncidence;

uniform	int u_numberLights;
uniform	int u_lightType[MAX_LIGHTS];
uniform	int u_hasDiffuseTexture;
uniform	int u_hasSpecularTexture;
uniform	int u_hasNormalMapTexture;
uniform	int u_hasCubeMapTexture;
uniform	int u_hasDynamicCubeMapTexture;

uniform int u_convertDirectX;

uniform	int u_writeBrightColor;
uniform	float u_brightColorLimit;

uniform vec4 u_eyePosition;

uniform samplerCube u_cubemap;

uniform	int u_shadowType[MAX_LIGHTS];
uniform sampler2DShadow u_shadowTexture[MAX_LIGHTS];

in vec4 v_vertex;
in vec3 v_normal;
in vec3 v_bitangent;
in vec3 v_tangent;
in vec2 v_texCoord;

in ArrayData
{
	vec4 projCoord[MAX_LIGHTS];
} v_inData;

layout(location = 0, index = 0) out vec4 fragColor;
layout(location = 1, index = 0) out vec4 brightColor;

void main(void)
{
	vec4 diffuseTexture = vec4(1.0, 1.0, 1.0, 1.0);

	if (u_hasDiffuseTexture != 0)
	{
 		diffuseTexture = texture2D(u_material.diffuseTexture, v_texCoord);
 	}

	vec4 specularTexture = vec4(1.0, 1.0, 1.0, 1.0);

	if (u_hasSpecularTexture != 0)
	{
 		specularTexture = texture2D(u_material.specularTexture, v_texCoord);
 	}

	vec3 normal;
 	if (u_hasNormalMapTexture == 0)
 	{
		normal = normalize(v_normal);
	}
	else
	{
		vec3 normalTextureSpace = normalize(texture2D(u_material.normalMapTexture, v_texCoord).xyz * 2.0 - 1.0);
		mat3 textureToWorldSpace = mat3(normalize(v_tangent), normalize(v_bitangent), normalize(v_normal));	
		vec3 normalDX = textureToWorldSpace * normalTextureSpace;
		if (u_convertDirectX != 0)
		{
			// DirectX to OpenGL tangent space
			normal.x = dot(vec3(1.0, 0.0, 0.0), normalDX);
			normal.y = dot(vec3(0.0, -1.0, 0.0), normalDX);
			normal.z = dot(vec3(0.0, 0.0, 1.0), normalDX);
		}
		else
		{
			normal = normalDX; 
		}
	}
		
	vec4 color = u_ambientLightColor * u_material.ambientColor * diffuseTexture;
	
	vec3 eyeDirection = normalize(u_eyePosition.xyz - v_vertex.xyz);
	
	for (int indexLight = 0; indexLight < u_numberLights; indexLight++)
	{ 
		vec3 lightDirection;		
		float attenuation = 1.0;
		float falloff = 1.0;
		
		if (u_lightType[indexLight] == 0)
		{
			lightDirection = normalize(u_light[indexLight].direction);
		}
		else
		{
			vec3 lightVector = u_light[indexLight].position.xyz - v_vertex.xyz;
			
			lightDirection = normalize(lightVector);
			
			if (u_light[indexLight].constantAttenuation != 1.0 || u_light[indexLight].linearAttenuation != 0.0 || u_light[indexLight].quadraticAttenuation != 0.0)
			{
				float d = length(lightVector);
				
				if (d != 0.0 || u_light[indexLight].constantAttenuation != 0.0)
				{
					attenuation = 1.0 / (u_light[indexLight].constantAttenuation + u_light[indexLight].linearAttenuation * d + u_light[indexLight].quadraticAttenuation * d * d);
				}
			}
			
			if (u_lightType[indexLight] == 2)
			{
				float spotDot = dot(-lightDirection, u_light[indexLight].spotDirection);
				
				if (spotDot >= u_light[indexLight].spotCosCutOffOuter && spotDot > 0.0)
				{
					float penumbra = clamp((spotDot - u_light[indexLight].spotCosCutOffOuter) / (u_light[indexLight].spotCosCutOff - u_light[indexLight].spotCosCutOffOuter), 0.0, 1.0);
				
					attenuation *= spotDot * penumbra;
				}
				else
				{
					attenuation = 0.0;
				}
			}		
		}
		
		float diffuseIntensity = max(dot(normal, lightDirection), 0.0);
		
		float eDotR = max(dot(reflect(-lightDirection, normal), eyeDirection), 0.0);
		
		float specularIntensity = 0.0;
		
		if (eDotR > 0.0)
		{
			specularIntensity = pow(eDotR, u_material.shininess);
		}

		if (u_shadowType[indexLight] >= 0)
		{
			if (u_shadowType[indexLight] == 0)
			{
				if (textureProj(u_shadowTexture[indexLight], v_inData.projCoord[indexLight]) < 1.0)
				{
					diffuseIntensity = 0.0;
					specularIntensity = 0.0;
				}
			}
		}			
		
		color += u_material.emissiveColor*diffuseTexture + falloff*attenuation*u_light[indexLight].diffuseColor*u_material.diffuseColor*diffuseTexture * diffuseIntensity + falloff*attenuation*u_light[indexLight].specularColor*u_material.specularColor*specularTexture * specularIntensity;
	}
	
	if (u_hasCubeMapTexture != 0 && (dot(vec3(1.0), u_material.reflectionColor.rgb) > 0.0 || dot(vec3(1.0), u_material.refractionColor.rgb) > 0.0))
	{
		vec3 reflection = normalize(reflect(-eyeDirection, normal));
	
		vec4 reflectionColor = texture(u_cubemap, reflection);
		
		if (u_hasDynamicCubeMapTexture != 0)
		{
			vec4 temp = texture(u_material.dynamicCubeMapTexture, reflection);
		
			if (temp.a > 0.0)
			{
				float alpha = clamp(temp.a * ALPHA_FACTOR, 0.0, 1.0);
					
				reflectionColor = vec4(reflectionColor.rgb * (1.0 - alpha) + temp.rgb * alpha, 1.0);
			}  
		}

		if (u_eta > 0.0)
		{
			vec3 refraction = normalize(refract(-eyeDirection, normal, u_eta));
			
			vec4 refractionColor = texture(u_cubemap, refraction);

			if (u_hasDynamicCubeMapTexture != 0)
			{
				vec4 temp = texture(u_material.dynamicCubeMapTexture, refraction);
			
				if (temp.a > 0.0)
				{		
					float alpha = clamp(temp.a * ALPHA_FACTOR, 0.0, 1.0);

					refractionColor = vec4(refractionColor.rgb * (1.0 - alpha) + temp.rgb * alpha, 1.0);
				}  
			}
			
			float fresnel = u_reflectanceNormalIncidence + (1.0 - u_reflectanceNormalIncidence) * pow((1.0 - dot(eyeDirection, normal)), 5.0);
			
			color += mix(refractionColor * u_material.refractionColor, reflectionColor * u_material.reflectionColor, fresnel);	
		}
		else
		{
			color += reflectionColor * u_material.reflectionColor;	
		}
	}

	fragColor = vec4(color.rgb, color.a * u_material.transparency);
	
	if (u_writeBrightColor != 0)
	{
		vec3 tempBrightColor = max(color.rgb - vec3(u_brightColorLimit), vec3(0.0));
		
		float bright = dot(tempBrightColor, vec3(1.0));
		bright = smoothstep(0.0, 0.5, bright);
	
		brightColor.rgb = mix(vec3(0.0), color.rgb, bright).rgb; 
	
		brightColor.a = 1.0;
	}
}
