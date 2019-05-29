#if 0
//
// Generated by Microsoft (R) D3D Shader Disassembler
//
//
//   fxc /nologo Instancing.hlsl /Tps_4_0 /Zi /Zpc /Qstrip_reflect
//    /Qstrip_debug /EPS_Main /FhInstancing_PS_Main.h /VnInstancing_PS_Main
//
//
// Input signature:
//
// Name                 Index   Mask Register SysValue Format   Used
// -------------------- ----- ------ -------- -------- ------ ------
// SV_POSITION              0   xyzw        0      POS  float       
// NORMAL                   0   xyz         1     NONE  float   xyz 
// COLOR                    0   xyzw        2     NONE  float   xyzw
//
//
// Output signature:
//
// Name                 Index   Mask Register SysValue Format   Used
// -------------------- ----- ------ -------- -------- ------ ------
// SV_TARGET                0   xyzw        0   TARGET  float   xyzw
//
ps_4_0
dcl_constantbuffer cb0[3], immediateIndexed
dcl_input_ps linear v1.xyz
dcl_input_ps linear v2.xyzw
dcl_output o0.xyzw
dcl_temps 1
dp3 r0.x, v1.xyzx, v1.xyzx
rsq r0.x, r0.x
mul r0.xyz, r0.xxxx, v1.xyzx
dp3_sat r0.x, r0.xyzx, -cb0[2].xyzx
lt r0.y, l(0.000000), r0.x
mad r0.xzw, cb0[1].xxyz, r0.xxxx, cb0[0].xxyz
movc_sat r0.xyz, r0.yyyy, r0.xzwx, cb0[0].xyzx
mul o0.xyz, r0.xyzx, v2.xyzx
mov o0.w, v2.w
ret 
// Approximately 0 instruction slots used
#endif

const BYTE Instancing_PS_Main[] =
{
     68,  88,  66,  67, 232, 109, 
     22, 201, 246, 191, 255, 253, 
    196, 202,  84, 228, 133,  70, 
    155,  51,   1,   0,   0,   0, 
     52,   2,   0,   0,   3,   0, 
      0,   0,  44,   0,   0,   0, 
    160,   0,   0,   0, 212,   0, 
      0,   0,  73,  83,  71,  78, 
    108,   0,   0,   0,   3,   0, 
      0,   0,   8,   0,   0,   0, 
     80,   0,   0,   0,   0,   0, 
      0,   0,   1,   0,   0,   0, 
      3,   0,   0,   0,   0,   0, 
      0,   0,  15,   0,   0,   0, 
     92,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      3,   0,   0,   0,   1,   0, 
      0,   0,   7,   7,   0,   0, 
     99,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      3,   0,   0,   0,   2,   0, 
      0,   0,  15,  15,   0,   0, 
     83,  86,  95,  80,  79,  83, 
     73,  84,  73,  79,  78,   0, 
     78,  79,  82,  77,  65,  76, 
      0,  67,  79,  76,  79,  82, 
      0, 171, 171, 171,  79,  83, 
     71,  78,  44,   0,   0,   0, 
      1,   0,   0,   0,   8,   0, 
      0,   0,  32,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   3,   0,   0,   0, 
      0,   0,   0,   0,  15,   0, 
      0,   0,  83,  86,  95,  84, 
     65,  82,  71,  69,  84,   0, 
    171, 171,  83,  72,  68,  82, 
     88,   1,   0,   0,  64,   0, 
      0,   0,  86,   0,   0,   0, 
     89,   0,   0,   4,  70, 142, 
     32,   0,   0,   0,   0,   0, 
      3,   0,   0,   0,  98,  16, 
      0,   3, 114,  16,  16,   0, 
      1,   0,   0,   0,  98,  16, 
      0,   3, 242,  16,  16,   0, 
      2,   0,   0,   0, 101,   0, 
      0,   3, 242,  32,  16,   0, 
      0,   0,   0,   0, 104,   0, 
      0,   2,   1,   0,   0,   0, 
     16,   0,   0,   7,  18,   0, 
     16,   0,   0,   0,   0,   0, 
     70,  18,  16,   0,   1,   0, 
      0,   0,  70,  18,  16,   0, 
      1,   0,   0,   0,  68,   0, 
      0,   5,  18,   0,  16,   0, 
      0,   0,   0,   0,  10,   0, 
     16,   0,   0,   0,   0,   0, 
     56,   0,   0,   7, 114,   0, 
     16,   0,   0,   0,   0,   0, 
      6,   0,  16,   0,   0,   0, 
      0,   0,  70,  18,  16,   0, 
      1,   0,   0,   0,  16,  32, 
      0,   9,  18,   0,  16,   0, 
      0,   0,   0,   0,  70,   2, 
     16,   0,   0,   0,   0,   0, 
     70, 130,  32, 128,  65,   0, 
      0,   0,   0,   0,   0,   0, 
      2,   0,   0,   0,  49,   0, 
      0,   7,  34,   0,  16,   0, 
      0,   0,   0,   0,   1,  64, 
      0,   0,   0,   0,   0,   0, 
     10,   0,  16,   0,   0,   0, 
      0,   0,  50,   0,   0,  11, 
    210,   0,  16,   0,   0,   0, 
      0,   0,   6, 137,  32,   0, 
      0,   0,   0,   0,   1,   0, 
      0,   0,   6,   0,  16,   0, 
      0,   0,   0,   0,   6, 137, 
     32,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,  55,  32, 
      0,  10, 114,   0,  16,   0, 
      0,   0,   0,   0,  86,   5, 
     16,   0,   0,   0,   0,   0, 
    134,   3,  16,   0,   0,   0, 
      0,   0,  70, 130,  32,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,  56,   0,   0,   7, 
    114,  32,  16,   0,   0,   0, 
      0,   0,  70,   2,  16,   0, 
      0,   0,   0,   0,  70,  18, 
     16,   0,   2,   0,   0,   0, 
     54,   0,   0,   5, 130,  32, 
     16,   0,   0,   0,   0,   0, 
     58,  16,  16,   0,   2,   0, 
      0,   0,  62,   0,   0,   1
};
