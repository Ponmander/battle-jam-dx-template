#include "battle/battle.h"
#include "script_api/battle.h"
#include "sprite/npc/BattleBowser.h"
#include "dx/debug_menu.h"

#define NAMESPACE A(false_bowser)

extern EvtScript N(EVS_Init);
extern EvtScript N(EVS_Idle);
extern EvtScript N(EVS_TakeTurn);
extern EvtScript N(EVS_TakeTurn_Inner);
extern EvtScript N(EVS_HandleEvent);
extern EvtScript N(EVS_HandlePhase);
extern EvtScript N(EVS_Attack_FireBall);
extern EvtScript N(EVS_Attack_Jump);
extern EvtScript N(EVS_UseAttack);
extern EvtScript N(EVS_Hit);
extern EvtScript N(EVS_Death);
extern EvtScript N(EVS_NoDamageHit);
extern EvtScript N(EVS_BasicHit);
extern EvtScript N(EVS_BurnHit);
extern EvtScript N(EVS_ShockReaction);
extern EvtScript N(EVS_ReturnHome);
extern EvtScript N(EVS_PlayFootstepSounds);

enum N(ActorPartIDs) {
    PRT_MAIN        = 1,
};

enum N(ActorVars) {
    AVAR_TurnCount              = 0,
};

enum N(ActorParams) {
    DMG_JUMP                    = 1,
    DMG_FIRE_BALL               = 1,
};

s32 N(DefaultAnims)[] = {
    STATUS_KEY_NORMAL,    ANIM_BattleBowser_BowsersBrother_Idle,
    STATUS_KEY_STONE,     ANIM_BattleBowser_BowsersBrother_Still,
    STATUS_KEY_SLEEP,     ANIM_BattleBowser_BowsersBrother_Idle,
    STATUS_KEY_POISON,    ANIM_BattleBowser_BowsersBrother_Idle,
    STATUS_KEY_STOP,      ANIM_BattleBowser_BowsersBrother_Still,
    STATUS_KEY_STATIC,    ANIM_BattleBowser_BowsersBrother_Still,
    STATUS_KEY_PARALYZE,  ANIM_BattleBowser_BowsersBrother_Still,
    STATUS_KEY_PARALYZE,  ANIM_BattleBowser_BowsersBrother_Still,
    STATUS_KEY_DIZZY,     ANIM_BattleBowser_BowsersBrother_Idle,
    STATUS_KEY_DIZZY,     ANIM_BattleBowser_BowsersBrother_Idle,
    STATUS_END,
};

s32 N(DefenseTable)[] = {
    ELEMENT_NORMAL,   0,
    ELEMENT_END,
};

s32 N(StatusTable)[] = {
    STATUS_KEY_NORMAL,              0,
    STATUS_KEY_DEFAULT,             0,
    STATUS_KEY_SLEEP,              30,
    STATUS_KEY_POISON,              0,
    STATUS_KEY_FROZEN,              0,
    STATUS_KEY_DIZZY,              30,
    STATUS_KEY_FEAR,                0,
    STATUS_KEY_STATIC,              0,
    STATUS_KEY_PARALYZE,           30,
    STATUS_KEY_SHRINK,             50,
    STATUS_KEY_STOP,               30,
    STATUS_TURN_MOD_DEFAULT,        0,
    STATUS_TURN_MOD_SLEEP,         -1,
    STATUS_TURN_MOD_POISON,         0,
    STATUS_TURN_MOD_FROZEN,         0,
    STATUS_TURN_MOD_DIZZY,         -1,
    STATUS_TURN_MOD_FEAR,           0,
    STATUS_TURN_MOD_STATIC,         0,
    STATUS_TURN_MOD_PARALYZE,      -1,
    STATUS_TURN_MOD_SHRINK,        -1,
    STATUS_TURN_MOD_STOP,          -2,
    STATUS_END,
};

s32 N(BoostedStatusTable)[] = {
    STATUS_KEY_NORMAL,              0,
    STATUS_KEY_DEFAULT,             0,
    STATUS_KEY_SLEEP,               0,
    STATUS_KEY_POISON,              0,
    STATUS_KEY_FROZEN,              0,
    STATUS_KEY_DIZZY,               0,
    STATUS_KEY_FEAR,                0,
    STATUS_KEY_STATIC,              0,
    STATUS_KEY_PARALYZE,            0,
    STATUS_KEY_SHRINK,              0,
    STATUS_KEY_STOP,                0,
    STATUS_TURN_MOD_DEFAULT,        0,
    STATUS_TURN_MOD_SLEEP,         -1,
    STATUS_TURN_MOD_POISON,         0,
    STATUS_TURN_MOD_FROZEN,         0,
    STATUS_TURN_MOD_DIZZY,         -1,
    STATUS_TURN_MOD_FEAR,           0,
    STATUS_TURN_MOD_STATIC,         0,
    STATUS_TURN_MOD_PARALYZE,      -1,
    STATUS_TURN_MOD_SHRINK,        -1,
    STATUS_TURN_MOD_STOP,          -2,
    STATUS_END,
};

ActorPartBlueprint N(ActorParts)[] = {
    {
        .flags = ACTOR_PART_FLAG_PRIMARY_TARGET,
        .index = PRT_MAIN,
        .posOffset = { 0, 0, 0 },
        .targetOffset = { -18, 72 },
        .opacity = 255,
        .idleAnimations = N(DefaultAnims),
        .defenseTable = N(DefenseTable),
        .eventFlags = 0,
        .elementImmunityFlags = 0,
        .projectileTargetOffset = { 0, -8 },
    },
};

ActorBlueprint NAMESPACE = {
    .flags = 0,
    .type = ACTOR_TYPE_FALSE_BOWSER,
    .level = ACTOR_LEVEL_FALSE_BOWSER,
    .maxHP = 10,
    .partCount = ARRAY_COUNT(N(ActorParts)),
    .partsData = N(ActorParts),
    .initScript = &N(EVS_Init),
    .statusTable = N(StatusTable),
    .escapeChance = 0,
    .airLiftChance = 0,
    .hurricaneChance = 0,
    .spookChance = 0,
    .upAndAwayChance = 0,
    .spinSmashReq = 4,
    .powerBounceChance = 65,
    .coinReward = 0,
    .size = { 80, 82 },
    .healthBarOffset = { 0, 0 },
    .statusIconOffset = { -40, 67 },
    .statusTextOffset = { 10, 60 },
};

#include "common/StartRumbleWithParams.inc.c"

EvtScript N(EVS_Init) = {
    Call(SetActorVar, ACTOR_SELF, AVAR_TurnCount, 0)
    Call(BindTakeTurn, ACTOR_SELF, Ref(N(EVS_TakeTurn)))
    Call(BindIdle, ACTOR_SELF, Ref(N(EVS_Idle)))
    Call(BindHandleEvent, ACTOR_SELF, Ref(N(EVS_HandleEvent)))
    Call(BindHandlePhase, ACTOR_SELF, Ref(N(EVS_HandlePhase)))
    Return
    End
};

EvtScript N(EVS_Idle) = {
    Call(GetActorPos, ACTOR_SELF, LVar0, LVar1, LVar2)
    Call(GetAnimation, ACTOR_SELF, PRT_MAIN, LVar3)
    Label(0)
        Call(ActorExists, ACTOR_SELF, LVarB)
        IfEq(LVarB, 0)
            Return
        EndIf
        Call(GetStatusFlags, ACTOR_SELF, LVarB)
        IfNotFlag(LVarB, STATUS_FLAG_SHRINK)
            Call(ModifyActorDecoration, ACTOR_SELF, PRT_MAIN, 1, 100, 0, 0, 0)
        Else
            Call(ModifyActorDecoration, ACTOR_SELF, PRT_MAIN, 1, 40, 0, 0, 0)
        EndIf
        ExecWait(N(EVS_PlayFootstepSounds))
        Wait(1)
        Goto(0)
    Return
    End
};

EvtScript N(EVS_PlayFootstepSounds) = {
    Call(GetAnimation, ACTOR_SELF, PRT_MAIN, LVar7)
    IfNe(LVar7, ANIM_BattleBowser_BowsersBrother_Walk)
        IfEq(LVar3, ANIM_BattleBowser_BowsersBrother_Walk)
            Call(PlaySoundAtActor, ACTOR_SELF, SOUND_GOOMBA_STEP)
            Call(PlaySoundAtActor, ACTOR_SELF, SOUND_GOOMBA_STEP)
        EndIf
        Set(LVar3, LVar7)
        Call(GetActorPos, ACTOR_SELF, LVar0, LVar1, LVar2)
        Return
    EndIf
    Call(GetActorPos, ACTOR_SELF, LVar5, LVar6, LVar7)
    Sub(LVar5, LVar0)
    IfGt(LVar5, -30)
        IfLt(LVar5, 30)
            Set(LVar3, LVar7)
            Return
        EndIf
    EndIf
    Call(GetActorPos, ACTOR_SELF, LVar0, LVar1, LVar2)
    Call(PlaySoundAtActor, ACTOR_SELF, SOUND_GOOMBA_STEP)
    Call(PlaySoundAtActor, ACTOR_SELF, SOUND_GOOMBA_STEP)
    Set(LVar3, LVar7)
    Return
    End
};

EvtScript N(EVS_HandlePhase) = {
    Call(UseIdleAnimation, ACTOR_SELF, FALSE)
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_ENABLE)
    Call(UseIdleAnimation, ACTOR_SELF, TRUE)
    Return
    End
};

EvtScript N(EVS_HandleEvent) = {
    Call(UseIdleAnimation, ACTOR_SELF, FALSE)
    Call(GetLastEvent, ACTOR_SELF, LVar0)
    Switch(LVar0)
        CaseEq(EVENT_BEGIN_FIRST_STRIKE)
        CaseEq(EVENT_HIT_COMBO)
            Set(LVar1, ANIM_BattleBowser_BowsersBrother_Hurt)
            ExecWait(N(EVS_Hit))
            Return
        CaseEq(EVENT_HIT)
            Set(LVar1, ANIM_BattleBowser_BowsersBrother_Hurt)
            ExecWait(N(EVS_BasicHit))
            Return
        CaseEq(EVENT_DEATH)
            SetConst(LVar1, ANIM_BattleBowser_BowsersBrother_Hurt)
            ExecWait(N(EVS_Hit))
            Wait(10)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_BattleBowser_BowsersBrother_Hurt)
            ExecWait(N(EVS_Death))
            Return
        CaseEq(EVENT_BURN_HIT)
            Set(LVar0, PRT_MAIN)
            Set(LVar1, ANIM_BattleBowser_BowsersBrother_BurnHurt)
            Set(LVar2, ANIM_BattleBowser_BowsersBrother_BurnStill)
            ExecWait(N(EVS_BurnHit))
            Return
        CaseEq(EVENT_BURN_DEATH)
            Set(LVar0, PRT_MAIN)
            Set(LVar1, ANIM_BattleBowser_BowsersBrother_BurnHurt)
            Set(LVar2, ANIM_BattleBowser_BowsersBrother_BurnStill)
            ExecWait(N(EVS_BurnHit))
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_BattleBowser_BowsersBrother_BurnStill)
            ExecWait(N(EVS_Death))
            Return
        CaseEq(EVENT_SPIN_SMASH_HIT)
            Set(LVar1, ANIM_BattleBowser_BowsersBrother_Hurt)
            ExecWait(N(EVS_BasicHit))
            Return
        CaseEq(EVENT_SPIN_SMASH_DEATH)
            SetConst(LVar1, ANIM_BattleBowser_BowsersBrother_Hurt)
            ExecWait(N(EVS_Hit))
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_BattleBowser_BowsersBrother_Hurt)
            ExecWait(N(EVS_Death))
            Return
        CaseEq(EVENT_SHOCK_HIT)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_BattleBowser_BowsersBrother_Hurt)
            ExecWait(EVS_Enemy_ShockHit)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_BattleBowser_BowsersBrother_Hurt)
            ExecWait(N(EVS_ShockReaction))
            Call(SetActorSpeed, ACTOR_SELF, Float(6.0))
            Set(LVar1, ANIM_BattleBowser_BowsersBrother_Walk)
            ExecWait(N(EVS_ReturnHome))
        CaseEq(EVENT_SHOCK_DEATH)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_BattleBowser_BowsersBrother_Hurt)
            ExecWait(EVS_Enemy_ShockHit)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_BattleBowser_BowsersBrother_Hurt)
            ExecWait(N(EVS_ShockReaction))
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_BattleBowser_BowsersBrother_Hurt)
            ExecWait(N(EVS_Death))
            Return
        CaseOrEq(EVENT_ZERO_DAMAGE)
        CaseOrEq(EVENT_IMMUNE)
            Set(LVar1, ANIM_BattleBowser_BowsersBrother_Idle)
            ExecWait(N(EVS_NoDamageHit))
        EndCaseGroup
        CaseEq(EVENT_AIR_LIFT_FAILED)
            SetConst(LVar0, PRT_MAIN)
            Set(LVar1, ANIM_BattleBowser_BowsersBrother_Idle)
            ExecWait(EVS_Enemy_NoDamageHit)
        CaseEq(EVENT_RECOVER_STATUS)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_BattleBowser_BowsersBrother_Jump)
            ExecWait(EVS_Enemy_Recover)
            Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_BattleBowser_BowsersBrother_PostJump)
            Call(ShakeCam, CAM_BATTLE, 0, 4, Float(3.0))
        CaseDefault
    EndSwitch
    Call(UseIdleAnimation, ACTOR_SELF, TRUE)
    Return
    End
};

EvtScript N(EVS_BasicHit) = {
    ExecWait(N(EVS_Hit))
    Call(UseIdleAnimation, ACTOR_SELF, TRUE)
    Return
    End
};

EvtScript N(EVS_Hit) = {
    SetConst(LVar0, PRT_MAIN)
    ExecWait(EVS_Enemy_Hit)
    Return
    End
};

EvtScript N(EVS_Death) = {
    Call(HideHealthBar, ACTOR_SELF)
    Call(UseIdleAnimation, ACTOR_SELF, FALSE)
    Set(LVar2, EXEC_DEATH_NO_SPINNING)
    Call(GetActorPos, ACTOR_SELF, LVar0, LVar1, LVar2)
    Set(LVar3, 1000)
    Set(LVar4, 1000)
    PlayEffect(EFFECT_SHAPE_SPELL, 2, LVar0, LVar1, LVar2, LVar3, LVar4, LVar5, 30, 0)
    Call(GetActorPos, ACTOR_SELF, LVar0, LVar1, LVar2)
    Add(LVar1, 10)
    Add(LVar2, 10)
    PlayEffect(EFFECT_BIG_SMOKE_PUFF, LVar0, LVar1, LVar2, 0, 0, 0, 0, 0)
    Call(PlaySoundAtActor, ACTOR_SELF, SOUND_ACTOR_DEATH)
    Call(DropStarPoints, ACTOR_SELF)
    Call(SetPartFlagBits, ACTOR_SELF, PRT_MAIN, ACTOR_PART_FLAG_INVISIBLE, TRUE)
    Call(SetActorFlagBits, ACTOR_SELF, ACTOR_FLAG_NO_SHADOW, TRUE)
    Wait(30)
    Call(UseBattleCamPreset, BTL_CAM_DEFAULT)
    // ExecWait(EVS_ForceNextTarget)
    Call(RemoveActor, ACTOR_SELF)
    Return
    End
};

EvtScript N(EVS_BurnHit) = {
    Call(GetLastEvent, ACTOR_SELF, LVar3)
    IfEq(LVar3, 36)
        Call(RemoveActorDecoration, ACTOR_SELF, PRT_MAIN, 1)
        Call(SetPartEventBits, ACTOR_SELF, PRT_MAIN, ACTOR_EVENT_FLAG_ENCHANTED, FALSE)
    EndIf
    Call(SetAnimation, ACTOR_SELF, LVar0, LVar1)
    Call(GetDamageSource, LVar3)
    Switch(LVar3)
        CaseEq(DMG_SRC_FIRE_SHELL)
            Call(GetOriginalActorType, ACTOR_SELF, LVar7)
            Switch(LVar7)
                CaseOrEq(ACTOR_TYPE_MONTY_MOLE)
                CaseOrEq(ACTOR_TYPE_MONTY_MOLE_BOSS)
                EndCaseGroup
                CaseDefault
                    Call(GetActorPos, ACTOR_SELF, LVar4, LVar5, LVar6)
                    Call(SetActorJumpGravity, ACTOR_SELF, Float(0.01))
                    Add(LVar5, 55)
                    Call(SetGoalPos, ACTOR_SELF, LVar4, LVar5, LVar6)
                    Call(JumpToGoal, ACTOR_SELF, 8, FALSE, FALSE, FALSE)
            EndSwitch
            Set(LVar7, 0)
            Loop(30)
                Add(LVar7, 48)
                Call(SetActorYaw, ACTOR_SELF, LVar7)
                Wait(1)
            EndLoop
            Call(GetOriginalActorType, ACTOR_SELF, LVar7)
            Switch(LVar7)
                CaseOrEq(ACTOR_TYPE_MONTY_MOLE)
                CaseOrEq(ACTOR_TYPE_MONTY_MOLE_BOSS)
                    Wait(30)
                EndCaseGroup
                CaseDefault
                    Sub(LVar5, 55)
                    Call(SetActorJumpGravity, ACTOR_SELF, Float(0.8))
                    IfEq(LVar5, 0)
                        Call(SetGoalPos, ACTOR_SELF, LVar4, LVar5, LVar6)
                        Call(JumpToGoal, ACTOR_SELF, 15, FALSE, TRUE, FALSE)
                        Call(SetGoalPos, ACTOR_SELF, LVar4, LVar5, LVar6)
                        Call(JumpToGoal, ACTOR_SELF, 10, FALSE, TRUE, FALSE)
                        Call(SetGoalPos, ACTOR_SELF, LVar4, LVar5, LVar6)
                        Call(JumpToGoal, ACTOR_SELF, 5, FALSE, TRUE, FALSE)
                    Else
                        Call(SetGoalPos, ACTOR_SELF, LVar4, LVar5, LVar6)
                        Call(JumpToGoal, ACTOR_SELF, 15, FALSE, FALSE, FALSE)
                        Call(SetGoalPos, ACTOR_SELF, LVar4, LVar5, LVar6)
                        Call(JumpToGoal, ACTOR_SELF, 10, FALSE, FALSE, FALSE)
                        Call(SetGoalPos, ACTOR_SELF, LVar4, LVar5, LVar6)
                        Call(JumpToGoal, ACTOR_SELF, 5, FALSE, FALSE, FALSE)
                    EndIf
            EndSwitch
        CaseDefault
            Wait(20)
    EndSwitch
    IfNe(LVar2, -1)
        Call(SetAnimation, ACTOR_SELF, LVar0, LVar2)
    EndIf
    Wait(10)
    Call(GetLastEvent, ACTOR_SELF, LVar1)
    Switch(LVar1)
        CaseEq(EVENT_BURN_HIT)
            Call(GetPartEventFlags, ACTOR_SELF, LVar0, LVar1)
            IfNotFlag(LVar1, ACTOR_EVENT_FLAG_FIREY | ACTOR_EVENT_FLAG_EXPLODE_ON_IGNITION)
                Call(GetActorPos, ACTOR_SELF, LVar0, LVar1, LVar2)
                Call(GetActorSize, ACTOR_SELF, LVar3, LVar4)
                Call(GetStatusFlags, ACTOR_SELF, LVar5)
                IfFlag(LVar5, STATUS_FLAG_SHRINK)
                    MulF(LVar3, Float(0.4))
                    MulF(LVar4, Float(0.4))
                EndIf
                DivF(LVar3, Float(2.0))
                Call(GetActorFlags, ACTOR_SELF, LVar5)
                IfFlag(LVar5, ACTOR_FLAG_UPSIDE_DOWN)
                    SubF(LVar1, LVar3)
                Else
                    AddF(LVar1, LVar3)
                EndIf
                AddF(LVar2, Float(5.0))
                DivF(LVar3, Float(10.0))
                Call(GetStatusFlags, ACTOR_SELF, LVar5)
                IfFlag(LVar5, STATUS_FLAG_SHRINK)
                    MulF(LVar3, Float(0.4))
                    IfLt(LVar3, 1)
                        SetF(LVar3, Float(1.0))
                    EndIf
                EndIf
                PlayEffect(EFFECT_SMOKE_BURST, 0, LVar0, LVar1, LVar2, LVar3, 10, 0)
            EndIf
        CaseEq(EVENT_BURN_DEATH)
            // do nothing
    EndSwitch
    Return
    End
};

EvtScript N(EVS_ShockReaction) = {
    IfNe(LVar1, -1)
        Call(SetAnimation, ACTOR_SELF, LVar0, LVar1)
    EndIf
    Call(UseBattleCamPreset, BTL_CAM_DEFAULT)
    Call(SetActorJumpGravity, ACTOR_SELF, Float(1.4))
    Call(SetGoalToHome, ACTOR_SELF)
    Call(GetGoalPos, ACTOR_SELF, LVar2, LVar3, LVar4)
    Call(GetActorPos, ACTOR_SELF, LVar5, LVar6, LVar7)
    Set(LVar8, LVar2)
    Add(LVar8, LVar5)
    Div(LVar8, 2)
    Set(LVar9, LVar4)
    Add(LVar9, LVar7)
    Div(LVar9, 2)
    Call(SetGoalPos, ACTOR_SELF, LVar8, 0, LVar9)
    Call(JumpToGoal, ACTOR_SELF, 15, FALSE, TRUE, FALSE)
    Add(LVar8, LVar2)
    Div(LVar8, 2)
    Add(LVar9, LVar4)
    Div(LVar9, 2)
    Call(SetGoalPos, ACTOR_SELF, LVar8, 0, LVar9)
    Call(JumpToGoal, ACTOR_SELF, 8, FALSE, TRUE, FALSE)
    Call(SetGoalPos, ACTOR_SELF, LVar2, LVar3, LVar4)
    Call(JumpToGoal, ACTOR_SELF, 5, FALSE, TRUE, FALSE)
    Return
    End
};

EvtScript N(EVS_NoDamageHit) = {
    SetConst(LVar0, PRT_MAIN)
    ExecWait(EVS_Enemy_NoDamageHit)
    Return
    End
};

EvtScript N(EVS_ReturnHome) = {
    Set(LVar0, PRT_MAIN)
    Call(GetActorPos, ACTOR_SELF, LVarA, LVarB, LVarC)
    Call(SetGoalToHome, ACTOR_SELF)
    Call(GetGoalPos, ACTOR_SELF, LVarD, LVarE, LVarF)
    Call(SetActorYaw, ACTOR_SELF, 0)
    Call(GetDist2D, LVar9, LVarA, LVarC, LVarD, LVarF)
    IfGe(LVar9, Float(5.0))
        Call(SetAnimation, ACTOR_SELF, LVar0, LVar1)
        Call(RunToGoal, ACTOR_SELF, 0, FALSE)
    EndIf
    IfEq(LVarB, 180)
        Loop(15)
            Sub(LVarB, 12)
            Call(SetActorYaw, ACTOR_SELF, LVarB)
            Wait(1)
        EndLoop
    EndIf
    Call(SetActorYaw, ACTOR_SELF, 0)
    Return
    End
};

EvtScript N(EVS_TakeTurn) = {
    Call(UseIdleAnimation, ACTOR_SELF, FALSE)
    Call(UseIdleAnimation, ACTOR_SELF, FALSE)
    ExecWait(N(EVS_TakeTurn_Inner))
    Call(UseIdleAnimation, ACTOR_SELF, TRUE)
    Return
    End
};

EvtScript N(EVS_TakeTurn_Inner) = {
    Call(AddActorVar, ACTOR_SELF, AVAR_TurnCount, 1)
    ExecWait(N(EVS_UseAttack))
    Return
    End
};

#define LBL_JUMP_ATTACK 0
#define LBL_FIRE_BALL_ATTACK 1
EvtScript N(EVS_UseAttack) = {
    Call(RandInt, 100, LVar1)
    Switch(LVar1)
        CaseLt(50)
            Set(LVar0, LBL_JUMP_ATTACK)
        CaseDefault
            Set(LVar0, LBL_FIRE_BALL_ATTACK)
    EndSwitch
    // Set(LVar0, LBL_FIRE_BALL_ATTACK)
    IfEq(LVar0, LBL_JUMP_ATTACK)
        DebugPrintf("Attack: %s\n", "Jump")
    Else
        DebugPrintf("Attack: %s\n", "Fire Ball")
    EndIf
    Switch(LVar0)
        CaseEq(LBL_JUMP_ATTACK)
            ExecWait(N(EVS_Attack_Jump))
        CaseEq(LBL_FIRE_BALL_ATTACK)
            ExecWait(N(EVS_Attack_FireBall))
    EndSwitch
    Return
    End
};

EvtScript N(EVS_Attack_Jump) = {
    Call(SetTargetActor, ACTOR_SELF, ACTOR_PLAYER)
    Call(GetStatusFlags, ACTOR_SELF, LVar0)
    IfNotFlag(LVar0, STATUS_FLAG_SHRINK)
        Call(UseBattleCamPreset, BTL_CAM_ENEMY_APPROACH)
        Call(SetBattleCamZoom, 300)
        Call(SetBattleCamOffsetZ, 36)
        Call(BattleCamTargetActor, ACTOR_SELF)
        Call(MoveBattleCamOver, 40)
        Call(func_8024ECF8, BTL_CAM_MODEY_MINUS_1, BTL_CAM_MODEX_1, FALSE)
    Else
        Call(UseBattleCamPreset, BTL_CAM_ENEMY_APPROACH)
        Call(SetBattleCamZoom, 240)
        Call(SetBattleCamOffsetZ, 14)
        Call(BattleCamTargetActor, ACTOR_SELF)
        Call(MoveBattleCamOver, 40)
        Call(func_8024ECF8, BTL_CAM_MODEY_MINUS_1, BTL_CAM_MODEX_1, FALSE)
    EndIf
    Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_BattleBowser_BowsersBrother_Walk)
    Call(SetGoalToTarget, ACTOR_SELF)
    Call(AddGoalPos, ACTOR_SELF, 60, 0, 0)
    Call(SetActorSpeed, ACTOR_SELF, Float(4.5))
    Call(RunToGoal, ACTOR_SELF, 0, FALSE)
    Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_BattleBowser_BowsersBrother_Idle)
    Wait(15)
    Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_BattleBowser_BowsersBrother_PreJump)
    Wait(3)
    Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_BattleBowser_BowsersBrother_Jump)
    Call(SetActorJumpGravity, ACTOR_SELF, Float(1.2))
    Call(EnemyTestTarget, ACTOR_SELF, LVarA, 0, 0, 1, BS_FLAGS1_INCLUDE_POWER_UPS)
    Switch(LVarA)
        CaseOrEq(HIT_RESULT_MISS)
        CaseOrEq(HIT_RESULT_LUCKY)
            Call(SetGoalToTarget, ACTOR_SELF)
            Call(GetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
            Sub(LVar0, 30)
            Set(LVar1, 0)
            Call(SetActorSounds, ACTOR_SELF, ACTOR_SOUND_JUMP, SOUND_NONE, 0)
            Call(SetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
            Call(JumpToGoal, ACTOR_SELF, 18, FALSE, TRUE, FALSE)
            Thread
                Call(N(StartRumbleWithParams), 80, 14)
                Call(ShakeCam, CAM_BATTLE, 0, 4, Float(3.0))
            EndThread
            Sub(LVar0, 35)
            Call(SetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
            Call(JumpToGoal, ACTOR_SELF, 14, FALSE, TRUE, FALSE)
            Thread
                Call(N(StartRumbleWithParams), 80, 14)
                Call(ShakeCam, CAM_BATTLE, 0, 4, Float(2.0))
            EndThread
            Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_BattleBowser_BowsersBrother_PostJump)
            Wait(3)
            Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_BattleBowser_BowsersBrother_Idle)
            Wait(25)
            Call(UseBattleCamPreset, BTL_CAM_DEFAULT)
            IfEq(LVarA, HIT_RESULT_LUCKY)
                Call(EnemyTestTarget, ACTOR_SELF, LVar0, DAMAGE_TYPE_TRIGGER_LUCKY, 0, 0, 0)
            EndIf
            Call(SetActorSpeed, ACTOR_SELF, Float(6.0))
            Set(LVar1, ANIM_BattleBowser_BowsersBrother_Walk)
            ExecWait(N(EVS_ReturnHome))
            Return
        EndCaseGroup
        CaseDefault
    EndSwitch
    Call(SetGoalToTarget, ACTOR_SELF)
    Call(JumpToGoal, ACTOR_SELF, 15, FALSE, TRUE, FALSE)
    Thread
        Call(ShakeCam, CAM_BATTLE, 0, 4, Float(3.0))
    EndThread
    Wait(2)
    Call(SetGoalToTarget, ACTOR_SELF)
    Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_BattleBowser_BowsersBrother_Land)
    // Call(SetDamageSource, DMG_SRC_CRUSH)
    Call(EnemyDamageTarget, ACTOR_SELF, LVarF, 0, 0, 0, DMG_JUMP, BS_FLAGS1_TRIGGER_EVENTS)
    // Call(LandJump, ACTOR_SELF)
    Call(N(StartRumbleWithParams), 80, 14)
    Call(GetActorPos, ACTOR_SELF, LVar0, LVar1, LVar2)
    Add(LVar0, 40)
    Set(LVar1, 0)
    Call(SetActorSounds, ACTOR_SELF, ACTOR_SOUND_JUMP, SOUND_NONE, 0)
    Call(SetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
    Call(JumpToGoal, ACTOR_SELF, 18, FALSE, TRUE, FALSE)
    Call(PlaySoundAtActor, ACTOR_SELF, SOUND_HEAVY_NPC_LANDING)
    Thread
        Call(N(StartRumbleWithParams), 80, 14)
        Call(ShakeCam, CAM_BATTLE, 0, 4, Float(3.0))
    EndThread
    Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_BattleBowser_BowsersBrother_PostJump)
    Wait(3)
    Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_BattleBowser_BowsersBrother_Idle)
    Wait(15)
    Switch(LVarF)
        CaseOrEq(HIT_RESULT_HIT)
        CaseOrEq(HIT_RESULT_NO_DAMAGE)
        CaseOrEq(HIT_RESULT_10)
            IfEq(LVarF, HIT_RESULT_10)
                Return
            EndIf
            Call(UseBattleCamPreset, BTL_CAM_DEFAULT)
            Call(SetActorSpeed, ACTOR_SELF, Float(6.0))
            Set(LVar1, ANIM_BattleBowser_BowsersBrother_Walk)
            ExecWait(N(EVS_ReturnHome))
        EndCaseGroup
    EndSwitch
    Return
    End
};

// EvtScript N(EVS_AttackMissed) = {
//     Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_BattleBowser_BowsersBrother_Tantrum)
//     Thread
//         Wait(5)
//         Loop(4)
//             Call(N(StartRumbleWithParams), 80, 14)
//             Call(ShakeCam, CAM_BATTLE, 0, 2, Float(2.0))
//             Wait(4)
//         EndLoop
//         Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_BattleBowser_BowsersBrother_Idle)
//     EndThread
//     Return
//     End
// };

EvtScript N(EVS_Attack_FireBall) = {
    Call(SetTargetActor, ACTOR_SELF, ACTOR_PLAYER)
    Call(SetGoalToTarget, ACTOR_SELF)
    Call(UseBattleCamPreset, BTL_CAM_PRESET_19)
    Call(GetActorPos, ACTOR_SELF, LVar0, LVar1, LVar2)
    Sub(LVar0, 30)
    Add(LVar1, 65)
    Call(SetBattleCamTarget, LVar0, LVar1, LVar2)
    Call(SetBattleCamZoom, 400)
    Call(MoveBattleCamOver, 40)
    Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_BattleBowser_BowsersBrother_PreFireBreath)
    Wait(45)
    Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_BattleBowser_BowsersBrother_FireBreathStill)
    Call(UseBattleCamPreset, BTL_CAM_DEFAULT)
    Call(MoveBattleCamOver, 55)
    Wait(15)
    Call(GetActorPos, ACTOR_SELF, LVar0, LVar1, LVar2)

    Wait(15)
    Call(SetGoalToTarget, ACTOR_SELF)
    Call(EnemyTestTarget, ACTOR_SELF, LVarA, 0, 0, 1, BS_FLAGS1_INCLUDE_POWER_UPS)
    Switch(LVarA)
        CaseOrEq(HIT_RESULT_MISS)
        CaseOrEq(HIT_RESULT_LUCKY)
            // Wait(10)
            // Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_BattleBowser_BowsersBrother_FireBreathLoop)
            // Wait(30)
            Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_BattleBowser_BowsersBrother_PostFireBreath)
            Wait(15)
            Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_BattleBowser_BowsersBrother_Idle)
            // Wait(15)
            // ExecWait(N(EVS_AttackMissed))
            IfEq(LVarA, HIT_RESULT_LUCKY)
                Call(EnemyTestTarget, ACTOR_SELF, LVar0, DAMAGE_TYPE_TRIGGER_LUCKY, 0, 0, 0)
            EndIf
            Wait(15)
            Return
        EndCaseGroup
        CaseDefault
    EndSwitch
    Wait(2)
    DebugPrintf("Guard Now!\n")
    DebugPrintf("Frame: %s\n", "1/3")
    Wait(1)
    DebugPrintf("Guard Now!\n")
    DebugPrintf("Frame: %s\n", "2/3")
    Wait(1)
    DebugPrintf("Guard Now!\n")
    DebugPrintf("Frame: %s\n", "3/3")
    Call(SetGoalToTarget, ACTOR_SELF)
    Call(EnemyDamageTarget, ACTOR_SELF, LVarF, DAMAGE_TYPE_FIRE, SUPPRESS_EVENT_ALL, 0, DMG_FIRE_BALL, BS_FLAGS1_TRIGGER_EVENTS)
    Switch(LVarF)
        CaseOrEq(HIT_RESULT_HIT)
        CaseOrEq(HIT_RESULT_NO_DAMAGE)
        CaseOrEq(HIT_RESULT_10)
            // Wait(10)
            // Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_BattleBowser_BowsersBrother_FireBreathLoop)
            // Wait(30)
            Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_BattleBowser_BowsersBrother_PostFireBreath)
            Wait(15)
            Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_BattleBowser_BowsersBrother_Idle)
            IfEq(LVarF, HIT_RESULT_10)
                Return
            EndIf
        EndCaseGroup
    EndSwitch
    Return
    End
};
