//
//
//		SMIECIARKI
//
//

enum rubbish_pars
{
	Float:rubbishX,
	Float:rubbishY,
	Float:rubbishZ,
}

new RubbishSpot[5][rubbish_pars] = 
{
	{2726.1443,-1936.3428,13.5394},
	{2086.1758,-988.0045,51.5169},
	{1127.4784,-1031.2784,31.8959},
	{792.5594,-1334.4568,13.3828},
	{917.9134,-1796.8820,13.5547}
};

stock IsVehicleOccupied(vehicleid)
{
	for(new i = 0 ; i < MAX_PLAYERS; i++)
	{	
		if(IsPlayerInVehicle(i, vehicleid))
		{
			return 1;
		}
	}
	return 0;
}

stock RandPlayerCash(playerid)			//funkcja na dawanie hajsu
{
	loss:
	new cash = random(10);
	
	if(cash < 5)
	{
		goto loss;
	}
	else
	{
		AddPlayerMoney(playerid,cash);
		new output[128];
		format(output,sizeof(output),"Zarobi³eœ %i$ rozwo¿¹c pizzê.",cash);
		GuiInfo(playerid,output);
	}
}

COMMAND:smieci(playerid,params[])
{
	new vehicleid = GetPlayerVehicleID(playerid);
	new seat = GetPlayerVehicleSeat(playerid);
	
	
	if(seat == 0)
	{
		if(vehicle[vehicleid][vownertype] == 1)
		{
			if(vehicle[vehicleid][vowneruid] == GROUP_TYPE_RUBBISH_UID)
			{
				if(PlayerCache[playerid][pRubbishDrive] > 0)
				{
					//anuluj
					DisablePlayerCheckpoint(playerid);
					PlayerCache[playerid][pRubbishDrive] = 0;
					PlayerCache[playerid][pRubbishCheckpoint] = false;
				}
				else
				{
					//dodaj jezdzenie
					PlayerCache[playerid][pRubbishDrive] = 1;
					new trasa = random(5);
					SetPlayerCheckpoint(playerid, RubbishSpot[trasa][rubbishX], RubbishSpot[trasa][rubbishY], RubbishSpot[trasa][rubbishZ], 3.0);
					PlayerCache[playerid][pRubbishCheckpoint] = true;
				}
			}
			else
			{
				//nie ten typ pojazdu
			}
		}
		else
		{
			//nie ten typ wlasiciela
		}
	}
	else
	{
		//nie to miejsce w pojezdzie
	}
	return 1;
}

COMMAND:pizza(playerid,params[])
{
	PlayerCache[playerid][pPizzaTicks]++;
	if(PlayerCache[playerid][pPizzaTicks] > 6)
	{
		system_kick(playerid,"Pizza-loss");
	}
	else if(PlayerCache[playerid][pPizzaTicks] > 3)
	{
		GuiInfo(playerid,"Nie spamuj tak komend¹ bo jeszcze kicka dostaniesz! :)");
	}
	
	new vehicleid = GetPlayerVehicleID(playerid);
	//new seat = GetPlayerVehicleSeat(playerid);
	
	if(IsPlayerInAnyVehicle(playerid))
	{
		if(vehicle[vehicleid][vownertype] == 1)
		{
			if(vehicle[vehicleid][vowneruid] == GROUP_TYPE_PIZZA_UID)
			{
				if(PlayerCache[playerid][pRubbishDrive] > 0)
				{
					//anuluj
					DisablePlayerCheckpoint(playerid);
					PlayerCache[playerid][pRubbishDrive] = 0;
					PlayerCache[playerid][pRubbishCheckpoint] = false;
				}
				else
				{
					//dodaj jezdzenie
					PlayerCache[playerid][pRubbishDrive] = 1;
					new trasa = random(5);
					SetPlayerCheckpoint(playerid, RubbishSpot[trasa][rubbishX], RubbishSpot[trasa][rubbishY], RubbishSpot[trasa][rubbishZ], 3.0);
					PlayerCache[playerid][pRubbishCheckpoint] = true;
					
					//uruchom silnik pojazdu				
					new engine, lights, alarm, doors, bonnet, boot, objective;
					GetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, bonnet, boot, objective);
					SetVehicleParamsEx(vehicleid, 1, lights, alarm, doors, bonnet, boot, objective);
				}
			}
			else
			{
				//nie ten typ pojazdu
				GuiInfo(playerid,"To nie jest pojazd do rozwo¿enia pizzy.");
			}
		}
		else
		{
			//nie ten typ wlasiciela
		}
	}
	else
	{
		GuiInfo(playerid,"Musisz byæ w Pizza Boy'u.");
	}
	
	return 1;
}