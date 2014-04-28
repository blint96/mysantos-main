/*
	Skrypt umożliwia wykupywanie ziemi oraz dowolną edycję obiektów.
*/

#define MAX_PROPERTIES 1024
#define INVALID_AREA_ID -1


/*new Float:PlayerAreaMaxX[MAX_PLAYERS];
new Float:PlayerAreaMaxY[MAX_PLAYERS];
new Float:PlayerAreaMinX[MAX_PLAYERS];
new Float:PlayerAreaMinY[MAX_PLAYERS];*/

enum areasPars
{
	areaSID,
	areaUID,
	areaOwner,
	areaOwnerType,
	Float:areaMinX,
	Float:areaMinY,
	Float:areaMaxX,
	Float:areaMaxY,
	areaName[64],
	areaMaxObjects,
	areaColor,
}
new PropertyInfo[MAX_PROPERTIES][areasPars];

stock LoadProperties()
{
	print("[load] wczytuję moduł gruntow");
	new buffer[256];
	//new gameid = 0;
	format(buffer,sizeof(buffer),"SELECT * FROM areas");
	mysql_query(buffer);
	mysql_store_result();
	new num = 0;
	
	while(mysql_fetch_row(buffer,"|"))
	{
		sscanf(buffer,"p<|>iiiffffs[64]ii",
		PropertyInfo[num][areaUID],
		PropertyInfo[num][areaOwner],
		PropertyInfo[num][areaOwnerType],
		PropertyInfo[num][areaMinX],
		PropertyInfo[num][areaMinY],
		PropertyInfo[num][areaMaxX],
		PropertyInfo[num][areaMaxY],
		PropertyInfo[num][areaName],
		PropertyInfo[num][areaMaxObjects],
		PropertyInfo[num][areaColor]);
		
		//tworz area
		PropertyInfo[num][areaSID] = GangZoneCreate(PropertyInfo[num][areaMinX], PropertyInfo[num][areaMinY], PropertyInfo[num][areaMaxX], PropertyInfo[num][areaMaxY]);
		
		
		num++;
	}
	printf("[load] wczytano %i gruntow",num);
	
	mysql_free_result();
	return 1;
}

stock ShowProperties(playerid)
{
	/*
		to musi byc zmienione ze tylko dla org. przestępczych
	*/
	
	if(IsPlayerInGroupType(playerid,TYPE_MAFIA) == 1 || IsPlayerInGroupType(playerid,TYPE_SZAJK) == 1 || IsPlayerInGroupType(playerid,TYPE_SCIGANT) == 1 || IsPlayerInGroupType(playerid,TYPE_ORG) == 1 || IsPlayerInGroupType(playerid,TYPE_KARTEL) == 1 || IsPlayerInGroupType(playerid,TYPE_SYNDYKAT) == 1)
	{
		//jest w grupie
	}
	else
	{
		//nie jest no to niech nie wyświetla przecież , nie ?
		return 1;
	}
	
	for(new i = 0 ; i < MAX_PROPERTIES; i++)
	{
		if(PropertyInfo[i][areaUID] > 0)
		{
			switch(PropertyInfo[i][areaColor])
			{
				case 1:
				{
					GangZoneFlashForPlayer(playerid,PropertyInfo[i][areaSID],COLOR_WHITE);
				}
				case 2:
				{
					GangZoneFlashForPlayer(playerid,PropertyInfo[i][areaSID],COLOR_BLUE);
				}
				case 3:
				{
					GangZoneFlashForPlayer(playerid,PropertyInfo[i][areaSID],COLOR_ORANGE);
				}
				case 4:
				{
					GangZoneFlashForPlayer(playerid,PropertyInfo[i][areaSID],COLOR_RED);
				}
				case 5:
				{
					GangZoneFlashForPlayer(playerid,PropertyInfo[i][areaSID],COLOR_AC);
				}
			}
		}
	}
	return 1;
}

stock UnloadProperties()
{

}

stock CreateProperty()
{

}

stock DeleteProperty()
{

}

stock GetPlayerAreaID(playerid)
{

}

stock OnPlayerEnterProperty(playerid,areaid)
{
	return 1;
}

stock IsPlayerInAnyArea(playerid)
{
    new Float:X, Float:Y, Float:Z; 
    for(new i=0; i < MAX_PROPERTIES; i++) 
    {
		if(PropertyInfo[i][areaUID] > 0)
		{
			GetPlayerPos(playerid, X, Y, Z); 
			if (X >= PropertyInfo[i][areaMinX] && X <= PropertyInfo[i][areaMaxX] && Y >= PropertyInfo[i][areaMinY] && Y <= PropertyInfo[i][areaMaxY])
			{
				PlayerCache[playerid][pArea] = i;
				return 1;
			}
		}
    }
	return 0;
}

CMD:ateren(playerid,params[])
{
	if(CheckPlayerAccess(playerid) > PERMS_SUPPORT)
	{
		
	}
	else
	{
		GuiInfo(playerid,"Brak uprawnień.");
	}
	
	return 1;
}

//
//
//
//		Ikony na mapce, stawia je tylko administrator.
//
//

stock LoadIcons(playerid)
{
	for(new i = 0 ; i < 100; i++)
	{
		//czyszczenie ikonek dla gracza
		RemovePlayerMapIcon( playerid, i );
	}
	
	//dodanie ich na nowo
	new gameid = 0;
	new buffer[256];
	format(buffer,sizeof(buffer),"SELECT * FROM icons");
	mysql_query(buffer);
	mysql_store_result();
	
	while(mysql_fetch_row(buffer,"|"))
	{
		new null,Float:Pos[3],markertype,style;
		sscanf(buffer,"p<|>ifffii",null,Pos[0],Pos[1],Pos[2],markertype,style);
		
		SetPlayerMapIcon( playerid, gameid, Pos[0],Pos[1],Pos[2], markertype, 0, MAPICON_LOCAL );
		
		gameid++;
	}
	
	mysql_free_result();
	return 1;
}

CMD:aicon(playerid,params[])
{
	if(CheckPlayerAccess(playerid) > PERMS_SUPPORT)
	{
		new type[32], varchar[64];
		if(sscanf(params, "s[32]S()[64]", type, varchar))
		{
			vlive_err(playerid, "/aicon [stworz | usun]");
			return 1;
		}
		
		if(!strcmp(type, "stworz", true))
		{
			ShowPlayerDialog(playerid,DIAL_CICON,DIALOG_STYLE_LIST,"Wybierz ikone","1. Restauracja\n2. Warsztat\n3. Salon samochodowy\n4. Dolar\n5. Ubranie","Wybierz","Anuluj");
		}
		
		if(!strcmp(type, "usun", true))
		{
			new buffer[256];
			format(buffer,sizeof(buffer),"SELECT * FROM icons");
			mysql_query(buffer);
			mysql_store_result();
			
			new todel = -1;
			
			while(mysql_fetch_row(buffer,"|"))
			{
				new null,Float:Pos[3],markertype,style;
				sscanf(buffer,"p<|>ifffii",null,Pos[0],Pos[1],Pos[2],markertype,style);
				
				if(IsPlayerInRangeOfPoint(playerid,15,Pos[0],Pos[1],Pos[2]))
				{
					todel = null;
					break;
				}
			}
			
			mysql_free_result();
			
			format(buffer,sizeof(buffer),"DELETE FROM icons WHERE uid=%i",todel);
			mysql_query(buffer);
			
			GuiInfo(playerid,"Usunąłes ikonę..");
			
			for(new i = 0 ; i < MAX_PLAYERS;i ++)
			{
				 LoadIcons(i);
			}
			
		}
	}
	else
	{
		GuiInfo(playerid,"Brak uprawnień");
	}
	
	return 1;
}