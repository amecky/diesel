#if 0
//
// Generated by Microsoft (R) D3D Shader Disassembler
//
//
//   fxc /nologo desaturate.fx /Tcs_5_0 /Zi /Zpc /Qstrip_reflect /Qstrip_debug
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
cs_5_0
dcl_globalFlags refactoringAllowed
dcl_constantbuffer cb0[1], immediateIndexed
dcl_resource_structured t0, 4 
dcl_uav_typed_texture2d (float,float,float,float) u0
dcl_input vThreadID.xy
dcl_temps 2
dcl_thread_group 32, 16, 1
ishl r0.x, vThreadID.y, l(10)
iadd r0.x, vThreadID.x, r0.x
ld_structured_indexable(structured_buffer, stride=4)(mixed,mixed,mixed,mixed) r0.x, r0.x, l(0), t0.xxxx
and r0.y, r0.x, l(255)
ubfe r0.xz, l(8, 0, 8, 0), l(8, 0, 16, 0), r0.xxxx
utof r0.xz, r0.xxzx
mul r1.yz, r0.xxzx, l(0.000000, 0.003922, 0.003922, 0.000000)
utof r0.x, r0.y
mul r1.xw, r0.xxxx, l(0.003922, 0.000000, 0.000000, 0.003922)
dp3 r0.x, r1.wyzw, l(0.300000, 0.590000, 0.110000, 0.000000)
add r0.xyzw, -r1.wyzw, r0.xxxx
mad r0.xyzw, cb0[0].wwww, r0.xyzw, r1.xyzw
store_uav_typed u0.xyzw, vThreadID.xyyy, r0.xyzw
ret 
// Approximately 0 instruction slots used
#endif

const BYTE desaturate_CSMain[] =
{
     68,  88,  66,  67,  98, 156, 
    194,   9, 124,  62, 170,  29, 
     49,  85, 206, 100, 239,  91, 
    168,  33,   1,   0,   0,   0, 
    104,   2,   0,   0,   3,   0, 
      0,   0,  44,   0,   0,   0, 
     60,   0,   0,   0,  76,   0, 
      0,   0,  73,  83,  71,  78, 
      8,   0,   0,   0,   0,   0, 
      0,   0,   8,   0,   0,   0, 
     79,  83,  71,  78,   8,   0, 
      0,   0,   0,   0,   0,   0, 
      8,   0,   0,   0,  83,  72, 
     69,  88,  20,   2,   0,   0, 
     80,   0,   5,   0, 133,   0, 
      0,   0, 106,   8,   0,   1, 
     89,   0,   0,   4,  70, 142, 
     32,   0,   0,   0,   0,   0, 
      1,   0,   0,   0, 162,   0, 
      0,   4,   0, 112,  16,   0, 
      0,   0,   0,   0,   4,   0, 
      0,   0, 156,  24,   0,   4, 
      0, 224,  17,   0,   0,   0, 
      0,   0,  85,  85,   0,   0, 
     95,   0,   0,   2,  50,   0, 
      2,   0, 104,   0,   0,   2, 
      2,   0,   0,   0, 155,   0, 
      0,   4,  32,   0,   0,   0, 
     16,   0,   0,   0,   1,   0, 
      0,   0,  41,   0,   0,   6, 
     18,   0,  16,   0,   0,   0, 
      0,   0,  26,   0,   2,   0, 
      1,  64,   0,   0,  10,   0, 
      0,   0,  30,   0,   0,   6, 
     18,   0,  16,   0,   0,   0, 
      0,   0,  10,   0,   2,   0, 
     10,   0,  16,   0,   0,   0, 
      0,   0, 167,   0,   0, 139, 
      2,  35,   0, 128, 131, 153, 
     25,   0,  18,   0,  16,   0, 
      0,   0,   0,   0,  10,   0, 
     16,   0,   0,   0,   0,   0, 
      1,  64,   0,   0,   0,   0, 
      0,   0,   6, 112,  16,   0, 
      0,   0,   0,   0,   1,   0, 
      0,   7,  34,   0,  16,   0, 
      0,   0,   0,   0,  10,   0, 
     16,   0,   0,   0,   0,   0, 
      1,  64,   0,   0, 255,   0, 
      0,   0, 138,   0,   0,  15, 
     82,   0,  16,   0,   0,   0, 
      0,   0,   2,  64,   0,   0, 
      8,   0,   0,   0,   0,   0, 
      0,   0,   8,   0,   0,   0, 
      0,   0,   0,   0,   2,  64, 
      0,   0,   8,   0,   0,   0, 
      0,   0,   0,   0,  16,   0, 
      0,   0,   0,   0,   0,   0, 
      6,   0,  16,   0,   0,   0, 
      0,   0,  86,   0,   0,   5, 
     82,   0,  16,   0,   0,   0, 
      0,   0,   6,   2,  16,   0, 
      0,   0,   0,   0,  56,   0, 
      0,  10,  98,   0,  16,   0, 
      1,   0,   0,   0,   6,   2, 
     16,   0,   0,   0,   0,   0, 
      2,  64,   0,   0,   0,   0, 
      0,   0, 129, 128, 128,  59, 
    129, 128, 128,  59,   0,   0, 
      0,   0,  86,   0,   0,   5, 
     18,   0,  16,   0,   0,   0, 
      0,   0,  26,   0,  16,   0, 
      0,   0,   0,   0,  56,   0, 
      0,  10, 146,   0,  16,   0, 
      1,   0,   0,   0,   6,   0, 
     16,   0,   0,   0,   0,   0, 
      2,  64,   0,   0, 129, 128, 
    128,  59,   0,   0,   0,   0, 
      0,   0,   0,   0, 129, 128, 
    128,  59,  16,   0,   0,  10, 
     18,   0,  16,   0,   0,   0, 
      0,   0, 118,  14,  16,   0, 
      1,   0,   0,   0,   2,  64, 
      0,   0, 154, 153, 153,  62, 
     61,  10,  23,  63, 174,  71, 
    225,  61,   0,   0,   0,   0, 
      0,   0,   0,   8, 242,   0, 
     16,   0,   0,   0,   0,   0, 
    118,  14,  16, 128,  65,   0, 
      0,   0,   1,   0,   0,   0, 
      6,   0,  16,   0,   0,   0, 
      0,   0,  50,   0,   0,  10, 
    242,   0,  16,   0,   0,   0, 
      0,   0, 246, 143,  32,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,  70,  14,  16,   0, 
      0,   0,   0,   0,  70,  14, 
     16,   0,   1,   0,   0,   0, 
    164,   0,   0,   6, 242, 224, 
     17,   0,   0,   0,   0,   0, 
     70,   5,   2,   0,  70,  14, 
     16,   0,   0,   0,   0,   0, 
     62,   0,   0,   1
};