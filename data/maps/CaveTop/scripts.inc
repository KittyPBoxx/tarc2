CaveTop_MapScripts::
	.byte 0


DanteNPC::
# 4 "data/maps/CaveTop/scripts.pory"
	lock
# 6 "data/maps/CaveTop/scripts.pory"
	checktrainerflag TRAINER_EXAMINER_DANTE
	goto_if 1, DanteNPC_2
# 19 "data/maps/CaveTop/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_BERUS
# 20 "data/maps/CaveTop/scripts.pory"
	msgbox DanteNPC_Text_2, MSGBOX_YESNO
# 22 "data/maps/CaveTop/scripts.pory"
	compare VAR_RESULT, 1
	goto_if_eq DanteNPC_5
# 26 "data/maps/CaveTop/scripts.pory"
	removefieldmugshot
DanteNPC_1:
# 31 "data/maps/CaveTop/scripts.pory"
	release
	end

DanteNPC_2:
# 8 "data/maps/CaveTop/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_BERUS
# 9 "data/maps/CaveTop/scripts.pory"
	msgbox DanteNPC_Text_0
# 10 "data/maps/CaveTop/scripts.pory"
	removefieldmugshot
# 11 "data/maps/CaveTop/scripts.pory"
	trainerbattle_double TRAINER_EXAMINER_DANTE, Debug_EventScript_Script_1_Text_Battle1_Intro, Debug_EventScript_Script_1_Text_Battle1_Defeat, Debug_EventScript_Script_1_Text_Battle1_NotEnoughMons
# 12 "data/maps/CaveTop/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_BERUS
# 13 "data/maps/CaveTop/scripts.pory"
	msgbox DanteNPC_Text_1
# 14 "data/maps/CaveTop/scripts.pory"
	removefieldmugshot
	goto DanteNPC_1

DanteNPC_5:
# 23 "data/maps/CaveTop/scripts.pory"
	removefieldmugshot
# 24 "data/maps/CaveTop/scripts.pory"
	trainerbattle_double TRAINER_EXAMINER_DANTE, Debug_EventScript_Script_1_Text_Battle1_Intro, Debug_EventScript_Script_1_Text_Battle1_Defeat, Debug_EventScript_Script_1_Text_Battle1_NotEnoughMons
	goto DanteNPC_1


CaveSeal::
# 36 "data/maps/CaveTop/scripts.pory"
	lock
# 38 "data/maps/CaveTop/scripts.pory"
	checktrainerflag TRAINER_EXAMINER_RUNE
	goto_if 1, CaveSeal_8
CaveSeal_6:
# 41 "data/maps/CaveTop/scripts.pory"
	checktrainerflag TRAINER_EXAMINER_RUNE
	goto_if 1, CaveSeal_3
# 41 "data/maps/CaveTop/scripts.pory"
	checktrainerflag TRAINER_EXAMINER_DANTE
	goto_if 1, CaveSeal_3
# 45 "data/maps/CaveTop/scripts.pory"
	msgbox CaveSeal_Text_2
CaveSeal_1:
# 48 "data/maps/CaveTop/scripts.pory"
	release
	end

CaveSeal_2:
# 39 "data/maps/CaveTop/scripts.pory"
	msgbox CaveSeal_Text_0
	goto CaveSeal_1

CaveSeal_3:
# 42 "data/maps/CaveTop/scripts.pory"
	msgbox CaveSeal_Text_1
	goto CaveSeal_1

CaveSeal_8:
# 38 "data/maps/CaveTop/scripts.pory"
	checktrainerflag TRAINER_EXAMINER_DANTE
	goto_if 1, CaveSeal_2
	goto CaveSeal_6


DanteNPC_Text_0:
# 9 "data/maps/CaveTop/scripts.pory"
	.string "You've made it far kid. I'm the last\n"
	.string "candidate you'll face before the\l"
	.string "champion.$"

DanteNPC_Text_1:
# 13 "data/maps/CaveTop/scripts.pory"
	.string "The champion awaits good luck.$"

DanteNPC_Text_2:
# 20 "data/maps/CaveTop/scripts.pory"
	.string "Did you want a rematch?$"

CaveSeal_Text_0:
# 39 "data/maps/CaveTop/scripts.pory"
	.string "The seal has been broken. You may pass\n"
	.string "through.$"

CaveSeal_Text_1:
# 42 "data/maps/CaveTop/scripts.pory"
	.string "You must win one more battle to break\n"
	.string "this seal.$"

CaveSeal_Text_2:
# 45 "data/maps/CaveTop/scripts.pory"
	.string "You must have won four battles to\n"
	.string "break this seal.$"
