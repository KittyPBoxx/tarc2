#include "global.h"
#include "battle.h"
#include "battle_ai_main.h"
#include "battle_anim.h"
#include "battle_controllers.h"
#include "battle_message.h"
#include "battle_interface.h"
#include "bg.h"
#include "data.h"
#include "item_menu.h"
#include "item_use.h"
#include "main.h"
#include "m4a.h"
#include "palette.h"
#include "party_menu.h"
#include "pokeball.h"
#include "pokemon.h"
#include "reshow_battle_screen.h"
#include "sound.h"
#include "string_util.h"
#include "task.h"
#include "test_runner.h"
#include "text.h"
#include "util.h"
#include "window.h"
#include "constants/battle_anim.h"
#include "constants/songs.h"
#include "constants/trainers.h"

static void RecordedPlayerHandleLoadMonSprite(u32 battler);
static void RecordedPlayerHandleSwitchInAnim(u32 battler);
static void RecordedPlayerHandleDrawTrainerPic(u32 battler);
static void RecordedPlayerHandleTrainerSlideBack(u32 battler);
static void RecordedPlayerHandleMoveAnimation(u32 battler);
static void RecordedPlayerHandlePrintString(u32 battler);
static void RecordedPlayerHandleChooseAction(u32 battler);
static void RecordedPlayerHandleChooseMove(u32 battler);
static void RecordedPlayerHandleChooseItem(u32 battler);
static void RecordedPlayerHandleChoosePokemon(u32 battler);
static void RecordedPlayerHandleHealthBarUpdate(u32 battler);
static void RecordedPlayerHandleStatusIconUpdate(u32 battler);
static void RecordedPlayerHandleStatusAnimation(u32 battler);
static void RecordedPlayerHandleIntroTrainerBallThrow(u32 battler);
static void RecordedPlayerHandleDrawPartyStatusSummary(u32 battler);
static void RecordedPlayerHandleBattleAnimation(u32 battler);
static void RecordedPlayerHandleEndLinkBattle(u32 battler);

static void RecordedPlayerBufferRunCommand(u32 battler);
static void RecordedPlayerBufferExecCompleted(u32 battler);
static void SwitchIn_WaitAndEnd(u32 battler);

static void (*const sRecordedPlayerBufferCommands[CONTROLLER_CMDS_COUNT])(u32 battler) =
{
    [CONTROLLER_GETMONDATA]               = BtlController_HandleGetMonData,
    [CONTROLLER_GETRAWMONDATA]            = BtlController_Empty,
    [CONTROLLER_SETMONDATA]               = BtlController_HandleSetMonData,
    [CONTROLLER_SETRAWMONDATA]            = BtlController_HandleSetRawMonData,
    [CONTROLLER_LOADMONSPRITE]            = RecordedPlayerHandleLoadMonSprite,
    [CONTROLLER_SWITCHINANIM]             = RecordedPlayerHandleSwitchInAnim,
    [CONTROLLER_RETURNMONTOBALL]          = BtlController_HandleReturnMonToBall,
    [CONTROLLER_DRAWTRAINERPIC]           = RecordedPlayerHandleDrawTrainerPic,
    [CONTROLLER_TRAINERSLIDE]             = BtlController_Empty,
    [CONTROLLER_TRAINERSLIDEBACK]         = RecordedPlayerHandleTrainerSlideBack,
    [CONTROLLER_FAINTANIMATION]           = BtlController_HandleFaintAnimation,
    [CONTROLLER_PALETTEFADE]              = BtlController_Empty,
    [CONTROLLER_SUCCESSBALLTHROWANIM]     = BtlController_Empty,
    [CONTROLLER_BALLTHROWANIM]            = PlayerHandleBallThrowAnim,
    [CONTROLLER_PAUSE]                    = BtlController_Empty,
    [CONTROLLER_MOVEANIMATION]            = RecordedPlayerHandleMoveAnimation,
    [CONTROLLER_PRINTSTRING]              = RecordedPlayerHandlePrintString,
    [CONTROLLER_PRINTSTRINGPLAYERONLY]    = BtlController_Empty,
    [CONTROLLER_CHOOSEACTION]             = RecordedPlayerHandleChooseAction,
    [CONTROLLER_YESNOBOX]                 = BtlController_Empty,
    [CONTROLLER_CHOOSEMOVE]               = RecordedPlayerHandleChooseMove,
    [CONTROLLER_OPENBAG]                  = RecordedPlayerHandleChooseItem,
    [CONTROLLER_CHOOSEPOKEMON]            = RecordedPlayerHandleChoosePokemon,
    [CONTROLLER_23]                       = BtlController_Empty,
    [CONTROLLER_HEALTHBARUPDATE]          = RecordedPlayerHandleHealthBarUpdate,
    [CONTROLLER_EXPUPDATE]                = PlayerHandleExpUpdate,
    [CONTROLLER_STATUSICONUPDATE]         = RecordedPlayerHandleStatusIconUpdate,
    [CONTROLLER_STATUSANIMATION]          = RecordedPlayerHandleStatusAnimation,
    [CONTROLLER_STATUSXOR]                = BtlController_Empty,
    [CONTROLLER_DATATRANSFER]             = BtlController_Empty,
    [CONTROLLER_DMA3TRANSFER]             = BtlController_Empty,
    [CONTROLLER_PLAYBGM]                  = BtlController_Empty,
    [CONTROLLER_32]                       = BtlController_Empty,
    [CONTROLLER_TWORETURNVALUES]          = BtlController_Empty,
    [CONTROLLER_CHOSENMONRETURNVALUE]     = BtlController_Empty,
    [CONTROLLER_ONERETURNVALUE]           = BtlController_Empty,
    [CONTROLLER_ONERETURNVALUE_DUPLICATE] = BtlController_Empty,
    [CONTROLLER_HITANIMATION]             = BtlController_HandleHitAnimation,
    [CONTROLLER_CANTSWITCH]               = BtlController_Empty,
    [CONTROLLER_PLAYSE]                   = BtlController_HandlePlaySE,
    [CONTROLLER_PLAYFANFAREORBGM]         = BtlController_HandlePlayFanfareOrBGM,
    [CONTROLLER_FAINTINGCRY]              = BtlController_HandleFaintingCry,
    [CONTROLLER_INTROSLIDE]               = BtlController_HandleIntroSlide,
    [CONTROLLER_INTROTRAINERBALLTHROW]    = RecordedPlayerHandleIntroTrainerBallThrow,
    [CONTROLLER_DRAWPARTYSTATUSSUMMARY]   = RecordedPlayerHandleDrawPartyStatusSummary,
    [CONTROLLER_HIDEPARTYSTATUSSUMMARY]   = BtlController_HandleHidePartyStatusSummary,
    [CONTROLLER_ENDBOUNCE]                = BtlController_Empty,
    [CONTROLLER_SPRITEINVISIBILITY]       = BtlController_HandleSpriteInvisibility,
    [CONTROLLER_BATTLEANIMATION]          = RecordedPlayerHandleBattleAnimation,
    [CONTROLLER_LINKSTANDBYMSG]           = BtlController_Empty,
    [CONTROLLER_RESETACTIONMOVESELECTION] = BtlController_Empty,
    [CONTROLLER_ENDLINKBATTLE]            = RecordedPlayerHandleEndLinkBattle,
    [CONTROLLER_DEBUGMENU]                = BtlController_Empty,
    [CONTROLLER_TERMINATOR_NOP]           = BtlController_TerminatorNop
};

void SetControllerToRecordedPlayer(u32 battler)
{
    gBattlerControllerEndFuncs[battler] = RecordedPlayerBufferExecCompleted;
    gBattlerControllerFuncs[battler] = RecordedPlayerBufferRunCommand;
}

static void RecordedPlayerBufferRunCommand(u32 battler)
{
    if (gBattleControllerExecFlags & (1u << battler))
    {
        if (gBattleResources->bufferA[battler][0] < ARRAY_COUNT(sRecordedPlayerBufferCommands))
            sRecordedPlayerBufferCommands[gBattleResources->bufferA[battler][0]](battler);
        else
            RecordedPlayerBufferExecCompleted(battler);
    }
}

static void WaitForMonAnimAfterLoad(u32 battler)
{
    if (gSprites[gBattlerSpriteIds[battler]].animEnded && gSprites[gBattlerSpriteIds[battler]].x2 == 0)
        RecordedPlayerBufferExecCompleted(battler);
}

static void SwitchIn_ShowSubstitute(u32 battler)
{
    if (SwitchIn_ShowSubstituteUtil(battler))
        gBattlerControllerFuncs[battler] = SwitchIn_WaitAndEnd;
}

static void SwitchIn_WaitAndEnd(u32 battler)
{
    if (SwitchIn_WaitAndEndUtil(battler))
        RecordedPlayerBufferExecCompleted(battler);
}

static void SwitchIn_ShowHealthbox(u32 battler)
{
    if (SwitchIn_ShowHealthboxUtil(battler))
        gBattlerControllerFuncs[battler] = SwitchIn_ShowSubstitute;
}

static void SwitchIn_TryShinyAnim(u32 battler)
{
    if (SwitchIn_TryShinyAnimUtil(battler))
        gBattlerControllerFuncs[battler] = SwitchIn_ShowHealthbox;
}

static void RecordedPlayerBufferExecCompleted(u32 battler)
{
    gBattlerControllerFuncs[battler] = RecordedPlayerBufferRunCommand;
    gBattleControllerExecFlags &= ~(1u << battler);
}

static void RecordedPlayerHandleLoadMonSprite(u32 battler)
{
    BtlController_HandleLoadMonSprite(battler, WaitForMonAnimAfterLoad);
}

static void RecordedPlayerHandleSwitchInAnim(u32 battler)
{
    BtlController_HandleSwitchInAnim(battler, TRUE, SwitchIn_TryShinyAnim);
}

static void RecordedPlayerHandleDrawTrainerPic(u32 battler)
{
    // TODO: remove
}

static void RecordedPlayerHandleTrainerSlideBack(u32 battler)
{
    BtlController_HandleTrainerSlideBack(battler, 35, FALSE);
}

static void RecordedPlayerHandleMoveAnimation(u32 battler)
{
    BtlController_HandleMoveAnimation(battler, FALSE);
}

static void RecordedPlayerHandlePrintString(u32 battler)
{
    BtlController_HandlePrintString(battler, FALSE, FALSE);
}

static void ChooseActionInBattlePalace(u32 battler)
{
    if (gBattleCommunication[4] >= gBattlersCount / 2)
    {
        RecordedPlayerBufferExecCompleted(battler);
    }
}

static void RecordedPlayerHandleChooseAction(u32 battler)
{
    if (gBattleTypeFlags & BATTLE_TYPE_PALACE)
    {
        gBattlerControllerFuncs[battler] = ChooseActionInBattlePalace;
    }
    else
    {
        RecordedPlayerBufferExecCompleted(battler);
    }
}

static void RecordedPlayerHandleChooseMove(u32 battler)
{
    RecordedPlayerBufferExecCompleted(battler);
}

static void RecordedPlayerHandleChooseItem(u32 battler)
{
    RecordedPlayerBufferExecCompleted(battler);
}

static void RecordedPlayerHandleChoosePokemon(u32 battler)
{
    RecordedPlayerBufferExecCompleted(battler);
}

static void RecordedPlayerHandleHealthBarUpdate(u32 battler)
{
    BtlController_HandleHealthBarUpdate(battler, TRUE);
}

static void RecordedPlayerHandleStatusIconUpdate(u32 battler)
{
    if (!IsBattleSEPlaying(battler))
    {
        DoStatusIconUpdate(battler);
        if (gTestRunnerEnabled)
            TestRunner_Battle_RecordStatus1(battler, GetMonData(GetBattlerMon(battler), MON_DATA_STATUS));
    }
}

static void RecordedPlayerHandleStatusAnimation(u32 battler)
{
    BtlController_HandleStatusAnimation(battler);
}

static void RecordedPlayerHandleIntroTrainerBallThrow(u32 battler)
{
    // TODO: remove
}

static void RecordedPlayerHandleDrawPartyStatusSummary(u32 battler)
{
    BtlController_HandleDrawPartyStatusSummary(battler, B_SIDE_PLAYER, TRUE);
}

static void RecordedPlayerHandleBattleAnimation(u32 battler)
{
    BtlController_HandleBattleAnimation(battler, FALSE, FALSE);
}

static void RecordedPlayerHandleEndLinkBattle(u32 battler)
{
    gBattleOutcome = gBattleResources->bufferA[battler][1];
    FadeOutMapMusic(5);
    BeginFastPaletteFade(3);
    RecordedPlayerBufferExecCompleted(battler);
    gBattlerControllerFuncs[battler] = SetBattleEndCallbacks;
}
