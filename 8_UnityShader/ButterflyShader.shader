Shader "Custom/ButterflyShader"
{
    Properties
    {
        _Color("Color", Color) = (1,1,1,1)
        _MainTex("Albedo (RGB)", 2D) = "white" {}
        _Speed("Speed",Range(0,5)) = 5.0
        _Cutoff("Alpha cutoff", Range(0,1)) = 0.5
    }
        SubShader
        {
            Tags {  "RenderType" = "TransparentCutout" "Queue" = "AlphaTest" "IgnoreProjector" = "True" }
            Cull Off
            LOD 200

            CGPROGRAM
            #pragma surface surf Lambert addshadow alphatest:_Cutoff vertex:wing
            #pragma target 3.0

            sampler2D _MainTex;
            float _Speed;

            struct Input
            {
                float2 uv_MainTex;
            };

            fixed4 _Color;

            struct appdata {
                float4 vertex : POSITION;
                float3 normal : NORMAL;
                float4 color : COLOR;
                float4 texcoord : TEXCOORD0;
            };

            void wing(inout appdata v)
            {
                float2 value = v.texcoord.xy;
                float dist = distance(value.x, 0.5); // 중심과의 거리
                // 2차 함수 형태로 움직이는 날개
                v.vertex.xyz += v.normal * sin(_Time.y * _Speed) * dist * dist * 10; 
            }

            void surf(Input IN, inout SurfaceOutput o)
            {
                fixed4 c = tex2D(_MainTex, IN.uv_MainTex) * _Color;
                o.Albedo = c.rgb;
                o.Alpha = c.a;
            }
            ENDCG
        }
            FallBack "Diffuse"
}