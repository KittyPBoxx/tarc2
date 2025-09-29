CaveTop_MapScripts::
	.byte 0


CaveSeal::
# 5 "data/maps/CaveTop/scripts.pory"
	lock
# 7 "data/maps/CaveTop/scripts.pory"
	checktrainerflag TRAINER_EXAMINER_RUNE
	goto_if 1, CaveSeal_8
CaveSeal_6:
# 10 "data/maps/CaveTop/scripts.pory"
	checktrainerflag TRAINER_EXAMINER_RUNE
	goto_if 1, CaveSeal_3
# 10 "data/maps/CaveTop/scripts.pory"
	checktrainerflag TRAINER_EXAMINER_DANTE
	goto_if 1, CaveSeal_3
# 14 "data/maps/CaveTop/scripts.pory"
	msgbox CaveSeal_Text_2
CaveSeal_1:
# 17 "data/maps/CaveTop/scripts.pory"
	release
	end

CaveSeal_2:
# 8 "data/maps/CaveTop/scripts.pory"
	msgbox CaveSeal_Text_0
	goto CaveSeal_1

CaveSeal_3:
# 11 "data/maps/CaveTop/scripts.pory"
	msgbox CaveSeal_Text_1
	goto CaveSeal_1

CaveSeal_8:
# 7 "data/maps/CaveTop/scripts.pory"
	checktrainerflag TRAINER_EXAMINER_DANTE
	goto_if 1, CaveSeal_2
	goto CaveSeal_6


CaveSeal_Text_0:
# 8 "data/maps/CaveTop/scripts.pory"
	.string "The seal has been broken. You may pass\n"
	.string "through.$"

CaveSeal_Text_1:
# 11 "data/maps/CaveTop/scripts.pory"
	.string "You must win one more battle to break\n"
	.string "this seal.$"

CaveSeal_Text_2:
# 14 "data/maps/CaveTop/scripts.pory"
	.string "You must have won four battles to\n"
	.string "break this seal.$"
