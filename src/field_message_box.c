#include "global.h"
#include "menu.h"
#include "string_util.h"
#include "task.h"
#include "text.h"
#include "field_message_box.h"
#include "text_window.h"
#include "script.h"
#include "field_mugshot.h"
#include "sprite.h"
#include "event_data.h"

static EWRAM_DATA u8 sFieldMessageBoxMode = 0;
EWRAM_DATA u8 gWalkAwayFromSignpostTimer = 0;
EWRAM_DATA const u8* gSpeakerName = NULL;

static void ExpandStringAndStartDrawFieldMessage(const u8 *, bool32);
static void StartDrawFieldMessage(void);

void InitFieldMessageBox(void)
{
    sFieldMessageBoxMode = FIELD_MESSAGE_BOX_HIDDEN;
    gTextFlags.canABSpeedUpPrint = FALSE;
    gTextFlags.useAlternateDownArrow = FALSE;
    gTextFlags.autoScroll = FALSE;
    gTextFlags.forceMidTextSpeed = FALSE;
}

#define tState data[0]

static void Task_DrawFieldMessage(u8 taskId)
{
    struct Task *task = &gTasks[taskId];

    switch (task->tState)
    {
        case 0:
            if (gMsgIsSignPost)
                LoadSignPostWindowFrameGfx();
            else
                LoadMessageBoxAndBorderGfx();
            task->tState++;
            break;
        case 1:
            DrawDialogueFrame(0, TRUE);
            if (gSpeakerName != NULL)
            {
                DrawNamePlate(1, TRUE);
            }
            task->tState++;
            break;
        case 2:
            if (RunTextPrintersAndIsPrinter0Active() != TRUE)
            {
                sFieldMessageBoxMode = FIELD_MESSAGE_BOX_HIDDEN;
                DestroyTask(taskId);
            }
    }
}

#undef tState

static void CreateTask_DrawFieldMessage(void)
{
    CreateTask(Task_DrawFieldMessage, 0x50);
}

static void DestroyTask_DrawFieldMessage(void)
{
    u8 taskId = FindTaskIdByFunc(Task_DrawFieldMessage);
    if (taskId != TASK_NONE)
        DestroyTask(taskId);
}

bool8 ShowFieldMessage(const u8 *str)
{
    if (sFieldMessageBoxMode != FIELD_MESSAGE_BOX_HIDDEN)
        return FALSE;
    ExpandStringAndStartDrawFieldMessage(str, TRUE);
    sFieldMessageBoxMode = FIELD_MESSAGE_BOX_NORMAL;
    return TRUE;
}

bool8 ShowPokenavFieldMessage(const u8 *str)
{
    return FALSE;
}

bool8 ShowFieldAutoScrollMessage(const u8 *str)
{
    if (sFieldMessageBoxMode != FIELD_MESSAGE_BOX_HIDDEN)
        return FALSE;
    sFieldMessageBoxMode = FIELD_MESSAGE_BOX_AUTO_SCROLL;
    ExpandStringAndStartDrawFieldMessage(str, FALSE);
    return TRUE;
}

// Same as ShowFieldMessage, but instead of accepting a
// string arg it just prints whats already in gStringVar4
bool8 ShowFieldMessageFromBuffer(void)
{
    if (sFieldMessageBoxMode != FIELD_MESSAGE_BOX_HIDDEN)
        return FALSE;
    sFieldMessageBoxMode = FIELD_MESSAGE_BOX_NORMAL;
    StartDrawFieldMessage();
    return TRUE;
}

extern void FillDialogFramePlate();
extern int GetDialogFramePlateWidth();
static void ExpandStringAndStartDrawFieldMessage(const u8 *str, bool32 allowSkippingDelayWithButtonPress)
{
    if (gSpeakerName != NULL)
    {
        int strLen;
        const u8 colors[3] = {0, 1, 14};

        StringExpandPlaceholders(gStringVar4, gSpeakerName);
        strLen = GetStringWidth(FONT_SMALL, gStringVar4, -1);

        if (strLen > 0)
        {
            strLen = GetDialogFramePlateWidth() / 2 - strLen / 2;
            gNamePlateBuffer[0] = EXT_CTRL_CODE_BEGIN;
            gNamePlateBuffer[1] = EXT_CTRL_CODE_CLEAR_TO;
            gNamePlateBuffer[2] = strLen;
            StringExpandPlaceholders(&gNamePlateBuffer[3], gStringVar4);
        }
        else
        {
            StringExpandPlaceholders(&gNamePlateBuffer[0], gStringVar4);
        }

        FillDialogFramePlate();
        AddTextPrinterParameterized3(1, FONT_SMALL, 0, 0, colors, 0, gNamePlateBuffer);
    }
    StringExpandPlaceholders(gStringVar4, str);
    AddTextPrinterForMessage(allowSkippingDelayWithButtonPress);
    CreateTask_DrawFieldMessage();
    if (IsFieldMugshotActive())
    {
        gSprites[GetFieldMugshotSpriteId()].data[0] = TRUE;
    }
}

static void StartDrawFieldMessage(void)
{
    AddTextPrinterForMessage(TRUE);
    CreateTask_DrawFieldMessage();
}

void HideFieldMessageBox(void)
{
    DestroyTask_DrawFieldMessage();
    ClearDialogWindowAndFrame(0, TRUE);
    sFieldMessageBoxMode = FIELD_MESSAGE_BOX_HIDDEN;
    if (IsFieldMugshotActive())
    {
        gSprites[GetFieldMugshotSpriteId()].data[0] = FALSE;
        RemoveFieldMugshot();
    }
    gSpeakerName = NULL;
}

u8 GetFieldMessageBoxMode(void)
{
    return sFieldMessageBoxMode;
}

bool8 IsFieldMessageBoxHidden(void)
{
    if (sFieldMessageBoxMode == FIELD_MESSAGE_BOX_HIDDEN)
        return TRUE;
    return FALSE;
}

void StopFieldMessage(void)
{
    DestroyTask_DrawFieldMessage();
    sFieldMessageBoxMode = FIELD_MESSAGE_BOX_HIDDEN;
}

void SetSpeakerName(const u8* name)
{
    gSpeakerName = name;
}