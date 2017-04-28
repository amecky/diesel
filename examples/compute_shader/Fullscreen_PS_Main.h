#if 0
//
// Generated by Microsoft (R) D3D Shader Disassembler
//
//
//   fxc /nologo Fullscreen.fx /Tps_4_0 /Zi /Zpc /Qstrip_reflect /Qstrip_debug
//    /EPS_Main /FhFullscreen_PS_Main.h /VnFullscreen_PS_Main
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
dcl_sampler s0, mode_default
dcl_resource_texture2d (float,float,float,float) t0
dcl_resource_texture2d (float,float,float,float) t1
dcl_input_ps linear v1.xy
dcl_output o0.xyzw
dcl_temps 3
lt r0.x, v1.y, l(0.500000)
mul r0.yz, v1.xxyx, l(0.000000, 1.000000, 2.000000, 0.000000)
sample r1.xyzw, r0.yzyy, t0.xyzw, s0
add r0.yz, v1.xxyx, l(0.000000, 0.000000, -0.500000, 0.000000)
mul r0.yz, r0.yyzy, l(0.000000, 1.000000, 2.000000, 0.000000)
sample r2.xyzw, r0.yzyy, t1.xyzw, s0
if_nz r0.x
  mov o0.xyzw, r1.xyzw
  ret 
else 
  mov o0.xyz, r2.xyzx
  mov o0.w, l(1.000000)
  ret 
endif 
ret 
// Approximately 0 instruction slots used
#endif

const BYTE Fullscreen_PS_Main[] =
{
     68,  88,  66,  67, 184, 193, 
    149, 117,  16, 114,   5,  58, 
     12, 159,  29, 232, 149,  72, 
     64,  33,   1,   0,   0,   0, 
     76,   2,   0,   0,   3,   0, 
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
     68,  82, 140,   1,   0,   0, 
     64,   0,   0,   0,  99,   0, 
      0,   0,  90,   0,   0,   3, 
      0,  96,  16,   0,   0,   0, 
      0,   0,  88,  24,   0,   4, 
      0, 112,  16,   0,   0,   0, 
      0,   0,  85,  85,   0,   0, 
     88,  24,   0,   4,   0, 112, 
     16,   0,   1,   0,   0,   0, 
     85,  85,   0,   0,  98,  16, 
      0,   3,  50,  16,  16,   0, 
      1,   0,   0,   0, 101,   0, 
      0,   3, 242,  32,  16,   0, 
      0,   0,   0,   0, 104,   0, 
      0,   2,   3,   0,   0,   0, 
     49,   0,   0,   7,  18,   0, 
     16,   0,   0,   0,   0,   0, 
     26,  16,  16,   0,   1,   0, 
      0,   0,   1,  64,   0,   0, 
      0,   0,   0,  63,  56,   0, 
      0,  10,  98,   0,  16,   0, 
      0,   0,   0,   0,   6,  17, 
     16,   0,   1,   0,   0,   0, 
      2,  64,   0,   0,   0,   0, 
      0,   0,   0,   0, 128,  63, 
      0,   0,   0,  64,   0,   0, 
      0,   0,  69,   0,   0,   9, 
    242,   0,  16,   0,   1,   0, 
      0,   0, 150,   5,  16,   0, 
      0,   0,   0,   0,  70, 126, 
     16,   0,   0,   0,   0,   0, 
      0,  96,  16,   0,   0,   0, 
      0,   0,   0,   0,   0,  10, 
     98,   0,  16,   0,   0,   0, 
      0,   0,   6,  17,  16,   0, 
      1,   0,   0,   0,   2,  64, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0, 191,   0,   0,   0,   0, 
     56,   0,   0,  10,  98,   0, 
     16,   0,   0,   0,   0,   0, 
     86,   6,  16,   0,   0,   0, 
      0,   0,   2,  64,   0,   0, 
      0,   0,   0,   0,   0,   0, 
    128,  63,   0,   0,   0,  64, 
      0,   0,   0,   0,  69,   0, 
      0,   9, 242,   0,  16,   0, 
      2,   0,   0,   0, 150,   5, 
     16,   0,   0,   0,   0,   0, 
     70, 126,  16,   0,   1,   0, 
      0,   0,   0,  96,  16,   0, 
      0,   0,   0,   0,  31,   0, 
      4,   3,  10,   0,  16,   0, 
      0,   0,   0,   0,  54,   0, 
      0,   5, 242,  32,  16,   0, 
      0,   0,   0,   0,  70,  14, 
     16,   0,   1,   0,   0,   0, 
     62,   0,   0,   1,  18,   0, 
      0,   1,  54,   0,   0,   5, 
    114,  32,  16,   0,   0,   0, 
      0,   0,  70,   2,  16,   0, 
      2,   0,   0,   0,  54,   0, 
      0,   5, 130,  32,  16,   0, 
      0,   0,   0,   0,   1,  64, 
      0,   0,   0,   0, 128,  63, 
     62,   0,   0,   1,  21,   0, 
      0,   1,  62,   0,   0,   1
};
