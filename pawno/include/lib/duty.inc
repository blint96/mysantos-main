#define MINIUM_DUTY_TIME	0

#define BAZOWA_KWOTA_PD	200


new EnabledGPS = 0;
new FocusedCar = 0;


new Text:TextDrawDutyTime[MAX_PLAYERS];
new Text:TextDrawInfo[MAX_PLAYERS];

new PlayerDutySeconds[MAX_PLAYERS];
new IsPlayerDuty[MAX_PLAYERS];

new RepairTime[MAX_PLAYERS];

new pTimerDutyTime[MAX_PLAYERS];

stock SetPlayerDuty(playerid,groupid)
{
	SetTimerEx("IsPlayerOnDuty", 1000, false, "i", playerid);
	PlayerDutyGroup[playerid] = groupid;
	IsPlayerDuty[playerid] = 1;
	
	statusPlayer[playerid] = 1;		//odœwie¿ statusy
}

forward IsPlayerOnDuty(playerid);
public IsPlayerOnDuty(playerid)
{
	if(IsPlayerDuty[playerid] > 0)
	{
		//duty dalej leci
		if(IsPlayerConnected(playerid))
		{
			SetTimerEx("IsPlayerOnDuty", 1000, false, "i", playerid);
			PlayerDutySeconds[playerid]++;
			
			//tutaj niech refreszuje txdraw ((CODE REFRESH))
			
			//new currentDuty = gettime() - pTimerDutyTime[playerid];
			new Float:minutes = PlayerDutySeconds[playerid]/60;
			//new Float:hours = minutes / 60;
			
			new string[32];
			format(string,sizeof(string),"~y~Duty ~g~%i ~y~min",floatround(minutes));
			
			TextDrawSetString(Text:TextDrawDutyTime[playerid], string);
			//TextDrawShowForPlayer(playerid, Text:TextDrawDutyTime[playerid]);
		}
	}
	else
	{
		//koniec duty		
		new buffer[256],date[16],time[16];
		new rok,miesiac,dzien,godzina,minuta,sekunda;
		getdate(rok,miesiac,dzien);
		gettime(godzina,minuta,sekunda);
		format(date,16,"%i-%i-%i",rok,miesiac,dzien);
		format(time,16,"%i:%i:%i",godzina,minuta,sekunda);
		
		format(buffer,sizeof(buffer),"INSERT INTO duty_history VALUES (NULL,'%s',%i,%i,'%s','%s')",pInfo[playerid][pName],pGrupa[playerid][PlayerDutyGroup[playerid]][pGuid],PlayerDutySeconds[playerid]/60,date,time);
		mysql_query(buffer);

		
		//pobieranie kasy, dodawanie kasy
		new Days = getdate();
		format(buffer,sizeof(buffer),"SELECT last_pd FROM groups_workers WHERE char_uid=%i AND group_uid=%i",GetPlayerUID(playerid),pGrupa[playerid][PlayerDutyGroup[playerid]][pGuid]);
		mysql_query(buffer);
		mysql_store_result();
		new last_pd = mysql_fetch_int();
		
		format(buffer,sizeof(buffer),"SELECT group_money FROM groups_list WHERE group_uid=%i",pGrupa[playerid][PlayerDutyGroup[playerid]][pGuid]);
		mysql_query(buffer);
		mysql_store_result();
		new gmoney = mysql_fetch_int();
		mysql_free_result();
		
		mysql_free_result();
		if(Days > last_pd)
		{
			if(PlayerDutySeconds[playerid]/60 >= 30)
			{
				//AddPlayerMoney(playerid,pGrupa[playerid][PlayerDutyGroup[playerid]][pPayday]);
				//dodatek
				if(DutyGroupType[playerid] != TYPE_ORG || DutyGroupType[playerid] != TYPE_MAFIA)
				{
					AddPlayerMoney(playerid,BAZOWA_KWOTA_PD);
				}
				
				if(pGrupa[playerid][PlayerDutyGroup[playerid]][pPayday] <= gmoney)
				{
					TakeGroupMoney(pGrupa[playerid][PlayerDutyGroup[playerid]][pGuid], pGrupa[playerid][PlayerDutyGroup[playerid]][pPayday] );
					
					AddPlayerMoney(playerid,pGrupa[playerid][PlayerDutyGroup[playerid]][pPayday]);
				}
				else
				{
					vlive_err(playerid,"grupa nie ma tyle kasy, by wyp³aciæ PayDay");
					return 1;
				}
				//new buffer[256];
				
				format(buffer,sizeof(buffer),"UPDATE groups_workers SET last_pd=%i WHERE char_uid=%i AND group_uid=%i",Days,GetPlayerUID(playerid),pGrupa[playerid][PlayerDutyGroup[playerid]][pGuid]);
				mysql_query(buffer);
				
				new output[128];
				format(output,sizeof(output),"Otrzyma³eœ wyp³atê z grupy %s w wysokoœci "COL_GREEN"%i$",pGrupa[playerid][PlayerDutyGroup[playerid]][pGname],pGrupa[playerid][PlayerDutyGroup[playerid]][pPayday]);
				GuiInfo(playerid,output);
			}
			
			IsPlayerDuty[playerid] = 0;
			PlayerDutyGroup[playerid] = 0;
			PlayerDutySeconds[playerid] = 0;
			DutyGroupType[playerid] = 0;
		}

	}
	return 1;
}

COMMAND:duty(playerid,params[])
{
	if(DutyTimeToReduty[playerid] > 0)
	{
		GuiInfo(playerid,"Nie mo¿esz tak czêsto u¿ywaæ tej komendy.");
		return 1;
	}
	new type[32],varchar[32];
	if(sscanf(params,"s[32]S()[32]",type,varchar))
	{
		SendClientMessage(playerid,COLOR_GREY,"/duty [admin | grupa] [slot]");
		return 1;
	}
	if(!strcmp(type, "admin", true))
	{
		if(CheckPlayerAccess(playerid) > PERMS_HELPER)
		{
			if(pInfo[playerid][pAdmin] > 0)
			{
				pInfo[playerid][pAdmin] = 0;	
				statusPlayer[playerid] = 1;				
				vlive_err(playerid,""COL_SUPPORT"schodzisz ze s³u¿by administratora");	
				pInfo[playerid][pHealth] = 100;
				SetPlayerHealth(playerid,pInfo[playerid][pHealth]);				
			}
			else
			{
				vlive_err(playerid,""COL_SUPPORT"wchodzisz na s³u¿bê administratora");
				pInfo[playerid][pAdmin] = 1;		
				statusPlayer[playerid] = 1;
				pInfo[playerid][pHealth] = 100;
				SetPlayerHealth(playerid,pInfo[playerid][pHealth]);
				switch(CheckPlayerAccess(playerid))
				{
					case 0:
					{
						//brak
					}
					case 1:		//helper
					{
						SetPlayerColor(playerid,COLOR_ORANGE);
					}
					case 2:		//support
					{
						SetPlayerColor(playerid,COLOR_SUPPORT);
					}
					case 3:		//gm
					{
						SetPlayerColor(playerid,COLOR_GM);
					}
					case 4:		//admin
					{
						SetPlayerColor(playerid,COLOR_RED);
					}
					case 5:		//programista
					{
						SetPlayerColor(playerid,COLOR_RED);
					}
				}
			}
		}
	}
	if(!strcmp(type, "grupa", true))
	{
		new groupid;
		if(sscanf(varchar,"d",groupid))
		{
			TextDrawHideForPlayer(playerid, Text:TextDrawDutyTime[playerid]);
			vlive_err(playerid,"/duty grupa [SLOT]");
			new output[128];
			format(output,128,"zakoñczy³eœ s³u¿bê w grupie - przepracowa³eœ %i minut",PlayerDutySeconds[playerid]/60);
			duty_msg(playerid,output);
			return 1;
		}
		if(IsPlayerDuty[playerid] <= 0)
		{

			if(pGrupa[playerid][groupid][pGuid] <= 0)
			{
				GameTextForPlayer(playerid, "~r~Nieprawidlowy slot grupy", 5000, 5);
				return 1;
			}
			
			
			statusPlayer[playerid] = 1;
			PlayerDutySeconds[playerid] = 0;
			PlayerDutyGroup[playerid] = groupid;
			IsPlayerDuty[playerid] = 1;
			
			new buffer[256];
			format(buffer,sizeof(buffer),"SELECT group_type FROM groups_list WHERE group_uid=%i",pGrupa[playerid][groupid][pGuid]);
			mysql_query(buffer);
			mysql_store_result();
			new groupType = mysql_fetch_int();
			mysql_free_result();
			DutyGroupType[playerid] = groupType;
			
			SetTimerEx("IsPlayerOnDuty", 1000, false, "i", playerid);
			
			pTimerDutyTime[playerid] = gettime();
			TextDrawShowForPlayer(playerid, Text:TextDrawDutyTime[playerid]);
			
			new output[128];
			format(output,128,"wchodzisz na s³u¿bê grupy %s",pGrupa[playerid][groupid][pGname]);
			duty_msg(playerid,output);
			DutyTimeToReduty[playerid] = 12;
		}
		else
		{
			TextDrawHideForPlayer(playerid, Text:TextDrawDutyTime[playerid]);
			statusPlayer[playerid] = 1;
			new output[128];
			format(output,128,"zakoñczy³eœ s³u¿bê w grupie %s - przepracowa³eœ %i minut",pGrupa[playerid][groupid][pGname],PlayerDutySeconds[playerid]/60);
			duty_msg(playerid,output);
			IsPlayerDuty[playerid]  = 0;
			DutyGroupType[playerid] = EOS;
		}
		
	}
	return 1;
}

stock duty_msg(playerid,tresc[])
{
	new output[256];
	format(output,sizeof(output),""COL_SERV" S³u¿ba:"COL_DUTY" %s",tresc);
	SendClientMessage(playerid,COLOR_GREY,output);
}

COMMAND:blokuj(playerid,params[])
{
	new vehicleID = GetNearVehicle(playerid);
	if(DutyGroupType[playerid] == TYPE_LSPD)
	{
		new price;
		if(sscanf(params,"d",price))
		{
			vlive_err(playerid,"/blokuj [cena]");
			return 1;
		}
		
		if(IsVehicleBlocked(vehicleID))
		{
			GuiInfo(playerid,"Ten pojazd jest ju¿ zablokowany.");
			return 1;
		}
		
		new buffer[256];
		format(buffer,sizeof(buffer),"INSERT INTO veh_blocks VALUES(%i,%i)",vehicle[vehicleID][vuid],price);
		mysql_query(buffer);
		
		new msg[128];
		format(msg,sizeof(msg),"Zablokowa³eœ pojazd %s (SampID: %i) - cena odblokowania: %i",vehicle[vehicleID][vname],vehicleID,price);
		GuiInfo(playerid,msg);
	}
	else
	{
		GuiInfo(playerid,"Nie posiadasz odpowiednich uprawnieñ do pojazdu.");
	}
	
	return 1;
}

COMMAND:odblokuj(playerid,params[])
{
	new vehicleid,targetid;
	if(sscanf(params,"dd",vehicleid,targetid))
	{
		vlive_err(playerid,"/odblokuj [sampid pojazdu] [gracz]");
		return 1;
	} 
	
	if(IsPlayerInRangeOfVehicle(playerid,vehicleid,5))
	{
		if(IsVehicleBlocked(vehicleid))
		{
			if(IsPlayerInRangeOfPlayer(playerid,targetid,5))
			{
				//odblokuj ten pojazd
				new buffer[256];
				format(buffer,sizeof(buffer),"SELECT cost FROM veh_blocks WHERE uid=%i",vehicle[vehicleid][vuid]);
				mysql_query(buffer);
				mysql_store_result();
				new cost = mysql_fetch_int();
				mysql_free_result();
				
				
				SoldItemUID[targetid] = vehicleid;
				Sprzedawca[targetid] = playerid;
				Kupiec[playerid] = targetid;
				OfferPrice[targetid] = cost;
				OfferType[targetid] = OFFER_TYPE_UNBLOCK;
				
				ConstructOffer(playerid,targetid,OFFER_TYPE_UNBLOCK,"",cost);
			}
			else
			{
				GuiInfo(playerid,"Gracz nie jest w pobli¿u.");
			}
		}
		else
		{
			GuiInfo(playerid,"Ten pojazd nie jest zablokowany.");
		}
	}
	else
	{
		GuiInfo(playerid,"Pojazd nie jest w pobli¿u.");
	}
	
	return 1;
}


//czaty do poprawy póŸniej
CMD:ro(playerid,params[])
{
	new gsid,text[256];
	if(sscanf(params,"is[256]",gsid,text))
	{
		vlive_err(playerid,"/ro [slot] [wiadomosc]");
		return 1;
	}
	
	if(pInfo[playerid][blockOOC] > 0)
	{
		vlive_err(playerid,"nadal masz na³o¿on¹ blokadê OOC, nie napiszesz nic na czacie grupy");
		return 1;
	}
	
	if(gsid > MAX_PLAYER_GROUPS)
	{
		GameTextForPlayer(playerid, "~r~Nieprawidlowy slot grupy", 5000, 5);
		return 1;
	}
	
	new globalGroupID;
	if(pGrupa[playerid][gsid][pGuid] <= 0)
	{
		GameTextForPlayer(playerid, "~r~Nieprawidlowy slot grupy", 5000, 5);
		return 1;
	}
	
	for(new i=0; i < MAX_GROUPS; i++)
	{
		if(grupa[i][Guid] == pGrupa[playerid][gsid][pGuid])
		{
			globalGroupID = i;
			break;
		}
	}
		
	//wiadomosc do pozostalych w grupie
	for(new g = 0; g < MAX_PLAYERS ; g++)
	{
		new privateGroupID;
		for(new i= 1; i < MAX_PLAYER_GROUPS;i++)
		{
			if(pGrupa[g][i][pGuid] == pGrupa[playerid][gsid][pGuid])
			{
				privateGroupID = i;
				break;
			}
		}
		if(privateGroupID != 0)
		{
			//tutaj wiadomosc jak juz mamy guid
			new other[128];
			format(other,sizeof(other),"[#%i %s] (( %s [%i]: %s ))",privateGroupID,grupa[globalGroupID][Gtag],pInfo[playerid][pName],playerid,text);
			SendCustomPlayerMessage(g,COLOR_DEFALUT_GROUP,other);
		}
	}
	
	return 1;
}

CMD:r(playerid,params[])
{
	new gsid,text[256];
	if(sscanf(params,"ds[256]",gsid,text))
	{
		vlive_err(playerid,"/r [slot] [wiadomosc]");
		return 1;
	}
	
	if(gsid > MAX_PLAYER_GROUPS)
	{
		GameTextForPlayer(playerid, "~r~Nieprawidlowy slot grupy", 5000, 5);
		return 1;
	}
	new globalGroupID;
	if(pGrupa[playerid][gsid][pGuid] <= 0)
	{
		GameTextForPlayer(playerid, "~r~Nieprawidlowy slot grupy", 5000, 5);
		return 1;
	}
	for(new i=0; i < MAX_GROUPS; i++)
	{
		if(grupa[i][Guid] == pGrupa[playerid][gsid][pGuid])
		{
			globalGroupID = i;
			break;
		}
	}
		
	if(grupa[globalGroupID][Gtype] == TYPE_LSPD ) { }
	else if(grupa[globalGroupID][Gtype] == TYPE_LSFD ) { }
	else if(grupa[globalGroupID][Gtype] == TYPE_LSMC ) { }
	else if(grupa[globalGroupID][Gtype] == TYPE_GOV ) { }
	else if(grupa[globalGroupID][Gtype] == TYPE_FBI) { }
	else if(grupa[globalGroupID][Gtype] == TYPE_OCHRONIARZE) { }
	else
	{
		GameTextForPlayer(playerid, "~r~Ta grupa nie ma dostepu do radia", 5000, 5);
		return 1;
	}
		
		
	//wiadomosc do pozostalych w grupie
	for(new g = 0; g < MAX_PLAYERS ; g++)
	{
		new privateGroupID;
		for(new i= 1; i < MAX_PLAYER_GROUPS;i++)
		{
			if(pGrupa[g][i][pGuid] == pGrupa[playerid][gsid][pGuid])
			{
				privateGroupID = i;
				break;
			}
		}
		if(privateGroupID != 0)
		{
			//tutaj wiadomosc jak juz mamy guid
			new other[128];
			format(other,sizeof(other),"#%i ** [%s] %s: %s **",privateGroupID,grupa[globalGroupID][Gtag],pInfo[playerid][pName],text);
			//SendClientMessage(g,COLOR_DEFAULT_RADIO,other);
			SendCustomPlayerMessage(g, COLOR_DEFAULT_RADIO, other);
		}
	}
	SendRadioTalking(playerid,text);
	return 1;
}

COMMAND:montuj(playerid,params[])
{
	new vehicleid,part[32];
	if(sscanf(params,"ds[32]",vehicleid,part))
	{
		vlive_err(playerid,"/montuj [vehicle] [szyby | silnik]");
		return 1;
	}
	
	if(DutyGroupType[playerid] == TYPE_SCIGANT || DutyGroupType[playerid] == TYPE_WORKSHOP)
	{
		// mala przerobka
	}
	else
	{
		GuiInfo(playerid,"Nie masz uprawnieñ do wykonania tej komendy.");
		return 1;
	}
	
	if(IsPlayerInRangeOfVehicle(playerid,vehicleid,5))
	{
		if(!strcmp(part, "szyby", true))
		{
			if(DutyGroupType[playerid] != TYPE_SCIGANT )
			{
				GuiInfo(playerid,"Brak uprawnieñ.");
				return 1;
			}
			
			
			if(vehicle[vehicleid][vPrzyciemniane] > 0)
			{
				GuiInfo(playerid,"Ten pojazd posiada ju¿ zamontowane szyby przyciemniane.");
				return 1;
			}
			else
			{
				new buffer[256];
				format(buffer,sizeof(buffer),"UPDATE vehicles_list SET veh_windows = 1 WHERE veh_uid=%i",vehicle[vehicleid][vuid]);
				mysql_query(buffer);
				vehicle[vehicleid][vPrzyciemniane] = 1;
				
				GuiInfo(playerid,"Zamontowa³eœ przyciemniane szyby w tym pojeŸdzie.");
			}
		}
		
		if(!strcmp(part, "silnik", true))
		{
			ModdingVehicle[playerid] = vehicleid;
			ShowPlayerDialog(playerid,DIAL_MODDING_VEHICLE,DIALOG_STYLE_LIST,"Monta¿ silnika","1. Firmowy\n2. Modyfikowany\n3. Sportowy","Wybierz","Anuluj");
		}
	}
	else
	{
		GuiInfo(playerid,"Ten pojazd jest poza Twoim zasiêgiem.");
	}
	
	return 1;
}

COMMAND:wymontuj(playerid,params[])
{
	new vehicleid,part[32];
	if(sscanf(params,"ds[32]",vehicleid,part))
	{
		vlive_err(playerid,"/wymontuj [vehicle] [szyby]");
		return 1;
	}
	
	if(DutyGroupType[playerid] != TYPE_SCIGANT)
	{
		GuiInfo(playerid,"Nie masz uprawnieñ do wykonania tej komendy.");
		return 1;
	}
	
	if(IsPlayerInRangeOfVehicle(playerid,vehicleid,5))
	{
		if(!strcmp(part, "szyby", true))
		{
			if(vehicle[vehicleid][vPrzyciemniane] == 0)
			{
				GuiInfo(playerid,"Ten pojazd nie ma przyciemnianych szyb.");
				return 1;
			}
			else
			{
				new buffer[256];
				format(buffer,sizeof(buffer),"UPDATE vehicles_list SET veh_windows = 0 WHERE veh_uid=%i",vehicle[vehicleid][vuid]);
				mysql_query(buffer);
				vehicle[vehicleid][vPrzyciemniane] = 0;
				
				GuiInfo(playerid,"Wymontowa³eœ przyciemniane szyby w tym pojeŸdzie.");
			}
		}
	}
	else
	{
		GuiInfo(playerid,"Ten pojazd jest poza Twoim zasiêgiem.");
	}
	
	return 1;
}

COMMAND:napraw(playerid,params[])
{
	new target;
	if(sscanf(params,"d",target))
	{
		vlive_err(playerid,"/napraw [playerid]");
		return 1;
	}
	
	if(IsPlayerInAnyVehicle(target))
	{
		if(!IsPlayerHasNarzedzia(playerid))
		{
			GuiInfo(playerid,"Nie posiadasz narzêdzi, nie mo¿esz wiêc niczego oferowaæ.");
			return 1;
		}
		
		new vehicleid = GetPlayerVehicleID(target);
		if(IsPlayerInRangeOfPlayer(playerid,target,5))
		{
			new Float:VehHP;
			GetVehicleHealth(vehicleid,VehHP);
			if(VehHP >= 1000)
			{
				GuiInfo(playerid,"Ten pojazd jest w pe³ni sprawny.");
			}
			else
			{
				new vehicleModel = GetVehicleModel(vehicleid);
				new Float:hp = 1000 - VehHP;
				new cena = floatround(GetVehicleModelPrice(vehicleModel) * 0.00005 * hp);
				Sprzedawca[target] = playerid;
				Kupiec[playerid] = target;
				OfferPrice[target] = cena;
				OfferType[target] = OFFER_TYPE_REPAIR_NON;					
				ConstructOffer(playerid,target,OFFER_TYPE_REPAIR_NON,"",cena);
			}
		}
		else
		{
			GuiInfo(playerid,"Gracz poza zasiêgiem.");
		}
	}
	else
	{
		GuiInfo(playerid,"Ten gracz nie jest w pojeŸdzie.");
	}
	return 1;
}

COMMAND:kogut(playerid,params[])
{
	new kogutid;
	if(sscanf(params,"d",kogutid))
	{
		vlive_err(playerid,"/kogut [1 | 2]");
		return 1;
	}
	
	if(DutyGroupType[playerid] != TYPE_LSPD)
	{
		GuiInfo(playerid,"Brak uprawnieñ.");
		return 1;
	}
	
	if(IsPlayerInAnyVehicle(playerid))
	{
		if(kogutid == 1)
		{
			new vehicleid = GetPlayerVehicleID(playerid);
			if(IsValidObject(VehicleKogut[vehicleid]))
			{
				DestroyObject(VehicleKogut[vehicleid]);
				return 1;
			}
			
			VehicleKogut[vehicleid] = CreateObject(19420, 0,0,0, 0.0, 0.0, 96.0); 
			AttachObjectToVehicle(VehicleKogut[vehicleid], vehicleid, 0.0, 0.0, 0.85, 0.0, 0.0, 0.0);
		}
		if(kogutid == 2)
		{
			new vehicleid = GetPlayerVehicleID(playerid);
			if(IsValidObject(VehicleKogut[vehicleid]))
			{
				DestroyObject(VehicleKogut[vehicleid]);
				return 1;
			}
			VehicleKogut[vehicleid] = CreateObject(18646, 0,0,0, 0.0, 0.0, 96.0); 
			AttachObjectToVehicle(VehicleKogut[vehicleid], vehicleid, 0.0, 0.0, 0.85, 0.0, 0.0, 0.0);
		}
	}
	else
	{
		GuiInfo(playerid,"Musisz byæ w pojeŸdzie.");
	}
	return 1;
}

CMD:produkt(playerid,params[])
{
	if(GetPlayerDoorID(playerid) <= 0)
	{
		GuiInfo(playerid,"Musisz byæ w budynku by podaæ jakikolwiek produkt.");
		return 1;
	}

	new type[32], varchar[64];
	if(sscanf(params, "s[32]S()[64]", type, varchar))
	{
		vlive_err(playerid, "/produkt [podaj]");
		return 1;
	}
	
	if(!strcmp(type, "podaj", true))
	{
		new target,cena,item;
		if(sscanf(varchar,"ddd",target,item,cena))
		{
			vlive_err(playerid,"/produkt podaj [gracz] [itemuid] [cena]");
			return 1;
		}
		
		new doorid = GetPlayerDoorID(playerid);
		if(IsPlayerPermsDoors(playerid,doorid))
		{
			if(IsItemInDoors(item,doorid))
			{
				if(pInfo[playerid][pMoney] >= cena)
				{
					SoldItemUID[target] = item;
					Sprzedawca[target] = playerid;
					Kupiec[playerid] = target;
					OfferPrice[target] = cena;
					OfferType[target] = OFFER_TYPE_BITEM;
					
					//konstruktor ofert
					ConstructOffer(playerid,target,OFFER_TYPE_BITEM,"",cena);
				}
				else
				{
					GuiInfo(playerid,"Ten gracz nie ma tyle pieniêdzy.");
				}
			}
			else
			{
				GuiInfo(playerid,"Nie ma takiego przedmiotu w magazynie obecnego pomieszczenia.");
			}
		}
		else
		{
			GuiInfo(playerid,"Brak uprawnieñ.");
		}
		
	}
	
	return 1;
}

CMD:gps(playerid,params[])
{
	if(DutyGroupType[playerid] != TYPE_LSPD)
	{
		GuiInfo(playerid,"Brak uprawnieñ.");
		return 1;
	}
	
	new type[32], varchar[64];
	if(sscanf(params, "s[32]S()[64]", type, varchar))
	{
		vlive_err(playerid, "/gps [wylacz | zaznacz | wlacz | odznacz]");
		return 1;
	}
	
	if(!strcmp(type, "wlacz", true))
	{
		PlayerCache[playerid][pGPS] = 1;
		GuiInfo(playerid,"W³¹czy³eœ GPS.");
	}
	
	if(!strcmp(type, "wylacz", true))
	{
		PlayerCache[playerid][pGPS] = 0;
		GuiInfo(playerid,"Wy³¹czy³eœ GPS.");
		//TogglePlayerAllDynamicCPs(playerid, 0);
	}
	
	if(!strcmp(type, "zaznacz", true))
	{
		if(IsPlayerInAnyVehicle(playerid))
		{
			new plveh = GetPlayerVehicleID(playerid);
			
			EnabledGPS = 1;
			FocusedCar = plveh;
			
			HudText(playerid,"~b~wlaczyles namierzanie");
			
			SetTimer("RefreshGPS", 100, false);
		}
		else
		{
			GuiInfo(playerid,"Musisz byæ w pojeŸdzie by w³¹czyæ zaznaczanie.");
		}
	}
	
	if(!strcmp(type, "odznacz", true))
	{
		HudText(playerid,"~b~wylaczyles namierzanie");
		EnabledGPS = 0;
		FocusedCar = 0;
	}
	
	return 1;
}

forward RefreshGPS();
public RefreshGPS()
{
	for(new i = 0 ; i < MAX_PLAYERS;i++)
	{
		
		if(DutyGroupType[i] == TYPE_LSPD)
		{
			if(IsPlayerInVehicle(i,FocusedCar))
			{
				continue;
			}
			
			if(PlayerCache[i][pGPS] == 1)
			{
				DisablePlayerCheckpoint(i);
				//TogglePlayerAllDynamicCPs(i, 0);
				PlayerCache[i][pRubbishCheckpoint] = false;
				PlayerCache[i][pVehCheckpoint] = 0;
				PlayerCache[i][pGPSCP] = 1;
				
				//zaznacz od nowa
				new Float:Pos[3];
				GetVehiclePos(FocusedCar,Pos[0],Pos[1],Pos[2]);
				//CreateDynamicCP(Pos[0],Pos[1],Pos[2], 4.5,  -1, -1, i, 2000.0);
				SetPlayerCheckpoint(i, Pos[0],Pos[1],Pos[2], 6.0);
			}
		}
	}
	
	if(EnabledGPS == 1)
	{
		//timer ponów
		SetTimer("RefreshGPS", 1000, false);
	}
	
	return 1;
}



stock LoadDutyText()
{
	//textdrawy duty
	for(new i = 0 ; i < MAX_PLAYERS;i++)
	{
		//duty
		TextDrawDutyTime[i] = TextDrawCreate(503.000000, 125.000000, "_");
		TextDrawBackgroundColor(TextDrawDutyTime[i], 255);
		TextDrawFont(TextDrawDutyTime[i], 1);
		TextDrawLetterSize(TextDrawDutyTime[i], 0.200000, 0.899999);
		TextDrawColor(TextDrawDutyTime[i], -1);
		TextDrawSetOutline(TextDrawDutyTime[i], 1);
		TextDrawSetProportional(TextDrawDutyTime[i], 1);
		
		//info
		TextDrawInfo[i] = TextDrawCreate(503.000000, 135.000000, "_");
		TextDrawBackgroundColor(TextDrawInfo[i], 255);
		TextDrawFont(TextDrawInfo[i], 1);
		TextDrawLetterSize(TextDrawInfo[i], 0.200000, 0.899999);
		TextDrawColor(TextDrawInfo[i], -1);
		TextDrawSetOutline(TextDrawInfo[i], 1);
		TextDrawSetProportional(TextDrawInfo[i], 1);
	}
}

stock IsPlayerInBusinnesBuilding(playerid,groupuid)
{
	new doorid = GetPlayerDoorID(playerid);
	
	if(doorid <= 0)
	{
		return false;
	}
	else
	{
		if(DoorInfo[doorid][doorOwnerUID] == groupuid)
		{
			return true;
		}
	}
	
	return false;
}