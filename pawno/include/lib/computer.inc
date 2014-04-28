new IsPlayerComputerUse[MAX_PLAYERS];
new Text:TextDrawComputer[MAX_PLAYERS];
new Text:TextDrawComputerOption1[MAX_PLAYERS];
new Text:TextDrawComputerOption2[MAX_PLAYERS];
new Text:TextDrawComputerOption3[MAX_PLAYERS];

stock ShowPlayerComputer(playerid)
{

}

stock LoadComputerText()
{
	for (new i= 0 ; i < MAX_PLAYERS ; i++)
	{	
		TextDrawComputer[i] = TextDrawCreate(330, 150, "_");
		TextDrawSetSelectable(TextDrawComputer[i],1);
		TextDrawAlignment(TextDrawComputer[i], 2);
		TextDrawBackgroundColor(TextDrawComputer[i], 255);
		TextDrawFont(TextDrawComputer[i], 1);
		TextDrawLetterSize(TextDrawComputer[i], 0.379999, 1.299999);
		TextDrawColor(TextDrawComputer[i], -1);
		TextDrawSetOutline(TextDrawComputer[i], 1);
		TextDrawSetProportional(TextDrawComputer[i], 1);
		TextDrawUseBox(TextDrawComputer[i], 1);
		TextDrawBoxColor(TextDrawComputer[i], 68);
		TextDrawTextSize(TextDrawComputer[i], 540.000000,400.000000);
		
		TextDrawComputerOption1[i] = TextDrawCreate(330, 190, "_");
		TextDrawSetSelectable(TextDrawComputerOption1[i],1);
		TextDrawAlignment(TextDrawComputerOption1[i], 2);
		TextDrawBackgroundColor(TextDrawComputerOption1[i], 255);
		TextDrawFont(TextDrawComputerOption1[i], 1);
		TextDrawLetterSize(TextDrawComputerOption1[i], 0.379999, 1.299999);
		TextDrawColor(TextDrawComputerOption1[i], -1);
		TextDrawSetOutline(TextDrawComputerOption1[i], 1);
		TextDrawSetProportional(TextDrawComputerOption1[i], 1);
		TextDrawUseBox(TextDrawComputerOption1[i], 1);
		TextDrawBoxColor(TextDrawComputerOption1[i], 68);
		TextDrawTextSize(TextDrawComputerOption1[i], 540.000000,400.000000);
		
		TextDrawComputerOption2[i] = TextDrawCreate(330, 210, "_");
		TextDrawSetSelectable(TextDrawComputerOption2[i],1);
		TextDrawAlignment(TextDrawComputerOption2[i], 2);
		TextDrawBackgroundColor(TextDrawComputerOption2[i], 255);
		TextDrawFont(TextDrawComputerOption2[i], 1);
		TextDrawLetterSize(TextDrawComputerOption2[i], 0.379999, 1.299999);
		TextDrawColor(TextDrawComputerOption2[i], -1);
		TextDrawSetOutline(TextDrawComputerOption2[i], 1);
		TextDrawSetProportional(TextDrawComputerOption2[i], 1);
		TextDrawUseBox(TextDrawComputerOption2[i], 1);
		TextDrawBoxColor(TextDrawComputerOption2[i], 68);
		TextDrawTextSize(TextDrawComputerOption2[i], 540.000000,400.000000);
		
		TextDrawComputerOption3[i] = TextDrawCreate(330,230, "_");
		TextDrawSetSelectable(TextDrawComputerOption3[i],1);
		TextDrawAlignment(TextDrawComputerOption3[i], 2);
		TextDrawBackgroundColor(TextDrawComputerOption3[i], 255);
		TextDrawFont(TextDrawComputerOption3[i], 1);
		TextDrawLetterSize(TextDrawComputerOption3[i], 0.379999, 1.299999);
		TextDrawColor(TextDrawComputerOption3[i], -1);
		TextDrawSetOutline(TextDrawComputerOption3[i], 1);
		TextDrawSetProportional(TextDrawComputerOption3[i], 1);
		TextDrawUseBox(TextDrawComputerOption3[i], 1);
		TextDrawBoxColor(TextDrawComputerOption3[i], 68);
		TextDrawTextSize(TextDrawComputerOption3[i], 540.000000,400.000000);
	}
}

stock LoadComputerSystem()
{
	LoadComputerText();
}

COMMAND:pcgui(playerid,params[])
{
	if(IsPlayerComputerUse[playerid] != 0)
	{
		//uzywa, wiec wy³¹cz
		CancelSelectTextDraw(playerid);
		
		TextDrawHideForPlayer(playerid,TextDrawComputer[playerid]);
		TextDrawHideForPlayer(playerid,TextDrawComputerOption1[playerid]);
		TextDrawHideForPlayer(playerid,TextDrawComputerOption2[playerid]);
		TextDrawHideForPlayer(playerid,TextDrawComputerOption3[playerid]);
		
		//RemovePlayerAttachedObject(playerid, SLOT_BOTTLE);
		//SetPlayerSpecialAction(playerid,SPECIAL_ACTION_NONE);
		IsPlayerComputerUse[playerid] = 0;
	}
	else
	{
		//nie u¿ywa wiêc w³¹cz
		new string[256];
		SelectTextDraw(playerid, COLOR_GOLD);
		
		
		format(string, sizeof(string), "~r~Santos ~g~O~y~S~w~ 1.0~n~ Kliknij myszka na wybrany program, lub akcje");
		TextDrawSetString(TextDrawComputer[playerid], string);
		TextDrawShowForPlayer(playerid,TextDrawComputer[playerid]);
		
		format(string, sizeof(string), "~g~Opcja 1: ~w~Otworz komunikator");
		TextDrawSetString(TextDrawComputerOption1[playerid], string);
		TextDrawShowForPlayer(playerid,TextDrawComputerOption1[playerid]);
		
		format(string, sizeof(string), "~g~Opcja 2: ~w~Wlacz przegladarke");
		TextDrawSetString(TextDrawComputerOption2[playerid], string);
		TextDrawShowForPlayer(playerid,TextDrawComputerOption2[playerid]);
		
		format(string, sizeof(string), "~g~Opcja 3: ~w~Wylacz komputer");
		TextDrawSetString(TextDrawComputerOption3[playerid], string);
		TextDrawShowForPlayer(playerid,TextDrawComputerOption3[playerid]);
		
		
		//SetPlayerAttachedObject(playerid, SLOT_BOTTLE, 19848,6 ,0.092000 ,0.001000 ,-0.245000 ,-102.400016 ,2.600036 ,90.299980 ,0.058998 ,0.064999, 0.057999);
		//SetPlayerSpecialAction(playerid,SPECIAL_ACTION_CARRY );
		IsPlayerComputerUse[playerid] = 1;
	}
	return 1;
}