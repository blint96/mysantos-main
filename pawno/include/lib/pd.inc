new CheckArrested[MAX_PLAYERS];
new ArrestTime[MAX_PLAYERS];

enum ar_pars
{
	aid,
	Float:aX,
	Float:aY,
	Float:aZ,
}
new ArrestSpot[5][ar_pars] = 
{
	{0,1929.4873,-2298.2185,13.5728},
	{1,1928.8525,-2302.7744,13.5728},
	{2,1928.3171,-2306.9841,13.5728},
	{3,1929.1514,-2312.1726,13.5728},
	{4,1929.7942,-2318.6812,13.5728}
};


CMD:kartoteka(playerid,params[])
{
	new targetid;
	if(sscanf(params,"d",targetid))
	{
		vlive_err(playerid,"/kartoteka [playerid]");
		return 1;
	}
	
	if(DutyGroupType[playerid] != TYPE_LSPD)
	{
		GuiInfo(playerid,"Brak uprawnieñ.");
		return 1;
	}
	
	new playerVehicleID = GetPlayerVehicleID(playerid);
	new playerVehicle = GetVehicleModel(playerVehicleID);
	if(playerVehicle == 497 || playerVehicle == 596 || playerVehicle == 597 || playerVehicle == 598 || playerVehicle == 599 )
	{
		new buffer[256];
		format(buffer,sizeof(buffer),"SELECT reason,cost,points FROM lspd_kartoteka WHERE player=%i",GetPlayerUID(targetid));
		mysql_query(buffer);
		mysql_store_result();
		new lista[1024];
		if(mysql_num_rows() > 0)
		{
			format(lista,sizeof(lista),""COL_GREEN"Koszt mandatu\t\tPKT Karne\t\tPowód\n"COL_GRAY2"##########################");
		}
		while(mysql_fetch_row(buffer,"|"))
		{
			new reason[64],cost,points;
			sscanf(buffer,"p<|>s[64]ii",reason,cost,points);
			format(lista,sizeof(lista),"%s\n%i$\t\t%i\t\t%s",lista,cost,points,reason);
		}
		mysql_free_result();
		
		if(!lista[0])
		{
			GuiInfo(playerid,"Brak wpisów w kartotece dla tego cz³owieka.");
		}
		else
		{
			new who[32];
			format(who,sizeof(who),"Kartoteka %s",pInfo[targetid][pName]);
			ShowPlayerDialog(playerid,404,DIALOG_STYLE_LIST,who,lista,"OK","");
		}
	}
	else
	{
		GuiInfo(playerid,"W tym pojeŸdzie nie ma komputera pok³adowego.");
	}
	return 1;
}

CMD:wypusc(playerid,params[])
{
	new target;
	if(sscanf(params,"d",target))
	{
		vlive_err(playerid,"/wypusc [gracz]");
		return 1;
	}
	
	if(DutyGroupType[playerid] != TYPE_LSPD)
	{
		GuiInfo(playerid,"Nie posiadasz uprawnieñ do wykonania tej komendy.");
		return 1;
	}
	
	if(GetPlayerDoorID(playerid) <= 0 )
	{
		GuiInfo(playerid,"Musisz byæ na posterunku by u¿yæ tej komendy.");
		return 1;
	}
	
	if(IsPlayerArrested(target))
	{
		ArrestTime[target] = 1;
	}
	else
	{
		GuiInfo(playerid,"Ten gracz nie jest aresztowany.");
	}
	
	return 1;
}

CMD:areszt(playerid,params[])
{
	new arrestTime,target;
	if(sscanf(params,"dd",arrestTime,target))
	{
		vlive_err(playerid,"/areszt [czas w minutach] [gracz]");
		return 1;
	}
	
	if(DutyGroupType[playerid] != TYPE_LSPD)
	{
		GuiInfo(playerid,"Nie posiadasz uprawnieñ do wykonania tej komendy.");
		return 1;
	}
	
	if(GetPlayerDoorID(playerid) <= 0 )
	{
		GuiInfo(playerid,"Musisz byæ na posterunku by u¿yæ tej komendy.");
		return 1;
	}
	
	if(IsPlayerInRangeOfPlayer(playerid,target,5))
	{
		if(IsPlayerArrested(target))
		{
			GuiInfo(playerid,"Ten gracz jest ju¿ aresztowany.");
			return 1;
		}
		else
		{
			//aresztuj
			if(arrestTime > 120)
			{
				vlive_err(playerid,"nie przesadzasz z tymi minutami?");
				return 1;
			}
			new buffer[256];
			format(buffer,sizeof(buffer),"INSERT INTO lspd_arrests VALUES(NULL,%i,%i,%i)",GetPlayerUID(playerid),GetPlayerUID(target),arrestTime);
			mysql_query(buffer);
			
			if(CheckArrested[target] <= 0)
			{
				if(IsPlayerArrested(target))
				{
					//ustaw do pierdla
					format(buffer,sizeof(buffer),"SELECT minutes FROM lspd_arrests WHERE gainer = %i",GetPlayerUID(target));
					mysql_query(buffer);
					mysql_store_result();
					ArrestTime[target] = mysql_fetch_int();
					mysql_free_result();
					
					new jail = random(5);
					SetPlayerVirtualWorld(target,3169);
					SetPlayerPos(target,ArrestSpot[jail][aX],ArrestSpot[jail][aY],ArrestSpot[jail][aZ]);
					OnEnterToDoor(target);
					CheckArrested[target] = 1;
					SetTimerEx("PoliceArrest", 500, false, "i", target);
				}
			}
		}
	}
	else
	{
		GuiInfo(playerid,"Gracz poza zasiêgiem.");
	}
	
	return 1;
}

stock IsPlayerArrested(playerid)
{
	new buffer[256];
	format(buffer,sizeof(buffer),"SELECT * FROM lspd_arrests WHERE gainer = %i",GetPlayerUID(playerid));
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

forward PoliceArrest(playerid);
public PoliceArrest(playerid)
{
	if(ArrestTime[playerid] > 0)
	{
		new msg[64];
		format(msg,sizeof(msg),"czas odsiadki: ~r~%i min",ArrestTime[playerid]);
		GameTextForPlayer(playerid, msg, 60000, 5);
		new buffer[256];
		format(buffer,sizeof(buffer),"UPDATE lspd_arrests SET minutes = %i WHERE gainer = %i",ArrestTime[playerid],GetPlayerUID(playerid));
		mysql_query(buffer);
		ArrestTime[playerid]--;
		SetTimerEx("PoliceArrest", 60000, false, "i", playerid);
	}
	else
	{
		new buffer[256];
		format(buffer,sizeof(buffer),"DELETE FROM lspd_arrests WHERE gainer = %i",GetPlayerUID(playerid));
		mysql_query(buffer);
		ArrestTime[playerid] = 0;
	}
	return 1;
}

//
//
//
//
//			FBI
//
//
//

#define FBI_DB_HUMAN 1
#define FBI_DB_CAR	2
#define FBI_DB_PROPERTY	3
#define FBI_DB_WEAPON	4

COMMAND:database(playerid,params[])
{
	// /database gracz [playerid] [wpis]
	// /database pojazd [vehicleid] [wpis]
	// /database broñ [UID broni] [wpis]
	// /database posiad³oœæ [ID drzwi] [wpis]
	return 1;
}

