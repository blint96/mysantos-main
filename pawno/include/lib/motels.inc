#define MOTEL_TYPE_BIEDNY 1
#define MOTEL_TYPE_SREDNI 2
#define MOTEL_TYPE_BOGATY 3

#define MAX_HOTELS 15

enum hot_params
{
	motDoorUID,
}
new Hotel[MAX_HOTELS][hot_params];

stock GetFreeMotelID()
{
	new motID = -1;
	for(new i = 0 ; i < MAX_HOTELS; i++)
	{
		if(Hotel[i][motDoorUID] <= 0)
		{
			motID = i;
			break;
		}
	}
	
	return motID;
}

stock LoadMotels()
{
	for(new i= 0 ; i < MAX_DOORS; i++)
	{
		if(DoorInfo[i][doorOwnerType] == DOOR_TYPE_HOTEL)
		{
			new motID = GetFreeMotelID();
			Hotel[motID][motDoorUID] = DoorInfo[i][doorUID];
		}
	}
}

stock IsPlayerHouseLocator(playerid,doorid)
{
	new dooruid = DoorInfo[doorid][doorUID];
	new buffer[256];
	format(buffer,sizeof(buffer),"SELECT * FROM houseinvites WHERE dooruid=%i AND charuid=%i",dooruid,GetPlayerUID(playerid));
	mysql_query(buffer);
	mysql_store_result();
	if(mysql_num_rows() > 0)
	{
		mysql_free_result();
		return 1;
	}
	else
	{
		mysql_free_result();
		return 0;
	}
}

stock SetPlayerSpawning(playerid,spawn)
{
	new buffer[256];
	format(buffer,sizeof(buffer),"UPDATE core_players SET char_spawn=%i WHERE char_uid=%i",spawn,GetPlayerUID(playerid));
	mysql_query(buffer);
	
	return 1;
}

CMD:pokoj(playerid,params[])
{
	new doorid = GetPlayerDoorID(playerid);
	if(DoorInfo[doorid][doorOwnerType] != DOOR_TYPE_HOTEL)
	{
		GuiInfo(playerid,"Musisz byæ w motelu by u¿yæ tej komendy.");
		return 1;
	}
	
	//ackjca
	
	return 1;
}

COMMAND:dom(playerid,params[])
{
	// dodac zapros i wypros
	new type[32],varchar[128];
	if(sscanf(params, "s[32]S()[64]", type, varchar))
	{
		ShowPlayerDialog(playerid,DIAL_SHOW_HOUSE_MENU,DIALOG_STYLE_LIST,"Menu mieszkañ","1. Lista mieszkañ\n2. Lokatorzy","Wybierz","Anuluj");
		vlive_err(playerid,"/dom zapros [playerid]");
		return 1;
	}
	
	if(!strcmp(type, "zapros", true))
	{
		new target;
		if(sscanf(varchar,"d",target))
		{
			vlive_err(playerid,"/dom zapros [playerid]");
			return 1;
		}
		
		new doorid = GetPlayerDoorID(playerid);
		if(IsPlayerPermsDoors(playerid,doorid))
		{
			if(IsPlayerHouseLocator(target,doorid))
			{
				GuiInfo(playerid,"Ten gracz jest ju¿ lokatorem tego mieszkania.");
			}
			else
			{
				new buffer[256];
				format(buffer,sizeof(buffer),"INSERT INTO houseinvites (charuid,dooruid,charname) VALUES (%i,%i,'%s')",GetPlayerUID(target),DoorInfo[doorid][doorUID],pInfo[target][pName]);
				mysql_query(buffer);
				GuiInfo(playerid,"Doda³eœ tego gracza jako lokatora Twojego mieszkania.");
				
				new msg[128];
				format(msg,sizeof(msg),"Gracz %s (SampID: %i) zaprosi³ Cie do swojego mieszkania %s (SampID: %i)",pInfo[playerid][pName],playerid,DoorInfo[doorid][doorName],doorid);
			
				GuiInfo(target,msg);
			}
		}
		else
		{
			GuiInfo(playerid,"Brak uprawnieñ.");
		}
	}
	
	return 1;
}