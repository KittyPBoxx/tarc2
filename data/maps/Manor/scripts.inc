Manor_MapScripts::
	.byte 0


OrthyNPC::
# 4 "data/maps/Manor/scripts.pory"
	lock
# 5 "data/maps/Manor/scripts.pory"
	faceplayer
# 7 "data/maps/Manor/scripts.pory"
	special GetPlayerMonSpecies
# 8 "data/maps/Manor/scripts.pory"
	waitstate
# 10 "data/maps/Manor/scripts.pory"
	compare VAR_0x8008, SPECIES_MR_RIME
	goto_if_eq OrthyNPC_2
# 18 "data/maps/Manor/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_BERUS
# 19 "data/maps/Manor/scripts.pory"
	msgbox OrthyNPC_Text_1
# 20 "data/maps/Manor/scripts.pory"
	removefieldmugshot
OrthyNPC_1:
# 23 "data/maps/Manor/scripts.pory"
	release
	end

OrthyNPC_2:
# 11 "data/maps/Manor/scripts.pory"
	createfieldmugshot MUGSHOT_TEST, EMOTE_BERUS
# 12 "data/maps/Manor/scripts.pory"
	msgbox OrthyNPC_Text_0
# 13 "data/maps/Manor/scripts.pory"
	removefieldmugshot
# 14 "data/maps/Manor/scripts.pory"
	applymovement 1, MoveOutTheWay
	goto OrthyNPC_1


# 27 "data/maps/Manor/scripts.pory"
MoveOutTheWay:
# 28 "data/maps/Manor/scripts.pory"
	face_left
# 29 "data/maps/Manor/scripts.pory"
	walk_left
# 29 "data/maps/Manor/scripts.pory"
	walk_left
	step_end

OrthyNPC_Text_0:
# 12 "data/maps/Manor/scripts.pory"
	.string "What? No I wasn't napping! Go right in\n"
	.string "big bro.$"

OrthyNPC_Text_1:
# 19 "data/maps/Manor/scripts.pory"
	.string "They'll be a masked ball later where the\n"
	.string "gods reveal which trainers they want to\l"
	.string "sponsor. My big bro said not to let\l"
	.string "anyone in unless he was personally\l"
	.string "escorting them so don't go getting any\l"
	.string "ideas.$"
