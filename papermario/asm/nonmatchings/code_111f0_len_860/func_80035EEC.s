.set noat # allow manual use of $at
.set noreorder # don't insert nops after branches

.include "include/macro.inc"

glabel func_80035EEC
/* 0112EC 80035EEC 27BDFFE8 */  addiu $sp, $sp, -0x18
/* 0112F0 80035EF0 AFB00010 */  sw    $s0, 0x10($sp)
/* 0112F4 80035EF4 3C10800A */  lui   $s0, 0x800a
/* 0112F8 80035EF8 26100944 */  addiu $s0, $s0, 0x944
/* 0112FC 80035EFC AFBF0014 */  sw    $ra, 0x14($sp)
/* 011300 80035F00 86030000 */  lh    $v1, ($s0)
/* 011304 80035F04 24020001 */  addiu $v0, $zero, 1
/* 011308 80035F08 1062003D */  beq   $v1, $v0, .L80036000
/* 01130C 80035F0C 28620002 */   slti  $v0, $v1, 2
/* 011310 80035F10 50400005 */  beql  $v0, $zero, .L80035F28
/* 011314 80035F14 24020002 */   addiu $v0, $zero, 2
/* 011318 80035F18 10600007 */  beqz  $v1, .L80035F38
/* 01131C 80035F1C 00000000 */   nop   
/* 011320 80035F20 0800D83B */  j     .L800360EC
/* 011324 80035F24 00000000 */   nop   

.L80035F28:
/* 011328 80035F28 10620055 */  beq   $v1, $v0, .L80036080
/* 01132C 80035F2C 00000000 */   nop   
/* 011330 80035F30 0800D83B */  j     .L800360EC
/* 011334 80035F34 00000000 */   nop   

.L80035F38:
/* 011338 80035F38 3C04800A */  lui   $a0, 0x800a
/* 01133C 80035F3C 24840946 */  addiu $a0, $a0, 0x946
/* 011340 80035F40 84820000 */  lh    $v0, ($a0)
/* 011344 80035F44 94830000 */  lhu   $v1, ($a0)
/* 011348 80035F48 1440003B */  bnez  $v0, .L80036038
/* 01134C 80035F4C 2462FFFF */   addiu $v0, $v1, -1
/* 011350 80035F50 3C038007 */  lui   $v1, 0x8007
/* 011354 80035F54 2463419C */  addiu $v1, $v1, 0x419c
/* 011358 80035F58 8C620000 */  lw    $v0, ($v1)
/* 01135C 80035F5C A0400070 */  sb    $zero, 0x70($v0)
/* 011360 80035F60 8C620000 */  lw    $v0, ($v1)
/* 011364 80035F64 A0400078 */  sb    $zero, 0x78($v0)
/* 011368 80035F68 3C02800A */  lui   $v0, 0x800a
/* 01136C 80035F6C 84420948 */  lh    $v0, 0x948($v0)
/* 011370 80035F70 14400006 */  bnez  $v0, .L80035F8C
/* 011374 80035F74 24060001 */   addiu $a2, $zero, 1
/* 011378 80035F78 8C620000 */  lw    $v0, ($v1)
/* 01137C 80035F7C 84440086 */  lh    $a0, 0x86($v0)
/* 011380 80035F80 8445008C */  lh    $a1, 0x8c($v0)
/* 011384 80035F84 0800D7E6 */  j     .L80035F98
/* 011388 80035F88 0000302D */   daddu $a2, $zero, $zero

.L80035F8C:
/* 01138C 80035F8C 8C620000 */  lw    $v0, ($v1)
/* 011390 80035F90 84440086 */  lh    $a0, 0x86($v0)
/* 011394 80035F94 8445008C */  lh    $a1, 0x8c($v0)
.L80035F98:
/* 011398 80035F98 0C0169BD */  jal   load_map_by_IDs
/* 01139C 80035F9C 00000000 */   nop   
/* 0113A0 80035FA0 3C108007 */  lui   $s0, 0x8007
/* 0113A4 80035FA4 2610419C */  addiu $s0, $s0, 0x419c
/* 0113A8 80035FA8 8E020000 */  lw    $v0, ($s0)
/* 0113AC 80035FAC 94430086 */  lhu   $v1, 0x86($v0)
/* 0113B0 80035FB0 0000202D */  daddu $a0, $zero, $zero
/* 0113B4 80035FB4 0C009C22 */  jal   func_80027088
/* 0113B8 80035FB8 A4430088 */   sh    $v1, 0x88($v0)
/* 0113BC 80035FBC 8E020000 */  lw    $v0, ($s0)
/* 0113C0 80035FC0 80420071 */  lb    $v0, 0x71($v0)
/* 0113C4 80035FC4 14400003 */  bnez  $v0, .L80035FD4
/* 0113C8 80035FC8 00000000 */   nop   
/* 0113CC 80035FCC 0C03805E */  jal   func_800E0178
/* 0113D0 80035FD0 00000000 */   nop   
.L80035FD4:
/* 0113D4 80035FD4 0C00B500 */  jal   update_cameras
/* 0113D8 80035FD8 00000000 */   nop   
/* 0113DC 80035FDC 3C02800A */  lui   $v0, 0x800a
/* 0113E0 80035FE0 24420944 */  addiu $v0, $v0, 0x944
/* 0113E4 80035FE4 94430000 */  lhu   $v1, ($v0)
/* 0113E8 80035FE8 24040002 */  addiu $a0, $zero, 2
/* 0113EC 80035FEC 3C01800A */  lui   $at, 0x800a
/* 0113F0 80035FF0 A4240946 */  sh    $a0, 0x946($at)
/* 0113F4 80035FF4 24630001 */  addiu $v1, $v1, 1
/* 0113F8 80035FF8 0800D83B */  j     .L800360EC
/* 0113FC 80035FFC A4430000 */   sh    $v1, ($v0)

.L80036000:
/* 011400 80036000 0C00F949 */  jal   func_8003E524
/* 011404 80036004 00000000 */   nop   
/* 011408 80036008 0C00E64C */  jal   func_80039930
/* 01140C 8003600C 00000000 */   nop   
/* 011410 80036010 0C037DD0 */  jal   func_800DF740
/* 011414 80036014 00000000 */   nop   
/* 011418 80036018 0C00B500 */  jal   update_cameras
/* 01141C 8003601C 00000000 */   nop   
/* 011420 80036020 3C04800A */  lui   $a0, 0x800a
/* 011424 80036024 24840946 */  addiu $a0, $a0, 0x946
/* 011428 80036028 84820000 */  lh    $v0, ($a0)
/* 01142C 8003602C 94830000 */  lhu   $v1, ($a0)
/* 011430 80036030 10400003 */  beqz  $v0, .L80036040
/* 011434 80036034 2462FFFF */   addiu $v0, $v1, -1
.L80036038:
/* 011438 80036038 0800D83B */  j     .L800360EC
/* 01143C 8003603C A4820000 */   sh    $v0, ($a0)

.L80036040:
/* 011440 80036040 3C028007 */  lui   $v0, 0x8007
/* 011444 80036044 8C42419C */  lw    $v0, 0x419c($v0)
/* 011448 80036048 0C0B1059 */  jal   does_script_exist
/* 01144C 8003604C 8C44006C */   lw    $a0, 0x6c($v0)
/* 011450 80036050 14400026 */  bnez  $v0, .L800360EC
/* 011454 80036054 00000000 */   nop   
/* 011458 80036058 3C05800A */  lui   $a1, 0x800a
/* 01145C 8003605C 24A5A650 */  addiu $a1, $a1, -0x59b0
/* 011460 80036060 2404FFF7 */  addiu $a0, $zero, -9
/* 011464 80036064 8CA20000 */  lw    $v0, ($a1)
/* 011468 80036068 96030000 */  lhu   $v1, ($s0)
/* 01146C 8003606C 00441024 */  and   $v0, $v0, $a0
/* 011470 80036070 24630001 */  addiu $v1, $v1, 1
/* 011474 80036074 ACA20000 */  sw    $v0, ($a1)
/* 011478 80036078 0800D83B */  j     .L800360EC
/* 01147C 8003607C A6030000 */   sh    $v1, ($s0)

.L80036080:
/* 011480 80036080 0C00E64C */  jal   func_80039930
/* 011484 80036084 00000000 */   nop   
/* 011488 80036088 0C037DD0 */  jal   func_800DF740
/* 01148C 8003608C 00000000 */   nop   
/* 011490 80036090 0C016754 */  jal   func_80059D50
/* 011494 80036094 00000000 */   nop   
/* 011498 80036098 0C00B500 */  jal   update_cameras
/* 01149C 8003609C 00000000 */   nop   
/* 0114A0 800360A0 3C04800A */  lui   $a0, 0x800a
/* 0114A4 800360A4 24840940 */  addiu $a0, $a0, 0x940
/* 0114A8 800360A8 0C04E175 */  jal   func_801385D4
/* 0114AC 800360AC 00000000 */   nop   
/* 0114B0 800360B0 00021400 */  sll   $v0, $v0, 0x10
/* 0114B4 800360B4 1040000D */  beqz  $v0, .L800360EC
/* 0114B8 800360B8 00000000 */   nop   
/* 0114BC 800360BC 3C028007 */  lui   $v0, 0x8007
/* 0114C0 800360C0 8C42419C */  lw    $v0, 0x419c($v0)
/* 0114C4 800360C4 80420071 */  lb    $v0, 0x71($v0)
/* 0114C8 800360C8 14400003 */  bnez  $v0, .L800360D8
/* 0114CC 800360CC 00000000 */   nop   
/* 0114D0 800360D0 0C038069 */  jal   func_800E01A4
/* 0114D4 800360D4 00000000 */   nop   
.L800360D8:
/* 0114D8 800360D8 3C05BF80 */  lui   $a1, 0xbf80
/* 0114DC 800360DC 0C04DF62 */  jal   func_80137D88
/* 0114E0 800360E0 240400FF */   addiu $a0, $zero, 0xff
/* 0114E4 800360E4 0C00CD3C */  jal   set_game_mode
/* 0114E8 800360E8 24040004 */   addiu $a0, $zero, 4
.L800360EC:
/* 0114EC 800360EC 8FBF0014 */  lw    $ra, 0x14($sp)
/* 0114F0 800360F0 8FB00010 */  lw    $s0, 0x10($sp)
/* 0114F4 800360F4 03E00008 */  jr    $ra
/* 0114F8 800360F8 27BD0018 */   addiu $sp, $sp, 0x18
