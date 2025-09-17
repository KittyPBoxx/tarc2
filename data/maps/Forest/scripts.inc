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
	createvobject OBJ_EVENT_GFX_TRUCK, 1, 20, 0
# 10 "data/maps/Forest/scripts.pory"
	createvobject OBJ_EVENT_GFX_TRUCK, 1, 15, 4
# 11 "data/maps/Forest/scripts.pory"
	return

SoraNPC::
# 15 "data/maps/Forest/scripts.pory"
	lock
# 16 "data/maps/Forest/scripts.pory"
	faceplayer
# 18 "data/maps/Forest/scripts.pory"
	checktrainerflag TRAINER_EXAMINER_SORA
	goto_if 1, SoraNPC_2
# 31 "data/maps/Forest/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_NORMAL
# 32 "data/maps/Forest/scripts.pory"
	msgbox SoraNPC_Text_2, MSGBOX_YESNO
# 34 "data/maps/Forest/scripts.pory"
	compare VAR_RESULT, 1
	goto_if_eq SoraNPC_5
# 38 "data/maps/Forest/scripts.pory"
	removefieldmugshot
SoraNPC_1:
# 43 "data/maps/Forest/scripts.pory"
	release
	end

SoraNPC_2:
# 20 "data/maps/Forest/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_NORMAL
# 21 "data/maps/Forest/scripts.pory"
	msgbox SoraNPC_Text_0
# 22 "data/maps/Forest/scripts.pory"
	removefieldmugshot
# 23 "data/maps/Forest/scripts.pory"
	trainerbattle_single TRAINER_EXAMINER_SORA, Debug_EventScript_Script_1_Text_Battle1_Intro, Debug_EventScript_Script_1_Text_Battle1_Defeat
# 24 "data/maps/Forest/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_NORMAL
# 25 "data/maps/Forest/scripts.pory"
	msgbox SoraNPC_Text_1
# 26 "data/maps/Forest/scripts.pory"
	removefieldmugshot
	goto SoraNPC_1

SoraNPC_5:
# 35 "data/maps/Forest/scripts.pory"
	removefieldmugshot
# 36 "data/maps/Forest/scripts.pory"
	trainerbattle_single TRAINER_EXAMINER_SORA, Debug_EventScript_Script_1_Text_Battle1_Intro, Debug_EventScript_Script_1_Text_Battle1_Defeat
	goto SoraNPC_1


FionnNPC::
# 48 "data/maps/Forest/scripts.pory"
	lock
# 49 "data/maps/Forest/scripts.pory"
	faceplayer
# 51 "data/maps/Forest/scripts.pory"
	checktrainerflag TRAINER_EXAMINER_FIONN
	goto_if 1, FionnNPC_2
# 64 "data/maps/Forest/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_NORMAL
# 65 "data/maps/Forest/scripts.pory"
	msgbox SoraNPC_Text_2, MSGBOX_YESNO
# 67 "data/maps/Forest/scripts.pory"
	compare VAR_RESULT, 1
	goto_if_eq FionnNPC_5
# 71 "data/maps/Forest/scripts.pory"
	removefieldmugshot
FionnNPC_1:
# 76 "data/maps/Forest/scripts.pory"
	release
	end

FionnNPC_2:
# 53 "data/maps/Forest/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_NORMAL
# 54 "data/maps/Forest/scripts.pory"
	msgbox FionnNPC_Text_0
# 55 "data/maps/Forest/scripts.pory"
	removefieldmugshot
# 56 "data/maps/Forest/scripts.pory"
	trainerbattle_single TRAINER_EXAMINER_FIONN, Debug_EventScript_Script_1_Text_Battle1_Intro, Debug_EventScript_Script_1_Text_Battle1_Defeat
# 57 "data/maps/Forest/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_NORMAL
# 58 "data/maps/Forest/scripts.pory"
	msgbox FionnNPC_Text_1
# 59 "data/maps/Forest/scripts.pory"
	removefieldmugshot
	goto FionnNPC_1

FionnNPC_5:
# 68 "data/maps/Forest/scripts.pory"
	removefieldmugshot
# 69 "data/maps/Forest/scripts.pory"
	trainerbattle_single TRAINER_EXAMINER_FIONN, Debug_EventScript_Script_1_Text_Battle1_Intro, Debug_EventScript_Script_1_Text_Battle1_Defeat
	goto FionnNPC_1


ForestSeal::
# 81 "data/maps/Forest/scripts.pory"
	lock
# 82 "data/maps/Forest/scripts.pory"
	faceplayer
# 84 "data/maps/Forest/scripts.pory"
	checktrainerflag TRAINER_EXAMINER_FIONN
	goto_if 1, ForestSeal_8
ForestSeal_6:
# 87 "data/maps/Forest/scripts.pory"
	checktrainerflag TRAINER_EXAMINER_FIONN
	goto_if 1, ForestSeal_3
# 87 "data/maps/Forest/scripts.pory"
	checktrainerflag TRAINER_EXAMINER_SORA
	goto_if 1, ForestSeal_3
# 91 "data/maps/Forest/scripts.pory"
	msgbox ForestSeal_Text_2
ForestSeal_1:
# 94 "data/maps/Forest/scripts.pory"
	release
	end

ForestSeal_2:
# 85 "data/maps/Forest/scripts.pory"
	msgbox ForestSeal_Text_0
	goto ForestSeal_1

ForestSeal_3:
# 88 "data/maps/Forest/scripts.pory"
	msgbox ForestSeal_Text_1
	goto ForestSeal_1

ForestSeal_8:
# 84 "data/maps/Forest/scripts.pory"
	checktrainerflag TRAINER_EXAMINER_SORA
	goto_if 1, ForestSeal_2
	goto ForestSeal_6


SoraNPC_Text_0:
# 21 "data/maps/Forest/scripts.pory"
	.string "There are four examiners and one\n"
	.string "champion to beat. Our sponsoring gods\l"
	.string "pay us based on how many candidates we\l"
	.string "stop.$"

SoraNPC_Text_1:
# 25 "data/maps/Forest/scripts.pory"
	.string "How could I loose? I guess dinner will\n"
	.string "be uncooked ramen again...$"

SoraNPC_Text_2:
# 32 "data/maps/Forest/scripts.pory"
	.string "Did you want a rematch?$"

FionnNPC_Text_0:
# 54 "data/maps/Forest/scripts.pory"
	.string "Each area is locked with a seal. You\n"
	.string "can't progress untill you beat all the\l"
	.string "examiners first.$"

FionnNPC_Text_1:
# 58 "data/maps/Forest/scripts.pory"
	.string "Don't get cocky kid. Beating me only\n"
	.string "puts you one step closer to passing the\l"
	.string "exam.$"

ForestSeal_Text_0:
# 85 "data/maps/Forest/scripts.pory"
	.string "The seal has been broken. You may pass\n"
	.string "through.$"

ForestSeal_Text_1:
# 88 "data/maps/Forest/scripts.pory"
	.string "You must win one more battle to break\n"
	.string "this seal.$"

ForestSeal_Text_2:
# 91 "data/maps/Forest/scripts.pory"
	.string "You must have won two battles to break\n"
	.string "this seal.$"
