//
//
//		Vehicles Script VLIVE.PL
//
//	Known issues:
//	1. Po "odpisaniu pojazdu" buguje siê tablica gracza, zostawiaj¹c tym samym pojazd do czasu /q
//
//
// TODO:


#define MAX_VEHS 1500
#define MAX_PLAYER_VEHS 5
#define CAR_UNSPAWN_DELAY -1		//w sekundach

// kto mo¿e mieæ pojazd , typy w³aœciwieli
#define TYPE_NONE 0
#define TYPE_GROUP 1
#define TYPE_PLAYER 2
#define TYPE_FRAKCJA 3

//logowanie spawnowania
#define STATE_SPAWN 1				//pojazd spawnowany
#define STATE_UNSPAWN 2			//pojazd unspawn
#define STATE_HIT 3						//pojazd uderzy³ w cokolwiek
#define STATE_REPAIR 4				//pojazd zosta³ naprawiony
#define STATE_BLOWUP 5				//pojazd wybuch³
#define WHO_PLAYER 1					//wykonawca : gracz
#define WHO_SERVER 2					//wykonawca : system
#define WHO_ADMIN 3					//wykonawca : admin

#define ENGINE_TYPE_BASE 1
#define ENGINE_TYPE_MODIFY 2
#define ENGINE_TYPE_SPORT 3


//tablica do trzymanai wszystkich zespawnowanych wlasnie pojazdów
enum vehpars
{
	vuid,
	vsampid,
	Float:vhp,
	vowneruid,
	vownertype,
	vspawned,
	vname[32],
	vmodelid,
	Float:vx,
	Float:vy,
	Float:vz,
	Float:vangle,
	vPrzebieg,				//przebieg pojazdu  w metrach
	vcol1,
	vcol2,
	vpj,
	vplates[32],
	Float:vfuel,
	vpanels,
	vdoors,
	vlights,
	vtires,
	vworld,
	venginestate,
	vStarted,
	vAdminCar,
	vRubbishUnspawn,
	vSzyba,
	vPrzyciemniane,
	vEngine,
	vShooted,
}
new vehicle[MAX_VEHS][vehpars];

enum passengers
{
	passVehID,
	passVehSeat,
}
new vehiclePassengers[MAX_PLAYERS][passengers];

enum vehcache
{
	Float:vcx,
	Float:vcy,
	Float:vcz,
}
new VehicleCache[MAX_VEHS][vehcache];

// tablica dla kazdego gracza do trzymania posiadanych pojazdów
enum pvehpars
{
	car_uid,
	car_name[32],
}
new pveh[MAX_PLAYERS][5][pvehpars];


enum ownlesspars
{
	ovuid,
	ovname[32]
}
new ovehicle[100][ownlesspars];

new vehiclelist = mS_INVALID_LISTID;


forward GetPlayerVehicles(playerid);
forward SpawnPlayerVehicle(playerid,caruid);
forward GetFractionsVehicles();
forward InitVehicleSystem();
forward IsPlayerCarOwner(playerid,vehicleid);
forward ShowPlayerVehicles(playerid);
forward DestroyVehiclesOnDisconnect(playerid);
forward ClearPlayerVehiclesTable(playerid);
forward VehiclePark(playerid);
forward CloseNearVehicle(playerid);
forward CleanCarArray(vehicleid);
forward GetEmptyVehSlot();
forward GetEmptyVehUID();
forward GetNobodyVehicles();
forward IsPlayerPermVehicle(playerid,vehicleid);
forward AddCarTuning(vehicleid);

new SelectedVehicle[MAX_PLAYERS];
new SpawnedVehicle[MAX_PLAYERS];
new SelectedNoneVeh[MAX_PLAYERS];

new Text:TextDrawVehicleInfo[MAX_PLAYERS];

stock ClearPlayerVehiclesTable(playerid)
{
	for(new i=1; i< 5; i++)
	{
		pveh[playerid][i][car_uid] = EOS;
		pveh[playerid][i][car_name][0] = EOS;
		pveh[playerid][i][car_name][1] = EOS;
	}
}

stock SearchSpawnedVehicle(playerid)
{
	for(new i=1; i < MAX_VEHICLES ; i++)
	{
		if(GetVehicleModel(i) > 0)
		{
			if(vehicle[i][vownertype] == TYPE_PLAYER)
			{
				if(vehicle[i][vowneruid] == GetPlayerUID(playerid))
				{
					return vehicle[i][vuid];
				}
			}
		}
	}
	return 0;
}

public OnPlayerStateChange(playerid, newstate, oldstate)
{
	if(oldstate == PLAYER_STATE_PASSENGER && newstate == PLAYER_STATE_ONFOOT)
	{
		if(PlayerCache[playerid][pTaxed] == true)
		{
			PlayerCache[playerid][pTaxed] = false;
			
		}
		//pobieranie hajsu za taxy
		
		if(pInfo[playerid][pVehMask] == 1)
		{
			pInfo[playerid][pMasked] = 0;
			pInfo[playerid][pVehMask] = 0;
			statusPlayer[playerid] = 1;
		}
	}
	
	if(oldstate == PLAYER_STATE_DRIVER && newstate == PLAYER_STATE_ONFOOT) 
	{
		TextDrawHideForPlayer(playerid,TextDrawVehicleInfo[playerid]);
		GetPlayerVehicle[playerid] = EOS;
		
		if(pInfo[playerid][pVehMask] == 1)
		{
			pInfo[playerid][pMasked] = 0;
			pInfo[playerid][pVehMask] = 0;
			statusPlayer[playerid] = 1;
		}
	}
	
	if(oldstate == PLAYER_STATE_ONFOOT && newstate == PLAYER_STATE_PASSENGER) 
	{
		new vehicleid = GetPlayerVehicleID(playerid);
		if(vehicle[vehicleid][vPrzyciemniane] == 1)
		{
			if(pInfo[playerid][pMasked] == 0)
			{
				pInfo[playerid][pMasked] = 1;
				pInfo[playerid][pVehMask] = 1;
				statusPlayer[playerid] = 1;
			}
		}
	}
	
	if(oldstate == PLAYER_STATE_ONFOOT && newstate == PLAYER_STATE_DRIVER) // Player entered a vehicle as a driver
    {
        new vehicleid = GetPlayerVehicleID(playerid);
		if(vehicle[vehicleid][vPrzyciemniane] == 1)
		{
			if(pInfo[playerid][pMasked] == 0)
			{
				pInfo[playerid][pMasked] = 1;
				pInfo[playerid][pVehMask] = 1;
				statusPlayer[playerid] = 1;
			}
		}
		
		new seat = GetPlayerVehicleSeat(playerid);
		if(vehicleid > 0)
		{
			if(seat == 0)
			{
				if(vehicle[vehicleid][vownertype] == 1)
				{
					if(vehicle[vehicleid][vowneruid] == GROUP_TYPE_RUBBISH_UID)
					{
						if(pInfo[playerid][pHasPrawkoC] <= 0)
						{
							RemovePlayerFromVehicle(playerid);
							GuiInfo(playerid,"Nie posiadasz odpowiedniego prawa jazdy by prowadziæ ten pojazd.");
						}
					}
				}
				
				GetPlayerVehicle[playerid] = vehicleid;
				if(pInfo[playerid][blockVehicles] > 0)
				{
					RemovePlayerFromVehicle(playerid);
					GuiInfo(playerid,"Posiadasz aktywn¹ blokadê prowadzenia pojazdów.");
				}
				
				if(IsVehicleBlocked(vehicleid))
				{
					RemovePlayerFromVehicle(playerid);
					GuiInfo(playerid,"Pojazd ma za³o¿on¹ blokadê na ko³o.");
				}
				
				//kick za nieautoryzowane
				new engine, lights, alarm, doors, bonnet, boot, objective;
				GetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, bonnet, boot, objective);
				if(doors == 1)
				{
					system_kick(playerid,"Nieautoryzowane wejscie do pojazdu");
				}
				
				new info[256];
				
				if(vehicle[vehicleid][vEngine] == ENGINE_TYPE_BASE)
				{
					SetPlayerSpeedCap( playerid, 0.6);
				}
				else if(vehicle[vehicleid][vEngine] == ENGINE_TYPE_MODIFY)
				{
					SetPlayerSpeedCap( playerid, 0.8);
				}
				else if(vehicle[vehicleid][vEngine] == ENGINE_TYPE_SPORT)
				{
					DisablePlayerSpeedCap( playerid );
				}
				
				//pobranie max paliwa
				new vehicleMaxFuel = 0;
				new vehicleModel = GetVehicleModel(vehicleid);
				for(new i= 0; i < 300; i++)
				{
					if(vehicleModel == VehicleData[i][vModel])
					{
						vehicleMaxFuel = VehicleData[i][vFuel];
						break;
					}
				}
				
				if(vehicleModel == 509 || vehicleModel == 510 || vehicleModel == 481)
				{
					return 1;
				}
				
				format(info, sizeof(info), "~y~Pojazd: ~w~%s\n~y~~n~Paliwo: ~w~%i/%il",vehicle[vehicleid][vname],floatround(vehicle[vehicleid][vfuel]),vehicleMaxFuel);
				TextDrawSetString(TextDrawVehicleInfo[playerid], info);
				TextDrawShowForPlayer(playerid,TextDrawVehicleInfo[playerid]);
				
				GetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, bonnet, boot, objective);
				if(engine > 0)
				{
					SetTimerEx("EngineStopIfDestroy", 1000, false, "i",vehicleid,playerid);
				}
			}
		}
    }
	return 1;
}

stock GetPlayerSpawnedVehicle(playerid)
{
	for(new i = 0; i < MAX_VEHICLES ; i ++)
	{
		if(GetVehicleModel(i) > 0)
		{
			if(vehicle[i][vownertype] == 2)
			{
				if(vehicle[i][vowneruid] == GetPlayerUID(playerid))
				{
					SpawnedVehicle[playerid] = vehicle[i][vuid];
					break;
				}
			}
		}
	}
}

public InitVehicleSystem()
{
	GetFractionsVehicles();
	
	print("[vehicles] Modu³ systemu pojazdów za³adowany.");
	
	ManualVehicleEngineAndLights();
	
	LoadVehicleText();
	
	LoadVehicles();
	
	return 1;
}

stock LoadVehicleText()
{
	for(new i=0; i < MAX_PLAYERS; i ++)
	{
		TextDrawVehicleInfo[i] = TextDrawCreate(450.000000, 350, "_");
		TextDrawAlignment(TextDrawVehicleInfo[i], 2);
		TextDrawBackgroundColor(TextDrawVehicleInfo[i], 255);
		TextDrawFont(TextDrawVehicleInfo[i], 1);
		TextDrawLetterSize(TextDrawVehicleInfo[i], 0.379999, 1.299999);
		TextDrawColor(TextDrawVehicleInfo[i], -1);
		TextDrawSetOutline(TextDrawVehicleInfo[i], 1);
		TextDrawSetProportional(TextDrawVehicleInfo[i], 1);
		TextDrawUseBox(TextDrawVehicleInfo[i], 1);
		TextDrawBoxColor(TextDrawVehicleInfo[i], 68);
		TextDrawTextSize(TextDrawVehicleInfo[i], 490.000000, 221.000000);
	}
}

public GetPlayerVehicles(playerid)		//pobieranie posiadanych pojazdów
{
	new buffer[512],retval;
	format(buffer,sizeof(buffer),"SELECT veh_uid,veh_name FROM vehicles_list WHERE veh_owner=%i and veh_ownertype=%i",GetPlayerUID(playerid),TYPE_PLAYER);
	mysql_query(buffer);
	mysql_store_result();
	if(mysql_num_rows() > 0)
	{
		new num = 1;
		while(mysql_fetch_row(buffer,"|"))
		{			
			sscanf(buffer,"p<|>is[32]",pveh[playerid][num][car_uid],pveh[playerid][num][car_name]);
			num++;
		}
		retval = 1;
	}
	else
	{
		retval = 0;
	}
	mysql_free_result();
	return retval;
}

stock ShowPlayerVehicles(playerid)		//GUI z list¹ posiadanych pojazdów
{
	new vlist[512];
	if(GetPlayerVehicles(playerid) > 0)
	{
		for(new i=1;i < 5;i++)
		{
			if(!pveh[playerid][i][car_name][0])
			{
				//koniec, dalej pojazdów ju¿ nie ma
				break;
			}
			else
			{
				if(!vlist[0])
				{
					format(vlist,sizeof(vlist),"%i\t\t%s",pveh[playerid][i][car_uid],pveh[playerid][i][car_name]);
				}
				else
				{
					format(vlist,sizeof(vlist),"%s\n%i\t\t%s",vlist,pveh[playerid][i][car_uid],pveh[playerid][i][car_name]);
				}
			}
		}
		ShowPlayerDialog(playerid,DIAL_V,DIALOG_STYLE_LIST,"Posiadane pojazdy",vlist,"Wybierz","Zamknij");
	}
	else
	{
		GuiInfo(playerid,"Nie posiadasz ¿adnego pojazdu!");
	}
	return 1;
}

public AddCarTuning(vehicleid)
{
	new buffer[256];
	format(buffer,sizeof(buffer),"SELECT item_value1,item_uid FROM core_items WHERE item_value2=%i AND item_type=%i AND item_owneruid=%i",vehicle[vehicleid][vuid],ITEM_COMPONENT,0);
	mysql_query(buffer);
	mysql_store_result();
	if(mysql_num_rows() > 0)
	{
		while(mysql_fetch_row(buffer,"|"))
		{
			new componentid,uid;
			sscanf(buffer,"p<|>ii",componentid,uid);
			AddVehicleComponent(vehicleid, componentid);
		}
	}
	else
	{
		return 0;
	}
	mysql_free_result();
	return 0;
}

public GetFractionsVehicles()
{
	// przy starcie serwera spawnuje pojazdy frakcyjne
	
	return 1;
}

public IsPlayerCarOwner(playerid,vehicleid)
{
	new buffer[256];
	format(buffer,sizeof(buffer),"SELECT veh_uid FROM vehicles_list WHERE veh_uid=%i and veh_owner=%i",vehicle[vehicleid][vuid],GetPlayerUID(playerid));
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

stock SaveVehicle(vehicleid)
{
	if(vehicle[vehicleid][vuid] > 0)
	{
		new buffer[256];
		format(buffer,sizeof(buffer),"UPDATE vehicles_list SET veh_fuel = '%f' , veh_przebieg=%i WHERE veh_uid=%i",vehicle[vehicleid][vfuel],vehicle[vehicleid][vPrzebieg],vehicle[vehicleid][vuid]);
		mysql_query(buffer);
		return 1;
	}
	else
	{
		return 0;
	}
}

public VehiclePark(playerid)
{
	new buffer[256], Float:x,Float:y,Float:z,Float:angle;
	if(IsPlayerInAnyVehicle(playerid))
	{
		new vehicleid = GetPlayerVehicleID(playerid);
		new pmiejsce = GetPlayerVehicleSeat(playerid);
		if (pmiejsce == 0)
		{
			if(vehicleid > 0)
			{
				if(IsPlayerPermVehicle(playerid,vehicleid) == 1)
				{
					new engine, lights, alarm, doors, bonnet, boot, objective;
					GetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, bonnet, boot, objective);
					
					new vehModel = GetVehicleModel(vehicleid);
					
					if(vehModel == 509 || vehModel == 510 || vehModel == 481)
					{
						//rower
						GetVehiclePos(vehicleid,x,y,z);
						GetVehicleZAngle(vehicleid,angle);
						new world = GetVehicleVirtualWorld(vehicleid);
						
						format(buffer,sizeof(buffer),"UPDATE vehicles_list SET veh_x=%f, veh_y = %f, veh_z = %f, veh_angle=%f,veh_world=%i WHERE veh_uid=%i",x,y,z,angle,world,vehicle[vehicleid][vuid]);
						mysql_query(buffer);
						GuiInfo(playerid,"Pojazd zosta³ zaparkowany.");
						
						return 1;
					}
					
					if(engine > 0)
					{
						GuiInfo(playerid,"Aby zaparkowaæ pojazd, musisz go zgasiæ.");
						return 0;
					}
					else
					{									
						GetVehiclePos(vehicleid,x,y,z);
						GetVehicleZAngle(vehicleid,angle);
						new world = GetVehicleVirtualWorld(vehicleid);
						
						format(buffer,sizeof(buffer),"UPDATE vehicles_list SET veh_x=%f, veh_y = %f, veh_z = %f, veh_angle=%f,veh_world=%i WHERE veh_uid=%i",x,y,z,angle,world,vehicle[vehicleid][vuid]);
						mysql_query(buffer);
						GuiInfo(playerid,"Pojazd zosta³ zaparkowany.");
						return 2;
					}
				}
				else
				{
					GuiInfo(playerid,"Nie jesteœ w³aœcicielem tego pojazdu.");
				}
			}
		}
		else
		{
			GuiInfo(playerid,"Aby zaparkowaæ ten pojazd musisz byæ jego kierowc¹.");
		}
	}
	else
	{
		GuiInfo(playerid,"Musisz siedzieæ w pojeŸdzie by u¿yæ tej komendy.");
	}
	return 1;
}

//-- 	najwazniejsze publiki

public OnVehicleDamageStatusUpdate(vehicleid, playerid)
{
	if(vehicle[vehicleid][vShooted] == 1)
	{
		vehicle[vehicleid][vShooted] = 0;
		SetVehicleHealth(vehicleid,vehicle[vehicleid][vhp]);
		return 1;
	}
	
	new panels, doors, lights, tires;	
    GetVehicleDamageStatus(vehicleid, panels, doors, lights, tires);
	new Float:hapsy;
	GetVehicleHealth(vehicleid,hapsy);
	new Float:oldhp = vehicle[vehicleid][vhp];
	
	
	if(oldhp - hapsy > 50)
	{
		//PlayerNarracja(playerid,"Wystrzeli³y poduszki powietrzne.");
		
		new loss = random(2);
		switch(loss)
		{
			case 0:
			{
				//PlayerNarracja(playerid,"Na jezdni gromadzi siê coraz to wiêksza plama jakiejœ cieczy wydobywaj¹cej siê z pojazdu.");
			}
			case 1:
			{
				//PlayerNarracja(playerid,"Spod maski wydobywa siê niewielka smu¿ka bia³ego dymu.");
			}
		}
	}
	
	
	if( hapsy > vehicle[vehicleid][vhp])
	{
		SetVehicleHealth(vehicleid,vehicle[vehicleid][vhp]);
		UpdateVehicleDamageStatus(vehicleid, vehicle[vehicleid][vpanels] , vehicle[vehicleid][vdoors], vehicle[vehicleid][vlights], vehicle[vehicleid][vtires]);
		return 1;
	}
	
	vehicle[vehicleid][vhp] = hapsy;
	vehicle[vehicleid][vpanels] = panels;
	vehicle[vehicleid][vdoors] = doors;
	vehicle[vehicleid][vlights] = lights;
	vehicle[vehicleid][vtires] = tires;
	
	new buffer[256];
	format(buffer,sizeof(buffer),"UPDATE vehicles_list SET dm_panels=%i, dm_doors=%i,dm_lights=%i,dm_tires=%i,veh_hp=%f WHERE veh_uid=%i",panels,doors,lights,tires,hapsy,vehicle[vehicleid][vuid]);
	mysql_query(buffer);
	
	new rok,miesiac,dzien,godzina,minuta,sekunda,data[16],time[16];
	gettime(godzina,minuta,sekunda);
	getdate(rok,miesiac,dzien);
	format(time,sizeof(time),"%i:%i:%i",godzina,minuta,sekunda);
	format(data,sizeof(data),"%i-%i-%i",rok,miesiac,dzien);
	format(buffer,sizeof(buffer),"INSERT INTO log_vehdmg VALUES(NULL,%i,%i,'%s','%s',%i,%i,%f,%f)",vehicle[vehicleid][vuid],GetPlayerUID(playerid),time,data,STATE_HIT,WHO_PLAYER,oldhp,hapsy);
	mysql_query(buffer);
	return 1;
}

public OnVehicleDeath(vehicleid, killerid)
{
	new buffer[256];
	format(buffer,sizeof(buffer),"UPDATE vehicles_list SET veh_hp=350 WHERE veh_uid=%i",vehicle[vehicleid][vuid]);
	mysql_query(buffer);
	
	format(buffer,sizeof(buffer),"INSERT INTO log_vehdmg VALUES(NULL,%i,%i,'%s','%s',%i,%i,%f,%f)",vehicle[vehicleid][vuid],GetPlayerUID(killerid),timelog(),datelog(),STATE_BLOWUP,WHO_PLAYER,0,350);
	
	/*new string[250];
    format(string, sizeof(string), "Vehicle %i was destroyed. Reported by player %i.", vehicleid, killerid);
    SendClientMessageToAll(0xFFFFFFFF, string);*/
	SetTimerEx("UsunWrak", 4000, false, "ii", killerid,vehicleid);
	TextDrawHideForPlayer(killerid,TextDrawVehicleInfo[killerid]);
	
	if(IsValidObject(VehicleKogut[vehicleid]))
	{
		DestroyObject(VehicleKogut[vehicleid]);
	}
	
	return 1;
}

forward UsunWrak(killerid,vehicleid);
public UsunWrak(killerid,vehicleid)
{
	new uid = vehicle[vehicleid][vuid];
	SpawnedVehicle[killerid] = EOS;
	UnspawnVehicle(vehicleid);
	SpawnVehicle(uid);
}

public OnPlayerExitVehicle(playerid, vehicleid)
{
	return 0;
}

forward EngineTimer(playerid,vehicleid,lights, alarm, doors, bonnet, boot, objective);
public EngineTimer(playerid,vehicleid,lights, alarm, doors, bonnet, boot, objective)
{
	SetVehicleParamsEx(vehicleid, 1, lights, alarm, doors, bonnet, boot, objective);
	GameTextForPlayer(playerid, "Silnik ~g~ wlaczony", 5000, 5);
	//PlayerNarracja(playerid,"Pojazd zosta³ odpalony.");
	SetTimerEx("EngineStopIfDestroy", 5000, false, "i",vehicleid);
}

// -- komendy

COMMAND:silnik(playerid,cmd[])
{
	if(IsPlayerInAnyVehicle(playerid))
	{
		new vehicleid = GetPlayerVehicleID(playerid);
		new pmiejsce = GetPlayerVehicleSeat(playerid);
		
		new vehicleModel = vehicle[vehicleid][vmodelid];
		if(vehicleModel == 509 || vehicleModel == 510 || vehicleModel == 481)
		{
			GuiInfo(playerid,"Chcesz odpaliæ rower? No trzymajcie mnie.");
			return 1;
		}
		
		if (pmiejsce == 0)
		{
			if (IsPlayerPermVehicle(playerid,vehicleid)== 1)
			{
				new engine, lights, alarm, doors, bonnet, boot, objective;
				GetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, bonnet, boot, objective);
				
				//printf(" LOL ?  : %i",engine);
				
				if (engine == 1)
				{
					SetVehicleParamsEx(vehicleid, 0, lights, alarm, doors, bonnet, boot, objective);
					GameTextForPlayer(playerid, "Silnik ~r~ wylaczony", 5000, 5);
				}
				if(engine == 0 || engine == -1)
				{
					new Float:hapsy;
					GetVehicleHealth(vehicleid,hapsy);
					if(hapsy <= 350)
					{
						GameTextForPlayer(playerid, "Silnik w tym pojezdzie jest kompletnie~r~ zniszczony", 5000, 5);
						PlayerNarracja(playerid,"Kierowca nie móg³ odpaliæ silnika ze wzglêdu na jego zniszczenia.");
						return 1;
					}
					if(vehicle[vehicleid][vfuel] <= 0)
					{
						GameTextForPlayer(playerid, "~r~Brak paliwa!", 5000, 5);
						return 1;
					}
					else
					{
						//ta funkcja
						if(hapsy < 400)
						{
							SetTimerEx("EngineTimer", 10000, false, "iiiiiiii", playerid,vehicleid,lights, alarm, doors, bonnet, boot, objective);
							GameTextForPlayer(playerid, "Silnik jest w fatalnym stanie, ledwo co odpala", 5000, 5);
						}
						else if(hapsy < 500)
						{
							SetTimerEx("EngineTimer", 6000, false, "iiiiiiii", playerid,vehicleid,lights, alarm, doors, bonnet, boot, objective);
							GameTextForPlayer(playerid, "Silnik jest mocno uszkodzony, wiec jego odpalenie chwile potrwa", 5000, 5);
						}
						else if(hapsy < 700)
						{
							SetTimerEx("EngineTimer", 4000, false, "iiiiiiii", playerid,vehicleid,lights, alarm, doors, bonnet, boot, objective);
							GameTextForPlayer(playerid, "Silnik jest troche uszkodzony, wiec chwile sie odpala", 5000, 5);
						}
						else if(hapsy > 700)
						{
							SetTimerEx("EngineTimer", 2500, false, "iiiiiiii", playerid,vehicleid,lights, alarm, doors, bonnet, boot, objective);
							GameTextForPlayer(playerid, "Wlaczasz silnik w pojezdzie", 5000, 5);
						}
						//PublicMe(playerid,"przekrêca kluczyk w stacyjce.");
						//GameTextForPlayer(playerid, "Wlaczasz silnik w pojezdzie", 5000, 5);
						//SetTimerEx("EngineTimer", 4000, false, "iiiiiiii", playerid,vehicleid,lights, alarm, doors, bonnet, boot, objective);
					}
				}
			}
			else
			{
				GuiInfo(playerid,"Nie jesteœ posiadaczem tego pojazdu.");
			}
		}
	}
	else
	{
		GuiInfo(playerid,"Aby u¿yæ tej komendy musisz siedzieæ w jakimkolwiek pojeŸdzie.");
	}
	return 1;
}

public IsPlayerPermVehicle(playerid,vehicleid)
{
	if(CheckPlayerAccess(playerid) > PERMS_SUPPORT)
	{
		return 1;
	}
	else
	{
		switch (vehicle[vehicleid][vownertype])
		{
			case TYPE_NONE:
			{
				return 0;
			}
			case TYPE_GROUP:
			{
				new guid = vehicle[vehicleid][vowneruid];
				for(new i= 1; i<MAX_PLAYER_GROUPS; i ++)
				{
					if(pGrupa[playerid][i][pGuid] == guid)
					{
						if(pGrupa[playerid][i][permVehicles] == 1)
						{
							return 1;
						}
						else
						{
							GuiInfo(playerid,"Nie posiadasz kluczyków do tego pojazdu.");
							return 0;
						}
					}
				}				
				return 0;
			}
			case TYPE_PLAYER:
			{
				if(IsPlayerCarOwner(playerid,vehicleid) > 0)
				{
					return 1;
				}
				else
				{
					return 0;
				}
			}
		}
	}
	return 0;
}

public CloseNearVehicle(playerid)
{
	new Float:x,Float:y,Float:z;
	for (new i=0; i < MAX_VEHS; i++)
	{
		GetVehiclePos(i,x,y,z);
		if(IsPlayerInRangeOfPoint(playerid, 3.5,x,y,z))
		{
			if(IsPlayerPermVehicle(playerid,i) == 1)
			{
				new engine, lights, alarm, doors, bonnet, boot, objective;
				GetVehicleParamsEx(i, engine, lights, alarm, doors, bonnet, boot, objective);
				ApplyAnimation(playerid, "INT_HOUSE", "wash_up",4.1, 0, 0, 0, 0, 0, 1);
				
				if(doors == 0)
				{
					SetVehicleParamsEx(i, engine, lights, alarm, 1, bonnet, boot, objective);
					GameTextForPlayer(playerid, "Zamek w pojezdzie~r~ zamkniety", 5000, 5);
				}
				if(doors == 1 || doors == -1)
				{
					SetVehicleParamsEx(i, engine, lights, alarm, 0, bonnet, boot, objective);
					GameTextForPlayer(playerid, "Zamek w pojezdzie~g~ otwarty", 5000, 5);
				}
			}
			else
			{
				GuiInfo(playerid,"Nie jesteœ w³aœcicielem tego pojazdu.");
			}
			
			break;
		}
	}
	return 1;
}

public GetEmptyVehSlot()
{
	new freeslot;
	for (new i = 0; i < MAX_VEHS; i++)
	{
		if(GetVehicleModel(i) > 0)
		{
			//nie robimy nic
		}
		else
		{
			freeslot = i;
			break;
		}
	}
	return freeslot;
}

public GetEmptyVehUID()
{
	new buffer[256];
	format(buffer,sizeof(buffer),"SELECT veh_uid FROM vehicles_list");
	mysql_query(buffer);
	mysql_store_result();
	//new rows = mysql_num_rows();
	mysql_free_result();
}

stock CleanCarArray(vehicleid)
{
	vehicle[vehicleid][vuid] = EOS;
	vehicle[vehicleid][vsampid]= EOS;
	vehicle[vehicleid][vhp]= EOS;
	vehicle[vehicleid][vowneruid]= EOS;
	vehicle[vehicleid][vownertype]= EOS;
	vehicle[vehicleid][vspawned]= EOS;
	vehicle[vehicleid][vname][0]= EOS;
	vehicle[vehicleid][vmodelid]= EOS;
	vehicle[vehicleid][vx]= EOS;
	vehicle[vehicleid][vy]= EOS;
	vehicle[vehicleid][vz]= EOS;
	vehicle[vehicleid][vangle]= EOS;
	vehicle[vehicleid][vcol1]= EOS;
	vehicle[vehicleid][vcol2]= EOS;
	vehicle[vehicleid][vplates][0]= EOS;
	vehicle[vehicleid][vfuel]= EOS;
	vehicle[vehicleid][vpanels]= EOS;
	vehicle[vehicleid][vlights]= EOS;
	vehicle[vehicleid][vdoors]= EOS;
	vehicle[vehicleid][vtires]= EOS;
	vehicle[vehicleid][vworld]= EOS;
	vehicle[vehicleid][venginestate]= EOS;
}

stock LoadVehicles()
{
	new buffer[256],gameid;
	format(buffer,sizeof(buffer),"SELECT * FROM vehicles_list WHERE veh_ownertype = 1");
	mysql_query(buffer);
	mysql_store_result();
	while(mysql_fetch_row(buffer,"|"))
	{
		gameid++;
		new pusty;
		sscanf(buffer,"p<|>iifiiis[32]iffffiiis[24]fiiiiiiiii",
		vehicle[gameid][vuid],
		pusty,
		vehicle[gameid][vhp],
		vehicle[gameid][vowneruid],
		vehicle[gameid][vownertype],
		vehicle[gameid][vspawned],
		vehicle[gameid][vname],
		vehicle[gameid][vmodelid],
		vehicle[gameid][vx],
		vehicle[gameid][vy],
		vehicle[gameid][vz],
		vehicle[gameid][vangle],
		vehicle[gameid][vcol1],
		vehicle[gameid][vcol2],
		vehicle[gameid][vpj],
		vehicle[gameid][vplates],
		vehicle[gameid][vfuel],
		vehicle[gameid][vpanels],
		vehicle[gameid][vdoors],
		vehicle[gameid][vlights],
		vehicle[gameid][vtires],
		vehicle[gameid][vworld],
		vehicle[gameid][vPrzebieg],
		vehicle[gameid][vStarted],
		vehicle[gameid][vPrzyciemniane],
		vehicle[gameid][vEngine]);
		
		vehicle[gameid][vAdminCar] = 0;

		CreateVehicle(vehicle[gameid][vmodelid], vehicle[gameid][vx], vehicle[gameid][vy], vehicle[gameid][vz], vehicle[gameid][vangle], vehicle[gameid][vcol1], vehicle[gameid][vcol2], CAR_UNSPAWN_DELAY);
		
		SetVehicleHealth(gameid,vehicle[gameid][vhp]);
		new engine, lights, alarm, doors, bonnet, boot, objective;
		GetVehicleParamsEx(gameid, engine, lights, alarm, doors, bonnet, boot, objective);
		
		if(vehicle[gameid][vowneruid] == GROUP_TYPE_PIZZA_UID)
		{
			SetVehicleParamsEx(gameid, engine, lights, alarm, 0, bonnet, boot, objective);
		}
		else
		{
			SetVehicleParamsEx(gameid, engine, lights, alarm, 1, bonnet, boot, objective);
		}
		
		new vehicleModel = vehicle[gameid][vmodelid];
		if(vehicleModel == 509 || vehicleModel == 510 || vehicleModel == 481)
		{
			SetVehicleParamsEx(gameid, 1, lights, alarm, 0, bonnet, boot, objective);
		}
		
		SetVehicleVirtualWorld(gameid,vehicle[gameid][vworld] );
		
		ChangeVehiclePaintjob(gameid,vehicle[gameid][vpj]);
		
		UpdateVehicleDamageStatus(gameid, vehicle[gameid][vpanels], vehicle[gameid][vdoors], vehicle[gameid][vlights], vehicle[gameid][vtires]);
	
		if(!vehicle[gameid][vplates][0])
		{
			SetVehicleNumberPlate(gameid,"_"); 
		}
		else
		{
			SetVehicleNumberPlate(gameid,vehicle[gameid][vplates]); 
		}
	}
}

stock RespawnVehicle(vehicleid)
{
	new uid = vehicle[vehicleid][vuid];
	UnspawnVehicle(vehicleid);
	SpawnVehicle(uid);
}

stock LoadVehicle(gameid,uid)
{
	new buffer[256];
	format(buffer,sizeof(buffer),"SELECT * FROM vehicles_list WHERE veh_uid=%i",uid);
	mysql_query(buffer);
	mysql_store_result();
	while(mysql_fetch_row(buffer,"|"))
	{
		new pusty;
		sscanf(buffer,"p<|>iifiiis[32]iffffiiis[24]fiiiiiiiii",
		vehicle[gameid][vuid],
		pusty,
		vehicle[gameid][vhp],
		vehicle[gameid][vowneruid],
		vehicle[gameid][vownertype],
		vehicle[gameid][vspawned],
		vehicle[gameid][vname],
		vehicle[gameid][vmodelid],
		vehicle[gameid][vx],
		vehicle[gameid][vy],
		vehicle[gameid][vz],
		vehicle[gameid][vangle],
		vehicle[gameid][vcol1],
		vehicle[gameid][vcol2],
		vehicle[gameid][vpj],
		vehicle[gameid][vplates],
		vehicle[gameid][vfuel],
		vehicle[gameid][vpanels],
		vehicle[gameid][vdoors],
		vehicle[gameid][vlights],
		vehicle[gameid][vtires],
		vehicle[gameid][vworld],
		vehicle[gameid][vPrzebieg],
		vehicle[gameid][vStarted],
		vehicle[gameid][vPrzyciemniane],
		vehicle[gameid][vEngine]);
		
		vehicle[gameid][vAdminCar] = 0;
		
		vehicle[gameid][vSzyba] = 0;
		
		SetVehicleHealth(gameid,vehicle[gameid][vhp]);
		new engine, lights, alarm, doors, bonnet, boot, objective;
		GetVehicleParamsEx(gameid, engine, lights, alarm, doors, bonnet, boot, objective);
		
		if(vehicle[gameid][vowneruid] == GROUP_TYPE_PIZZA_UID)
		{
			SetVehicleParamsEx(gameid, engine, lights, alarm, 0, bonnet, boot, objective);
		}
		else
		{
			SetVehicleParamsEx(gameid, engine, lights, alarm, 1, bonnet, boot, objective);
		}
		
		//SetVehicleParamsEx(gameid, engine, lights, alarm, 1, bonnet, boot, objective);
		
		new vehicleModel = vehicle[gameid][vmodelid];
		if(vehicleModel == 509 || vehicleModel == 510 || vehicleModel == 481)
		{
			SetVehicleParamsEx(gameid, 1, lights, alarm, 0, bonnet, boot, objective);
		}
		
		SetVehicleVirtualWorld(gameid,vehicle[gameid][vworld] );
		
		ChangeVehiclePaintjob(gameid,vehicle[gameid][vpj]);
		
		UpdateVehicleDamageStatus(gameid, vehicle[gameid][vpanels], vehicle[gameid][vdoors], vehicle[gameid][vlights], vehicle[gameid][vtires]);
		
		//dodac labelka z opisami wozow
		UpdateDynamic3DTextLabelText(Text3D:VehicleDesc[gameid], COLOR_DO, " ");
	
		if(!vehicle[gameid][vplates][0])
		{
			SetVehicleNumberPlate(gameid,"_"); 
		}
		else
		{
			SetVehicleNumberPlate(gameid,vehicle[gameid][vplates]); 
		}
		
		if(vehicle[gameid][vowneruid] == GROUP_TYPE_PIZZA_UID)
		{
			//jezeli to ten motor do pizzy
			UpdateVehicleDamageStatus(gameid, 0, 0, 0, 0);
			SetVehicleHealth(gameid,1000);
			vehicle[gameid][vhp] = 1000;
		}
	}
}

stock ShowOwnerlessVehs(playerid)
{
	new buffer[256],lista[1024];
	format(buffer,sizeof(buffer),"SELECT veh_uid,veh_name FROM vehicles_list WHERE veh_owner=0 AND veh_ownertype=0 AND veh_spawned = 0");
	mysql_query(buffer);
	mysql_store_result();
	new num = 1;
	while(mysql_fetch_row(buffer,"|"))
	{
		new uid,name[32];
		sscanf(buffer,"p<|>is[32]",uid,name);
		ovehicle[num][ovuid] = uid;
		ovehicle[num][ovname] = name;
		format(lista,sizeof(lista),"%s\n%i\t\t%s",lista,uid,name);
		num++;
	}
	mysql_free_result();
	ShowPlayerDialog(playerid,DIAL_AVOLIST,DIALOG_STYLE_LIST,"Admin Control Panel » Vehicles Without Owner",lista,"Wybierz","Zamknij");
	return 1;
}

stock SpawnVehicle(uid)
{
	new buffer[256];
	format(buffer,sizeof(buffer),"SELECT veh_model,veh_x,veh_y,veh_z,veh_angle,veh_col1,veh_col2 FROM vehicles_list WHERE veh_uid=%i",uid);
	mysql_query(buffer);
	new modelid, Float:posx, Float:posy, Float:posz, Float:posa, color1, color2;
	mysql_store_result();
	printf("[load] Wczytujê statystyki pojazdu...");
	
	while(mysql_fetch_row(buffer,"|"))
	{
		sscanf(buffer,"p<|>iffffii",modelid,posx,posy,posz,posa,color1,color2);
	}
	
	mysql_free_result();
	new gameid = CreateVehicle(modelid, posx, posy, posz, posa, color1, color2, CAR_UNSPAWN_DELAY);
	
	LoadVehicle(gameid, uid);
	
	mysql_free_result();
	
	AddCarTuning(gameid); 			//dobre miejsce?
	return 1;
}

stock DeleteVehicle(vehicleid)
{
	new buffer[256];
	format(buffer,sizeof(buffer),"UPDATE vehicles_list SET veh_spawned=0 , veh_sampid=0 WHERE veh_uid=%i",vehicle[vehicleid][vuid]);
	mysql_query(buffer);
	CleanCarArray(vehicleid);
	DestroyVehicle(vehicleid);
	printf("[admin/vehs] Administrator usun¹³ auto o sampid : ( %i )",vehicleid);
}

stock GetVehicleSpeed(vehid)
{
    new Float:vaX, Float:vaY, Float:vaZ;
    GetVehicleVelocity(vehid, vaX, vaY, vaZ);
    return floatround(floatsqroot(vaX*vaX + vaY*vaY + vaZ*vaZ));
}

forward  EngineStopIfDestroy(vehicleid);
public EngineStopIfDestroy(vehicleid)
{	
	new vehicleModel = vehicle[vehicleid][vmodelid];
	
	if(vehicleModel == 509 || vehicleModel == 510 || vehicleModel == 481)
	{
		return 1;
	}
	
	new playerid = INVALID_PLAYER_ID;
	for(new i = 0 ; i < MAX_PLAYERS; i ++)
	{
		if(IsPlayerInVehicle(i,vehicleid))
		{
			playerid = i;
			break;
		}
	}
	new engine, lights, alarm, doors, bonnet, boot, objective;
	GetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, bonnet, boot, objective);
	if(engine > 0)
	{
		if(vehicle[vehicleid][vfuel] <= 0)
		{
			//zgaœ silnik
			SetVehicleParamsEx(vehicleid, 0, lights, alarm, doors, bonnet, boot, objective);
			GameTextForPlayer(playerid, "~r~Paliwo sie skonczylo!", 5000, 5);
		}
		else
		{
			//dodaj kilometry
			if(vehicle[vehicleid][vworld] != 0)
			{
				//nic póki co
			}
			else
			{
				new Float:distance = GetVehicleDistanceFromPoint(vehicleid,VehicleCache[vehicleid][vcx],VehicleCache[vehicleid][vcy],VehicleCache[vehicleid][vcz]);
				new distance_int = floatround(distance);
				vehicle[vehicleid][vPrzebieg] = vehicle[vehicleid][vPrzebieg]+distance_int;
				GetVehiclePos(vehicleid,VehicleCache[vehicleid][vcx],VehicleCache[vehicleid][vcy],VehicleCache[vehicleid][vcz]);
			}
			
			
			
			//odejmij paliwko
			if(GetVehicleSpeed(vehicleid) < 1)
			{
				vehicle[vehicleid][vfuel] = vehicle[vehicleid][vfuel] - (2 *0.002);
			}
			else if(GetVehicleSpeed(vehicleid) >=1 && GetVehicleSpeed(vehicleid) <= 40)
			{
				vehicle[vehicleid][vfuel] = vehicle[vehicleid][vfuel] - (2 * 0.0025);
			}
			else if(GetVehicleSpeed(vehicleid) >=41 && GetVehicleSpeed(vehicleid) <= 80)
			{
				vehicle[vehicleid][vfuel] = vehicle[vehicleid][vfuel] - (2 * 0.005);
			}
			else if(GetVehicleSpeed(vehicleid) >=81 && GetVehicleSpeed(vehicleid) <= 120)
			{
				vehicle[vehicleid][vfuel] = vehicle[vehicleid][vfuel] - (2 * 0.0075);
			}
			else if(GetVehicleSpeed(vehicleid) >=121 && GetVehicleSpeed(vehicleid) <= 150)
			{
				vehicle[vehicleid][vfuel] = vehicle[vehicleid][vfuel] - (2 * 0.01);
			}
			else
			{
				vehicle[vehicleid][vfuel] = vehicle[vehicleid][vfuel] - (2 * 0.0125);
			}
			if(GetPlayerVehicle[playerid] == vehicleid)
			{
				//pobranie max paliwa
				new vehicleMaxFuel = 0;
				//new vehicleModel = GetVehicleModel(vehicleid);
				for(new i= 0; i < 300; i++)
				{
					if(vehicleModel == VehicleData[i][vModel])
					{
						vehicleMaxFuel = VehicleData[i][vFuel];
						break;
					}
				}
				
				
				new info[256];
				format(info, sizeof(info), "~y~Pojazd: ~w~%s ~n~ ~y~Paliwo: ~w~%i/%il",vehicle[vehicleid][vname],floatround(vehicle[vehicleid][vfuel]),vehicleMaxFuel);
				TextDrawSetString(TextDrawVehicleInfo[playerid], info);
			}
			
		}
		
		new Float:tHP;
		GetVehicleHealth(vehicleid,tHP);
		
		if(tHP > 899)
		{
			new panelss,doorss,lightss,tiress;
			GetVehicleDamageStatus(vehicleid,panelss,doorss,lightss,tiress);
			
			UpdateVehicleDamageStatus(vehicleid, 0, 0, 0, tiress);
		}
		
		if(vehicle[vehicleid][vhp] < 600)
		{
			new loss = random(12);
			switch(loss)
			{
				case 7:
				{
					if(vehicle[vehicleid][vAdminCar] > 0)
					{
					
					}
					else
					{
						SetVehicleParamsEx(vehicleid, 0, lights, alarm, doors, bonnet, boot, objective);
						PlayerNarracja(playerid,"Uszkodzony ju¿ silnik zgas³ na skutek odniesionych obra¿eñ.");
					}
				}
			}
		}
		if(GetPlayerVehicle[playerid] == vehicleid)
		{
			SetTimerEx("EngineStopIfDestroy", 1000, false, "i",vehicleid,playerid);
		}
	}
	else
	{
		//koniec, wy³¹czy³ silnik wiêc timer wymiera
	}
	return 1;
}

stock UnspawnVehicle(vehicleid)
{
	if(IsValidObject(VehicleKogut[vehicleid]))
	{
		DestroyObject(VehicleKogut[vehicleid]);
	}
	new buffer[256];
	format(buffer,sizeof(buffer),"UPDATE vehicles_list SET veh_spawned=0 , veh_sampid=0 WHERE veh_uid=%i",vehicle[vehicleid][vuid]);
	mysql_query(buffer);
	if(SaveVehicle(vehicleid) > 0)
	{
		CleanCarArray(vehicleid);
		DestroyVehicle(vehicleid);
	}
}

stock ShowVehicleUpgrades(playerid)
{
	if(IsPlayerInAnyVehicle(playerid))
	{
		new vehicleid = GetPlayerVehicleID(playerid);
		if(IsPlayerCarOwner(playerid,vehicleid))
		{
			//jest posiadaczem, gut
			new buffer[256],lista[512];
			new vehicleUID = vehicle[vehicleid][vuid];
			format(buffer,sizeof(buffer),"SELECT item_uid,item_name FROM core_items WHERE item_value2 = %i AND item_owneruid = 0 ",vehicleUID);
			mysql_query(buffer);
			mysql_store_result();
			if(mysql_num_rows() <= 0)
			{
				mysql_free_result();
				GuiInfo(playerid,"Ten pojazd nie posiada zamontowanych czêœci.");
				return 0;
			}
			while(mysql_fetch_row(buffer,"|"))
			{
				new uid,nazwa[32];
				sscanf(buffer,"p<|>is[64]",uid,nazwa);
				format(lista,sizeof(lista),"%s\n%i\t\t%s",lista,uid,nazwa);
			}
			mysql_free_result();
			
			ShowPlayerDialog(playerid,DIAL_TUNING,DIALOG_STYLE_LIST,"Czêœci zamontowane w pojeŸdzie",lista,"Wybierz","Anuluj");
			return 1;
		}
		else
		{
			GuiInfo(playerid,"Nie jesteœ w³aœcicielem tego pojazdu.");
			return 0;
		}
	}
	else
	{
		GuiInfo(playerid,"Nie jesteœ w ¿adnym pojeŸdzie.");
		return 0;
	}
}

stock NextEmptySlot()
{
	new wolnyslot;
	for(new i=1;i< MAX_VEHS; i++)
	{
		new Float:testfloat;
		GetVehicleHealth(i,testfloat);
		new testint = floatround(testfloat);
		if (testint > 0)
		{
				
		}
		else
		{
			wolnyslot = i;
			break;
		}
	}
	return wolnyslot;
}

stock GetOccupiedVehicleInfo(playerid)
{
	new vehicleid = GetPlayerVehicleID(playerid);
	if(vehicleid > 0)
	{
		new paliwo_int = floatround(vehicle[vehicleid][vfuel]);
		new hp_int = floatround(vehicle[vehicleid][vhp]);
		
		//pobranie max paliwa
		new vehicleMaxFuel = 0;
		new vehicleModel = GetVehicleModel(vehicleid);
		for(new i= 0; i < 300; i++)
		{
			if(vehicleModel == VehicleData[i][vModel])
			{
				vehicleMaxFuel = VehicleData[i][vFuel];
				break;
			}
		}
		
		new vinfo[512],title[128];
		format(vinfo,sizeof(vinfo),"Model: \t\t\t%i",vehicle[vehicleid][vmodelid]);
		format(vinfo,sizeof(vinfo),"%s\nKolory: \t\t\t%i/%i",vinfo,vehicle[vehicleid][vcol1],vehicle[vehicleid][vcol2]);
		//format(vinfo,sizeof(vinfo),"%s\nTyp w³aœciciela: \t\t%i/%i",vinfo,vehicle[vehicleid][vcol1],vehicle[vehicleid][vcol2]);
		format(vinfo,sizeof(vinfo),"%s\nUID w³aœciciela: \t\t%i",vinfo,vehicle[vehicleid][vowneruid]);
		format(vinfo,sizeof(vinfo),"%s\nStan techniczny: \t\t%i",vinfo,hp_int);
		format(vinfo,sizeof(vinfo),"%s\nPaliwo: \t\t\t%i/%il",vinfo,paliwo_int,vehicleMaxFuel);
		format(vinfo,sizeof(vinfo),"%s\nRejestracja: \t\t\t%s",vinfo,vehicle[vehicleid][vplates]);
		format(vinfo,sizeof(vinfo),"%s\nPrzebieg: \t\t\t%i km",vinfo,vehicle[vehicleid][vPrzebieg]/1000);
		
		format(title,sizeof(title),"%s (SampID: %i) (UID: %i)",vehicle[vehicleid][vname],vehicleid,vehicle[vehicleid][vuid]);
		ShowPlayerDialog(playerid,404,DIALOG_STYLE_LIST,title,vinfo,"OK","");
	}
}

forward UnspawnPlayerVehicle(vehicleid);
public UnspawnPlayerVehicle(vehicleid)
{
	if(VehicleUnspawnTimer[vehicleid] <=0)
	{
		printf("[vehicles] Usuniêto pojazd %s (SID:%i) ",vehicle[vehicleid][vname],vehicleid);
		UnspawnVehicle(vehicleid);
	}
	else
	{
		new owneruid = vehicle[vehicleid][vowneruid];
		new czy_jest = 0;
		for(new i =0 ;i < MAX_PLAYERS; i++)
		{
			if(GetPlayerUID(i) == owneruid)
			{
				czy_jest = 1;
				break;
			}
		}
		if(czy_jest > 0)
		{
			printf("[vehicles] Wszed³ w³aœciciel pojazdu, wiêc pojazd nie zostanie unspawnowany...");
			VehicleUnspawnTimer[vehicleid] = 0;
		}
		else
		{
			//printf("TIMER 1 : %i ",VehicleUnspawnTimer[vehicleid]);
			VehicleUnspawnTimer[vehicleid]--;
			SetTimerEx("UnspawnPlayerVehicle", 1000, false, "i", vehicleid);
		}
	}
}

stock SetPlayerVehicleToUnspawn(playerid)
{
	//usuwanie pojazdów jeœli s¹
	new vehiclee = 0;
	for(new i=0; i < MAX_VEHICLES; i++)
	{
		if(vehicle[i][vownertype] == 2)
		{
			if(vehicle[i][vowneruid] == GetPlayerUID(playerid))
			{
				vehiclee = i;
				break;
			}
		}
	}
	VehicleUnspawnTimer[vehiclee] = 15*60;
	SetTimerEx("UnspawnPlayerVehicle", 1000, false, "i", vehiclee);
}

stock IsPlayerInRangeOfVehicle(playerid,vehicleid,Float:range)
{
	new playerWorld = GetPlayerVirtualWorld(playerid);
	new Float:Pos[3];
	GetVehiclePos(vehicleid,Pos[0],Pos[1],Pos[2]);
	
	if(GetVehicleVirtualWorld(vehicleid) == playerWorld)
	{
		if(IsPlayerInRangeOfPoint(playerid,range,Pos[0],Pos[1],Pos[2]))
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
	else
	{
		return 0;
	}
}

stock SetVehicleSpeed(vehicleid, Float:speed)
{
    new Float:x1, Float:y1, Float:z1, Float:x2, Float:y2, Float:z2, Float:a;
    GetVehicleVelocity(vehicleid, x1, y1, z1);
    GetVehiclePos(vehicleid, x2, y2, z2);
    GetVehicleZAngle(vehicleid, a); a = 360 - a;
    x1 = (floatsin(a, degrees) * (speed/100) + floatcos(a, degrees) * 0 + x2) - x2;
    y1 = (floatcos(a, degrees) * (speed/100) + floatsin(a, degrees) * 0 + y2) - y2;
    SetVehicleVelocity(vehicleid, x1, y1, z1);
}

COMMAND:przejazd(playerid,params[])
{
	if(IsPlayerInAnyVehicle(playerid))
	{
		new vehicleid = GetPlayerVehicleID(playerid);
		new seat = GetPlayerVehicleSeat(playerid);
		

		for(new i = 0; i < MAX_PLAYERS;i++)
		{
			if(GetPlayerVehicleID(i) == vehicleid)
			{
				vehiclePassengers[i][passVehID] = vehicleid;
				vehiclePassengers[i][passVehSeat] = GetPlayerVehicleSeat(i);
			}
		}
		
		if(seat == 0)
		{
			new playervw = GetPlayerVirtualWorld(playerid);
			for (new i = 0 ; i< MAX_DOORS; i ++ )
			{
				if(DoorInfo[i][doorCars] > 0)
				{
					if(DoorInfo[i][doorClose] == 0)
					{
						if(IsPlayerInRangeOfPoint(playerid,5.0,DoorInfo[i][doorEnterX],DoorInfo[i][doorEnterY],DoorInfo[i][doorEnterZ]))
						{
							if(playervw != DoorInfo[i][doorEnterVW])
							{
								return 1;
							}
							
							
							//gracz
							SetPlayerPos(playerid,DoorInfo[i][doorExitX],DoorInfo[i][doorExitY],DoorInfo[i][doorExitZ]);
							SetPlayerVirtualWorld(playerid,DoorInfo[i][doorExitVW]);
							SetPlayerFacingAngle( playerid, DoorInfo[i][doorExitA]);
							SetPlayerInterior(playerid,DoorInfo[i][doorExitINT]);
							OnEnterToDoor(playerid);
							PlayerCache[playerid][pAudioHandle] = Audio_PlayStreamed(playerid, DoorInfo[i][doorMuzyka]);
							
							//pojazd
							SetVehicleVirtualWorld(vehicleid, DoorInfo[i][doorExitVW]);
							SetVehiclePos(vehicleid, DoorInfo[i][doorExitX],DoorInfo[i][doorExitY],DoorInfo[i][doorExitZ]);
							SetVehicleZAngle(vehicleid, DoorInfo[i][doorExitA]);
							PutPlayerInVehicle(playerid,vehicleid,0);
							
							for(new p = 0; p < MAX_PLAYERS; p++)
							{
								if(vehiclePassengers[p][passVehID] == vehicleid)
								{
									SetPlayerVirtualWorld(p,DoorInfo[i][doorExitVW]);
									PutPlayerInVehicle(p,vehicleid,vehiclePassengers[p][passVehSeat]);
									vehiclePassengers[p][passVehID] = INVALID_VEHICLE_ID;
								}
							}
						}
						else if(IsPlayerInRangeOfPoint(playerid,2.0,DoorInfo[i][doorExitX],DoorInfo[i][doorExitY],DoorInfo[i][doorExitZ]))
						{
							if(playervw != DoorInfo[i][doorExitVW])
							{
								return 1;
							}
							
							//gracz
							SetPlayerPos(playerid,DoorInfo[i][doorEnterX],DoorInfo[i][doorEnterY],DoorInfo[i][doorEnterZ]);
							SetPlayerVirtualWorld(playerid,DoorInfo[i][doorEnterVW]);
							SetPlayerFacingAngle( playerid, DoorInfo[i][doorEnterA]);
							SetPlayerInterior(playerid,DoorInfo[i][doorEnterINT]);
							OnEnterToDoor(playerid);
							Audio_Stop(playerid, PlayerCache[playerid][pAudioHandle]);
							
							//pojazd
							SetVehicleVirtualWorld(vehicleid, DoorInfo[i][doorEnterVW]);
							SetVehiclePos(vehicleid, DoorInfo[i][doorEnterX],DoorInfo[i][doorEnterY],DoorInfo[i][doorEnterZ]);
							SetVehicleZAngle(vehicleid, DoorInfo[i][doorEnterA]);
							PutPlayerInVehicle(playerid,vehicleid,0);
							
							for(new p = 0; p < MAX_PLAYERS; p++)
							{
								if(vehiclePassengers[p][passVehID] == vehicleid)
								{
									SetPlayerVirtualWorld(p,DoorInfo[i][doorEnterVW]);
									PutPlayerInVehicle(p,vehicleid,vehiclePassengers[p][passVehSeat]);
									
									vehiclePassengers[p][passVehID] = INVALID_VEHICLE_ID;
								}
							}
						}
					}
				}
			}
		}
		else
		{
			GuiInfo(playerid,"Musisz byæ kierowc¹ tego pojazdu.");
		}
	}
	else
	{
		GuiInfo(playerid,"Nie jesteœ w aucie aby u¿yæ tej komendy.");
	}
	return 1;
}

/*COMMAND:hol(playerid,params[])
{
	new vehicleID = GetPlayerVehicleID(playerid);
	new vehicleModel = GetVehicleModel(vehicleID);
	
	if(vehicleModel == 525)
	{
		new vehiclee = GetNearVehicleAsHolownik(playerid);
		AttachTrailerToVehicle(vehiclee,vehicleID);
	}
	else
	{
		GuiInfo(playerid,"Holowaæ pojazdy mo¿na tylko dziêki holownikowi.");
	}
	
	return 1;
}	*/

COMMAND:hol(playerid,params[])
{
	new vToAttach = INVALID_VEHICLE_ID;
	if(sscanf(params,"d",vToAttach))
	{	
		vlive_err(playerid,"/hol [id pojazdu]");
		return 1;
	}
	
	new vehicleID = GetPlayerVehicleID(playerid);
	new vehicleModel = GetVehicleModel(vehicleID);
	
	if(vehicleModel == 525)
	{
		//new vehiclee = GetNearVehicleAsHolownik(playerid);
		new Float:Pos[3];
		GetVehiclePos(vToAttach,Pos[0],Pos[1],Pos[2]);
		if(IsPlayerInRangeOfPoint(playerid,4,Pos[0],Pos[1],Pos[2]))
		{
			if(GetVehicleVirtualWorld(vehicleID) != GetVehicleVirtualWorld(vToAttach))
			{
				GuiInfo(playerid,"Pojazd poza zasiêgiem.");
			}
			else
			{
				AttachTrailerToVehicle(vToAttach,vehicleID);
			}
		}
		else
		{
			GuiInfo(playerid,"Pojazd poza zasiêgiem.");
		}
	}
	else
	{
		GuiInfo(playerid,"Holowaæ pojazdy mo¿na tylko dziêki holownikowi.");
	}
	return 1;
}

/*
CMD:pdparkuj(playerid,params[])
{

}
*/

COMMAND:opusc(playerid,params[])
{
	new vehicleID = GetPlayerVehicleID(playerid);
	new vehicleModel = GetVehicleModel(vehicleID);
	
	if(vehicleModel == 525)
	{
		if(GetVehicleTrailer(vehicleID) > 0)
		{
			DetachTrailerFromVehicle(vehicleID);
		}
	}
	return 1;
}

COMMAND:maska(playerid,params[])
{
	for(new i = 0 ; i < MAX_VEHICLES; i++)
	{
		if(IsPlayerInRangeOfVehicle(playerid,i,4))
		{
			if(IsPlayerPermVehicle(playerid,i))
			{
				new engine, lights, alarm, doors, bonnet, boot, objective;
				GetVehicleParamsEx(i, engine, lights, alarm, doors, bonnet, boot, objective);
				if(bonnet == 1)
				{
					SetVehicleParamsEx(i, engine, lights, alarm, doors, 0, boot, objective);
					HudText(playerid,"maska ~r~zamknieta");
				}
				else
				{
					SetVehicleParamsEx(i, engine, lights, alarm, doors, 1, boot, objective);
					HudText(playerid,"maska ~g~otwarta");
				}
			}
		}
	}
	return 1;
}

COMMAND:szyba(playerid,params[])
{
	if(IsPlayerInAnyVehicle(playerid))
	{
		new vehicleid = GetPlayerVehicleID(playerid);
		if(vehicle[vehicleid][vSzyba] == 0)
		{
			vehicle[vehicleid][vSzyba] = 1;
			for(new i = 0 ; i < MAX_PLAYERS; i++)
			{
				if(IsPlayerInVehicle(i,vehicleid))
				{
					HudText(i,"szyba ~r~zamknieta");
				}
			}
		}
		else
		{
			vehicle[vehicleid][vSzyba] = 0;
			for(new i = 0 ; i < MAX_PLAYERS; i++)
			{
				if(IsPlayerInVehicle(i,vehicleid))
				{
					HudText(i,"szyba ~g~otwarta");
				}
			}
		}
	}
	else
	{
		GuiInfo(playerid,"Musisz byæ w pojeŸdzie by u¿yæ tej komendy.");
	}
	return 1;
}

COMMAND:bagaznik(playerid,params[])
{
	for(new i = 0 ; i < MAX_VEHICLES; i++)
	{
		if(IsPlayerInRangeOfVehicle(playerid,i,4))
		{
			if(IsPlayerPermVehicle(playerid,i))
			{
				new engine, lights, alarm, doors, bonnet, boot, objective;
				GetVehicleParamsEx(i, engine, lights, alarm, doors, bonnet, boot, objective);
				if(boot == 1)
				{
					SetVehicleParamsEx(i, engine, lights, alarm, doors, bonnet, 0, objective);
					HudText(playerid,"bagaznik ~r~zamkniety");
				}
				else
				{
					SetVehicleParamsEx(i, engine, lights, alarm, doors, bonnet, 1, objective);
					HudText(playerid,"bagaznik ~g~otwarty");
				}
			}
		}
	}
	return 1;
}

COMMAND:zp(playerid,params[])
{
	if(IsPlayerInAnyVehicle(playerid))
	{
		if(PlayerCache[playerid][pZP] > 0)
		{
			HudText(playerid,"Pasy ~r~odpiete");
			PublicMe(playerid,"odpina pasy.");
			PlayerCache[playerid][pZP] = 0;
			statusPlayer[playerid] = 1;
		}
		else
		{
			HudText(playerid,"Pasy ~g~zapiete");
			PublicMe(playerid,"zapina pasy.");
			PlayerCache[playerid][pZP] = 1;
			statusPlayer[playerid] = 1;
		}
	}
	return 1;
}

CMD:vopis(playerid,params[])
{
	if(!IsPlayerInAnyVehicle(playerid))
	{
		GuiInfo(playerid,"Musisz byæ w pojeŸdzie by u¿yæ tej komendy.");
		return 1;
	}
	
	new vehicleid = GetPlayerVehicleID(playerid);
	new seat = GetPlayerVehicleSeat(playerid);
	
	if(seat != 0)
	{
		GuiInfo(playerid, "Musisz siedzieæ na miejscu kierowcy by u¿yæ tej komendy.");
		return 1;
	}
	
	new opis[128];
	if(sscanf(params,"s[128]",opis))
	{
		GuiInfo(playerid,"Wprowadzono z³y opis.");
		return 1;
	}
	
	if(!strcmp(opis, "usun", true))
	{
		UpdateDynamic3DTextLabelText(Text3D:VehicleDesc[vehicleid], COLOR_DO, " ");
		return 1;
	}
	
	//FormatDesc(text[])
	
	if(strlen(opis) > 128 || strlen(opis) < 5)
	{
		GuiInfo(playerid,"Opis jest za d³ugi lub zbyt krótki.");
		return 1;
	}
	
	UpdateDynamic3DTextLabelText(Text3D:VehicleDesc[vehicleid], COLOR_DO, FormatDesc(opis));
	
	return 1;
}

COMMAND:wyrzuc(playerid,params[])
{
	new target;
	if(sscanf(params,"d",target))
	{
		vlive_err(playerid,"/wyrzuc [playerid]");
		return 1;
	}
	
	if(IsPlayerInAnyVehicle(playerid))
	{
		new vehicleid = GetPlayerVehicleID(playerid);
		new seat = GetPlayerVehicleSeat(playerid);
		
		if(seat == 0)
		{
			new targetvid = GetPlayerVehicleID(target);
			if(vehicleid == targetvid)
			{
				RemovePlayerFromVehicle(target);
				HudText(playerid,"wyrzuciles gracza z pojazdu");
			}
			else
			{
				GuiInfo(playerid,"Wskazany gracz nie jest w tym pojeŸdzie.");
			}
		}
		else
		{
			GuiInfo(playerid,"Musisz byæ kierowc¹ by u¿yæ tej komendy.");
		}
	}
	else
	{
		GuiInfo(playerid,"Musisz byæ w pojeŸdzie by u¿yæ tej komendy.");
	}
	
	return 1;
}

stock IsPlayerHasStartVehicle(playerid)
{
	new buffer[256];
	format(buffer,sizeof(buffer),"SELECT * FROM vehicles_list WHERE veh_owner= %i AND veh_ownertype=%i AND veh_started = 1",GetPlayerUID(playerid),TYPE_PLAYER);
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

stock GetNearVehicle(playerid)
{
	new vehicleid = INVALID_VEHICLE_ID;
	for(new i = 0 ; i < MAX_VEHICLES;i++)
	{
		new Float:Pos[3];
		GetVehiclePos(i,Pos[0],Pos[1],Pos[2]);
		if(IsPlayerInRangeOfPoint(playerid,4,Pos[0],Pos[1],Pos[2]))
		{
			vehicleid = i;
			break;
		}
	}
	return vehicleid;
}

stock GetNearVehicleAsHolownik(playerid)
{
	new vehicleid = INVALID_VEHICLE_ID;
	for(new i = 0 ; i < 1000 ;i++)
	{
		if(IsPlayerInRangeOfVehicle(playerid,i,4))
		{
			vehicleid = i;
			break;
		}
	}
	return vehicleid;
}

stock IsVehicleBlocked(vehicleid)
{
	new buffer[256];
	format(buffer,sizeof(buffer),"SELECT * FROM veh_blocks WHERE uid=%i",vehicle[vehicleid][vuid]);
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

stock NavigateVehicle(playerid)
{
	if(SpawnedVehicle[playerid] > 0)
	{
		new Float:x,Float:y,Float:z;
		for(new i = 0 ; i < MAX_VEHICLES; i++)
		{
			if(vehicle[i][vownertype] == TYPE_PLAYER)
			{
				if(vehicle[i][vowneruid] == GetPlayerUID(playerid))
				{
					GetVehiclePos(i,x,y,z);
					SetPlayerCheckpoint(playerid, x,y,z, 5.0);
					PlayerCache[playerid][pVehCheckpoint] = 1;
				}
			}
		}
	}
	else
	{
		GuiInfo(playerid,"Nie posiadasz ¿adnego zespawnowanego pojazdu.");
		return 1;
	}
	return 1;
}

public OnUnoccupiedVehicleUpdate(vehicleid, playerid, passenger_seat)
{
	new Float:VehHP;
	GetVehicleHealth(vehicleid,VehHP);
	if(VehHP < 350)
	{
		SetVehicleHealth(vehicleid,350);
		vehicle[vehicleid][vhp] = 350;
	}
}

forward forceGVR();
public forceGVR()			//funkcja do respawnu wszystkich grupowych, czysta formalnosc
{
	for(new i = 0 ; i < MAX_VEHICLES; i++)
	{
		if(vehicle[i][vownertype] == 1)
		{
			new uid = vehicle[i][vuid];
			UnspawnVehicle(i);
			SpawnVehicle(uid);
		}
	}
}