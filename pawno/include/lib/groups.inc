//
//	1. trzeba zabezpieczyæ - nie dwa razy do jedenj grupy itp
//
//
#define PAYDAY_DELAY 60		// w sekundach

//
//	permisje
//
#define PERM_DENY 0
#define PERM_DOORS	1
#define PERM_LEADER	2
#define PERM_VEHICLES	3
#define PERM_OOC	4
#define PERM_OFFERS	5
#define PERM_ZAMAWIANIE	6

//
//	typy grup
//
#define TYPE_MAFIA 100
#define TYPE_ORG 101
#define TYPE_SCIGANT 102
#define TYPE_SZAJK 103
#define TYPE_LSFD 5
#define TYPE_LSPD 6
#define TYPE_LSMC 7
#define TYPE_GOV 8
#define TYPE_ADMINISTRACJA 666

#define TYPE_NONE 0
#define TYPE_WORKSHOP 1
#define TYPE_RESTAURANT 2
#define TYPE_247 3
#define TYPE_CARDEALER 4
#define TYPE_GYM 9
#define TYPE_FBI 10
#define TYPE_SANNEWS 11
//nowe
#define TYPE_APTEKA	12
#define TYPE_KOMIS	13
#define TYPE_OCHRONIARZE 14
#define TYPE_TAXI	15
#define TYPE_COFFESHOP	16
#define TYPE_KARTEL	17
#define TYPE_SYNDYKAT 18

#define MAX_GROUPS_VEHICLES 100


new SelectedAdminGroup[MAX_PLAYERS];

new AdSend = 0;


enum gtypesparams
{
	grTypeInt,
	grType[32],
}
new GroupTypes[17][gtypesparams] =
{
	{0,"Brak"},
	{1,"Warsztat"},
	{2,"Restauracja"},
	{3,"24/7"},
	{4,"Salon samochodowy"},
	{9,"Si³ownia"},
	{10,"FBI"},
	{11,"San News"},
	{100,"Mafia"},
	{101,"Organizacja przestêpcza"},
	{102,"Œciganci"},
	{103,"Szajka"},
	{5,"LSFD"},
	{6,"LSPD"},
	{7,"LSMC"},
	{8,"GOV"},
	{666,"Administracja"}
};


forward public InitGroups();
forward LoadGroupsOnStart();
forward CreateGroup();
forward DeleteGroup();
forward InviteToGroup();
forward UninviteFromGroup();
forward GetPlayerGroups(playerid);
forward ShowPlayerGroups(playerid);
forward GiveGroupMoney(groupid,amount);
forward SpawnGroupVehicles(gid);
forward LoadGroupsVehicles();
forward ShowAllGroupsForPlayer(playerid);
forward ClearPlayerGroupsTable(playerid);


public InitGroups()
{
	printf("[grupy] System zaladowany.");
	
	LoadGroupsOnStart();
	
	return 1;
}

forward LoadGroupsOnConnect(playerid);
public LoadGroupsOnConnect(playerid)
{
	new buffer[256];
	new num=1;
	format(buffer,sizeof(buffer),"SELECT group_uid,char_uid,char_rank,char_payday,char_skin,char_perms,char_dutytime,group_name FROM groups_workers WHERE char_uid=%i",GetPlayerUID(playerid));				//pamietaj ¿e tutaj 1 !!!, ponadto do naprawienia kurwa
	mysql_query(buffer);
	mysql_store_result();
	if(mysql_num_rows() > 0)
	{
		while(mysql_fetch_row(buffer,"|"))
		{
			sscanf(buffer,"p<|>iis[32]iis[32]is[32]",pGrupa[playerid][num][pGuid],pGrupa[playerid][num][pCharuid],pGrupa[playerid][num][pRank],pGrupa[playerid][num][pPayday],pGrupa[playerid][num][pSkin],pGrupa[playerid][num][pPerms],pGrupa[playerid][num][pDutytime],pGrupa[playerid][num][pGname]);
			sscanf(pGrupa[playerid][num][pPerms],"p<->iiiiii",pGrupa[playerid][num][permDoors],pGrupa[playerid][num][permLeader],pGrupa[playerid][num][permVehicles],pGrupa[playerid][num][permOoc],pGrupa[playerid][num][permOffers],pGrupa[playerid][num][permZamawianie]);
			num++;
		}
		mysql_free_result();
	}
}

public LoadGroupsOnStart()
{
	new buffer[256];
	format(buffer,sizeof(buffer),"SELECT * FROM groups_list");
	mysql_query(buffer);
	mysql_store_result();
	new num=1;
	while(mysql_fetch_row(buffer,"|"))
	{
		sscanf(buffer,"p<|>is[64]s[6]iii",
		grupa[num][Guid],
		grupa[num][Gname],
		grupa[num][Gtag],
		grupa[num][Gtype],
		grupa[num][Gmoney],
		grupa[num][Gcolor]);
		num++;
		//SpawnGroupVehicles(grupa[num][Guid]);
	}
	printf("[grupy] Ilosc wczytanych grup : %i",num-1);
	mysql_free_result();
	return 1;
}

public CreateGroup()
{
	return 1;
}

public DeleteGroup()
{
	return 1;
}

public InviteToGroup()
{
	return 1;
}

public UninviteFromGroup()
{
	return 1;
}

stock GetPlayerGroups(playerid)
{
	//print("laduje grupy");
	new buffer[256];
	new num=1;
	format(buffer,sizeof(buffer),"SELECT group_uid,char_uid,char_rank,char_payday,char_skin,char_perms,char_dutytime,group_name FROM groups_workers WHERE char_uid=%i",GetPlayerUID(playerid));				//pamietaj ¿e tutaj 1 !!!, ponadto do naprawienia kurwa
	mysql_query(buffer);
	mysql_store_result();
	if(mysql_num_rows() > 0)
	{
		while(mysql_fetch_row(buffer,"|"))
		{
			sscanf(buffer,"p<|>iis[32]iis[32]is[32]",pGrupa[playerid][num][pGuid],pGrupa[playerid][num][pCharuid],pGrupa[playerid][num][pRank],pGrupa[playerid][num][pPayday],pGrupa[playerid][num][pSkin],pGrupa[playerid][num][pPerms],pGrupa[playerid][num][pDutytime],pGrupa[playerid][num][pGname]);
			sscanf(pGrupa[playerid][num][pPerms],"p<->iiiiii",pGrupa[playerid][num][permDoors],pGrupa[playerid][num][permLeader],pGrupa[playerid][num][permVehicles],pGrupa[playerid][num][permOoc],pGrupa[playerid][num][permOffers],pGrupa[playerid][num][permZamawianie]);
			num++;
		}
		mysql_free_result();
	}
	else
	{
		//GuiInfo(playerid,"Nie jesteœ w ¿adnej grupie.");
	}
	return 1;
}

stock ClearPlayerGroupsTable(playerid)
{
	for(new gid=0;gid<10;gid++)
	{
		pGrupa[playerid][gid][pGuid] = EOS;
	}
	return 1;
}

stock test123(playerid)
{
	if(pGrupa[playerid][1][pGuid] <=0)
	{
		//return GuiInfo(playerid,"Nie jesteœ w ¿adnej grupie");
	}
	new glist[512];
	for (new i=1;i< MAX_PLAYER_GROUPS;i++)
	{
		if (pGrupa[playerid][i][pGuid] > 0)
		{
			format(glist,sizeof(glist),"%s\n%i # \t\t%s",glist,i,pGrupa[playerid][i][pGname]);
		}
	}
	
	//ShowPlayerDialog(playerid,DIAL_G,DIALOG_STYLE_LIST,"System Grup",glist,"Wybierz","Zamknij");
	return 1;
}

public ShowPlayerGroups(playerid)
{
	if(pGrupa[playerid][1][pGuid] <=0)
	{
		return GuiInfo(playerid,"Nie jesteœ w ¿adnej grupie");
	}
	new glist[512];
	for (new i=1;i< MAX_PLAYER_GROUPS;i++)
	{
		if (pGrupa[playerid][i][pGuid] > 0)
		{
			format(glist,sizeof(glist),"%s\n%i # \t\t%s",glist,i,pGrupa[playerid][i][pGname]);
		}
	}
	
	ShowPlayerDialog(playerid,DIAL_G,DIALOG_STYLE_LIST,"System Grup",glist,"Wybierz","Zamknij");
	return 1;
}

stock ShowPlayerGroupToPlayer(playerid,whowatch)
{
	if(pGrupa[playerid][1][pGuid] <=0)
	{
		return GuiInfo(whowatch,"Gracz nie jest w ¿adnej grupie.");
	}
	new glist[512];
	for (new i=1;i< MAX_PLAYER_GROUPS;i++)
	{
		if (pGrupa[playerid][i][pGuid] > 0)
		{
			format(glist,sizeof(glist),"%s\n%i # \t\t%s",glist,i,pGrupa[playerid][i][pGname]);
		}
	}
	
	ShowPlayerDialog(whowatch,DIAL_G,DIALOG_STYLE_LIST,"Lista grup gracza",glist,"Wybierz","Zamknij");
	return 1;
}

stock CleanGroups()
{
	for(new i=0;i < MAX_GROUPS; i++)
	{
		grupa[i][Guid] = EOS;
		grupa[i][Gname][0] = EOS;
		grupa[i][Gtype] = EOS;
		grupa[i][Gmoney] = EOS;
		grupa[i][Gcolor] = EOS;
		grupa[i][Gtag][0] = EOS;
	}
}

stock IsPlayerInGroupType(playerid,gtype)
{
	for (new i =1; i < MAX_PLAYER_GROUPS; i++)
	{
		new buffer[256];
		format(buffer,sizeof(buffer),"SELECT group_type FROM groups_list WHERE group_uid=%i",pGrupa[playerid][i][pGuid]);
		mysql_query(buffer);
		mysql_store_result();
		if (mysql_fetch_int() == gtype)
		{
			return 1;
		}
		mysql_free_result();
	}
	return 0;
}

stock IsPlayerCanOffer(playerid,gid)
{
	for(new i= 1; i < MAX_PLAYER_GROUPS ; i++)
	{
		if(pGrupa[playerid][i][pGuid] == gid)
		{
			if(pGrupa[playerid][i][permOffers] == 1)
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

stock GetGroupSampID(groupUid)
{
	for(new i = 0 ; i < MAX_GROUPS; i++)
	{
		if(grupa[i][Guid] == groupUid)
		{
			return i;
		}
	}
	return -1;
}

public ShowAllGroupsForPlayer(playerid)
{
	new glist[1024];
	if(CheckPlayerAccess(playerid) >=PERMS_SUPPORT)
	{
		for (new i=1; i < MAX_GROUPS ; i++)
		{
			if(grupa[i][Guid] == 0)
			{
				break;
			}
			else
			{
				format(glist,sizeof(glist),"%s\n%i\t\t%s",glist,i,grupa[i][Gname]);
			}
		}
		ShowPlayerDialog(playerid,404,DIALOG_STYLE_LIST,"Admin Control Panel » Groups » Show all groups",glist,"Wybierz","Zamknij");
		return 1;
	}
	else
	{
		return 0;
	}
}

stock GetGroupIDbyUID(uid)
{
	for(new i = 0 ; i < MAX_GROUPS; i++)
	{
		if(grupa[i][Guid] == uid)
		{
			return i;
		}
	}
	return 0;
}


//
//
//		KOMENDY GRUPY
//
//

COMMAND:coszjebanejest(playerid,params[])
{
	GetPlayerGroups(playerid);
	return 1;
}


COMMAND:g(playerid,params[])
{
	GetPlayerGroups(playerid);
	
	
	new type[32],varchar[32];
	if(sscanf(params,"s[32]S()[32]",type,varchar))
	{
		ShowPlayerGroups(playerid);
		SendClientMessage(playerid,COLOR_GREY,"/g [zapros,wypros,wplac,wyplac,res,namierz, przebierz]");
		return 1;
	}
	if(!strcmp(type, "przebierz", true))
	{
		new gid;
		if(sscanf(varchar,"d",gid))
		{
			vlive_err(playerid,"/g przebierz [groupid]");
			return 1;
		}
		
		if(pGrupa[playerid][gid][pGuid] <= 0 )
		{
			vlive_err(playerid,"nie jesteœ w takiej grupie");
			return 1;
		}
		
		if(pGrupa[playerid][gid][pSkin] <= 0 )
		{
			vlive_err(playerid,"nie posiadasz nadanego skina");
			return 1;
		}
		
		SetPlayerSkin(playerid,pGrupa[playerid][gid][pSkin] );
		
	}
	if(!strcmp(type, "wplac", true))
	{
		new gsid,amount;
		if(sscanf(varchar,"dd",gsid,amount))
		{
			vlive_err(playerid,"/g wplac [grupa] [ilosc]");
			return 1;
		}
		
		new doorid = GetPlayerDoorID(playerid);
		if(doorid != 0 && DoorInfo[doorid][doorOwnerType] == DOOR_TYPE_BANK)
		{

		}
		else
		{
			GuiInfo(playerid,"Musisz byæ w banku by u¿yæ tej komendy.");
			return 1;
		}
		
		if(amount > pInfo[playerid][pMoney])
		{
			GuiInfo(playerid,"Nie masz tyle kasy.");
			return 1;
		}
		
		if(pGrupa[playerid][gsid][pGuid] <= 0)
		{
			GuiInfo(playerid,"Nie nale¿ysz do takiej grupy.");
			return 1;
		}
		
		if(pGrupa[playerid][gsid][permLeader] <= 0)
		{
			GuiInfo(playerid,"Nie posiadasz takich uprawnieñ.");
			return 1;
		}
		
		mysa_LogMoney(playerid,amount,MONEY_GROUP_FOR_PLAYER,pGrupa[playerid][gsid][pGuid]);
		
		TakePlayerMoney(playerid,amount);
		AddGroupMoney(pGrupa[playerid][gsid][pGuid],amount);
		
		new groupSID = GetGroupSampID(pGrupa[playerid][gsid][pGuid]);
		
		grupa[groupSID][Gmoney] = grupa[groupSID][Gmoney] + amount;
	}
	if(!strcmp(type, "wyplac", true))
	{
		new gsid,amount,buffer[256];
		if(sscanf(varchar,"dd",gsid,amount))
		{
			vlive_err(playerid,"/g wyplac [grupa] [ilosc]");
			return 1;
		}
		
		//1. sprawdz czy w banku
		//2. sprawdz czy jest w takiej grupie
		//3. sprawdz czy grupa posiada tyle hajsu
		//4. sprawdz czy koleœ ma uprawnienia do wyp³acenia
		
		new doorid = GetPlayerDoorID(playerid);
		
		if(DoorInfo[doorid][doorOwnerType] != DOOR_TYPE_BANK)
		{
			GuiInfo(playerid,"Musisz byæ w banku by móc wyp³acaæ pieni¹dze z konta grupowego.");
			return 1;
		}
		
		if(pGrupa[playerid][gsid][pGuid] <= 0)
		{
			GuiInfo(playerid,"Nie jesteœ w takiej grupie.");
			return 1;
		}
		
		if(pGrupa[playerid][gsid][permLeader] != 1)
		{
			GuiInfo(playerid,"Brak uprawnieñ.");
			return 1;
		}
		
		//pobieramy hajsiwo
		format(buffer,sizeof(buffer),"SELECT group_money FROM groups_list WHERE group_uid=%i",pGrupa[playerid][gsid][pGuid]);
		mysql_query(buffer);
		mysql_store_result();
		new gMoney = mysql_fetch_int();
		mysql_free_result();
		
		if(gMoney < amount)
		{
			GuiInfo(playerid,"Grupa nie posiada tyle pieniêdzy.");
			return 1;
		}
		
		//pobierz z konta grupy, dodaj graczowi
		TakeGroupMoney(pGrupa[playerid][gsid][pGuid],amount);
		AddPlayerMoney(playerid,amount);
		
		new groupsid = GetGroupIDbyUID(pGrupa[playerid][gsid][pGuid]);
		grupa[groupsid][Gmoney] = grupa[groupsid][Gmoney] -amount;
		
		//blad?
		mysa_LogMoney(pGrupa[playerid][gsid][pGuid],amount,MONEY_PLAYER_FOR_GROUP,GetPlayerUID(playerid));
		
	}
	if(!strcmp(type, "lista", true))
	{
		ShowPlayerGroups(playerid);
	}
	if(!strcmp(type, "online", true))
	{
		// PlayerDutyGroup[playerid] 
		new gsid;
		if(sscanf(varchar,"d",gsid))
		{
			vlive_err(playerid,"/g online [slot]");
			return 1;
		}
		if(pGrupa[playerid][gsid][pGuid] <= 0 )
		{
			GuiInfo(playerid,"Nie jesteœ w takiej grupie.");
			return 1;
		}
		
		new groupUID = pGrupa[playerid][gsid][pGuid];
		
		new lista[512];
		for(new i = 0 ; i < MAX_PLAYERS; i++)
		{
			if(grupa[PlayerDutyGroup[i]][Guid] == pGrupa[playerid][gsid][pGuid])
			{
				format(lista,sizeof(lista),"%s\n"COL_GREEN"%s",lista,pInfo[i][pName]);
			}
			else
			{
				
				for(new gid = 1; i < MAX_PLAYER_GROUPS; i++)
				{
					if(pGrupa[i][gid][pGuid] == groupUID)
					{
						format(lista,sizeof(lista),"%s\n%s",lista,pInfo[i][pName]);
					}
				}
			}
		}
		
		ShowPlayerDialog(playerid,404,DIALOG_STYLE_LIST,"Gracze online",lista,"OK","");
		
	}
	if(!strcmp(type, "res", true))
	{
		new gsid;
		if(sscanf(varchar,"d",gsid))
		{
			vlive_err(playerid,"/g res [slot] - reset pojazdów");
			return 1;
		}
		
		if(pGrupa[playerid][gsid][pGuid] <= 0 )
		{
			GuiInfo(playerid,"Nie jesteœ w takiej grupie.");
			return 1;
		}
		
		if(pGrupa[playerid][gsid][permLeader] == 1)
		{
			for(new i = 0 ; i < MAX_VEHICLES; i++)
			{
				if(vehicle[i][vownertype] == 1)
				{
					if(vehicle[i][vowneruid] == pGrupa[playerid][gsid][pGuid])
					{
						new uid = vehicle[i][vuid];
						UnspawnVehicle(i);
						SpawnVehicle(uid);
					}
				}
			}
			
			//SpawnGroupVehicles(pGrupa[playerid][gsid][pGuid]);	
		}
		else
		{
			GuiInfo(playerid,"Brak uprawnieñ.");
		}
	}
	if(!strcmp(type, "zapros", true))
	{
		new gsid,worker;
		if(sscanf(varchar, "dd", gsid,worker))
	    {
	        SendClientMessage(playerid,COLOR_GREY, "/g zapros [slot] [playerid]");
	        return 1;
	    }
		if(pGrupa[playerid][gsid][pGuid] > 0)
		{
			if(pGrupa[playerid][gsid][permLeader] == 1)
			{
				if(IsPlayerConnected(worker))
				{
					new buffer[256];
					format(buffer,sizeof(buffer),"SELECT * FROM groups_workers WHERE char_uid=%i AND group_uid=%i",GetPlayerUID(worker),pGrupa[playerid][gsid][pGuid]);
					mysql_query(buffer);
					mysql_store_result();
					if (mysql_num_rows() > 0)
					{
						mysql_free_result();
						GuiInfo(playerid,"Ten gracz jest ju¿ w tej grupie.");
					}
					else
					{
						mysql_free_result();
						format(buffer,sizeof(buffer),"INSERT INTO groups_workers VALUES(%i,%i,'Brak',0,0,'0-0-0-0-0-0',0,'%s',0)",pGrupa[playerid][gsid][pGuid],GetPlayerUID(worker),pGrupa[playerid][gsid][pGname]);
						mysql_query(buffer);
						format(buffer,sizeof(buffer),"Gracz %s zaprosi³ Ciê do grupy %s (GID:%i)",pInfo[playerid][pName],pGrupa[playerid][gsid][pGname],pGrupa[playerid][gsid][pGuid]);
						SendClientMessage(worker,COLOR_GROUPS,buffer);
						format(buffer,sizeof(buffer),"Zaprosi³eœ gracza %s do grupy %s (GID:%i)",pInfo[worker][pName],pGrupa[playerid][gsid][pGname],pGrupa[playerid][gsid][pGuid]);
						SendClientMessage(playerid,COLOR_GROUPS,buffer);
						GetPlayerGroups(worker);
					}
				}
				else
				{
					GameTextForPlayer(playerid, "~r~ Ten gracz nie jest zalogowany.", 5000, 5);
				}
			}
			else
			{
				GuiInfo(playerid,"Brak uprawnieñ do wykonania tego polecenia.");
			}
		}
		else
		{
			GameTextForPlayer(playerid, "~r~ Nieprawid³owy slot grupy.", 5000, 5);
		}
	}
	if(!strcmp(type, "wypros", true))
	{
		new gsid,worker;
		if(sscanf(varchar, "dd", gsid,worker))
	    {
	        SendClientMessage(playerid,COLOR_GREY, "/g wypros [slot] [playerid]");
	        return 1;
	    }
		if(pGrupa[playerid][gsid][pGuid] > 0)
		{
			if(pGrupa[playerid][gsid][permLeader] == 1)
			{
				if(IsPlayerConnected(worker))
				{
					new buffer[256];
					format(buffer,sizeof(buffer),"SELECT * FROM groups_workers WHERE char_uid=%i AND group_uid=%i",GetPlayerUID(worker),pGrupa[playerid][gsid][pGuid]);
					mysql_query(buffer);
					mysql_store_result();
					if (mysql_num_rows() > 0)
					{
						// jest w grupie
						mysql_free_result();
						format(buffer,sizeof(buffer),"DELETE FROM groups_workers WHERE char_uid=%i AND group_uid=%i",GetPlayerUID(worker),pGrupa[playerid][gsid][pGuid]);
						mysql_query(buffer);
						new output[128];
						format(output,sizeof(output),"Wyprosi³eœ gracza %s z grupy %s (GID:%i)",pInfo[worker][pName],pGrupa[playerid][gsid][pGname],pGrupa[playerid][gsid][pGuid]);
						ClearPlayerGroupsTable(worker);
						SendClientMessage(playerid,COLOR_GROUPS,output);
						format(output,sizeof(output),"Zosta³eœ wyproszony przez gracza %s z grupy %s (GID:%i)",pInfo[playerid][pName],pGrupa[playerid][gsid][pGname],pGrupa[playerid][gsid][pGuid]);
						SendClientMessage(worker,COLOR_GROUPS,output);
						GetPlayerGroups(worker);
					}
					else
					{
						//nie jest w grupie
						mysql_free_result();
						SendClientMessage(playerid,COLOR_GROUPS,"Ten gracz nie jest cz³onkiem wskazanej grupy.");
					}
				}
				else
				{
					GameTextForPlayer(playerid, "~r~ Ten gracz nie jest zalogowany.", 5000, 5);
				}
			}
			else
			{
				GuiInfo(playerid,"Brak uprawnieñ do wykonania tego polecenia.");
			}
		}
		else
		{
			GameTextForPlayer(playerid, "~r~ Nieprawid³owy slot grupy.", 5000, 5);
		}
	}
	return 1;
}

COMMAND:agrupa(playerid,params[])
{
	if(CheckPlayerAccess(playerid) < PERMS_GAMEMASTER)
	{
		return 1;
	}
	
	if(pInfo[playerid][pGMGroups] != 1)
	{
		GuiInfo(playerid,"Brak uprawnieñ.");
		return 1;
	}
	
	new type[32], varchar[32];
	if(sscanf(params,"s[32]S()[32]",type,varchar))
	{
		SendClientMessage(playerid,COLOR_GREY,"/agrupa [info | stworz | usun | res | lider | lista | typ]");
		return 1;
	}
	if(!strcmp(type, "info", true))
	{
		new sampid;
		if(sscanf(varchar,"d",sampid))
		{
			SendClientMessage(playerid,COLOR_GREY,"/agrupa info [SampID]");
			return 1;
		}
		new info[512];
		new buffer[256];
		format(buffer,256,"SELECT * FROM groups_workers WHERE group_uid=%i",grupa[sampid][Guid]);
		mysql_query(buffer);
		mysql_store_result();
		new count_pracownicy = mysql_num_rows();
		mysql_free_result();
		new typ = grupa[sampid][Gtype];
		new typee[32];
		for(new i= 0; i < 17; i++)
		{
			if(GroupTypes[i][grTypeInt] == typ)
			{
				format(typee,32,"%s",GroupTypes[i][grType]);
			}
		}
		format(info,512,"Informacje o grupie\n===============\nNazwa grupy: \t\t%s\nIloœæ pracowników: \t\t%i\nTyp grupy: \t\t%s\nFundusze: \t\t%i\nUID grupy: \t\t%i",grupa[sampid][Gname],count_pracownicy,typee,grupa[sampid][Gmoney],grupa[sampid][Guid]);
		ShowPlayerDialog(playerid,404,DIALOG_STYLE_LIST,"Informacje o grupie",info,"OK","");
	}
	if(!strcmp(type, "stworz", true))
	{
		new name[32];
		if(sscanf(varchar,"s[32]",name))
		{
			SendClientMessage(playerid,COLOR_GREY,"/agrupa stworz [nazwa]");
			return 1;
		}
		new buffer[256];
		format(buffer,256,"INSERT INTO groups_list VALUES(NULL,'%s','BRAK',0,0,808280)",name);
		mysql_query(buffer);
		LoadGroupsOnStart();	//niech reloaduje grupy po wpisaniu nowej
		vlive_err(playerid,"stworzy³eœ now¹ grupê");
		
		new adminLog[128];
		format(adminLog,sizeof(adminLog),"~r~[grupy]~y~ Administrator %s ~g~(UID: %i | ID: %i)~y~ tworzy grupe ~g~%s",pInfo[playerid][pName],GetPlayerUID(playerid),playerid,name);
		LogAdminAction(adminLog);
	}
	if(!strcmp(type, "usun", true))
	{
		new gid;
		if(sscanf(varchar,"d",gid))
		{
			vlive_err(playerid,"/agrupa usun [groupid]");
			return 1;
		}
		new buffer[256];
		format(buffer,sizeof(buffer),"DELETE FROM groups_list WHERE group_uid=%i",grupa[gid][Guid]);
		mysql_query(buffer);
		format(buffer,sizeof(buffer),"DELETE FROM groups_workers WHERE group_uid=%i",grupa[gid][Guid]);
		new test = grupa[gid][Guid];
		mysql_query(buffer);
		
		new adminLog[128];
		format(adminLog,sizeof(adminLog),"~r~[grupy]~y~ Administrator %s ~g~(UID: %i | ID: %i)~y~ usuwa grupe %s ~g~(UID: %i)",pInfo[playerid][pName],GetPlayerUID(playerid),playerid,grupa[gid][Gname],grupa[gid][Guid]);
		LogAdminAction(adminLog);
		
		//reload
		CleanGroups();
		LoadGroupsOnStart();
		vlive_err(playerid,"usun¹³eœ grupê");
		
		for(new i=0;i< MAX_PLAYERS;i++)
		{
			for(new pgid = 1; pgid < MAX_PLAYER_GROUPS;pgid++)
			{
				if(pGrupa[i][pgid][pGuid] == test)
				{
					pGrupa[i][pgid][pGuid] = EOS;
				}
			}
		}
	}
	
	if(!strcmp(type, "res", true))
	{
		new gsid;
		if(sscanf(varchar,"d",gsid))
		{
			vlive_err(playerid,"/agrupa res [groupid]");
			return 1;
		}
		
		
		for(new i = 0 ; i < MAX_VEHICLES; i++)
		{
			if(vehicle[i][vownertype] == 1)
			{
				if(vehicle[i][vowneruid] == grupa[gsid][Guid])
				{
					new uid = vehicle[i][vuid];
					UnspawnVehicle(i);
					SpawnVehicle(uid);
				}
			}
		}			
	}
	
	if(!strcmp(type, "lider", true))
	{
		new gid,target;
		if(sscanf(varchar,"dd",gid,target))
		{
			vlive_err(playerid,"/agrupa lider [groupid] [playerid]");
			return 1;
		}
		new buffer[256];
		format(buffer,sizeof(buffer),"INSERT INTO groups_workers VALUES(%i,%i,'Lider',0,0,'1-1-1-1-1-1',0,'%s',0)",grupa[gid][Guid],pInfo[target][pUID],grupa[gid][Gname]);
		mysql_query(buffer);
		new output[256];
		format(output,256,"doda³eœ gracza %s jako lidera grupy %s (SID:%i)",pInfo[target][pName],grupa[gid][Gname],gid);
		vlive_err(playerid,output);
		GuiInfo(target,"Administrator mianowa³ Ciê liderem grupy.");
		
		new adminLog[128];
		format(adminLog,sizeof(adminLog),"~r~[grupy]~y~ Administrator %s ~g~(UID: %i | ID: %i)~y~ nadaje lidera grupy %s (SID: %i) graczowi %s ~g~(UID: %i | ID: %i)",
		pInfo[playerid][pName],
		GetPlayerUID(playerid),
		playerid,
		grupa[gid][Gname],
		gid,
		pInfo[target][pName],
		GetPlayerUID(target),
		target);
		LogAdminAction(adminLog);
	}
	if(!strcmp(type, "lista", true))
	{
		/*new lista[2048];
		for(new i=0;i < MAX_GROUPS; i++)
		{
			format(lista,1024,"%s\n%i\t\t%s",lista,i,grupa[i][Gname]);
		}
		ShowPlayerDialog(playerid,404,DIALOG_STYLE_LIST,"Lista grup",lista,"OK","");*/
		new lista[512];
		PlayerCache[playerid][pGroupListPage] = 0;
		for(new i=1;i<10;i++)
		{
			if(i == 9)
			{
				format(lista,1024,"%s\n###########\n>>>>>>>>>>>>",lista);
			}
			else
			{
				format(lista,1024,"%s\n%i\t\t%s",lista,i,grupa[i][Gname]);
			}
		}
		ShowPlayerDialog(playerid,DIAL_SHOW_GROUPS,DIALOG_STYLE_LIST,"Lista grup",lista,"OK","");
	}
	if(!strcmp(type, "typ", true))
	{
		new gid;
		if(sscanf(varchar,"d",gid))
		{
			vlive_err(playerid,"/agrupa typ [groupid]");
			return 1;
		}
		
		if(grupa[gid][Guid] <= 0)
		{
			vlive_err(playerid,"nie ma takiej grupy");
			return 1;
		}
		
		SelectedAdminGroup[playerid] = grupa[gid][Guid];
		ShowPlayerDialog(playerid,DIAL_G_TYPE,DIALOG_STYLE_LIST,"Wybierz nowy typ dla grupy","1. Warsztat\n2. Restauracja\n3. Salon samochodowy\n4. Si³ownia\n5. FBI\n6. San News\n7. Mafia\n8. Organizacja przestêpcza\n9. Œciganci\n10. Szajka\n11. LSFD\n12. LSPD\n13. LSMC\n14. GOV\n15. Brak\n16. Ochroniarze\n17. TAXI\n18. CoffeeShop\n19. Kartel\n20. Syndykat","ZatwierdŸ","Anuluj");
	}
	return 1;
}

stock AddGroupMoney(gid,amount)
{
	new buffer[256];
	format(buffer,sizeof(buffer),"UPDATE groups_list SET group_money=group_money+%i WHERE group_uid=%i",amount,gid);
	mysql_query(buffer);
}

stock TakeGroupMoney(gid,amount)
{
	new buffer[256];
	format(buffer,sizeof(buffer),"UPDATE groups_list SET group_money=group_money-%i WHERE group_uid=%i",amount,gid);
	mysql_query(buffer);
}

COMMAND:reklama(playerid,params[])
{
	if(AdSend > 0)
	{
		GuiInfo(playerid,"Jedna reklama ju¿ jest nadana.");
		return 1;
	}
	new tresc[128];
	if(sscanf(params,"s[128]",tresc))
	{
		vlive_err(playerid,"/reklama [tresc]");
		return 1;
	}
	
	if(DutyGroupType[playerid] != TYPE_SANNEWS)
	{
		GuiInfo(playerid,"Nie posiadasz odpowiednich uprawnieñ.");
		return 1;
	}
	
	new sanMsg[256];
	format(sanMsg,sizeof(sanMsg),"~g~Reklama ~>~ ~w~%s",tresc);
	TextDrawSetString(Text:TextDrawSanNews, sanMsg);
	
	SetTimer("SanNewsTextdrawReturn", 180000, false);
	AdSend = 1;
	
	return 1;
}

forward SanNewsTextdrawReturn();
public SanNewsTextdrawReturn()
{
	TextDrawSetString(Text:TextDrawSanNews,"San News ~>~ W radiu nadawany standardowy blok muzyczny.");
	AdSend = 0;
}

COMMAND:wywiad(playerid,params[])
{
	return 1;
}

COMMAND:mikrofon(playerid,params[])
{
	return 1;
}

COMMAND:live(playerid,params[])
{
	new tresc[128];
	if(sscanf(params,"s[128]",tresc))
	{
		vlive_err(playerid,"/live [tresc | koniec]");
		return 1;
	}
	
	if(!strcmp(tresc, "koniec", true))
	{
		TextDrawSetString(Text:TextDrawSanNews,"San News ~>~ W radiu nadawany standardowy blok muzyczny.");
		return 1;
	}
	
	if(DutyGroupType[playerid] != TYPE_SANNEWS)
	{
		GuiInfo(playerid,"Nie posiadasz odpowiednich uprawnieñ.");
		return 1;
	}
	
	new sanMsg[128];
	new imie[32],nazwisko[32];
	sscanf(pInfo[playerid][pName],"p<_>s[32]s[32]",imie,nazwisko);
	format(sanMsg,sizeof(sanMsg),"Na zywo ~>~ ~g~%s %s:~w~ %s",imie,nazwisko, tresc);
	TextDrawSetString(Text:TextDrawSanNews,sanMsg);
	
	new nearMsg[256];
	format(nearMsg,sizeof(nearMsg),": %s",tresc);
	SendPlayerRadioToNearPlayers(playerid,nearMsg);
	return 1;
}