EventScript_FieldPoison::
	lockall
	special TryFieldPoisonWhiteOut
	waitstate
	goto_if_eq VAR_RESULT, FLDPSN_WHITEOUT, EventScript_FieldWhiteOut
	releaseall
	end

EventScript_FieldWhiteOut::
	message gText_PlayerWhitedOut
	waitmessage
	waitbuttonpress
	special Script_FadeOutMapMusic
	waitstate
	fadescreen FADE_TO_BLACK
	special SetCB2WhiteOut
	waitstate
	end
