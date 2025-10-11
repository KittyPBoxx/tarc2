# 1 "data/maps/Forest/scripts.pory"

# 2 "data/maps/Forest/scripts.pory"
Forest_MapScripts::
# 3 "data/maps/Forest/scripts.pory"
	map_script MAP_SCRIPT_ON_WARP_INTO_MAP_TABLE, Forest_MapScripts_CreateTiles
# 4 "data/maps/Forest/scripts.pory"
	map_script MAP_SCRIPT_ON_RESUME, Forest_MapScripts_CreateTiles
# 5 "data/maps/Forest/scripts.pory"
	.byte 0
# 6 "data/maps/Forest/scripts.pory"

# 7 "data/maps/Forest/scripts.pory"

# 8 "data/maps/Forest/scripts.pory"
Forest_MapScripts_CreateTiles::
# 9 "data/maps/Forest/scripts.pory"
	@ createvobject OBJ_EVENT_GFX_TRUCK, 1, 20, 0
# 10 "data/maps/Forest/scripts.pory"
	@ createvobject OBJ_EVENT_GFX_TRUCK, 1, 15, 4
# 11 "data/maps/Forest/scripts.pory"
	return

SoraNPC::
# 15 "data/maps/Forest/scripts.pory"
	lock
# 17 "data/maps/Forest/scripts.pory"
	checktrainerflag TRAINER_EXAMINER_SORA
	goto_if 0, SoraNPC_2
# 34 "data/maps/Forest/scripts.pory"
	msgbox SoraNPC_Text_2, MSGBOX_YESNO
# 36 "data/maps/Forest/scripts.pory"
	compare VAR_RESULT, 1
	goto_if_eq SoraNPC_7
SoraNPC_1:
# 42 "data/maps/Forest/scripts.pory"
	release
	end

SoraNPC_2:
# 19 "data/maps/Forest/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_EXAMINER
# 20 "data/maps/Forest/scripts.pory"
	msgbox SoraNPC_Text_0
# 21 "data/maps/Forest/scripts.pory"
	removefieldmugshot
# 22 "data/maps/Forest/scripts.pory"
	trainerbattle_single TRAINER_EXAMINER_SORA, Debug_EventScript_Script_1_Text_Battle1_Intro, Debug_EventScript_Script_1_Text_Battle1_Defeat
# 24 "data/maps/Forest/scripts.pory"
	special GetBattleOutcome
# 25 "data/maps/Forest/scripts.pory"
	compare VAR_RESULT, B_OUTCOME_WON
	goto_if_eq SoraNPC_5
	goto SoraNPC_1

SoraNPC_5:
# 27 "data/maps/Forest/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_EXAMINER
# 28 "data/maps/Forest/scripts.pory"
	msgbox SoraNPC_Text_1
# 29 "data/maps/Forest/scripts.pory"
	removefieldmugshot
	goto SoraNPC_1

SoraNPC_7:
# 37 "data/maps/Forest/scripts.pory"
	trainerbattle_single TRAINER_EXAMINER_SORA, Debug_EventScript_Script_1_Text_Battle1_Intro, Debug_EventScript_Script_1_Text_Battle1_Defeat
	goto SoraNPC_1


FionnNPC::
# 47 "data/maps/Forest/scripts.pory"
	lock
# 49 "data/maps/Forest/scripts.pory"
	checktrainerflag TRAINER_EXAMINER_FIONN
	goto_if 0, FionnNPC_2
# 66 "data/maps/Forest/scripts.pory"
	msgbox SoraNPC_Text_2, MSGBOX_YESNO
# 67 "data/maps/Forest/scripts.pory"
	compare VAR_RESULT, 1
	goto_if_eq FionnNPC_7
FionnNPC_1:
# 73 "data/maps/Forest/scripts.pory"
	release
	end

FionnNPC_2:
# 51 "data/maps/Forest/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_EXAMINER
# 52 "data/maps/Forest/scripts.pory"
	msgbox FionnNPC_Text_0
# 53 "data/maps/Forest/scripts.pory"
	removefieldmugshot
# 54 "data/maps/Forest/scripts.pory"
	trainerbattle_single TRAINER_EXAMINER_FIONN, Debug_EventScript_Script_1_Text_Battle1_Intro, Debug_EventScript_Script_1_Text_Battle1_Defeat
# 56 "data/maps/Forest/scripts.pory"
	special GetBattleOutcome
# 57 "data/maps/Forest/scripts.pory"
	compare VAR_RESULT, B_OUTCOME_WON
	goto_if_eq FionnNPC_5
	goto FionnNPC_1

FionnNPC_5:
# 59 "data/maps/Forest/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_EXAMINER
# 60 "data/maps/Forest/scripts.pory"
	msgbox FionnNPC_Text_1
# 61 "data/maps/Forest/scripts.pory"
	removefieldmugshot
	goto FionnNPC_1

FionnNPC_7:
# 68 "data/maps/Forest/scripts.pory"
	trainerbattle_single TRAINER_EXAMINER_FIONN, Debug_EventScript_Script_1_Text_Battle1_Intro, Debug_EventScript_Script_1_Text_Battle1_Defeat
	goto FionnNPC_1


ForestSeal::
# 78 "data/maps/Forest/scripts.pory"
	lock
# 80 "data/maps/Forest/scripts.pory"
	checktrainerflag TRAINER_EXAMINER_FIONN
	goto_if 1, ForestSeal_8
ForestSeal_6:
# 93 "data/maps/Forest/scripts.pory"
	checktrainerflag TRAINER_EXAMINER_FIONN
	goto_if 1, ForestSeal_3
# 93 "data/maps/Forest/scripts.pory"
	checktrainerflag TRAINER_EXAMINER_SORA
	goto_if 1, ForestSeal_3
# 97 "data/maps/Forest/scripts.pory"
	msgbox ForestSeal_Text_3
ForestSeal_1:
# 100 "data/maps/Forest/scripts.pory"
	release
	end

ForestSeal_2:
# 82 "data/maps/Forest/scripts.pory"
	goto_if_unset FLAG_UNLOCK_CAVE_WARP, ForestSeal_12
ForestSeal_11:
# 90 "data/maps/Forest/scripts.pory"
	msgbox ForestSeal_Text_1
	goto ForestSeal_1

ForestSeal_3:
# 94 "data/maps/Forest/scripts.pory"
	msgbox ForestSeal_Text_2
	goto ForestSeal_1

ForestSeal_8:
# 80 "data/maps/Forest/scripts.pory"
	checktrainerflag TRAINER_EXAMINER_SORA
	goto_if 1, ForestSeal_2
	goto ForestSeal_6

ForestSeal_12:
# 83 "data/maps/Forest/scripts.pory"
	setflag FLAG_UNLOCK_CAVE_WARP
# 84 "data/maps/Forest/scripts.pory"
	special AutoSave
# 85 "data/maps/Forest/scripts.pory"
	playfanfare MUS_OBTAIN_BADGE
# 86 "data/maps/Forest/scripts.pory"
	msgbox ForestSeal_Text_0
# 87 "data/maps/Forest/scripts.pory"
	waitfanfare
	goto ForestSeal_11


SoraNPC_Text_0:
# 20 "data/maps/Forest/scripts.pory"
	.string "There are four examiners and one\n"
	.string "champion to beat.\p"
	.string "Our sponsoring gods pay us based on\n"
	.string "how many candidates we stop.$"

SoraNPC_Text_1:
# 28 "data/maps/Forest/scripts.pory"
	.string "How could I lose? I guess dinner will be\n"
	.string "uncooked ramen again...$"

SoraNPC_Text_2:
# 34 "data/maps/Forest/scripts.pory"
	.string "Did you want a rematch?$"

FionnNPC_Text_0:
# 52 "data/maps/Forest/scripts.pory"
	.string "Each area is locked until you defeat us\n"
	.string "examiners.$"

FionnNPC_Text_1:
# 60 "data/maps/Forest/scripts.pory"
	.string "Don't get cocky, kid. Beating me only\n"
	.string "puts you one step closer to passing the\l"
	.string "exam.$"

ForestSeal_Text_0:
# 86 "data/maps/Forest/scripts.pory"
	.string "A new warp was unlocked!$"

ForestSeal_Text_1:
# 90 "data/maps/Forest/scripts.pory"
	.string "Use {SELECT_BUTTON} to warp to the next area.$"

ForestSeal_Text_2:
# 94 "data/maps/Forest/scripts.pory"
	.string "The crystal is not active.\n"
	.string "REQUIREMENT: ONE MORE TRAINER BATTLE$"

ForestSeal_Text_3:
# 97 "data/maps/Forest/scripts.pory"
	.string "The crystal is not active.\n"
	.string "REQUIREMENT: TWO TRAINER BATTLES$"
