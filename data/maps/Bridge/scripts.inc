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
# 12 "data/maps/Bridge/scripts.pory"
	goto_if_unset FLAG_CHOSE_STARTER, Bridge_OnResume_5
Bridge_OnResume_4:
	end

Bridge_OnResume_2:
# 10 "data/maps/Bridge/scripts.pory"
	setvar VAR_TEMP_0, 1
	goto Bridge_OnResume_1

Bridge_OnResume_5:
# 13 "data/maps/Bridge/scripts.pory"
	setvar VAR_TEMP_1, 1
# 14 "data/maps/Bridge/scripts.pory"
	setflag FLAG_TEMP_HIDE_FOLLOWER
	goto Bridge_OnResume_4


BridgeIntro::
# 20 "data/maps/Bridge/scripts.pory"
	compare VAR_TEMP_0, 1
	goto_if_ne BridgeIntro_2
# 24 "data/maps/Bridge/scripts.pory"
	setvar VAR_TEMP_0, 0
# 25 "data/maps/Bridge/scripts.pory"
	setflag FLAG_TEMP_HIDE_FOLLOWER
# 26 "data/maps/Bridge/scripts.pory"
	setvar VAR_TEMP_1, 1
# 28 "data/maps/Bridge/scripts.pory"
	lock
# 30 "data/maps/Bridge/scripts.pory"
	applymovement OBJ_EVENT_ID_PLAYER, LookUp
# 32 "data/maps/Bridge/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_BERUS
# 33 "data/maps/Bridge/scripts.pory"
	msgbox BridgeIntro_Text_0
# 34 "data/maps/Bridge/scripts.pory"
	removefieldmugshot
# 36 "data/maps/Bridge/scripts.pory"
	msgbox BridgeIntro_Text_1, MSGBOX_YESNO
# 38 "data/maps/Bridge/scripts.pory"
	compare VAR_RESULT, 1
	goto_if_eq BridgeIntro_5
# 43 "data/maps/Bridge/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_RAE
# 44 "data/maps/Bridge/scripts.pory"
	msgbox BridgeIntro_Text_3
# 45 "data/maps/Bridge/scripts.pory"
	removefieldmugshot
BridgeIntro_4:
# 48 "data/maps/Bridge/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_BERUS
# 49 "data/maps/Bridge/scripts.pory"
	msgbox BridgeIntro_Text_4
# 50 "data/maps/Bridge/scripts.pory"
	removefieldmugshot
# 52 "data/maps/Bridge/scripts.pory"
	special DoPlayerNamingScreen
# 53 "data/maps/Bridge/scripts.pory"
	setflag FLAG_BRIDGE_INTRO_DONE
# 54 "data/maps/Bridge/scripts.pory"
	waitstate
# 56 "data/maps/Bridge/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_BERUS
# 57 "data/maps/Bridge/scripts.pory"
	msgbox BridgeIntro_Text_5
# 58 "data/maps/Bridge/scripts.pory"
	removefieldmugshot
# 60 "data/maps/Bridge/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_RAE
# 61 "data/maps/Bridge/scripts.pory"
	msgbox BridgeIntro_Text_6
# 62 "data/maps/Bridge/scripts.pory"
	removefieldmugshot
# 64 "data/maps/Bridge/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_BERUS
# 65 "data/maps/Bridge/scripts.pory"
	msgbox BridgeIntro_Text_7
# 66 "data/maps/Bridge/scripts.pory"
	removefieldmugshot
# 68 "data/maps/Bridge/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_RAE
# 69 "data/maps/Bridge/scripts.pory"
	msgbox BridgeIntro_Text_8
# 70 "data/maps/Bridge/scripts.pory"
	removefieldmugshot
# 72 "data/maps/Bridge/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_BERUS
# 73 "data/maps/Bridge/scripts.pory"
	msgbox BridgeIntro_Text_9
# 74 "data/maps/Bridge/scripts.pory"
	removefieldmugshot
# 76 "data/maps/Bridge/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_RAE
# 77 "data/maps/Bridge/scripts.pory"
	msgbox BridgeIntro_Text_10
# 78 "data/maps/Bridge/scripts.pory"
	removefieldmugshot
# 80 "data/maps/Bridge/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_BERUS
# 81 "data/maps/Bridge/scripts.pory"
	msgbox BridgeIntro_Text_11
# 82 "data/maps/Bridge/scripts.pory"
	removefieldmugshot
# 84 "data/maps/Bridge/scripts.pory"
	release
	end

BridgeIntro_2:
	end

BridgeIntro_5:
# 39 "data/maps/Bridge/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_RAE
# 40 "data/maps/Bridge/scripts.pory"
	msgbox BridgeIntro_Text_2
# 41 "data/maps/Bridge/scripts.pory"
	removefieldmugshot
	goto BridgeIntro_4


# 88 "data/maps/Bridge/scripts.pory"
LookUp:
# 89 "data/maps/Bridge/scripts.pory"
	face_up
	step_end

BerusNPC::
# 96 "data/maps/Bridge/scripts.pory"
	lock
# 98 "data/maps/Bridge/scripts.pory"
	goto_if_unset FLAG_CHOSE_STARTER, BerusNPC_2
# 103 "data/maps/Bridge/scripts.pory"
	checktrainerflag TRAINER_DEITY_TENJIN
	goto_if 1, BerusNPC_3
# 114 "data/maps/Bridge/scripts.pory"
	checktrainerflag TRAINER_BERUS
	goto_if 0, BerusNPC_4
# 123 "data/maps/Bridge/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_BERUS
# 124 "data/maps/Bridge/scripts.pory"
	msgbox BerusNPC_Text_4
# 125 "data/maps/Bridge/scripts.pory"
	removefieldmugshot
BerusNPC_1:
# 129 "data/maps/Bridge/scripts.pory"
	release
	end

BerusNPC_2:
# 99 "data/maps/Bridge/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_BERUS
# 100 "data/maps/Bridge/scripts.pory"
	msgbox BerusNPC_Text_0
# 101 "data/maps/Bridge/scripts.pory"
	removefieldmugshot
	goto BerusNPC_1

BerusNPC_3:
# 105 "data/maps/Bridge/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_BERUS
# 106 "data/maps/Bridge/scripts.pory"
	msgbox BerusNPC_Text_1
# 107 "data/maps/Bridge/scripts.pory"
	removefieldmugshot
# 108 "data/maps/Bridge/scripts.pory"
	trainerbattle_single TRAINER_BERUS, Debug_EventScript_Script_1_Text_Battle1_Intro, Debug_EventScript_Script_1_Text_Battle1_Defeat
# 109 "data/maps/Bridge/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_BERUS
# 110 "data/maps/Bridge/scripts.pory"
	msgbox BerusNPC_Text_2
# 111 "data/maps/Bridge/scripts.pory"
	removefieldmugshot
	goto BerusNPC_1

BerusNPC_4:
# 116 "data/maps/Bridge/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_BERUS
# 117 "data/maps/Bridge/scripts.pory"
	msgbox BerusNPC_Text_3
# 118 "data/maps/Bridge/scripts.pory"
	removefieldmugshot
	goto BerusNPC_1


StarterBag::
# 137 "data/maps/Bridge/scripts.pory"
	lock
# 139 "data/maps/Bridge/scripts.pory"
	goto_if_unset FLAG_CHOSE_STARTER, StarterBag_2
# 165 "data/maps/Bridge/scripts.pory"
	checktrainerflag TRAINER_BERUS
	goto_if 1, StarterBag_3
# 181 "data/maps/Bridge/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_BERUS
# 182 "data/maps/Bridge/scripts.pory"
	msgbox StarterBag_Text_8
# 183 "data/maps/Bridge/scripts.pory"
	removefieldmugshot
StarterBag_1:
# 186 "data/maps/Bridge/scripts.pory"
	release
	end

StarterBag_2:
# 140 "data/maps/Bridge/scripts.pory"
	setflag FLAG_CHOSE_STARTER
# 141 "data/maps/Bridge/scripts.pory"
	special ChooseStarter
# 142 "data/maps/Bridge/scripts.pory"
	waitstate
# 144 "data/maps/Bridge/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_RAE
# 145 "data/maps/Bridge/scripts.pory"
	msgbox StarterBag_Text_0
# 146 "data/maps/Bridge/scripts.pory"
	removefieldmugshot
# 148 "data/maps/Bridge/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_BERUS
# 149 "data/maps/Bridge/scripts.pory"
	msgbox StarterBag_Text_1
# 150 "data/maps/Bridge/scripts.pory"
	removefieldmugshot
# 152 "data/maps/Bridge/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_RAE
# 153 "data/maps/Bridge/scripts.pory"
	msgbox StarterBag_Text_2
# 154 "data/maps/Bridge/scripts.pory"
	removefieldmugshot
# 156 "data/maps/Bridge/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_BERUS
# 157 "data/maps/Bridge/scripts.pory"
	msgbox StarterBag_Text_3
# 158 "data/maps/Bridge/scripts.pory"
	removefieldmugshot
# 160 "data/maps/Bridge/scripts.pory"
	msgbox StarterBag_Text_4
# 162 "data/maps/Bridge/scripts.pory"
	clearflag FLAG_TEMP_HIDE_FOLLOWER
# 163 "data/maps/Bridge/scripts.pory"
	setvar VAR_TEMP_1, 0
	goto StarterBag_1

StarterBag_3:
# 167 "data/maps/Bridge/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_BERUS
# 168 "data/maps/Bridge/scripts.pory"
	msgbox StarterBag_Text_5
# 169 "data/maps/Bridge/scripts.pory"
	removefieldmugshot
# 171 "data/maps/Bridge/scripts.pory"
	msgbox StarterBag_Text_6, MSGBOX_YESNO
# 172 "data/maps/Bridge/scripts.pory"
	compare VAR_RESULT, 1
	goto_if_eq StarterBag_7
	goto StarterBag_1

StarterBag_7:
# 173 "data/maps/Bridge/scripts.pory"
	setvar VAR_0x8008, SPECIES_MR_RIME
# 174 "data/maps/Bridge/scripts.pory"
	special SetPlayerMonSpecies
# 175 "data/maps/Bridge/scripts.pory"
	waitstate
# 176 "data/maps/Bridge/scripts.pory"
	msgbox StarterBag_Text_7
	goto StarterBag_1


# 190 "data/maps/Bridge/scripts.pory"
WalkLeft:
# 191 "data/maps/Bridge/scripts.pory"
	walk_left
	step_end

PreventEarlyStart::
# 195 "data/maps/Bridge/scripts.pory"
	lock
# 197 "data/maps/Bridge/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_BERUS
# 198 "data/maps/Bridge/scripts.pory"
	msgbox BerusNPC_Text_0
# 199 "data/maps/Bridge/scripts.pory"
	removefieldmugshot
# 201 "data/maps/Bridge/scripts.pory"
	applymovement OBJ_EVENT_ID_PLAYER, WalkLeft
# 202 "data/maps/Bridge/scripts.pory"
	waitmovement 0
# 204 "data/maps/Bridge/scripts.pory"
	release
	end


BridgeWarpOrb::
# 210 "data/maps/Bridge/scripts.pory"
	lock
# 212 "data/maps/Bridge/scripts.pory"
	goto_if_unset FLAG_UNLOCK_MANOR_WARP, BridgeWarpOrb_2
BridgeWarpOrb_1:
# 219 "data/maps/Bridge/scripts.pory"
	msgbox BridgeWarpOrb_Text_1
# 221 "data/maps/Bridge/scripts.pory"
	release
	end

BridgeWarpOrb_2:
# 214 "data/maps/Bridge/scripts.pory"
	msgbox BridgeWarpOrb_Text_0
# 215 "data/maps/Bridge/scripts.pory"
	setflag FLAG_UNLOCK_MANOR_WARP
	goto BridgeWarpOrb_1


BridgeIntro_Text_0:
# 33 "data/maps/Bridge/scripts.pory"
	.string "Decided to finally show up, did we?\p"
	.string "Is there a good reason you're six hours\n"
	.string "late?$"

BridgeIntro_Text_1:
# 36 "data/maps/Bridge/scripts.pory"
	.string "Tell the truth?$"

BridgeIntro_Text_2:
# 40 "data/maps/Bridge/scripts.pory"
	.string "Well, the truth is... I overslept.$"

BridgeIntro_Text_3:
# 44 "data/maps/Bridge/scripts.pory"
	.string "Well, the truth is... I had to... er... give\n"
	.string "my train seat to a grandma...\p"
	.string "who was in a wheelchair...\p"
	.string "and uh... also pregnant?\p"
	.string "Then she beat me up she stole my\n"
	.string "ticket.$"

BridgeIntro_Text_4:
# 49 "data/maps/Bridge/scripts.pory"
	.string "Enough of the excuses! Just tell me\n"
	.string "your name so I can mark you down.$"

BridgeIntro_Text_5:
# 57 "data/maps/Bridge/scripts.pory"
	.string "{PLAYER}... ah! You must be Thya's\n"
	.string "child.\p"
	.string "Now there's a great trainer, plenty of\n"
	.string "deities after her contract.$"

BridgeIntro_Text_6:
# 61 "data/maps/Bridge/scripts.pory"
	.string "She's so busy I haven't seen her in\n"
	.string "weeks.$"

BridgeIntro_Text_7:
# 65 "data/maps/Bridge/scripts.pory"
	.string "An inspiration to us all!\p"
	.string "Well {PLAYER}, they say the apple\n"
	.string "dosn't fall far from the tree.\p"
	.string "But that just means they spoil and\n"
	.string "never sprout.$"

BridgeIntro_Text_8:
# 69 "data/maps/Bridge/scripts.pory"
	.string "...?$"

BridgeIntro_Text_9:
# 73 "data/maps/Bridge/scripts.pory"
	.string "I'm saying I cannot allow someone so\n"
	.string "tardy to take the exam.$"

BridgeIntro_Text_10:
# 77 "data/maps/Bridge/scripts.pory"
	.string "But there's still an hour left! I'll be\n"
	.string "real quick.$"

BridgeIntro_Text_11:
# 81 "data/maps/Bridge/scripts.pory"
	.string "Not possible, even the best students\n"
	.string "take a few hours.\p"
	.string "Although....\p"
	.string "Very well, pick a Pokémon, you won't\n"
	.string "pass but I'll enjoy watching you suffer.$"

BerusNPC_Text_0:
# 100 "data/maps/Bridge/scripts.pory"
	.string "Choose a Pokémon from the bag before\n"
	.string "we proceed.$"

BerusNPC_Text_1:
# 106 "data/maps/Bridge/scripts.pory"
	.string "Finally ready to admit defeat? What?\n"
	.string "You beat Tenjin? That's impossible! You\l"
	.string "must have cheated somehow. If you want\l"
	.string "me to accept that you passed, you'll\l"
	.string "have to beat me in battle.$"

BerusNPC_Text_2:
# 110 "data/maps/Bridge/scripts.pory"
	.string "You...YOU...you may have beaten me, but\n"
	.string "look at the sky! It's after sundown\l"
	.string "now... which means you still FAIL. Now if\l"
	.string "you'll excuse me I need to go and heal\l"
	.string "my poor doggy.$"

BerusNPC_Text_3:
# 117 "data/maps/Bridge/scripts.pory"
	.string "Don't my waste time. You should focus\n"
	.string "on trying to pass the test... not that\l"
	.string "you have a chance.$"

BerusNPC_Text_4:
# 124 "data/maps/Bridge/scripts.pory"
	.string "I don't accept this. I do not accept\n"
	.string "that a delinquent like you could pass.$"

StarterBag_Text_0:
# 145 "data/maps/Bridge/scripts.pory"
	.string "Wait, there's only one Pokémon in here...$"

StarterBag_Text_1:
# 149 "data/maps/Bridge/scripts.pory"
	.string "A good trainer makes the best choice in\n"
	.string "any situation.\p"
	.string "A great trainer makes sure there is\n"
	.string "always a good choice to make.\p"
	.string "You're the last to pick so you get the\n"
	.string "leftovers.$"

StarterBag_Text_2:
# 153 "data/maps/Bridge/scripts.pory"
	.string "I guess that's my punishment for\n"
	.string "turning up late.$"

StarterBag_Text_3:
# 157 "data/maps/Bridge/scripts.pory"
	.string "Oh, no. This is your punishment for\n"
	.string "turning up late.$"

StarterBag_Text_4:
# 160 "data/maps/Bridge/scripts.pory"
	.string "Berus used a prayer.\n"
	.string "{PLAYER} was cursed.\p"
	.string "You will receive terrible luck for the\n"
	.string "rest of the test...\p"
	.string "Also there might be some glitches.\n"
	.string "This is the curse...\p"
	.string "and definitely not unfixed bugs.$"

StarterBag_Text_5:
# 168 "data/maps/Bridge/scripts.pory"
	.string "Berus's photo is on the front. I wonder\n"
	.string "if ditto can take his shape?$"

StarterBag_Text_6:
# 171 "data/maps/Bridge/scripts.pory"
	.string "Get ditto to transform?$"

StarterBag_Text_7:
# 176 "data/maps/Bridge/scripts.pory"
	.string "Ditto took the form of Berus$"

StarterBag_Text_8:
# 182 "data/maps/Bridge/scripts.pory"
	.string "You already picked a Pokémon.\n"
	.string "Stop touching my bag!$"

BridgeWarpOrb_Text_0:
# 214 "data/maps/Bridge/scripts.pory"
	.string "A new warp was unlocked!$"

BridgeWarpOrb_Text_1:
# 219 "data/maps/Bridge/scripts.pory"
	.string "Use {SELECT_BUTTON} to warp to the next area.$"
