CaveBottom_MapScripts::
	.byte 0


RuneNPC::
# 4 "data/maps/CaveBottom/scripts.pory"
	lock
# 5 "data/maps/CaveBottom/scripts.pory"
	faceplayer
# 7 "data/maps/CaveBottom/scripts.pory"
	checktrainerflag TRAINER_EXAMINER_RUNE
	goto_if 1, RuneNPC_2
# 20 "data/maps/CaveBottom/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_NORMAL
# 21 "data/maps/CaveBottom/scripts.pory"
	msgbox RuneNPC_Text_2, MSGBOX_YESNO
# 23 "data/maps/CaveBottom/scripts.pory"
	compare VAR_RESULT, 1
	goto_if_eq RuneNPC_5
# 27 "data/maps/CaveBottom/scripts.pory"
	removefieldmugshot
RuneNPC_1:
# 32 "data/maps/CaveBottom/scripts.pory"
	release
	end

RuneNPC_2:
# 9 "data/maps/CaveBottom/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_NORMAL
# 10 "data/maps/CaveBottom/scripts.pory"
	msgbox RuneNPC_Text_0
# 11 "data/maps/CaveBottom/scripts.pory"
	removefieldmugshot
# 12 "data/maps/CaveBottom/scripts.pory"
	trainerbattle_single TRAINER_EXAMINER_RUNE, Debug_EventScript_Script_1_Text_Battle1_Intro, Debug_EventScript_Script_1_Text_Battle1_Defeat
# 13 "data/maps/CaveBottom/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_NORMAL
# 14 "data/maps/CaveBottom/scripts.pory"
	msgbox RuneNPC_Text_1
# 15 "data/maps/CaveBottom/scripts.pory"
	removefieldmugshot
	goto RuneNPC_1

RuneNPC_5:
# 24 "data/maps/CaveBottom/scripts.pory"
	removefieldmugshot
# 25 "data/maps/CaveBottom/scripts.pory"
	trainerbattle_single TRAINER_EXAMINER_RUNE, Debug_EventScript_Script_1_Text_Battle1_Intro, Debug_EventScript_Script_1_Text_Battle1_Defeat
	goto RuneNPC_1


CaveDoor::
# 37 "data/maps/CaveBottom/scripts.pory"
	lock
# 38 "data/maps/CaveBottom/scripts.pory"
	faceplayer
# 40 "data/maps/CaveBottom/scripts.pory"
	compare VAR_0x8008, 10
	goto_if_ge CaveDoor_2
# 44 "data/maps/CaveBottom/scripts.pory"
	msgbox CaveDoor_Text_1
CaveDoor_1:
# 47 "data/maps/CaveBottom/scripts.pory"
	release
	end

CaveDoor_2:
# 41 "data/maps/CaveBottom/scripts.pory"
	msgbox CaveDoor_Text_0
	goto CaveDoor_1


RuneNPC_Text_0:
# 10 "data/maps/CaveBottom/scripts.pory"
	.string "If you beat me I'll share a secret about\n"
	.string "this room.$"

RuneNPC_Text_1:
# 14 "data/maps/CaveBottom/scripts.pory"
	.string "When crossing this room you can only\n"
	.string "stand on five tiles or the door will\l"
	.string "lock. Bet you didn't know that!$"

RuneNPC_Text_2:
# 21 "data/maps/CaveBottom/scripts.pory"
	.string "Did you want a rematch?$"

CaveDoor_Text_0:
# 41 "data/maps/CaveBottom/scripts.pory"
	.string "Don’t overstep your bounds. The path\n"
	.string "yields the trainer with measured stride.$"

CaveDoor_Text_1:
# 44 "data/maps/CaveBottom/scripts.pory"
	.string "For now, you may pass.$"
