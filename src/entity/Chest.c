#include "common.h"
#include "effects.h"
#include "ld_addrs.h"
#include "entity.h"

extern EntityScript Entity_Chest_ScriptOpened;

extern Gfx Entity_Chest_RenderBox[];
extern Gfx Entity_Chest_RenderLid[];
extern Mtx Entity_Chest_lidMtx;

EvtScript Entity_Chest_AdjustCam_ISK = {
    EVT_THREAD
        EVT_CALL(GetPlayerPos, LW(0), LW(1), LW(2))
        EVT_CALL(UseSettingsFrom, 0, -195, -358, -555)
        EVT_CALL(SetPanTarget, 0, LW(0), LW(1), LW(2))
        EVT_CALL(SetCamDistance, 0, EVT_FLOAT(290.0))
        EVT_CALL(SetCamPitch, 0, EVT_FLOAT(20.0), EVT_FLOAT(-10.0))
        EVT_CALL(SetCamSpeed, 0, EVT_FLOAT(8.0))
        EVT_CALL(PanToTarget, 0, 0, 1)
        EVT_CALL(WaitForCam, 0, EVT_FLOAT(1.0))
    EVT_END_THREAD
    EVT_RETURN
    EVT_END
};

EvtScript Entity_Chest_ResetCam_ISK = {
    EVT_THREAD
        EVT_CALL(ResetCam, 0, 3)
    EVT_END_THREAD
    EVT_RETURN
    EVT_END
};

EvtScript Entity_Chest_AdjustCam_TIK = {
    EVT_THREAD
        EVT_CALL(AdjustCam, 0, EVT_FLOAT(8.0), 0, EVT_FLOAT(300.0), EVT_FLOAT(17.5), EVT_FLOAT(-9.5))
    EVT_END_THREAD
    EVT_RETURN
    EVT_END
};

EvtScript Entity_Chest_AdjustCam_KZN = {
    EVT_THREAD
        EVT_CALL(AdjustCam, 0, EVT_FLOAT(8.0), 0, EVT_FLOAT(210.0), EVT_FLOAT(21.0), EVT_FLOAT(-16.0))
    EVT_END_THREAD
    EVT_RETURN
    EVT_END
};

EvtScript Entity_Chest_ResetCam_Default = {
    EVT_THREAD
        EVT_CALL(ResetCam, 0, 3)
    EVT_END_THREAD
    EVT_RETURN
    EVT_END
};

void entity_Chest_adjust_camera(Entity* entity) {
    s16 areaID;
    EvtScript* script;

    areaID = gGameStatusPtr->areaID;
    script = NULL;
    if (areaID == AREA_ISK) {
        script = &Entity_Chest_AdjustCam_ISK;
    } else if (areaID == AREA_TIK) {
        script = &Entity_Chest_AdjustCam_TIK;
    } else if (areaID == AREA_KZN) {
        script = &Entity_Chest_AdjustCam_KZN;
    }
    if (script != NULL) {
        start_script(script, EVT_PRIORITY_A, EVT_FLAG_20);
    }
}

void entity_Chest_reset_camera(Entity* entity) {
    s16 areaID;
    EvtScript* script;

    areaID = gGameStatusPtr->areaID;
    script = NULL;
    if (areaID == AREA_ISK) {
        script = &Entity_Chest_ResetCam_ISK;
    } else {
        script = &Entity_Chest_ResetCam_Default;
    }
    start_script(script, EVT_PRIORITY_A, EVT_FLAG_20);
}

void entity_Chest_setupGfx(s32 entityIndex) {
    Gfx* gfxPos = gMasterGfxPos;
    Entity* entity = get_entity_by_index(entityIndex);
    ChestData* data = entity->dataBuf.chest;
    Matrix4f sp18;
    Matrix4f sp58;
    Gfx* gfx;

    guRotateF(sp58, data->lidAngle, 1.0f, 0.0f, 0.0f);
    guMtxL2F(sp18, ENTITY_ADDR(entity, Mtx*, &Entity_Chest_lidMtx));
    guMtxCatF(sp58, sp18, sp18);
    guMtxF2L(sp18, &gDisplayContext->matrixStack[gMatrixListPos]);
    gSPMatrix(gfxPos++, &gDisplayContext->matrixStack[gMatrixListPos++], G_MTX_PUSH | G_MTX_MUL | G_MTX_MODELVIEW);
    gfx = ENTITY_ADDR(entity, Gfx*, Entity_Chest_RenderLid);
    gSPDisplayList(gfxPos++, gfx);
    gSPPopMatrix(gfxPos++, G_MTX_MODELVIEW);
    gMasterGfxPos = gfxPos;
}

void entity_Chest_check_opened(Entity* entity) {
    ChestData* data = entity->dataBuf.chest;
    if ((data->gameFlagIndex != 0xFFFF) && (get_global_flag(data->gameFlagIndex) != 0)) {
        entity->flags |= ENTITY_FLAGS_4000;
        data->itemID = -1;
        data->lidAngle = -28.7f;
        set_entity_commandlist(entity, Entity_Chest_ScriptOpened);
    }
}

void entity_Chest_idle(Entity* entity) {
    f32 rotation;
    f32 angle; // angle from the Chest to the Player
    ChestData* data;
    PlayerStatus* playerStatus = &gPlayerStatus;

    rotation = clamp_angle(180.0f - entity->rotation.y);
    angle = fabsf(rotation - clamp_angle(atan2(entity->position.x, entity->position.z, playerStatus->position.x, playerStatus->position.z)));
    if ((!(playerStatus->animFlags & PLAYER_STATUS_ANIM_FLAGS_HOLDING_WATT)) &&
        (!(entity->collisionFlags & ENTITY_COLLISION_PLAYER_TOUCH_FLOOR)) &&
        ((angle <= 40.0f) || (angle >= 320.0f))) {
        entity->flags |= ENTITY_FLAGS_SHOWS_INSPECT_PROMPT;
        if ((playerStatus->animFlags & PLAYER_STATUS_ANIM_FLAGS_INTERACT_PROMPT_AVAILABLE) && (entity->collisionFlags & ENTITY_COLLISION_PLAYER_TOUCH_WALL)) {
            exec_entity_commandlist(entity);
            data = entity->dataBuf.chest;
            data->state = 0;
            entity->flags &= ~ENTITY_FLAGS_SHOWS_INSPECT_PROMPT;
            if (data->itemID != 0) {
                disable_player_input();
            }
            func_800EF3E4();
        }
    } else {
        entity->flags &= ~ENTITY_FLAGS_SHOWS_INSPECT_PROMPT;
    }
}

void entity_Chest_begin_opening(Entity* entity) {
    ChestData* data = entity->dataBuf.chest;
    data->postLidAnimDelay = 10;
    data->lidAngle = 0.0f;
    data->openState = 0;
    data->giveItemTimer = 40;
    sfx_play_sound(SOUND_1D3);
}

void entity_Chest_open(Entity* entity) {
    ChestData* data = entity->dataBuf.chest;
    f32 temp;

    switch (data->openState) {
        case 0:
            data->postLidAnimDelay--;
            data->lidAngle -= 1.0f;
            if (data->lidAngle < -2.0f) {
                data->lidAngle = -2.0f;
            }

            if (data->postLidAnimDelay == 0) {
                data->postLidAnimDelay = 8;
                data->openState++;
            }
            break;
        case 1:
            data->postLidAnimDelay--;
            if (data->postLidAnimDelay == 0) {
                data->lidAnimInterpPhase = 0.0f;
                data->openState++;
            }
            break;
        case 2:
            data->lidAnimInterpPhase += 4.0f;
            if (data->lidAnimInterpPhase >= 180.0f) {
                data->openState++;
            }
            temp = sin_rad(data->lidAnimInterpPhase * TAU / 360.0f) * 3.0f;
            data->lidAngle -= temp;
            break;
        case 3:
            data->lidAnimInterpPhase += 1.0f;
            if (data->lidAnimInterpPhase >= 190.0f) {
                data->postLidAnimDelay = 10;
                data->openState++;
            }
            temp = sin_rad(data->lidAnimInterpPhase * TAU / 360.0f) * 2.0f;
            data->lidAngle -= temp;
            break;
        case 4:
            data->postLidAnimDelay--;
            if (data->postLidAnimDelay == 0) {
                if (data->unk_07 != 0) {
                    exec_entity_commandlist(entity);
                } else {
                    data->openState++;
                }
            }
            break;
        case 5:
            break;

    }
}

void entity_Chest_close(Entity* entity) {
    ChestData* data = entity->dataBuf.chest;
    f32 delta;

    switch (data->state) {
        case 0:
            data->lidAnimInterpPhase = 0.0f;
            data->state++;
            // fallthrough
        case 1:
            data->lidAnimInterpPhase += 5.0f;
            if (data->lidAnimInterpPhase >= 180.0f) {
                data->lidAnimInterpPhase = 180.0f;
                data->state++;
            }
            delta = 2.6f * sin_rad(data->lidAnimInterpPhase * TAU / 360.0f);
            data->lidAngle += delta;
            break;
        case 2:
            data->lidAnimInterpPhase += 1.0f;
            if (data->lidAnimInterpPhase >= 185.0f) {
                data->lidAnimInterpPhase = 185.0f;
                data->postLidAnimDelay = 10;
                data->state++;
            }
            delta = 2.0f * sin_rad(data->lidAnimInterpPhase * TAU / 360.0f);
            data->lidAngle += delta;
            break;
        case 3:
            data->postLidAnimDelay--;
            if (data->postLidAnimDelay == 0) {
                data->state++;
                entity->flags |= ENTITY_FLAGS_4000;
            }
            break;
        case 4:
            break;
    }
}

void entity_GiantChest_hide_effect(Entity* entity) {
    ChestData* chest = entity->dataBuf.chest;
    EffectInstance* effect = chest->gotItemEffect;

    if (effect != NULL) {
        effect->data.gotItemOutline->unk_14 = 10;
    }
}

void entity_GiantChest_open(Entity* entity) {
    PlayerStatus* playerStatus = &gPlayerStatus;
    ChestData* chest = entity->dataBuf.chest;
    f32 cosFwd;
    f32 sinFwd;
    f32 cosRight;
    f32 sinRight;
    f32 theta;
    f32 giveItemLerpAlpha;
    f32 horizontalOffset;
    f32 dy;
    f32 depthOffset;
    f32 intermediateTheta;
    f32 radius;

    if (chest->itemID != 0) {
        set_item_entity_position(chest->itemEntityIndex, chest->itemEntityPos.x, chest->itemEntityPos.y, chest->itemEntityPos.z);
    }
    entity_Chest_open(entity);

    switch (chest->state) {
        case 0:
            chest->giveItemTimer--;
            if (chest->giveItemTimer == 0) {
                chest->giveItemRadiusInterpPhase = 0.0f;
                chest->state++;
                chest->giveItemHeightInterpPhase = 20.0f;
                chest->itemVelY = 8.0f;
            }
            break;
        case 1:
            chest->itemEntityPos.y += chest->itemVelY;
            chest->itemVelY -= 0.7;
            if (chest->itemVelY <= 2.0f) {
                chest->itemVelY = 6.0f;
                chest->state++;
            }
            break;
        case 2:
            if (chest->giveItemHeightInterpPhase < 140.0f) {
                chest->itemEntityPos.y += cos_rad(chest->giveItemHeightInterpPhase * TAU / 360.0f) * 3.0f;
            } else {
                dy = (chest->itemEntityPos.y - playerStatus->position.y - 30.0f) * 0.25f;
                if (dy <= 0.4) {
                    dy = 0.4f;
                }
                chest->itemEntityPos.y -= dy;
            }
            giveItemLerpAlpha = sin_rad(chest->giveItemRadiusInterpPhase * TAU / 360.0f);
            theta = intermediateTheta = clamp_angle(atan2(entity->position.x, entity->position.z, playerStatus->position.x, playerStatus->position.z));

            if (gGameStatusPtr->areaID == AREA_KZN) {
                radius = 3.0f;
            } else {
                radius = 4.0f;
            }

            theta *= TAU;
            theta = theta / 360.0f;
            chest->itemEntityPos.x += (radius * sin_rad(theta) * giveItemLerpAlpha);
            chest->itemEntityPos.z -= (radius * cos_rad(theta) * giveItemLerpAlpha);
            chest->giveItemRadiusInterpPhase += 8.0f;
            if (chest->giveItemRadiusInterpPhase >= 180.0f) {
                chest->giveItemRadiusInterpPhase = 180.0f;
            }
            chest->giveItemHeightInterpPhase += 13.0f;
            if (chest->giveItemHeightInterpPhase >= 180.0f){
                chest->giveItemTimer = 7;
                chest->giveItemHeightInterpPhase = 180.0f;
                chest->state++;
                if (chest->itemID != 0) {
                    suggest_player_anim_setUnkFlag(0x6000C);
                    sin_cos_rad((90.0f - gCameras[CAM_DEFAULT].currentYaw) * TAU / 360.0f, &sinRight, &cosRight);
                    sin_cos_rad((180.0f - gCameras[CAM_DEFAULT].currentYaw) * TAU / 360.0f, &sinFwd, &cosFwd);
                    horizontalOffset = 0.0f;
                    depthOffset = 4.0f;
                    //RadialFlowOut
                    fx_radial_shimmer(0, chest->itemEntityPos.x + horizontalOffset * sinRight - depthOffset * sinFwd,
                              chest->itemEntityPos.y + 12.0f, chest->itemEntityPos.z - cosRight * horizontalOffset + cosFwd * depthOffset, 1.0f, 0x64);
                    //GotItemOutline
                    fx_got_item_outline(0, chest->itemEntityPos.x + horizontalOffset * sinRight - sinFwd * depthOffset,
                              chest->itemEntityPos.y + 12.0f, chest->itemEntityPos.z - cosRight * horizontalOffset + cosFwd * depthOffset, 1.0f, &chest->gotItemEffect);
                }
            }
            break;
        case 3:
            chest->giveItemTimer--;
            if (chest->giveItemTimer == 0) {
                exec_entity_commandlist(entity);
                chest->state = 0;
            }
            break;
    }
}

void entity_GiantChest_give_equipment(Entity* entity) {
    ChestData* data = entity->dataBuf.chest;
    f32 angle;
    s32 flag;

    switch (data->itemID) {
        case ITEM_JUMP:
            gPlayerData.bootsLevel = 0;
            break;
        case ITEM_SPIN_JUMP:
            gPlayerData.bootsLevel = 1;
            break;
        case ITEM_TORNADO_JUMP:
            gPlayerData.bootsLevel = 2;
            break;
        case ITEM_HAMMER:
            gPlayerData.hammerLevel = 0;
            break;
        case ITEM_SUPER_HAMMER:
            gPlayerData.hammerLevel = 1;
            break;
        case ITEM_ULTRA_HAMMER:
            gPlayerData.hammerLevel = 2;
            break;
    }

    if (data->itemID != 0) {
        angle = (entity->rotation.y * TAU) / 360.0f;
        data->itemEntityPos.x = entity->position.x + (sin_rad(angle) * 10.0f);
        data->itemEntityPos.y = entity->position.y;
        data->itemEntityPos.z = entity->position.z + (cos_rad(angle) * 10.0f);
        data->itemEntityIndex = make_item_entity_nodelay(data->itemID, data->itemEntityPos.x, data->itemEntityPos.y, data->itemEntityPos.z, 1, -1);
    }

    flag = data->gameFlagIndex;
    if (flag <= EVT_SAVE_FLAG(10000000)) {
        flag += 130000000;
    }
    set_global_flag(flag);
}

void entity_Chest_start_bound_script(Entity* entity) {
    entity_start_script(entity);
}

void entity_Chest_enable_player_input(Entity* entity) {
    ChestData* data = entity->dataBuf.chest;
    if (data->itemID != 0) {
        enable_player_input();
    }
}

void entity_GiantChest_await_got_item(Entity* entity) {
    ChestData* data = entity->dataBuf.chest;
    if (data->itemID != 0) {
        if (data->unk_30 != 0) {
            exec_entity_commandlist(entity);
            remove_item_entity_by_index(data->itemEntityIndex);
            suggest_player_anim_clearUnkFlag(0x10002);
            enable_player_input();
            data->itemID = -1;
        }
        return;
    }
    exec_entity_commandlist(entity);
}

void entity_Chest_clear_item_id(Entity* entity) {
    ChestData* data = entity->dataBuf.chest;
    data->itemID = -1;
}

void entity_Chest_readargs(Entity* entity) {
    ChestData* data = entity->dataBuf.chest;
    data->itemID = CreateEntityVarArgBuffer[0];
    data->gameFlagIndex = 0xFFFF;
}

void entity_GiantChest_init(Entity* entity) {
    entity_Chest_readargs(entity);
    entity->renderSetupFunc = &entity_Chest_setupGfx;
}

void entity_Chest_init(Entity* entity) {
    ChestData* data = entity->dataBuf.chest;
    data->unk_07 = 1;
    entity_Chest_readargs(entity);
    entity->renderSetupFunc = &entity_Chest_setupGfx;
    entity->scale.x = 0.56f;
    entity->scale.y = 0.56f;
    entity->scale.z = 0.56f;
}

EntityScript Entity_GiantChest_Script = {
    es_Call(entity_Chest_check_opened)
    es_SetCallback(entity_Chest_idle, 0)
    es_Call(entity_Chest_adjust_camera)
    es_Call(entity_GiantChest_give_equipment)
    es_Call(entity_Chest_begin_opening)
    es_SetCallback(entity_GiantChest_open, 0)
    es_Call(entity_Chest_start_bound_script)
    es_SetCallback(entity_GiantChest_await_got_item, 0)
    es_Call(entity_GiantChest_hide_effect)
    es_Call(entity_Chest_reset_camera)
    es_SetCallback(entity_Chest_close, 0)
    es_SetCallback(NULL, 0)
    es_Restart
    es_End
};

EntityScript Entity_Chest_ScriptOpened = {
    es_SetCallback(NULL, 0)
    es_End
};
EntityScript Entity_Chest_Script = {
    es_Call(entity_Chest_check_opened)
    es_SetCallback(entity_Chest_idle, 0)
    es_Call(entity_Chest_begin_opening)
    es_Call(entity_Chest_start_bound_script)
    es_Call(entity_Chest_clear_item_id)
    es_SetCallback(entity_Chest_open, 0)
    es_Call(entity_Chest_enable_player_input)
    es_SetCallback(entity_Chest_close, 0)
    es_SetCallback(NULL, 0)
    es_Restart
    es_End
};
EntityModelScript Entity_Chest_RenderScript = STANDARD_ENTITY_MODEL_SCRIPT(Entity_Chest_RenderBox, RENDER_MODE_SURFACE_OPA);

EntityBlueprint Entity_GiantChest = {
    .flags = ENTITY_FLAGS_4000,
    .typeDataSize = sizeof(ChestData),
    .renderCommandList = Entity_Chest_RenderScript,
    .modelAnimationNodes = 0,
    .fpInit = entity_GiantChest_init,
    .updateEntityScript = Entity_GiantChest_Script,
    .fpHandleCollision = NULL,
    { .dma = ENTITY_ROM(Chest) },
    .entityType = ENTITY_TYPE_CHEST,
    .aabbSize = { 50, 45, 46 }
};
EntityBlueprint Entity_Chest = {
    .flags = ENTITY_FLAGS_8000 | ENTITY_FLAGS_4000,
    .typeDataSize = sizeof(ChestData),
    .renderCommandList = Entity_Chest_RenderScript,
    .modelAnimationNodes = 0,
    .fpInit = entity_Chest_init,
    .updateEntityScript = Entity_Chest_Script,
    .fpHandleCollision = NULL,
    { .dma = ENTITY_ROM(Chest) },
    .entityType = ENTITY_TYPE_CHEST,
    .aabbSize = { 28, 26, 23 }
};
