/*
*	funkcje dla organizacji przestêpczych
*/

#define MAX_CORNERS 200

enum cornerp
{
	c_uid,
	Float:c_posx,
	Float:c_posy,
	Float:c_posz,
	c_owner,
}
new corner[MAX_CORNERS][cornerp];

stock LoadCrimeSys()
{
	LoadCorners();
	
	return 1;
}

stock LoadCorners()
{
	new buffer[256];
	new gameid = 0;
	format(buffer,sizeof(buffer),"SELECT * FROM corners");
	mysql_query(buffer);
	mysql_store_result();
	
	while(mysql_fetch_row(buffer,"|"))
	{
		sscanf(buffer,"p<|>ifffi",
		corner[gameid][c_uid],
		corner[gameid][c_posx],
		corner[gameid][c_posy],
		corner[gameid][c_posz],
		corner[gameid][c_owner]);
		gameid++;
	}
	
	print("[load] corners loaded");
	
	mysql_free_result();
	return 1;
}

stock ReloadCorners()
{
	for(new i = 0; i < MAX_CORNERS; i++)
	{
		corner[i][c_uid] = 0;
		corner[i][c_posx] = 0;
		corner[i][c_posy] = 0;
		corner[i][c_posz] = 0;
	}
	
	LoadCorners();
	
	print ("[corners] reloaded");
	
	return 1;
}

stock IsPlayerInAnyCorner(playerid)
{
	for(new i=0;i<MAX_CORNERS;i++)
	{
		if(IsPlayerInRangeOfPoint(playerid,30,corner[i][c_posx],corner[i][c_posy],corner[i][c_posz]))
		{
			if(pGrupa[playerid][PlayerDutyGroup[playerid]][pGuid] == corner[i][c_owner])
			{
				return 1;
			}
		}
	}

	return 0;
}

stock GetPlayerCurrentCorner(playerid)
{
	for(new i=0;i<MAX_CORNERS;i++)
	{
		if(IsPlayerInRangeOfPoint(playerid,30,corner[i][c_posx],corner[i][c_posy],corner[i][c_posz]))
		{
			return i;
		}
	}
	
	return -1;
}

CMD:corner(playerid,params[])
{
	if(PlayerCache[playerid][pCornerTime] > 600)
	{
		new selled = 0;
		for(new i = 0 ; i < MAX_PLAYER_ITEMS; i++)
		{
			if(itemInfo[playerid][i][iType] == ITEM_MARIHUANA)
			{
				if(itemInfo[playerid][i][iValue1] == 1)
				{
					new buffer[256];
					format(buffer,sizeof(buffer),"DELETE FROM core_items WHERE item_uid=%i",itemInfo[playerid][i][iUID]);
					mysql_query(buffer);
					
					PlayerCache[playerid][pCornerTime] = 0;
					AddPlayerMoney(playerid,70);
				}
			}
		}
		
		if(selled == 0)
		{
			GuiInfo(playerid,"Nie posiadasz przy sobie marihuany.");
		}
	}
	else
	{
		if(!IsPlayerInAnyCorner(playerid))
		{
			GuiInfo(playerid,"Musisz byæ na terenie jednego z cornerów.");
			return 1;
		}
		
		GuiInfo(playerid,"Musisz staæ tutaj minium dziesiêæ minut by sprzedaæ jakikolwiek towar.");
	}
	
	return 1;
}

CMD:acorner(playerid,params[])
{
	if(CheckPlayerAccess(playerid) == PERMS_PROGRAMMER)
	{
		new type[32], varchar[64];
		if(sscanf(params, "s[32]S()[64]", type, varchar))
		{
			vlive_err(playerid, "/acorner [stworz | usun]");
			return 1;
		}
		
		if(!strcmp(type, "stworz", true))
		{
			new groupid;
			if(sscanf(varchar,"d",groupid))
			{
				vlive_err(playerid,"/acorner stworz [groupid]");
				return 1;
			}
			
			new Float:Pos[3];
			GetPlayerPos(playerid,Pos[0],Pos[1],Pos[2]);
			
			new buffer[256];
			format(buffer,sizeof(buffer),"INSERT INTO corners VALUES(NULL,%f,%f,%f,%i)",Pos[0],Pos[1],Pos[2],grupa[groupid][Guid]);
			mysql_query(buffer);
			
			ReloadCorners();
		}
		
		if(!strcmp(type, "usun", true))
		{
			new dcid = -1;
			for(new i = 0 ; i < MAX_CORNERS; i++)
			{
				if(IsPlayerInRangeOfPoint(playerid,30,corner[i][c_posx],corner[i][c_posy],corner[i][c_posz]))
				{
					dcid = i;
					break;
				}
			}
			
			if(dcid == -1)
			{
				GuiInfo(playerid,"Nie znaleziono ¿adnego cornera w pobli¿u.");
				return 1;
			}
			else
			{
				GuiInfo(playerid,"Usuniêto corner.");
				new buffer[256];
				format(buffer,sizeof(buffer),"DELETE FROM corners WHERE uid=%i",corner[dcid][c_uid]);
				mysql_query(buffer);
				ReloadCorners();
			}
		}
	}
	else
	{
		GuiInfo(playerid,"Brak uprawnieñ.");
	}
	
	return 1;
}
