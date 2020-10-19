.set noat      # allow manual use of $at
.set noreorder # don't insert nops after branches

glabel func_80240000_871A30
/* 871A30 80240000 27BDFFE8 */  addiu     $sp, $sp, -0x18
/* 871A34 80240004 AFB00010 */  sw        $s0, 0x10($sp)
/* 871A38 80240008 0080802D */  daddu     $s0, $a0, $zero
/* 871A3C 8024000C AFBF0014 */  sw        $ra, 0x14($sp)
/* 871A40 80240010 8E0300A8 */  lw        $v1, 0xa8($s0)
/* 871A44 80240014 8E020088 */  lw        $v0, 0x88($s0)
/* 871A48 80240018 00621821 */  addu      $v1, $v1, $v0
/* 871A4C 8024001C 04610004 */  bgez      $v1, .L80240030
/* 871A50 80240020 AE0300A8 */   sw       $v1, 0xa8($s0)
/* 871A54 80240024 3C020002 */  lui       $v0, 2
/* 871A58 80240028 08090010 */  j         .L80240040
/* 871A5C 8024002C 00621021 */   addu     $v0, $v1, $v0
.L80240030:
/* 871A60 80240030 3C040002 */  lui       $a0, 2
/* 871A64 80240034 0083102A */  slt       $v0, $a0, $v1
/* 871A68 80240038 10400002 */  beqz      $v0, .L80240044
/* 871A6C 8024003C 00641023 */   subu     $v0, $v1, $a0
.L80240040:
/* 871A70 80240040 AE0200A8 */  sw        $v0, 0xa8($s0)
.L80240044:
/* 871A74 80240044 8E0300AC */  lw        $v1, 0xac($s0)
/* 871A78 80240048 8E02008C */  lw        $v0, 0x8c($s0)
/* 871A7C 8024004C 00621821 */  addu      $v1, $v1, $v0
/* 871A80 80240050 04610004 */  bgez      $v1, .L80240064
/* 871A84 80240054 AE0300AC */   sw       $v1, 0xac($s0)
/* 871A88 80240058 3C020002 */  lui       $v0, 2
/* 871A8C 8024005C 0809001D */  j         .L80240074
/* 871A90 80240060 00621021 */   addu     $v0, $v1, $v0
.L80240064:
/* 871A94 80240064 3C040002 */  lui       $a0, 2
/* 871A98 80240068 0083102A */  slt       $v0, $a0, $v1
/* 871A9C 8024006C 10400002 */  beqz      $v0, .L80240078
/* 871AA0 80240070 00641023 */   subu     $v0, $v1, $a0
.L80240074:
/* 871AA4 80240074 AE0200AC */  sw        $v0, 0xac($s0)
.L80240078:
/* 871AA8 80240078 8E0300B0 */  lw        $v1, 0xb0($s0)
/* 871AAC 8024007C 8E020090 */  lw        $v0, 0x90($s0)
/* 871AB0 80240080 00621821 */  addu      $v1, $v1, $v0
/* 871AB4 80240084 04610004 */  bgez      $v1, .L80240098
/* 871AB8 80240088 AE0300B0 */   sw       $v1, 0xb0($s0)
/* 871ABC 8024008C 3C020002 */  lui       $v0, 2
/* 871AC0 80240090 0809002A */  j         .L802400A8
/* 871AC4 80240094 00621021 */   addu     $v0, $v1, $v0
.L80240098:
/* 871AC8 80240098 3C040002 */  lui       $a0, 2
/* 871ACC 8024009C 0083102A */  slt       $v0, $a0, $v1
/* 871AD0 802400A0 10400002 */  beqz      $v0, .L802400AC
/* 871AD4 802400A4 00641023 */   subu     $v0, $v1, $a0
.L802400A8:
/* 871AD8 802400A8 AE0200B0 */  sw        $v0, 0xb0($s0)
.L802400AC:
/* 871ADC 802400AC 8E0300B4 */  lw        $v1, 0xb4($s0)
/* 871AE0 802400B0 8E020094 */  lw        $v0, 0x94($s0)
/* 871AE4 802400B4 00621821 */  addu      $v1, $v1, $v0
/* 871AE8 802400B8 04610004 */  bgez      $v1, .L802400CC
/* 871AEC 802400BC AE0300B4 */   sw       $v1, 0xb4($s0)
/* 871AF0 802400C0 3C020002 */  lui       $v0, 2
/* 871AF4 802400C4 08090037 */  j         .L802400DC
/* 871AF8 802400C8 00621021 */   addu     $v0, $v1, $v0
.L802400CC:
/* 871AFC 802400CC 3C040002 */  lui       $a0, 2
/* 871B00 802400D0 0083102A */  slt       $v0, $a0, $v1
/* 871B04 802400D4 10400002 */  beqz      $v0, .L802400E0
/* 871B08 802400D8 00641023 */   subu     $v0, $v1, $a0
.L802400DC:
/* 871B0C 802400DC AE0200B4 */  sw        $v0, 0xb4($s0)
.L802400E0:
/* 871B10 802400E0 8E040084 */  lw        $a0, 0x84($s0)
/* 871B14 802400E4 0C046F07 */  jal       set_main_pan_u
/* 871B18 802400E8 8E0500A8 */   lw       $a1, 0xa8($s0)
/* 871B1C 802400EC 8E040084 */  lw        $a0, 0x84($s0)
/* 871B20 802400F0 0C046F0D */  jal       set_main_pan_v
/* 871B24 802400F4 8E0500AC */   lw       $a1, 0xac($s0)
/* 871B28 802400F8 8E040084 */  lw        $a0, 0x84($s0)
/* 871B2C 802400FC 0C046F13 */  jal       set_aux_pan_u
/* 871B30 80240100 8E0500B0 */   lw       $a1, 0xb0($s0)
/* 871B34 80240104 8E040084 */  lw        $a0, 0x84($s0)
/* 871B38 80240108 0C046F19 */  jal       set_aux_pan_v
/* 871B3C 8024010C 8E0500B4 */   lw       $a1, 0xb4($s0)
/* 871B40 80240110 8FBF0014 */  lw        $ra, 0x14($sp)
/* 871B44 80240114 8FB00010 */  lw        $s0, 0x10($sp)
/* 871B48 80240118 0000102D */  daddu     $v0, $zero, $zero
/* 871B4C 8024011C 03E00008 */  jr        $ra
/* 871B50 80240120 27BD0018 */   addiu    $sp, $sp, 0x18