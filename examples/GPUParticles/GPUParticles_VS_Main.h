#if 0
//
// Generated by Microsoft (R) D3D Shader Disassembler
//
//
//   fxc /nologo GPUParticles.hlsl /Tvs_4_0 /Zi /Zpc /Qstrip_reflect
//    /Qstrip_debug /EVS_Main /FhGPUParticles_VS_Main.h
//    /VnGPUParticles_VS_Main
//
//
// Input signature:
//
// Name                 Index   Mask Register SysValue Format   Used
// -------------------- ----- ------ -------- -------- ------ ------
// SV_VERTEXID              0   x           0   VERTID   uint   x   
//
//
// Output signature:
//
// Name                 Index   Mask Register SysValue Format   Used
// -------------------- ----- ------ -------- -------- ------ ------
// SV_POSITION              0   xyzw        0      POS  float   xyzw
// TEXCOORD                 0   xy          1     NONE  float   xy  
// COLOR                    0   xyzw        2     NONE  float   xyzw
//
vs_4_0
dcl_globalFlags refactoringAllowed | enableRawAndStructuredBuffers
dcl_constantbuffer cb0[12], immediateIndexed
dcl_resource_structured t1, 60 
dcl_input_sgv v0.x, vertex_id
dcl_output_siv o0.xyzw, position
dcl_output o1.xy
dcl_output o2.xyzw
dcl_temps 9
ushr r0.x, v0.x, l(2)
ld_structured r1.xyzw, r0.x, l(32), t1.xyzw
add r0.y, r1.y, l(21.582300)
round_ni r0.z, r0.y
mul r0.w, r0.z, l(171.000000)
sincos r0.w, null, r0.w
mul r0.w, r0.w, l(43758.546875)
frc r0.yw, r0.yyyw
mad r2.xyzw, r0.zzzz, l(171.000000, 171.000000, 171.000000, 171.000000), l(1.000000, 57.000000, 58.000000, 113.000000)
mad r3.xyz, r0.zzzz, l(171.000000, 171.000000, 171.000000, 0.000000), l(114.000000, 170.000000, 171.000000, 0.000000)
sincos r3.xyz, null, r3.xyzx
mul r3.xyz, r3.xyzx, l(43758.546875, 43758.546875, 43758.546875, 0.000000)
frc r3.xyz, r3.xyzx
sincos r2.xyzw, null, r2.xyzw
mul r2.xyzw, r2.xyzw, l(43758.546875, 43758.546875, 43758.546875, 43758.546875)
frc r2.xyzw, r2.xyzw
add r0.z, -r0.w, r2.x
mul r2.x, r0.y, r0.y
mad r0.y, -r0.y, l(2.000000), l(3.000000)
mul r0.y, r0.y, r2.x
mad r0.z, r0.y, r0.z, r0.w
add r0.w, -r2.y, r2.z
mad r0.w, r0.y, r0.w, r2.y
add r0.w, -r0.z, r0.w
mad r0.z, r0.y, r0.w, r0.z
add r0.w, -r2.w, r3.x
mad r0.w, r0.y, r0.w, r2.w
add r2.x, -r3.y, r3.z
mad r2.x, r0.y, r2.x, r3.y
add r2.x, -r0.w, r2.x
mad r0.w, r0.y, r2.x, r0.w
add r0.w, -r0.z, r0.w
mad r0.y, r0.y, r0.w, r0.z
mad r0.y, r0.y, l(0.500000), l(0.500000)
ld_structured r2.xyzw, r0.x, l(0), t1.xyzw
lt r0.z, r2.y, l(-1.000000)
ld_structured r3.xyzw, r0.x, l(16), t1.zwxy
ld_structured r4.xyz, r0.x, l(48), t1.xyzx
movc r0.x, r0.z, -r3.z, r3.z
mul r0.y, r0.y, r0.x
mul r5.xy, r1.yyyy, l(1.300000, 0.600000, 0.000000, 0.000000)
round_ni r6.xyzw, r5.xxxy
frc r5.xy, r5.xyxx
mad r7.xyzw, r6.xyzw, l(171.000000, 171.000000, 171.000000, 171.000000), l(114.000000, 170.000000, 171.000000, 1.000000)
sincos r7.xyzw, null, r7.xyzw
mul r7.xyzw, r7.xyzw, l(43758.546875, 43758.546875, 43758.546875, 43758.546875)
frc r7.xyzw, r7.xyzw
mad r8.xyzw, r6.zzzz, l(171.000000, 171.000000, 171.000000, 171.000000), l(1.000000, 57.000000, 58.000000, 113.000000)
sincos r8.xyzw, null, r8.xyzw
mul r8.xyzw, r8.xyzw, l(43758.546875, 43758.546875, 43758.546875, 43758.546875)
frc r8.xyzw, r8.xyzw
add r0.w, r7.x, -r8.w
mul r5.zw, r5.xxxy, r5.xxxy
mad r5.xy, -r5.xyxx, l(2.000000, 2.000000, 0.000000, 0.000000), l(3.000000, 3.000000, 0.000000, 0.000000)
mul r5.xy, r5.xyxx, r5.zwzz
mad r0.w, r5.x, r0.w, r8.w
add r5.z, -r7.y, r7.z
mad r5.z, r5.x, r5.z, r7.y
add r5.z, -r0.w, r5.z
mad r0.w, r5.x, r5.z, r0.w
mul r5.zw, r6.zzzw, l(0.000000, 0.000000, 171.000000, 171.000000)
sincos r5.zw, null, r5.zzzw
mul r5.zw, r5.zzzw, l(0.000000, 0.000000, 43758.546875, 43758.546875)
frc r5.zw, r5.zzzw
add r6.x, -r5.z, r8.x
mad r5.z, r5.x, r6.x, r5.z
add r6.x, -r8.y, r8.z
mad r6.x, r5.x, r6.x, r8.y
add r6.x, -r5.z, r6.x
mad r5.z, r5.x, r6.x, r5.z
add r0.w, r0.w, -r5.z
mad r0.w, r5.x, r0.w, r5.z
mul r0.x, r0.w, r2.w
add r0.w, -r5.w, r7.w
mad r0.w, r5.y, r0.w, r5.w
mad r7.xyzw, r6.wwww, l(171.000000, 171.000000, 171.000000, 171.000000), l(57.000000, 58.000000, 113.000000, 114.000000)
mad r5.xz, r6.wwww, l(171.000000, 0.000000, 171.000000, 0.000000), l(170.000000, 0.000000, 171.000000, 0.000000)
sincos r5.xz, null, r5.xxzx
mul r5.xz, r5.xxzx, l(43758.546875, 0.000000, 43758.546875, 0.000000)
frc r5.xz, r5.xxzx
sincos r6.xyzw, null, r7.xyzw
mul r6.xyzw, r6.xyzw, l(43758.546875, 43758.546875, 43758.546875, 43758.546875)
frc r6.xyzw, r6.xyzw
add r6.yw, -r6.xxxz, r6.yyyw
mad r6.xy, r5.yyyy, r6.ywyy, r6.xzxx
add r2.w, -r0.w, r6.x
mad r0.w, r5.y, r2.w, r0.w
add r2.w, -r5.x, r5.z
mad r2.w, r5.y, r2.w, r5.x
add r2.w, -r6.y, r2.w
mad r2.w, r5.y, r2.w, r6.y
add r2.w, -r0.w, r2.w
mad r0.w, r5.y, r2.w, r0.w
mul r0.z, r0.w, r3.w
mad r0.xyz, r0.xyzx, r1.yyyy, r2.xyzx
mul r0.w, r1.y, r1.y
mov r3.z, r1.x
mad r0.xyz, r0.wwww, r3.xyzx, r0.xyzx
add r2.xyz, -r0.zxyz, cb0[10].zxyz
dp3 r0.w, r2.xyzx, r2.xyzx
rsq r0.w, r0.w
mul r2.xyz, r0.wwww, r2.xyzx
mul r3.xyz, r2.yzxy, l(0.000000, 0.000000, 1.000000, 0.000000)
mad r3.xyz, r2.xyzx, l(0.000000, 1.000000, 0.000000, 0.000000), -r3.xyzx
mul r5.xyz, r2.xyzx, r3.xyzx
mad r2.xyz, r2.zxyz, r3.yzxy, -r5.xyzx
mov r4.w, r1.w
mad_sat r1.xy, r4.yzyy, r1.yyyy, r4.wxww
and r3.xw, v0.xxxx, l(1, 0, 0, 2)
movc r4.xy, r3.wxww, l(0.500000,0.500000,0,0), l(-0.500000,-0.500000,0,0)
movc o1.xy, r3.xwxx, cb0[11].zwzz, cb0[11].xyxx
mul r1.xy, r1.xyxx, r4.xyxx
mul r2.xyz, r1.yyyy, r2.xyzx
mul r1.xy, r1.xxxx, r3.zyzz
mul r3.xz, r1.xxyx, l(0.500000, 0.000000, 0.500000, 0.000000)
mov r3.y, l(0)
mad r1.xyw, -r2.xyxz, l(0.500000, 0.500000, 0.000000, 0.500000), r3.xyxz
add r0.xyz, r0.xyzx, r1.xywx
mov r0.w, l(1.000000)
dp4 o0.x, r0.xyzw, cb0[2].xyzw
dp4 o0.y, r0.xyzw, cb0[3].xyzw
dp4 o0.z, r0.xyzw, cb0[4].xyzw
dp4 o0.w, r0.xyzw, cb0[5].xyzw
add r0.xyzw, -cb0[0].xyzw, cb0[1].xyzw
mad o2.xyzw, r1.zzzz, r0.xyzw, cb0[0].xyzw
ret 
// Approximately 0 instruction slots used
#endif

const BYTE GPUParticles_VS_Main[] =
{
     68,  88,  66,  67,  87, 154, 
      1, 148, 226,  45,  79, 203, 
    251,  55, 197,  94, 159,  71, 
     17, 244,   1,   0,   0,   0, 
    196,  17,   0,   0,   4,   0, 
      0,   0,  48,   0,   0,   0, 
    100,   0,   0,   0, 216,   0, 
      0,   0, 180,  17,   0,   0, 
     73,  83,  71,  78,  44,   0, 
      0,   0,   1,   0,   0,   0, 
      8,   0,   0,   0,  32,   0, 
      0,   0,   0,   0,   0,   0, 
      6,   0,   0,   0,   1,   0, 
      0,   0,   0,   0,   0,   0, 
      1,   1,   0,   0,  83,  86, 
     95,  86,  69,  82,  84,  69, 
     88,  73,  68,   0,  79,  83, 
     71,  78, 108,   0,   0,   0, 
      3,   0,   0,   0,   8,   0, 
      0,   0,  80,   0,   0,   0, 
      0,   0,   0,   0,   1,   0, 
      0,   0,   3,   0,   0,   0, 
      0,   0,   0,   0,  15,   0, 
      0,   0,  92,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   3,   0,   0,   0, 
      1,   0,   0,   0,   3,  12, 
      0,   0, 101,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   3,   0,   0,   0, 
      2,   0,   0,   0,  15,   0, 
      0,   0,  83,  86,  95,  80, 
     79,  83,  73,  84,  73,  79, 
     78,   0,  84,  69,  88,  67, 
     79,  79,  82,  68,   0,  67, 
     79,  76,  79,  82,   0, 171, 
     83,  72,  69,  88, 212,  16, 
      0,   0,  64,   0,   1,   0, 
     53,   4,   0,   0, 106,  72, 
      0,   1,  89,   0,   0,   4, 
     70, 142,  32,   0,   0,   0, 
      0,   0,  12,   0,   0,   0, 
    162,   0,   0,   4,   0, 112, 
     16,   0,   1,   0,   0,   0, 
     60,   0,   0,   0,  96,   0, 
      0,   4,  18,  16,  16,   0, 
      0,   0,   0,   0,   6,   0, 
      0,   0, 103,   0,   0,   4, 
    242,  32,  16,   0,   0,   0, 
      0,   0,   1,   0,   0,   0, 
    101,   0,   0,   3,  50,  32, 
     16,   0,   1,   0,   0,   0, 
    101,   0,   0,   3, 242,  32, 
     16,   0,   2,   0,   0,   0, 
    104,   0,   0,   2,   9,   0, 
      0,   0,  85,   0,   0,   7, 
     18,   0,  16,   0,   0,   0, 
      0,   0,  10,  16,  16,   0, 
      0,   0,   0,   0,   1,  64, 
      0,   0,   2,   0,   0,   0, 
    167,   0,   0,   9, 242,   0, 
     16,   0,   1,   0,   0,   0, 
     10,   0,  16,   0,   0,   0, 
      0,   0,   1,  64,   0,   0, 
     32,   0,   0,   0,  70, 126, 
     16,   0,   1,   0,   0,   0, 
      0,   0,   0,   7,  34,   0, 
     16,   0,   0,   0,   0,   0, 
     26,   0,  16,   0,   1,   0, 
      0,   0,   1,  64,   0,   0, 
    141, 168, 172,  65,  65,   0, 
      0,   5,  66,   0,  16,   0, 
      0,   0,   0,   0,  26,   0, 
     16,   0,   0,   0,   0,   0, 
     56,   0,   0,   7, 130,   0, 
     16,   0,   0,   0,   0,   0, 
     42,   0,  16,   0,   0,   0, 
      0,   0,   1,  64,   0,   0, 
      0,   0,  43,  67,  77,   0, 
      0,   6, 130,   0,  16,   0, 
      0,   0,   0,   0,   0, 208, 
      0,   0,  58,   0,  16,   0, 
      0,   0,   0,   0,  56,   0, 
      0,   7, 130,   0,  16,   0, 
      0,   0,   0,   0,  58,   0, 
     16,   0,   0,   0,   0,   0, 
      1,  64,   0,   0, 140, 238, 
     42,  71,  26,   0,   0,   5, 
    162,   0,  16,   0,   0,   0, 
      0,   0,  86,  13,  16,   0, 
      0,   0,   0,   0,  50,   0, 
      0,  15, 242,   0,  16,   0, 
      2,   0,   0,   0, 166,  10, 
     16,   0,   0,   0,   0,   0, 
      2,  64,   0,   0,   0,   0, 
     43,  67,   0,   0,  43,  67, 
      0,   0,  43,  67,   0,   0, 
     43,  67,   2,  64,   0,   0, 
      0,   0, 128,  63,   0,   0, 
    100,  66,   0,   0, 104,  66, 
      0,   0, 226,  66,  50,   0, 
      0,  15, 114,   0,  16,   0, 
      3,   0,   0,   0, 166,  10, 
     16,   0,   0,   0,   0,   0, 
      2,  64,   0,   0,   0,   0, 
     43,  67,   0,   0,  43,  67, 
      0,   0,  43,  67,   0,   0, 
      0,   0,   2,  64,   0,   0, 
      0,   0, 228,  66,   0,   0, 
     42,  67,   0,   0,  43,  67, 
      0,   0,   0,   0,  77,   0, 
      0,   6, 114,   0,  16,   0, 
      3,   0,   0,   0,   0, 208, 
      0,   0,  70,   2,  16,   0, 
      3,   0,   0,   0,  56,   0, 
      0,  10, 114,   0,  16,   0, 
      3,   0,   0,   0,  70,   2, 
     16,   0,   3,   0,   0,   0, 
      2,  64,   0,   0, 140, 238, 
     42,  71, 140, 238,  42,  71, 
    140, 238,  42,  71,   0,   0, 
      0,   0,  26,   0,   0,   5, 
    114,   0,  16,   0,   3,   0, 
      0,   0,  70,   2,  16,   0, 
      3,   0,   0,   0,  77,   0, 
      0,   6, 242,   0,  16,   0, 
      2,   0,   0,   0,   0, 208, 
      0,   0,  70,  14,  16,   0, 
      2,   0,   0,   0,  56,   0, 
      0,  10, 242,   0,  16,   0, 
      2,   0,   0,   0,  70,  14, 
     16,   0,   2,   0,   0,   0, 
      2,  64,   0,   0, 140, 238, 
     42,  71, 140, 238,  42,  71, 
    140, 238,  42,  71, 140, 238, 
     42,  71,  26,   0,   0,   5, 
    242,   0,  16,   0,   2,   0, 
      0,   0,  70,  14,  16,   0, 
      2,   0,   0,   0,   0,   0, 
      0,   8,  66,   0,  16,   0, 
      0,   0,   0,   0,  58,   0, 
     16, 128,  65,   0,   0,   0, 
      0,   0,   0,   0,  10,   0, 
     16,   0,   2,   0,   0,   0, 
     56,   0,   0,   7,  18,   0, 
     16,   0,   2,   0,   0,   0, 
     26,   0,  16,   0,   0,   0, 
      0,   0,  26,   0,  16,   0, 
      0,   0,   0,   0,  50,   0, 
      0,  10,  34,   0,  16,   0, 
      0,   0,   0,   0,  26,   0, 
     16, 128,  65,   0,   0,   0, 
      0,   0,   0,   0,   1,  64, 
      0,   0,   0,   0,   0,  64, 
      1,  64,   0,   0,   0,   0, 
     64,  64,  56,   0,   0,   7, 
     34,   0,  16,   0,   0,   0, 
      0,   0,  26,   0,  16,   0, 
      0,   0,   0,   0,  10,   0, 
     16,   0,   2,   0,   0,   0, 
     50,   0,   0,   9,  66,   0, 
     16,   0,   0,   0,   0,   0, 
     26,   0,  16,   0,   0,   0, 
      0,   0,  42,   0,  16,   0, 
      0,   0,   0,   0,  58,   0, 
     16,   0,   0,   0,   0,   0, 
      0,   0,   0,   8, 130,   0, 
     16,   0,   0,   0,   0,   0, 
     26,   0,  16, 128,  65,   0, 
      0,   0,   2,   0,   0,   0, 
     42,   0,  16,   0,   2,   0, 
      0,   0,  50,   0,   0,   9, 
    130,   0,  16,   0,   0,   0, 
      0,   0,  26,   0,  16,   0, 
      0,   0,   0,   0,  58,   0, 
     16,   0,   0,   0,   0,   0, 
     26,   0,  16,   0,   2,   0, 
      0,   0,   0,   0,   0,   8, 
    130,   0,  16,   0,   0,   0, 
      0,   0,  42,   0,  16, 128, 
     65,   0,   0,   0,   0,   0, 
      0,   0,  58,   0,  16,   0, 
      0,   0,   0,   0,  50,   0, 
      0,   9,  66,   0,  16,   0, 
      0,   0,   0,   0,  26,   0, 
     16,   0,   0,   0,   0,   0, 
     58,   0,  16,   0,   0,   0, 
      0,   0,  42,   0,  16,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   8, 130,   0,  16,   0, 
      0,   0,   0,   0,  58,   0, 
     16, 128,  65,   0,   0,   0, 
      2,   0,   0,   0,  10,   0, 
     16,   0,   3,   0,   0,   0, 
     50,   0,   0,   9, 130,   0, 
     16,   0,   0,   0,   0,   0, 
     26,   0,  16,   0,   0,   0, 
      0,   0,  58,   0,  16,   0, 
      0,   0,   0,   0,  58,   0, 
     16,   0,   2,   0,   0,   0, 
      0,   0,   0,   8,  18,   0, 
     16,   0,   2,   0,   0,   0, 
     26,   0,  16, 128,  65,   0, 
      0,   0,   3,   0,   0,   0, 
     42,   0,  16,   0,   3,   0, 
      0,   0,  50,   0,   0,   9, 
     18,   0,  16,   0,   2,   0, 
      0,   0,  26,   0,  16,   0, 
      0,   0,   0,   0,  10,   0, 
     16,   0,   2,   0,   0,   0, 
     26,   0,  16,   0,   3,   0, 
      0,   0,   0,   0,   0,   8, 
     18,   0,  16,   0,   2,   0, 
      0,   0,  58,   0,  16, 128, 
     65,   0,   0,   0,   0,   0, 
      0,   0,  10,   0,  16,   0, 
      2,   0,   0,   0,  50,   0, 
      0,   9, 130,   0,  16,   0, 
      0,   0,   0,   0,  26,   0, 
     16,   0,   0,   0,   0,   0, 
     10,   0,  16,   0,   2,   0, 
      0,   0,  58,   0,  16,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   8, 130,   0,  16,   0, 
      0,   0,   0,   0,  42,   0, 
     16, 128,  65,   0,   0,   0, 
      0,   0,   0,   0,  58,   0, 
     16,   0,   0,   0,   0,   0, 
     50,   0,   0,   9,  34,   0, 
     16,   0,   0,   0,   0,   0, 
     26,   0,  16,   0,   0,   0, 
      0,   0,  58,   0,  16,   0, 
      0,   0,   0,   0,  42,   0, 
     16,   0,   0,   0,   0,   0, 
     50,   0,   0,   9,  34,   0, 
     16,   0,   0,   0,   0,   0, 
     26,   0,  16,   0,   0,   0, 
      0,   0,   1,  64,   0,   0, 
      0,   0,   0,  63,   1,  64, 
      0,   0,   0,   0,   0,  63, 
    167,   0,   0,   9, 242,   0, 
     16,   0,   2,   0,   0,   0, 
     10,   0,  16,   0,   0,   0, 
      0,   0,   1,  64,   0,   0, 
      0,   0,   0,   0,  70, 126, 
     16,   0,   1,   0,   0,   0, 
     49,   0,   0,   7,  66,   0, 
     16,   0,   0,   0,   0,   0, 
     26,   0,  16,   0,   2,   0, 
      0,   0,   1,  64,   0,   0, 
      0,   0, 128, 191, 167,   0, 
      0,   9, 242,   0,  16,   0, 
      3,   0,   0,   0,  10,   0, 
     16,   0,   0,   0,   0,   0, 
      1,  64,   0,   0,  16,   0, 
      0,   0, 230, 116,  16,   0, 
      1,   0,   0,   0, 167,   0, 
      0,   9, 114,   0,  16,   0, 
      4,   0,   0,   0,  10,   0, 
     16,   0,   0,   0,   0,   0, 
      1,  64,   0,   0,  48,   0, 
      0,   0,  70, 114,  16,   0, 
      1,   0,   0,   0,  55,   0, 
      0,  10,  18,   0,  16,   0, 
      0,   0,   0,   0,  42,   0, 
     16,   0,   0,   0,   0,   0, 
     42,   0,  16, 128,  65,   0, 
      0,   0,   3,   0,   0,   0, 
     42,   0,  16,   0,   3,   0, 
      0,   0,  56,   0,   0,   7, 
     34,   0,  16,   0,   0,   0, 
      0,   0,  26,   0,  16,   0, 
      0,   0,   0,   0,  10,   0, 
     16,   0,   0,   0,   0,   0, 
     56,   0,   0,  10,  50,   0, 
     16,   0,   5,   0,   0,   0, 
     86,   5,  16,   0,   1,   0, 
      0,   0,   2,  64,   0,   0, 
    102, 102, 166,  63, 154, 153, 
     25,  63,   0,   0,   0,   0, 
      0,   0,   0,   0,  65,   0, 
      0,   5, 242,   0,  16,   0, 
      6,   0,   0,   0,   6,   4, 
     16,   0,   5,   0,   0,   0, 
     26,   0,   0,   5,  50,   0, 
     16,   0,   5,   0,   0,   0, 
     70,   0,  16,   0,   5,   0, 
      0,   0,  50,   0,   0,  15, 
    242,   0,  16,   0,   7,   0, 
      0,   0,  70,  14,  16,   0, 
      6,   0,   0,   0,   2,  64, 
      0,   0,   0,   0,  43,  67, 
      0,   0,  43,  67,   0,   0, 
     43,  67,   0,   0,  43,  67, 
      2,  64,   0,   0,   0,   0, 
    228,  66,   0,   0,  42,  67, 
      0,   0,  43,  67,   0,   0, 
    128,  63,  77,   0,   0,   6, 
    242,   0,  16,   0,   7,   0, 
      0,   0,   0, 208,   0,   0, 
     70,  14,  16,   0,   7,   0, 
      0,   0,  56,   0,   0,  10, 
    242,   0,  16,   0,   7,   0, 
      0,   0,  70,  14,  16,   0, 
      7,   0,   0,   0,   2,  64, 
      0,   0, 140, 238,  42,  71, 
    140, 238,  42,  71, 140, 238, 
     42,  71, 140, 238,  42,  71, 
     26,   0,   0,   5, 242,   0, 
     16,   0,   7,   0,   0,   0, 
     70,  14,  16,   0,   7,   0, 
      0,   0,  50,   0,   0,  15, 
    242,   0,  16,   0,   8,   0, 
      0,   0, 166,  10,  16,   0, 
      6,   0,   0,   0,   2,  64, 
      0,   0,   0,   0,  43,  67, 
      0,   0,  43,  67,   0,   0, 
     43,  67,   0,   0,  43,  67, 
      2,  64,   0,   0,   0,   0, 
    128,  63,   0,   0, 100,  66, 
      0,   0, 104,  66,   0,   0, 
    226,  66,  77,   0,   0,   6, 
    242,   0,  16,   0,   8,   0, 
      0,   0,   0, 208,   0,   0, 
     70,  14,  16,   0,   8,   0, 
      0,   0,  56,   0,   0,  10, 
    242,   0,  16,   0,   8,   0, 
      0,   0,  70,  14,  16,   0, 
      8,   0,   0,   0,   2,  64, 
      0,   0, 140, 238,  42,  71, 
    140, 238,  42,  71, 140, 238, 
     42,  71, 140, 238,  42,  71, 
     26,   0,   0,   5, 242,   0, 
     16,   0,   8,   0,   0,   0, 
     70,  14,  16,   0,   8,   0, 
      0,   0,   0,   0,   0,   8, 
    130,   0,  16,   0,   0,   0, 
      0,   0,  10,   0,  16,   0, 
      7,   0,   0,   0,  58,   0, 
     16, 128,  65,   0,   0,   0, 
      8,   0,   0,   0,  56,   0, 
      0,   7, 194,   0,  16,   0, 
      5,   0,   0,   0,   6,   4, 
     16,   0,   5,   0,   0,   0, 
      6,   4,  16,   0,   5,   0, 
      0,   0,  50,   0,   0,  16, 
     50,   0,  16,   0,   5,   0, 
      0,   0,  70,   0,  16, 128, 
     65,   0,   0,   0,   5,   0, 
      0,   0,   2,  64,   0,   0, 
      0,   0,   0,  64,   0,   0, 
      0,  64,   0,   0,   0,   0, 
      0,   0,   0,   0,   2,  64, 
      0,   0,   0,   0,  64,  64, 
      0,   0,  64,  64,   0,   0, 
      0,   0,   0,   0,   0,   0, 
     56,   0,   0,   7,  50,   0, 
     16,   0,   5,   0,   0,   0, 
     70,   0,  16,   0,   5,   0, 
      0,   0, 230,  10,  16,   0, 
      5,   0,   0,   0,  50,   0, 
      0,   9, 130,   0,  16,   0, 
      0,   0,   0,   0,  10,   0, 
     16,   0,   5,   0,   0,   0, 
     58,   0,  16,   0,   0,   0, 
      0,   0,  58,   0,  16,   0, 
      8,   0,   0,   0,   0,   0, 
      0,   8,  66,   0,  16,   0, 
      5,   0,   0,   0,  26,   0, 
     16, 128,  65,   0,   0,   0, 
      7,   0,   0,   0,  42,   0, 
     16,   0,   7,   0,   0,   0, 
     50,   0,   0,   9,  66,   0, 
     16,   0,   5,   0,   0,   0, 
     10,   0,  16,   0,   5,   0, 
      0,   0,  42,   0,  16,   0, 
      5,   0,   0,   0,  26,   0, 
     16,   0,   7,   0,   0,   0, 
      0,   0,   0,   8,  66,   0, 
     16,   0,   5,   0,   0,   0, 
     58,   0,  16, 128,  65,   0, 
      0,   0,   0,   0,   0,   0, 
     42,   0,  16,   0,   5,   0, 
      0,   0,  50,   0,   0,   9, 
    130,   0,  16,   0,   0,   0, 
      0,   0,  10,   0,  16,   0, 
      5,   0,   0,   0,  42,   0, 
     16,   0,   5,   0,   0,   0, 
     58,   0,  16,   0,   0,   0, 
      0,   0,  56,   0,   0,  10, 
    194,   0,  16,   0,   5,   0, 
      0,   0, 166,  14,  16,   0, 
      6,   0,   0,   0,   2,  64, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
     43,  67,   0,   0,  43,  67, 
     77,   0,   0,   6, 194,   0, 
     16,   0,   5,   0,   0,   0, 
      0, 208,   0,   0, 166,  14, 
     16,   0,   5,   0,   0,   0, 
     56,   0,   0,  10, 194,   0, 
     16,   0,   5,   0,   0,   0, 
    166,  14,  16,   0,   5,   0, 
      0,   0,   2,  64,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0, 140, 238,  42,  71, 
    140, 238,  42,  71,  26,   0, 
      0,   5, 194,   0,  16,   0, 
      5,   0,   0,   0, 166,  14, 
     16,   0,   5,   0,   0,   0, 
      0,   0,   0,   8,  18,   0, 
     16,   0,   6,   0,   0,   0, 
     42,   0,  16, 128,  65,   0, 
      0,   0,   5,   0,   0,   0, 
     10,   0,  16,   0,   8,   0, 
      0,   0,  50,   0,   0,   9, 
     66,   0,  16,   0,   5,   0, 
      0,   0,  10,   0,  16,   0, 
      5,   0,   0,   0,  10,   0, 
     16,   0,   6,   0,   0,   0, 
     42,   0,  16,   0,   5,   0, 
      0,   0,   0,   0,   0,   8, 
     18,   0,  16,   0,   6,   0, 
      0,   0,  26,   0,  16, 128, 
     65,   0,   0,   0,   8,   0, 
      0,   0,  42,   0,  16,   0, 
      8,   0,   0,   0,  50,   0, 
      0,   9,  18,   0,  16,   0, 
      6,   0,   0,   0,  10,   0, 
     16,   0,   5,   0,   0,   0, 
     10,   0,  16,   0,   6,   0, 
      0,   0,  26,   0,  16,   0, 
      8,   0,   0,   0,   0,   0, 
      0,   8,  18,   0,  16,   0, 
      6,   0,   0,   0,  42,   0, 
     16, 128,  65,   0,   0,   0, 
      5,   0,   0,   0,  10,   0, 
     16,   0,   6,   0,   0,   0, 
     50,   0,   0,   9,  66,   0, 
     16,   0,   5,   0,   0,   0, 
     10,   0,  16,   0,   5,   0, 
      0,   0,  10,   0,  16,   0, 
      6,   0,   0,   0,  42,   0, 
     16,   0,   5,   0,   0,   0, 
      0,   0,   0,   8, 130,   0, 
     16,   0,   0,   0,   0,   0, 
     58,   0,  16,   0,   0,   0, 
      0,   0,  42,   0,  16, 128, 
     65,   0,   0,   0,   5,   0, 
      0,   0,  50,   0,   0,   9, 
    130,   0,  16,   0,   0,   0, 
      0,   0,  10,   0,  16,   0, 
      5,   0,   0,   0,  58,   0, 
     16,   0,   0,   0,   0,   0, 
     42,   0,  16,   0,   5,   0, 
      0,   0,  56,   0,   0,   7, 
     18,   0,  16,   0,   0,   0, 
      0,   0,  58,   0,  16,   0, 
      0,   0,   0,   0,  58,   0, 
     16,   0,   2,   0,   0,   0, 
      0,   0,   0,   8, 130,   0, 
     16,   0,   0,   0,   0,   0, 
     58,   0,  16, 128,  65,   0, 
      0,   0,   5,   0,   0,   0, 
     58,   0,  16,   0,   7,   0, 
      0,   0,  50,   0,   0,   9, 
    130,   0,  16,   0,   0,   0, 
      0,   0,  26,   0,  16,   0, 
      5,   0,   0,   0,  58,   0, 
     16,   0,   0,   0,   0,   0, 
     58,   0,  16,   0,   5,   0, 
      0,   0,  50,   0,   0,  15, 
    242,   0,  16,   0,   7,   0, 
      0,   0, 246,  15,  16,   0, 
      6,   0,   0,   0,   2,  64, 
      0,   0,   0,   0,  43,  67, 
      0,   0,  43,  67,   0,   0, 
     43,  67,   0,   0,  43,  67, 
      2,  64,   0,   0,   0,   0, 
    100,  66,   0,   0, 104,  66, 
      0,   0, 226,  66,   0,   0, 
    228,  66,  50,   0,   0,  15, 
     82,   0,  16,   0,   5,   0, 
      0,   0, 246,  15,  16,   0, 
      6,   0,   0,   0,   2,  64, 
      0,   0,   0,   0,  43,  67, 
      0,   0,   0,   0,   0,   0, 
     43,  67,   0,   0,   0,   0, 
      2,  64,   0,   0,   0,   0, 
     42,  67,   0,   0,   0,   0, 
      0,   0,  43,  67,   0,   0, 
      0,   0,  77,   0,   0,   6, 
     82,   0,  16,   0,   5,   0, 
      0,   0,   0, 208,   0,   0, 
      6,   2,  16,   0,   5,   0, 
      0,   0,  56,   0,   0,  10, 
     82,   0,  16,   0,   5,   0, 
      0,   0,   6,   2,  16,   0, 
      5,   0,   0,   0,   2,  64, 
      0,   0, 140, 238,  42,  71, 
      0,   0,   0,   0, 140, 238, 
     42,  71,   0,   0,   0,   0, 
     26,   0,   0,   5,  82,   0, 
     16,   0,   5,   0,   0,   0, 
      6,   2,  16,   0,   5,   0, 
      0,   0,  77,   0,   0,   6, 
    242,   0,  16,   0,   6,   0, 
      0,   0,   0, 208,   0,   0, 
     70,  14,  16,   0,   7,   0, 
      0,   0,  56,   0,   0,  10, 
    242,   0,  16,   0,   6,   0, 
      0,   0,  70,  14,  16,   0, 
      6,   0,   0,   0,   2,  64, 
      0,   0, 140, 238,  42,  71, 
    140, 238,  42,  71, 140, 238, 
     42,  71, 140, 238,  42,  71, 
     26,   0,   0,   5, 242,   0, 
     16,   0,   6,   0,   0,   0, 
     70,  14,  16,   0,   6,   0, 
      0,   0,   0,   0,   0,   8, 
    162,   0,  16,   0,   6,   0, 
      0,   0,   6,   8,  16, 128, 
     65,   0,   0,   0,   6,   0, 
      0,   0,  86,  13,  16,   0, 
      6,   0,   0,   0,  50,   0, 
      0,   9,  50,   0,  16,   0, 
      6,   0,   0,   0,  86,   5, 
     16,   0,   5,   0,   0,   0, 
    214,   5,  16,   0,   6,   0, 
      0,   0, 134,   0,  16,   0, 
      6,   0,   0,   0,   0,   0, 
      0,   8, 130,   0,  16,   0, 
      2,   0,   0,   0,  58,   0, 
     16, 128,  65,   0,   0,   0, 
      0,   0,   0,   0,  10,   0, 
     16,   0,   6,   0,   0,   0, 
     50,   0,   0,   9, 130,   0, 
     16,   0,   0,   0,   0,   0, 
     26,   0,  16,   0,   5,   0, 
      0,   0,  58,   0,  16,   0, 
      2,   0,   0,   0,  58,   0, 
     16,   0,   0,   0,   0,   0, 
      0,   0,   0,   8, 130,   0, 
     16,   0,   2,   0,   0,   0, 
     10,   0,  16, 128,  65,   0, 
      0,   0,   5,   0,   0,   0, 
     42,   0,  16,   0,   5,   0, 
      0,   0,  50,   0,   0,   9, 
    130,   0,  16,   0,   2,   0, 
      0,   0,  26,   0,  16,   0, 
      5,   0,   0,   0,  58,   0, 
     16,   0,   2,   0,   0,   0, 
     10,   0,  16,   0,   5,   0, 
      0,   0,   0,   0,   0,   8, 
    130,   0,  16,   0,   2,   0, 
      0,   0,  26,   0,  16, 128, 
     65,   0,   0,   0,   6,   0, 
      0,   0,  58,   0,  16,   0, 
      2,   0,   0,   0,  50,   0, 
      0,   9, 130,   0,  16,   0, 
      2,   0,   0,   0,  26,   0, 
     16,   0,   5,   0,   0,   0, 
     58,   0,  16,   0,   2,   0, 
      0,   0,  26,   0,  16,   0, 
      6,   0,   0,   0,   0,   0, 
      0,   8, 130,   0,  16,   0, 
      2,   0,   0,   0,  58,   0, 
     16, 128,  65,   0,   0,   0, 
      0,   0,   0,   0,  58,   0, 
     16,   0,   2,   0,   0,   0, 
     50,   0,   0,   9, 130,   0, 
     16,   0,   0,   0,   0,   0, 
     26,   0,  16,   0,   5,   0, 
      0,   0,  58,   0,  16,   0, 
      2,   0,   0,   0,  58,   0, 
     16,   0,   0,   0,   0,   0, 
     56,   0,   0,   7,  66,   0, 
     16,   0,   0,   0,   0,   0, 
     58,   0,  16,   0,   0,   0, 
      0,   0,  58,   0,  16,   0, 
      3,   0,   0,   0,  50,   0, 
      0,   9, 114,   0,  16,   0, 
      0,   0,   0,   0,  70,   2, 
     16,   0,   0,   0,   0,   0, 
     86,   5,  16,   0,   1,   0, 
      0,   0,  70,   2,  16,   0, 
      2,   0,   0,   0,  56,   0, 
      0,   7, 130,   0,  16,   0, 
      0,   0,   0,   0,  26,   0, 
     16,   0,   1,   0,   0,   0, 
     26,   0,  16,   0,   1,   0, 
      0,   0,  54,   0,   0,   5, 
     66,   0,  16,   0,   3,   0, 
      0,   0,  10,   0,  16,   0, 
      1,   0,   0,   0,  50,   0, 
      0,   9, 114,   0,  16,   0, 
      0,   0,   0,   0, 246,  15, 
     16,   0,   0,   0,   0,   0, 
     70,   2,  16,   0,   3,   0, 
      0,   0,  70,   2,  16,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   9, 114,   0,  16,   0, 
      2,   0,   0,   0,  38,   9, 
     16, 128,  65,   0,   0,   0, 
      0,   0,   0,   0,  38, 137, 
     32,   0,   0,   0,   0,   0, 
     10,   0,   0,   0,  16,   0, 
      0,   7, 130,   0,  16,   0, 
      0,   0,   0,   0,  70,   2, 
     16,   0,   2,   0,   0,   0, 
     70,   2,  16,   0,   2,   0, 
      0,   0,  68,   0,   0,   5, 
    130,   0,  16,   0,   0,   0, 
      0,   0,  58,   0,  16,   0, 
      0,   0,   0,   0,  56,   0, 
      0,   7, 114,   0,  16,   0, 
      2,   0,   0,   0, 246,  15, 
     16,   0,   0,   0,   0,   0, 
     70,   2,  16,   0,   2,   0, 
      0,   0,  56,   0,   0,  10, 
    114,   0,  16,   0,   3,   0, 
      0,   0, 150,   4,  16,   0, 
      2,   0,   0,   0,   2,  64, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
    128,  63,   0,   0,   0,   0, 
     50,   0,   0,  13, 114,   0, 
     16,   0,   3,   0,   0,   0, 
     70,   2,  16,   0,   2,   0, 
      0,   0,   2,  64,   0,   0, 
      0,   0,   0,   0,   0,   0, 
    128,  63,   0,   0,   0,   0, 
      0,   0,   0,   0,  70,   2, 
     16, 128,  65,   0,   0,   0, 
      3,   0,   0,   0,  56,   0, 
      0,   7, 114,   0,  16,   0, 
      5,   0,   0,   0,  70,   2, 
     16,   0,   2,   0,   0,   0, 
     70,   2,  16,   0,   3,   0, 
      0,   0,  50,   0,   0,  10, 
    114,   0,  16,   0,   2,   0, 
      0,   0,  38,   9,  16,   0, 
      2,   0,   0,   0, 150,   4, 
     16,   0,   3,   0,   0,   0, 
     70,   2,  16, 128,  65,   0, 
      0,   0,   5,   0,   0,   0, 
     54,   0,   0,   5, 130,   0, 
     16,   0,   4,   0,   0,   0, 
     58,   0,  16,   0,   1,   0, 
      0,   0,  50,  32,   0,   9, 
     50,   0,  16,   0,   1,   0, 
      0,   0, 150,   5,  16,   0, 
      4,   0,   0,   0,  86,   5, 
     16,   0,   1,   0,   0,   0, 
     54,  15,  16,   0,   4,   0, 
      0,   0,   1,   0,   0,  10, 
    146,   0,  16,   0,   3,   0, 
      0,   0,   6,  16,  16,   0, 
      0,   0,   0,   0,   2,  64, 
      0,   0,   1,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   2,   0,   0,   0, 
     55,   0,   0,  15,  50,   0, 
     16,   0,   4,   0,   0,   0, 
     54,  15,  16,   0,   3,   0, 
      0,   0,   2,  64,   0,   0, 
      0,   0,   0,  63,   0,   0, 
      0,  63,   0,   0,   0,   0, 
      0,   0,   0,   0,   2,  64, 
      0,   0,   0,   0,   0, 191, 
      0,   0,   0, 191,   0,   0, 
      0,   0,   0,   0,   0,   0, 
     55,   0,   0,  11,  50,  32, 
     16,   0,   1,   0,   0,   0, 
    198,   0,  16,   0,   3,   0, 
      0,   0, 230, 138,  32,   0, 
      0,   0,   0,   0,  11,   0, 
      0,   0,  70, 128,  32,   0, 
      0,   0,   0,   0,  11,   0, 
      0,   0,  56,   0,   0,   7, 
     50,   0,  16,   0,   1,   0, 
      0,   0,  70,   0,  16,   0, 
      1,   0,   0,   0,  70,   0, 
     16,   0,   4,   0,   0,   0, 
     56,   0,   0,   7, 114,   0, 
     16,   0,   2,   0,   0,   0, 
     86,   5,  16,   0,   1,   0, 
      0,   0,  70,   2,  16,   0, 
      2,   0,   0,   0,  56,   0, 
      0,   7,  50,   0,  16,   0, 
      1,   0,   0,   0,   6,   0, 
     16,   0,   1,   0,   0,   0, 
    102,  10,  16,   0,   3,   0, 
      0,   0,  56,   0,   0,  10, 
     82,   0,  16,   0,   3,   0, 
      0,   0,   6,   1,  16,   0, 
      1,   0,   0,   0,   2,  64, 
      0,   0,   0,   0,   0,  63, 
      0,   0,   0,   0,   0,   0, 
      0,  63,   0,   0,   0,   0, 
     54,   0,   0,   5,  34,   0, 
     16,   0,   3,   0,   0,   0, 
      1,  64,   0,   0,   0,   0, 
      0,   0,  50,   0,   0,  13, 
    178,   0,  16,   0,   1,   0, 
      0,   0,  70,   8,  16, 128, 
     65,   0,   0,   0,   2,   0, 
      0,   0,   2,  64,   0,   0, 
      0,   0,   0,  63,   0,   0, 
      0,  63,   0,   0,   0,   0, 
      0,   0,   0,  63,  70,   8, 
     16,   0,   3,   0,   0,   0, 
      0,   0,   0,   7, 114,   0, 
     16,   0,   0,   0,   0,   0, 
     70,   2,  16,   0,   0,   0, 
      0,   0,  70,   3,  16,   0, 
      1,   0,   0,   0,  54,   0, 
      0,   5, 130,   0,  16,   0, 
      0,   0,   0,   0,   1,  64, 
      0,   0,   0,   0, 128,  63, 
     17,   0,   0,   8,  18,  32, 
     16,   0,   0,   0,   0,   0, 
     70,  14,  16,   0,   0,   0, 
      0,   0,  70, 142,  32,   0, 
      0,   0,   0,   0,   2,   0, 
      0,   0,  17,   0,   0,   8, 
     34,  32,  16,   0,   0,   0, 
      0,   0,  70,  14,  16,   0, 
      0,   0,   0,   0,  70, 142, 
     32,   0,   0,   0,   0,   0, 
      3,   0,   0,   0,  17,   0, 
      0,   8,  66,  32,  16,   0, 
      0,   0,   0,   0,  70,  14, 
     16,   0,   0,   0,   0,   0, 
     70, 142,  32,   0,   0,   0, 
      0,   0,   4,   0,   0,   0, 
     17,   0,   0,   8, 130,  32, 
     16,   0,   0,   0,   0,   0, 
     70,  14,  16,   0,   0,   0, 
      0,   0,  70, 142,  32,   0, 
      0,   0,   0,   0,   5,   0, 
      0,   0,   0,   0,   0,  10, 
    242,   0,  16,   0,   0,   0, 
      0,   0,  70, 142,  32, 128, 
     65,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
     70, 142,  32,   0,   0,   0, 
      0,   0,   1,   0,   0,   0, 
     50,   0,   0,  10, 242,  32, 
     16,   0,   2,   0,   0,   0, 
    166,  10,  16,   0,   1,   0, 
      0,   0,  70,  14,  16,   0, 
      0,   0,   0,   0,  70, 142, 
     32,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,  62,   0, 
      0,   1,  83,  70,  73,  48, 
      8,   0,   0,   0,   2,   0, 
      0,   0,   0,   0,   0,   0
};
