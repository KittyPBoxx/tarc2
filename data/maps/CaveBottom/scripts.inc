CaveBottom_MapScripts::
	.byte 0


RuneNPC::
# 4 "data/maps/CaveBottom/scripts.pory"
	lock
# 6 "data/maps/CaveBottom/scripts.pory"
	checktrainerflag TRAINER_EXAMINER_RUNE
	goto_if 0, RuneNPC_2
# 24 "data/maps/CaveBottom/scripts.pory"
	msgbox RuneNPC_Text_2, MSGBOX_YESNO
# 25 "data/maps/CaveBottom/scripts.pory"
	compare VAR_RESULT, 1
	goto_if_eq RuneNPC_7
RuneNPC_1:
# 31 "data/maps/CaveBottom/scripts.pory"
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
	trainerbattle_single TRAINER_EXAMINER_RUNE, Debug_EventScript_Script_1_Text_Battle1_Intro, Debug_EventScript_Script_1_Text_Battle1_Defeat
# 13 "data/maps/CaveBottom/scripts.pory"
	special GetBattleOutcome
# 14 "data/maps/CaveBottom/scripts.pory"
	compare VAR_RESULT, B_OUTCOME_WON
	goto_if_eq RuneNPC_5
	goto RuneNPC_1

RuneNPC_5:
# 16 "data/maps/CaveBottom/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_EXAMINER
# 17 "data/maps/CaveBottom/scripts.pory"
	msgbox RuneNPC_Text_1
# 18 "data/maps/CaveBottom/scripts.pory"
	removefieldmugshot
	goto RuneNPC_1

RuneNPC_7:
# 26 "data/maps/CaveBottom/scripts.pory"
	trainerbattle_single TRAINER_EXAMINER_RUNE, Debug_EventScript_Script_1_Text_Battle1_Intro, Debug_EventScript_Script_1_Text_Battle1_Defeat
	goto RuneNPC_1


DanteNPC::
# 36 "data/maps/CaveBottom/scripts.pory"
	lock
# 38 "data/maps/CaveBottom/scripts.pory"
	setvar VAR_0x8008, 4
# 39 "data/maps/CaveBottom/scripts.pory"
	special SetBattleWeather
# 41 "data/maps/CaveBottom/scripts.pory"
	checktrainerflag TRAINER_EXAMINER_DANTE
	goto_if 0, DanteNPC_2
# 60 "data/maps/CaveBottom/scripts.pory"
	msgbox RuneNPC_Text_2, MSGBOX_YESNO
# 61 "data/maps/CaveBottom/scripts.pory"
	compare VAR_RESULT, 1
	goto_if_eq DanteNPC_7
DanteNPC_1:
# 67 "data/maps/CaveBottom/scripts.pory"
	setvar VAR_0x8008, 0
# 68 "data/maps/CaveBottom/scripts.pory"
	special SetBattleWeather
# 70 "data/maps/CaveBottom/scripts.pory"
	release
	end

DanteNPC_2:
# 44 "data/maps/CaveBottom/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_EXAMINER
# 45 "data/maps/CaveBottom/scripts.pory"
	msgbox DanteNPC_Text_0
# 46 "data/maps/CaveBottom/scripts.pory"
	removefieldmugshot
# 47 "data/maps/CaveBottom/scripts.pory"
	trainerbattle_double TRAINER_EXAMINER_DANTE, Debug_EventScript_Script_1_Text_Battle1_Intro, Debug_EventScript_Script_1_Text_Battle1_Defeat, Debug_EventScript_Script_1_Text_Battle1_NotEnoughMons
# 49 "data/maps/CaveBottom/scripts.pory"
	special GetBattleOutcome
# 50 "data/maps/CaveBottom/scripts.pory"
	compare VAR_RESULT, B_OUTCOME_WON
	goto_if_eq DanteNPC_5
	goto DanteNPC_1

DanteNPC_5:
# 52 "data/maps/CaveBottom/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_EXAMINER
# 53 "data/maps/CaveBottom/scripts.pory"
	msgbox DanteNPC_Text_1
# 54 "data/maps/CaveBottom/scripts.pory"
	removefieldmugshot
	goto DanteNPC_1

DanteNPC_7:
# 62 "data/maps/CaveBottom/scripts.pory"
	trainerbattle_double TRAINER_EXAMINER_DANTE, Debug_EventScript_Script_1_Text_Battle1_Intro, Debug_EventScript_Script_1_Text_Battle1_Defeat, Debug_EventScript_Script_1_Text_Battle1_NotEnoughMons
	goto DanteNPC_1


CaveDoor::
# 75 "data/maps/CaveBottom/scripts.pory"
	lock
# 77 "data/maps/CaveBottom/scripts.pory"
	checktrainerflag TRAINER_EXAMINER_RUNE
	goto_if 1, CaveDoor_8
CaveDoor_6:
# 88 "data/maps/CaveBottom/scripts.pory"
	checktrainerflag TRAINER_EXAMINER_RUNE
	goto_if 1, CaveDoor_3
# 88 "data/maps/CaveBottom/scripts.pory"
	checktrainerflag TRAINER_EXAMINER_DANTE
	goto_if 1, CaveDoor_3
# 92 "data/maps/CaveBottom/scripts.pory"
	msgbox CaveDoor_Text_3
CaveDoor_1:
# 95 "data/maps/CaveBottom/scripts.pory"
	release
	end

CaveDoor_2:
# 80 "data/maps/CaveBottom/scripts.pory"
	goto_if_unset FLAG_UNLOCK_SUMMIT_WARP, CaveDoor_12
CaveDoor_11:
# 85 "data/maps/CaveBottom/scripts.pory"
	msgbox CaveDoor_Text_1
	goto CaveDoor_1

CaveDoor_3:
# 89 "data/maps/CaveBottom/scripts.pory"
	msgbox CaveDoor_Text_2
	goto CaveDoor_1

CaveDoor_8:
# 77 "data/maps/CaveBottom/scripts.pory"
	checktrainerflag TRAINER_EXAMINER_DANTE
	goto_if 1, CaveDoor_2
	goto CaveDoor_6

CaveDoor_12:
# 81 "data/maps/CaveBottom/scripts.pory"
	msgbox CaveDoor_Text_0
# 82 "data/maps/CaveBottom/scripts.pory"
	setflag FLAG_UNLOCK_SUMMIT_WARP
	goto CaveDoor_11


RuneNPC_Text_0:
# 9 "data/maps/CaveBottom/scripts.pory"
	.string "A twerp like you made it half way? Too\n"
	.string "bad you're exam ends here.$"

RuneNPC_Text_1:
# 17 "data/maps/CaveBottom/scripts.pory"
	.string "Sorry I called you a twerp. I'm kinda\n"
	.string "insecure about this job.$"

RuneNPC_Text_2:
# 24 "data/maps/CaveBottom/scripts.pory"
	.string "Did you want a rematch?$"

DanteNPC_Text_0:
# 45 "data/maps/CaveBottom/scripts.pory"
	.string "You've made it far kid. But you're not\n"
	.string "ready for what I can bring.$"

DanteNPC_Text_1:
# 53 "data/maps/CaveBottom/scripts.pory"
	.string "Whoa, you might have what it takes to\n"
	.string "beat the champ.$"

CaveDoor_Text_0:
# 81 "data/maps/CaveBottom/scripts.pory"
	.string "A new warp was unlocked!$"

CaveDoor_Text_1:
# 85 "data/maps/CaveBottom/scripts.pory"
	.string "Use {SELECT_BUTTON} to warp to the next area.$"

CaveDoor_Text_2:
# 89 "data/maps/CaveBottom/scripts.pory"
	.string "The crystal is not active.\n"
	.string "REQUIREMENT: ONE MORE TRAINER BATTLE$"

CaveDoor_Text_3:
# 92 "data/maps/CaveBottom/scripts.pory"
	.string "The crystal is not active.\n"
	.string "REQUIREMENT: TWO MORE TRAINER\l"
	.string "BATTLES$"
