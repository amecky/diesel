cbuffer cbChangesPerFrame : register(b0) {
	float4 data;
};

struct PS_Input {
	float4 pos  : SV_POSITION;
	float2 tex : TEXCOORD;
};

SamplerState colorSampler_ : register(s0);

float sdPlane(float3 p) {
	return p.y;
}

float2 map(in float3 pos) {
	return sdPlane(pos);
}

float2 castRay(in float3 ro, in float3 rd) {
	float tmin = 1.0;
	float tmax = 20.0;
	float t = tmin;
	float m = -1.0;
	for (int i = 0; i<64; i++)	{
		float precis = 0.0005*t;
		float2 res = map(ro + rd*t);
		if (res.x<precis || t>tmax) break;
		t += res.x;
		m = res.y;
	}

	if (t>tmax) m = -1.0;
	return float2(t, m);
}


float3 render(in float3 ro, in float3 rd) {
	float3 col = float3(0.7, 0.9, 1.0) + rd.y*0.8;
	float2 res = castRay(ro, rd);
	float t = res.x;
	float m = res.y;
	if (m > -0.5) {
		//vec3 pos = ro + t*rd;
		//vec3 nor = calcNormal(pos);
		//vec3 ref = reflect(rd, nor);

		// material        
		col = 0.45 + 0.35*float3(float3(0.05, 0.08, 0.10)*(m - 1.0));
		//if (m < 1.5)
		//{

			//float f = mod(floor(5.0*pos.z) + floor(5.0*pos.x), 2.0);
			//col = 0.3 + 0.1*f*vec3(1.0);
		//}
	}
	return float3(clamp(col, 0.0, 1.0));
}

matrix setCamera(in float3 ro, in float3 ta, float cr) {
	float3 cw = normalize(ta - ro);
	float3 cp = float3(sin(cr), cos(cr), 0.0);
	float3 cu = normalize(cross(cw, cp));
	float3 cv = normalize(cross(cu, cw));
	return matrix(float4(cu,1.0), float4(cv,1.0), float4(cw,1.0),float4(0,0,0,1));
}

float4 PS_Main( PS_Input frag ) : SV_TARGET {	

	float2 iResolution = float2(1024,768);
	float2 p = (-iResolution.xy + 2.0*frag.tex) / iResolution.y;
	float3 tot = float(0.0);
	//float3 ro = vec3(-0.5 + 3.5*cos(0.1*data.x + 6.0*mo.x), 1.0 + 2.0*mo.y, 0.5 + 4.0*sin(0.1*time + 6.0*mo.x));



	float3 cameraOrigin = float3(2.0, 2.0, 2.0);
	float3 cameraTarget = float3(0.0, 0.0, 0.0);
	float3 upDirection = float3(0.0, 1.0, 0.0);
	float3 cameraDir = normalize(cameraTarget - cameraOrigin);
	float3 cameraRight = normalize(cross(upDirection, cameraOrigin));
	float3 cameraUp = cross(cameraDir, cameraRight);

	float3 rayDir = normalize(cameraRight * p.x + cameraUp * p.y + cameraDir);

	float3 ro = float3(-0.5, 1.0,0.5);
	float3 ta = float3(-0.5, -0.4, 0.5);
	// camera-to-world transformation
	matrix ca = setCamera(ro, ta, 0.0);
	// ray direction
	//float4 rd = ca * normalize(float4(p.xy, 2.0, 1.0));

	// render	
	float3 col = render(cameraOrigin, rayDir);

	// gamma
	col = pow(col, float3(0.4545, 0.4545, 0.4545));

	tot += col;
	return float4(tot.rgb, 1.0);
}

