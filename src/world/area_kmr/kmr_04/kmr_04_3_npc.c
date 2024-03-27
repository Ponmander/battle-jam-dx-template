#include "kmr_04.h"
#include "effects.h"

NpcSettings N(NpcSettings_JrTroopa_1) = {
    .height = 32,
    .radius = 24,
    .level = ACTOR_LEVEL_NONE,
};

EvtScript N(EVS_NpcAI_JrTroopa_01) = {
    Return
    End
};

EvtScript N(EVS_NpcIdle_JrTroopa) = {
    Label(0)
    Call(GetSelfVar, 0, LVar0)
    IfEq(LVar0, 0)
        Wait(1)
        Goto(0)
    EndIf
    IfEq(LVar0, 1)
        Call(StartBossBattle, SONG_JR_TROOPA_BATTLE)
        Label(1)
            Call(GetSelfVar, 0, LVar0)
            IfNe(LVar0, 0)
                Wait(1)
                Goto(1)
            EndIf
            Goto(0)
    EndIf
    Return
    End
};

EvtScript N(EVS_NpcInteract_JrTroopa) = {
    Switch(GB_StoryProgress)
        CaseEq(STORY_CH0_FOUND_HAMMER)
            Call(SpeakToPlayer, NPC_SELF, ANIM_JrTroopa_Talk, ANIM_JrTroopa_Idle, 0, MSG_Misc_JrTroopaInteract)
            Call(ShowChoice, MSG_Choice_0044)
            Call(EndSpeech, NPC_SELF, ANIM_JrTroopa_PointTalk, ANIM_JrTroopa_PointIdle, 0)
            Set(LVarA, LVar0)
            Switch(LVarA)
                CaseEq(0)
                    Call(SpeakToPlayer, NPC_SELF, ANIM_JrTroopa_Talk, ANIM_JrTroopa_Idle, 0, MSG_Misc_JrTroopaTaunt)
                CaseEq(1)
                    Call(SpeakToPlayer, NPC_SELF, ANIM_JrTroopa_Talk, ANIM_JrTroopa_Idle, 0, MSG_Misc_JrTroopaTaunt)
                    Call(SetSelfEnemyFlagBits, ENEMY_FLAG_CANT_INTERACT, TRUE)
                    Call(SetNpcAnimation, NPC_SELF, ANIM_JrTroopa_ChargeArmsUp)
                    Call(SetNpcVar, NPC_SELF, 0, 1)
            EndSwitch
        CaseEq(STORY_CH0_DEFEATED_JR_TROOPA)
            Call(SpeakToPlayer, NPC_SELF, ANIM_JrTroopa_Defeated, ANIM_JrTroopa_Defeated, 0, MSG_CH1_012B)
    EndSwitch
    Return
    End
};

EvtScript N(EVS_NpcDefeat_JrTroopa) = {
    Call(GetBattleOutcome, LVar0)
    Switch(LVar0)
        CaseEq(OUTCOME_PLAYER_WON)
            // Call(SetSelfEnemyFlagBits, ENEMY_FLAG_CANT_INTERACT, FALSE)
            Call(ClearDefeatedEnemies)
            Call(SetNpcAnimation, NPC_SELF, ANIM_JrTroopa_Defeated)
            Call(GetNpcPos, NPC_SELF, LVar0, LVar1, LVar2)
            Call(SetNpcPos, NPC_JrTroopa_2, LVar0, LVar1, LVar2)
            Call(SetNpcCollisionSize, NPC_SELF, 24, 32)
            Call(SetNpcCollisionSize, NPC_JrTroopa_2, 24, 32)
            Set(GB_StoryProgress, STORY_CH0_DEFEATED_JR_TROOPA)
            Call(BindNpcAI, NPC_SELF, Ref(N(EVS_NpcAI_JrTroopa_01)))
            Call(SetNpcFlagBits, NPC_JrTroopa_2, NPC_FLAG_INVISIBLE, TRUE)
            Call(ResetCam, CAM_DEFAULT, Float(90.0))
            Call(DisablePlayerInput, FALSE)
    EndSwitch
    Return
    End
};

EvtScript N(EVS_NpcInit_JrTroopa_1) = {
    Call(EnableNpcShadow, NPC_SELF, FALSE)
    Call(BindNpcIdle, NPC_SELF, Ref(N(EVS_NpcIdle_JrTroopa)))
    Call(BindNpcInteract, NPC_SELF, Ref(N(EVS_NpcInteract_JrTroopa)))
    Call(BindNpcDefeat, NPC_SELF, Ref(N(EVS_NpcDefeat_JrTroopa)))
    Return
    End
};

EvtScript N(EVS_NpcHit_JrTroopa_2) = {
    Call(GetOwnerEncounterTrigger, LVar0)
    Switch(LVar0)
        CaseOrEq(ENCOUNTER_TRIGGER_JUMP)
        CaseOrEq(ENCOUNTER_TRIGGER_HAMMER)
        CaseOrEq(ENCOUNTER_TRIGGER_PARTNER)
            Call(DisablePlayerInput, TRUE)
            Call(SpeakToPlayer, NPC_SELF, ANIM_JrTroopa_Defeated, ANIM_JrTroopa_Defeated, 5, MSG_MAC_Port_00BD)
            Call(DisablePlayerInput, FALSE)
        EndCaseGroup
    EndSwitch
    Return
    End
};

EvtScript N(EVS_NpcInit_JrTroopa_2) = {
    Call(EnableNpcShadow, NPC_SELF, FALSE)
    Call(SetNpcFlagBits, NPC_SELF, NPC_FLAG_INVISIBLE, TRUE)
    Call(SetNpcPos, NPC_SELF, NPC_DISPOSE_LOCATION)
    Call(BindNpcHit, NPC_SELF, Ref(N(EVS_NpcHit_JrTroopa_2)))
    Return
    End
};

NpcData N(NpcData_JrTroopa)[] = {
    {
        .id = NPC_JrTroopa_1,
        .pos = { -210, 0, 5 },
        .yaw = 0,
        .init = &N(EVS_NpcInit_JrTroopa_1),
        .settings = &N(NpcSettings_JrTroopa_1),
        .flags = ENEMY_FLAG_PASSIVE | ENEMY_FLAG_4 | ENEMY_FLAG_IGNORE_WORLD_COLLISION | ENEMY_FLAG_IGNORE_ENTITY_COLLISION | ENEMY_FLAG_FLYING | ENEMY_FLAG_100000 | ENEMY_FLAG_ACTIVE_WHILE_OFFSCREEN | ENEMY_FLAG_400000 | ENEMY_FLAG_NO_DROPS,
        .drops = NO_DROPS,
        .animations = {
            .idle   = ANIM_JrTroopa_Idle,
            .walk   = ANIM_JrTroopa_Walk,
            .run    = ANIM_JrTroopa_Walk,
            .chase  = ANIM_JrTroopa_Walk,
            .anim_4 = ANIM_JrTroopa_Idle,
            .anim_5 = ANIM_JrTroopa_Idle,
            .death  = ANIM_JrTroopa_Idle,
            .hit    = ANIM_JrTroopa_Idle,
            .anim_8 = ANIM_JrTroopa_Idle,
            .anim_9 = ANIM_JrTroopa_Idle,
            .anim_A = ANIM_JrTroopa_Idle,
            .anim_B = ANIM_JrTroopa_Idle,
            .anim_C = ANIM_JrTroopa_Idle,
            .anim_D = ANIM_JrTroopa_Idle,
            .anim_E = ANIM_JrTroopa_Idle,
            .anim_F = ANIM_JrTroopa_Idle,
        },
    },
    {
        .id = NPC_JrTroopa_2,
        .pos = { NPC_DISPOSE_LOCATION },
        .yaw = 0,
        .init = &N(EVS_NpcInit_JrTroopa_2),
        .settings = &N(NpcSettings_JrTroopa_1),
        .flags = ENEMY_FLAG_4 | ENEMY_FLAG_ENABLE_HIT_SCRIPT | ENEMY_FLAG_IGNORE_WORLD_COLLISION | ENEMY_FLAG_IGNORE_PLAYER_COLLISION | ENEMY_FLAG_IGNORE_ENTITY_COLLISION | ENEMY_FLAG_FLYING | ENEMY_FLAG_40000 | ENEMY_FLAG_100000 | ENEMY_FLAG_400000 | ENEMY_FLAG_IGNORE_TOUCH | ENEMY_FLAG_IGNORE_SPIN,
        .drops = NO_DROPS,
        .animations = {
            .idle   = ANIM_JrTroopa_Idle,
            .walk   = ANIM_JrTroopa_Walk,
            .run    = ANIM_JrTroopa_Walk,
            .chase  = ANIM_JrTroopa_Walk,
            .anim_4 = ANIM_JrTroopa_Idle,
            .anim_5 = ANIM_JrTroopa_Idle,
            .death  = ANIM_JrTroopa_Idle,
            .hit    = ANIM_JrTroopa_Idle,
            .anim_8 = ANIM_JrTroopa_Idle,
            .anim_9 = ANIM_JrTroopa_Idle,
            .anim_A = ANIM_JrTroopa_Idle,
            .anim_B = ANIM_JrTroopa_Idle,
            .anim_C = ANIM_JrTroopa_Idle,
            .anim_D = ANIM_JrTroopa_Idle,
            .anim_E = ANIM_JrTroopa_Idle,
            .anim_F = ANIM_JrTroopa_Idle,
        },
    },
};

NpcGroupList N(DefaultNPCs) = {
    NPC_GROUP(N(NpcData_JrTroopa), BTL_JAM_FORMATION_00),
    {}
};
