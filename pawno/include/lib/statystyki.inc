new Bar:stamina[MAX_PLAYERS];

new Text:TextDrawGymInfo[MAX_PLAYERS];
new Text:TextDrawFlashbang[MAX_PLAYERS];

#define SKILL_TYPE_STAMINA 1
#define SKILL_TYPE_SHOOTING 2
#define SKILL_TYPE_POWER 3

#define TRAIN_TYPE_HANTLE 1
#define TRAIN_TYPE_STAMINA 2

#define HANTLE_20 1
#define HANTLE_40 2
#define HANTLE_60 3
#define HANTLE_80 4
#define HANTLE_100 5

stock LoadStatsSystem()
{
	LoadBars();
	print("[load] wczytuje system statystyk");
	LoadGymTextDraws();
	LoadFlashbangText();
	return 1;
}

stock LoadBars()
{
	for(new i = 0 ; i < MAX_PLAYERS; i++)
	{
		stamina[i] = CreateProgressBar(503,115,100,_, COLOR_STAMINA, 100);
	}
}

stock DisableCarnet(playerid)
{
	new buffer[256];
	format(buffer,sizeof(buffer),"UPDATE core_items SET item_value1=%i, item_used = 0 WHERE item_uid=%i",PlayerCache[playerid][pKarnetTime],PlayerCache[playerid][pKarnet]);
	mysql_query(buffer);
	
	PlayerCache[playerid][pKarnet] = 0;
	PlayerCache[playerid][pKarnetTime] = 0;
	
	return 1;
}

stock EnableCarnet(playerid,itemuid)
{
	new buffer[256];
	format(buffer,sizeof(buffer),"SELECT item_value1,item_value2 FROM core_items WHERE item_uid=%i",itemuid);
	mysql_query(buffer);
	mysql_store_result();
	
	while(mysql_fetch_row(buffer,"|"))
	{
		sscanf(buffer,"p<|>ii",PlayerCache[playerid][pKarnetTime],PlayerCache[playerid][pKarnetBiznes]);
		PlayerCache[playerid][pKarnet] = itemuid;
	}
	
	mysql_free_result();
	
	format(buffer,sizeof(buffer),"UPDATE core_items SET item_used = 1 WHERE item_uid=%i",PlayerCache[playerid][pKarnet]);
	mysql_query(buffer);
	
	return 1;
}

forward OnPlayerSkillUp(playerid,skilltype,Float:skillvalue);
public OnPlayerSkillUp(playerid,skilltype,Float:skillvalue)
{
	return 1;
}

//sila
stock SetPlayerTrain(playerid,traintype)
{
	if(traintype == TRAIN_TYPE_HANTLE)
	{
		if(!IsPlayerInRangeOfObject(playerid,2915,4))
		{	
			GuiInfo(playerid,"Brak hantli w pobli¿u.");
			return 1;
		}
		
		GuiInfo(playerid,"Wybra³eœ trening hantlami.");
		PlayerCache[playerid][pTrain] = 1;
		new string[256];
		format(string, sizeof(string), "~b~Klawisz sprintu~w~, aby wyciskac ~n~~b~Enter~w~ aby przerwaæ trening", PlayerCache[playerid][pGymVal2]);
		TextDrawSetString(Text:TextDrawGymInfo[playerid], string);
		TextDrawShowForPlayer(playerid, Text:TextDrawGymInfo[playerid]);
			
		ApplyAnimation(playerid, "FREEWEIGHTS", "gym_free_pickup", 4.0, 0, 0, 0, 1, 0, 1);
	
		SetPlayerAttachedObject(playerid, SLOT_GYM, 2915, 6, 0.0, 0.0, -0.45, 0.0, 90.0, 0.0);
			
		PlayerCache[playerid][pGymType] = TRAIN_TYPE_HANTLE;
		PlayerCache[playerid][pGymFaza] = 1;
		PlayerCache[playerid][pGymVal1] = 0;	//kilosy
		PlayerCache[playerid][pGymVal2] = 0;	//powtorzenia
	}
	
	return 1;
}

stock StopPlayerTrain(playerid)
{
	ApplyAnimation(playerid, "FREEWEIGHTS", "gym_free_putdown", 4.0, 0, 0, 0, 0, 0, 1);
	PlayerCache[playerid][pTrain] = 0;
	TextDrawHideForPlayer(playerid, Text:TextDrawGymInfo[playerid]);	
	PlayerCache[playerid][pGymType] = 0;
	PlayerCache[playerid][pGymFaza] = 0;	
	RemovePlayerAttachedObject(playerid,SLOT_GYM);
	return 1;
}

stock LoadFlashbangText()
{
	for(new i = 0 ; i < MAX_PLAYERS; i++)
	{
		TextDrawFlashbang[i] = TextDrawCreate(-20.000000,2.000000,"|");
        TextDrawUseBox(TextDrawFlashbang[i] ,1);
        TextDrawBoxColor(TextDrawFlashbang[i] ,0xFFFFFFff);
        TextDrawTextSize(TextDrawFlashbang[i] ,660.000000,22.000000);
        TextDrawAlignment(TextDrawFlashbang[i] ,0);
        TextDrawBackgroundColor(TextDrawFlashbang[i] ,0xFFFFFFff);
        TextDrawFont(TextDrawFlashbang[i] ,3);
        TextDrawLetterSize(TextDrawFlashbang[i] ,1.000000,52.200000);
        TextDrawColor(TextDrawFlashbang[i] ,0xFFFFFFff);
        TextDrawSetOutline(TextDrawFlashbang[i] ,1);
        TextDrawSetProportional(TextDrawFlashbang[i] ,1);
        TextDrawSetShadow(TextDrawFlashbang[i] ,1);
	}
}

stock LoadGymTextDraws()
{
	for(new i = 0 ; i < MAX_PLAYERS; i++)
	{
		// TextDraw w si³owni (pakernia)
		TextDrawGymInfo[i] = TextDrawCreate(174.000000, 370.000000, "_");
		TextDrawBackgroundColor(TextDrawGymInfo[i], 255);
		TextDrawFont(TextDrawGymInfo[i], 2);
		TextDrawLetterSize(TextDrawGymInfo[i], 0.300000, 1.000000);
		TextDrawColor(TextDrawGymInfo[i], -1);
		TextDrawSetOutline(TextDrawGymInfo[i], 1);
		TextDrawSetProportional(TextDrawGymInfo[i], 1);
		TextDrawUseBox(TextDrawGymInfo[i], 1);
		TextDrawBoxColor(TextDrawGymInfo[i], 68);
		TextDrawTextSize(TextDrawGymInfo[i], 429.000000, 300.000000);
	}
	
	return 1;
}

forward TrainTick(playerid);
public TrainTick(playerid)
{
	if(PlayerCache[playerid][pGymType] == TRAIN_TYPE_HANTLE)
	{
		
		//pobierz kondyche
		if(PlayerCache[playerid][pGymFaza] == 2)
		{
			new Float:getCon = 0;
			if(pInfo[playerid][maxCondition] < 20)
			{
				getCon = pInfo[playerid][maxCondition] * 0.05;
			}
			else if(pInfo[playerid][maxCondition] < 40)
			{
				getCon = pInfo[playerid][maxCondition] * 0.035;
			}
			else if(pInfo[playerid][maxCondition] < 60)
			{
				getCon = pInfo[playerid][maxCondition] * 0.025;
			}
			else if(pInfo[playerid][maxCondition] < 80)
			{
				getCon = pInfo[playerid][maxCondition] * 0.015;
			}
			else
			{
				getCon = pInfo[playerid][maxCondition] * 0.01;
			}
			
			pInfo[playerid][pCondition] = pInfo[playerid][pCondition] - getCon;
			SetProgressBarValue(stamina[playerid], pInfo[playerid][pCondition]);
			UpdateProgressBar(stamina[playerid], playerid);
			
			
			new string[256];
			
			PlayerCache[playerid][pGymFaza] = 1;
			
			switch(PlayerCache[playerid][pGymHantleType])
			{
				case HANTLE_20:
				{
					format(string, sizeof(string), "~b~Klawisz sprintu~w~, aby wyciskac~n~~n~Ciezar: ~y~~h~20 kg~n~~w~Powtorzenia: ~b~~h~%d", PlayerCache[playerid][pGymVal2]);
				}
				case HANTLE_40:
				{
					format(string, sizeof(string), "~w~Klawisz ~b~sprintu~w~, aby wyciskac~n~~n~Ciezar: ~y~~h~40 kg~n~~w~Powtorzenia: ~b~~h~%d", PlayerCache[playerid][pGymVal2]);
				}
				case HANTLE_60:
				{
					format(string, sizeof(string), "~b~Klawisz sprintu~w~, aby wyciskac~n~~n~Ciezar: ~y~~h~60 kg~n~~w~Powtorzenia: ~b~~h~%d", PlayerCache[playerid][pGymVal2]);
				}
				case HANTLE_80:
				{
					format(string, sizeof(string), "~b~Klawisz sprintu~w~, aby wyciskac~n~~n~Ciezar: ~y~~h~80 kg~n~~w~Powtorzenia: ~b~~h~%d", PlayerCache[playerid][pGymVal2]);
				}
				case HANTLE_100:
				{
					format(string, sizeof(string), "~b~Klawisz sprintu~w~, aby wyciskac~n~~n~Ciezar: ~y~~h~100 kg~n~~w~Powtorzenia: ~b~~h~%d", PlayerCache[playerid][pGymVal2]);
				}
			}
			
   			TextDrawSetString(Text:TextDrawGymInfo[playerid], string);
		
	        ApplyAnimation(playerid, "FREEWEIGHTS", "gym_free_down", 4.0, 0, 0, 0, 1, 0, 1);
	        
	        PlayerCache[playerid][pGymVal2] ++;
			
			//tutaj sile dodawac
			
			pInfo[playerid][pPower] = pInfo[playerid][pPower] + PlayerCache[playerid][pGymPowerPerTick];
			
			return 1;
		}
	}
	
	return 1;
}

CMD:trening(playerid,params[])
{
	new doorid = GetPlayerDoorID(playerid);
	new doorOwner = DoorInfo[doorid][doorOwnerUID];
	
	if(PlayerCache[playerid][pKarnet] <= 0)
	{
		GuiInfo(playerid,"Nie posiadasz karnetu w u¿yciu, by móc rozpocz¹æ trening.");
		return 1;
	}
	
	if(PlayerCache[playerid][pKarnetBiznes] != doorOwner)
	{
		GuiInfo(playerid,"Aktualnie wykorzystywany karnet nie obowi¹zuje na tej silowni.");
		return 1;
	}

	/*
		dodac sprawdzanie czy w jego poblizu jest obiekt o id.	
	*/
	
	new type[32], varchar[64];
	if(sscanf(params, "s[32]S()[64]", type, varchar))
	{
		vlive_err(playerid, "/trening [rozpocznij | zakoncz]");
		return 1;
	}
	
	if(!strcmp(type, "zakoncz", true))
	{
		if(PlayerCache[playerid][pTrain] == 1)
		{
			StopPlayerTrain(playerid);
		}
		else
		{
			GuiInfo(playerid,"Nie jesteœ podczas treningu.");
		}
	}
	
	if(!strcmp(type, "rozpocznij", true))
	{
		new tType[32];
		if(sscanf(varchar,"s[32]",tType))
		{
			vlive_err(playerid,"/trening rozpocznij [hantle]");
			return 1;
		}
		
		if(!strcmp(tType, "hantle", true))
		{
			ShowPlayerDialog(playerid,DIAL_SELECT_WEIGHT,DIALOG_STYLE_LIST,"Wybierz ciê¿ar","1. 20 KG\n2. 40 KG\n3. 60 KG\n4. 80 KG\n5. 100KG","Wybierz","Anuluj");
		}
	}
	
	return 1;
}
