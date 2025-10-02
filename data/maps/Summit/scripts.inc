Summit_MapScripts::
	.byte 0


TenjinNPC::
# 4 "data/maps/Summit/scripts.pory"
	lock
# 6 "data/maps/Summit/scripts.pory"
	goto_if_unset FLAG_TENJIN_INTRO, TenjinNPC_2
TenjinNPC_1:
# 38 "data/maps/Summit/scripts.pory"
	checktrainerflag TRAINER_DEITY_TENJIN
	goto_if 0, TenjinNPC_5
# 64 "data/maps/Summit/scripts.pory"
	goto_if_unset FLAG_UNLOCK_BRIDGE_WARP, TenjinNPC_10
# 80 "data/maps/Summit/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_TENJIN
# 81 "data/maps/Summit/scripts.pory"
	msgbox TenjinNPC_Text_10
# 82 "data/maps/Summit/scripts.pory"
	removefieldmugshot
# 84 "data/maps/Summit/scripts.pory"
	msgbox TenjinNPC_Text_11, MSGBOX_YESNO
# 86 "data/maps/Summit/scripts.pory"
	compare VAR_RESULT, 1
	goto_if_eq TenjinNPC_13
TenjinNPC_4:
# 93 "data/maps/Summit/scripts.pory"
	release
	end

TenjinNPC_2:
# 7 "data/maps/Summit/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_TENJIN
# 8 "data/maps/Summit/scripts.pory"
	msgbox TenjinNPC_Text_0
# 9 "data/maps/Summit/scripts.pory"
	removefieldmugshot
# 11 "data/maps/Summit/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_TENJIN
# 12 "data/maps/Summit/scripts.pory"
	msgbox TenjinNPC_Text_1
# 13 "data/maps/Summit/scripts.pory"
	removefieldmugshot
# 15 "data/maps/Summit/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_RAE
# 16 "data/maps/Summit/scripts.pory"
	msgbox TenjinNPC_Text_2
# 17 "data/maps/Summit/scripts.pory"
	removefieldmugshot
# 19 "data/maps/Summit/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_TENJIN
# 20 "data/maps/Summit/scripts.pory"
	msgbox TenjinNPC_Text_3
# 21 "data/maps/Summit/scripts.pory"
	removefieldmugshot
# 23 "data/maps/Summit/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_RAE
# 24 "data/maps/Summit/scripts.pory"
	msgbox TenjinNPC_Text_2
# 25 "data/maps/Summit/scripts.pory"
	removefieldmugshot
# 27 "data/maps/Summit/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_TENJIN
# 28 "data/maps/Summit/scripts.pory"
	msgbox TenjinNPC_Text_4
# 29 "data/maps/Summit/scripts.pory"
	removefieldmugshot
# 31 "data/maps/Summit/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_RAE
# 32 "data/maps/Summit/scripts.pory"
	msgbox TenjinNPC_Text_2
# 33 "data/maps/Summit/scripts.pory"
	removefieldmugshot
# 35 "data/maps/Summit/scripts.pory"
	setflag FLAG_TENJIN_INTRO
	goto TenjinNPC_1

TenjinNPC_5:
# 40 "data/maps/Summit/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_TENJIN
# 41 "data/maps/Summit/scripts.pory"
	msgbox TenjinNPC_Text_5
# 42 "data/maps/Summit/scripts.pory"
	removefieldmugshot
# 43 "data/maps/Summit/scripts.pory"
	trainerbattle_double TRAINER_DEITY_TENJIN, Debug_EventScript_Script_1_Text_Battle1_Intro, Debug_EventScript_Script_1_Text_Battle1_Defeat, Debug_EventScript_Script_1_Text_Battle1_NotEnoughMons
# 45 "data/maps/Summit/scripts.pory"
	special GetBattleOutcome
# 46 "data/maps/Summit/scripts.pory"
	compare VAR_RESULT, B_OUTCOME_WON
	goto_if_eq TenjinNPC_8
	goto TenjinNPC_4

TenjinNPC_8:
# 48 "data/maps/Summit/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_TENJIN
# 49 "data/maps/Summit/scripts.pory"
	msgbox TenjinNPC_Text_6
# 50 "data/maps/Summit/scripts.pory"
	msgbox TenjinNPC_Text_7
# 51 "data/maps/Summit/scripts.pory"
	removefieldmugshot
# 53 "data/maps/Summit/scripts.pory"
	playfanfare MUS_OBTAIN_BADGE
# 54 "data/maps/Summit/scripts.pory"
	msgbox TenjinNPC_Text_8
# 55 "data/maps/Summit/scripts.pory"
	setflag FLAG_UNLOCK_BRIDGE_WARP
# 56 "data/maps/Summit/scripts.pory"
	clearflag FLAG_HIDE_FROSSGLASS
# 58 "data/maps/Summit/scripts.pory"
	msgbox TenjinNPC_Text_9
	goto TenjinNPC_4

TenjinNPC_10:
# 66 "data/maps/Summit/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_TENJIN
# 67 "data/maps/Summit/scripts.pory"
	msgbox TenjinNPC_Text_6
# 68 "data/maps/Summit/scripts.pory"
	msgbox TenjinNPC_Text_7
# 69 "data/maps/Summit/scripts.pory"
	removefieldmugshot
# 71 "data/maps/Summit/scripts.pory"
	playfanfare MUS_OBTAIN_BADGE
# 72 "data/maps/Summit/scripts.pory"
	msgbox TenjinNPC_Text_8
# 73 "data/maps/Summit/scripts.pory"
	setflag FLAG_UNLOCK_BRIDGE_WARP
# 74 "data/maps/Summit/scripts.pory"
	clearflag FLAG_HIDE_FROSSGLASS
# 76 "data/maps/Summit/scripts.pory"
	msgbox TenjinNPC_Text_9
	goto TenjinNPC_4

TenjinNPC_13:
# 87 "data/maps/Summit/scripts.pory"
	trainerbattle_double TRAINER_DEITY_TENJIN, Debug_EventScript_Script_1_Text_Battle1_Intro, Debug_EventScript_Script_1_Text_Battle1_Defeat, Debug_EventScript_Script_1_Text_Battle1_NotEnoughMons
	goto TenjinNPC_4


SlidingTilePuzzle::
# 99 "data/maps/Summit/scripts.pory"
	lock
# 101 "data/maps/Summit/scripts.pory"
	msgbox SlidingTilePuzzle_Text_0
# 104 "data/maps/Summit/scripts.pory"
	setvar VAR_0x8004, 1
# 105 "data/maps/Summit/scripts.pory"
	setvar VAR_0x8005, 0
# 106 "data/maps/Summit/scripts.pory"
	setvar VAR_0x8008, 0
# 107 "data/maps/Summit/scripts.pory"
	special DoSlidingPuzzle
# 108 "data/maps/Summit/scripts.pory"
	waitstate
# 110 "data/maps/Summit/scripts.pory"
	compare VAR_0x8008, 0
	goto_if_ne SlidingTilePuzzle_2
SlidingTilePuzzle_1:
# 120 "data/maps/Summit/scripts.pory"
	release
	end

SlidingTilePuzzle_2:
# 112 "data/maps/Summit/scripts.pory"
	msgbox SlidingTilePuzzle_Text_1, MSGBOX_YESNO
# 114 "data/maps/Summit/scripts.pory"
	compare VAR_RESULT, 1
	goto_if_eq SlidingTilePuzzle_4
	goto SlidingTilePuzzle_1

SlidingTilePuzzle_4:
# 115 "data/maps/Summit/scripts.pory"
	special SetPlayerMonSpecies
# 116 "data/maps/Summit/scripts.pory"
	msgbox SlidingTilePuzzle_Text_2
	goto SlidingTilePuzzle_1


# 124 "data/maps/Summit/scripts.pory"
MakeShortcut:
# 125 "data/maps/Summit/scripts.pory"
	walk_left
# 125 "data/maps/Summit/scripts.pory"
	walk_left
	step_end

TenjinNPC_Text_0:
# 8 "data/maps/Summit/scripts.pory"
	.string "Teamwork.\p"
	.string "The power of two.\p"
	.string "You finally figured out the secret to\n"
	.string "this exam.\p"
	.string "Learning to work with another\n"
	.string "candidate.$"

TenjinNPC_Text_1:
# 12 "data/maps/Summit/scripts.pory"
	.string "...\p"
	.string "Where is the other candidate?$"

TenjinNPC_Text_2:
# 16 "data/maps/Summit/scripts.pory"
	.string "...\p"
	.string "$"

TenjinNPC_Text_3:
# 20 "data/maps/Summit/scripts.pory"
	.string "...but the last battle was supposed to\n"
	.string "be impossible to solo...\p"
	.string "Give me one sec, I need to rewrite my\n"
	.string "speech.$"

TenjinNPC_Text_4:
# 28 "data/maps/Summit/scripts.pory"
	.string "Umm... determination.\p"
	.string "The power of er... perseverance.\p"
	.string "You finally figured out the secret...$"

TenjinNPC_Text_5:
# 41 "data/maps/Summit/scripts.pory"
	.string "Sigh... let's just do the battle, ok?$"

TenjinNPC_Text_6:
# 49 "data/maps/Summit/scripts.pory"
	.string "That was the most exciting battle I've\n"
	.string "had in years!\p"
	.string "You definitely deserve an internship\n"
	.string "with a god.\p"
	.string "Maybe even a paid one!\p"
	.string "I'm not supposed to do this but I'll let\n"
	.string "you warp straight to Berus.$"

TenjinNPC_Text_7:
# 50 "data/maps/Summit/scripts.pory"
	.string "You need to hurry because the exam is\n"
	.string "almost over.\p"
	.string "And avoid the Lake! Scary Pokémon\n"
	.string "appear there at night.$"

TenjinNPC_Text_8:
# 54 "data/maps/Summit/scripts.pory"
	.string "A new warp was unlocked!$"

TenjinNPC_Text_9:
# 58 "data/maps/Summit/scripts.pory"
	.string "Use {SELECT_BUTTON} to warp to the next area.$"

TenjinNPC_Text_10:
# 81 "data/maps/Summit/scripts.pory"
	.string "You should go back now.\p"
	.string "And avoid the Lake! Scary Pokémon\n"
	.string "appear there at night.$"

TenjinNPC_Text_11:
# 84 "data/maps/Summit/scripts.pory"
	.string "Unless you want a rematch?$"

SlidingTilePuzzle_Text_0:
# 101 "data/maps/Summit/scripts.pory"
	.string "These rocks look like an image...$"

SlidingTilePuzzle_Text_1:
# 112 "data/maps/Summit/scripts.pory"
	.string "The image of a pokemon was revealed.\nDo you want Ditto to take its form?$"

SlidingTilePuzzle_Text_2:
# 116 "data/maps/Summit/scripts.pory"
	.string "Ditto took on a new form!$"
