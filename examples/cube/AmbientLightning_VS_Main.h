#if 0
//
// Generated by Microsoft (R) D3D Shader Disassembler
//
//
//   fxc /nologo AmbientLightning.hlsl /Tvs_4_0 /Zi /Zpc /Qstrip_reflect
//    /Qstrip_debug /EVS_Main /FhAmbientLightning_VS_Main.h
//    /VnAmbientLightning_VS_Main
//
//
// Input signature:
//
// Name                 Index   Mask Register SysValue Format   Used
// -------------------- ----- ------ -------- -------- ------ ------
// POSITION                 0   xyzw        0     NONE  float   xyz 
// COLOR                    0   xyzw        1     NONE  float   xyzw
// NORMAL                   0   xyz         2     NONE  float   xyz 
//
//
// Output signature:
//
// Name                 Index   Mask Register SysValue Format   Used
// -------------------- ----- ------ -------- -------- ------ ------
// SV_POSITION              0   xyzw        0      POS  float   xyzw
// COLOR                    0   xyzw        1     NONE  float   xyzw
// NORMAL                   0   xyz         2     NONE  float   xyz 
//
vs_4_0
dcl_constantbuffer cb0[8], immediateIndexed
dcl_input v0.xyz
dcl_input v1.xyzw
dcl_input v2.xyz
dcl_output_siv o0.xyzw, position
dcl_output o1.xyzw
dcl_output o2.xyz
dcl_temps 2
mov r0.xyz, v0.xyzx
mov r0.w, l(1.000000)
dp4 r1.x, r0.xyzw, cb0[4].xyzw
dp4 r1.y, r0.xyzw, cb0[5].xyzw
dp4 r1.z, r0.xyzw, cb0[6].xyzw
dp4 r1.w, r0.xyzw, cb0[7].xyzw
dp4 o0.x, r1.xyzw, cb0[0].xyzw
dp4 o0.y, r1.xyzw, cb0[1].xyzw
dp4 o0.z, r1.xyzw, cb0[2].xyzw
dp4 o0.w, r1.xyzw, cb0[3].xyzw
mov o1.xyzw, v1.xyzw
dp3 r0.x, v2.xyzx, cb0[4].xyzx
dp3 r0.y, v2.xyzx, cb0[5].xyzx
dp3 r0.z, v2.xyzx, cb0[6].xyzx
dp3 r0.w, r0.xyzx, r0.xyzx
rsq r0.w, r0.w
mul o2.xyz, r0.wwww, r0.xyzx
ret 
// Approximately 0 instruction slots used
#endif

const BYTE AmbientLightning_VS_Main[] =
{
     68,  88,  66,  67, 220, 243, 
    173, 141,  67,  78,  79, 212, 
     59, 230, 174, 115, 151, 190, 
    190,  60,   1,   0,   0,   0, 
    112,   3,   0,   0,   3,   0, 
      0,   0,  44,   0,   0,   0, 
    156,   0,   0,   0,  16,   1, 
      0,   0,  73,  83,  71,  78, 
    104,   0,   0,   0,   3,   0, 
      0,   0,   8,   0,   0,   0, 
     80,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      3,   0,   0,   0,   0,   0, 
      0,   0,  15,   7,   0,   0, 
     89,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      3,   0,   0,   0,   1,   0, 
      0,   0,  15,  15,   0,   0, 
     95,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      3,   0,   0,   0,   2,   0, 
      0,   0,   7,   7,   0,   0, 
     80,  79,  83,  73,  84,  73, 
     79,  78,   0,  67,  79,  76, 
     79,  82,   0,  78,  79,  82, 
     77,  65,  76,   0, 171, 171, 
     79,  83,  71,  78, 108,   0, 
      0,   0,   3,   0,   0,   0, 
      8,   0,   0,   0,  80,   0, 
      0,   0,   0,   0,   0,   0, 
      1,   0,   0,   0,   3,   0, 
      0,   0,   0,   0,   0,   0, 
     15,   0,   0,   0,  92,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   3,   0, 
      0,   0,   1,   0,   0,   0, 
     15,   0,   0,   0,  98,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   3,   0, 
      0,   0,   2,   0,   0,   0, 
      7,   8,   0,   0,  83,  86, 
     95,  80,  79,  83,  73,  84, 
     73,  79,  78,   0,  67,  79, 
     76,  79,  82,   0,  78,  79, 
     82,  77,  65,  76,   0, 171, 
    171, 171,  83,  72,  68,  82, 
     88,   2,   0,   0,  64,   0, 
      1,   0, 150,   0,   0,   0, 
     89,   0,   0,   4,  70, 142, 
     32,   0,   0,   0,   0,   0, 
      8,   0,   0,   0,  95,   0, 
      0,   3, 114,  16,  16,   0, 
      0,   0,   0,   0,  95,   0, 
      0,   3, 242,  16,  16,   0, 
      1,   0,   0,   0,  95,   0, 
      0,   3, 114,  16,  16,   0, 
      2,   0,   0,   0, 103,   0, 
      0,   4, 242,  32,  16,   0, 
      0,   0,   0,   0,   1,   0, 
      0,   0, 101,   0,   0,   3, 
    242,  32,  16,   0,   1,   0, 
      0,   0, 101,   0,   0,   3, 
    114,  32,  16,   0,   2,   0, 
      0,   0, 104,   0,   0,   2, 
      2,   0,   0,   0,  54,   0, 
      0,   5, 114,   0,  16,   0, 
      0,   0,   0,   0,  70,  18, 
     16,   0,   0,   0,   0,   0, 
     54,   0,   0,   5, 130,   0, 
     16,   0,   0,   0,   0,   0, 
      1,  64,   0,   0,   0,   0, 
    128,  63,  17,   0,   0,   8, 
     18,   0,  16,   0,   1,   0, 
      0,   0,  70,  14,  16,   0, 
      0,   0,   0,   0,  70, 142, 
     32,   0,   0,   0,   0,   0, 
      4,   0,   0,   0,  17,   0, 
      0,   8,  34,   0,  16,   0, 
      1,   0,   0,   0,  70,  14, 
     16,   0,   0,   0,   0,   0, 
     70, 142,  32,   0,   0,   0, 
      0,   0,   5,   0,   0,   0, 
     17,   0,   0,   8,  66,   0, 
     16,   0,   1,   0,   0,   0, 
     70,  14,  16,   0,   0,   0, 
      0,   0,  70, 142,  32,   0, 
      0,   0,   0,   0,   6,   0, 
      0,   0,  17,   0,   0,   8, 
    130,   0,  16,   0,   1,   0, 
      0,   0,  70,  14,  16,   0, 
      0,   0,   0,   0,  70, 142, 
     32,   0,   0,   0,   0,   0, 
      7,   0,   0,   0,  17,   0, 
      0,   8,  18,  32,  16,   0, 
      0,   0,   0,   0,  70,  14, 
     16,   0,   1,   0,   0,   0, 
     70, 142,  32,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
     17,   0,   0,   8,  34,  32, 
     16,   0,   0,   0,   0,   0, 
     70,  14,  16,   0,   1,   0, 
      0,   0,  70, 142,  32,   0, 
      0,   0,   0,   0,   1,   0, 
      0,   0,  17,   0,   0,   8, 
     66,  32,  16,   0,   0,   0, 
      0,   0,  70,  14,  16,   0, 
      1,   0,   0,   0,  70, 142, 
     32,   0,   0,   0,   0,   0, 
      2,   0,   0,   0,  17,   0, 
      0,   8, 130,  32,  16,   0, 
      0,   0,   0,   0,  70,  14, 
     16,   0,   1,   0,   0,   0, 
     70, 142,  32,   0,   0,   0, 
      0,   0,   3,   0,   0,   0, 
     54,   0,   0,   5, 242,  32, 
     16,   0,   1,   0,   0,   0, 
     70,  30,  16,   0,   1,   0, 
      0,   0,  16,   0,   0,   8, 
     18,   0,  16,   0,   0,   0, 
      0,   0,  70,  18,  16,   0, 
      2,   0,   0,   0,  70, 130, 
     32,   0,   0,   0,   0,   0, 
      4,   0,   0,   0,  16,   0, 
      0,   8,  34,   0,  16,   0, 
      0,   0,   0,   0,  70,  18, 
     16,   0,   2,   0,   0,   0, 
     70, 130,  32,   0,   0,   0, 
      0,   0,   5,   0,   0,   0, 
     16,   0,   0,   8,  66,   0, 
     16,   0,   0,   0,   0,   0, 
     70,  18,  16,   0,   2,   0, 
      0,   0,  70, 130,  32,   0, 
      0,   0,   0,   0,   6,   0, 
      0,   0,  16,   0,   0,   7, 
    130,   0,  16,   0,   0,   0, 
      0,   0,  70,   2,  16,   0, 
      0,   0,   0,   0,  70,   2, 
     16,   0,   0,   0,   0,   0, 
     68,   0,   0,   5, 130,   0, 
     16,   0,   0,   0,   0,   0, 
     58,   0,  16,   0,   0,   0, 
      0,   0,  56,   0,   0,   7, 
    114,  32,  16,   0,   2,   0, 
      0,   0, 246,  15,  16,   0, 
      0,   0,   0,   0,  70,   2, 
     16,   0,   0,   0,   0,   0, 
     62,   0,   0,   1
};
