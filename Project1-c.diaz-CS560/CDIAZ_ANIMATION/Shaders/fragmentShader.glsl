#version 410 

in vec2 TexCoord0;
out vec4 out_Color;
uniform sampler2D gColorMap;   

uniform sampler2D textureSampler;
uniform float shininess;



uniform vec3 phongSpecular;
uniform float phongShininess;
uniform vec3 lightValue, lightAmbient;
uniform vec3 lightPos;
uniform vec3 eyePos;

in vec3 FragPos; 
in vec3 normalVec;

out vec4 FragColor;
//out vec3 color;
void main()
{
	 
	 vec3 objectColor = texture( gColorMap, TexCoord0.xy ).xyz;
	
	 float ambientStrength = 0.3f;
     vec3 ambient = ambientStrength * lightValue;
	 
	 vec3 norm = normalize(normalVec);
	 vec3 lightDir = normalize(lightPos - FragPos);

	 float diff = max(dot(norm, lightDir), 0.0);
     vec3 diffuse = diff * lightValue;

	 // Specular
    float specularStrength = 1.0f;
    vec3 viewDir = normalize(eyePos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightValue;  

	vec3 result = (ambient + diffuse + specular) * objectColor;
	
	FragColor = vec4(result, 1.0f);
	//color = objectColor;
}
