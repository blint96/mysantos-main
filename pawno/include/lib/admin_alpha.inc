//
//		Znane b³êdy :
//	1. niektóre funkcjie niepozabezpieczane przed dostêpem
//
//
//	niszczenie timerów jak disconnect
//
//	jeœli timer ju¿ istnieje to niech go nie robi dwa razy

#define AJ_X 153.979110
#define AJ_Y -1952.149414
#define AJ_Z 47.875000
#define MAX_JAILS 1000
#define MAX_ADMINS 100

#define UNAJ_X 820.849426
#define UNAJ_Y -1349.170654
#define UNAJ_Z 13.525882

#define MAX_LABELS	1000

//new AdminJail[MAX_JAILS];
new AjTimer[MAX_PLAYERS];

forward CheckPlayerAccess(playerid);
forward RepairVehicleByID(vehicleid,playerid);
forward PlayerAj(playerid);
forward KickWithMessage(playerid);
forward GetPlayerBlocks(playerid);

enum apars
{
	aSid,
	aName[MAX_PLAYER_NAME],
	aPerms,
}
new Admins[MAX_ADMINS][apars];

new PlayerText:AjInfo[MAX_PLAYERS];

public CheckPlayerAccess(playerid)	
{
	return  GetPlayerAccess(playerid);
}

stock InitAdmins()
{
	if(!fexist("admins_log.txt"))
	{
		print("[warning - admins] BRAK PLIKU NA LOGI ADMINISTRATORSKIE - BRAK ZAPISU");
	}
	
	print("[info] laduje modul administratora");
}

stock vlive_AdminLog(string[])
{
	string[0] = EOS;
	//tutaj akcja bêdzie
}

stock MySQL_log(playerid,command[])
{
	new buffer[256];
	format(buffer,sizeof(buffer),"INSERT INTO log_admincmd VALUES(NULL,'%s',%i,'%s','%s')",command,GetPlayerUID(playerid),datelog(),timelog());
	mysql_query(buffer);
	return 1;
}

stock GetAdminPermissions(playerid)
{
	new buffer[256];
	format(buffer,sizeof(buffer),"SELECT pojazdy,grupy,przedmioty,drzwi FROM uprawnienia WHERE gid=%i",pInfo[playerid][pGID]);
	mysql_query(buffer);
	mysql_store_result();
	
	if(mysql_num_rows() <= 0)
	{
		mysql_free_result();
		format(buffer,sizeof(buffer),"INSERT INTO uprawnienia (gid) VALUES(%i)",pInfo[playerid][pGID]);
		mysql_query(buffer);
		return 0;
	}
	else
	{
		while(mysql_fetch_row(buffer,"|"))
		{
			new door,car,item,grupy;
			sscanf(buffer,"p<|>iiii",car,grupy,item,door);
			pInfo[playerid][pGMCars] = car;
			pInfo[playerid][pGMGroups] = grupy;
			pInfo[playerid][pGMItems] = item;
			pInfo[playerid][pGMDoors] = door;
		}
	}
	
	mysql_free_result();
	return 1;
}

stock TruncateVehicle(vehicleid)
{
	new buffer[256];
	format(buffer,sizeof(buffer),"DELETE FROM vehicles_list WHERE veh_uid=%i",vehicle[vehicleid][vuid]);
	mysql_query(buffer);
	DestroyVehicle(vehicleid);
	CleanCarArray(vehicleid);
}

public GetPlayerBlocks(playerid)
{
	new buffer[256];
	format(buffer,sizeof(buffer),"SELECT block_ooc,block_char,block_vehs FROM core_players WHERE char_uid=%i",GetPlayerUID(playerid));
	mysql_query(buffer);
	mysql_store_result();
	while(mysql_fetch_row(buffer,"|"))
	{
		sscanf(buffer,"p<|>iii",pInfo[playerid][blockOOC],pInfo[playerid][blockChar],pInfo[playerid][blockVehicles]);
	}
	mysql_free_result();
	return 1;
}

stock ApplyDildo(playerid)
{
	// tutaj umieszczamy GIDy osób, które s¹ nam niewygodne
	if(pInfo[playerid][pGID] == 9999)
	{
		//a tutaj wciska im dildo w dupê
		SetPlayerAttachedObject(playerid, 9, 321, 1, -0.409999	,0.034001	,-0.052999	,90.9,	-50.5	,0 ,1.000000 ,1.000000 ,1.000000);
	}
}

stock ForceLatex(playerid)
{
	if(pInfo[playerid][pGID] == 0)
	{
		if(GetPlayerSkin(playerid) != 178)
		{
			SetPlayerSkin(playerid,178);
		}
	}
}


stock CheckIsJailed(playerid)
{
	new buffer[256];
	format(buffer,sizeof(buffer),"SELECT char_ajtime FROM core_players WHERE char_uid=%i",GetPlayerUID(playerid));
	mysql_query(buffer);
	mysql_store_result();
	new ajtime = mysql_fetch_int();
	if (ajtime > 0)
	{
		mysql_free_result();
		SetPlayerVirtualWorld(playerid,playerid+9999);
		SetPlayerPos(playerid,AJ_X,AJ_Y,AJ_Z);
		SetPlayerSpecialAction(playerid,SPECIAL_ACTION_CUFFED);
		//PlayerTextDrawShow(playerid,AjInfo[playerid]);
		//PlayerTextDrawSetString(playerid, AjInfo[playerid], "Jestes w AJ");
		pInfo[playerid][pAj] = ajtime;
		AjTimer[playerid] = SetTimerEx("PlayerAj", 60000, false, "i", playerid);
		
		format(buffer,sizeof(buffer),"Jesteœ umieszczony w AdminJail na %i minut.",ajtime);
		SendClientMessage(playerid,COLOR_SERVER,buffer);
		return 1;
	}
	else
	{
		mysql_free_result();
		return 0;
	}
}

stock LogAdminAction(string[])
{
	TextDrawSetString(Text:TextDrawAdminLog, string);
	
	//zapisz do pliku
	new File:afile = fopen("admins_log.txt", io_append);
	
	if(afile)
	{
		new log[512];
		format(log,sizeof(log),"\n[%s] [%s] %s",timelog(),datelog(),string);
		fwrite(afile, log);
		fclose(afile); 
	}
	else
	{
		print("[WARNING] brak mozliwosci zapisu do pliku logowan");
	}
	
	for(new i= 0 ; i < MAX_PLAYERS;i++)
	{
		if(pInfo[i][pAdmin] > 0  || CheckPlayerAccess(i) == PERMS_PROGRAMMER)
		{
			//pokaz mu
			TextDrawShowForPlayer(i,TextDrawAdminLog);
		}
	}
	
	SetTimer("ClearAdminAction", 5000, false);
	return 1;
}

forward ClearAdminAction();
public ClearAdminAction()
{
	TextDrawHideForAll(TextDrawAdminLog);
	return 1;
}

stock LoadAjText(playerid)
{
	AjInfo[playerid] = CreatePlayerTextDraw(playerid,370,330,"AJ");

	PlayerTextDrawSetShadow(playerid,AjInfo[playerid],0);
	PlayerTextDrawFont(playerid,AjInfo[playerid],2);
}

stock PlayerUnaj(playerid)
{
	new buffer[256];
	format(buffer,sizeof(buffer),"UPDATE core_players SET char_ajtime=0 WHERE char_uid=%i",GetPlayerUID(playerid));
	mysql_query(buffer);
	KillTimer(AjTimer[playerid]);
	AjTimer[playerid] = EOS;
	//PlayerTextDrawDestroy(playerid, AjInfo[playerid]);
	//PlayerTextDrawHide(playerid,AjInfo[playerid]);
	SetPlayerSpecialAction(playerid,SPECIAL_ACTION_NONE);
	SetPlayerVirtualWorld(playerid,0);
	SetPlayerPos(playerid,UNAJ_X,UNAJ_Y,UNAJ_Z);
	pInfo[playerid][pAj] = 0;
	GuiInfo(playerid,"Zosta³eœ zwolniony z AdminJail przez administratora.");
}

stock SystemPenaltyMessage(playerid,target,punish[],reason[])
{
	//textdraw
	new string[256];
	new pname[MAX_PLAYER_NAME];
	GetPlayerName(playerid,pname,sizeof(pname));
	format(string, sizeof(string), "~r~~h~%s~n~~w~Gracz: %s~n~Nadajacy: %s~n~~y~~h~%s",punish, pInfo[target][pName],  GetPlayerGlobalNickname(playerid), reason);
	TextDrawSetString(Text:TextDrawPunishInfo, string);
	TextDrawShowForAll(Text:TextDrawPunishInfo);
	SetTimer("HidePunishText",6000,false);
}

public KickWithMessage(playerid)
{
	Kick(playerid);
	return 1;
}

stock system_ck(target,reason[])
{
	new string[256],buffer[256];
	format(string, sizeof(string), "~r~~h~CK~n~~w~Gracz: %s~n~Nadajacy: System~n~~y~~h~%s", pInfo[target][pName], reason);
	TextDrawSetString(Text:TextDrawPunishInfo, string);
	TextDrawShowForAll(Text:TextDrawPunishInfo);
	SetTimer("HidePunishText",6000,false);
	format(buffer,sizeof(buffer),"UPDATE core_players SET block_char=%i WHERE char_uid=%i",BLOCK_CHAR,GetPlayerUID(target));
	mysql_query(buffer);	
	SetTimerEx("KickWithMessage", 500, false, "i", target);
	printf("[kick] Gracz %s (UID: %i) zostal wyrzucony z serwera - powod: CK",pInfo[target][pName],GetPlayerUID(target));
	
	
	format(buffer,sizeof(buffer),"INSERT INTO penalties VALUES(NULL,'%s','%s',%i,0,'%s','%s',0)",getStringDate(),getStringTime(),GetPlayerUID(target),"akceptujsmierc","Blokada postaci");
	mysql_query(buffer);
}

stock GetFreeAdminSlot()
{
	new free;
	for(new i=0; i < MAX_ADMINS; i++)
	{
		if(Admins[i][aSid] != 0)
		{
			
		}
		else
		{
			free = i;
			break;
		}
	}
	return free;
}

public PlayerAj(playerid)
{
	new buffer[256];
	format(buffer,sizeof(buffer),"SELECT char_ajtime FROM core_players WHERE char_uid=%i",GetPlayerUID(playerid));
	mysql_query(buffer);
	mysql_store_result();
	new ajtime = mysql_fetch_int();
	
	pInfo[playerid][pAj] = ajtime;
	if(ajtime <= 0)
	{
		//wychodzi
		SetPlayerVirtualWorld(playerid,0);
		SetPlayerPos(playerid,UNAJ_X,UNAJ_Y,UNAJ_Z);
		KillTimer(AjTimer[playerid]);
		AjTimer[playerid] = EOS;
		//PlayerTextDrawDestroy(playerid, AjInfo[playerid]);
		//PlayerTextDrawHide(playerid,AjInfo[playerid]);
		SetPlayerSpecialAction(playerid,SPECIAL_ACTION_NONE);
		return 1;
	}
	format(buffer,sizeof(buffer),"Koniec AJ : %i min",ajtime);
	//PlayerTextDrawSetString(playerid, AjInfo[playerid], buffer);
	new ou[64];
	format(ou,sizeof(ou),"Koniec AJ :~r~%i min",ajtime);
	GameTextForPlayer(playerid, ou, 60000, 5);
	mysql_free_result();
	format(buffer,sizeof(buffer),"UPDATE core_players SET char_ajtime=char_ajtime-1 WHERE char_uid=%i",GetPlayerUID(playerid));
	mysql_query(buffer);
	AjTimer[playerid] = SetTimerEx("PlayerAj", 60000, false, "i", playerid);
	return 1;
}

public RepairVehicleByID(vehicleid,playerid)
{
	if(CheckPlayerAccess(playerid) >= PERMS_SUPPORT)
	{
		new Float:testhp;
		GetVehicleHealth(vehicleid,testhp);
		if(testhp > 0)
		{
			new bufferr[256],aname[MAX_PLAYER_NAME];
			GetPlayerName(playerid,aname,sizeof(aname));
			format(bufferr,sizeof(bufferr),"UPDATE vehicles_list SET veh_hp=1000, dm_panels = 0, dm_doors = 0,dm_lights = 0, dm_tires = 0 WHERE veh_uid=%i",vehicle[vehicleid][vuid]);
			mysql_query(bufferr);
			printf("[admin/vehicle] administrator %s naprawi³ pojazd %i", aname,vehicleid);
			SetVehicleHealth(vehicleid,1000);
			UpdateVehicleDamageStatus(vehicleid, 0, 0, 0, 0);
			SendClientMessage(playerid,COLOR_SERVER,"[admin/vehicle] Naprawi³eœ pojazd.");
			vehicle[vehicleid][vhp] = 1000;
			vehicle[vehicleid][vdoors] = 0;
			vehicle[vehicleid][vtires] = 0;
			vehicle[vehicleid][vpanels] = 0;
			vehicle[vehicleid][vlights] = 0;
			
		}
		else
		{
			SendClientMessage(playerid,COLOR_SERVER,"[admin/vehicle] Taki pojazd nie istnieje.");
		}
	}
	else
	{
		GuiInfo(playerid,"Nie masz uprawnieñ do wykonania tej komendy.");
	}
	return 1;
}

COMMAND:apojazd(playerid,params[])
{
	if(CheckPlayerAccess(playerid) >  PERMS_SUPPORT)
	{
		if(pInfo[playerid][pGMCars] != 1)
		{
			GuiInfo(playerid,"Brak uprawnieñ.");
			return 1;
		}
		
		new param[32], vehicleid,rejestracja[32],modelid,col1,col2;
		if(sscanf(params,"s[32]i",param,vehicleid)) return SendClientMessage(playerid,COLOR_GREY,"U¿ycie komendy : /apojazd (napraw,rejestracja,stworz,przypisz,usun,unspawn,paintjob,paliwo,res)");

		if(!strcmp(param,"res"))
		{
			if(sscanf(params,"s[32]ii",param,vehicleid)) 
			{
				vlive_err(playerid,"/apojazd paintjob [vehid]");
				return 1;
			}
			
			RespawnVehicle(vehicleid);
			GuiInfo(playerid,"Pojazd respawnowany.");
		}
		
		if(!strcmp(param,"paliwo"))
		{
			new litry;
			if(sscanf(params,"s[32]iii",param,vehicleid,litry)) 
			{
				vlive_err(playerid,"/apojazd paintjob [vehid] [litry]");
				return 1;
			}
			
			vehicle[vehicleid][vfuel] = float(litry);
		}
		
		if(!strcmp(param,"paintjob"))
		{
			new paintjobid;
			if(sscanf(params,"s[32]iii",param,vehicleid,paintjobid)) 
			{
				vlive_err(playerid,"/apojazd paintjob [vehid] [paintjobid]");
				return 1;
			}
			
			ChangeVehiclePaintjob(vehicleid, paintjobid);
			
			new buffer[256];
			format(buffer,sizeof(buffer),"UPDATE vehicles_list SET veh_paintjob = %i WHERE veh_uid=%i",paintjobid,vehicle[vehicleid][vuid]);
			mysql_query(buffer);
		}
		
		if(!strcmp(param,"respawn"))
		{
			new vehicle_id;
			if(sscanf(params,"d",vehicle_id))
			{
				vlive_err(playerid,"/apojazd respawn [vehicleid]");
				return 1;
			}
			
			new uid = vehicle[vehicle_id][vuid];
			UnspawnVehicle(vehicle_id);
			SpawnVehicle(uid);
			
			//logowania admina
			new adminLog[128];
			format(adminLog,sizeof(adminLog),"~r~[pojazdy]~y~ Administrator %s ~g~(UID: %i)~y~ odrespawnowal pojazd %s (UID: %i)~y~",pInfo[playerid][pName],GetPlayerUID(playerid),vehicle[vehicle_id][vname],vehicle[vehicle_id][vuid]);
			LogAdminAction(adminLog);
			
			HudText(playerid,"~b~pojazd zrespawnowany");
		}
		
		if(!strcmp(param,"napraw"))
		{
			new vehUID = vehicle[vehicleid][vuid];
			new log[128];
			format(log,sizeof(log),"/apojazd napraw [%i]",vehUID);
			MySQL_log(playerid,log);
			RepairVehicleByID(vehicleid,playerid);
			
			//logowania admina
			new adminLog[128];
			format(adminLog,sizeof(adminLog),"~r~[pojazdy]~y~ Administrator %s ~g~(UID: %i)~y~ naprawil pojazd %s (UID: %i | SID: %i)~y~",pInfo[playerid][pName],GetPlayerUID(playerid),vehicle[vehicleid][vname],vehicle[vehicleid][vuid],vehicleid);
			LogAdminAction(adminLog);
		}
		
		if(!strcmp(param,"usun"))
		{
			if(CheckPlayerAccess(playerid) > PERMS_SUPPORT)
			{
				new vehUID = vehicle[vehicleid][vuid];
				new log[128];
				format(log,sizeof(log),"/apojazd usun [%i]",vehUID);
				MySQL_log(playerid,log);
				
				
				TruncateVehicle(vehicleid);
				SendClientMessage(playerid,COLOR_SERVER,"[admin/vehicle] Usun¹³eœ pojazd.");
				
				//logowania admina
				new adminLog[128];
				format(adminLog,sizeof(adminLog),"~r~[pojazdy]~y~ Administrator %s ~g~(UID: %i)~y~ usunal pojazd %s (UID: %i)~y~",pInfo[playerid][pName],GetPlayerUID(playerid),vehicle[vehicleid][vname],vehicle[vehicleid][vuid]);
				LogAdminAction(adminLog);
			}
		}
		
		if(!strcmp(param,"rejestracja"))
		{
			if(CheckPlayerAccess(playerid) > PERMS_SUPPORT)
			{
				if(sscanf(params,"s[32]is[32]",param,vehicleid,rejestracja)) return SendClientMessage(playerid,COLOR_GREY,"U¿ycie komendy : /apojazd rejestracja [id pojazdu] [nowa rejestracja]");
				new buffer[256];
				format(buffer,sizeof(buffer),"UPDATE vehicles_list SET veh_plates='%s' WHERE veh_uid = %i", rejestracja,vehicle[vehicleid][vuid]);
				mysql_query(buffer);
				SendClientMessage(playerid,COLOR_SERVER,"[admin/vehicle] Pomyœlnie zmieniono rejestracjê pojazdu.");
				SetVehicleNumberPlate(vehicleid, rejestracja);
				
				//logowania admina
				new adminLog[128];
				format(adminLog,sizeof(adminLog),"~r~[pojazdy]~y~ Administrator %s ~g~(UID: %i)~y~ zmienil rejestracje pojazdu %s(UID: %i)~y~",pInfo[playerid][pName],GetPlayerUID(playerid),vehicle[vehicleid][vname],vehicle[vehicleid][vuid]);
				LogAdminAction(adminLog);
			}
		}
		
		if(!strcmp(param,"stworz"))
		{
			if(CheckPlayerAccess(playerid) > PERMS_SUPPORT)
			{
				new Float:x,Float:y,Float:z,nazwa[16];
				GetPlayerPos(playerid,x,y,z);
				if(sscanf(params,"s[32]is[32]",param,modelid,nazwa)) return SendClientMessage(playerid,COLOR_GREY,"U¿ycie komendy : /apojazd stworz [model]");
				new buffer[256];
				format(buffer,sizeof(buffer),"INSERT INTO vehicles_list VALUES (NULL,%i,1000,0,0,0,'%s',%i,%f,%f,%f,0,1,1,3,'',10,0,0,0,0,0,0,0,0,1)",0,nazwa,modelid,x,y,z);
				mysql_query(buffer);
				SendClientMessage(playerid,COLOR_SERVER,"[admin/vehicle] Stworzy³eœ nowy pojazd.");
				
				
				new log[128];
				format(log,sizeof(log),"/apojazd stworz [%i]",modelid);
				MySQL_log(playerid,log);
				
				//logowania admina
				new adminLog[128];
				format(adminLog,sizeof(adminLog),"~r~[pojazdy]~y~ Administrator %s ~g~(UID: %i)~y~ stworzyl nowy pojazd ~g~%s",pInfo[playerid][pName],GetPlayerUID(playerid),nazwa);
				LogAdminAction(adminLog);
			}	
		}
		
		if(!strcmp(param,"kolor"))
		{
			if(CheckPlayerAccess(playerid) > PERMS_SUPPORT)
			{
				if(sscanf(params,"s[32]iii",param,vehicleid,col1,col2)) return SendClientMessage(playerid,COLOR_GREY,"U¿ycie komendy : /apojazd kolor [vehicleid] [col1] [col2]");
				new buffer[256];
				format(buffer,sizeof(buffer),"UPDATE vehicles_list SET veh_col1 = %i, veh_col2 = %i WHERE veh_uid=%i",col1,col2,vehicle[vehicleid][vuid]);
				mysql_query(buffer);
				vehicle[vehicleid][vcol1] = col1;
				vehicle[vehicleid][vcol2] = col2;
				ChangeVehicleColor(vehicleid, col1, col2);
				SendClientMessage(playerid,COLOR_SERVER,"[admin/vehicle] Zmieni³eœ kolorystykê pojazdu.");		
				
				new vehUID = vehicle[vehicleid][vuid];
				new log[128];
				format(log,sizeof(log),"/apojazd kolor [%i]",vehUID);
				MySQL_log(playerid,log);
				
				//logowania admina
				new adminLog[128];
				format(adminLog,sizeof(adminLog),"~r~[pojazdy]~y~ Administrator %s ~g~(UID: %i)~y~ zmienil kolorystyke pojazdu %s(UID: %i | SID: %i)~y~ ~g~(%i/%i)",pInfo[playerid][pName],GetPlayerUID(playerid),vehicle[vehicleid][vname],vehicle[vehicleid][vuid],vehicleid,col1,col2);
				LogAdminAction(adminLog);
			}
		}
		
		if(!strcmp(param,"przypisz"))
		{
			if(CheckPlayerAccess(playerid) > PERMS_SUPPORT)
			{
				new ownertype,owneruid;
				//apojazd przypisz [vehicleid] [ownertype] [owneruid]
				if(sscanf(params,"s[32]iii",param,vehicleid,ownertype,owneruid)) return SendClientMessage(playerid,COLOR_GREY,"U¿ycie komendy : /apojazd przypisz [vehicleid] [ownertype] [owneruid]");
				new buffer[256];
				new uiddd = vehicle[vehicleid][vuid];
				format(buffer,sizeof(buffer),"UPDATE vehicles_list SET veh_ownertype=%i,veh_owner=%i WHERE veh_uid=%i",ownertype,owneruid,vehicle[vehicleid][vuid]);
				mysql_query(buffer);
				printf("[admin/veh] administrator [%i] przypisa³ pojazd [%i] na gracza/grupê [%i]",playerid,vehicleid,owneruid);
				SendClientMessage(playerid,COLOR_SERVER,"[admin/vehicle] Przepisa³eœ pojazd pomyœlnie.");
				DeleteVehicle(vehicleid);
				format(buffer,sizeof(buffer),"UPDATE vehicles_list SET veh_spawned=0, veh_sampid=0 WHERE veh_uid=%i",vehicle[vehicleid][vuid]);
				mysql_query(buffer);
				CleanCarArray(vehicleid);
				if(ownertype == 2)
				{
					ClearPlayerVehiclesTable(GetPlayerIDbyUID(owneruid));
				}
				
				SpawnVehicle(uiddd);
				
				//logowania admina
				new adminLog[128];
				format(adminLog,sizeof(adminLog),"~r~[pojazdy]~y~ Administrator %s ~g~(UID: %i)~y~ przypisal pojazd %s(UID: %i)~y~ ~r~(typ: %i) (UID: %i)",pInfo[playerid][pName],GetPlayerUID(playerid),vehicle[vehicleid][vname],vehicle[vehicleid][vuid],ownertype,owneruid);
				LogAdminAction(adminLog);
			}
		}
		
		if(!strcmp(param,"unspawn"))
		{
			UnspawnVehicle(vehicleid);
		}
	}
	else
	{
		GuiInfo(playerid,"Nie posiadasz uprawnieñ, aby uzyskaæ dostêp do ACP.");
	}
	return 1;
}

forward HidePunishText();
public HidePunishText()
{
	TextDrawHideForAll(Text:TextDrawPunishInfo);
}

COMMAND:setskill(playerid,params[])
{
	if(CheckPlayerAccess(playerid) > PERMS_SUPPORT)
	{
		new type[32], varchar[64];
		if(sscanf(params, "s[32]S()[64]", type, varchar))
		{
			vlive_err(playerid, "/setskill [stamina | sila | strzelanie] [playerid] [ilosc]");
			return 1;
		}
		
		if(!strcmp(type, "sila", true))
		{
			new target,Float:amount;
			if(sscanf(varchar,"df",target,amount))
			{
				vlive_err(playerid,"/setskill sila [playerid] [wartosc]");
				return 1;
			}
			
			if(IsPlayerConnect(target))
			{
				pInfo[target][pPower] = amount;
				new msg[128];
				format(msg,sizeof(msg),"Ustawi³eœ si³ê gracza %s (SampID: %i) na wartoœæ: %f",pInfo[target][pName],target,amount);
				GuiInfo(playerid,msg);
				
				new adminLog[128];
				format(adminLog,sizeof(adminLog),"~r~[skill]~y~ Administrator %s ~g~(UID: %i | SID: %i)~y~ ustawil skill sily gracza ~g~(UID: %i) ~u~na ~g~%i",pInfo[playerid][pName],GetPlayerUID(playerid),playerid,GetPlayerUID(target),floatround(amount));
				LogAdminAction(adminLog);
			}
			else
			{
				GuiInfo(playerid,"Ten gracz nie jest pod³¹czony do serwera.");
			}
		}
		
		if(!strcmp(type, "strzelanie", true))
		{
			new target,Float:amount;
			if(sscanf(varchar,"df",target,amount))
			{
				vlive_err(playerid,"/setskill strzelanie [playerid] [wartosc]");
				return 1;
			}
			
			if(IsPlayerConnect(target))
			{
				pInfo[target][pShootskill] = amount;
				new msg[128];
				format(msg,sizeof(msg),"Ustawi³eœ skill strzelania gracza %s (SampID: %i) na wartoœæ: %f",pInfo[target][pName],target,amount);
				GuiInfo(playerid,msg);
				
				new adminLog[128];
				format(adminLog,sizeof(adminLog),"~r~[skill]~y~ Administrator %s ~g~(UID: %i | SID: %i)~y~ ustawil skill strzelania gracza ~g~(UID: %i) ~u~na ~g~%i",pInfo[playerid][pName],GetPlayerUID(playerid),playerid,GetPlayerUID(target),floatround(amount));
				LogAdminAction(adminLog);
			}
			else
			{
				GuiInfo(playerid,"Ten gracz nie jest pod³¹czony do serwera.");
			}
		}
		
		if(!strcmp(type, "stamina", true))
		{
			new target,Float:amount;
			if(sscanf(varchar,"df",target,amount))
			{
				vlive_err(playerid,"/setskill stamina [playerid] [wartosc]");
				return 1;
			}
			
			if(IsPlayerConnect(target))
			{
				pInfo[target][maxCondition] = amount;
				new msg[128];
				format(msg,sizeof(msg),"Ustawi³eœ kondycjê gracza %s (SampID: %i) na wartoœæ: %f",pInfo[target][pName],target,amount);
				GuiInfo(playerid,msg);
				
				new adminLog[128];
				format(adminLog,sizeof(adminLog),"~r~[skill]~y~ Administrator %s ~g~(UID: %i | SID: %i)~y~ ustawil skill staminy gracza ~g~(UID: %i) ~u~na ~g~%i",pInfo[playerid][pName],GetPlayerUID(playerid),playerid,GetPlayerUID(target),floatround(amount));
				LogAdminAction(adminLog);
			}
			else
			{
				GuiInfo(playerid,"Ten gracz nie jest pod³¹czony do serwera.");
			}
		}
	}
	else
	{
		GuiInfo(playerid,"Brak uprawnieñ.");
	}
	return 1;
}

COMMAND:setskin(playerid,params[])
{
	if(CheckPlayerAccess(playerid) >= PERMS_SUPPORT)
	{
		new target,skinid;
		if(sscanf(params,"dd",target,skinid))
		{
			vlive_err(playerid,"/setskin [playerid] [id skina]");
			return 1;
		}
		
		SetPlayerSkin(target,skinid);
		pInfo[target][pSkin] = skinid;
	}
	else
	{
		GuiInfo(playerid,"Brak uprawnieñ.");
	}
	
	return 1;
}

COMMAND:kokpit(playerid,pars[])
{
	if(CheckPlayerAccess(playerid) == PERMS_PROGRAMMER)
	{
		ShowPlayerDialog(playerid,DIAL_KOKPIT,DIALOG_STYLE_LIST,"Kokpit","1. Restart grup\n2. Restart pojazdów\n3. Restart drzwi\n4. Restart serwera\n5. Wy³¹cz skrypt\n6. Czyszczenie paczek\n7. Reload obiektów\n8. Reload przystanków\n9. Reload 3D Text\n10. Debug Mode","Wybierz","Zamknij");
	}
	else
	{
		vlive_err(playerid,"tylko programista posiada dostêp do kokpitu");
	}
	return 1;
}

COMMAND:uprawnienia(playerid,params[])
{
	if(CheckPlayerAccess(playerid) == PERMS_PROGRAMMER)
	{
		new target, level;
		if(sscanf(params,"dd",target,level))
		{
			vlive_err(playerid,"/uprawnienia [playerid] [poziom]");
			return 1;
		}
		
		new gid = pInfo[target][pGID];
		new buffer[256];
		format(buffer,sizeof(buffer),"UPDATE mysa_members SET gmaccess=%i WHERE member_id=%i",level,gid);
		mysql_query(buffer);
		
		switch(level)
		{
			case 0:
			{
				GuiInfo(target,"Otrzymales uprawnienia gracza.");
			}
			case 1:
			{
				GuiInfo(target,"Otrzymales uprawnienia helpera.");
			}
			case 2:
			{
				GuiInfo(target,"Otrzymales uprawnienia supportera.");
			}	
			case 3:
			{
				GuiInfo(target,"Otrzymales uprawnienia gamemastera LVL 1.");
			}
			case 4:
			{
				GuiInfo(target,"Otrzymales uprawnienia moderatora gry.");
			}
		}
		
		GuiInfo(playerid,"Zmieni³eœ uprawnienia gracza.");
	}
	else
	{
		GuiInfo(playerid,"Brak uprawnieñ.");
	}
	
	return 1;
}

COMMAND:acp(playerid,pars[])
{
	if(CheckPlayerAccess(playerid) > PERMS_SUPPORT)
	{
		ShowPlayerDialog(playerid,DIAL_ACP,DIALOG_STYLE_LIST,"Admin Control Panel","1. Panel pojazdów\n2. Respawn pojazdow grupowych\n3. Respawn wszystkich pojazdów\n4. Globalna naprawa pojazdu","Wybierz","Zamknij");
	}
	else
	{
		GuiInfo(playerid,"Nie posiadasz uprawnieñ, aby uzyskaæ dostêp do ACP.");
	}
	return 1;
}

COMMAND:setmoney(playerid,pars[])
{
	if(CheckPlayerAccess(playerid) > PERMS_SUPPORT)
	{
		new target,amount;
		if(sscanf(pars,"dd",target,amount)) return vlive_err(playerid,"/setmoney [gracz] [ilosc]");
		
		TakePlayerMoney(target,pInfo[target][pMoney]);
		AddPlayerMoney(target,amount);
		
		new adminLog[128];
		format(adminLog,sizeof(adminLog),"~r~[hajs]~y~ Administrator %s ~g~(UID: %i | SID: %i)~y~ zmienil stan konta gracza ~g~(UID: %i)~y~ na ~g~%i $",pInfo[playerid][pName],GetPlayerUID(playerid),playerid,pInfo[target][pUID],amount);
		LogAdminAction(adminLog);
	}
	else
	{
		GuiInfo(playerid,"Brak uprawnieñ.");
	}
	return 1;
}

COMMAND:premium(playerid,pars[])
{
	new premDays,target;
	if(sscanf(pars,"dd",premDays,target))
	{
		vlive_err(playerid,"/premium [dni] [gracz]");
		return 1;
	}
	
	if(CheckPlayerAccess(playerid) == PERMS_PROGRAMMER)
	{
		new buffer[256];
		format(buffer,sizeof(buffer),"UPDATE mysa_members SET premium=%i WHERE member_id=%i",premDays,pInfo[target][pGID]);
		mysql_query(buffer);
		
		GuiInfo(playerid,"Premium nadane.");
	}
	return 1;
}

COMMAND:whois(playerid,pars[])
{
	new uid;
	new gosciu = INVALID_PLAYER_ID;
	if(CheckPlayerAccess(playerid) > PERMS_HELPER)
	{
		if(sscanf(pars,"d",uid))
		{
			vlive_err(playerid,"/whois [uid gracza]");
			return 1;
		}
		
		for(new i = 0 ; i < MAX_PLAYERS; i++)
		{
			if(pInfo[i][pUID] == uid)
			{
				gosciu = i;
				break;
			}
		}
	}
	
	new output[128];
	format(output,sizeof(output),"ID wskazanego gracza: %i",gosciu);
	
	GuiInfo(playerid,output);
	return 1;
}

COMMAND:goto(playerid,params[])
{
	if(CheckPlayerAccess(playerid) >= PERMS_SUPPORT)
	{
		new target = strval(params);
		new Float:x,Float:y,Float:z,world;
		GetPlayerPos(target,x,y,z);
		SetPlayerPos(playerid,x+2,y,z);
		world = GetPlayerVirtualWorld(target);
		SetPlayerVirtualWorld(playerid,world);
		new interior = GetPlayerInterior(target);
		SetPlayerInterior(playerid,interior);
	}
	else
	{
		GuiInfo(playerid,"Brak uprawnieñ.");
	}
	return 1;
}

COMMAND:fly(playerid,params[])
{
	if(CheckPlayerAccess(playerid) > PERMS_SUPPORT)
	{
		StartFly(playerid);
	}
	else
	{
		GuiInfo(playerid,"Error.");
	}
	return 1;
}

COMMAND:unfly(playerid,params[])
{
	if(CheckPlayerAccess(playerid) > PERMS_SUPPORT)
	{
		StopFly(playerid);
	}
	else
	{
		GuiInfo(playerid,"Error.");
	}
	return 1;
}

COMMAND:gethere(playerid,params[])
{
	new target;
	if(sscanf(params,"i",target)) return SendClientMessage(playerid,COLOR_GREY,"U¿ycie komendy : /gethere [sampid]");
	if(CheckPlayerAccess(playerid) >= PERMS_SUPPORT)
	{
		if(IsPlayerConnected(target))
		{
			new Float:x,Float:y,Float:z,world;
			GetPlayerPos(playerid,x,y,z);
			world = GetPlayerVirtualWorld(playerid);
			SetPlayerPos(target,x+2,y,z);
			SetPlayerVirtualWorld(target,world);
			new interior = GetPlayerInterior(playerid);
			SetPlayerInterior(target,interior);
		}
		else
		{
			SendClientMessage(playerid,COLOR_GREY,"Gethere : nie ma gracza o takim SampID.");
		}
	}
	else
	{
		GuiInfo(playerid,"Brak uprawnieñ.");
	}
	return 1;
}

COMMAND:printpos(playerid,pars[])
{
	#pragma unused pars
	if (CheckPlayerAccess(playerid) >= PERMS_GAMEMASTER)
	{
		new Float:x,Float:y,Float:z;
		GetPlayerPos(playerid,x,y,z);
		printf("%f %f %f",x,y,z);
	}
	return 1;
}

COMMAND:gs(playerid,params[])
{
	new akcja[32],target,amount,reason[32];
	if(CheckPlayerAccess(playerid) > PERMS_SUPPORT)
	{
		if(sscanf(params,"s[32]dds[32]",akcja,target,amount,reason))
		{
			vlive_err(playerid,"/gs [nadaj | zabierz] [playerid] [ilosc] [powod]");
			return 1;
		}
		
		if(IsPlayerConnected(target))
		{
			if(!strcmp(akcja, "nadaj", true))
			{
				//pInfo[target][pGamescore] = pInfo[target][pGamescore]+amount;
				new score = GetPlayerGameScore(playerid) + amount;
				SetPlayerScore(target,score);
				new title[32];
				format(title,sizeof(title),"Gamescore +%i",amount);
				SystemPenaltyMessage(playerid,target,title,reason);
				new buffer[256];
				format(buffer,sizeof(buffer),"UPDATE mysa_members SET gamescore=%i WHERE member_id=%i",score,GetPlayerUID(target));
				mysql_query(buffer);
				return 1;
			}
			if(!strcmp(akcja, "zabierz", true))
			{
				if(amount > GetPlayerGameScore(playerid))
				{
					vlive_err(playerid,"ten gracz nie ma tyle GSów");
					return 1;
				}
				else
				{
					if (amount <= 0)
					{
						vlive_err(playerid,"podano nieprawid³ow¹ wartoœæ");
						return 1;
					}
					// tu zabieraj
					new score = GetPlayerGameScore(playerid) - amount;
					new buffer[256];
					format(buffer,sizeof(buffer),"UPDATE mysa_members SET gamescore=%i WHERE member_id=%i",score,GetPlayerUID(target));
					mysql_query(buffer);
					new title[32];
					format(title,sizeof(title),"Gamescore -%i",amount);
					SystemPenaltyMessage(playerid,target,title,reason);
					SetPlayerScore(target,GetPlayerGameScore(playerid));
					return 1;
				}
			}
		}
	}
	return 1;
}

COMMAND:aj(playerid,pars[])
{
	if (CheckPlayerAccess(playerid) >= PERMS_SUPPORT)
	{
		LoadAjText(playerid);
		new player,time,reason[32],buffer[256],string[256];
		if(sscanf(pars,"iis[32]",player,time,reason)) return SendClientMessage(playerid,COLOR_GREY,"U¿ycie komendy : /aj [player] [time] [powod]");
		//new freejail = CheckFreeJail();
		
		if(CheckPlayerAccess(playerid) < PERMS_GAMEMASTER)
		{
			if(time > 120)
			{
				GuiInfo(playerid,"Nadajesz zbyt du¿¹ iloœæ minut.");
				return 1;
			}
		}
		
		if(AjTimer[player] > 0)
		{
			SendClientMessage(playerid,COLOR_GREY,"AdminJail : ten gracz jest ju¿ w AJ");
		}
		else
		{
			SetPlayerVirtualWorld(player,player+9999);
			SetPlayerPos(player,AJ_X,AJ_Y,AJ_Z);
			SetPlayerSpecialAction(player,SPECIAL_ACTION_CUFFED);
			format(buffer,sizeof(buffer),"UPDATE core_players SET char_ajtime=%i WHERE char_uid=%i",time,GetPlayerUID(player));
			mysql_query(buffer);
			
			//textdraw
			new pname[MAX_PLAYER_NAME];
			GetPlayerName(playerid,pname,sizeof(pname));
			format(string, sizeof(string), "~r~~h~AdminJail~n~~w~Gracz: %s~n~Nadajacy: %s~n~~y~~h~%s", pInfo[player][pName],  GetPlayerGlobalNickname(playerid), reason);
			TextDrawSetString(Text:TextDrawPunishInfo, string);
			TextDrawShowForAll(Text:TextDrawPunishInfo);
			SetTimer("HidePunishText",6000,false);
			
			
			AjTimer[player] = SetTimerEx("PlayerAj", 60000, false, "i", player);
			
			format(buffer,sizeof(buffer),"Zosta³eœ umieszczony a AdminJail na %i minut.",time);
			SendClientMessage(player,COLOR_SERVER,buffer);
			//PlayerTextDrawShow(player,AjInfo[player]);
			//PlayerTextDrawSetString(player, AjInfo[player], buffer);
			pInfo[player][pAj] = time;
			
			new adminLog[128];
			format(adminLog,sizeof(adminLog),"~r~[aj]~y~ Administrator %s ~g~(UID: %i | SID: %i)~y~ wsadzil gracza %s (SID: %i) do AdminJail na ~r~%i minut",pInfo[playerid][pName],GetPlayerUID(playerid),playerid,pInfo[player][pName],player,time);
			LogAdminAction(adminLog);
			
			format(buffer,sizeof(buffer),"INSERT INTO penalties VALUES(NULL,'%s','%s',%i,%i,'%s','%s',0)",getStringDate(),getStringTime(),GetPlayerUID(player),GetPlayerUID(playerid),reason,"AdminJail");
			mysql_query(buffer);
		}
	}
	else
	{
		GuiInfo(playerid,"Nie posiadasz odpowiednich uprawnieñ.");
	}
	return 1;
}

COMMAND:kick(playerid,params[])
{
	if(CheckPlayerAccess(playerid) >= PERMS_SUPPORT)
	{
		new reason[32],target;
		if(sscanf(params,"is[32]",target,reason)) return SendClientMessage(playerid,COLOR_GREY,"U¿ycie komendy : /kick [playerid] [reason]");
		
		if(IsPlayerConnected(target))
		{
			new string[256];
			//textdraw
			new pname[MAX_PLAYER_NAME];
			GetPlayerName(playerid,pname,sizeof(pname));
			format(string, sizeof(string), "~r~~h~Kick~n~~w~Gracz: %s~n~Nadajacy: %s~n~~y~~h~%s", pInfo[target][pName],  GetPlayerGlobalNickname(playerid), reason);
			TextDrawSetString(Text:TextDrawPunishInfo, string);
			TextDrawShowForAll(Text:TextDrawPunishInfo);
			SetTimer("HidePunishText",6000,false);
			
			
			new output[150];
			format(output,sizeof(output),"Zosta³eœ wyrzucony z serwera, powód : %s",reason);
			SendClientMessage(target,COLOR_SERVER,output);
			SetTimerEx("KickWithMessage", 2000, false, "i", target);
			printf("[kick] Gracz %s (UID: %i) zostal wyrzucony z serwera - powod: admin kick",pInfo[playerid][pName],GetPlayerUID(playerid));
			
			new buffer[256];
			format(buffer,sizeof(buffer),"INSERT INTO penalties VALUES(NULL,'%s','%s',%i,%i,'%s','%s',0)",getStringDate(),getStringTime(),GetPlayerUID(target),GetPlayerUID(playerid),reason,"Kick");
			mysql_query(buffer);
			
			new adminLog[128];
			format(adminLog,sizeof(adminLog),"~r~[kick]~y~ Administrator %s ~g~(UID: %i | SID: %i)~y~ wyrzucil gracza %s (SID: %i) z serwera",pInfo[playerid][pName],GetPlayerUID(playerid),playerid,pInfo[target][pName],target);
			LogAdminAction(adminLog);
		}
		else
		{
			SendClientMessage(playerid,COLOR_GREY,"kick : nie ma gracza o tym SampID");
		}
	}
	return 1;
}

COMMAND:setvw(playerid,pars[])
{
	if(CheckPlayerAccess(playerid) > PERMS_HELPER)
	{
		new world = strval(pars);
		SetPlayerVirtualWorld(playerid,world);
		new output[64];
		format(output,sizeof(output),"[server] przenios³eœ siê na œwiat : %i",world);
		SendClientMessage(playerid,COLOR_SERVER,output);
	}
	else
	{
		GuiInfo(playerid,"Nie posiadasz odpowiednich uprawnieñ.");
	}
	return 1;
}

COMMAND:count(playerid,pars[])
{
	#pragma unused pars
	if(CheckPlayerAccess(playerid) > 0)
	{
		new output[64];
		format(output,sizeof(output),"[objects] liczba obiektów na tym vw : %i",CountObjects(playerid));
		SendClientMessage(playerid,COLOR_SERVER,output);
	}
	return 1;
}

COMMAND:i(playerid,pars[])
{
	if(CheckPlayerAccess(playerid) > PERMS_SUPPORT)
	{
		new wiadomosc[256];
		format(wiadomosc,sizeof(wiadomosc),"(( %s: %s ))",GetPlayerGlobalNickname(playerid),pars);
		SendClientMessageToAll(COLOR_WHITE,wiadomosc);
	}
	else
	{
		GuiInfo(playerid,"Brak odpowiednich uprawnieñ.");
	}
	return 1;
}

COMMAND:ado(playerid,pars[])
{
	new msg[256];
	if(CheckPlayerAccess(playerid) > PERMS_SUPPORT)
	{
		if(sscanf(pars,"s[256]",msg))
		{
			vlive_err(playerid,"/ado [narracja]");
			return 1;
		}
		
		format(msg,sizeof(msg),"** %s **",msg);
		
		for(new i = 0 ; i < MAX_PLAYERS; i++)
		{
			SendCustomPlayerMessage(i,COLOR_DO,msg);
		}
	}
	else
	{
		GuiInfo(playerid,"Nie masz uprawnieñ.");
	}
	return 1;
}

COMMAND:warn(playerid,params[])
{
	if(CheckPlayerAccess(playerid) >= PERMS_SUPPORT)
	{
		new target,reason[32];
		if(sscanf(params,"ds[32]",target,reason))
		{
			vlive_err(playerid,"/warn [playerid] [powod]");
			return 1;
		}
		new buffer[256];
		pInfo[target][pWarns]++;
		if(pInfo[target][pWarns] == 5)
		{
			//ban bo za duzo warnow
			format(buffer,256,"UPDATE core_players SET char_warns=char_warns+1, block_ban = 1, ban_reason='Warn 5/5' WHERE char_uid=%i",GetPlayerUID(target));
			mysql_query(buffer);
			SystemPenaltyMessage(playerid,target,"Ban (5/5)",reason);
			Kick(playerid);
			return 1;
		}
		
		//dodaj warna
		format(buffer,256,"UPDATE core_players SET char_warns =  char_warns+1 WHERE char_uid=%i",pInfo[target][pUID]);
		mysql_query(buffer);
		SystemPenaltyMessage(playerid,target,"Warn",reason);
	}
	return 1;
}

COMMAND:atime(playerid,p[])
{
	if(CheckPlayerAccess(playerid) > PERMS_SUPPORT)
	{
		new time;
		if(sscanf(p,"i",time)) return SendClientMessage(playerid,COLOR_GREY,"/atime [time]");
		SetWorldTime(time);
		
		new adminLog[128];
		format(adminLog,sizeof(adminLog),"~r~[timer]~y~ Administrator %s ~g~(UID: %i | SID: %i)~y~ zmienil czas serwera",pInfo[playerid][pName],GetPlayerUID(playerid),playerid);
		LogAdminAction(adminLog);
	}
	return 1;
}

COMMAND:unaj(playerid,params[])
{
	if (CheckPlayerAccess(playerid) >= PERMS_SUPPORT)
	{
		new target;
		if(sscanf(params,"i",target)) return SendClientMessage(playerid,COLOR_GREY,"/unaj [playerid]");
		PlayerUnaj(target);
		SendClientMessage(playerid,COLOR_GREY,"AdminJail : zwolni³eœ gracza z AJ.");
		
		new adminLog[128];
		format(adminLog,sizeof(adminLog),"~r~[aj]~y~ Administrator %s ~g~(UID: %i | SID: %i)~y~ zwolnil gracza %s (SID: %i) z AdminJail",pInfo[playerid][pName],GetPlayerUID(playerid),playerid,pInfo[target][pName],target);
		LogAdminAction(adminLog);
	}
	else
	{
		GuiInfo(playerid,"Brak uprawnieñ do wykonania komendy.");
	}
	return 1;
}

COMMAND:block(playerid,params[])
{
	if (CheckPlayerAccess(playerid) > PERMS_SUPPORT)
	{
		new string[32],target;
		if(sscanf(params,"s[32]i",string,target)) return SendClientMessage(playerid,COLOR_GREY,"/block [char,ooc,vehicles] [playerid]");
		
		if(!strcmp(string,"char"))
		{
			new buffer[256];
			format(buffer,sizeof(buffer),"UPDATE core_players SET block_char=%i WHERE char_uid=%i",BLOCK_CHAR,GetPlayerUID(target));
			mysql_query(buffer);
			Kick(target);
			printf("[kick] Gracz %s (UID: %i) zostal wyrzucony z serwera - powod: blokada postaci",pInfo[playerid][pName],GetPlayerUID(playerid));
			GuiInfo(playerid,"Zablokowa³eœ postaæ gracza.");
			
			new adminLog[128];
			format(adminLog,sizeof(adminLog),"~r~[aj]~y~ Administrator %s ~g~(UID: %i | SID: %i)~y~ zablokowal postac gracza %s (SID: %i)",pInfo[playerid][pName],GetPlayerUID(playerid),playerid,pInfo[target][pName],target);
			LogAdminAction(adminLog);
			
			system_msg(target,"Blokada postaci",playerid,"Wola administratora");
		}
		if(!strcmp(string,"ooc"))
		{
			new buffer[256];
			format(buffer,sizeof(buffer),"SELECT block_ooc FROM core_players WHERE char_uid=%i",GetPlayerUID(target));
			mysql_query(buffer);
			mysql_store_result();
			new ooc = mysql_fetch_int();
			mysql_free_result();
			if(ooc == 1)
			{
				format(buffer,sizeof(buffer),"UPDATE core_players SET block_ooc=0 WHERE char_uid=%i",GetPlayerUID(target));
				mysql_query(buffer);
				pInfo[target][blockOOC] = 0;
				GuiInfo(target,"Administrator zdj¹³ Ci blokadê czatu OOC.");
				GuiInfo(playerid,"Zdj¹³eœ blokadê czatu OOC dla gracza.");
				
				new adminLog[128];
				format(adminLog,sizeof(adminLog),"~r~[ooc]~y~ Administrator %s ~g~(UID: %i | SID: %i)~y~ zdjal blokade ooc graczowi %s (SID: %i)",pInfo[playerid][pName],GetPlayerUID(playerid),playerid,pInfo[target][pName],target);
				LogAdminAction(adminLog);
			}
			else
			{
				format(buffer,sizeof(buffer),"UPDATE core_players SET block_ooc=1 WHERE char_uid=%i",GetPlayerUID(target));
				mysql_query(buffer);
				pInfo[target][blockOOC] = 1;
				GuiInfo(target,"Administrator na³o¿y³ Ci blokadê czatu OOC.");
				GuiInfo(playerid,"Na³o¿y³eœ blokadê czatu OOC dla gracza.");
			}
		}
		if(!strcmp(string,"vehicles"))
		{
			new buffer[256];
			format(buffer,sizeof(buffer),"SELECT block_vehs FROM core_players WHERE char_uid=%i",GetPlayerUID(target));
			mysql_query(buffer);
			mysql_store_result();
			new block = mysql_fetch_int();
			mysql_free_result();
			if (block == 1)
			{
				format(buffer,sizeof(buffer),"UPDATE core_players SET block_vehs = 0 WHERE char_uid=%i",GetPlayerUID(target));
				mysql_query(buffer);
				pInfo[target][blockVehicles] = 0;
				GuiInfo(target,"Administrator zdj¹³ Ci blokadê prowadzenia pojazdów.");
				GuiInfo(playerid,"Zdj¹³eœ blokadê prowadzenia pojazdów dla gracza.");
			}
			else
			{
				format(buffer,sizeof(buffer),"UPDATE core_players SET block_vehs = 1 WHERE char_uid=%i",GetPlayerUID(target));
				mysql_query(buffer);
				pInfo[target][blockVehicles] = 1;
				GuiInfo(target,"Administrator na³o¿y³ Ci blokadê prowadzenia pojazdów.");
				GuiInfo(playerid,"Na³o¿y³eœ blokadê prowadzenia pojazdów dla gracza.");
			}
		}
	}
	else
	{
		GuiInfo(playerid,"Brak uprawnieñ do wykonania komendy.");
	}
	return 1;
}

COMMAND:ban(playerid,pars[])
{
	if(CheckPlayerAccess(playerid) >= PERMS_SUPPORT)
	{
		new target,reason[32],buffer[256],string[256];
		if(sscanf(pars,"is[32]",target,reason)) return SendClientMessage(playerid,COLOR_GREY,"/ban [playerid] [powód]");
		if(IsPlayerConnected(target))
		{
			format(buffer,sizeof(buffer),"UPDATE core_players SET block_ban=1 , ban_reason='%s' WHERE char_uid=%i",reason,GetPlayerUID(target));
			mysql_query(buffer);
			//format(buffer,sizeof(buffer),"Zosta³eœ zbanowany, powód : "COL_USE"%s",reason);
			//SendClientMessage(target,COLOR_SERVER,buffer);
			new pname[MAX_PLAYER_NAME];
			GetPlayerName(playerid,pname,sizeof(pname));
			format(string, sizeof(string), "~r~~h~Ban~n~~w~Gracz: %s~n~Nadajacy: %s~n~~y~~h~%s", pInfo[target][pName],  GetPlayerGlobalNickname(playerid), reason);
			TextDrawSetString(Text:TextDrawPunishInfo, string);
			TextDrawShowForAll(Text:TextDrawPunishInfo);
			
			//dodawanie ip
			new ip[16];
			GetPlayerIp(target, ip, sizeof(ip));
			format(buffer,sizeof(buffer),"INSERT INTO banned_ip VALUES(NULL,'%s')",ip);
			mysql_query(buffer);
			
			//penalties
			format(buffer,sizeof(buffer),"INSERT INTO penalties VALUES(NULL,'%s','%s',%i,%i,'%s','%s',0)",getStringDate(),getStringTime(),GetPlayerUID(target),GetPlayerUID(playerid),reason,"Ban");
			mysql_query(buffer);
			
			SetTimer("HidePunishText",6000,false);
			SetTimerEx("KickWithMessage",500,false,"i",target);
		}
		else
		{
			GuiInfo(playerid,"Wskazany gracz nie jest aktywny.");
		}
	}
	else
	{
		GuiInfo(playerid,"Brak uprawnieñ do wykonania komendy.");
	}
	return 1;
}

COMMAND:spec(playerid,pars[])
{
	if(CheckPlayerAccess(playerid) > PERMS_HELPER)
	{
		new target;
		if(sscanf(pars,"d",target))
		{
			vlive_err(playerid,"/spec [playerid] - podgl¹danie gracza");
			return 1;
		}
		if(target == playerid)
		{
			vlive_err(playerid,"nie mo¿esz podgl¹daæ sam siebie");
			return 1;
		}
		if(pInfo[playerid][pSpectate] != 0)
		{
			//specuje
			pInfo[playerid][pSpectate] = 0;
			PlayerCache[playerid][pSpecClose] = 1;
			TogglePlayerSpectating(playerid, 0);		
		}
		else
		{
			//nie specuje
			if(pInfo[target][pSpecPossible] > 0)
			{
				GuiInfo(playerid,"Nie mo¿esz podgl¹daæ tej osoby.");
				return 1;
			}
			
			GetPlayerPos(playerid,PlayerCache[playerid][pSpecX],PlayerCache[playerid][pSpecY],PlayerCache[playerid][pSpecZ]);
			PlayerCache[playerid][pSpecVW] = GetPlayerVirtualWorld(playerid);
			pInfo[playerid][pSpectate] = 1;
			new targetvw = GetPlayerVirtualWorld(target);
			SetPlayerVirtualWorld(playerid,targetvw);
			new targetint = GetPlayerInterior(target);
			SetPlayerInterior(playerid,targetint);
			TogglePlayerSpectating(playerid, 1);
			
			if(IsPlayerInAnyVehicle(target))
			{
				new vehicleid = GetPlayerVehicleID(target);
				PlayerSpectateVehicle(playerid, vehicleid);
			}
			else
			{
				PlayerSpectatePlayer(playerid, target);
			}
				
			new adminLog[128];
			format(adminLog,sizeof(adminLog),"~r~[kick]~y~ Administrator %s ~g~(UID: %i | SID: %i)~y~ podglada gracza %s (SID: %i)",pInfo[playerid][pName],GetPlayerUID(playerid),playerid,pInfo[target][pName],target);
			LogAdminAction(adminLog);
		}
	}
	else
	{
		vlive_err(playerid,"brak uprawnieñ");
	}
	return 1;
}

COMMAND:acuff(playerid,pars[])
{
	if(CheckPlayerAccess(playerid) >  PERMS_SUPPORT)
	{
		new target;
		if(sscanf(pars,"i",target)) return SendClientMessage(playerid,COLOR_GREY,"/acuff [playerid] - zakuwa wskazanego gracza");
		if(IsPlayerConnected(target))
		{
			if(pInfo[target][pCuffed] > 0)
			{
				pInfo[target][pCuffed] = 0;
				SetPlayerSpecialAction(target,SPECIAL_ACTION_NONE);
				RemovePlayerAttachedObject(target,0);
			}
			else
			{
				pInfo[target][pCuffed] = 1;
				SetPlayerSpecialAction(target,SPECIAL_ACTION_CUFFED);
				SetPlayerAttachedObject(target, 0, 19418, 6, -0.011000, 0.028000, -0.022000, -15.600012, -33.699977, -81.700035, 0.891999, 1.000000, 1.168000);
			}
		}
	}
	return 1;
}

COMMAND:freeze(playerid,pars[])
{
	if(CheckPlayerAccess(playerid) > PERMS_SUPPORT)
	{
		new target;
		if(sscanf(pars,"i",target)) return SendClientMessage(playerid,COLOR_GREY,"/freeze [playerid] - zamra¿a wskazanego gracza");
		if(IsPlayerConnected(target))
		{
			if(pInfo[target][pFreeze] > 0)
			{
				pInfo[target][pFreeze] = 0;
				TogglePlayerControllable(target,1);
			}
			else
			{
				pInfo[target][pFreeze] = 1;
				TogglePlayerControllable(target,0);
			}
		}
	}
	return 1;
}

COMMAND:dav(playerid,perms[])
{
	if(CheckPlayerAccess(playerid) >= PERMS_PROGRAMMER)
	{
		for(new i = 0 ; i < MAX_VEHICLES; i++)
		{
			if(GetVehicleModel(i) > 0)
			{
				UnspawnVehicle(i);
			}
		}
	}
	return 1;
}

COMMAND:gotov(playerid,params[])
{
	if(CheckPlayerAccess(playerid) > PERMS_HELPER)
	{
		new veh;
		if(sscanf(params,"d",veh)) return vlive_err(playerid,"/gotov [vehicleid]");
		new Float:x,Float:y,Float:z;
		if(GetVehicleModel(veh) > 0)
		{
			GetVehiclePos(veh,x,y,z);
			SetPlayerPos(playerid,x+3,y,z);
			new world = GetVehicleVirtualWorld(veh);
			SetPlayerVirtualWorld(playerid,world);
			
		}
		else
		{
			vlive_err(playerid,"nie ma pojazdu o wskazanym SampID");
		}
	}
	return 1;
}

COMMAND:getherev(playerid,params[])
{
	if(CheckPlayerAccess(playerid) > PERMS_SUPPORT)
	{
		new veh;
		if (sscanf(params,"d",veh)) return vlive_err(playerid,"/getherev [vehicleid]");
		new Float:x,Float:y,Float:z;
		GetPlayerPos(playerid,x,y,z);
		if(GetVehicleModel(veh) > 0)
		{
			SetVehiclePos(veh,x+3,y,z);
			new world = GetPlayerVirtualWorld(playerid);
			SetVehicleVirtualWorld(veh,world);
		}
		else
		{
			vlive_err(playerid,"nie ma pojazdu o takim SampID");
		}
	}
	else { }
	return 1;
}

stock test2()
{
	for(new v = 1; v < MAX_VEHICLES ; v++)
	{
		UnspawnVehicle(v);
		new buffer[256];
		format(buffer,sizeof(buffer),"UPDATE vehicles_list SET veh_spawned = 0 WHERE veh_uid=%i",vehicle[v][vuid]);
		mysql_query(buffer);
	}
}

stock test1(playerid)
{
	if(CheckPlayerAccess(playerid) >= PERMS_PROGRAMMER)
	{
		GuiInfo(playerid,"Usun¹³eœ wszystkie pojazdy z mapy.");
		for(new v = 1; v < MAX_VEHICLES ; v++)
		{
			UnspawnVehicle(v);
		}
	}
	else
	{
		vlive_err(playerid,"brak uprawnieñ.");
	}
}

COMMAND:sgv(playerid,params[])
{
	if(CheckPlayerAccess(playerid) >= PERMS_PROGRAMMER)
	{
		//LoadGroupsVehicles();
		GuiInfo(playerid,"Przywróci³eœ wszystkie grupowe pojazdy.");
	}
	return 1;
}

COMMAND:bw(playerid,params[])
{
	if(CheckPlayerAccess(playerid) > PERMS_HELPER)
	{
		new akcja[32],target;
		if(sscanf(params,"s[32]i",akcja,target)) return SendClientMessage(playerid,COLOR_GREY,"U¿ycie : /bw [zdejmij,naloz] [target]");
		if(!strcmp(akcja,"zdejmij"))
		{
			if(PlayerBwTimer[target] <= 0)
			{
				vlive_err(playerid,"ten gracz nie ma BW");
			}
			else
			{
				if(IsPlayerConnected(target))
				{
					PlayerBwTimer[target] = 1;
					GuiInfo(playerid,"Zdj¹³eœ BW graczowi.");
				}
			}
		}
	}
	else
	{
		GuiInfo(playerid,"Brak uprawnieñ.");
	}
	return 1;
}

COMMAND:hc(playerid,params[])
{
	if(CheckPlayerAccess(playerid) < PERMS_ADMIN)
	{
		GuiInfo(playerid,"Brak uprawnieñ.");
		return 1;
	}
	
	new msg[256];
	if(sscanf(params,"s[256]",msg))
	{
		vlive_err(playerid,"/ac [wiadomosc]");
		return 1;
	}
	
	format(msg,sizeof(msg),"[Head Admin Chat] %s [%i]: %s",GetPlayerGlobalNickname(playerid),playerid,msg);
	
	for(new i = 0 ; i < MAX_PLAYERS; i++)
	{
		if(CheckPlayerAccess(i) > PERMS_GAMEMASTER)
		{
			SendCustomPlayerMessage(i, COLOR_CRIMSON, msg);
		}
	}
	
	return 1;
}

COMMAND:ac(playerid,params[])
{
	if(CheckPlayerAccess(playerid) < PERMS_SUPPORT)
	{
		GuiInfo(playerid,"Brak uprawnieñ.");
		return 1;
	}
	
	new msg[256];
	if(sscanf(params,"s[256]",msg))
	{
		vlive_err(playerid,"/ac [wiadomosc]");
		return 1;
	}
	
	format(msg,sizeof(msg),"[Admin Chat] %s [%i]: %s",GetPlayerGlobalNickname(playerid),playerid,msg);
	
	for(new i = 0 ; i < MAX_PLAYERS; i++)
	{
		if(CheckPlayerAccess(i) > PERMS_HELPER)
		{
			SendCustomPlayerMessage(i, COLOR_AC, msg);
		}
	}
	
	return 1;
}

COMMAND:sethp(playerid,pars[])
{
	if(CheckPlayerAccess(playerid) > PERMS_HELPER)
	{
		new leczony,Float:hp;
		if(sscanf(pars,"if",leczony,hp))
		{
			SendClientMessage(playerid,COLOR_GREY,"/sethp [playerid] [Float:hp]");
			return 1;
		}
		new buffer[256];
		format(buffer,sizeof(buffer),"UPDATE core_players SET char_health=%f WHERE char_uid=%i",hp,GetPlayerUID(leczony));
		mysql_query(buffer);
		SetPlayerHealth(leczony,hp);
		pInfo[leczony][pHealth] = hp;
		GuiInfo(playerid,"Uleczy³eœ gracza.");
		return 1;
	}
	else
	{
		GuiInfo(playerid,"Brak odpowiednich uprawnieñ.");
	}
	return 1;
}

COMMAND:addbankomat(playerid,params[])
{
	if(CheckPlayerAccess(playerid) > PERMS_GAMEMASTER)
	{
		new buffer[256], Float:x,Float:y,Float:z,vw;
		vw = GetPlayerVirtualWorld(playerid);
		GetPlayerPos(playerid,x,y,z);
		new id = NewBankomatID();
		format(buffer,sizeof(buffer),"INSERT INTO bankomaty VALUES(NULL,%f,%f,%f,%i)",x,y,z,vw);
		mysql_query(buffer);
		
		format(buffer,sizeof(buffer),"SELECT MAX(uid) FROM bankomaty");
		mysql_query(buffer);
		mysql_store_result();
		new uid = mysql_fetch_int();
		mysql_free_result();
		
		format(buffer,sizeof(buffer),"SELECT * FROM bankomaty WHERE uid=%i",uid);
		mysql_query(buffer);
		mysql_store_result();
		while(mysql_fetch_row(buffer,"|"))
		{
			sscanf(buffer,"p<|>ifffi",
			bankomat[id][bkUID],
			bankomat[id][bkX],
			bankomat[id][bkY],
			bankomat[id][bkZ],
			bankomat[id][bkVW]);
		}
		mysql_free_result();
		
		GuiInfo(playerid,"Doda³eœ nowy bankomat.");
	}
	return 1;
}

COMMAND:delbankomat(playerid,params[])
{
	if(CheckPlayerAccess(playerid) > PERMS_GAMEMASTER)
	{
		new vw = GetPlayerVirtualWorld(playerid);
		for(new i = 0; i < MAX_BANKOMATS ; i++)
		{
			if(bankomat[i][bkVW] == vw)
			{
				if(IsPlayerInRangeOfPoint(playerid,3,bankomat[i][bkX],bankomat[i][bkY],bankomat[i][bkZ]))
				{
					new buffer[256];
					format(buffer,sizeof(buffer),"DELETE FROM bankomaty WHERE uid=%i",bankomat[i][bkUID]);
					mysql_query(buffer);
					GuiInfo(playerid,"Usun¹³eœ bankomat.");
					break;
				}
			}
		}
	}
	return 1;
}

COMMAND:addpetrol(playerid,params[])
{
	if(CheckPlayerAccess(playerid) > PERMS_GAMEMASTER)
	{
		new Float:x,Float:y,Float:z,buffer[256];
		new vw = GetPlayerVirtualWorld(playerid);
		GetPlayerPos(playerid,x,y,z);
		new id = CheckFreePetrolSlot();
		format(buffer,sizeof(buffer),"INSERT INTO petrolstation VALUES(NULL,%f,%f,%f,%i)",x,y,z,vw);
		mysql_query(buffer);
		
		format(buffer,sizeof(buffer),"SELECT MAX(uid) FROM petrolstation");
		mysql_query(buffer);
		mysql_store_result();
		new uid = mysql_fetch_int();
		mysql_free_result();
		
		format(buffer,sizeof(buffer),"SELECT * FROM petrolstation WHERE uid=%i",uid);
		mysql_query(buffer);
		mysql_store_result();
		while(mysql_fetch_row(buffer,"|"))
		{
			sscanf(buffer,"p<|>ifffi",
			petrol[id][pUID],
			petrol[id][peX],
			petrol[id][peY],
			petrol[id][peZ],
			petrol[id][peVW]);
		}
		mysql_free_result();
		GuiInfo(playerid,"Stworzy³eœ stacjê benzynow¹.");
	}
	else
	{
		vlive_err(playerid,"brak Ci uprawnieñ, tylko programista mo¿e u¿ywaæ tej komendy");
	}
	return 1;
}

COMMAND:delpetrol(playerid,params[])
{
	if(CheckPlayerAccess(playerid) > PERMS_GAMEMASTER)
	{
		new Float:x,Float:y,Float:z;
		GetPlayerPos(playerid,x,y,z);
		new vw = GetPlayerVirtualWorld(playerid);
		for(new i =0 ; i <MAX_PETROLS; i ++)
		{
			if(IsPlayerInRangeOfPoint(playerid,5,petrol[i][peX],petrol[i][peY],petrol[i][peZ]))
			{
				if(petrol[i][peVW] == vw)
				{
					new buffer[256];
					format(buffer,sizeof(buffer),"DELETE FROM petrolstation WHERE uid=%i",petrol[i][pUID]);
					mysql_query(buffer);
					petrol[i][pUID] = EOS;
					petrol[i][peX] = EOS;
					petrol[i][peY] = EOS;
					petrol[i][peZ] = EOS;
					GuiInfo(playerid,"Usun¹³eœ w³aœnie stacjê benzynow¹.");
					break;
				}
			}
		}
	}
	else
	{
		vlive_err(playerid,"brak Ci uprawnieñ, tylko programista mo¿e u¿ywaæ tej komendy");
	}
	return 1;
}

COMMAND:setpos(playerid,params[])
{
	if(CheckPlayerAccess(playerid) > PERMS_SUPPORT)
	{
		new Float:x,Float:y,Float:z;
		if(sscanf(params,"%f%f%f",x,y,z))
		{
			vlive_err(playerid,"/setpos [x] [y] [z]");
			return 1;
		}
		SetPlayerPos(playerid,x,y,z);
	}
	else
	{
		vlive_err(playerid,"brak uprawnieñ");
	}
	return 1;
}

COMMAND:aprzedmiot(playerid,params[])
{
	if(CheckPlayerAccess(playerid) >= PERMS_GAMEMASTER)
	{
		if(pInfo[playerid][pGMItems] != 1)
		{
			GuiInfo(playerid,"Brak uprawnieñ.");
			return 1;
		}
		
		new akcja[32],itemid;
		if(sscanf(params,"s[32]i",akcja,itemid))
		{
			vlive_err(playerid,"/aprzedmiot [usun] [itemuid]");
			new types[1024];
			format(types,sizeof(types),"[ == LISTA DOSTÊPNYCH TYPÓW PRZEDMIOTÓW ==]\n");
			strcat(types, "\n» broñ", sizeof(types));	//1
			strcat(types, "\n» amunicja", sizeof(types));	//2
			strcat(types, "\n» ubranie", sizeof(types));	//3
			strcat(types, "\n» jedzenie", sizeof(types));	//4
			strcat(types, "\n» czêœæ monta¿owa (pojazdy)", sizeof(types));	//5
			strcat(types, "\n» zegarek", sizeof(types));	//6	
			strcat(types, "\n» notatnik", sizeof(types));	//7
			strcat(types, "\n» telefon", sizeof(types));	//8
			strcat(types, "\n» kasa", sizeof(types));		//9
			strcat(types, "\n» kajdanki", sizeof(types));		//10
			strcat(types, "\n» wino", sizeof(types));		//11
			strcat(types, "\n» papierosy", sizeof(types));		//12
			strcat(types, "\n» maska", sizeof(types));		//13
			strcat(types, "\n» kamizelka", sizeof(types));		//14
			strcat(types, "\n» megafon", sizeof(types));		//15
			strcat(types, "\n» paralizator", sizeof(types));		//16
			strcat(types, "\n» marihuana", sizeof(types));		//17
			strcat(types, "\n» amfetamina", sizeof(types));		//18
			strcat(types, "\n» lakier", sizeof(types));		//19
			strcat(types, "\n» przyczepialny", sizeof(types));		//20
			strcat(types, "\n» nasiona marihuany", sizeof(types));		//21
			strcat(types, "\n» specjalna broñ bia³a", sizeof(types));		//22
			strcat(types, "\n» karnet", sizeof(types));		//23
			strcat(types, "\n» wytrych", sizeof(types));		//24
			strcat(types, "\n» SWAT Kamerka", sizeof(types));		//25
			
			
			ShowPlayerDialog(playerid,DIAL_APRZEDMIOT,DIALOG_STYLE_LIST,"Admin Control Panel » Create Item",types,"Wybierz","Zamknij");
			return 1;
		}
		if(itemid > 0)
		{
			new buffer[256];
			format(buffer,sizeof(buffer),"DELETE FROM core_items WHERE item_uid=%i",itemid);
			mysql_query(buffer);
			SendClientMessage(playerid,COLOR_SERVER,"[przedmiot] usun¹³eœ wskazany przedmiot");
			return 1;
		}
	}
	else
	{
		GuiInfo(playerid,"Brak uprawnieñ.");
	}
	return 1;
}

COMMAND:a(playerid,params[])
{
	new alista[512];
	for(new i=0;i<MAX_PLAYERS;i++)
	{
		if(IsPlayerConnect(i))
		{
			if(pInfo[i][pAdmin] > 0)
			{
				new pname[MAX_PLAYER_NAME];
				format(pname,sizeof(pname),"%s",GetPlayerGlobalNickname(i));
				if(CheckPlayerAccess(i) == PERMS_HELPER)
				{
					format(alista,512,"%s\n%i\t\t%s\t\t"COL_ORANGE"Helper",alista,i ,pname);
				}
				if(CheckPlayerAccess(i) == PERMS_SUPPORT)
				{
					format(alista,512,"%s\n%i\t\t%s\t\t"COL_SUPPORT"Supporter",alista,i ,pname);
				}
				if(CheckPlayerAccess(i) == PERMS_GAMEMASTER)
				{
					format(alista,512,"%s\n%i\t\t%s\t\t"COL_GM"Gamemaster",alista,i ,pname);
				}
				if(CheckPlayerAccess(i) == PERMS_ADMIN)
				{
					format(alista,512,"%s\n%i\t\t%s\t\t"COL_RED"Moderator Serwera",alista,i ,pname);
				}
				if(CheckPlayerAccess(i)== PERMS_PROGRAMMER)
				{
					format(alista,512,"%s\n%i\t\t%s\t\t"COL_RED"Programista",alista,i ,pname);
				}
			}
		}
	}
	
	if(!alista[0])
	{
		GuiInfo(playerid,"Nie ma ¿adnego administratora online.");
		return 1;
	}
	
	ShowPlayerDialog(playerid,404,DIALOG_STYLE_LIST,"Administratorzy online",alista,"OK","");
	return 1;
}

COMMAND:ptp(playerid,params[])
{
	//gracz1 do gracza 2
	if(CheckPlayerAccess(playerid) > PERMS_HELPER)
	{
		new playerOne,playerTwo;
		if(sscanf(params,"dd",playerOne,playerTwo))
		{
			vlive_err(playerid,"/ptp [gracz1] [gracz2] - teleportowanie gracza2 do gracza1");
			return 1;
		}
		
		if(IsPlayerConnect(playerOne))
		{
			if(IsPlayerConnect(playerTwo))
			{
				new playerWorld = GetPlayerVirtualWorld(playerOne);
				new Float:x,Float:y,Float:z;
				GetPlayerPos(playerOne,x,y,z);
				
				SetPlayerPos(playerTwo,x,y,z);
				SetPlayerVirtualWorld(playerTwo,playerWorld);
			}
			else
			{
				GuiInfo(playerid,"Jeden z graczy nie jest pod³¹czony do serwera.");
			}
		}
		else
		{
			GuiInfo(playerid,"Jeden z graczy nie jest pod³¹czony do serwera.");
		}
	}
	else
	{
		GuiInfo(playerid,"Brak odpowiednich uprawnieñ.");
	}
	
	return 1;
}

COMMAND:setpvw(playerid,params[])
{
	if(CheckPlayerAccess(playerid) > PERMS_HELPER)
	{
		new target,vw;
		if(sscanf(params,"dd",target,vw))
		{
			vlive_err(playerid,"/setpvw [playerid] [world]");
			return 1;
		}
		
		SetPlayerVirtualWorld(target,vw);
		vlive_err(playerid,"zmieniles world gracza");
	}
	else
	{
		GuiInfo(playerid,"Brak odpowiednich uprawnieñ.");
	}
	
	return 1;
}

COMMAND:spawnplayer(playerid,params[])
{
	if(CheckPlayerAccess(playerid) > PERMS_HELPER)
	{
		new target;
		if(sscanf(params,"d",target))
		{
			vlive_err(playerid,"/spawnplayer [playerid]");
			return 1;
		}
		
		CitySpawn(target);
		vlive_err(playerid,"odspawnowa³eœ gracza");
	}
	else
	{
		GuiInfo(playerid,"Brak odpowiednich uprawnieñ.");
	}
	
	return 1;
}

COMMAND:astats(playerid,params[])
{
	if(CheckPlayerAccess(playerid) > 0)
	{
		new target;
		if(sscanf(params,"d",target))
		{
			vlive_err(playerid,"/astats [id]");
			return 1;
		}
		
		new buffer[256],output[512],title[256];
		format(buffer,sizeof(buffer),"SELECT char_time FROM core_players WHERE char_uid=%i",GetPlayerUID(target));
		mysql_query(buffer);
		mysql_store_result();
		new time = mysql_fetch_int();
		mysql_free_result();
		time = time/60/60;
		format(buffer,sizeof(buffer),"SELECT member_id FROM %s WHERE name='%s'",MYBB_TABLE,GetPlayerGlobalNickname(target));
		mysql_query(buffer);
		mysql_store_result();
		new gid = mysql_fetch_int();
		mysql_free_result();
		format(title,sizeof(title),"%s [SampID : %i] [UID : %i] [GID : %i]",pInfo[target][pName],playerid,pInfo[target][pUID],gid);
			
		format(output, sizeof(output), "Konto globalne : "COL_GREEN2"\t\t%s",GetPlayerGlobalNickname(target));
		format(output, sizeof(output), "%s\nCzas gry postaci : "COL_GREEN2"\t\t%i godzin",output,time);
		format(output, sizeof(output), "%s\nPunkty ¿ycia : "COL_GREEN2"\t\t%f",output,pInfo[target][pHealth]);
		format(output, sizeof(output), "%s\nPieni¹dze w portfelu : "COL_GREEN2"\t\t%i$",output,pInfo[target][pMoney]);
		format(output, sizeof(output), "%s\nAktualny skin : "COL_GREEN2"\t\t%i",output,pInfo[target][pSkin]);
		format(output, sizeof(output), "%s\nLiczba warnów : "COL_GREEN2"\t\t%i/5",output,pInfo[target][pWarns]);
		format(output, sizeof(output), "%s\nNumer konta bankowego : "COL_GREEN2"\t\t%i",output,pInfo[target][pBankNR]);
		format(output, sizeof(output), "%s\nStan konta bankowego : "COL_GREEN2"\t\t%i$",output,pInfo[target][pBank]);
		format(output, sizeof(output), "%s\nPKT kondycji : "COL_GREEN2"\t\t%f",output,pInfo[target][maxCondition] );
		format(output, sizeof(output), "%s\nPKT sily : "COL_GREEN2"\t\t%f",output,pInfo[target][pPower] );
		format(output, sizeof(output), "%s\nPKT celnoœci : "COL_GREEN2"\t\t%f",output,pInfo[target][pShootskill] );
		format(output, sizeof(output), "%s\n-------",output);
			
		ShowPlayerDialog(playerid,DIAL_STATS,DIALOG_STYLE_LIST,title,output,"OK","");
	}
	return 1;
}

COMMAND:report(playerid,params[])
{
	new target,tresc[256];
	if(sscanf(params,"is[256]",target,tresc))
	{
		vlive_err(playerid,"/report [id gracza] [opis zdarzenia]");
		return 1;
	}
	if(!IsPlayerConnected(target))
	{
		vlive_err(playerid,"nie ma takiego gracza");
		return 1;
	}
	
	format(tresc,256,"Zg³oszenie od %s(%i) na (SID: %i): %s",pInfo[playerid][pName],playerid,target,tresc);
	
	SendCustomPlayerMessage(playerid,COLOR_GROUPS,"Zg³osi³eœ gracza, teraz poczekaj na odpowiedŸ administratora.");
	
	for(new i=0;i<MAX_PLAYERS;i++)
	{
		if(pInfo[i][pAdmin] > 0)
		{
			SendCustomPlayerMessage(i,COLOR_GROUPS,tresc);
		}
	}
	return 1;
}

COMMAND:admin(playerid,params[])
{
	return cmd_a(playerid,params);
}

COMMAND:create(playerid,params[])
{
	SetPlayerAttachedObject(playerid, SLOT_PLAYERONE, 373, 1, 0.013744 ,0.007423 ,-0.010019 ,-2.759115, 8.353719, 9.100319 ,1.000000 ,1.000000 ,1.000000);
	return 1;
}

CMD:edit(playerid,params[])
{
	EditAttachedObject(playerid,SLOT_PLAYERONE);
}

COMMAND:getvw(playerid,params[])
{
	if(CheckPlayerAccess(playerid) > PERMS_HELPER)
	{
		new vw = GetPlayerVirtualWorld(playerid);
		new ou[128];
		format(ou,sizeof(ou),"twój wirtualny œwiat to: %i",vw);
		vlive_err(playerid,ou);
	}
	return 1;
}

COMMAND:setinterior(playerid,params[])
{
	if(CheckPlayerAccess(playerid) > PERMS_SUPPORT)
	{
		new interior;
		if(sscanf(params,"d",interior))
		{
			vlive_err(playerid,"/setinterior [id]");
			return 1;
		}
		SetPlayerInterior(playerid,interior);
		vlive_err(playerid,"zmieni³eœ interior");
	}
	return 1;
}

COMMAND:reloadstatus(playerid,params[])
{
	if(CheckPlayerAccess(playerid) > PERMS_HELPER)
	{
		new target;
		if(sscanf(params,"d",target))
		{
			vlive_err(playerid,"/reloadstatus [target] - odœwie¿anie statusu gracza");
			return 1;
		}
		
		statusPlayer[target] = 1;
	}
	return 1;
}

COMMAND:reloadopis(playerid,params[])
{
	if(CheckPlayerAccess(playerid) > PERMS_HELPER)
	{
		new target;
		if(sscanf(params,"d",target))
		{
			vlive_err(playerid,"/reloadopis [target] - odœwie¿anie opisu gracza");
			return 1;
		}
		
		UpdateDynamic3DTextLabelText(Text3D:PlayerDesc[target], COLOR_ME, " ");
		statusPlayer[target] = 1;
	}
	return 1;
}

COMMAND:busblock(playerid,params[])
{
	if(CheckPlayerAccess(playerid) > PERMS_SUPPORT)
	{
		if(busEnabled == 1)
		{
			busEnabled = 0;
			HudText(playerid,"wylaczyles busy");
		}
		else
		{
			busEnabled = 1;
			HudText(playerid,"wlaczyles busy");
		}
	}
	return 1;
}

COMMAND:sul(playerid,params[])
{
	if(CheckPlayerAccess(playerid) > PERMS_GAMEMASTER)
	{
		if(IsPlayerInAnyVehicle(playerid))
		{
			new vehicle_id = GetPlayerVehicleID(playerid);
			
			vehicle[vehicle_id][vAdminCar] = 1;
			GuiInfo(playerid,"Pojazd, w którym teraz siedzisz zosta³ wy³¹czony z systemu obra¿eñ.");
		}
	}
	return 1;
}

COMMAND:aprzeszukaj(playerid,params[])
{
	if(CheckPlayerAccess(playerid) > PERMS_SUPPORT)
	{
		new target;
		if(sscanf(params,"d",target))
		{
			vlive_err(playerid,"/aprzeszukaj [id]");
			return 1;
		}
		ShowPlayerItemsToPlayer(target,playerid);
	}
	else
	{
		GuiInfo(playerid,"Brak uprawnieñ.");
	}
	return 1;
}

COMMAND:reloadperms(playerid,params[])
{
	GetAdminPermissions(playerid);
	
	GuiInfo(playerid,"Prze³adowa³eœ swoje uprawnienia.");
	
	return 1;
}

COMMAND:upr(playerid,params[])
{
	if(CheckPlayerAccess(playerid) != PERMS_PROGRAMMER)
	{
		return 1;
	}
	else
	{
		new target,pojazdy,grupy,przedmioty,drzwi;
		if(sscanf(params,"ddddd",target,pojazdy,grupy,przedmioty,drzwi))
		{
			vlive_err(playerid,"/upr [gracz] [pojazdy] [grupy] [przedmioty] [drzwi]");
			return 1;
		}
		
		new buffer[256];
		format(buffer,sizeof(buffer),"UPDATE uprawnienia SET przedmioty = %i, grupy=%i,drzwi=%i,pojazdy=%i WHERE gid=%i",przedmioty,grupy,drzwi,pojazdy,pInfo[target][pGID]);
		mysql_query(buffer);
		
		new msg[512];
		
		if(pojazdy == 1)
		{
			format(msg,sizeof(msg),"%s\nPojazdy: "COL_GREEN"zezwolone",msg);
		}
		else
		{
			format(msg,sizeof(msg),"%s\nPojazdy: "COL_RED"zabronione",msg);
		}
		
		if(grupy == 1)
		{
			format(msg,sizeof(msg),"%s\nGrupy: "COL_GREEN"zezwolone",msg);
		}
		else
		{
			format(msg,sizeof(msg),"%s\nGrupy:"COL_RED" zabronione",msg);
		}
		
		if(przedmioty == 1)
		{
			format(msg,sizeof(msg),"%s\nPrzedmioty: "COL_GREEN"zezwolone",msg);
		}
		else
		{
			format(msg,sizeof(msg),"%s\nPrzedmioty: "COL_RED"zabronione",msg);
		}
		
		if(drzwi == 1)
		{
			format(msg,sizeof(msg),"%s\nDrzwi: "COL_GREEN"zezwolone",msg);
		}
		else
		{
			format(msg,sizeof(msg),"%s\nDrzwi: "COL_RED"zabronione",msg);
		}
		
		GuiInfo(playerid,msg);
		
		GuiInfo(target,msg);
		
	}
	
	return 1;
}

COMMAND:setfs(playerid,params[])
{
	if(CheckPlayerAccess(playerid) > PERMS_SUPPORT)
	{
		ShowPlayerDialog(playerid,DIAL_STYLE,DIALOG_STYLE_LIST,"Wybierz styl","1. Normalny\n2. Boks\n3. Kungfu\n4. Kneehead\n5. Grabkick\n6. Elbow","Wybierz","Anuluj");
	}
	
	return 1;
}

COMMAND:specdeny(playerid,params[])
{
	if(CheckPlayerAccess(playerid) > PERMS_ADMIN)
	{
		if(pInfo[playerid][pSpecPossible] == 1)
		{
			pInfo[playerid][pSpecPossible] = 0;
			HudText(playerid,"specing you: ~g~possible");
		}
		else
		{
			pInfo[playerid][pSpecPossible] = 1;
			HudText(playerid,"specing you: ~r~impossible");
		}
	}
	else
	{
		GuiInfo(playerid,"Brak uprawnieñ.");
	}
	
	return 1;
}

COMMAND:tnick(playerid,params[])
{
	if(CheckPlayerAccess(playerid) > PERMS_GAMEMASTER)
	{
		new target;
		if(sscanf(params,"d",target))
		{
			vlive_err(playerid,"/tnick [playerid] - usuwasz nick z nad g³owy gracza");
			return 1;
		}
		
		if(PlayerCache[target][pHidden] == 0)
		{
			PlayerCache[target][pHidden] = 1;
			statusPlayer[target] = 1;
			GuiInfo(playerid,"Ukry³eœ nick gracza.");
		}
		else
		{
			PlayerCache[target][pHidden] = 0;
			statusPlayer[target] = 1;
			GuiInfo(playerid,"Przywróci³eœ nick gracza.");
		}
	}
	else
	{
		GuiInfo(playerid,"Brak uprawnieñ.");
	}
	
	return 1;
}

COMMAND:sg(playerid,params[])
{
	if(CheckPlayerAccess(playerid) > PERMS_SUPPORT)
	{
		new target;
		if(sscanf(params,"d",target))
		{
			vlive_err(playerid,"/sg [id]");
			return 1;
		}
		
		ShowPlayerGroupToPlayer(target,playerid);
	}
	else
	{
		GuiInfo(playerid,"Czy jesteœ administratorem? ERROR ERROR XD");
	}
	return 1;
}