#if 0
//
// Generated by Microsoft (R) D3D Shader Disassembler
//
//
//   fxc /nologo desaturate.fx /Tcs_4_0 /Zi /Zpc /Qstrip_reflect /Qstrip_debug
//    /ECSMain /Fhdesaturate_CSMain.h /Vndesaturate_CSMain
//
//
// Input signature:
//
// Name                 Index   Mask Register SysValue Format   Used
// -------------------- ----- ------ -------- -------- ------ ------
// no Input
//
// Output signature:
//
// Name                 Index   Mask Register SysValue Format   Used
// -------------------- ----- ------ -------- -------- ------ ------
// no Output
cs_4_0
dcl_globalFlags refactoringAllowed
dcl_resource_structured t0, 4 
dcl_uav_structured u0, 4
dcl_input vThreadID.xy
dcl_temps 3
dcl_thread_group 32, 16, 1
ishl r0.x, vThreadID.y, l(10)
iadd r0.x, vThreadID.x, r0.x
ld_structured r1.x, r0.x, l(0), t0.xxxx
ushr r2.x, r1.x, l(8)
ushr r2.y, r1.x, l(16)
and r0.y, r1.x, l(255)
and r0.zw, r2.xxxy, l(0, 0, 255, 255)
utof r0.yzw, r0.yyzw
mul r0.z, r0.z, l(0.002314)
mad r0.y, r0.y, l(0.001176), r0.z
mad r0.y, r0.w, l(0.000431), r0.y
mul r0.y, r0.y, l(255.000000)
ftoi r0.y, r0.y
ishl r0.z, r0.y, l(8)
iadd r0.z, r0.y, r0.z
ishl r0.y, r0.y, l(16)
iadd r0.y, r0.z, r0.y
store_structured u0.x, r0.x, l(0), r0.y
ret 
// Approximately 0 instruction slots used
#endif

const BYTE desaturate_CSMain[] =
{
     68,  88,  66,  67, 127,  35, 
    180,  43, 105, 156, 190,  38, 
    126, 122, 234, 172, 137, 156, 
     71, 180,   1,   0,   0,   0, 
    176,   2,   0,   0,   3,   0, 
      0,   0,  44,   0,   0,   0, 
     60,   0,   0,   0,  76,   0, 
      0,   0,  73,  83,  71,  78, 
      8,   0,   0,   0,   0,   0, 
      0,   0,   8,   0,   0,   0, 
     79,  83,  71,  78,   8,   0, 
      0,   0,   0,   0,   0,   0, 
      8,   0,   0,   0,  83,  72, 
     69,  88,  92,   2,   0,   0, 
     64,   0,   5,   0, 151,   0, 
      0,   0, 106,   8,   0,   1, 
    162,   0,   0,   4,   0, 112, 
     16,   0,   0,   0,   0,   0, 
      4,   0,   0,   0, 158,   0, 
      0,   4,   0, 224,  17,   0, 
      0,   0,   0,   0,   4,   0, 
      0,   0,  95,   0,   0,   2, 
     50,   0,   2,   0, 104,   0, 
      0,   2,   3,   0,   0,   0, 
    155,   0,   0,   4,  32,   0, 
      0,   0,  16,   0,   0,   0, 
      1,   0,   0,   0,  41,   0, 
      0,   6,  18,   0,  16,   0, 
      0,   0,   0,   0,  26,   0, 
      2,   0,   1,  64,   0,   0, 
     10,   0,   0,   0,  30,   0, 
      0,   6,  18,   0,  16,   0, 
      0,   0,   0,   0,  10,   0, 
      2,   0,  10,   0,  16,   0, 
      0,   0,   0,   0, 167,   0, 
      0,   9,  18,   0,  16,   0, 
      1,   0,   0,   0,  10,   0, 
     16,   0,   0,   0,   0,   0, 
      1,  64,   0,   0,   0,   0, 
      0,   0,   6, 112,  16,   0, 
      0,   0,   0,   0,  85,   0, 
      0,   7,  18,   0,  16,   0, 
      2,   0,   0,   0,  10,   0, 
     16,   0,   1,   0,   0,   0, 
      1,  64,   0,   0,   8,   0, 
      0,   0,  85,   0,   0,   7, 
     34,   0,  16,   0,   2,   0, 
      0,   0,  10,   0,  16,   0, 
      1,   0,   0,   0,   1,  64, 
      0,   0,  16,   0,   0,   0, 
      1,   0,   0,   7,  34,   0, 
     16,   0,   0,   0,   0,   0, 
     10,   0,  16,   0,   1,   0, 
      0,   0,   1,  64,   0,   0, 
    255,   0,   0,   0,   1,   0, 
      0,  10, 194,   0,  16,   0, 
      0,   0,   0,   0,   6,   4, 
     16,   0,   2,   0,   0,   0, 
      2,  64,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
    255,   0,   0,   0, 255,   0, 
      0,   0,  86,   0,   0,   5, 
    226,   0,  16,   0,   0,   0, 
      0,   0,  86,  14,  16,   0, 
      0,   0,   0,   0,  56,   0, 
      0,   7,  66,   0,  16,   0, 
      0,   0,   0,   0,  42,   0, 
     16,   0,   0,   0,   0,   0, 
      1,  64,   0,   0, 223, 161, 
     23,  59,  50,   0,   0,   9, 
     34,   0,  16,   0,   0,   0, 
      0,   0,  26,   0,  16,   0, 
      0,   0,   0,   0,   1,  64, 
      0,   0, 205,  51, 154,  58, 
     42,   0,  16,   0,   0,   0, 
      0,   0,  50,   0,   0,   9, 
     34,   0,  16,   0,   0,   0, 
      0,   0,  58,   0,  16,   0, 
      0,   0,   0,   0,   1,  64, 
      0,   0, 216,  41, 226,  57, 
     26,   0,  16,   0,   0,   0, 
      0,   0,  56,   0,   0,   7, 
     34,   0,  16,   0,   0,   0, 
      0,   0,  26,   0,  16,   0, 
      0,   0,   0,   0,   1,  64, 
      0,   0,   0,   0, 127,  67, 
     27,   0,   0,   5,  34,   0, 
     16,   0,   0,   0,   0,   0, 
     26,   0,  16,   0,   0,   0, 
      0,   0,  41,   0,   0,   7, 
     66,   0,  16,   0,   0,   0, 
      0,   0,  26,   0,  16,   0, 
      0,   0,   0,   0,   1,  64, 
      0,   0,   8,   0,   0,   0, 
     30,   0,   0,   7,  66,   0, 
     16,   0,   0,   0,   0,   0, 
     26,   0,  16,   0,   0,   0, 
      0,   0,  42,   0,  16,   0, 
      0,   0,   0,   0,  41,   0, 
      0,   7,  34,   0,  16,   0, 
      0,   0,   0,   0,  26,   0, 
     16,   0,   0,   0,   0,   0, 
      1,  64,   0,   0,  16,   0, 
      0,   0,  30,   0,   0,   7, 
     34,   0,  16,   0,   0,   0, 
      0,   0,  42,   0,  16,   0, 
      0,   0,   0,   0,  26,   0, 
     16,   0,   0,   0,   0,   0, 
    168,   0,   0,   9,  18, 224, 
     17,   0,   0,   0,   0,   0, 
     10,   0,  16,   0,   0,   0, 
      0,   0,   1,  64,   0,   0, 
      0,   0,   0,   0,  26,   0, 
     16,   0,   0,   0,   0,   0, 
     62,   0,   0,   1
};
