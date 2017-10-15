#if 0
//
// Generated by Microsoft (R) D3D Shader Disassembler
//
//
//   fxc /nologo Blurr.hlsl /Tps_4_0 /Zi /Zpc /Qstrip_reflect /Qstrip_debug
//    /EPS_Main /FhBlurr_PS_Main.h /VnBlurr_PS_Main
//
//
// Input signature:
//
// Name                 Index   Mask Register SysValue Format   Used
// -------------------- ----- ------ -------- -------- ------ ------
// SV_POSITION              0   xyzw        0      POS  float       
// TEXCOORD                 0   xy          1     NONE  float   xy  
//
//
// Output signature:
//
// Name                 Index   Mask Register SysValue Format   Used
// -------------------- ----- ------ -------- -------- ------ ------
// SV_TARGET                0   xyzw        0   TARGET  float   xyzw
//
ps_4_0
dcl_immediateConstantBuffer { { 1.000000, 0, 0, 0},
                              { 0, 1.000000, 0, 0},
                              { 0, 0, 1.000000, 0},
                              { 0, 0, 0, 1.000000} }
dcl_constantbuffer cb0[9], immediateIndexed
dcl_sampler s0, mode_default
dcl_resource_texture2d (float,float,float,float) t0
dcl_input_ps linear v1.xy
dcl_output o0.xyzw
dcl_temps 6
mov r0.xyzw, l(0,0,0,0)
mov r1.x, l(0)
loop 
  ige r1.y, r1.x, l(4)
  breakc_nz r1.y
  dp4 r2.x, cb0[0].xyzw, icb[r1.x + 0].xyzw
  dp4 r2.y, cb0[1].xyzw, icb[r1.x + 0].xyzw
  dp4 r2.z, cb0[2].xyzw, icb[r1.x + 0].xyzw
  dp4 r2.w, cb0[3].xyzw, icb[r1.x + 0].xyzw
  dp4 r3.x, cb0[4].xyzw, icb[r1.x + 0].xyzw
  dp4 r3.y, cb0[5].xyzw, icb[r1.x + 0].xyzw
  dp4 r3.z, cb0[6].xyzw, icb[r1.x + 0].xyzw
  dp4 r3.w, cb0[7].xyzw, icb[r1.x + 0].xyzw
  mov r4.xyzw, r0.xyzw
  mov r1.y, l(0)
  loop 
    ige r1.z, r1.y, l(4)
    breakc_nz r1.z
    dp4 r1.z, r2.xyzw, icb[r1.y + 0].xyzw
    div r1.zw, r1.zzzz, cb0[8].zzzw
    mad r1.zw, cb0[8].xxxy, r1.zzzw, v1.xxxy
    sample r5.xyzw, r1.zwzz, t0.xyzw, s0
    dp4 r1.z, r3.xyzw, icb[r1.y + 0].xyzw
    mad r4.xyzw, r5.xyzw, r1.zzzz, r4.xyzw
    iadd r1.y, r1.y, l(1)
  endloop 
  mov r0.xyzw, r4.xyzw
  iadd r1.x, r1.x, l(1)
endloop 
mov o0.xyzw, r0.xyzw
ret 
// Approximately 0 instruction slots used
#endif

const BYTE Blurr_PS_Main[] =
{
     68,  88,  66,  67,  11,  20, 
     70,  93,  29,  42, 225,  34, 
     57, 236, 235,   1, 194,  49, 
    145,  19,   1,   0,   0,   0, 
    108,   4,   0,   0,   3,   0, 
      0,   0,  44,   0,   0,   0, 
    132,   0,   0,   0, 184,   0, 
      0,   0,  73,  83,  71,  78, 
     80,   0,   0,   0,   2,   0, 
      0,   0,   8,   0,   0,   0, 
     56,   0,   0,   0,   0,   0, 
      0,   0,   1,   0,   0,   0, 
      3,   0,   0,   0,   0,   0, 
      0,   0,  15,   0,   0,   0, 
     68,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      3,   0,   0,   0,   1,   0, 
      0,   0,   3,   3,   0,   0, 
     83,  86,  95,  80,  79,  83, 
     73,  84,  73,  79,  78,   0, 
     84,  69,  88,  67,  79,  79, 
     82,  68,   0, 171, 171, 171, 
     79,  83,  71,  78,  44,   0, 
      0,   0,   1,   0,   0,   0, 
      8,   0,   0,   0,  32,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   3,   0, 
      0,   0,   0,   0,   0,   0, 
     15,   0,   0,   0,  83,  86, 
     95,  84,  65,  82,  71,  69, 
     84,   0, 171, 171,  83,  72, 
     68,  82, 172,   3,   0,   0, 
     64,   0,   0,   0, 235,   0, 
      0,   0,  53,  24,   0,   0, 
     18,   0,   0,   0,   0,   0, 
    128,  63,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0, 128,  63,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0, 128,  63, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
    128,  63,  89,   0,   0,   4, 
     70, 142,  32,   0,   0,   0, 
      0,   0,   9,   0,   0,   0, 
     90,   0,   0,   3,   0,  96, 
     16,   0,   0,   0,   0,   0, 
     88,  24,   0,   4,   0, 112, 
     16,   0,   0,   0,   0,   0, 
     85,  85,   0,   0,  98,  16, 
      0,   3,  50,  16,  16,   0, 
      1,   0,   0,   0, 101,   0, 
      0,   3, 242,  32,  16,   0, 
      0,   0,   0,   0, 104,   0, 
      0,   2,   6,   0,   0,   0, 
     54,   0,   0,   8, 242,   0, 
     16,   0,   0,   0,   0,   0, 
      2,  64,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,  54,   0,   0,   5, 
     18,   0,  16,   0,   1,   0, 
      0,   0,   1,  64,   0,   0, 
      0,   0,   0,   0,  48,   0, 
      0,   1,  33,   0,   0,   7, 
     34,   0,  16,   0,   1,   0, 
      0,   0,  10,   0,  16,   0, 
      1,   0,   0,   0,   1,  64, 
      0,   0,   4,   0,   0,   0, 
      3,   0,   4,   3,  26,   0, 
     16,   0,   1,   0,   0,   0, 
     17,   0,   0,   9,  18,   0, 
     16,   0,   2,   0,   0,   0, 
     70, 142,  32,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
     70, 158, 144,   0,  10,   0, 
     16,   0,   1,   0,   0,   0, 
     17,   0,   0,   9,  34,   0, 
     16,   0,   2,   0,   0,   0, 
     70, 142,  32,   0,   0,   0, 
      0,   0,   1,   0,   0,   0, 
     70, 158, 144,   0,  10,   0, 
     16,   0,   1,   0,   0,   0, 
     17,   0,   0,   9,  66,   0, 
     16,   0,   2,   0,   0,   0, 
     70, 142,  32,   0,   0,   0, 
      0,   0,   2,   0,   0,   0, 
     70, 158, 144,   0,  10,   0, 
     16,   0,   1,   0,   0,   0, 
     17,   0,   0,   9, 130,   0, 
     16,   0,   2,   0,   0,   0, 
     70, 142,  32,   0,   0,   0, 
      0,   0,   3,   0,   0,   0, 
     70, 158, 144,   0,  10,   0, 
     16,   0,   1,   0,   0,   0, 
     17,   0,   0,   9,  18,   0, 
     16,   0,   3,   0,   0,   0, 
     70, 142,  32,   0,   0,   0, 
      0,   0,   4,   0,   0,   0, 
     70, 158, 144,   0,  10,   0, 
     16,   0,   1,   0,   0,   0, 
     17,   0,   0,   9,  34,   0, 
     16,   0,   3,   0,   0,   0, 
     70, 142,  32,   0,   0,   0, 
      0,   0,   5,   0,   0,   0, 
     70, 158, 144,   0,  10,   0, 
     16,   0,   1,   0,   0,   0, 
     17,   0,   0,   9,  66,   0, 
     16,   0,   3,   0,   0,   0, 
     70, 142,  32,   0,   0,   0, 
      0,   0,   6,   0,   0,   0, 
     70, 158, 144,   0,  10,   0, 
     16,   0,   1,   0,   0,   0, 
     17,   0,   0,   9, 130,   0, 
     16,   0,   3,   0,   0,   0, 
     70, 142,  32,   0,   0,   0, 
      0,   0,   7,   0,   0,   0, 
     70, 158, 144,   0,  10,   0, 
     16,   0,   1,   0,   0,   0, 
     54,   0,   0,   5, 242,   0, 
     16,   0,   4,   0,   0,   0, 
     70,  14,  16,   0,   0,   0, 
      0,   0,  54,   0,   0,   5, 
     34,   0,  16,   0,   1,   0, 
      0,   0,   1,  64,   0,   0, 
      0,   0,   0,   0,  48,   0, 
      0,   1,  33,   0,   0,   7, 
     66,   0,  16,   0,   1,   0, 
      0,   0,  26,   0,  16,   0, 
      1,   0,   0,   0,   1,  64, 
      0,   0,   4,   0,   0,   0, 
      3,   0,   4,   3,  42,   0, 
     16,   0,   1,   0,   0,   0, 
     17,   0,   0,   8,  66,   0, 
     16,   0,   1,   0,   0,   0, 
     70,  14,  16,   0,   2,   0, 
      0,   0,  70, 158, 144,   0, 
     26,   0,  16,   0,   1,   0, 
      0,   0,  14,   0,   0,   8, 
    194,   0,  16,   0,   1,   0, 
      0,   0, 166,  10,  16,   0, 
      1,   0,   0,   0, 166, 142, 
     32,   0,   0,   0,   0,   0, 
      8,   0,   0,   0,  50,   0, 
      0,  10, 194,   0,  16,   0, 
      1,   0,   0,   0,   6, 132, 
     32,   0,   0,   0,   0,   0, 
      8,   0,   0,   0, 166,  14, 
     16,   0,   1,   0,   0,   0, 
      6,  20,  16,   0,   1,   0, 
      0,   0,  69,   0,   0,   9, 
    242,   0,  16,   0,   5,   0, 
      0,   0, 230,  10,  16,   0, 
      1,   0,   0,   0,  70, 126, 
     16,   0,   0,   0,   0,   0, 
      0,  96,  16,   0,   0,   0, 
      0,   0,  17,   0,   0,   8, 
     66,   0,  16,   0,   1,   0, 
      0,   0,  70,  14,  16,   0, 
      3,   0,   0,   0,  70, 158, 
    144,   0,  26,   0,  16,   0, 
      1,   0,   0,   0,  50,   0, 
      0,   9, 242,   0,  16,   0, 
      4,   0,   0,   0,  70,  14, 
     16,   0,   5,   0,   0,   0, 
    166,  10,  16,   0,   1,   0, 
      0,   0,  70,  14,  16,   0, 
      4,   0,   0,   0,  30,   0, 
      0,   7,  34,   0,  16,   0, 
      1,   0,   0,   0,  26,   0, 
     16,   0,   1,   0,   0,   0, 
      1,  64,   0,   0,   1,   0, 
      0,   0,  22,   0,   0,   1, 
     54,   0,   0,   5, 242,   0, 
     16,   0,   0,   0,   0,   0, 
     70,  14,  16,   0,   4,   0, 
      0,   0,  30,   0,   0,   7, 
     18,   0,  16,   0,   1,   0, 
      0,   0,  10,   0,  16,   0, 
      1,   0,   0,   0,   1,  64, 
      0,   0,   1,   0,   0,   0, 
     22,   0,   0,   1,  54,   0, 
      0,   5, 242,  32,  16,   0, 
      0,   0,   0,   0,  70,  14, 
     16,   0,   0,   0,   0,   0, 
     62,   0,   0,   1
};