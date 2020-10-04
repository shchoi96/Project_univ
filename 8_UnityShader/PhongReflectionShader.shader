Shader "Custom/PhongReflectionShader"
{
    Properties
    {
        _Color("Color", Color) = (1,0,0,1)
        _Ambient("Ambient", Range(0,1)) = 0.25
        _DiffuseTex("Texture", 2D) = "white" {}
        _SpecColor("Specular Material Color", Color) = (1,1,1,1)
        _Shininess("Shininess", Float) = 10
    }
    SubShader
    {
        Pass
        {
            Tags { "LightMode" = "ForwardBase" }
            CGPROGRAM
            #pragma vertex vert
            #pragma fragment frag
            #pragma multi_compile_fwdbase

            #include "UnityCG.cginc"
            #include "UnityLightingCommon.cginc"

            struct appdata
            {
                float4 vertex : POSITION;
                float3 normal : NORMAL;
                float2 uv : TEXCOORD0;
            };

            struct v2f
            {
                float2 uv : TEXCOORD0;
                float4 vertexClip : SV_POSITION;
                float4 vertexWorld : TEXCOORD2;
                float3 worldNormal : TEXCOORD1;
            };

            sampler2D _DiffuseTex;
            float4 _DiffuseTex_ST;
            float4 _Color;
            float _Ambient;
            float _Shininess;

            v2f vert (appdata v)
            {
                v2f o;
                o.uv = TRANSFORM_TEX(v.uv, _DiffuseTex);
                o.vertexClip = UnityObjectToClipPos(v.vertex);
                o.vertexWorld = mul(unity_ObjectToWorld, v.vertex);
                o.worldNormal = UnityObjectToWorldNormal(v.normal);
                return o;
            }

            fixed4 frag(v2f i) : SV_Target
            {
                float3 normalDirection = normalize(i.worldNormal);
                float3 viewDirection = normalize(UnityWorldSpaceViewDir(i.vertexWorld));
                float3 lightDirection = normalize(UnityWorldSpaceLightDir(i.vertexWorld));
                // Ambient
                float4 ambientTerm = _Ambient * _LightColor0;
                // Diffuse
                float4 tex = tex2D(_DiffuseTex, i.uv);
                float nl = max(0.0, dot(normalDirection, lightDirection));
                float4 diffuseTerm = nl * _Color * tex * _LightColor0;
                // Specular
                float3 reflectionDirection = reflect(-lightDirection, normalDirection);
                float3 vr = max(0.0, dot(viewDirection, reflectionDirection));
                float3 specular = pow(vr, _Shininess);
                float4 specularTerm = float4(specular, 1) * _SpecColor * _LightColor0;
                float4 outColor = ambientTerm + diffuseTerm + specularTerm;
                return outColor;
            }
            ENDCG
        }
        Pass
        {
            Tags { "LightMode" = "ForwardAdd" }
            Blend One One
            CGPROGRAM
            #pragma vertex vert
            #pragma fragment frag
            #pragma multi_compile_fwdadd

            #include "UnityCG.cginc"
            #include "UnityLightingCommon.cginc"

            struct appdata
            {
                float4 vertex : POSITION;
                float3 normal : NORMAL;
                float2 uv : TEXCOORD0;
            };

            struct v2f
            {
                float2 uv : TEXCOORD0;
                float4 vertexClip : SV_POSITION;
                float4 vertexWorld : TEXCOORD2;
                float3 worldNormal : TEXCOORD1;
            };

            sampler2D _DiffuseTex;
            float4 _DiffuseTex_ST;
            float4 _Color;
            float _Shininess;

            v2f vert(appdata v)
            {
                v2f o;
                o.uv = TRANSFORM_TEX(v.uv, _DiffuseTex);
                o.vertexClip = UnityObjectToClipPos(v.vertex);
                o.vertexWorld = mul(unity_ObjectToWorld, v.vertex);
                o.worldNormal = UnityObjectToWorldNormal(v.normal);
                return o;
            }

            fixed4 frag(v2f i) : SV_Target
            {
                float3 normalDirection = normalize(i.worldNormal);
                float3 viewDirection = normalize(UnityWorldSpaceViewDir(i.vertexWorld));
                float3 lightDirection = normalize(UnityWorldSpaceLightDir(i.vertexWorld));
                // Diffuse
                float4 tex = tex2D(_DiffuseTex, i.uv);
                float nl = max(0.0, dot(normalDirection, lightDirection));
                float4 diffuseTerm = nl * _Color * tex * _LightColor0;
                // Specular
                float3 reflectionDirection = reflect(-lightDirection, normalDirection);
                float3 vr = max(0.0, dot(viewDirection, reflectionDirection));
                float3 specular = pow(vr, _Shininess);
                float4 specularTerm = float4(specular, 1) * _SpecColor * _LightColor0;
                float4 outColor = diffuseTerm + specularTerm;
                return outColor;
            }
            ENDCG

        }

    }
}
