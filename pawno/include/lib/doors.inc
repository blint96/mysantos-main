//
//
//			drzwi

#define MAX_DOORS 1000
#define PICKUPS_STREAM_DISTANCE 100.0
#define DEFAULT_PICKUP_TYPE 2

#define DOOR_TYPE_NONE 0
#define DOOR_TYPE_GROUP 1
#define DOOR_TYPE_PLAYER 2
#define DOOR_TYPE_247	3					//drzwi typu 24/7 h
#define DOOR_TYPE_BANK	4			//drzwi typu bank
#define DOOR_TYPE_UBRANIA	5
#define DOOR_TYPE_ATTACHMENTS	6
#define DOOR_TYPE_HOTEL 7
#define DOOR_TYPE_APTEKA 8

#define NEW_DOOR_PICKUP 1318

#define MAX_SCHOWEK	100

enum dpars
{
	doorSampID,
	doorUID,
	doorType,
	doorOwnerType,
	doorOwnerUID,
	doorModel,
	doorName[64],
	doorMuzyka[128],				//link do muzy
	doorEntry,					//koszt wejscia
	doorClose,
	doorCars,
	Float:doorEnterX,
	Float:doorEnterY,
	Float:doorEnterZ,
	Float:doorEnterA,
	Float:doorExitX,
	Float:doorExitY,
	Float:doorExitZ,
	Float:doorExitA,
	doorEnterVW,
	doorEnterINT,
	doorExitVW,
	doorExitINT,
	doorObjectsMax,
}
new DoorInfo[MAX_DOORS][dpars];


forward InitDoors();
forward LoadDoors();
forward EnterDoorIfCuffed(playerid);


new Text:TextDrawDoorInfo[MAX_PLAYERS];


public InitDoors()
{
	printf("[doors] Skrypt zaladowany.");
	
	LoadDoors();
	
	LoadDoorsText();
	
	return 1;
}

public LoadDoors()
{
	print("[doors] Rozpoczynam wczytywanie drzwi...");
	new liczba = 0;
	new buffer[512];
	format(buffer,sizeof(buffer),"SELECT * FROM doors");
	mysql_query(buffer);
	mysql_store_result();
	while(mysql_fetch_row(buffer,"|"))
	{
		new doorid = CheckForFreeDoorSlot();
		sscanf(buffer,"p<|>iiiiis[64]s[128]iiiffffffffiiiii",
		DoorInfo[doorid][doorUID],
		DoorInfo[doorid][doorType],
		DoorInfo[doorid][doorOwnerType],
		DoorInfo[doorid][doorOwnerUID],
		DoorInfo[doorid][doorModel],
		DoorInfo[doorid][doorName],
		DoorInfo[doorid][doorMuzyka],
		DoorInfo[doorid][doorEntry],
		DoorInfo[doorid][doorClose],
		DoorInfo[doorid][doorCars],
		DoorInfo[doorid][doorEnterX],
		DoorInfo[doorid][doorEnterY],
		DoorInfo[doorid][doorEnterZ],
		DoorInfo[doorid][doorEnterA],
		DoorInfo[doorid][doorExitX],
		DoorInfo[doorid][doorExitY],
		DoorInfo[doorid][doorExitZ],
		DoorInfo[doorid][doorExitA],
		DoorInfo[doorid][doorEnterVW],
		DoorInfo[doorid][doorEnterINT],
		DoorInfo[doorid][doorExitVW],
		DoorInfo[doorid][doorExitINT],
		DoorInfo[doorid][doorObjectsMax]);
		
		DoorInfo[doorid][doorSampID] = CreateDynamicPickup(DoorInfo[doorid][doorModel], DEFAULT_PICKUP_TYPE, DoorInfo[doorid][doorEnterX], DoorInfo[doorid][doorEnterY],DoorInfo[doorid][doorEnterZ], DoorInfo[doorid][doorEnterVW], -1, -1, PICKUPS_STREAM_DISTANCE);
		//printf("%i , %f , %f , %f , %i , D: %i ",DoorInfo[doorid][doorSampID] , DoorInfo[doorid][doorEnterX], DoorInfo[doorid][doorEnterY],DoorInfo[doorid][doorEnterZ], DoorInfo[doorid][doorEnterVW],DEFAULT_PICKUP_TYPE);
		liczba++;
	}
	mysql_free_result();
	
	printf("[doors] Pomyœlnie za³adowano %i drzwi",liczba);
	return 1;
}


public EnterDoorIfCuffed(playerid)
{
	for (new i = 0 ; i< MAX_DOORS; i ++ )
	{
		if(IsPlayerInRangeOfPoint(playerid,2.0,DoorInfo[i][doorEnterX],DoorInfo[i][doorEnterY],DoorInfo[i][doorEnterZ]))
		{
			if(DoorInfo[i][doorClose] == 0)
			{
				SetPlayerPos(playerid,DoorInfo[i][doorExitX],DoorInfo[i][doorExitY],DoorInfo[i][doorExitZ]);
				SetPlayerVirtualWorld(playerid,DoorInfo[i][doorExitVW]);
				SetPlayerFacingAngle( playerid, DoorInfo[i][doorExitA]);
				OnEnterToDoor(playerid);
			}
			else
			{
				GameTextForPlayer(playerid, "~r~Drzwi sa zamkniete", 5000, 5);
			}
		}
		else if(IsPlayerInRangeOfPoint(playerid,2.0,DoorInfo[i][doorExitX],DoorInfo[i][doorExitY],DoorInfo[i][doorExitZ]))
		{
			if(DoorInfo[i][doorClose] == 0)
			{
				SetPlayerPos(playerid,DoorInfo[i][doorEnterX],DoorInfo[i][doorEnterY],DoorInfo[i][doorEnterZ]);
				SetPlayerVirtualWorld(playerid,DoorInfo[i][doorEnterVW]);
				SetPlayerFacingAngle( playerid, DoorInfo[i][doorEnterA]);
				OnEnterToDoor(playerid);
			}
			else
			{
				GameTextForPlayer(playerid, "~r~Drzwi sa zamkniete", 5000, 5);
			}
		}
	}
}

public OnPlayerPickUpDynamicPickup(playerid, pickupid)
{
	new enter_pay[64],lock_text[128],string[256];
	if(DoorInfo[pickupid][doorEntry] > 0)
	{
		format(enter_pay,sizeof(enter_pay),"~w~Koszt wstepu : ~g~%i$",DoorInfo[pickupid][doorEntry]);
	}
	if(DoorInfo[pickupid][doorClose] == 1)
	{
		format(lock_text,sizeof(lock_text),"~r~Drzwi sa zamkniete.");
	}
	else
	{
		 format(lock_text, sizeof(lock_text), "~y~Aby wejsc, wcisnij jednoczesnie~n~~w~[~b~~h~~h~~k~~SNEAK_ABOUT~ + ~k~~PED_SPRINT~~w~]");
	}
	format(string, sizeof(string), "%s (%i)~n~%s~n~%s", DoorInfo[pickupid][doorName],pickupid, enter_pay, lock_text);
	TextDrawSetString(Text:TextDrawDoorInfo[playerid], string);
	TextDrawShowForPlayer(playerid, Text:TextDrawDoorInfo[playerid]);
	SetTimerEx("HideDoorText", 5000, false, "i", playerid);
	return 1;
}

stock OnEnterToDoor(playerid)
{
	TogglePlayerControllable(playerid,0);
	SetTimerEx("FreezeDoor", 2500, false, "i", playerid);
}

forward FreezeDoor(playerid);
public FreezeDoor(playerid)
{
	TogglePlayerControllable(playerid,1);
}

forward HideDoorText(playerid);
public HideDoorText(playerid)
{
	TextDrawHideForPlayer(playerid, Text:TextDrawDoorInfo[playerid]);
}

/*stock GetPlayerDoorID(playerid)
{
	new vw = GetPlayerVirtualWorld(playerid);
	if(vw != 0)
	{
		for(new i=0;i<MAX_DOORS;i++)
		{
			if(DoorInfo[i][doorExitVW] == vw)
			{
				return i;
			}
			else if(DoorInfo[i][doorEnterVW] == vw)
			{
				return i;
			}
		}
	}
	return 0;
}*/

stock GetPlayerDoorID(playerid)
{
	new doorid = -1;
	for(new i = 0 ; i < MAX_DOORS; i++)
	{
		if(GetPlayerVirtualWorld(playerid) == DoorInfo[i][doorExitVW])
		{
			// tu bylo i
			doorid = i;
			break;
		}
	}	
	return doorid;
}

forward ReloadDoors();
public ReloadDoors()
{
	for(new i= 0 ; i < MAX_DOORS;i++)
	{
		UnloadDoor(i);
	}
	LoadDoors();
	return 1;
}

stock GetDoorIDbyUID(dooruid)
{
	new doorid = -1;
	for(new i = 0 ; i < MAX_DOORS; i++)
	{
		if(DoorInfo[i][doorUID] == dooruid)
		{
			doorid = i;
			break;
		}
	}
	
	return doorid;
}

stock LoadDoorsText()
{
	for (new i= 0 ; i < MAX_PLAYERS ; i++)
	{
		//info o drzwiach
		TextDrawDoorInfo[i] = TextDrawCreate(326.000000, 330.000000, "_");
		TextDrawAlignment(TextDrawDoorInfo[i], 2);
		TextDrawBackgroundColor(TextDrawDoorInfo[i], 255);
		TextDrawFont(TextDrawDoorInfo[i], 1);
		TextDrawLetterSize(TextDrawDoorInfo[i], 0.379999, 1.299999);
		TextDrawColor(TextDrawDoorInfo[i], -1);
		TextDrawSetOutline(TextDrawDoorInfo[i], 1);
		TextDrawSetProportional(TextDrawDoorInfo[i], 1);
		TextDrawUseBox(TextDrawDoorInfo[i], 1);
		TextDrawBoxColor(TextDrawDoorInfo[i], 68);
		TextDrawTextSize(TextDrawDoorInfo[i], 490.000000, 221.000000);
		
		//textdraw kar
		TextDrawPunishInfo = TextDrawCreate(14.000000, 286.000000, "_");
		TextDrawBackgroundColor(TextDrawPunishInfo, 255);
		TextDrawFont(TextDrawPunishInfo, 1);
		TextDrawLetterSize(TextDrawPunishInfo, 0.300000, 1.200000);
		TextDrawColor(TextDrawPunishInfo, -1);
		TextDrawSetOutline(TextDrawPunishInfo, 1);
		TextDrawSetProportional(TextDrawPunishInfo, 1);
		
		TextDrawHideForAll(Text:TextDrawPunishInfo);
	}
	
	//admin log
	TextDrawAdminLog = TextDrawCreate(125.000000, 423.000000, "_");
	TextDrawBackgroundColor(TextDrawAdminLog, 255);
	TextDrawFont(TextDrawAdminLog, 1);
	TextDrawLetterSize(TextDrawAdminLog, 0.200000, 0.899999);
	TextDrawColor(TextDrawAdminLog, -1);
	TextDrawSetOutline(TextDrawAdminLog, 1);
	TextDrawSetProportional(TextDrawAdminLog, 1);
	
	//textdraw kar
	TextDrawPunishInfo = TextDrawCreate(14.000000, 286.000000, "_");
	TextDrawBackgroundColor(TextDrawPunishInfo, 255);
	TextDrawFont(TextDrawPunishInfo, 1);
	TextDrawLetterSize(TextDrawPunishInfo, 0.300000, 1.200000);
	TextDrawColor(TextDrawPunishInfo, -1);
	TextDrawSetOutline(TextDrawPunishInfo, 1);
	TextDrawSetProportional(TextDrawPunishInfo, 1);		
	TextDrawHideForAll(Text:TextDrawPunishInfo);
	
	//san news
	TextDrawSanNews = TextDrawCreate(2.000000, 438.000000, "San News ~>~ W radiu nadawany standardowy blok muzyczny.");
	TextDrawBackgroundColor(TextDrawSanNews, 255);
	TextDrawFont(TextDrawSanNews, 1);
	TextDrawLetterSize(TextDrawSanNews, 0.250000, 0.899999);
	TextDrawColor(TextDrawSanNews, -1);
	TextDrawSetOutline(TextDrawSanNews, 1);
	TextDrawSetProportional(TextDrawSanNews, 1);
	TextDrawUseBox(TextDrawSanNews, 1);
	TextDrawBoxColor(TextDrawSanNews, 68);
	TextDrawTextSize(TextDrawSanNews, 640.000000, 600.000000);
	
	TextDrawShowForAll(TextDrawPunishInfo);
	// @ up wtf?!
	
	//servername
	new string_name[64], dzien[8], mies[8];
	new day2, mies2, year2;
	getdate(year2, mies2, day2);
	if(day2 < 10) format(dzien, sizeof(dzien), "0%d", day2);
	else format(dzien, sizeof(dzien), "%d", day2);
	if(mies2 < 10) format(mies, sizeof(mies), "0%d", mies2);
	else format(mies, sizeof(mies), "%d", mies2);
	format(string_name, sizeof(string_name), "~y~my~b~~h~Santos.~y~pl ~h~%s.%s", dzien[0], mies[0]);
	// Text Draw's
	//ServerName = Text:TextDrawCreate(35.000000, 425.000000,string_name);
	ServerName = Text:TextDrawCreate(492,6,string_name);
	TextDrawFont(Text:ServerName, 3);
	TextDrawColor(Text:ServerName, COLOR_GREY);
	TextDrawSetOutline(Text:ServerName, 1);
	TextDrawLetterSize(Text:ServerName, 0.40, 0.90);
	TextDrawSetProportional(Text:ServerName, 1);
}

stock CheckForFreeDoorSlot()
{
	new wolnyslot;
	for(new i=1;i< MAX_DOORS; i++)
	{
		if(DoorInfo[i][doorUID] <= 0)
		{
			wolnyslot = i;
			break;
		}
	}
	return wolnyslot;
}

stock LoadDoor(uid)
{
	new buffer[256];
	format(buffer,sizeof(buffer),"SELECT * FROM doors WHERE uid=%i",uid);
	mysql_query(buffer);
	mysql_store_result();
	while(mysql_fetch_row(buffer,"|"))
	{
		new doorid = CheckForFreeDoorSlot();
		sscanf(buffer,"p<|>iiiiis[64]s[128]iiiffffffffiiiii",
		DoorInfo[doorid][doorUID],
		DoorInfo[doorid][doorType],
		DoorInfo[doorid][doorOwnerType],
		DoorInfo[doorid][doorOwnerUID],
		DoorInfo[doorid][doorModel],
		DoorInfo[doorid][doorName],
		DoorInfo[doorid][doorMuzyka],
		DoorInfo[doorid][doorEntry],
		DoorInfo[doorid][doorClose],
		DoorInfo[doorid][doorCars],
		DoorInfo[doorid][doorEnterX],
		DoorInfo[doorid][doorEnterY],
		DoorInfo[doorid][doorEnterZ],
		DoorInfo[doorid][doorEnterA],
		DoorInfo[doorid][doorExitX],
		DoorInfo[doorid][doorExitY],
		DoorInfo[doorid][doorExitZ],
		DoorInfo[doorid][doorExitA],
		DoorInfo[doorid][doorEnterVW],
		DoorInfo[doorid][doorEnterINT],
		DoorInfo[doorid][doorExitVW],
		DoorInfo[doorid][doorExitINT],
		DoorInfo[doorid][doorObjectsMax]);
		
		DoorInfo[doorid][doorSampID] = CreateDynamicPickup(DoorInfo[doorid][doorModel], DEFAULT_PICKUP_TYPE, DoorInfo[doorid][doorEnterX], DoorInfo[doorid][doorEnterY],DoorInfo[doorid][doorEnterZ], DoorInfo[doorid][doorEnterVW], -1, -1, PICKUPS_STREAM_DISTANCE);
		//printf("%i , %f , %f , %f , %i , D: %i ",DoorInfo[doorid][doorSampID] , DoorInfo[doorid][doorEnterX], DoorInfo[doorid][doorEnterY],DoorInfo[doorid][doorEnterZ], DoorInfo[doorid][doorEnterVW],DEFAULT_PICKUP_TYPE);
	}
	mysql_free_result();
}

forward RotateCameraIndoor(playerid);
public RotateCameraIndoor(playerid)
{
	new Float:x = CamSWATIndoorPos[playerid][0] ;
	new Float:y = CamSWATIndoorPos[playerid][1]; 
	new Float:z = CamSWATIndoorPos[playerid][2]; 
	
	if(PlayerCache[playerid][pUseSWATCam] > 0)
	{
		SetPlayerCameraPos(playerid, x,y,z);
		SetPlayerCameraLookAt(playerid, x+random(5),y+random(5),z);
		SetTimerEx("RotateCameraIndoor",1000,false,"i",playerid);
	}
	
	return 1;
}

stock UnloadDoor(sampid)
{
	DoorInfo[sampid][doorEnterX] = 0;
	DoorInfo[sampid][doorEnterY] = 0;
	DoorInfo[sampid][doorEnterZ] = 0;
	DoorInfo[sampid][doorExitX] = 0;
	DoorInfo[sampid][doorExitY] = 0;
	DoorInfo[sampid][doorExitZ] = 0;
	DestroyDynamicPickup(sampid);
	DoorInfo[sampid][doorSampID] = EOS;
	DoorInfo[sampid][doorUID] = EOS;
}

stock ChangeDoorName(doorid,newname[])
{
	new buffer[256],test[64];
	format(buffer,sizeof(buffer),"UPDATE doors SET name='%s' WHERE uid=%i",newname,DoorInfo[doorid][doorUID]);
	mysql_query(buffer);
	format(test,sizeof(test),"%s",newname);
	DoorInfo[doorid][doorName] = test;
}

stock IsPlayerPermsDoors(playerid,doorid)
{
	if(DoorInfo[doorid][doorOwnerType] == DOOR_TYPE_PLAYER)
	{
		if(DoorInfo[doorid][doorOwnerUID] == pInfo[playerid][pUID])
		{
			return 1;
		}
	}
	else if(DoorInfo[doorid][doorOwnerType] == DOOR_TYPE_GROUP)
	{
		for(new gid=1; gid < MAX_PLAYER_GROUPS; gid++)
		{
			if(pGrupa[playerid][gid][pGuid] == DoorInfo[doorid][doorOwnerUID])
			{
				if(pGrupa[playerid][gid][permDoors] == 1)
				{
					return 1;
				}
			}
		}
	}

	return 0;
}

stock IsPlayerPermsDoorsLeader(playerid,doorid)
{
	if(DoorInfo[doorid][doorOwnerType] == DOOR_TYPE_PLAYER)
	{
		if(DoorInfo[doorid][doorOwnerUID] == pInfo[playerid][pUID])
		{
			return 1;
		}
	}
	else if(DoorInfo[doorid][doorOwnerType] == DOOR_TYPE_GROUP)
	{
		for(new gid=1; gid < MAX_PLAYER_GROUPS; gid++)
		{
			if(pGrupa[playerid][gid][pGuid] == DoorInfo[doorid][doorOwnerUID])
			{
				if(pGrupa[playerid][gid][permLeader] == 1)
				{
					return 1;
				}
			}
		}
	}

	return 0;
}

COMMAND:adrzwi(playerid,params[])
{
	if(CheckPlayerAccess(playerid) >= PERMS_GAMEMASTER)
	{
		if(pInfo[playerid][pGMDoors] != 1)
		{
			GuiInfo(playerid,"Brak uprawnieñ.");
			return 1;
		}
		
		new type[32], varchar[64];
		if(sscanf(params, "s[32]S()[64]", type, varchar))
		{
			vlive_err(playerid, "/adrzwi [stworz | wyjscie | wejscie | pickup | info | usun | nazwa | przypisz | zamknij | goto | lista | przejazd]");
			return 1;
		}
		
		if(!strcmp(type, "przejazd", true))
		{
			new doorid,string[16];
			if(sscanf(varchar, "is[16]", doorid,string))
			{
				vlive_err(playerid, "/adrzwi przejazd [doorid] [wlacz | wylacz]");
				return 1;
			}
			new dooruid = DoorInfo[doorid][doorUID];
			
			new buffer[256];
			if(!strcmp(string, "wlacz", true))
			{
				format(buffer,sizeof(buffer),"UPDATE doors SET cars = 1 WHERE uid=%i",DoorInfo[doorid][doorUID]);
				mysql_query(buffer);
			}
			if(!strcmp(string, "wylacz", true))
			{
				format(buffer,sizeof(buffer),"UPDATE doors SET cars = 0 WHERE uid=%i",DoorInfo[doorid][doorUID]);
				mysql_query(buffer);
			}
			
			UnloadDoor(doorid)	;
			LoadDoor(dooruid)	;		
			
			GuiInfo(playerid,"Zmieni³eœ status przejazdu w drzwiach.");
		}
		
		if(!strcmp(type, "stworz", true))
		{
			new doorname[64];
			if(sscanf(varchar, "s[32]", doorname))
			{
				vlive_err(playerid, "/adrzwi stworz [Nazwa drzwi]");
				return 1;
			}
			
			new Float:EnterX, Float:EnterY, Float:EnterZ, Float:EnterA;
			GetPlayerPos(playerid, EnterX, EnterY, EnterZ);
			GetPlayerFacingAngle(playerid, EnterA);
			
			new worldid = GetPlayerVirtualWorld(playerid);
			SendClientMessage(playerid,COLOR_GOLD,"[doors] Stworzy³eœ nowe drzwi.");
			
			new buffer[256];
			new interior = GetPlayerInterior(playerid);
			format(buffer,sizeof(buffer),"INSERT INTO doors (type,model,name,close,enterx,entery,enterz,entera,entervw,enterint) VALUES(%i,%i,'%s',%i,%f,%f,%f,%f,%i,%i)",DEFAULT_PICKUP_TYPE,NEW_DOOR_PICKUP,doorname,1,EnterX,EnterY,EnterZ,EnterA,worldid,interior);
			mysql_query(buffer);
			
			format(buffer,sizeof(buffer),"SELECT MAX(uid) FROM doors");
			mysql_query(buffer);
			mysql_store_result();
			new door_uid = mysql_fetch_int();
			mysql_free_result();
			
			format(buffer,sizeof(buffer),"SELECT * FROM doors WHERE uid=%i",door_uid);
			mysql_query(buffer);
			mysql_store_result();
			new loguid = 0;
			while(mysql_fetch_row(buffer,"|"))
			{
				new doorid = CheckForFreeDoorSlot();
				sscanf(buffer,"p<|>iiiiis[64]s[128]iiiffffffffiiiii",
				DoorInfo[doorid][doorUID],
				DoorInfo[doorid][doorType],
				DoorInfo[doorid][doorOwnerType],
				DoorInfo[doorid][doorOwnerUID],
				DoorInfo[doorid][doorModel],
				DoorInfo[doorid][doorName],
				DoorInfo[doorid][doorMuzyka],
				DoorInfo[doorid][doorEntry],
				DoorInfo[doorid][doorClose],
				DoorInfo[doorid][doorCars],
				DoorInfo[doorid][doorEnterX],
				DoorInfo[doorid][doorEnterY],
				DoorInfo[doorid][doorEnterZ],
				DoorInfo[doorid][doorEnterA],
				DoorInfo[doorid][doorExitX],
				DoorInfo[doorid][doorExitY],
				DoorInfo[doorid][doorExitZ],
				DoorInfo[doorid][doorExitA],
				DoorInfo[doorid][doorEnterVW],
				DoorInfo[doorid][doorEnterINT],
				DoorInfo[doorid][doorExitVW],
				DoorInfo[doorid][doorExitINT],
				DoorInfo[doorid][doorObjectsMax]);
				
				DoorInfo[doorid][doorSampID] = CreateDynamicPickup(DoorInfo[doorid][doorModel], DEFAULT_PICKUP_TYPE, DoorInfo[doorid][doorEnterX], DoorInfo[doorid][doorEnterY],DoorInfo[doorid][doorEnterZ], DoorInfo[doorid][doorEnterVW], -1, -1, PICKUPS_STREAM_DISTANCE);
				loguid = doorid;
			}
			mysql_free_result();
			
			new adminLog[128];
			format(adminLog,sizeof(adminLog),"~r~[drzwi]~y~ Administrator %s ~g~(UID: %i | SID: %i)~y~ stworzyl drzwi %s (UID: %i | SID: %i)",pInfo[playerid][pName],GetPlayerUID(playerid),playerid,doorname,door_uid,loguid);
			LogAdminAction(adminLog);
		
		}
		
		if(!strcmp(type, "wyjscie", true))
		{
			new doorid;
			if(sscanf(varchar, "i", doorid))
			{
				vlive_err(playerid, "/adrzwi wyjscie [doorid]");
				return 1;
			}
			
			new buffer[256],Float:x,Float:y,Float:z,Float:a;
			GetPlayerPos(playerid,x,y,z);
			GetPlayerFacingAngle(playerid, a);
			
			new worldid = GetPlayerVirtualWorld(playerid);
			new interior = GetPlayerInterior(playerid);
			
			
			if(DoorInfo[doorid][doorUID] > 0)
			{		
				format(buffer,sizeof(buffer),"UPDATE doors SET exitx=%f,exity=%f,exitz=%f,exita=%f,exitvw=%i,exitint=%i WHERE uid=%i",x,y,z,a,worldid,interior,DoorInfo[doorid][doorUID]);
				mysql_query(buffer);
				SendClientMessage(playerid,COLOR_GOLD,"[doors] Zmieni³eœ wyjœcie drzwi.");
				//reset drzwi
				
				new adminlog[128];
				format(adminlog,sizeof(adminlog),"Administrator %s zmieni³ wyjœcie drzwi %s (%i)",pInfo[playerid][pName],DoorInfo[doorid][doorName],DoorInfo[doorid][doorSampID]);
				vlive_AdminLog(adminlog);
				
				new load = DoorInfo[doorid][doorUID];
				UnloadDoor(DoorInfo[doorid][doorSampID]);
				LoadDoor(load);
			}
			else
			{
				vlive_err(playerid,"takie drzwi nie istniej¹");
			}
		}
		
		if(!strcmp(type, "wejscie", true))
		{
			new doorid;
			if(sscanf(varchar, "i", doorid))
			{
				vlive_err(playerid, "/adrzwi wejscie [doorid]");
				return 1;
			}
			
			if(DoorInfo[doorid][doorUID] > 0)
			{
				new buffer[256],Float:x,Float:y,Float:z,Float:a;
				GetPlayerPos(playerid,x,y,z);
				GetPlayerFacingAngle(playerid, a);
				
				new worldid = GetPlayerVirtualWorld(playerid);
				new interior = GetPlayerInterior(playerid);
				
				format(buffer,sizeof(buffer),"UPDATE doors SET enterx=%f,entery=%f,enterz=%f,entera=%f,entervw=%i,enterint=%i WHERE uid=%i",x,y,z,a,worldid,interior,DoorInfo[doorid][doorUID]);
				mysql_query(buffer);
				
				new adminlog[128];
				format(adminlog,sizeof(adminlog),"Administrator %s zmieni³ wejœcie drzwi %s (%i)",pInfo[playerid][pName],DoorInfo[doorid][doorName],DoorInfo[doorid][doorSampID]);
				vlive_AdminLog(adminlog);
				
				new load = DoorInfo[doorid][doorUID];
				UnloadDoor(DoorInfo[doorid][doorSampID]);
				LoadDoor(load);
			}
			else
			{
				vlive_err(playerid,"nie ma takich drzwi");
			}
		}
		
		if(!strcmp(type, "pickup", true))
		{
			new doorid;
			if(sscanf(varchar, "i", doorid))
			{
				vlive_err(playerid, "/adrzwi pickup [doorid]");
				return 1;
			}
			if(DoorInfo[doorid][doorUID] > 0)
			{
				SelectedAdminDoors[playerid] = doorid;
				ShowPlayerDialog(playerid,DIAL_PICKUP,DIALOG_STYLE_LIST,"Wybierz pickup dla drzwi","1.  Dolar\n2. Informacja\n3. Serce\n4. Niebieski domek\n5. Zielony domek\n6. Bia³a strza³ka\n7. Gwiazdka","Wybierz","Anuluj");
			}
			else
			{
				vlive_err(playerid,"takie drzwi nie istniej¹");
			}
		}
		
		if(!strcmp(type, "usun", true))
		{
			new doorid;
			if(sscanf(varchar, "i", doorid))
			{
				vlive_err(playerid, "/adrzwi usun [doorid]");
				return 1;
			}
			if(DoorInfo[doorid][doorUID] > 0)
			{
				new buffer[256];
				format(buffer,sizeof(buffer),"DELETE FROM doors WHERE uid=%i",DoorInfo[doorid][doorUID]);
				mysql_query(buffer);
				format(buffer,sizeof(buffer),"DELETE FROM core_items WHERE item_owneruid=%i AND item_whereis=%i",DoorInfo[doorid][doorUID],3);
				mysql_query(buffer);
				
				new adminlog[128];
				format(adminlog,sizeof(adminlog),"Administrator %s usun¹³ drzwi o nazwie %s (%i)",pInfo[playerid][pName],DoorInfo[doorid][doorName],DoorInfo[doorid][doorSampID]);
				vlive_AdminLog(adminlog);
				
				new adminLog[128];
				format(adminLog,sizeof(adminLog),"~r~[drzwi]~y~ Administrator %s ~g~(UID: %i | SID: %i)~y~ usuwa drzwi %s (UID: %i)",pInfo[playerid][pName],GetPlayerUID(playerid),playerid,DoorInfo[doorid][doorName],DoorInfo[doorid][doorUID]);
				LogAdminAction(adminLog);
				
				UnloadDoor(DoorInfo[doorid][doorSampID]);
				
				SendClientMessage(playerid,COLOR_GOLD,"[doors] Usun¹³eœ drzwi.");
			}
			else
			{
				vlive_err(playerid,"takie drzwi nie istniej¹");
			}
		}
		
		if(!strcmp(type, "nazwa", true))
		{
			new doorid,nazwa[64];
			if(sscanf(varchar, "is[64]", doorid,nazwa))
			{
				vlive_err(playerid, "/adrzwi nazwa [doorid] [nazwa]");
				return 1;
			}
			if(DoorInfo[doorid][doorUID] > 0)
			{
				ChangeDoorName(doorid,nazwa);
				new ainfo[256];
				format(ainfo,sizeof(ainfo),"Zmieni³eœ nazwê drzwi (SampID: %i) na: %s",DoorInfo[doorid][doorSampID],nazwa);
				GuiInfo(playerid,ainfo);
			}
			else
			{
				vlive_err(playerid,"takie drzwi nie istniej¹");
			}
		}
		
		if(!strcmp(type, "przypisz", true))
		{
			new doorid,typee,owner;
			if(sscanf(varchar, "iii", doorid,typee,owner))
			{
				vlive_err(playerid, "/adrzwi przypisz [doorid] [type 2=gracz | 1 = grupa] [gid/playerid]");
				return 1;
			}
			
			if (typee == DOOR_TYPE_PLAYER)
			{
				new gracz = -1;
				for(new i = 0 ; i < MAX_PLAYERS;i++)
				{
					if(IsPlayerConnect(i))
					{
						if(pInfo[i][pUID] == owner)
						{
							gracz = i;
							break;
						}
					}
				}
				
				if(DoorInfo[doorid][doorUID] <= 0)
				{
					vlive_err(playerid,"takie drzwi nie istniej¹");
					return 1;
				}
				
				//przypisz
				new buffer[256];
				format(buffer,sizeof(buffer),"UPDATE doors SET otype=%i, ouid=%i WHERE uid=%i",DOOR_TYPE_PLAYER,GetPlayerUID(gracz),DoorInfo[doorid][doorUID]);
				mysql_query(buffer);
					
				new output[126];
				format(output,sizeof(output),"Przypisa³eœ drzwi %s (ID:%i) pod gracza %s (ID:%i)",DoorInfo[doorid][doorName],doorid,pInfo[gracz][pName],gracz);
				GuiInfo(playerid,output);
				GuiInfo(gracz,"Administrator przypisa³ drzwi pod Ciebie.");
				
				new adminLog[128];
				format(adminLog,sizeof(adminLog),"~r~[drzwi]~y~ Administrator %s ~g~(UID: %i | SID: %i)~y~ przepisal drzwi %s ~g~(UID: %i)~y~ pod gracza %s ~g~(ID: %i)",pInfo[playerid][pName],GetPlayerUID(playerid),playerid,DoorInfo[doorid][doorName],DoorInfo[doorid][doorUID],pInfo[gracz][pName],gracz);
				LogAdminAction(adminLog);
				
				//reload drzwi
				new load = DoorInfo[doorid][doorUID];
				UnloadDoor(DoorInfo[doorid][doorSampID]);
				LoadDoor(load);
				
				
				return 1;
				
			}
			else if(typee == DOOR_TYPE_GROUP)
			{
				//owner = gid grupy
				if(grupa[owner][Guid] > 0)
				{
					new buffer[256];
					format(buffer,sizeof(buffer),"UPDATE doors SET otype=%i, ouid=%i WHERE uid=%i",DOOR_TYPE_GROUP,grupa[owner][Guid],DoorInfo[doorid][doorUID]);
					mysql_query(buffer);
					
					new output[126];
					format(output,sizeof(output),"Przypisa³eœ drzwi %s (ID:%i) pod grupê %s (ID:%i)",DoorInfo[doorid][doorName],doorid,grupa[owner][Gname],owner);
					GuiInfo(playerid,output);
					
					//reload drzwi
					new load = DoorInfo[doorid][doorUID];
					UnloadDoor(DoorInfo[doorid][doorSampID]);
					LoadDoor(load);
				}
				else
				{
					vlive_err(playerid,"taka grupa nie istnieje");
				}
			}
			else if(typee == DOOR_TYPE_BANK)
			{
				new buffer[256];
				format(buffer,sizeof(buffer),"UPDATE doors SET otype=%i WHERE uid=%i",DOOR_TYPE_BANK,DoorInfo[doorid][doorUID]);
				mysql_query(buffer);
				
				new output[126];
				format(output,sizeof(output),"Uczyni³eœ drzwi %s (ID:%i) bankiem.",DoorInfo[doorid][doorName],doorid);
				GuiInfo(playerid,output);
				
				//reload drzwi
				new load = DoorInfo[doorid][doorUID];
				UnloadDoor(DoorInfo[doorid][doorSampID]);
				LoadDoor(load);
			}
			else if(typee == DOOR_TYPE_247)
			{
				new buffer[256];
				format(buffer,sizeof(buffer),"UPDATE doors SET otype=%i WHERE uid=%i",DOOR_TYPE_247,DoorInfo[doorid][doorUID]);
				mysql_query(buffer);
				
				new output[126];
				format(output,sizeof(output),"Uczyni³eœ drzwi %s (ID:%i) sklepem typu 24/7.",DoorInfo[doorid][doorName],doorid);
				GuiInfo(playerid,output);
				
				//reload drzwi
				new load = DoorInfo[doorid][doorUID];
				UnloadDoor(DoorInfo[doorid][doorSampID]);
				LoadDoor(load);
			}
			else if(typee == DOOR_TYPE_UBRANIA)
			{
				new buffer[256];
				format(buffer,sizeof(buffer),"UPDATE doors SET otype=%i WHERE uid=%i",DOOR_TYPE_UBRANIA,DoorInfo[doorid][doorUID]);
				mysql_query(buffer);
				
				new output[126];
				format(output,sizeof(output),"Uczyni³eœ drzwi %s (ID:%i) sklepem typu Dider Sachs.",DoorInfo[doorid][doorName],doorid);
				GuiInfo(playerid,output);
				
				//reload drzwi
				new load = DoorInfo[doorid][doorUID];
				UnloadDoor(DoorInfo[doorid][doorSampID]);
				LoadDoor(load);
			}
			else if(typee == DOOR_TYPE_ATTACHMENTS)
			{
				new buffer[256];
				format(buffer,sizeof(buffer),"UPDATE doors SET otype=%i WHERE uid=%i",DOOR_TYPE_ATTACHMENTS,DoorInfo[doorid][doorUID]);
				mysql_query(buffer);
				
				new output[126];
				format(output,sizeof(output),"Uczyni³eœ drzwi %s (ID:%i) sklepem typu Akcesoria dla postaci.",DoorInfo[doorid][doorName],doorid);
				GuiInfo(playerid,output);
				
				//reload drzwi
				new load = DoorInfo[doorid][doorUID];
				UnloadDoor(DoorInfo[doorid][doorSampID]);
				LoadDoor(load);
			}
			else if(typee == DOOR_TYPE_HOTEL)
			{
				new buffer[256];
				format(buffer,sizeof(buffer),"UPDATE doors SET otype=%i WHERE uid=%i",DOOR_TYPE_HOTEL,DoorInfo[doorid][doorUID]);
				mysql_query(buffer);
				
				new output[126];
				format(output,sizeof(output),"Uczyni³eœ drzwi %s (ID:%i) hotelem.",DoorInfo[doorid][doorName],doorid);
				GuiInfo(playerid,output);
				
				//reload drzwi
				new load = DoorInfo[doorid][doorUID];
				UnloadDoor(DoorInfo[doorid][doorSampID]);
				LoadDoor(load);
			}
			else if(typee == DOOR_TYPE_APTEKA)
			{
				new buffer[256];
				format(buffer,sizeof(buffer),"UPDATE doors SET otype=%i WHERE uid=%i",DOOR_TYPE_APTEKA,DoorInfo[doorid][doorUID]);
				mysql_query(buffer);
				
				new output[126];
				format(output,sizeof(output),"Uczyni³eœ drzwi %s (ID:%i) aptek¹.",DoorInfo[doorid][doorName],doorid);
				GuiInfo(playerid,output);
				
				//reload drzwi
				new load = DoorInfo[doorid][doorUID];
				UnloadDoor(DoorInfo[doorid][doorSampID]);
				LoadDoor(load);
			}
		}
		
		if(!strcmp(type, "zamknij", true))
		{
			new doorid;
			if(sscanf(varchar, "i", doorid))
			{
				vlive_err(playerid, "/adrzwi zamknij [doorid]");
				return 1;
			}
			
			if(DoorInfo[doorid][doorUID] > 0)
			{
				if(DoorInfo[doorid][doorClose] != 0)
				{
					new buffer[256];
					format(buffer,sizeof(buffer),"UPDATE doors SET close = 0 WHERE uid=%i",DoorInfo[doorid][doorUID]);
					mysql_query(buffer);
					DoorInfo[doorid][doorClose] = 0;
					SendClientMessage(playerid,COLOR_GOLD,"[doors] Otworzy³eœ drzwi.");
					
					new adminlog[128];
					format(adminlog,sizeof(adminlog),"Administrator %s otworzy³ drzwi o nazwie %s (%i)",pInfo[playerid][pName],DoorInfo[doorid][doorName],DoorInfo[doorid][doorSampID]);
					vlive_AdminLog(adminlog);
					
				}
				else
				{
					new buffer[256];
					format(buffer,sizeof(buffer),"UPDATE doors SET close = 1 WHERE uid=%i",DoorInfo[doorid][doorUID]);
					mysql_query(buffer);
					DoorInfo[doorid][doorClose] = 1;
					SendClientMessage(playerid,COLOR_GOLD,"[doors] Zamkn¹³eœ drzwi.");
					
					new adminlog[128];
					format(adminlog,sizeof(adminlog),"Administrator %s zamkn¹³ drzwi o nazwie %s (%i)",pInfo[playerid][pName],DoorInfo[doorid][doorName],DoorInfo[doorid][doorSampID]);
					vlive_AdminLog(adminlog);
					
				}
			}
			else
			{
				vlive_err(playerid,"nie ma takich drzwi");
			}
		}
		
		if(!strcmp(type, "goto", true))
		{
			new doorid;
			if(sscanf(varchar, "i", doorid))
			{
				vlive_err(playerid, "/adrzwi goto [doorid]");
				return 1;
			}
			
			if(DoorInfo[doorid][doorUID] > 0)
			{
				SetPlayerPos(playerid,DoorInfo[doorid][doorEnterX]+2,DoorInfo[doorid][doorEnterY],DoorInfo[doorid][doorEnterZ]);
				SetPlayerVirtualWorld(playerid,DoorInfo[doorid][doorEnterVW]);
				SendClientMessage(playerid,COLOR_GOLD,"[doors] Przeteleportowa³eœ siê do drzwi.");
			}
			else
			{
				vlive_err(playerid,"nie ma takich drzwi");
			}
		}
		
		if(!strcmp(type, "lista", true))
		{
			new doorlist[2512];
			format(doorlist,sizeof(doorlist),"SELECT * FROM doors");
			mysql_query(doorlist);
			mysql_store_result();
			new ilerazy = mysql_num_rows();
			mysql_free_result();
			new petla = 0;
			format(doorlist,sizeof(doorlist),"");
			for(new i=0; i < MAX_DOORS; i++)
			{
				if(DoorInfo[i][doorUID] > 0)
				{
					if(DoorInfo[i][doorClose] != 0)
					{
						//zamkniete
						format(doorlist,sizeof(doorlist),"%s\n%i\t\t"COL_RED"%s",doorlist,DoorInfo[i][doorSampID],DoorInfo[i][doorName]);
					}
					else
					{
						//otwarte
						format(doorlist,sizeof(doorlist),"%s\n%i\t\t"COL_GREEN2"%s",doorlist,DoorInfo[i][doorSampID],DoorInfo[i][doorName]);
					}
					petla++;
					if(petla == ilerazy)
					{
						break;
					}
				}
			}
			ShowPlayerDialog(playerid,404,DIALOG_STYLE_LIST,"Spis wszystkich drzwi",doorlist,"Zamknij","");
		}
		if(!strcmp(type, "info", true))
		{
			new doorid;
			if(sscanf(varchar,"d",doorid))
			{
				vlive_err(playerid,"/adrzwi info [doorid]");
				return 1;
			}
			if(DoorInfo[doorid][doorUID] != 0)
			{
				new info[1024];
				new param[128];
				format(info,sizeof(info),"Nazwa drzwi: \t%s",DoorInfo[doorid][doorName]);
				format(param,128,"\nKoszt wstêpu: \t%i",DoorInfo[doorid][doorEntry]);
				strcat(info, param, sizeof(info));
				if(DoorInfo[doorid][doorClose] != 0)
				{
					format(param,128,"\nZamek: \tzamkniêty");
					strcat(info, param, sizeof(info));
				}
				else
				{
					format(param,128,"\nZamek: \totwarty");
					strcat(info, param, sizeof(info));
				}
				format(param,128,"\nUID w³aœciciela: \t%i",DoorInfo[doorid][doorOwnerUID]);
				strcat(info, param, sizeof(info));
				
				if(DoorInfo[doorid][doorOwnerType] == DOOR_TYPE_GROUP)
				{
					format(param,128,"\nTyp w³aœciciela: \tGrupa");
					strcat(info, param, sizeof(info));
				}
				else if(DoorInfo[doorid][doorOwnerType] == DOOR_TYPE_PLAYER)
				{
					format(param,128,"\nTyp w³aœciciela: \tGracz");
					strcat(info, param, sizeof(info));
				}
				else
				{
					//none
					format(param,128,"\nTyp w³aœciciela: \tNikt");
					strcat(info, param, sizeof(info));
				}
				
				
				ShowPlayerDialog(playerid,404,DIALOG_STYLE_LIST,"Informacje o drzwiach",info,"OK","");
			}
			else
			{
				vlive_err(playerid,"takie drzwi nie istniej¹");
			}
		}
	}
	else
	{
		GuiInfo(playerid,"Brak uprawnieñ.");
	}
	return 1;
}

COMMAND:drzwi(playerid,params[])
{
	new type[32], varchar[32];
	if(sscanf(params, "s[32]S()[32]", type, varchar))
	{
	    vlive_err(playerid, "/drzwi (zamknij | info | opcje)");
	 	return 1;
	}
	if(!strcmp(type, "zamknij", true))
	{
		new vw = GetPlayerVirtualWorld(playerid);
		for(new i=0;i < MAX_DOORS; i++)
		{
			if(IsPlayerInRangeOfPoint(playerid,2,DoorInfo[i][doorEnterX],DoorInfo[i][doorEnterY],DoorInfo[i][doorEnterZ]))
			{
				if(DoorInfo[i][doorEnterVW] == vw)
				{
					if(IsPlayerPermsDoors(playerid,i) > 0)
					{
						if(DoorInfo[i][doorClose] != 0)
						{
							DoorInfo[i][doorClose] = 0;
							GameTextForPlayer(playerid, "Drzwi ~g~otwarte", 5000, 5);
							ApplyAnimation(playerid, "INT_HOUSE", "wash_up",4.1, 0, 0, 0, 0, 0, 1);
							
							new buffer[256];
							format(buffer,sizeof(buffer),"UPDATE doors SET close = 0 WHERE uid= %i",DoorInfo[i][doorUID]);
							mysql_query(buffer);
							return 1;
						}	
						else
						{
							DoorInfo[i][doorClose] = 1;
							GameTextForPlayer(playerid, "Drzwi ~r~zamkniete", 5000, 5);
							ApplyAnimation(playerid, "INT_HOUSE", "wash_up",4.1, 0, 0, 0, 0, 0, 1);
							
							new buffer[256];
							format(buffer,sizeof(buffer),"UPDATE doors SET close = 1 WHERE uid= %i",DoorInfo[i][doorUID]);
							mysql_query(buffer);
							return 1;
						}
					}
					else
					{
						GuiInfo(playerid,"Nie posiadasz kluczy do tego budynku.");
					}
				}
			}
			else if(IsPlayerInRangeOfPoint(playerid,2,DoorInfo[i][doorExitX],DoorInfo[i][doorExitY],DoorInfo[i][doorExitZ]))
			{
				if(DoorInfo[i][doorExitVW] == vw)
				{
					if(IsPlayerPermsDoors(playerid,i) > 0)
					{
						if(DoorInfo[i][doorClose] != 0)
						{
							DoorInfo[i][doorClose] = 0;
							GameTextForPlayer(playerid, "Drzwi ~g~otwarte", 5000, 5);
							ApplyAnimation(playerid, "INT_HOUSE", "wash_up",4.1, 0, 0, 0, 0, 0, 1);
							return 1;
						}	
						else
						{
							DoorInfo[i][doorClose] = 1;
							GameTextForPlayer(playerid, "Drzwi ~r~zamkniete", 5000, 5);
							ApplyAnimation(playerid, "INT_HOUSE", "wash_up",4.1, 0, 0, 0, 0, 0, 1);
							return 1;
						}
					}
					else
					{
						GuiInfo(playerid,"Nie posiadasz kluczy do tego budynku.");
					}
				}
			}
		}
	}
	if(!strcmp(type, "opcje", true))
	{
		new doorid = GetPlayerDoorID(playerid);
		if(doorid <= 0)
		{
			GuiInfo(playerid,"Musisz byæ w budynku by u¿yæ tej komendy.");
			return 1;
		}
		
		if(IsPlayerPermsDoorsLeader(playerid,doorid) > 0)
		{
			ShowPlayerDialog(playerid,DIAL_DOOR_OPTIONS,DIALOG_STYLE_LIST,"Opcje drzwi","1. Zmieñ nazwê drzwi\n2. Ustal cenê wejœcia\n3. Ustaw muzykê w interiorze\n4. W³/Wy³ przejazd pojazdów","Wybierz","Zamknij");
		}
		else
		{
			GuiInfo(playerid,"Musisz byæ w³aœcicielem tego budynku.");
			return 1;
		}
	}
	return 1;
}

COMMAND:schowek(playerid,params[])
{
	new doorid = GetPlayerDoorID(playerid);
	if(IsPlayerPermsDoorsLeader(playerid,doorid))	// poprawki
	{
		ListDoorItems(playerid,doorid);
	}
	else
	{
		vlive_err(playerid,"brak uprawnieñ");
	}
	return 1;
}

COMMAND:magazyn(playerid,params[])
{
	new doorid = GetPlayerDoorID(playerid);
	if(IsPlayerPermsDoors(playerid,doorid))	// poprawki
	{
		ShowMagazyn(playerid,doorid);
	}
	else
	{
		vlive_err(playerid,"brak uprawnieñ");
	}
	return 1;
}