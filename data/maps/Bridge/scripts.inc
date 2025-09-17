Bridge_MapScripts::
# 2 "data/maps/Bridge/scripts.pory"
	map_script MAP_SCRIPT_ON_RESUME, Bridge_OnResume
# 3 "data/maps/Bridge/scripts.pory"
	map_script MAP_SCRIPT_ON_FRAME_TABLE, Bridge_MapScripts_MAP_SCRIPT_ON_FRAME_TABLE
	.byte 0

Bridge_MapScripts_MAP_SCRIPT_ON_FRAME_TABLE:
# 4 "data/maps/Bridge/scripts.pory"
	map_script_2 VAR_TEMP_0, 1, BridgeIntro
	.2byte 0


Bridge_OnResume::
# 9 "data/maps/Bridge/scripts.pory"
	goto_if_unset FLAG_BRIDGE_INTRO_DONE, Bridge_OnResume_2
Bridge_OnResume_1:
	end

Bridge_OnResume_2:
# 10 "data/maps/Bridge/scripts.pory"
	setvar VAR_TEMP_0, 1
	goto Bridge_OnResume_1


BridgeIntro::
# 16 "data/maps/Bridge/scripts.pory"
	compare VAR_TEMP_0, 1
	goto_if_ne BridgeIntro_2
# 20 "data/maps/Bridge/scripts.pory"
	setvar VAR_TEMP_0, 0
# 22 "data/maps/Bridge/scripts.pory"
	lock
# 24 "data/maps/Bridge/scripts.pory"
	applymovement OBJ_EVENT_ID_PLAYER, LookUp
# 26 "data/maps/Bridge/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_NORMAL
# 27 "data/maps/Bridge/scripts.pory"
	msgbox BridgeIntro_Text_0
# 28 "data/maps/Bridge/scripts.pory"
	removefieldmugshot
# 30 "data/maps/Bridge/scripts.pory"
	msgbox BridgeIntro_Text_1, MSGBOX_YESNO
# 32 "data/maps/Bridge/scripts.pory"
	compare VAR_RESULT, 1
	goto_if_eq BridgeIntro_5
# 37 "data/maps/Bridge/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_ALT
# 38 "data/maps/Bridge/scripts.pory"
	msgbox BridgeIntro_Text_3
# 39 "data/maps/Bridge/scripts.pory"
	removefieldmugshot
BridgeIntro_4:
# 42 "data/maps/Bridge/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_NORMAL
# 43 "data/maps/Bridge/scripts.pory"
	msgbox BridgeIntro_Text_4
# 44 "data/maps/Bridge/scripts.pory"
	removefieldmugshot
# 46 "data/maps/Bridge/scripts.pory"
	special DoPlayerNamingScreen
# 47 "data/maps/Bridge/scripts.pory"
	setflag FLAG_BRIDGE_INTRO_DONE
# 48 "data/maps/Bridge/scripts.pory"
	waitstate
# 50 "data/maps/Bridge/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_NORMAL
# 51 "data/maps/Bridge/scripts.pory"
	msgbox BridgeIntro_Text_5
# 52 "data/maps/Bridge/scripts.pory"
	removefieldmugshot
# 54 "data/maps/Bridge/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_ALT
# 55 "data/maps/Bridge/scripts.pory"
	msgbox BridgeIntro_Text_6
# 56 "data/maps/Bridge/scripts.pory"
	removefieldmugshot
# 58 "data/maps/Bridge/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_NORMAL
# 59 "data/maps/Bridge/scripts.pory"
	msgbox BridgeIntro_Text_7
# 60 "data/maps/Bridge/scripts.pory"
	removefieldmugshot
# 62 "data/maps/Bridge/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_ALT
# 63 "data/maps/Bridge/scripts.pory"
	msgbox BridgeIntro_Text_8
# 64 "data/maps/Bridge/scripts.pory"
	removefieldmugshot
# 66 "data/maps/Bridge/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_NORMAL
# 67 "data/maps/Bridge/scripts.pory"
	msgbox BridgeIntro_Text_9
# 68 "data/maps/Bridge/scripts.pory"
	removefieldmugshot
# 70 "data/maps/Bridge/scripts.pory"
	release
	end

BridgeIntro_2:
	end

BridgeIntro_5:
# 33 "data/maps/Bridge/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_ALT
# 34 "data/maps/Bridge/scripts.pory"
	msgbox BridgeIntro_Text_2
# 35 "data/maps/Bridge/scripts.pory"
	removefieldmugshot
	goto BridgeIntro_4


# 74 "data/maps/Bridge/scripts.pory"
LookUp:
# 75 "data/maps/Bridge/scripts.pory"
	face_up
	step_end

BerusNPC::
# 82 "data/maps/Bridge/scripts.pory"
	lock
# 83 "data/maps/Bridge/scripts.pory"
	faceplayer
# 85 "data/maps/Bridge/scripts.pory"
	goto_if_unset FLAG_CHOSE_STARTER, BerusNPC_2
# 90 "data/maps/Bridge/scripts.pory"
	checktrainerflag TRAINER_DEITY_TENJIN
	goto_if 1, BerusNPC_3
# 101 "data/maps/Bridge/scripts.pory"
	checktrainerflag TRAINER_BERUS
	goto_if 0, BerusNPC_4
# 110 "data/maps/Bridge/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_NORMAL
# 111 "data/maps/Bridge/scripts.pory"
	msgbox BerusNPC_Text_4
# 112 "data/maps/Bridge/scripts.pory"
	removefieldmugshot
BerusNPC_1:
# 116 "data/maps/Bridge/scripts.pory"
	release
	end

BerusNPC_2:
# 86 "data/maps/Bridge/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_NORMAL
# 87 "data/maps/Bridge/scripts.pory"
	msgbox BerusNPC_Text_0
# 88 "data/maps/Bridge/scripts.pory"
	removefieldmugshot
	goto BerusNPC_1

BerusNPC_3:
# 92 "data/maps/Bridge/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_NORMAL
# 93 "data/maps/Bridge/scripts.pory"
	msgbox BerusNPC_Text_1
# 94 "data/maps/Bridge/scripts.pory"
	removefieldmugshot
# 95 "data/maps/Bridge/scripts.pory"
	trainerbattle_single TRAINER_BERUS, Debug_EventScript_Script_1_Text_Battle1_Intro, Debug_EventScript_Script_1_Text_Battle1_Defeat
# 96 "data/maps/Bridge/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_NORMAL
# 97 "data/maps/Bridge/scripts.pory"
	msgbox BerusNPC_Text_2
# 98 "data/maps/Bridge/scripts.pory"
	removefieldmugshot
	goto BerusNPC_1

BerusNPC_4:
# 103 "data/maps/Bridge/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_NORMAL
# 104 "data/maps/Bridge/scripts.pory"
	msgbox BerusNPC_Text_3
# 105 "data/maps/Bridge/scripts.pory"
	removefieldmugshot
	goto BerusNPC_1


StarterBag::
# 124 "data/maps/Bridge/scripts.pory"
	lock
# 125 "data/maps/Bridge/scripts.pory"
	faceplayer
# 127 "data/maps/Bridge/scripts.pory"
	goto_if_unset FLAG_CHOSE_STARTER, StarterBag_2
# 150 "data/maps/Bridge/scripts.pory"
	checktrainerflag TRAINER_BERUS
	goto_if 0, StarterBag_3
# 166 "data/maps/Bridge/scripts.pory"
	msgbox StarterBag_Text_8
StarterBag_1:
# 169 "data/maps/Bridge/scripts.pory"
	release
	end

StarterBag_2:
# 128 "data/maps/Bridge/scripts.pory"
	setflag FLAG_CHOSE_STARTER
# 129 "data/maps/Bridge/scripts.pory"
	special ChooseStarter
# 130 "data/maps/Bridge/scripts.pory"
	waitstate
# 132 "data/maps/Bridge/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_ALT
# 133 "data/maps/Bridge/scripts.pory"
	msgbox StarterBag_Text_0
# 134 "data/maps/Bridge/scripts.pory"
	removefieldmugshot
# 136 "data/maps/Bridge/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_NORMAL
# 137 "data/maps/Bridge/scripts.pory"
	msgbox StarterBag_Text_1
# 138 "data/maps/Bridge/scripts.pory"
	removefieldmugshot
# 140 "data/maps/Bridge/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_ALT
# 141 "data/maps/Bridge/scripts.pory"
	msgbox StarterBag_Text_2
# 142 "data/maps/Bridge/scripts.pory"
	removefieldmugshot
# 144 "data/maps/Bridge/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_NORMAL
# 145 "data/maps/Bridge/scripts.pory"
	msgbox StarterBag_Text_3
# 146 "data/maps/Bridge/scripts.pory"
	removefieldmugshot
# 148 "data/maps/Bridge/scripts.pory"
	msgbox StarterBag_Text_4
	goto StarterBag_1

StarterBag_3:
# 152 "data/maps/Bridge/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_NORMAL
# 153 "data/maps/Bridge/scripts.pory"
	msgbox StarterBag_Text_5
# 154 "data/maps/Bridge/scripts.pory"
	removefieldmugshot
# 156 "data/maps/Bridge/scripts.pory"
	msgbox StarterBag_Text_6, MSGBOX_YESNO
# 157 "data/maps/Bridge/scripts.pory"
	compare VAR_RESULT, 1
	goto_if_eq StarterBag_7
	goto StarterBag_1

StarterBag_7:
# 158 "data/maps/Bridge/scripts.pory"
	setvar VAR_0x8008, SPECIES_MR_RIME
# 159 "data/maps/Bridge/scripts.pory"
	special SetPlayerMonSpecies
# 160 "data/maps/Bridge/scripts.pory"
	waitstate
# 161 "data/maps/Bridge/scripts.pory"
	msgbox StarterBag_Text_7
	goto StarterBag_1


CutTree::
# 177 "data/maps/Bridge/scripts.pory"
	lock
# 178 "data/maps/Bridge/scripts.pory"
	faceplayer
# 180 "data/maps/Bridge/scripts.pory"
	goto_if_unset FLAG_TREE_CUT, CutTree_2
# 196 "data/maps/Bridge/scripts.pory"
	msgbox CutTree_Text_1
CutTree_1:
# 199 "data/maps/Bridge/scripts.pory"
	release
	end

CutTree_2:
# 183 "data/maps/Bridge/scripts.pory"
	msgbox CutTree_Text_0
# 185 "data/maps/Bridge/scripts.pory"
	checkpartymove MOVE_CUT
# 187 "data/maps/Bridge/scripts.pory"
	compare VAR_RESULT, 1
	goto_if_eq CutTree_5
	goto CutTree_1

CutTree_5:
# 189 "data/maps/Bridge/scripts.pory"
	setflag FLAG_TREE_CUT
	goto CutTree_1


BridgeIntro_Text_0:
# 27 "data/maps/Bridge/scripts.pory"
	.string "Decided to finally show up, did we? Is\n"
	.string "there a good reason you're four hours\l"
	.string "late?$"

BridgeIntro_Text_1:
# 30 "data/maps/Bridge/scripts.pory"
	.string "Tell the truth?$"

BridgeIntro_Text_2:
# 34 "data/maps/Bridge/scripts.pory"
	.string "Well, the truth is... I overslept.$"

BridgeIntro_Text_3:
# 38 "data/maps/Bridge/scripts.pory"
	.string "Well, the truth is... I had to... er... give\n"
	.string "my train seat to a grandma... who was in\l"
	.string "a wheelchair... and uh... also pregnant?\l"
	.string "Then we had a fight and she stole my\l"
	.string "ticket.$"

BridgeIntro_Text_4:
# 43 "data/maps/Bridge/scripts.pory"
	.string "Enough of the excuses! Just tell me\n"
	.string "your name so I can mark you down.$"

BridgeIntro_Text_5:
# 51 "data/maps/Bridge/scripts.pory"
	.string "{PLAYER}... ah! You must be Thya's\n"
	.string "child. I heards she's sponsored by that\l"
	.string "trickster god now. How is she?$"

BridgeIntro_Text_6:
# 55 "data/maps/Bridge/scripts.pory"
	.string "She's so busy I haven't seen her in\n"
	.string "weeks.$"

BridgeIntro_Text_7:
# 59 "data/maps/Bridge/scripts.pory"
	.string "An inspiration to us all. Well\n"
	.string "{PLAYER}, they say the apple dosn't\l"
	.string "fall far from the tree. But did you know\l"
	.string "that when fruit lands in shade it never\l"
	.string "sprouts? In fact it tends to spoil. I\l"
	.string "cannot allow someone so tardy to take\l"
	.string "the exam.$"

BridgeIntro_Text_8:
# 63 "data/maps/Bridge/scripts.pory"
	.string "But the exam's not over untill sunset,\n"
	.string "surely I can still pass if I do it real\l"
	.string "quick.$"

BridgeIntro_Text_9:
# 67 "data/maps/Bridge/scripts.pory"
	.string "'Real quick'? You really think you can\n"
	.string "pass this prestigious test after\l"
	.string "turning up so late? Amusing. Very well,\l"
	.string "take your POKéMON and become\l"
	.string "cautionary tale for next years cohort.$"

BerusNPC_Text_0:
# 87 "data/maps/Bridge/scripts.pory"
	.string "Choose a Pokémon from the bag before\n"
	.string "we proceed.$"

BerusNPC_Text_1:
# 93 "data/maps/Bridge/scripts.pory"
	.string "Finally ready to admit defeat? What?\n"
	.string "You beat Tenjin? That's impossible! You\l"
	.string "must have cheated somehow. If you want\l"
	.string "me to accept that you passed, you'll\l"
	.string "have to beat me in battle.$"

BerusNPC_Text_2:
# 97 "data/maps/Bridge/scripts.pory"
	.string "You...YOU...you may have beaten me, but\n"
	.string "look at the sky! It's after sundown\l"
	.string "now... which means you still FAIL. Now if\l"
	.string "you'll excuse me I need to go and heal\l"
	.string "my poor doggy.$"

BerusNPC_Text_3:
# 104 "data/maps/Bridge/scripts.pory"
	.string "Don't my waste time. You should focus\n"
	.string "on trying to pass the test... not that\l"
	.string "you have a chance.$"

BerusNPC_Text_4:
# 111 "data/maps/Bridge/scripts.pory"
	.string "I don't accept this. I do not accept\n"
	.string "that a delinquent like you could pass.$"

StarterBag_Text_0:
# 133 "data/maps/Bridge/scripts.pory"
	.string "Wait, there's only one Pokémon in here...$"

StarterBag_Text_1:
# 137 "data/maps/Bridge/scripts.pory"
	.string "A good trainer makes the best choice in\n"
	.string "any situation. A great trainer makes\l"
	.string "sure there is always a good choice to\l"
	.string "make. You're the last to pick so you get\l"
	.string "the leftovers.$"

StarterBag_Text_2:
# 141 "data/maps/Bridge/scripts.pory"
	.string "I guess that's my punishment for\n"
	.string "turning up late.$"

StarterBag_Text_3:
# 145 "data/maps/Bridge/scripts.pory"
	.string "Oh, no. This is your punishment for\n"
	.string "turning up late.$"

StarterBag_Text_4:
# 148 "data/maps/Bridge/scripts.pory"
	.string "Berus used a prayer. {PLAYER} was\n"
	.string "cursed. You will receive terrible luck\l"
	.string "for the rest of the test.$"

StarterBag_Text_5:
# 153 "data/maps/Bridge/scripts.pory"
	.string "Berus's photo is on the front. I wonder\n"
	.string "if ditto can take his shape?$"

StarterBag_Text_6:
# 156 "data/maps/Bridge/scripts.pory"
	.string "Get ditto to transform?$"

StarterBag_Text_7:
# 161 "data/maps/Bridge/scripts.pory"
	.string "Ditto took the form of Berus$"

StarterBag_Text_8:
# 166 "data/maps/Bridge/scripts.pory"
	.string "It's an empty bag with Berus's photo on\n"
	.string "the front.$"

CutTree_Text_0:
# 183 "data/maps/Bridge/scripts.pory"
	.string "This tree looks like it could be cut\n"
	.string "down.$"

CutTree_Text_1:
# 196 "data/maps/Bridge/scripts.pory"
	.string "The tree has already been cleared.$"
