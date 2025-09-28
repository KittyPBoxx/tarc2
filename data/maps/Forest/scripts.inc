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
	goto_if 1, SoraNPC_2
# 30 "data/maps/Forest/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_BERUS
# 31 "data/maps/Forest/scripts.pory"
	msgbox SoraNPC_Text_2, MSGBOX_YESNO
# 33 "data/maps/Forest/scripts.pory"
	compare VAR_RESULT, 1
	goto_if_eq SoraNPC_5
# 37 "data/maps/Forest/scripts.pory"
	removefieldmugshot
SoraNPC_1:
# 42 "data/maps/Forest/scripts.pory"
	release
	end

SoraNPC_2:
# 19 "data/maps/Forest/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_BERUS
# 20 "data/maps/Forest/scripts.pory"
	msgbox SoraNPC_Text_0
# 21 "data/maps/Forest/scripts.pory"
	removefieldmugshot
# 22 "data/maps/Forest/scripts.pory"
	trainerbattle_single TRAINER_EXAMINER_SORA, Debug_EventScript_Script_1_Text_Battle1_Intro, Debug_EventScript_Script_1_Text_Battle1_Defeat
# 23 "data/maps/Forest/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_BERUS
# 24 "data/maps/Forest/scripts.pory"
	msgbox SoraNPC_Text_1
# 25 "data/maps/Forest/scripts.pory"
	removefieldmugshot
	goto SoraNPC_1

SoraNPC_5:
# 34 "data/maps/Forest/scripts.pory"
	removefieldmugshot
# 35 "data/maps/Forest/scripts.pory"
	trainerbattle_single TRAINER_EXAMINER_SORA, Debug_EventScript_Script_1_Text_Battle1_Intro, Debug_EventScript_Script_1_Text_Battle1_Defeat
	goto SoraNPC_1


FionnNPC::
# 47 "data/maps/Forest/scripts.pory"
	lock
# 49 "data/maps/Forest/scripts.pory"
	checktrainerflag TRAINER_EXAMINER_FIONN
	goto_if 1, FionnNPC_2
# 62 "data/maps/Forest/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_BERUS
# 63 "data/maps/Forest/scripts.pory"
	msgbox SoraNPC_Text_2, MSGBOX_YESNO
# 65 "data/maps/Forest/scripts.pory"
	compare VAR_RESULT, 1
	goto_if_eq FionnNPC_5
# 69 "data/maps/Forest/scripts.pory"
	removefieldmugshot
FionnNPC_1:
# 74 "data/maps/Forest/scripts.pory"
	release
	end

FionnNPC_2:
# 51 "data/maps/Forest/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_BERUS
# 52 "data/maps/Forest/scripts.pory"
	msgbox FionnNPC_Text_0
# 53 "data/maps/Forest/scripts.pory"
	removefieldmugshot
# 54 "data/maps/Forest/scripts.pory"
	trainerbattle_single TRAINER_EXAMINER_FIONN, Debug_EventScript_Script_1_Text_Battle1_Intro, Debug_EventScript_Script_1_Text_Battle1_Defeat
# 55 "data/maps/Forest/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_BERUS
# 56 "data/maps/Forest/scripts.pory"
	msgbox FionnNPC_Text_1
# 57 "data/maps/Forest/scripts.pory"
	removefieldmugshot
	goto FionnNPC_1

FionnNPC_5:
# 66 "data/maps/Forest/scripts.pory"
	removefieldmugshot
# 67 "data/maps/Forest/scripts.pory"
	trainerbattle_single TRAINER_EXAMINER_FIONN, Debug_EventScript_Script_1_Text_Battle1_Intro, Debug_EventScript_Script_1_Text_Battle1_Defeat
	goto FionnNPC_1


ForestSeal::
# 79 "data/maps/Forest/scripts.pory"
	lock
# 81 "data/maps/Forest/scripts.pory"
	checktrainerflag TRAINER_EXAMINER_FIONN
	goto_if 1, ForestSeal_8
ForestSeal_6:
# 84 "data/maps/Forest/scripts.pory"
	checktrainerflag TRAINER_EXAMINER_FIONN
	goto_if 1, ForestSeal_3
# 84 "data/maps/Forest/scripts.pory"
	checktrainerflag TRAINER_EXAMINER_SORA
	goto_if 1, ForestSeal_3
# 88 "data/maps/Forest/scripts.pory"
	msgbox ForestSeal_Text_2
ForestSeal_1:
# 91 "data/maps/Forest/scripts.pory"
	release
	end

ForestSeal_2:
# 82 "data/maps/Forest/scripts.pory"
	msgbox ForestSeal_Text_0
	goto ForestSeal_1

ForestSeal_3:
# 85 "data/maps/Forest/scripts.pory"
	msgbox ForestSeal_Text_1
	goto ForestSeal_1

ForestSeal_8:
# 81 "data/maps/Forest/scripts.pory"
	checktrainerflag TRAINER_EXAMINER_SORA
	goto_if 1, ForestSeal_2
	goto ForestSeal_6


SoraNPC_Text_0:
# 20 "data/maps/Forest/scripts.pory"
	.string "There are four examiners and one\n"
	.string "champion to beat. Our sponsoring gods\l"
	.string "pay us based on how many candidates we\l"
	.string "stop.$"

SoraNPC_Text_1:
# 24 "data/maps/Forest/scripts.pory"
	.string "How could I loose? I guess dinner will\n"
	.string "be uncooked ramen again...$"

SoraNPC_Text_2:
# 31 "data/maps/Forest/scripts.pory"
	.string "Did you want a rematch?$"

FionnNPC_Text_0:
# 52 "data/maps/Forest/scripts.pory"
	.string "Each area is locked with a seal. You\n"
	.string "can't progress untill you beat all the\l"
	.string "examiners first.$"

FionnNPC_Text_1:
# 56 "data/maps/Forest/scripts.pory"
	.string "Don't get cocky kid. Beating me only\n"
	.string "puts you one step closer to passing the\l"
	.string "exam.$"

ForestSeal_Text_0:
# 82 "data/maps/Forest/scripts.pory"
	.string "The seal has been broken. You may pass\n"
	.string "through.$"

ForestSeal_Text_1:
# 85 "data/maps/Forest/scripts.pory"
	.string "You must win one more battle to break\n"
	.string "this seal.$"

ForestSeal_Text_2:
# 88 "data/maps/Forest/scripts.pory"
	.string "You must have won two battles to break\n"
	.string "this seal.$"
