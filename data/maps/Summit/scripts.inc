Summit_MapScripts::
	.byte 0


TenjinNPC::
# 4 "data/maps/Summit/scripts.pory"
	lock
# 6 "data/maps/Summit/scripts.pory"
	goto_if_unset FLAG_TENJIN_INTRO, TenjinNPC_2
# 37 "data/maps/Summit/scripts.pory"
	checktrainerflag TRAINER_DEITY_TENJIN
	goto_if 1, TenjinNPC_3
# 52 "data/maps/Summit/scripts.pory"
	goto_if_unset FLAG_TILE_PUZZLE_SOLVED, TenjinNPC_10
# 60 "data/maps/Summit/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_BERUS
# 61 "data/maps/Summit/scripts.pory"
	msgbox TenjinNPC_Text_8
# 62 "data/maps/Summit/scripts.pory"
	removefieldmugshot
TenjinNPC_9:
# 66 "data/maps/Summit/scripts.pory"
	trainerbattle_double TRAINER_DEITY_TENJIN, Debug_EventScript_Script_1_Text_Battle1_Intro, Debug_EventScript_Script_1_Text_Battle1_Defeat, Debug_EventScript_Script_1_Text_Battle1_NotEnoughMons
# 67 "data/maps/Summit/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_BERUS
# 68 "data/maps/Summit/scripts.pory"
	msgbox TenjinNPC_Text_9
# 69 "data/maps/Summit/scripts.pory"
	applymovement 1, Common_Movement_QuestionMark
# 70 "data/maps/Summit/scripts.pory"
	msgbox TenjinNPC_Text_10
# 71 "data/maps/Summit/scripts.pory"
	removefieldmugshot
# 73 "data/maps/Summit/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_RAE
# 74 "data/maps/Summit/scripts.pory"
	msgbox TenjinNPC_Text_2
# 75 "data/maps/Summit/scripts.pory"
	removefieldmugshot
# 77 "data/maps/Summit/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_BERUS
# 78 "data/maps/Summit/scripts.pory"
	msgbox TenjinNPC_Text_11
# 79 "data/maps/Summit/scripts.pory"
	removefieldmugshot
# 81 "data/maps/Summit/scripts.pory"
	applymovement 1, MakeShortcut
TenjinNPC_1:
# 86 "data/maps/Summit/scripts.pory"
	release
	end

TenjinNPC_2:
# 8 "data/maps/Summit/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_BERUS
# 9 "data/maps/Summit/scripts.pory"
	msgbox TenjinNPC_Text_0
# 10 "data/maps/Summit/scripts.pory"
	removefieldmugshot
# 12 "data/maps/Summit/scripts.pory"
	faceplayer
# 13 "data/maps/Summit/scripts.pory"
	applymovement 1, Common_Movement_QuestionMark
# 14 "data/maps/Summit/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_BERUS
# 15 "data/maps/Summit/scripts.pory"
	msgbox TenjinNPC_Text_1
# 16 "data/maps/Summit/scripts.pory"
	removefieldmugshot
# 18 "data/maps/Summit/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_RAE
# 19 "data/maps/Summit/scripts.pory"
	msgbox TenjinNPC_Text_2
# 20 "data/maps/Summit/scripts.pory"
	removefieldmugshot
# 22 "data/maps/Summit/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_BERUS
# 23 "data/maps/Summit/scripts.pory"
	msgbox TenjinNPC_Text_3
# 24 "data/maps/Summit/scripts.pory"
	removefieldmugshot
# 26 "data/maps/Summit/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_RAE
# 27 "data/maps/Summit/scripts.pory"
	msgbox TenjinNPC_Text_2
# 28 "data/maps/Summit/scripts.pory"
	removefieldmugshot
# 30 "data/maps/Summit/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_BERUS
# 31 "data/maps/Summit/scripts.pory"
	msgbox TenjinNPC_Text_4
# 32 "data/maps/Summit/scripts.pory"
	removefieldmugshot
# 34 "data/maps/Summit/scripts.pory"
	setflag FLAG_TENJIN_INTRO
	goto TenjinNPC_1

TenjinNPC_3:
# 39 "data/maps/Summit/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_RAE
# 40 "data/maps/Summit/scripts.pory"
	msgbox TenjinNPC_Text_5, MSGBOX_YESNO
# 41 "data/maps/Summit/scripts.pory"
	removefieldmugshot
# 43 "data/maps/Summit/scripts.pory"
	compare VAR_RESULT, 1
	goto_if_eq TenjinNPC_7
	goto TenjinNPC_1

TenjinNPC_7:
# 44 "data/maps/Summit/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_RAE
# 45 "data/maps/Summit/scripts.pory"
	msgbox TenjinNPC_Text_6
# 46 "data/maps/Summit/scripts.pory"
	removefieldmugshot
# 47 "data/maps/Summit/scripts.pory"
	trainerbattle_double TRAINER_DEITY_TENJIN, Debug_EventScript_Script_1_Text_Battle1_Intro, Debug_EventScript_Script_1_Text_Battle1_Defeat, Debug_EventScript_Script_1_Text_Battle1_NotEnoughMons
	goto TenjinNPC_1

TenjinNPC_10:
# 54 "data/maps/Summit/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_BERUS
# 55 "data/maps/Summit/scripts.pory"
	msgbox TenjinNPC_Text_7
# 56 "data/maps/Summit/scripts.pory"
	removefieldmugshot
	goto TenjinNPC_9


SlidingTilePuzzle::
# 92 "data/maps/Summit/scripts.pory"
	lock
# 94 "data/maps/Summit/scripts.pory"
	goto_if_unset FLAG_TILE_PUZZLE_ROCK_BROKEN, SlidingTilePuzzle_2
# 109 "data/maps/Summit/scripts.pory"
	setvar VAR_0x8004, 1
# 110 "data/maps/Summit/scripts.pory"
	setvar VAR_0x8005, 0
# 111 "data/maps/Summit/scripts.pory"
	special DoSlidingPuzzle
# 113 "data/maps/Summit/scripts.pory"
	compare VAR_RESULT, 1
	goto_if_eq SlidingTilePuzzle_7
# 124 "data/maps/Summit/scripts.pory"
	compare VAR_RESULT, 1
	goto_if_eq SlidingTilePuzzle_8
SlidingTilePuzzle_1:
# 139 "data/maps/Summit/scripts.pory"
	release
	end

SlidingTilePuzzle_2:
# 96 "data/maps/Summit/scripts.pory"
	msgbox SlidingTilePuzzle_Text_0
# 98 "data/maps/Summit/scripts.pory"
	checkpartymove MOVE_ROCK_SMASH
# 100 "data/maps/Summit/scripts.pory"
	compare VAR_RESULT, 1
	goto_if_eq SlidingTilePuzzle_5
	goto SlidingTilePuzzle_1

SlidingTilePuzzle_5:
# 102 "data/maps/Summit/scripts.pory"
	setflag FLAG_TILE_PUZZLE_ROCK_BROKEN
# 104 "data/maps/Summit/scripts.pory"
	msgbox SlidingTilePuzzle_Text_1
	goto SlidingTilePuzzle_1

SlidingTilePuzzle_7:
# 115 "data/maps/Summit/scripts.pory"
	msgbox SlidingTilePuzzle_Text_2, MSGBOX_YESNO
# 117 "data/maps/Summit/scripts.pory"
	compare VAR_RESULT, 1
	goto_if_eq SlidingTilePuzzle_11
	goto SlidingTilePuzzle_1

SlidingTilePuzzle_8:
# 126 "data/maps/Summit/scripts.pory"
	msgbox SlidingTilePuzzle_Text_2, MSGBOX_YESNO
# 128 "data/maps/Summit/scripts.pory"
	compare VAR_RESULT, 1
	goto_if_eq SlidingTilePuzzle_13
	goto SlidingTilePuzzle_1

SlidingTilePuzzle_11:
# 118 "data/maps/Summit/scripts.pory"
	setvar VAR_0x8008, SPECIES_INKAY
# 119 "data/maps/Summit/scripts.pory"
	special SetPlayerMonSpecies
# 120 "data/maps/Summit/scripts.pory"
	waitstate
# 121 "data/maps/Summit/scripts.pory"
	msgbox SlidingTilePuzzle_Text_3
	goto SlidingTilePuzzle_1

SlidingTilePuzzle_13:
# 129 "data/maps/Summit/scripts.pory"
	setvar VAR_0x8008, SPECIES_SANDYGAST
# 130 "data/maps/Summit/scripts.pory"
	special SetPlayerMonSpecies
# 131 "data/maps/Summit/scripts.pory"
	waitstate
# 132 "data/maps/Summit/scripts.pory"
	msgbox SlidingTilePuzzle_Text_4
	goto SlidingTilePuzzle_1


# 143 "data/maps/Summit/scripts.pory"
MakeShortcut:
# 144 "data/maps/Summit/scripts.pory"
	walk_left
# 144 "data/maps/Summit/scripts.pory"
	walk_left
	step_end

TenjinNPC_Text_0:
# 9 "data/maps/Summit/scripts.pory"
	.string "Teamwork. The power of two. You finally\n"
	.string "worked out the one candidate couldn't\l"
	.string "beat exam alone, put your differences\l"
	.string "aside and now you face me.$"

TenjinNPC_Text_1:
# 15 "data/maps/Summit/scripts.pory"
	.string "Wait... why is there only one of you? The\n"
	.string "exams supposed to be impossible to do\l"
	.string "solo... I'm supposed to give a speech...$"

TenjinNPC_Text_2:
# 19 "data/maps/Summit/scripts.pory"
	.string "...$"

TenjinNPC_Text_3:
# 23 "data/maps/Summit/scripts.pory"
	.string "Umm... determination and er....\n"
	.string "perseverance. Yes! These were the\l"
	.string "qualities you need to pass the test!$"

TenjinNPC_Text_4:
# 31 "data/maps/Summit/scripts.pory"
	.string "Sigh... ok, I'll level with you. Candidates\n"
	.string "aren't supposed to fight me by\l"
	.string "themselves, but you're the last one who\l"
	.string "hasn't passed or dropped out. I'd be\l"
	.string "happy to face you but without a miracle\l"
	.string "this is where your journey ends.$"

TenjinNPC_Text_5:
# 40 "data/maps/Summit/scripts.pory"
	.string "Are you sure you have time for a rematch?$"

TenjinNPC_Text_6:
# 45 "data/maps/Summit/scripts.pory"
	.string "Very well, but don't expect me to go\n"
	.string "easy on you just because you're my\l"
	.string "favourite candidate.$"

TenjinNPC_Text_7:
# 55 "data/maps/Summit/scripts.pory"
	.string "Did you know some prayers work outside\n"
	.string "battle? I think you could transform\l"
	.string "that ditto ust by showing it an image of\l"
	.string "a pokemon. Prepare yourself because I\l"
	.string "won't hold back.$"

TenjinNPC_Text_8:
# 61 "data/maps/Summit/scripts.pory"
	.string "You're an excelent trainer {PLAYER}\n"
	.string "capable of bringing out that dittos\l"
	.string "full potential. But you're a hundred\l"
	.string "years too early to be facing me by\l"
	.string "yourself.$"

TenjinNPC_Text_9:
# 68 "data/maps/Summit/scripts.pory"
	.string "That was fantastic, it been a long time\n"
	.string "since I've felt such thrill in battle.$"

TenjinNPC_Text_10:
# 70 "data/maps/Summit/scripts.pory"
	.string "Are you {PLAYER}? There was another\n"
	.string "candidate waiting around for you\l"
	.string "earlier. They said that you-$"

TenjinNPC_Text_11:
# 78 "data/maps/Summit/scripts.pory"
	.string "I see... well {PLAYER} I understand\n"
	.string "you have a good reason for needing this\l"
	.string "sponsorshi so let me off you some\l"
	.string "advice. Never get in debt to a God. I\l"
	.string "wish we could talk more but if you want\l"
	.string "to pass you need to leave now. You can\l"
	.string "use this shortcut.$"

SlidingTilePuzzle_Text_0:
# 96 "data/maps/Summit/scripts.pory"
	.string "These rocks look breakable...$"

SlidingTilePuzzle_Text_1:
# 104 "data/maps/Summit/scripts.pory"
	.string "A wall of square tiles was revealed$"

SlidingTilePuzzle_Text_2:
# 115 "data/maps/Summit/scripts.pory"
	.string "The image of a pokemon was revealed. Do you want ditto to take its form?$"

SlidingTilePuzzle_Text_3:
# 121 "data/maps/Summit/scripts.pory"
	.string "Ditto took the form of Inkay!$"

SlidingTilePuzzle_Text_4:
# 132 "data/maps/Summit/scripts.pory"
	.string "Ditto took the form of Sandygast!$"
