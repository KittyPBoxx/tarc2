CaveBottom_MapScripts::
	.byte 0


RuneNPC::
# 4 "data/maps/CaveBottom/scripts.pory"
	lock
# 6 "data/maps/CaveBottom/scripts.pory"
	checktrainerflag TRAINER_EXAMINER_RUNE
	goto_if 0, RuneNPC_2
# 25 "data/maps/CaveBottom/scripts.pory"
	msgbox RuneNPC_Text_3, MSGBOX_YESNO
# 26 "data/maps/CaveBottom/scripts.pory"
	compare VAR_RESULT, 1
	goto_if_eq RuneNPC_7
RuneNPC_1:
# 32 "data/maps/CaveBottom/scripts.pory"
	release
	end

RuneNPC_2:
# 8 "data/maps/CaveBottom/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_EXAMINER
# 9 "data/maps/CaveBottom/scripts.pory"
	msgbox RuneNPC_Text_0
# 10 "data/maps/CaveBottom/scripts.pory"
	removefieldmugshot
# 11 "data/maps/CaveBottom/scripts.pory"
	msgbox RuneNPC_Text_1
# 12 "data/maps/CaveBottom/scripts.pory"
	trainerbattle_single TRAINER_EXAMINER_RUNE, Debug_EventScript_Script_1_Text_Battle1_Intro, Debug_EventScript_Script_1_Text_Battle1_Defeat
# 14 "data/maps/CaveBottom/scripts.pory"
	special GetBattleOutcome
# 15 "data/maps/CaveBottom/scripts.pory"
	compare VAR_RESULT, B_OUTCOME_WON
	goto_if_eq RuneNPC_5
	goto RuneNPC_1

RuneNPC_5:
# 17 "data/maps/CaveBottom/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_EXAMINER
# 18 "data/maps/CaveBottom/scripts.pory"
	msgbox RuneNPC_Text_2
# 19 "data/maps/CaveBottom/scripts.pory"
	removefieldmugshot
	goto RuneNPC_1

RuneNPC_7:
# 27 "data/maps/CaveBottom/scripts.pory"
	trainerbattle_single TRAINER_EXAMINER_RUNE, Debug_EventScript_Script_1_Text_Battle1_Intro, Debug_EventScript_Script_1_Text_Battle1_Defeat
	goto RuneNPC_1


DanteNPC::
# 37 "data/maps/CaveBottom/scripts.pory"
	lock
# 39 "data/maps/CaveBottom/scripts.pory"
	setvar VAR_0x8008, 4
# 40 "data/maps/CaveBottom/scripts.pory"
	special SetBattleWeather
# 42 "data/maps/CaveBottom/scripts.pory"
	checktrainerflag TRAINER_EXAMINER_DANTE
	goto_if 0, DanteNPC_2
# 62 "data/maps/CaveBottom/scripts.pory"
	msgbox RuneNPC_Text_3, MSGBOX_YESNO
# 63 "data/maps/CaveBottom/scripts.pory"
	compare VAR_RESULT, 1
	goto_if_eq DanteNPC_7
DanteNPC_1:
# 69 "data/maps/CaveBottom/scripts.pory"
	setvar VAR_0x8008, 0
# 70 "data/maps/CaveBottom/scripts.pory"
	special SetBattleWeather
# 72 "data/maps/CaveBottom/scripts.pory"
	release
	end

DanteNPC_2:
# 45 "data/maps/CaveBottom/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_EXAMINER
# 46 "data/maps/CaveBottom/scripts.pory"
	msgbox DanteNPC_Text_0
# 47 "data/maps/CaveBottom/scripts.pory"
	removefieldmugshot
# 48 "data/maps/CaveBottom/scripts.pory"
	msgbox DanteNPC_Text_1
# 49 "data/maps/CaveBottom/scripts.pory"
	trainerbattle_double TRAINER_EXAMINER_DANTE, Debug_EventScript_Script_1_Text_Battle1_Intro, Debug_EventScript_Script_1_Text_Battle1_Defeat, Debug_EventScript_Script_1_Text_Battle1_NotEnoughMons
# 51 "data/maps/CaveBottom/scripts.pory"
	special GetBattleOutcome
# 52 "data/maps/CaveBottom/scripts.pory"
	compare VAR_RESULT, B_OUTCOME_WON
	goto_if_eq DanteNPC_5
	goto DanteNPC_1

DanteNPC_5:
# 54 "data/maps/CaveBottom/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_EXAMINER
# 55 "data/maps/CaveBottom/scripts.pory"
	msgbox DanteNPC_Text_2
# 56 "data/maps/CaveBottom/scripts.pory"
	removefieldmugshot
	goto DanteNPC_1

DanteNPC_7:
# 64 "data/maps/CaveBottom/scripts.pory"
	trainerbattle_double TRAINER_EXAMINER_DANTE, Debug_EventScript_Script_1_Text_Battle1_Intro, Debug_EventScript_Script_1_Text_Battle1_Defeat, Debug_EventScript_Script_1_Text_Battle1_NotEnoughMons
	goto DanteNPC_1


CaveDoor::
# 77 "data/maps/CaveBottom/scripts.pory"
	lock
# 79 "data/maps/CaveBottom/scripts.pory"
	checktrainerflag TRAINER_EXAMINER_RUNE
	goto_if 1, CaveDoor_8
CaveDoor_6:
# 93 "data/maps/CaveBottom/scripts.pory"
	checktrainerflag TRAINER_EXAMINER_RUNE
	goto_if 1, CaveDoor_3
# 93 "data/maps/CaveBottom/scripts.pory"
	checktrainerflag TRAINER_EXAMINER_DANTE
	goto_if 1, CaveDoor_3
# 97 "data/maps/CaveBottom/scripts.pory"
	msgbox CaveDoor_Text_3
CaveDoor_1:
# 100 "data/maps/CaveBottom/scripts.pory"
	release
	end

CaveDoor_2:
# 82 "data/maps/CaveBottom/scripts.pory"
	goto_if_unset FLAG_UNLOCK_SUMMIT_WARP, CaveDoor_12
CaveDoor_11:
# 90 "data/maps/CaveBottom/scripts.pory"
	msgbox CaveDoor_Text_1
	goto CaveDoor_1

CaveDoor_3:
# 94 "data/maps/CaveBottom/scripts.pory"
	msgbox CaveDoor_Text_2
	goto CaveDoor_1

CaveDoor_8:
# 79 "data/maps/CaveBottom/scripts.pory"
	checktrainerflag TRAINER_EXAMINER_DANTE
	goto_if 1, CaveDoor_2
	goto CaveDoor_6

CaveDoor_12:
# 83 "data/maps/CaveBottom/scripts.pory"
	setflag FLAG_UNLOCK_SUMMIT_WARP
# 84 "data/maps/CaveBottom/scripts.pory"
	special AutoSave
# 85 "data/maps/CaveBottom/scripts.pory"
	playfanfare MUS_OBTAIN_BADGE
# 86 "data/maps/CaveBottom/scripts.pory"
	msgbox CaveDoor_Text_0
# 87 "data/maps/CaveBottom/scripts.pory"
	waitfanfare
	goto CaveDoor_11


RuneNPC_Text_0:
# 9 "data/maps/CaveBottom/scripts.pory"
	.string "You've made it far, kid. But you're not\n"
	.string "ready for what I can bring.$"

RuneNPC_Text_1:
# 11 "data/maps/CaveBottom/scripts.pory"
	.string "Rune used a prayer.\n"
	.string "Mega evo restrictions were removed.$"

RuneNPC_Text_2:
# 18 "data/maps/CaveBottom/scripts.pory"
	.string "Whoa, you might have what it takes to\n"
	.string "beat the champ.$"

RuneNPC_Text_3:
# 25 "data/maps/CaveBottom/scripts.pory"
	.string "Did you want a rematch?$"

DanteNPC_Text_0:
# 46 "data/maps/CaveBottom/scripts.pory"
	.string "You're not the only one with access to\n"
	.string "prayers.$"

DanteNPC_Text_1:
# 48 "data/maps/CaveBottom/scripts.pory"
	.string "Dante used a prayer.\n"
	.string "It began to hail.$"

DanteNPC_Text_2:
# 55 "data/maps/CaveBottom/scripts.pory"
	.string "I guess you're just that much\n"
	.string "stronger...$"

CaveDoor_Text_0:
# 86 "data/maps/CaveBottom/scripts.pory"
	.string "A new warp was unlocked!$"

CaveDoor_Text_1:
# 90 "data/maps/CaveBottom/scripts.pory"
	.string "Use {SELECT_BUTTON} to warp to the next area.$"

CaveDoor_Text_2:
# 94 "data/maps/CaveBottom/scripts.pory"
	.string "The crystal is not active.\n"
	.string "REQUIREMENT: ONE MORE TRAINER BATTLE$"

CaveDoor_Text_3:
# 97 "data/maps/CaveBottom/scripts.pory"
	.string "The crystal is not active.\n"
	.string "REQUIREMENT: TWO MORE TRAINER\l"
	.string "BATTLES$"
