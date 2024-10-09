uniform float SpecularPercent;

uniform float DiffusePercent;
uniform vec3 BaseColor;
uniform samplerCube SpecularEnvMap;
uniform samplerCube DiffuseEnvMap;

uniform float roughness_value;

uniform vec4 fvSpecular;
uniform vec4 fvDiffuse;
uniform vec3 move;

varying vec3 ViewDirection;
varying vec3 LightDirection;
varying vec3 Normal;
varying vec4 FrontColor;

void main( void )
{
   // find all vectors
   vec3  fvLightDirection = normalize(gl_LightSource[0].position.xyz);
   vec3  fvViewDirection  = normalize( gl_LightSource[0].position.xyz );   
   vec3  fvViewDirection_  = normalize( gl_LightSource[0].position.xyz - move );   
  
   vec3  fvNormal         = normalize( Normal );
   
   vec3  fvViewDirectionCube  = normalize(fvViewDirection_ );
   vec3  fvReflectionCube     = reflect(-fvViewDirectionCube, fvNormal);
   
   vec3  fvReflection     = reflect(-fvViewDirection, fvNormal);
   vec3  fvHalf_vector = normalize( fvLightDirection + fvViewDirection );  
   
   // find dot products
   float dotNL           = abs(dot( fvNormal, fvLightDirection ));    
   float dotRV           = max( 0.0, dot( fvReflection, fvViewDirection ) );
   
   float dotNH        = abs(dot( fvNormal, fvHalf_vector )) ;
   float dotNV        = abs(dot( fvNormal, fvViewDirection )) ;
   float dotVH        = abs(dot( fvViewDirection, fvHalf_vector )) ;
   
   float r_sq         = roughness_value * roughness_value;
   
    // calculate geometric coefficient
    // --------------------------------
    float geo_numerator   = 2.0 * dotNH;
    float geo_denominator = dotVH;
 
    float geo_b = (geo_numerator * dotNV ) / geo_denominator;
    float geo_c = (geo_numerator * dotNL ) / geo_denominator;
    float geo   = min( 1.0, min( geo_b, geo_c ) );
    
    //----------------------------------
    
    
    // calculate roughness
    // -------------------------------
    float roughness = 0.0;  
   
    {        
        float roughness_a = 1.0 / ( 4.0 * r_sq * pow( dotNH, 4.0 ) );
        float roughness_b = dotNH * dotNH - 1.0;
        float roughness_c = r_sq * dotNH * dotNH;
 
        roughness = roughness_a * exp( roughness_b / roughness_c );
    }
    
    // calculate Frenel coefficient
    // -------------------------------    
    float fresnel = pow((1.0 + cos(dotNL)),-8.0);
    

   // Put all the terms together to compute
    // the specular term in the equation
    // -------------------------------------
    vec3 Rs_numerator   = vec3( fresnel * geo * roughness );
    float Rs_denominator  = dotNV * dotNL;
    vec3 Rs = Rs_numerator / Rs_denominator;
    
    
   vec3 diffuseColor  = vec3(textureCube(DiffuseEnvMap,  fvNormal));
   vec3 specularColor = vec3(textureCube(SpecularEnvMap, fvReflectionCube));

   // Add lighting to base color and mix

   vec3 envColor = mix(BaseColor, diffuseColor * BaseColor, DiffusePercent);
   envColor      = mix(envColor, specularColor + envColor, SpecularPercent); 
 
    // Put all the parts together to generate
    // the final colour
    // --------------------------------------

   vec4 final = dotNL * fvSpecular * vec4(Rs, 1.0);
   final.w = 1.0;    
   
   gl_FragColor = final + vec4(envColor,0.0);  
}