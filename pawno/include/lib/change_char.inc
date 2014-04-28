//
//
//		zjebane, do wypierdolenia
//
//

enum chars
{
	charUID,
	charName[MAX_PLAYER_NAME],
}
new PlayerCharacters[MAX_PLAYER_CHARACTERS][chars];

new Float:PlayerCamX[MAX_PLAYERS];
new Float:PlayerCamY[MAX_PLAYERS];
new Float:PlayerCamZ[MAX_PLAYERS];

forward SetCameraOnPreviousChar(playerid);
forward SetCamerOnNewChar(playerid);
forward NewChangeTimer(playerid);
forward Step1(playerid);
forward Step2(playerid);
forward Step3(playerid);
forward Step4(playerid);
forward Step5(playerid);

stock SetCameraOnPreviousChar(playerid)
{
	pInfo[playerid][pOnChange] = 1;
	SetPlayerWeather(playerid, 30);				//zielona pogoda jak w GTA V
	GetPlayerPos(playerid,PlayerCamX[playerid],PlayerCamY[playerid],PlayerCamZ[playerid]);
	SetPlayerCameraLookAt(playerid, PlayerCamX[playerid],PlayerCamY[playerid],PlayerCamZ[playerid]);
	SetPlayerCameraPos(playerid, PlayerCamX[playerid], PlayerCamY[playerid], PlayerCamZ[playerid]+5);
	TogglePlayerControllable(playerid,0);
	SetTimerEx("Step1", 2000, false, "i", playerid);
}

stock OnSpawnNew(playerid)
{
	SetPlayerName(playerid, "Superman");
	
	CheckPlayerLogged(playerid);
	
	ShowPlayerDialog(playerid,DIAL_L,DIALOG_STYLE_PASSWORD,"Logowanie","Wpisz swoje has³o, aby po³¹czyæ siê z serwerem.","Gotowe","Wyloguj");
	
	SelectedItem[playerid] = -1 ;
	
	GetPlayerGroups(playerid);
	
	FirstSpawn[playerid] = 1;
	
	//pInfo[playerid][pChanged] = 0;
	
	CzyRozmawia[playerid] = 0;
	
	ToWho[playerid] = -1;
	
	GetPlayerBlocks(playerid);
	
	if(pInfo[playerid][blockChar] > 0)
	{
		Kick(playerid);
	}
	
	//GetPlayerGroups(playerid);
	
	SelectedVehicle[playerid] = -1;
	
	SelectedAdminDoors[playerid] = 0;
	
	LoadPlayerObjectTextdraws(playerid);
	
	SpawnedVehicle[playerid] = -1;
	
	NotatnikUID[playerid] = -1;
	
	new hour,minute;
	gettime(hour, minute);
	
	SetPlayerTime(playerid,hour,minute);
	
	PlayerWeapon[playerid] = -1;
	
	GetPlayerVehicles(playerid);
	
	Preloaded[playerid] = 0;
	
	pInfo[playerid][pCuffed] = 0;
	
	setGunSkill(playerid);
	
	SetPlayerNick(playerid);
	
	SetPlayerColor(playerid,COLOR_GREY);
	
	pInfo[playerid][pSID] = playerid;
	
	TextDrawShowForPlayer(playerid, Text:TextDrawSanNews);
	
	TextDrawShowForPlayer(playerid, Text:ServerName);
}

stock ClearThisChar(playerid)
{
	//czyszczenie dotychczasowej
	new query2[256];
	format(query2,sizeof(query2),"UPDATE core_players SET char_logged=0 WHERE char_uid=%i",GetPlayerUID(playerid));
	mysql_query(query2);
	
	ResetPlayerWeapons(playerid);
	
	CzyRozmawia[playerid] = 0;
	
	ToWho[playerid] = -1;
	
	ResetPlayerMoney(playerid);
	
	KillTimer(AjTimer[playerid]);
	
	CleanPlayerTable(playerid);
	
	ClearPlayerVehiclesTable(playerid);
	
	Delete3DTextLabel(PlayerNick[playerid]);
	
	DestroyVehiclesOnDisconnect(playerid);
	
	TextDrawHideForPlayer(playerid, Text:TextDrawSanNews);
	
	TextDrawHideForPlayer(playerid, Text:ServerName);
	
	if(PlayerBwTimer[playerid] > 0)
	{
		new Float:x,Float:y,Float:z;
		GetPlayerPos(playerid,x,y,z);
		new buffer[256];
		format(buffer,sizeof(buffer),"UPDATE core_players SET char_posx=%f, char_posy=%f,char_posz=%f WHERE char_uid=%i",x,y,z,GetPlayerUID(playerid));
		mysql_query(buffer);
	}
}


//momenty przejœcia przez kamerê (prze³ 1)
public Step1(playerid)
{
	SetPlayerCameraPos(playerid, PlayerCamX[playerid], PlayerCamY[playerid], PlayerCamZ[playerid]+25);
	SetPlayerCameraLookAt(playerid, PlayerCamX[playerid],PlayerCamY[playerid],PlayerCamZ[playerid]);
	SetTimerEx("Step2", 2000, false, "i", playerid);
}

public Step2(playerid)
{
	SetPlayerCameraPos(playerid, PlayerCamX[playerid], PlayerCamY[playerid], PlayerCamZ[playerid]+80);
	SetPlayerCameraLookAt(playerid, PlayerCamX[playerid],PlayerCamY[playerid],PlayerCamZ[playerid]);
	SetTimerEx("Step3", 2000, false, "i", playerid);
}

public Step3(playerid)
{
	SetPlayerCameraPos(playerid, PlayerCamX[playerid], PlayerCamY[playerid], PlayerCamZ[playerid]+150);
	SetPlayerCameraLookAt(playerid, PlayerCamX[playerid],PlayerCamY[playerid],PlayerCamZ[playerid]);
	SetTimerEx("Step4", 2000, false, "i", playerid);
}

public Step4(playerid)
{
	SetPlayerCameraPos(playerid, PlayerCamX[playerid], PlayerCamY[playerid], PlayerCamZ[playerid]+300);
	SetPlayerCameraLookAt(playerid, PlayerCamX[playerid],PlayerCamY[playerid],PlayerCamZ[playerid]);
	SetTimerEx("Step5", 2000, false, "i", playerid);
}
public Step5(playerid)
{
	SetPlayerCameraPos(playerid, PlayerCamX[playerid], PlayerCamY[playerid], PlayerCamZ[playerid]+500);
	SetPlayerCameraLookAt(playerid, PlayerCamX[playerid],PlayerCamY[playerid],PlayerCamZ[playerid]);
	//GuiInfo(playerid,"i co kurwo, dalej nie polecimy");
	ClearThisChar(playerid);
	OnSpawnNew(playerid);
}

//momenty przejœcia przez kamerê (prze³ 2)
public Krok1(playerid)
{
	SetPlayerCameraPos(playerid, PlayerCamX[playerid], PlayerCamY[playerid], PlayerCamZ[playerid]+25);
	SetPlayerCameraLookAt(playerid, PlayerCamX[playerid],PlayerCamY[playerid],PlayerCamZ[playerid]);
}

public Krok2(playerid)
{
	SetPlayerCameraPos(playerid, PlayerCamX[playerid], PlayerCamY[playerid], PlayerCamZ[playerid]+80);
	SetPlayerCameraLookAt(playerid, PlayerCamX[playerid],PlayerCamY[playerid],PlayerCamZ[playerid]);
	SetTimerEx("Krok1", 2000, false, "i", playerid);
}

public Krok3(playerid)
{
	SetPlayerCameraPos(playerid, PlayerCamX[playerid], PlayerCamY[playerid], PlayerCamZ[playerid]+150);
	SetPlayerCameraLookAt(playerid, PlayerCamX[playerid],PlayerCamY[playerid],PlayerCamZ[playerid]);
	SetTimerEx("Krok2", 2000, false, "i", playerid);
}

public Krok4(playerid)
{
	SetPlayerCameraPos(playerid, PlayerCamX[playerid], PlayerCamY[playerid], PlayerCamZ[playerid]+300);
	SetPlayerCameraLookAt(playerid, PlayerCamX[playerid],PlayerCamY[playerid],PlayerCamZ[playerid]);
	SetTimerEx("Krok3", 2000, false, "i", playerid);
}
public Krok5(playerid)
{
	SetPlayerCameraPos(playerid, PlayerCamX[playerid], PlayerCamY[playerid], PlayerCamZ[playerid]+500);
	SetPlayerCameraLookAt(playerid, PlayerCamX[playerid],PlayerCamY[playerid],PlayerCamZ[playerid]);
	SetTimerEx("Krok4", 2000, false, "i", playerid);
}

COMMAND:postac(playerid,params[])
{
	if(pInfo[playerid][pChanged] == 1)
	{
		GuiInfo(playerid,"Przed chwil¹ prze³¹cza³eœ siê na inn¹ postaæ, poczekaj chwilê zanim prze³¹czysz siê ponownie...");
		return 1;
	}
	new buffer[256];
	format(buffer,sizeof(buffer),"SELECT char_uid,char_name FROM core_players WHERE char_gid=%i",pInfo[playerid][pUID]);
	mysql_query(buffer);
	mysql_store_result();
	if(mysql_num_rows() > MAX_PLAYER_CHARACTERS)
	{
		GuiInfo(playerid,"Error CODE 1097 : Zg³oœ siê do administratora celem wyjaœnienia problemu.");
		return 1;
	}
	else
	{
		//git
		new num = 0,charlist[512];
		while(mysql_fetch_row(buffer,"|"))
		{
			sscanf(buffer,"p<|>is[32]",PlayerCharacters[num][charUID],PlayerCharacters[num][charName]);
			format(charlist,sizeof(charlist),"%s\n%i\t\t%s",charlist,PlayerCharacters[num][charUID],PlayerCharacters[num][charName]);
			num++;
		}
		ShowPlayerDialog(playerid,DIAL_POSTACIE,DIALOG_STYLE_LIST,"Lista Twoich postaci",charlist,"Wybierz","Zamknij");
	}
	mysql_free_result();
	return 1;
}

public NewChangeTimer(playerid)
{
	pInfo[playerid][pChanged] = 0;
}