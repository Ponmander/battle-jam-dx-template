#include "MontyMole.h"

EvtScript N(EVS_NpcAuxAI_MontyMole_WallAmbush) = {
    EVT_CALL(EnableNpcShadow, NPC_SELF, FALSE)
    EVT_CALL(SetNpcFlagBits, NPC_SELF, NPC_FLAG_4, TRUE)
    EVT_CALL(GetNpcPos, NPC_SELF, LVar0, LVar1, LVar2)
    EVT_SUB(LVar2, 50)
    EVT_CALL(SetNpcPos, NPC_SELF, LVar0, LVar1, LVar2)
    EVT_RETURN
    EVT_END
};

EvtScript N(EVS_NpcAuxAI_MontyMole_WallAmbush_Hole) = {
    EVT_CALL(SetNpcFlagBits, NPC_SELF, NPC_FLAG_40000, TRUE)
    EVT_CALL(EnableNpcShadow, NPC_SELF, FALSE)
    EVT_CALL(GetNpcPos, NPC_SELF, LVar0, LVar1, LVar2)
    EVT_SUB(LVar2, 50)
    EVT_CALL(SetNpcPos, NPC_SELF, LVar0, LVar1, LVar2)
    EVT_CALL(SetNpcAnimation, NPC_SELF, ANIM_MontyMole_Anim13)
    EVT_RETURN
    EVT_END
};

MobileAISettings N(AISettings_MontyMole_WallAmbush) = {
    .moveSpeed = 1.8f,
    .moveTime = 30,
    .waitTime = 30,
    .alertRadius = 90.0f,
    .alertOffsetDist = 70.0f,
    .playerSearchInterval = 2,
    .chaseSpeed = 3.0f,
    .chaseTurnRate = 90,
    .chaseUpdateInterval = 25,
    .chaseRadius = 120.0f,
    .chaseOffsetDist = 70.0f,
    .unk_AI_2C = 1,
};

EvtScript N(EVS_NpcAI_MontyMole_WallAmbush) = {
    EVT_CALL(func_800445D4, LVar0)
    EVT_IF_EQ(LVar0, 100)
        EVT_CALL(SetSelfEnemyFlagBits, ENEMY_FLAG_DISABLE_AI, 1)
        EVT_LABEL(10)
        EVT_CALL(GetNpcPos, NPC_SELF, LVar0, LVar1, LVar2)
        EVT_ADD(LVar2, 130)
        EVT_CALL(IsPlayerWithin, LVar0, LVar2, 80, LVar3)
        EVT_IF_NE(LVar3, 1)
            EVT_WAIT(1)
            EVT_GOTO(10)
        EVT_END_IF
        EVT_CALL(GetSelfNpcID, LVar0)
        EVT_ADD(LVar0, 1)
        EVT_CALL(GetNpcPos, LVar0, LVar1, LVar2, LVar3)
        EVT_ADD(LVar2, 30)
        EVT_ADD(LVar3, 50)
        EVT_CALL(SetNpcPos, LVar0, LVar1, LVar2, LVar3)
        EVT_WAIT(1)
        EVT_CALL(SetNpcFlagBits, NPC_SELF, NPC_FLAG_4, FALSE)
        EVT_CALL(SetNpcFlagBits, NPC_SELF, NPC_FLAG_IGNORE_WORLD_COLLISION, TRUE)
        EVT_CALL(EnableNpcShadow, NPC_SELF, TRUE)
        EVT_CALL(SetNpcAnimation, NPC_SELF, ANIM_MontyMole_Anim0E)
        EVT_CALL(SetNpcPos, NPC_SELF, LVar1, LVar2, LVar3)
        EVT_SUB(LVar2, 30)
        EVT_ADD(LVar3, 80)
        EVT_CALL(SetNpcJumpscale, NPC_SELF, EVT_FLOAT(0.7))
        EVT_CALL(PlaySoundAtNpc, NPC_SELF, SOUND_MOLE_POP, SOUND_SPACE_MODE_0)
        EVT_CALL(NpcJump0, NPC_SELF, LVar1, LVar2, LVar3, 20)
        EVT_CALL(SetNpcFlagBits, NPC_SELF, NPC_FLAG_IGNORE_WORLD_COLLISION, FALSE)
        EVT_CALL(SetSelfEnemyFlagBits, ENEMY_FLAG_DISABLE_AI, 0)
        EVT_CALL(NpcFacePlayer, NPC_SELF, 0)
        EVT_CALL(SetSelfEnemyFlagBits, ENEMY_FLAG_40000000, 1)
    EVT_END_IF
    EVT_CALL(BasicAI_Main, EVT_PTR(N(AISettings_MontyMole_WallAmbush)))
    EVT_RETURN
    EVT_END
};

EvtScript N(EVS_NpcAI_MontyMole_WallAmbush_Hole_Unused) = {
    EVT_RETURN
    EVT_END
};

NpcSettings N(NpcSettings_MontyMole_WallAmbush) = {
    .height = 24,
    .radius = 22,
    .level = 8,
    .otherAI = &N(EVS_NpcAuxAI_MontyMole_WallAmbush),
    .ai = &N(EVS_NpcAI_MontyMole_WallAmbush),
    .onHit = &EnemyNpcHit,
    .onDefeat = &EnemyNpcDefeat,
    .actionFlags = AI_ACTION_JUMP_WHEN_SEE_PLAYER,
};

NpcSettings N(NpcSettings_MontyMole_WallAmbush_Hole) = {
    .height = 24,
    .radius = 22,
    .level = 8,
    .otherAI = &N(EVS_NpcAuxAI_MontyMole_WallAmbush_Hole),
    .actionFlags = AI_ACTION_JUMP_WHEN_SEE_PLAYER,
};