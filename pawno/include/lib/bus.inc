#define MAX_BUS_STOPS	500
#define BUS_COST	10

enum b_params
{
	Float:busPosX,
	Float:busPosY,
	Float:busPosZ,
	
	busName[64],
}
new BusStop[MAX_BUS_STOPS][b_params];

new Text3D:BusName[MAX_BUS_STOPS];

stock LoadBusSystem()
{
	printf("[bus] ³adujê modu³ przystanków");
	LoadBusStops();
}

stock ReloadBusStops()
{
	for(new i = 0; i < MAX_BUS_STOPS; i++)
	{
		BusStop[i][busPosX] = 0;
		BusStop[i][busPosY] = 0;
		BusStop[i][busPosZ] = 0;
		BusStop[i][busName][0] = EOS;
		
		DestroyDynamic3DTextLabel(BusName[i]);
	}
	
	LoadBusStops();
	printf("[bus] przeladowano przystanki");
}

stock LoadBusStops()
{
	new buffer[256];
	format(buffer,sizeof(buffer),"SELECT x,y,z,name FROM bustops");
	mysql_query(buffer);
	mysql_store_result();
	new num = 0;
	while(mysql_fetch_row(buffer,"|"))
	{
		sscanf(buffer,"p<|>fffs[64]",BusStop[num][busPosX],BusStop[num][busPosY],BusStop[num][busPosZ],BusStop[num][busName]);
		new busname[128];
		format(busname,128,"Przystanek [%i] %s",num,BusStop[num][busName]);
		BusName[num] = CreateDynamic3DTextLabel(busname, COLOR_BUS,BusStop[num][busPosX], BusStop[num][busPosY], BusStop[num][busPosZ], 10.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1, -1, 0, -1,20);
		num++;
	}
	
	printf("[bus] za³adowano %i przystanków",num);
	
	mysql_free_result();
}

COMMAND:addbus(playerid,params[])
{
	if(CheckPlayerAccess(playerid) > PERMS_ADMIN)
	{
		new busname[64];
		if(sscanf(params,"s[64]",busname))
		{
			vlive_err(playerid,"/addbus [nazwa przystanku]");
			return 1;
		}
		new Float:Pos[3];
		GetPlayerPos(playerid,Pos[0],Pos[1],Pos[2]);
		new buffer[256];
		new wolny = 0;
		for(new i = 0; i < MAX_BUS_STOPS; i++)
		{
			if(BusStop[i][busPosX] <= 0)
			{
				wolny = i;
				break;
			}
		}
		
		BusStop[wolny][busPosX] = Pos[0];
		BusStop[wolny][busPosY] = Pos[1];
		BusStop[wolny][busPosZ] = Pos[2];
		format(BusStop[wolny][busName],64,"%s",busname);
		
		format(buffer,sizeof(buffer),"INSERT INTO bustops VALUES(NULL,%f,%f,%f,'%s')",Pos[0],Pos[1],Pos[2],busname);
		mysql_query(buffer);
		
		GuiInfo(playerid,"Stworzy³eœ przystanek.");
		
		
	}
	return 1;
}

COMMAND:delbus(playerid,params[])
{
	if(CheckPlayerAccess(playerid) > PERMS_ADMIN)
	{
		new Float:x,Float:y,Float:z;
		GetPlayerPos(playerid,x,y,z);
		for(new i = 0 ; i < MAX_BUS_STOPS; i++)
		{
			if(IsPlayerInRangeOfPoint(playerid,10,BusStop[i][busPosX] ,BusStop[i][busPosY] ,BusStop[i][busPosZ]))
			{
				BusStop[i][busPosX] = 0;
				BusStop[i][busPosY] = 0;
				BusStop[i][busPosZ] = 0;
				new buffer[256];
				format(buffer,sizeof(buffer),"DELETE FROM bustops WHERE name='%s'",BusStop[i][busName]);
				mysql_query(buffer);
				GuiInfo(playerid,"Usun¹³eœ przystanek.");
				
				DestroyDynamic3DTextLabel(BusName[i]);
			}
		}
	}
	return 1;
}

COMMAND:bus(playerid,params[])
{
	for(new i = 0 ; i < MAX_BUS_STOPS; i++)
	{
		if(pInfo[playerid][pMoney] < 10)
		{
			GuiInfo(playerid,"Nie masz tyle kasy.");
			return 1;
		}
		if(IsPlayerInRangeOfPoint(playerid,5,BusStop[i][busPosX],BusStop[i][busPosY],BusStop[i][busPosZ]))
		{
			new lista[1024];
			for(new b= 0; b < MAX_BUS_STOPS; b++)
			{
				if(BusStop[b][busPosX] > 0)
				{
					format(lista,sizeof(lista),"%s\n%i\t\t%s",lista,i,BusStop[b][busName]);
				}
				else
				{
					break;
				}
			}
			
			if(busEnabled == 1)
			{
				ShowPlayerDialog(playerid,DIAL_BUS,DIALOG_STYLE_LIST,"Lista przystanków",lista,"Wybierz","Zamknij");
			}
			else
			{
				GuiInfo(playerid,"(( Busy zosta³y wy³¹czone przez administratora, u¿ywaj taksówki. ))");
			}
			break;
		}
	}
	
	//ShowPlayerDialog(playerid,DIAL_BUS,DIALOG_STYLE_LIST,"Lista przystanków",lista,"Wybierz","Zamknij");
	return 1;
}