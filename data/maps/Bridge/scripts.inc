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
	delay 120
# 29 "data/maps/Bridge/scripts.pory"
	setvar VAR_TEMP_0, 0
# 30 "data/maps/Bridge/scripts.pory"
	setflag FLAG_TEMP_HIDE_FOLLOWER
# 31 "data/maps/Bridge/scripts.pory"
	setvar VAR_TEMP_1, 1
# 33 "data/maps/Bridge/scripts.pory"
	lock
# 35 "data/maps/Bridge/scripts.pory"
	applymovement OBJ_EVENT_ID_PLAYER, LookUp
# 37 "data/maps/Bridge/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_BERUS
# 38 "data/maps/Bridge/scripts.pory"
	msgbox BridgeIntro_Text_0
# 39 "data/maps/Bridge/scripts.pory"
	waitmessage
# 40 "data/maps/Bridge/scripts.pory"
	removefieldmugshot
# 42 "data/maps/Bridge/scripts.pory"
	msgbox BridgeIntro_Text_1, MSGBOX_YESNO
# 44 "data/maps/Bridge/scripts.pory"
	compare VAR_RESULT, 1
	goto_if_eq BridgeIntro_5
# 49 "data/maps/Bridge/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_RAE
# 50 "data/maps/Bridge/scripts.pory"
	msgbox BridgeIntro_Text_3
# 51 "data/maps/Bridge/scripts.pory"
	removefieldmugshot
BridgeIntro_4:
# 54 "data/maps/Bridge/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_BERUS
# 55 "data/maps/Bridge/scripts.pory"
	msgbox BridgeIntro_Text_4
# 56 "data/maps/Bridge/scripts.pory"
	removefieldmugshot
# 58 "data/maps/Bridge/scripts.pory"
	special DoPlayerNamingScreen
# 59 "data/maps/Bridge/scripts.pory"
	setflag FLAG_BRIDGE_INTRO_DONE
# 60 "data/maps/Bridge/scripts.pory"
	waitstate
# 62 "data/maps/Bridge/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_BERUS
# 63 "data/maps/Bridge/scripts.pory"
	msgbox BridgeIntro_Text_5
# 64 "data/maps/Bridge/scripts.pory"
	removefieldmugshot
# 66 "data/maps/Bridge/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_RAE
# 67 "data/maps/Bridge/scripts.pory"
	msgbox BridgeIntro_Text_6
# 68 "data/maps/Bridge/scripts.pory"
	removefieldmugshot
# 70 "data/maps/Bridge/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_BERUS
# 71 "data/maps/Bridge/scripts.pory"
	msgbox BridgeIntro_Text_7
# 72 "data/maps/Bridge/scripts.pory"
	removefieldmugshot
# 74 "data/maps/Bridge/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_RAE
# 75 "data/maps/Bridge/scripts.pory"
	msgbox BridgeIntro_Text_8
# 76 "data/maps/Bridge/scripts.pory"
	removefieldmugshot
# 78 "data/maps/Bridge/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_BERUS
# 79 "data/maps/Bridge/scripts.pory"
	msgbox BridgeIntro_Text_9
# 80 "data/maps/Bridge/scripts.pory"
	removefieldmugshot
# 82 "data/maps/Bridge/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_RAE
# 83 "data/maps/Bridge/scripts.pory"
	msgbox BridgeIntro_Text_10
# 84 "data/maps/Bridge/scripts.pory"
	removefieldmugshot
# 86 "data/maps/Bridge/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_BERUS
# 87 "data/maps/Bridge/scripts.pory"
	msgbox BridgeIntro_Text_11
# 88 "data/maps/Bridge/scripts.pory"
	removefieldmugshot
# 90 "data/maps/Bridge/scripts.pory"
	release
	end

BridgeIntro_2:
	end

BridgeIntro_5:
# 45 "data/maps/Bridge/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_RAE
# 46 "data/maps/Bridge/scripts.pory"
	msgbox BridgeIntro_Text_2
# 47 "data/maps/Bridge/scripts.pory"
	removefieldmugshot
	goto BridgeIntro_4


# 94 "data/maps/Bridge/scripts.pory"
LookUp:
# 95 "data/maps/Bridge/scripts.pory"
	face_up
	step_end

BerusNPC::
# 102 "data/maps/Bridge/scripts.pory"
	lock
# 104 "data/maps/Bridge/scripts.pory"
	checktrainerflag TRAINER_BERUS
	goto_if 1, BerusNPC_2
# 114 "data/maps/Bridge/scripts.pory"
	goto_if_unset FLAG_CHOSE_STARTER, BerusNPC_3
# 119 "data/maps/Bridge/scripts.pory"
	checktrainerflag TRAINER_DEITY_TENJIN
	goto_if 1, BerusNPC_4
# 140 "data/maps/Bridge/scripts.pory"
	checktrainerflag TRAINER_DEITY_TENJIN
	goto_if 0, BerusNPC_5
# 149 "data/maps/Bridge/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_BERUS
# 150 "data/maps/Bridge/scripts.pory"
	msgbox BerusNPC_Text_5
# 151 "data/maps/Bridge/scripts.pory"
	removefieldmugshot
BerusNPC_1:
# 155 "data/maps/Bridge/scripts.pory"
	release
	end

BerusNPC_2:
# 106 "data/maps/Bridge/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_BERUS
# 107 "data/maps/Bridge/scripts.pory"
	msgbox BerusNPC_Text_0
# 108 "data/maps/Bridge/scripts.pory"
	removefieldmugshot
# 110 "data/maps/Bridge/scripts.pory"
	setflag FLAG_HIDE_BERUS
# 111 "data/maps/Bridge/scripts.pory"
	removeobject 1
# 112 "data/maps/Bridge/scripts.pory"
	removeobject 5
	goto BerusNPC_1

BerusNPC_3:
# 115 "data/maps/Bridge/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_BERUS
# 116 "data/maps/Bridge/scripts.pory"
	msgbox BerusNPC_Text_1
# 117 "data/maps/Bridge/scripts.pory"
	removefieldmugshot
	goto BerusNPC_1

BerusNPC_4:
# 121 "data/maps/Bridge/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_BERUS
# 122 "data/maps/Bridge/scripts.pory"
	msgbox BerusNPC_Text_2
# 123 "data/maps/Bridge/scripts.pory"
	removefieldmugshot
# 124 "data/maps/Bridge/scripts.pory"
	trainerbattle_single TRAINER_BERUS, Debug_EventScript_Script_1_Text_Battle1_Intro, Debug_EventScript_Script_1_Text_Battle1_Defeat
# 126 "data/maps/Bridge/scripts.pory"
	special GetBattleOutcome
# 127 "data/maps/Bridge/scripts.pory"
	compare VAR_RESULT, B_OUTCOME_WON
	goto_if_eq BerusNPC_11
	goto BerusNPC_1

BerusNPC_5:
# 142 "data/maps/Bridge/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_BERUS
# 143 "data/maps/Bridge/scripts.pory"
	msgbox BerusNPC_Text_4
# 144 "data/maps/Bridge/scripts.pory"
	removefieldmugshot
	goto BerusNPC_1

BerusNPC_11:
# 129 "data/maps/Bridge/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_BERUS
# 130 "data/maps/Bridge/scripts.pory"
	msgbox BerusNPC_Text_0
# 131 "data/maps/Bridge/scripts.pory"
	removefieldmugshot
# 132 "data/maps/Bridge/scripts.pory"
	msgbox BerusNPC_Text_3
# 134 "data/maps/Bridge/scripts.pory"
	setflag FLAG_HIDE_BERUS
# 135 "data/maps/Bridge/scripts.pory"
	removeobject 1
# 136 "data/maps/Bridge/scripts.pory"
	removeobject 5
	goto BerusNPC_1


StarterBag::
# 163 "data/maps/Bridge/scripts.pory"
	lock
# 165 "data/maps/Bridge/scripts.pory"
	goto_if_unset FLAG_CHOSE_STARTER, StarterBag_2
# 191 "data/maps/Bridge/scripts.pory"
	goto_if_set FLAG_HIDE_BERUS, StarterBag_3
# 206 "data/maps/Bridge/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_BERUS
# 207 "data/maps/Bridge/scripts.pory"
	msgbox StarterBag_Text_8
# 208 "data/maps/Bridge/scripts.pory"
	removefieldmugshot
StarterBag_1:
# 211 "data/maps/Bridge/scripts.pory"
	release
	end

StarterBag_2:
# 166 "data/maps/Bridge/scripts.pory"
	setflag FLAG_CHOSE_STARTER
# 167 "data/maps/Bridge/scripts.pory"
	special ChooseStarter
# 168 "data/maps/Bridge/scripts.pory"
	waitstate
# 170 "data/maps/Bridge/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_RAE
# 171 "data/maps/Bridge/scripts.pory"
	msgbox StarterBag_Text_0
# 172 "data/maps/Bridge/scripts.pory"
	removefieldmugshot
# 174 "data/maps/Bridge/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_BERUS
# 175 "data/maps/Bridge/scripts.pory"
	msgbox StarterBag_Text_1
# 176 "data/maps/Bridge/scripts.pory"
	removefieldmugshot
# 178 "data/maps/Bridge/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_RAE
# 179 "data/maps/Bridge/scripts.pory"
	msgbox StarterBag_Text_2
# 180 "data/maps/Bridge/scripts.pory"
	removefieldmugshot
# 182 "data/maps/Bridge/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_BERUS
# 183 "data/maps/Bridge/scripts.pory"
	msgbox StarterBag_Text_3
# 184 "data/maps/Bridge/scripts.pory"
	removefieldmugshot
# 186 "data/maps/Bridge/scripts.pory"
	msgbox StarterBag_Text_4
# 188 "data/maps/Bridge/scripts.pory"
	clearflag FLAG_TEMP_HIDE_FOLLOWER
# 189 "data/maps/Bridge/scripts.pory"
	setvar VAR_TEMP_1, 0
	goto StarterBag_1

StarterBag_3:
# 193 "data/maps/Bridge/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_RAE
# 194 "data/maps/Bridge/scripts.pory"
	msgbox StarterBag_Text_5
# 195 "data/maps/Bridge/scripts.pory"
	removefieldmugshot
# 197 "data/maps/Bridge/scripts.pory"
	msgbox StarterBag_Text_6, MSGBOX_YESNO
# 198 "data/maps/Bridge/scripts.pory"
	compare VAR_RESULT, 1
	goto_if_eq StarterBag_7
	goto StarterBag_1

StarterBag_7:
# 199 "data/maps/Bridge/scripts.pory"
	setvar VAR_0x8008, SPECIES_MR_RIME
# 200 "data/maps/Bridge/scripts.pory"
	special SetPlayerMonSpecies
# 201 "data/maps/Bridge/scripts.pory"
	msgbox StarterBag_Text_7
# 202 "data/maps/Bridge/scripts.pory"
	special WarpToStairs
	goto StarterBag_1


# 215 "data/maps/Bridge/scripts.pory"
WalkLeft:
# 216 "data/maps/Bridge/scripts.pory"
	walk_left
	step_end

PreventEarlyStart::
# 220 "data/maps/Bridge/scripts.pory"
	lock
# 222 "data/maps/Bridge/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_BERUS
# 223 "data/maps/Bridge/scripts.pory"
	msgbox BerusNPC_Text_1
# 224 "data/maps/Bridge/scripts.pory"
	removefieldmugshot
# 226 "data/maps/Bridge/scripts.pory"
	applymovement OBJ_EVENT_ID_PLAYER, WalkLeft
# 227 "data/maps/Bridge/scripts.pory"
	waitmovement 0
# 229 "data/maps/Bridge/scripts.pory"
	release
	end


BridgeWarpOrb::
# 235 "data/maps/Bridge/scripts.pory"
	lock
# 237 "data/maps/Bridge/scripts.pory"
	goto_if_unset FLAG_UNLOCK_MANOR_WARP, BridgeWarpOrb_2
BridgeWarpOrb_1:
# 245 "data/maps/Bridge/scripts.pory"
	msgbox BridgeWarpOrb_Text_1
# 247 "data/maps/Bridge/scripts.pory"
	release
	end

BridgeWarpOrb_2:
# 238 "data/maps/Bridge/scripts.pory"
	setflag FLAG_UNLOCK_MANOR_WARP
# 239 "data/maps/Bridge/scripts.pory"
	special AutoSave
# 240 "data/maps/Bridge/scripts.pory"
	playfanfare MUS_OBTAIN_BADGE
# 241 "data/maps/Bridge/scripts.pory"
	msgbox BridgeWarpOrb_Text_0
# 242 "data/maps/Bridge/scripts.pory"
	waitfanfare
	goto BridgeWarpOrb_1


BridgeIntro_Text_0:
# 38 "data/maps/Bridge/scripts.pory"
	.string "Decided to finally show up, did we?\p"
	.string "Is there a good reason you're six hours\n"
	.string "late?$"

BridgeIntro_Text_1:
# 42 "data/maps/Bridge/scripts.pory"
	.string "Tell the truth?$"

BridgeIntro_Text_2:
# 46 "data/maps/Bridge/scripts.pory"
	.string "Well, the truth is... I overslept.$"

BridgeIntro_Text_3:
# 50 "data/maps/Bridge/scripts.pory"
	.string "Well, the truth is... I had to... er... give\n"
	.string "my train seat to a grandma...\p"
	.string "who was in a wheelchair...\p"
	.string "and uh... also pregnant?\p"
	.string "Then she beat me up and stole my\n"
	.string "ticket.$"

BridgeIntro_Text_4:
# 55 "data/maps/Bridge/scripts.pory"
	.string "Enough of the excuses! Just tell me\n"
	.string "your name so I can mark you down.$"

BridgeIntro_Text_5:
# 63 "data/maps/Bridge/scripts.pory"
	.string "{PLAYER}... ah! You must be Thya's\n"
	.string "child.\p"
	.string "Now there's a great trainer, plenty of\n"
	.string "deities after her contract.$"

BridgeIntro_Text_6:
# 67 "data/maps/Bridge/scripts.pory"
	.string "She's so busy I haven't seen her in\n"
	.string "weeks.$"

BridgeIntro_Text_7:
# 71 "data/maps/Bridge/scripts.pory"
	.string "An inspiration to us all!\p"
	.string "Well {PLAYER}, they say the apple\n"
	.string "doesn't fall far from the tree.\p"
	.string "But that just means they spoil and\n"
	.string "never sprout.$"

BridgeIntro_Text_8:
# 75 "data/maps/Bridge/scripts.pory"
	.string "...?$"

BridgeIntro_Text_9:
# 79 "data/maps/Bridge/scripts.pory"
	.string "I'm saying I cannot allow someone so\n"
	.string "tardy to take the exam.$"

BridgeIntro_Text_10:
# 83 "data/maps/Bridge/scripts.pory"
	.string "But there's still an hour left! I'll be\n"
	.string "real quick.$"

BridgeIntro_Text_11:
# 87 "data/maps/Bridge/scripts.pory"
	.string "Not possible, even the best students\n"
	.string "take a few hours.\p"
	.string "Although....\p"
	.string "Very well, pick a Pokémon, you won't\n"
	.string "pass but I'll enjoy watching you suffer.$"

BerusNPC_Text_0:
# 107 "data/maps/Bridge/scripts.pory"
	.string "You...\p"
	.string "YOU...\p"
	.string "You may have beaten me, but...\p"
	.string "The timer on the exam ran out during our\n"
	.string "battle...\p"
	.string "So you still FAIL.\p"
	.string "Now if you'll excuse me I need to go and\n"
	.string "heal my poor doggo.$"

BerusNPC_Text_1:
# 116 "data/maps/Bridge/scripts.pory"
	.string "Choose a Pokémon from the bag before\n"
	.string "we proceed.$"

BerusNPC_Text_2:
# 122 "data/maps/Bridge/scripts.pory"
	.string "Finally ready to admit defeat?\p"
	.string "What? You beat Tenjin? That's\n"
	.string "impossible!\p"
	.string "You must have cheated somehow.\n"
	.string "I'll show you what I do to cheaters!$"

BerusNPC_Text_3:
# 132 "data/maps/Bridge/scripts.pory"
	.string "Berus used a prayer.\n"
	.string "Mega evo restrictions were removed.$"

BerusNPC_Text_4:
# 143 "data/maps/Bridge/scripts.pory"
	.string "Don't waste my time. You should focus\n"
	.string "on trying to pass the test...\p"
	.string "Not that you have a chance.$"

BerusNPC_Text_5:
# 150 "data/maps/Bridge/scripts.pory"
	.string "I don't accept this. I do not accept\n"
	.string "that a delinquent like you could pass.$"

StarterBag_Text_0:
# 171 "data/maps/Bridge/scripts.pory"
	.string "Wait, there's only one Pokémon in here...$"

StarterBag_Text_1:
# 175 "data/maps/Bridge/scripts.pory"
	.string "A good trainer makes the best choice in\n"
	.string "any situation.\p"
	.string "A great trainer makes sure there is\n"
	.string "always a good choice to make.\p"
	.string "You're the last to pick so you get the\n"
	.string "leftovers.$"

StarterBag_Text_2:
# 179 "data/maps/Bridge/scripts.pory"
	.string "I guess that's my punishment for\n"
	.string "turning up late.$"

StarterBag_Text_3:
# 183 "data/maps/Bridge/scripts.pory"
	.string "Oh, no. This is your punishment for\n"
	.string "turning up late.$"

StarterBag_Text_4:
# 186 "data/maps/Bridge/scripts.pory"
	.string "Berus used a prayer.\n"
	.string "{PLAYER} was cursed.\p"
	.string "You will receive terrible luck for the\n"
	.string "rest of the test...\p"
	.string "Also there might be some glitches.\n"
	.string "This is the curse...\p"
	.string "and definitely not unfixed bugs.$"

StarterBag_Text_5:
# 194 "data/maps/Bridge/scripts.pory"
	.string "He was in such a hurry he left his bag...\p"
	.string "With his photo ID in it...\p"
	.string "$"

StarterBag_Text_6:
# 197 "data/maps/Bridge/scripts.pory"
	.string "Get ditto to commit identity theft?$"

StarterBag_Text_7:
# 201 "data/maps/Bridge/scripts.pory"
	.string "Ditto took the form of Berus.\p"
	.string "$"

StarterBag_Text_8:
# 207 "data/maps/Bridge/scripts.pory"
	.string "You already picked a Pokémon.\n"
	.string "Stop touching my bag!$"

BridgeWarpOrb_Text_0:
# 241 "data/maps/Bridge/scripts.pory"
	.string "A new warp was unlocked!$"

BridgeWarpOrb_Text_1:
# 245 "data/maps/Bridge/scripts.pory"
	.string "Use {SELECT_BUTTON} to warp to the next area.$"
