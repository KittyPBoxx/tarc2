CaveTop_MapScripts::
	.byte 0


DanteNPC::
# 4 "data/maps/CaveTop/scripts.pory"
	lock
# 5 "data/maps/CaveTop/scripts.pory"
	faceplayer
# 7 "data/maps/CaveTop/scripts.pory"
	checktrainerflag TRAINER_EXAMINER_DANTE
	goto_if 1, DanteNPC_2
# 20 "data/maps/CaveTop/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_BERUS
# 21 "data/maps/CaveTop/scripts.pory"
	msgbox DanteNPC_Text_2, MSGBOX_YESNO
# 23 "data/maps/CaveTop/scripts.pory"
	compare VAR_RESULT, 1
	goto_if_eq DanteNPC_5
# 27 "data/maps/CaveTop/scripts.pory"
	removefieldmugshot
DanteNPC_1:
# 32 "data/maps/CaveTop/scripts.pory"
	release
	end

DanteNPC_2:
# 9 "data/maps/CaveTop/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_BERUS
# 10 "data/maps/CaveTop/scripts.pory"
	msgbox DanteNPC_Text_0
# 11 "data/maps/CaveTop/scripts.pory"
	removefieldmugshot
# 12 "data/maps/CaveTop/scripts.pory"
	trainerbattle_double TRAINER_EXAMINER_DANTE, Debug_EventScript_Script_1_Text_Battle1_Intro, Debug_EventScript_Script_1_Text_Battle1_Defeat, Debug_EventScript_Script_1_Text_Battle1_NotEnoughMons
# 13 "data/maps/CaveTop/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_BERUS
# 14 "data/maps/CaveTop/scripts.pory"
	msgbox DanteNPC_Text_1
# 15 "data/maps/CaveTop/scripts.pory"
	removefieldmugshot
	goto DanteNPC_1

DanteNPC_5:
# 24 "data/maps/CaveTop/scripts.pory"
	removefieldmugshot
# 25 "data/maps/CaveTop/scripts.pory"
	trainerbattle_double TRAINER_EXAMINER_DANTE, Debug_EventScript_Script_1_Text_Battle1_Intro, Debug_EventScript_Script_1_Text_Battle1_Defeat, Debug_EventScript_Script_1_Text_Battle1_NotEnoughMons
	goto DanteNPC_1


CaveSeal::
# 37 "data/maps/CaveTop/scripts.pory"
	lock
# 38 "data/maps/CaveTop/scripts.pory"
	faceplayer
# 40 "data/maps/CaveTop/scripts.pory"
	checktrainerflag TRAINER_EXAMINER_RUNE
	goto_if 1, CaveSeal_8
CaveSeal_6:
# 43 "data/maps/CaveTop/scripts.pory"
	checktrainerflag TRAINER_EXAMINER_RUNE
	goto_if 1, CaveSeal_3
# 43 "data/maps/CaveTop/scripts.pory"
	checktrainerflag TRAINER_EXAMINER_DANTE
	goto_if 1, CaveSeal_3
# 47 "data/maps/CaveTop/scripts.pory"
	msgbox CaveSeal_Text_2
CaveSeal_1:
# 50 "data/maps/CaveTop/scripts.pory"
	release
	end

CaveSeal_2:
# 41 "data/maps/CaveTop/scripts.pory"
	msgbox CaveSeal_Text_0
	goto CaveSeal_1

CaveSeal_3:
# 44 "data/maps/CaveTop/scripts.pory"
	msgbox CaveSeal_Text_1
	goto CaveSeal_1

CaveSeal_8:
# 40 "data/maps/CaveTop/scripts.pory"
	checktrainerflag TRAINER_EXAMINER_DANTE
	goto_if 1, CaveSeal_2
	goto CaveSeal_6


DanteNPC_Text_0:
# 10 "data/maps/CaveTop/scripts.pory"
	.string "You've made it far kid. I'm the last\n"
	.string "candidate you'll face before the\l"
	.string "champion.$"

DanteNPC_Text_1:
# 14 "data/maps/CaveTop/scripts.pory"
	.string "The champion awaits good luck.$"

DanteNPC_Text_2:
# 21 "data/maps/CaveTop/scripts.pory"
	.string "Did you want a rematch?$"

CaveSeal_Text_0:
# 41 "data/maps/CaveTop/scripts.pory"
	.string "The seal has been broken. You may pass\n"
	.string "through.$"

CaveSeal_Text_1:
# 44 "data/maps/CaveTop/scripts.pory"
	.string "You must win one more battle to break\n"
	.string "this seal.$"

CaveSeal_Text_2:
# 47 "data/maps/CaveTop/scripts.pory"
	.string "You must have won four battles to\n"
	.string "break this seal.$"
