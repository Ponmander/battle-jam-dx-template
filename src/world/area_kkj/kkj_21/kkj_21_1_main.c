#include "kkj_21.h"

EvtScript N(EVS_ExitDoor_kkj_10_2) = {
    EVT_SET_GROUP(EVT_GROUP_1B)
    EVT_CALL(DisablePlayerInput, TRUE)
    EVT_SET(LVar0, kkj_21_ENTRY_0)
    EVT_SET(LVar1, COLLIDER_tte)
    EVT_SET(LVar2, MODEL_o57)
    EVT_SET(LVar3, DOOR_SWING_IN)
    EVT_EXEC(ExitSingleDoor)
    EVT_WAIT(17)
    EVT_CALL(GotoMap, EVT_PTR("kkj_10"), kkj_10_ENTRY_2)
    EVT_WAIT(100)
    EVT_RETURN
    EVT_END
};

EvtScript N(EVS_EnterMap) = {
    EVT_CALL(GetEntryID, LVar0)
    EVT_SWITCH(LVar0)
        EVT_CASE_EQ(kkj_21_ENTRY_0)
            EVT_SET(LVar2, MODEL_o57)
            EVT_SET(LVar3, DOOR_SWING_IN)
            EVT_EXEC_WAIT(EnterSingleDoor)
    EVT_END_SWITCH
    EVT_RETURN
    EVT_END
};

EvtScript N(EVS_Main) = {
    EVT_SET(GB_WorldLocation, LOCATION_PEACHS_CASTLE)
    EVT_CALL(SetSpriteShading, SHADING_NONE)
    EVT_SETUP_CAMERA_NO_LEAD()
    EVT_CALL(UseDoorSounds, DOOR_SOUNDS_BASIC)
    EVT_BIND_TRIGGER(EVT_PTR(N(EVS_ExitDoor_kkj_10_2)), TRIGGER_WALL_PRESS_A, COLLIDER_tte, 1, 0)
    EVT_EXEC(N(EVS_EnterMap))
    EVT_WAIT(1)
    EVT_RETURN
    EVT_END
};
