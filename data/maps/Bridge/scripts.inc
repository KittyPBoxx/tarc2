Bridge_MapScripts::
# 5 "data/maps/Bridge/scripts.pory"
	map_script MAP_SCRIPT_ON_RESUME, Bridge_OnResume
# 6 "data/maps/Bridge/scripts.pory"
	map_script MAP_SCRIPT_ON_FRAME_TABLE, Bridge_MapScripts_MAP_SCRIPT_ON_FRAME_TABLE
	.byte 0

Bridge_MapScripts_MAP_SCRIPT_ON_FRAME_TABLE:
# 7 "data/maps/Bridge/scripts.pory"
	map_script_2 VAR_TEMP_0, 1, BridgeIntro
	.2byte 0


Bridge_OnResume::
# 12 "data/maps/Bridge/scripts.pory"
	goto_if_unset FLAG_BRIDGE_INTRO_DONE, Bridge_OnResume_2
Bridge_OnResume_1:
# 15 "data/maps/Bridge/scripts.pory"
	goto_if_unset FLAG_CHOSE_STARTER, Bridge_OnResume_5
Bridge_OnResume_4:
	end

Bridge_OnResume_2:
# 13 "data/maps/Bridge/scripts.pory"
	setvar VAR_TEMP_0, 1
	goto Bridge_OnResume_1

Bridge_OnResume_5:
# 16 "data/maps/Bridge/scripts.pory"
	setvar VAR_TEMP_1, 1
# 17 "data/maps/Bridge/scripts.pory"
	setflag FLAG_TEMP_HIDE_FOLLOWER
	goto Bridge_OnResume_4


BridgeIntro::
# 23 "data/maps/Bridge/scripts.pory"
	compare VAR_TEMP_0, 1
	goto_if_ne BridgeIntro_2
# 27 "data/maps/Bridge/scripts.pory"
	setvar VAR_TEMP_0, 0
# 28 "data/maps/Bridge/scripts.pory"
	setflag FLAG_TEMP_HIDE_FOLLOWER
# 29 "data/maps/Bridge/scripts.pory"
	setvar VAR_TEMP_1, 1
# 31 "data/maps/Bridge/scripts.pory"
	lock
# 33 "data/maps/Bridge/scripts.pory"
	applymovement OBJ_EVENT_ID_PLAYER, LookUp
# 35 "data/maps/Bridge/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_BERUS
# 36 "data/maps/Bridge/scripts.pory"
	msgbox BridgeIntro_Text_0
# 37 "data/maps/Bridge/scripts.pory"
	removefieldmugshot
# 39 "data/maps/Bridge/scripts.pory"
	msgbox BridgeIntro_Text_1, MSGBOX_YESNO
# 41 "data/maps/Bridge/scripts.pory"
	compare VAR_RESULT, 1
	goto_if_eq BridgeIntro_5
# 46 "data/maps/Bridge/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_RAE
# 47 "data/maps/Bridge/scripts.pory"
	msgbox BridgeIntro_Text_3
# 48 "data/maps/Bridge/scripts.pory"
	removefieldmugshot
BridgeIntro_4:
# 51 "data/maps/Bridge/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_BERUS
# 52 "data/maps/Bridge/scripts.pory"
	msgbox BridgeIntro_Text_4
# 53 "data/maps/Bridge/scripts.pory"
	removefieldmugshot
# 55 "data/maps/Bridge/scripts.pory"
	special DoPlayerNamingScreen
# 56 "data/maps/Bridge/scripts.pory"
	setflag FLAG_BRIDGE_INTRO_DONE
# 57 "data/maps/Bridge/scripts.pory"
	waitstate
# 59 "data/maps/Bridge/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_BERUS
# 60 "data/maps/Bridge/scripts.pory"
	msgbox BridgeIntro_Text_5
# 61 "data/maps/Bridge/scripts.pory"
	removefieldmugshot
# 63 "data/maps/Bridge/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_RAE
# 64 "data/maps/Bridge/scripts.pory"
	msgbox BridgeIntro_Text_6
# 65 "data/maps/Bridge/scripts.pory"
	removefieldmugshot
# 67 "data/maps/Bridge/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_BERUS
# 68 "data/maps/Bridge/scripts.pory"
	msgbox BridgeIntro_Text_7
# 69 "data/maps/Bridge/scripts.pory"
	removefieldmugshot
# 71 "data/maps/Bridge/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_RAE
# 72 "data/maps/Bridge/scripts.pory"
	msgbox BridgeIntro_Text_8
# 73 "data/maps/Bridge/scripts.pory"
	removefieldmugshot
# 75 "data/maps/Bridge/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_BERUS
# 76 "data/maps/Bridge/scripts.pory"
	msgbox BridgeIntro_Text_9
# 77 "data/maps/Bridge/scripts.pory"
	removefieldmugshot
# 79 "data/maps/Bridge/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_RAE
# 80 "data/maps/Bridge/scripts.pory"
	msgbox BridgeIntro_Text_10
# 81 "data/maps/Bridge/scripts.pory"
	removefieldmugshot
# 83 "data/maps/Bridge/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_BERUS
# 84 "data/maps/Bridge/scripts.pory"
	msgbox BridgeIntro_Text_11
# 85 "data/maps/Bridge/scripts.pory"
	removefieldmugshot
# 87 "data/maps/Bridge/scripts.pory"
	release
	end

BridgeIntro_2:
	end

BridgeIntro_5:
# 42 "data/maps/Bridge/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_RAE
# 43 "data/maps/Bridge/scripts.pory"
	msgbox BridgeIntro_Text_2
# 44 "data/maps/Bridge/scripts.pory"
	removefieldmugshot
	goto BridgeIntro_4


# 91 "data/maps/Bridge/scripts.pory"
LookUp:
# 92 "data/maps/Bridge/scripts.pory"
	face_up
	step_end

BerusNPC::
# 99 "data/maps/Bridge/scripts.pory"
	lock
# 101 "data/maps/Bridge/scripts.pory"
	checktrainerflag TRAINER_BERUS
	goto_if 1, BerusNPC_2
# 111 "data/maps/Bridge/scripts.pory"
	goto_if_unset FLAG_CHOSE_STARTER, BerusNPC_3
# 116 "data/maps/Bridge/scripts.pory"
	checktrainerflag TRAINER_DEITY_TENJIN
	goto_if 1, BerusNPC_4
# 136 "data/maps/Bridge/scripts.pory"
	checktrainerflag TRAINER_DEITY_TENJIN
	goto_if 0, BerusNPC_5
# 145 "data/maps/Bridge/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_BERUS
# 146 "data/maps/Bridge/scripts.pory"
	msgbox BerusNPC_Text_4
# 147 "data/maps/Bridge/scripts.pory"
	removefieldmugshot
BerusNPC_1:
# 151 "data/maps/Bridge/scripts.pory"
	release
	end

BerusNPC_2:
# 103 "data/maps/Bridge/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_BERUS
# 104 "data/maps/Bridge/scripts.pory"
	msgbox BerusNPC_Text_0
# 105 "data/maps/Bridge/scripts.pory"
	removefieldmugshot
# 107 "data/maps/Bridge/scripts.pory"
	setflag FLAG_HIDE_BERUS
# 108 "data/maps/Bridge/scripts.pory"
	removeobject 1
# 109 "data/maps/Bridge/scripts.pory"
	removeobject 5
	goto BerusNPC_1

BerusNPC_3:
# 112 "data/maps/Bridge/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_BERUS
# 113 "data/maps/Bridge/scripts.pory"
	msgbox BerusNPC_Text_1
# 114 "data/maps/Bridge/scripts.pory"
	removefieldmugshot
	goto BerusNPC_1

BerusNPC_4:
# 118 "data/maps/Bridge/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_BERUS
# 119 "data/maps/Bridge/scripts.pory"
	msgbox BerusNPC_Text_2
# 120 "data/maps/Bridge/scripts.pory"
	removefieldmugshot
# 121 "data/maps/Bridge/scripts.pory"
	trainerbattle_single TRAINER_BERUS, Debug_EventScript_Script_1_Text_Battle1_Intro, Debug_EventScript_Script_1_Text_Battle1_Defeat
# 123 "data/maps/Bridge/scripts.pory"
	special GetBattleOutcome
# 124 "data/maps/Bridge/scripts.pory"
	compare VAR_RESULT, B_OUTCOME_WON
	goto_if_eq BerusNPC_11
	goto BerusNPC_1

BerusNPC_5:
# 138 "data/maps/Bridge/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_BERUS
# 139 "data/maps/Bridge/scripts.pory"
	msgbox BerusNPC_Text_3
# 140 "data/maps/Bridge/scripts.pory"
	removefieldmugshot
	goto BerusNPC_1

BerusNPC_11:
# 126 "data/maps/Bridge/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_BERUS
# 127 "data/maps/Bridge/scripts.pory"
	msgbox BerusNPC_Text_0
# 128 "data/maps/Bridge/scripts.pory"
	removefieldmugshot
# 130 "data/maps/Bridge/scripts.pory"
	setflag FLAG_HIDE_BERUS
# 131 "data/maps/Bridge/scripts.pory"
	removeobject 1
# 132 "data/maps/Bridge/scripts.pory"
	removeobject 5
	goto BerusNPC_1


StarterBag::
# 159 "data/maps/Bridge/scripts.pory"
	lock
# 161 "data/maps/Bridge/scripts.pory"
	goto_if_unset FLAG_CHOSE_STARTER, StarterBag_2
# 187 "data/maps/Bridge/scripts.pory"
	goto_if_set FLAG_HIDE_BERUS, StarterBag_3
# 202 "data/maps/Bridge/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_BERUS
# 203 "data/maps/Bridge/scripts.pory"
	msgbox StarterBag_Text_8
# 204 "data/maps/Bridge/scripts.pory"
	removefieldmugshot
StarterBag_1:
# 207 "data/maps/Bridge/scripts.pory"
	release
	end

StarterBag_2:
# 162 "data/maps/Bridge/scripts.pory"
	setflag FLAG_CHOSE_STARTER
# 163 "data/maps/Bridge/scripts.pory"
	special ChooseStarter
# 164 "data/maps/Bridge/scripts.pory"
	waitstate
# 166 "data/maps/Bridge/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_RAE
# 167 "data/maps/Bridge/scripts.pory"
	msgbox StarterBag_Text_0
# 168 "data/maps/Bridge/scripts.pory"
	removefieldmugshot
# 170 "data/maps/Bridge/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_BERUS
# 171 "data/maps/Bridge/scripts.pory"
	msgbox StarterBag_Text_1
# 172 "data/maps/Bridge/scripts.pory"
	removefieldmugshot
# 174 "data/maps/Bridge/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_RAE
# 175 "data/maps/Bridge/scripts.pory"
	msgbox StarterBag_Text_2
# 176 "data/maps/Bridge/scripts.pory"
	removefieldmugshot
# 178 "data/maps/Bridge/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_BERUS
# 179 "data/maps/Bridge/scripts.pory"
	msgbox StarterBag_Text_3
# 180 "data/maps/Bridge/scripts.pory"
	removefieldmugshot
# 182 "data/maps/Bridge/scripts.pory"
	msgbox StarterBag_Text_4
# 184 "data/maps/Bridge/scripts.pory"
	clearflag FLAG_TEMP_HIDE_FOLLOWER
# 185 "data/maps/Bridge/scripts.pory"
	setvar VAR_TEMP_1, 0
	goto StarterBag_1

StarterBag_3:
# 189 "data/maps/Bridge/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_RAE
# 190 "data/maps/Bridge/scripts.pory"
	msgbox StarterBag_Text_5
# 191 "data/maps/Bridge/scripts.pory"
	removefieldmugshot
# 193 "data/maps/Bridge/scripts.pory"
	msgbox StarterBag_Text_6, MSGBOX_YESNO
# 194 "data/maps/Bridge/scripts.pory"
	compare VAR_RESULT, 1
	goto_if_eq StarterBag_7
	goto StarterBag_1

StarterBag_7:
# 195 "data/maps/Bridge/scripts.pory"
	setvar VAR_0x8008, SPECIES_MR_RIME
# 196 "data/maps/Bridge/scripts.pory"
	special SetPlayerMonSpecies
# 197 "data/maps/Bridge/scripts.pory"
	msgbox StarterBag_Text_7
# 198 "data/maps/Bridge/scripts.pory"
	special WarpToStairs
	goto StarterBag_1


# 211 "data/maps/Bridge/scripts.pory"
WalkLeft:
# 212 "data/maps/Bridge/scripts.pory"
	walk_left
	step_end

PreventEarlyStart::
# 216 "data/maps/Bridge/scripts.pory"
	lock
# 218 "data/maps/Bridge/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_BERUS
# 219 "data/maps/Bridge/scripts.pory"
	msgbox BerusNPC_Text_1
# 220 "data/maps/Bridge/scripts.pory"
	removefieldmugshot
# 222 "data/maps/Bridge/scripts.pory"
	applymovement OBJ_EVENT_ID_PLAYER, WalkLeft
# 223 "data/maps/Bridge/scripts.pory"
	waitmovement 0
# 225 "data/maps/Bridge/scripts.pory"
	release
	end


BridgeWarpOrb::
# 231 "data/maps/Bridge/scripts.pory"
	lock
# 233 "data/maps/Bridge/scripts.pory"
	goto_if_unset FLAG_UNLOCK_MANOR_WARP, BridgeWarpOrb_2
BridgeWarpOrb_1:
# 240 "data/maps/Bridge/scripts.pory"
	msgbox BridgeWarpOrb_Text_1
# 242 "data/maps/Bridge/scripts.pory"
	release
	end

BridgeWarpOrb_2:
# 235 "data/maps/Bridge/scripts.pory"
	msgbox BridgeWarpOrb_Text_0
# 236 "data/maps/Bridge/scripts.pory"
	setflag FLAG_UNLOCK_MANOR_WARP
	goto BridgeWarpOrb_1


BridgeIntro_Text_0:
# 36 "data/maps/Bridge/scripts.pory"
	.string "Decided to finally show up, did we?\p"
	.string "Is there a good reason you're six hours\n"
	.string "late?$"

BridgeIntro_Text_1:
# 39 "data/maps/Bridge/scripts.pory"
	.string "Tell the truth?$"

BridgeIntro_Text_2:
# 43 "data/maps/Bridge/scripts.pory"
	.string "Well, the truth is... I overslept.$"

BridgeIntro_Text_3:
# 47 "data/maps/Bridge/scripts.pory"
	.string "Well, the truth is... I had to... er... give\n"
	.string "my train seat to a grandma...\p"
	.string "who was in a wheelchair...\p"
	.string "and uh... also pregnant?\p"
	.string "Then she beat me up she stole my\n"
	.string "ticket.$"

BridgeIntro_Text_4:
# 52 "data/maps/Bridge/scripts.pory"
	.string "Enough of the excuses! Just tell me\n"
	.string "your name so I can mark you down.$"

BridgeIntro_Text_5:
# 60 "data/maps/Bridge/scripts.pory"
	.string "{PLAYER}... ah! You must be Thya's\n"
	.string "child.\p"
	.string "Now there's a great trainer, plenty of\n"
	.string "deities after her contract.$"

BridgeIntro_Text_6:
# 64 "data/maps/Bridge/scripts.pory"
	.string "She's so busy I haven't seen her in\n"
	.string "weeks.$"

BridgeIntro_Text_7:
# 68 "data/maps/Bridge/scripts.pory"
	.string "An inspiration to us all!\p"
	.string "Well {PLAYER}, they say the apple\n"
	.string "dosn't fall far from the tree.\p"
	.string "But that just means they spoil and\n"
	.string "never sprout.$"

BridgeIntro_Text_8:
# 72 "data/maps/Bridge/scripts.pory"
	.string "...?$"

BridgeIntro_Text_9:
# 76 "data/maps/Bridge/scripts.pory"
	.string "I'm saying I cannot allow someone so\n"
	.string "tardy to take the exam.$"

BridgeIntro_Text_10:
# 80 "data/maps/Bridge/scripts.pory"
	.string "But there's still an hour left! I'll be\n"
	.string "real quick.$"

BridgeIntro_Text_11:
# 84 "data/maps/Bridge/scripts.pory"
	.string "Not possible, even the best students\n"
	.string "take a few hours.\p"
	.string "Although....\p"
	.string "Very well, pick a Pokémon, you won't\n"
	.string "pass but I'll enjoy watching you suffer.$"

BerusNPC_Text_0:
# 104 "data/maps/Bridge/scripts.pory"
	.string "You...\p"
	.string "YOU...\p"
	.string "You may have beaten me, but...\p"
	.string "The timer on the exam ran out during our\n"
	.string "battle...\p"
	.string "So you still FAIL.\p"
	.string "Now if you'll excuse me I need to go and\n"
	.string "heal my poor doggo.$"

BerusNPC_Text_1:
# 113 "data/maps/Bridge/scripts.pory"
	.string "Choose a Pokémon from the bag before\n"
	.string "we proceed.$"

BerusNPC_Text_2:
# 119 "data/maps/Bridge/scripts.pory"
	.string "Finally ready to admit defeat? What?\n"
	.string "You beat Tenjin? That's impossible!\p"
	.string "You must have cheated somehow.\n"
	.string "I'll show you what I do to cheaters!$"

BerusNPC_Text_3:
# 139 "data/maps/Bridge/scripts.pory"
	.string "Don't my waste time. You should focus\n"
	.string "on trying to pass the test...\p"
	.string "Not that you have a chance.$"

BerusNPC_Text_4:
# 146 "data/maps/Bridge/scripts.pory"
	.string "I don't accept this. I do not accept\n"
	.string "that a delinquent like you could pass.$"

StarterBag_Text_0:
# 167 "data/maps/Bridge/scripts.pory"
	.string "Wait, there's only one Pokémon in here...$"

StarterBag_Text_1:
# 171 "data/maps/Bridge/scripts.pory"
	.string "A good trainer makes the best choice in\n"
	.string "any situation.\p"
	.string "A great trainer makes sure there is\n"
	.string "always a good choice to make.\p"
	.string "You're the last to pick so you get the\n"
	.string "leftovers.$"

StarterBag_Text_2:
# 175 "data/maps/Bridge/scripts.pory"
	.string "I guess that's my punishment for\n"
	.string "turning up late.$"

StarterBag_Text_3:
# 179 "data/maps/Bridge/scripts.pory"
	.string "Oh, no. This is your punishment for\n"
	.string "turning up late.$"

StarterBag_Text_4:
# 182 "data/maps/Bridge/scripts.pory"
	.string "Berus used a prayer.\n"
	.string "{PLAYER} was cursed.\p"
	.string "You will receive terrible luck for the\n"
	.string "rest of the test...\p"
	.string "Also there might be some glitches.\n"
	.string "This is the curse...\p"
	.string "and definitely not unfixed bugs.$"

StarterBag_Text_5:
# 190 "data/maps/Bridge/scripts.pory"
	.string "He was in such a hurry he left his bag...\p"
	.string "With his photo id in it...\p"
	.string "$"

StarterBag_Text_6:
# 193 "data/maps/Bridge/scripts.pory"
	.string "Get ditto to commit identity theft?$"

StarterBag_Text_7:
# 197 "data/maps/Bridge/scripts.pory"
	.string "Ditto took the form of Berus\p"
	.string "$"

StarterBag_Text_8:
# 203 "data/maps/Bridge/scripts.pory"
	.string "You already picked a Pokémon.\n"
	.string "Stop touching my bag!$"

BridgeWarpOrb_Text_0:
# 235 "data/maps/Bridge/scripts.pory"
	.string "A new warp was unlocked!$"

BridgeWarpOrb_Text_1:
# 240 "data/maps/Bridge/scripts.pory"
	.string "Use {SELECT_BUTTON} to warp to the next area.$"
