#ifndef GUARD_SAVE_H
#define GUARD_SAVE_H

// Each 4 KiB flash sector contains 3968 bytes of actual data followed by 116 bytes of SaveBlock3 and then 12 bytes of footer.
#define SECTOR_DATA_SIZE 3968
#define SAVE_BLOCK_3_CHUNK_SIZE 116
#define SECTOR_FOOTER_SIZE 12

#define SAVE_STATUS_EMPTY    0
#define SAVE_STATUS_OK       1
#define SAVE_STATUS_CORRUPT  2
#define SAVE_STATUS_ERROR    0xFF

#define SLOT_INVALID 0xFF

enum
{
    ENABLE,
    DISABLE,
};

// Do save types
enum
{
    SAVE_NORMAL,
    SAVE_OVERWRITE_DIFFERENT_FILE
};

#define FRAM_BASE   ((vu8*)0x0E000000)
#define FRAM_SIZE   0x4000  // 8kb (0x2000) is the smallest save chip that actually works. Most emus will create a 32kb file anyway for compatibility 
#define GLOBAL_HEADER_SIZE 0x02
#define GLOBAL_HEADER_ADDR  ((vu8*)FRAM_BASE)
#define SAVE_SLOT_SIZE   0x100

// Fixed field offsets, relative to slot base
#define SLOT_OFF_NAME            (SAVE_HEADER_SIZE + 0x00)
#define SLOT_SZ_NAME             8

#define SLOT_OFF_NICK            (SAVE_HEADER_SIZE + 0x08)
#define SLOT_SZ_NICK             12

#define SLOT_OFF_MOVE1           (SAVE_HEADER_SIZE + 0x14)
#define SLOT_OFF_MOVE2           (SAVE_HEADER_SIZE + 0x16)
#define SLOT_OFF_MOVE3           (SAVE_HEADER_SIZE + 0x18)
#define SLOT_OFF_MOVE4           (SAVE_HEADER_SIZE + 0x1A)
#define SLOT_OFF_ITEM            (SAVE_HEADER_SIZE + 0x1C)
#define SLOT_OFF_ABILITY         (SAVE_HEADER_SIZE + 0x1E)
#define SLOT_OFF_SPECIES         (SAVE_HEADER_SIZE + 0x20)

#define SLOT_OFF_MAPNUM          (SAVE_HEADER_SIZE + 0x22)
#define SLOT_OFF_POSX            (SAVE_HEADER_SIZE + 0x23)
#define SLOT_OFF_POSY            (SAVE_HEADER_SIZE + 0x25)

#define SLOT_OFF_PLAY_H          (SAVE_HEADER_SIZE + 0x27)
#define SLOT_OFF_PLAY_M          (SAVE_HEADER_SIZE + 0x29)
#define SLOT_OFF_PLAY_S          (SAVE_HEADER_SIZE + 0x2A)
#define SLOT_OFF_PLAY_VB         (SAVE_HEADER_SIZE + 0x2B)

#define SLOT_OFF_COMPLETION      (SAVE_HEADER_SIZE + 0x2C)

#define SLOT_OFF_FLAGS           (SAVE_HEADER_SIZE + 0x2D)

#define SAVE_SLOT_ADDR(slot)  (GLOBAL_HEADER_SIZE + FRAM_BASE + (SAVE_SLOT_SIZE * (slot)))

enum 
{
    SAVE_AUTO_1,
    SAVE_AUTO_2,
    SAVE_MANUAL_1,
    SAVE_MANUAL_2,
    SAVE_MANUAL_3,
    SAVE_MANUAL_4,
    SAVE_SLOT_COUNT
};

#define LAST_SAVED_SLOT 0xFF

typedef struct {
    u8 lastAutosaveSlot;
    u8 lastSaveSlot;
    // --- new section for options ---
    u8  optionsButtonMode;  // OPTIONS_BUTTON_MODE_[NORMAL/LR/L_EQUALS_A]
    u16 optionsPacked;      // bitfield storage:
                            //   bits 0–2 = text speed
                            //   bits 3–7 = window frame type
                            //   bit 8    = sound
                            //   bit 9    = battle scene off
                            // (upper bits unused)
} GlobalSaveHeader;

typedef struct {
    u16 magic;        // marker (e.g. 0x7A2C)
    u16 checksum;
    u16 slotId;       // which logical slot this is (manual #1, autosave #2, etc.)
} SaveSlotHeader;

#define SAVE_HEADER_SIZE  sizeof(SaveSlotHeader)

extern u16 gSaveFileStatus;
extern void (*gGameContinueCallback)(void);

void ClearSaveData(void);
void ClearSaveSlot(u8 slotId);

u8 TrySavingData(u8 saveType, u8 slot);
u8 LoadGameSave(u8 saveType, u8 slotId);
bool8 SlotIsValid(u8 slotId);
void CopyPreviewDataToBuffer(u8 slot, u8 textId, u8 *dest);
void CheckAnySlotValid();
u8 GetCompletion(void);
void AutoSave(void);

u16 GetSaveBlocksPointersBaseOffset(void); // Always 0

// save_failed_screen.c
void DoSaveFailedScreen(u8 saveType);

#endif // GUARD_SAVE_H
