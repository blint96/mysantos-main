

/*	
*	Skrypt do oczyszczania bazy danych z niepotrzebnego gówna 
* 	powstały na potrzeby mySantos
*  lepiej wykonaj kopię zapasową bazy w razie gdyby coś miało się popsuć
*/


// World Without Door - czyli świat z obiektami bez drzwi, innymi słowy zbędne
new wwd[1000];



stock GetWWD()
{
	new num = 0;
	for(new i = 0 ; i < MAX_OBJECTS_VL; i++)
	{
		if(IsWorldHasDoor(object[i][object_world]) > 0)
		{
			continue;
		}
		else
		{
			num++;
			wwd[num] = object[i][object_world];
		}
	}
}

stock IsWorldHasDoor(world)
{
	for(new i = 0 ; i < MAX_DOORS; i++)
	{
		if(DoorInfo[i][doorExitVW] == world)
		{
			return 1;
		}
	}
	return 0;
}

stock SetupDatabaseClean()
{
	for(new i = 0 ; i < 1000; i++)
	{
		if(wwd[i] == 0)
		{
			continue;
		}
		else
		{
			new buffer[256];
			format(buffer,sizeof(buffer),"DELETE * FROM core_objects WHERE object_world=%i",wwd[i]);
			mysql_query(buffer);
		}
	}
}

CMD:dbprepare(playerid,params[])
{
	if(CheckPlayerAccess(playerid) == PERMS_PROGRAMMER)
	{
		GetWWD();
		GuiInfo(playerid,"Przygotowano listę - przejdź do kroku drugiego wpisując komendę /dbclean");
	}
	else
	{
		GuiInfo(playerid,"Brak uprawnień.");
	}
	
	return 1;
}

CMD:dbclean(playerid,params[])
{
	if(CheckPlayerAccess(playerid) == PERMS_PROGRAMMER)
	{
		SetupDatabaseClean();
		GuiInfo(playerid,"2/2 - proces oczyszczania bazy danych zakończony sukcesem");
	}
	else
	{
		GuiInfo(playerid,"Brak uprawnień.");
	}
	
	return 1;
}