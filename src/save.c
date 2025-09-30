#include "global.h"
#include "fieldmap.h"
#include "save.h"
#include "task.h"
#include "decompress.h"
#include "load_save.h"
#include "overworld.h"
#include "main.h"
#include "new_game.h"
#include "menu.h"
#include "event_data.h"
#include "battle_setup.h"
#include "constants/flags.h"
#include "constants/trainers.h"

COMMON_DATA u16 gSaveFileStatus = 0;
COMMON_DATA void (*gGameContinueCallback)(void) = NULL;

bool8 Save_VerifySlot(u8 slotId);
static u16 Save_CalcChecksum(u8 slotId);

void ClearSaveSlot(u8 slotId);
void ClearSaveData(void);

void Save_WriteBlock(u8 slotId, u16 offset, const void *data, u16 size);
void Save_Commit(u8 slotId);

static u8 Save_WriteChunk(u8 slotId, u16 offset, const void *data, u16 size);
static u8 Save_ReadChunk(u8 slotId, u16 offset, void *dest, u16 size);

void AutoSave(void) 
{
    TrySavingData(SAVE_NORMAL, SAVE_AUTO_2);
}

u8 GetCompletion(void)
{
    u8 result = 0;

    if (FlagGet(FLAG_UNLOCK_MANOR_WARP))
        result += 10;

    if (FlagGet(FLAG_UNLOCK_FOREST_WARP))
        result += 10;

    if (FlagGet(FLAG_UNLOCK_CAVE_WARP))
        result += 10;

    if (FlagGet(FLAG_UNLOCK_BRIDGE_WARP))
        result += 10;

    if (HasTrainerBeenFought(TRAINER_BERUS))
        result += 10;
    
    if (HasTrainerBeenFought(TRAINER_DEITY_TENJIN))
        result += 10;   

    if (HasTrainerBeenFought(TRAINER_EXAMINER_FIONN))
        result += 10;

    if (HasTrainerBeenFought(TRAINER_EXAMINER_SORA))
        result += 10;

    if (HasTrainerBeenFought(TRAINER_EXAMINER_DANTE))
        result += 10;

    if (HasTrainerBeenFought(TRAINER_EXAMINER_RUNE))
        result += 10;   
    
    return result;
}


void CheckAnySlotValid()
{
    if (SlotIsValid(SAVE_AUTO_1))
    {
        gSaveFileStatus = SAVE_STATUS_OK;
        return;
    }

    if (SlotIsValid(SAVE_AUTO_2))
    {
        gSaveFileStatus = SAVE_STATUS_OK;
        return;
    }

    if (SlotIsValid(SAVE_MANUAL_1))
    {
        gSaveFileStatus = SAVE_STATUS_OK;
        return;
    }

    if (SlotIsValid(SAVE_MANUAL_2))
    {
        gSaveFileStatus = SAVE_STATUS_OK;
        return;
    }

    if (SlotIsValid(SAVE_MANUAL_3))
    {
        gSaveFileStatus = SAVE_STATUS_OK;
        return;
    }

    if (SlotIsValid(SAVE_MANUAL_4))
    {
        gSaveFileStatus = SAVE_STATUS_OK;
        return;
    }

    gSaveFileStatus = SAVE_STATUS_EMPTY;
}

u16 GetSaveBlocksPointersBaseOffset(void)
{
    return 0;
}

u8 TrySavingData(u8 saveType, u8 slotId)
{
    GlobalSaveHeader *hdr = (GlobalSaveHeader *)FRAM_BASE;

    // Save options into global header
    hdr->optionsButtonMode = gSaveBlock2Ptr->optionsButtonMode;
    {
        u8 *base = &gSaveBlock2Ptr->optionsButtonMode;
        u16 *optionsPtr = (u16 *)(base + 1);
        hdr->optionsPacked = *optionsPtr;
    }

    // --- Player name ---
    Save_WriteChunk(slotId, SLOT_OFF_NAME, gSaveBlock2Ptr->playerName, SLOT_SZ_NAME);

    // --- Nickname ---
    {
        u8 nickname[SLOT_SZ_NICK];
        GetMonData(&gPlayerParty[0], MON_DATA_NICKNAME, nickname);
        Save_WriteChunk(slotId, SLOT_OFF_NICK, nickname, SLOT_SZ_NICK);
    }

    // --- Moves, item, ability, species ---
    u16 monData;
    monData = GetMonData(&gPlayerParty[0], MON_DATA_MOVE1);
    Save_WriteChunk(slotId, SLOT_OFF_MOVE1, &monData, 2);

    monData = GetMonData(&gPlayerParty[0], MON_DATA_MOVE2);
    Save_WriteChunk(slotId, SLOT_OFF_MOVE2, &monData, 2);

    monData = GetMonData(&gPlayerParty[0], MON_DATA_MOVE3);
    Save_WriteChunk(slotId, SLOT_OFF_MOVE3, &monData, 2);

    monData = GetMonData(&gPlayerParty[0], MON_DATA_MOVE4);
    Save_WriteChunk(slotId, SLOT_OFF_MOVE4, &monData, 2);

    monData = GetMonData(&gPlayerParty[0], MON_DATA_HELD_ITEM);
    Save_WriteChunk(slotId, SLOT_OFF_ITEM, &monData, 2);

    monData = GetMonData(&gPlayerParty[0], MON_DATA_OVERWRITTEN_ABILITY);
    Save_WriteChunk(slotId, SLOT_OFF_ABILITY, &monData, 2);

    monData = GetMonData(&gPlayerParty[0], MON_DATA_SPECIES);
    Save_WriteChunk(slotId, SLOT_OFF_SPECIES, &monData, 2);

    // --- Map / coords ---
    Save_WriteChunk(slotId, SLOT_OFF_MAPNUM, &gSaveBlock1Ptr->location.mapNum, 1);
    Save_WriteChunk(slotId, SLOT_OFF_POSX, &gSaveBlock1Ptr->pos.x, 2);
    Save_WriteChunk(slotId, SLOT_OFF_POSY, &gSaveBlock1Ptr->pos.y, 2);

    // --- Play time ---
    Save_WriteChunk(slotId, SLOT_OFF_PLAY_H, &gSaveBlock2Ptr->playTimeHours, 2);
    Save_WriteChunk(slotId, SLOT_OFF_PLAY_M, &gSaveBlock2Ptr->playTimeMinutes, 1);
    Save_WriteChunk(slotId, SLOT_OFF_PLAY_S, &gSaveBlock2Ptr->playTimeSeconds, 1);
    Save_WriteChunk(slotId, SLOT_OFF_PLAY_VB, &gSaveBlock2Ptr->playTimeVBlanks, 1);

    // --- Completion ---
    u8 completion = GetCompletion();
    Save_WriteChunk(slotId, SLOT_OFF_COMPLETION, &completion, 1);

    // --- Flags ---
    Save_WriteChunk(slotId, SLOT_OFF_FLAGS, gSaveBlock1Ptr->flags, NUM_FLAG_BYTES);

    Save_Commit(slotId);
    return SAVE_STATUS_OK;
}


u8 LoadGameSave(u8 saveType, u8 slotId)
{
    GlobalSaveHeader *hdr = (GlobalSaveHeader *)FRAM_BASE;

    if (slotId == LAST_SAVED_SLOT)
    {
        slotId = hdr->lastSaveSlot;
    }

    if (slotId == 0xFF || !SlotIsValid(slotId)) 
    {
        gSaveFileStatus = SAVE_STATUS_EMPTY;
        return SAVE_STATUS_ERROR;
    }

    // Restore options
    gSaveBlock2Ptr->optionsButtonMode = hdr->optionsButtonMode;
    {
        u8 *base = &gSaveBlock2Ptr->optionsButtonMode;
        u16 *optionsPtr = (u16 *)(base + 1);
        *optionsPtr = hdr->optionsPacked;
    }

    NewGameInitData();

    // --- Player name ---
    Save_ReadChunk(slotId, SLOT_OFF_NAME, gSaveBlock2Ptr->playerName, SLOT_SZ_NAME);

    // --- Nickname ---
    {
        u8 nickname[SLOT_SZ_NICK];
        Save_ReadChunk(slotId, SLOT_OFF_NICK, nickname, SLOT_SZ_NICK);
        SetMonData(&gPlayerParty[0], MON_DATA_NICKNAME, nickname);
    }

    // --- Moves, item, ability, species ---
    u16 monData;
    Save_ReadChunk(slotId, SLOT_OFF_MOVE1, &monData, 2);
    SetMonData(&gPlayerParty[0], MON_DATA_MOVE1, &monData);

    Save_ReadChunk(slotId, SLOT_OFF_MOVE2, &monData, 2);
    SetMonData(&gPlayerParty[0], MON_DATA_MOVE2, &monData);

    Save_ReadChunk(slotId, SLOT_OFF_MOVE3, &monData, 2);
    SetMonData(&gPlayerParty[0], MON_DATA_MOVE3, &monData);

    Save_ReadChunk(slotId, SLOT_OFF_MOVE4, &monData, 2);
    SetMonData(&gPlayerParty[0], MON_DATA_MOVE4, &monData);

    Save_ReadChunk(slotId, SLOT_OFF_ITEM, &monData, 2);
    SetMonData(&gPlayerParty[0], MON_DATA_HELD_ITEM, &monData);

    Save_ReadChunk(slotId, SLOT_OFF_ABILITY, &monData, 2);
    SetMonData(&gPlayerParty[0], MON_DATA_OVERWRITTEN_ABILITY, &monData);

    Save_ReadChunk(slotId, SLOT_OFF_SPECIES, &monData, 2);
    SetMonData(&gPlayerParty[0], MON_DATA_SPECIES, &monData);

    // --- Map / coords ---
    Save_ReadChunk(slotId, SLOT_OFF_MAPNUM, &gSaveBlock1Ptr->location.mapNum, 1);
    Save_ReadChunk(slotId, SLOT_OFF_POSX, &gSaveBlock1Ptr->pos.x, 2);
    Save_ReadChunk(slotId, SLOT_OFF_POSY, &gSaveBlock1Ptr->pos.y, 2);

    // --- Play time ---
    Save_ReadChunk(slotId, SLOT_OFF_PLAY_H, &gSaveBlock2Ptr->playTimeHours, 2);
    Save_ReadChunk(slotId, SLOT_OFF_PLAY_M, &gSaveBlock2Ptr->playTimeMinutes, 1);
    Save_ReadChunk(slotId, SLOT_OFF_PLAY_S, &gSaveBlock2Ptr->playTimeSeconds, 1);
    Save_ReadChunk(slotId, SLOT_OFF_PLAY_VB, &gSaveBlock2Ptr->playTimeVBlanks, 1);

    // --- Flags ---
    Save_ReadChunk(slotId, SLOT_OFF_FLAGS, gSaveBlock1Ptr->flags, NUM_FLAG_BYTES);

    gSaveFileStatus = SAVE_STATUS_OK;
    return SAVE_STATUS_OK;
}

void CopyPreviewDataToBuffer(u8 slot, u8 textId, u8 *dest)
{
    switch (textId)
    {
        case SAVE_MENU_NAME:
            Save_ReadChunk(slot, SLOT_OFF_NAME, dest, SLOT_SZ_NAME);
            break;
        case SAVE_MENU_PLAY_TIME:
            Save_ReadChunk(slot, SLOT_OFF_PLAY_H, dest, 2);
            dest += 2;
            Save_ReadChunk(slot, SLOT_OFF_PLAY_M, dest, 1);
            dest += 1;
            Save_ReadChunk(slot, SLOT_OFF_PLAY_S, dest, 1);
            dest += 1;
            Save_ReadChunk(slot, SLOT_OFF_PLAY_VB, dest, 1);
            break;
        case SAVE_MENU_BADGES:
            Save_ReadChunk(slot, SLOT_OFF_COMPLETION, dest, 1);
            break;
        case SAVE_MENU_CAUGHT:
        case SAVE_MENU_LOCATION:
            *(dest++) = CHAR_0;
            *(dest++) = CHAR_PERCENT;
            *(dest++) = EOS;
            break;
    }
}


bool8 SlotIsValid(u8 slotId)
{
    SaveSlotHeader hdr;

    Save_ReadChunk(slotId, 0, &hdr, sizeof(hdr));

    if (hdr.checksum == 0xFFFF)
    {
        return FALSE;
    }

    if (hdr.magic != 0x7A2C)
    {
        return FALSE;    
    }

    return Save_VerifySlot(slotId);
}

bool8 Save_VerifySlot(u8 slotId)
{
    u16 storedChecksum, calcChecksum;
    Save_ReadChunk(slotId, offsetof(SaveSlotHeader, checksum),
                   &storedChecksum, sizeof(storedChecksum));

    calcChecksum = Save_CalcChecksum(slotId);

    return (storedChecksum == calcChecksum);
}

static u16 Save_CalcChecksum(u8 slotId)
{
    u32 sum = 0;
    u8  buf[64];
    u16 remaining = (u16)(SAVE_SLOT_SIZE - SAVE_HEADER_SIZE);
    u16 pos = 0;

    while (remaining)
    {
        u16 chunk = (remaining > sizeof(buf)) ? sizeof(buf) : remaining;
        if (Save_ReadChunk(slotId, (u16)(SAVE_HEADER_SIZE + pos), buf, chunk) != SAVE_STATUS_OK)
        {
            return 0xFFFF;
        }

        for (u16 i = 0; i < chunk; i++)
            sum += buf[i];

        pos += chunk;
        remaining -= chunk;
    }

    return (u16)sum;
}


void ClearSaveSlot(u8 slotId)
{
    GlobalSaveHeader *hdr = (GlobalSaveHeader *)FRAM_BASE;
    u8 replacementSlot = SLOT_INVALID;
    u8 i;

    if (slotId == SAVE_AUTO_1 || slotId == SAVE_AUTO_2)
    {
        for (i = SAVE_AUTO_1; i <= SAVE_AUTO_2; i++)
        {
            if (i != slotId && SlotIsValid(i))
            {
                replacementSlot = i;
                break;
            }
        }

        hdr->lastAutosaveSlot = (replacementSlot != SLOT_INVALID) ? replacementSlot : 0xFF;
    }
    else
    {
        for (i = SAVE_MANUAL_1; i <= SAVE_MANUAL_3; i++)
        {
            if (i != slotId && SlotIsValid(i))
            {
                replacementSlot = i;
                break;
            }
        }

        hdr->lastSaveSlot = (replacementSlot != SLOT_INVALID) ? replacementSlot : 0xFF;
    }

    if (replacementSlot != SLOT_INVALID)
    {
        u8 buffer[SAVE_SLOT_SIZE];
        Save_ReadChunk(replacementSlot, 0, buffer, SAVE_SLOT_SIZE);
        Save_WriteChunk(slotId, 0, buffer, SAVE_SLOT_SIZE);
    }
    else
    {
        vu8 *p = SAVE_SLOT_ADDR(slotId);
        for (i = 0; i < SAVE_SLOT_SIZE; i++)
            p[i] = 0xFF;
    }
}

void ClearSaveData(void)
{
    for (u16 i = 0; i < sizeof(GlobalSaveHeader); i++)
        ((vu8*)FRAM_BASE)[i] = 0xFF;

    for (u8 slot = 0; slot < SAVE_SLOT_COUNT; slot++)
        ClearSaveSlot(slot);
}

void Save_WriteBlock(u8 slotId, u16 offset, const void *data, u16 size)
{
    Save_WriteChunk(slotId, SAVE_HEADER_SIZE + offset, data, size);
}

void Save_Commit(u8 slotId)
{
    u16 checksum;
    GlobalSaveHeader *hdr = (GlobalSaveHeader *)FRAM_BASE;
    u16 magic = 0x7A2C;

    checksum = Save_CalcChecksum(slotId);

    if (slotId == SAVE_AUTO_1 || slotId == SAVE_AUTO_2)
        hdr->lastAutosaveSlot = slotId;
    else
        hdr->lastSaveSlot = slotId;

    Save_WriteChunk(slotId, offsetof(SaveSlotHeader, magic), &magic, sizeof(magic));
    Save_WriteChunk(slotId, offsetof(SaveSlotHeader, checksum), &checksum, sizeof(checksum));
}

static u8 Save_WriteChunk(u8 slotId, u16 offset, const void *data, u16 size)
{
    vu8 *dst = SAVE_SLOT_ADDR(slotId) + offset;
    const u8 *src = (const u8*)data;
    u16 i;

    if (offset + size > SAVE_SLOT_SIZE)
    {
        DebugPrintfLevel(MGBA_LOG_ERROR, "Error writing to offset %x", offset + size);
        return SAVE_STATUS_ERROR;
    }

    for (i = 0; i < size; i++)
        dst[i] = src[i];

    return SAVE_STATUS_OK;
}

static u8 Save_ReadChunk(u8 slotId, u16 offset, void *dest, u16 size)
{
    const vu8 *src = SAVE_SLOT_ADDR(slotId) + offset;
    u8 *dst = (u8*)dest;
    u16 i;

    if (offset + size > SAVE_SLOT_SIZE)
        return SAVE_STATUS_ERROR;

    for (i = 0; i < size; i++)
        dst[i] = src[i];

    return SAVE_STATUS_OK;
}