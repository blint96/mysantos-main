#define BANNED_GROUP	//numerki zbanowanej grupy


// primary gun pos (pistolety)
#define PGUN_POS_OFFSETX -0.151255 
#define PGUN_POS_OFFSETY 0.007422
#define PGUN_POS_OFFSETZ 0.168980

#define PGUN_POS_ROTX 89.340888 
#define PGUN_POS_ROTY 168.753723 
#define PGUN_POS_ROTZ 2.200318 

//secondary gun pos (karabiny)
#define SGUN_POS_OFFSETX -0.132256
#define SGUN_POS_OFFSETY -0.175576
#define SGUN_POS_OFFSETZ 0.061981

#define SGUN_POS_ROTX -8.559116
#define SGUN_POS_ROTY 23.353717 
#define SGUN_POS_ROTZ 4.100318

//pa³ki i te inne
#define xGUN_POS_OFFSETX -0.313255
#define xGUN_POS_OFFSETY 0.100423
#define xGUN_POS_OFFSETZ -0.277019

#define xGUN_POS_ROTX 92.040893
#define xGUN_POS_ROTY -50.946281
#define xGUN_POS_ROTZ 9.100318

forward SetVariables(playerid);

stock SetMoney(playerid)
{
	if(GetPlayerMoney(playerid) != pInfo[playerid][pMoney])
	{
		ResetPlayerMoney(playerid);
		GivePlayerMoney(playerid,pInfo[playerid][pMoney]);
	}
}

stock SetHp(playerid)
{
	SetPlayerHealth(playerid,pInfo[playerid][pHealth]);
}

stock CheckJP(playerid)
{
	if(CheckPlayerAccess(playerid) > PERMS_SUPPORT)
	{
		return 0;
	}
	if(GetPlayerSpecialAction(playerid) == SPECIAL_ACTION_USEJETPACK)
	{
		system_kick(playerid, "Jetpack");
		return 1;
	}
	return 0;
}

stock CornersCheck(playerid)
{
	if(PlayerCache[playerid][pCornerID] == GetPlayerCurrentCorner(playerid))
	{
		PlayerCache[playerid][pCornerTime] += 1;
		
		if(PlayerCache[playerid][pCornerTime] < 600)
		{
			//moze sprzedac
			new string[128];
			format(string,sizeof(string),"~g~Jestes~y~ na terenie cornera ~n~poczekaj ~r~10 min~y~ by sprzedac towar");
			TextDrawSetString(Text:TextDrawInfo[playerid], string);
			TextDrawShowForPlayer(playerid, Text:TextDrawInfo[playerid]);
		}
		else
		{
			//nie moze
			new string[128];
			format(string,sizeof(string),"~g~Jestes na terenie cornera ~n~~g~mozesz~y~ sprzedac towar - komenda ~n~~r~/corner");
			TextDrawSetString(Text:TextDrawInfo[playerid], string);
			TextDrawShowForPlayer(playerid, Text:TextDrawInfo[playerid]);
		}
	}
	
	if(IsPlayerInAnyCorner(playerid))
	{
		PlayerCache[playerid][pCornerID] = GetPlayerCurrentCorner(playerid);
		PlayerCache[playerid][pCornered] = 1;
	}
	else
	{
		PlayerCache[playerid][pCornerID] = -1;
		PlayerCache[playerid][pCornered] = 0;
		TextDrawHideForPlayer(playerid, Text:TextDrawInfo[playerid]);
	}
}

stock CheckFly(playerid)
{
	if(PlayerCache[playerid][pCacheAccess] > PERMS_SUPPORT)
	{
		return 0;
	}
	new string[64];
	// Anty FlyHack
	new anim_id = GetPlayerAnimationIndex(playerid);
	new Float:x,Float:y,Float:PosZ;
	GetPlayerPos(playerid,x,y,PosZ);
	if(PosZ > 40.0 && (anim_id == 1543 || anim_id == 1538 || anim_id == 1539))
	{
		format(string, sizeof(string), "Flyhack: %.2f", PosZ);
		system_kick(playerid, string);
		return 1;
	}
	return 0;
}

stock GetPlayerWeaponAmmo(playerid, weaponid)
{
	new ammo, weapons[13][2];
	for(new i=0; i<13;i++)
	{
		GetPlayerWeaponData(playerid, i, weapons[i][0], weapons[i][1]);
		if(weapons[i][0] == weaponid)
		{
			ammo = weapons[i][1];
		}
	}
	return ammo;
}

new UsedWeaponID[MAX_PLAYERS];
stock GetPlayerWeaponID(playerid,weaponid)
{
	
}

stock AddWeaponModel(playerid)
{
	if(PlayerWeapon[playerid] > 0)
	{
		if(GetPlayerWeapon(playerid) <= 0)
		{
			//nie ma broni wiêc dodaj model
			if(UsedWeaponID[playerid] == 24||UsedWeaponID[playerid] == 23||UsedWeaponID[playerid] == 28|| UsedWeaponID[playerid] == 32|| UsedWeaponID[playerid] == 22|| UsedWeaponID[playerid] == 41)
			{
				switch(UsedWeaponID[playerid])
				{
					case 24:
					{
						//deagle
						SetPlayerAttachedObject(playerid, SLOT_CHUJWDUPIE, 348,1,  PGUN_POS_OFFSETX ,PGUN_POS_OFFSETY,PGUN_POS_OFFSETZ ,PGUN_POS_ROTX, PGUN_POS_ROTY, PGUN_POS_ROTZ ,1.000000 ,1.000000 ,1.000000);
					}
					case 23:
					{
						//silenced colt
						SetPlayerAttachedObject(playerid, SLOT_CHUJWDUPIE, 347, 1, PGUN_POS_OFFSETX ,PGUN_POS_OFFSETY,PGUN_POS_OFFSETZ ,PGUN_POS_ROTX, PGUN_POS_ROTY, PGUN_POS_ROTZ ,1.000000 ,1.000000 ,1.000000);
					}
					case 28:
					{
						//uzi
						SetPlayerAttachedObject(playerid, SLOT_CHUJWDUPIE, 352,1,  PGUN_POS_OFFSETX ,PGUN_POS_OFFSETY,PGUN_POS_OFFSETZ ,PGUN_POS_ROTX, PGUN_POS_ROTY, PGUN_POS_ROTZ ,1.000000 ,1.000000 ,1.000000);
					}
					case 32:
					{
						//tec 9
						SetPlayerAttachedObject(playerid, SLOT_CHUJWDUPIE, 372,1,  PGUN_POS_OFFSETX ,PGUN_POS_OFFSETY,PGUN_POS_OFFSETZ ,PGUN_POS_ROTX, PGUN_POS_ROTY, PGUN_POS_ROTZ ,1.000000 ,1.000000 ,1.000000);
					}
					case 22:
					{
						//colt
						SetPlayerAttachedObject(playerid, SLOT_CHUJWDUPIE, 346,1,  PGUN_POS_OFFSETX ,PGUN_POS_OFFSETY,PGUN_POS_OFFSETZ ,PGUN_POS_ROTX, PGUN_POS_ROTY, PGUN_POS_ROTZ ,1.000000 ,1.000000 ,1.000000);
					}
					case 41:
					{
						//spray
						SetPlayerAttachedObject(playerid, SLOT_CHUJWDUPIE, 365,1,  PGUN_POS_OFFSETX ,PGUN_POS_OFFSETY,PGUN_POS_OFFSETZ ,PGUN_POS_ROTX, PGUN_POS_ROTY, PGUN_POS_ROTZ ,1.000000 ,1.000000 ,1.000000);
					}
				}
			}
			
			//secondary
			else if(UsedWeaponID[playerid] == 8 || UsedWeaponID[playerid] == 9 || UsedWeaponID[playerid] == 31||UsedWeaponID[playerid] == 30||UsedWeaponID[playerid] == 27|| UsedWeaponID[playerid] == 25 || UsedWeaponID[playerid] == 29)
			{
				switch(UsedWeaponID[playerid])
				{
					case 8:
					{
						SetPlayerAttachedObject(playerid, SLOT_CHUJWDUPIE,339,1,  SGUN_POS_OFFSETX ,SGUN_POS_OFFSETY,SGUN_POS_OFFSETZ ,SGUN_POS_ROTX, SGUN_POS_ROTY, SGUN_POS_ROTZ ,1.000000 ,1.000000 ,1.000000);
					}
					case 9:
					{
						SetPlayerAttachedObject(playerid, SLOT_CHUJWDUPIE,341,1,  SGUN_POS_OFFSETX ,SGUN_POS_OFFSETY,SGUN_POS_OFFSETZ ,SGUN_POS_ROTX, SGUN_POS_ROTY, SGUN_POS_ROTZ ,1.000000 ,1.000000 ,1.000000);
					}
					case 31:
					{
						//m4
						SetPlayerAttachedObject(playerid, SLOT_CHUJWDUPIE,356,1,  SGUN_POS_OFFSETX ,SGUN_POS_OFFSETY,SGUN_POS_OFFSETZ ,SGUN_POS_ROTX, SGUN_POS_ROTY, SGUN_POS_ROTZ ,1.000000 ,1.000000 ,1.000000);
					}
					case 30:
					{
						//kalach
						SetPlayerAttachedObject(playerid, SLOT_CHUJWDUPIE,355,1,  SGUN_POS_OFFSETX ,SGUN_POS_OFFSETY,SGUN_POS_OFFSETZ ,SGUN_POS_ROTX, SGUN_POS_ROTY, SGUN_POS_ROTZ ,1.000000 ,1.000000 ,1.000000);
					}
					case 27:
					{
						//shotgan
						SetPlayerAttachedObject(playerid, SLOT_CHUJWDUPIE,351,1,  SGUN_POS_OFFSETX ,SGUN_POS_OFFSETY,SGUN_POS_OFFSETZ ,SGUN_POS_ROTX, SGUN_POS_ROTY, SGUN_POS_ROTZ ,1.000000 ,1.000000 ,1.000000);
					}
					case 25:
					{
						//shotgan
						SetPlayerAttachedObject(playerid, SLOT_CHUJWDUPIE,349,1,  SGUN_POS_OFFSETX ,SGUN_POS_OFFSETY,SGUN_POS_OFFSETZ ,SGUN_POS_ROTX, SGUN_POS_ROTY, SGUN_POS_ROTZ ,1.000000 ,1.000000 ,1.000000);
					}
					case 29:
					{
						SetPlayerAttachedObject(playerid, SLOT_CHUJWDUPIE,353,1,  SGUN_POS_OFFSETX ,SGUN_POS_OFFSETY,SGUN_POS_OFFSETZ ,SGUN_POS_ROTX, SGUN_POS_ROTY, SGUN_POS_ROTZ ,1.000000 ,1.000000 ,1.000000);
					}
				}
			}
			
			//pa³ki i tym podobne
			else if(UsedWeaponID[playerid] == 3 || UsedWeaponID[playerid] == 4 || UsedWeaponID[playerid] == 5 ||UsedWeaponID[playerid] == 9 )
			{
				switch(UsedWeaponID[playerid])
				{
					case 3:
					{
						SetPlayerAttachedObject(playerid, SLOT_CHUJWDUPIE,334,1,  xGUN_POS_OFFSETX ,xGUN_POS_OFFSETY,xGUN_POS_OFFSETZ ,xGUN_POS_ROTX, xGUN_POS_ROTY, xGUN_POS_ROTZ ,1.000000 ,1.000000 ,1.000000);
					}
					case 4:
					{
						SetPlayerAttachedObject(playerid, SLOT_CHUJWDUPIE,335,1,  xGUN_POS_OFFSETX ,xGUN_POS_OFFSETY,xGUN_POS_OFFSETZ ,xGUN_POS_ROTX, xGUN_POS_ROTY, xGUN_POS_ROTZ ,1.000000 ,1.000000 ,1.000000);
					}
					case 5:
					{
						SetPlayerAttachedObject(playerid, SLOT_CHUJWDUPIE,336,1,  xGUN_POS_OFFSETX ,xGUN_POS_OFFSETY,xGUN_POS_OFFSETZ ,xGUN_POS_ROTX, xGUN_POS_ROTY, xGUN_POS_ROTZ ,1.000000 ,1.000000 ,1.000000);
					}
				}
			}	
		}
		else
		{
			//ma broñ, wiêc usuñ model
			RemovePlayerAttachedObject(playerid, SLOT_CHUJWDUPIE);
		}
	}
	else
	{
		RemovePlayerAttachedObject(playerid, SLOT_CHUJWDUPIE);
	}
}

stock DeductAmmo(playerid)
{
	new weapon = GetPlayerWeapon(playerid);
	if(weapon != 0)
	{
		if(weapon == 46)
		{
			
		}
		else
		{
			new ammo = GetPlayerWeaponAmmo(playerid,PlayerCache[playerid][pWeaponID]);
			if(ammo < PlayerCache[playerid][pWeaponAmmo])
			{
		       	PlayerCache[playerid][pWeaponAmmo] = ammo;
			}
			if(ammo > PlayerCache[playerid][pWeaponAmmo])
			{
				//cziter!
				system_kick(playerid,"AmmoHack");
				return 1;
			}
			
			new buffer[256];
			format(buffer,sizeof(buffer),"UPDATE core_items SET item_value2=%i WHERE item_uid=%i",ammo,PlayerWeapon[playerid]);
			mysql_query(buffer);
		}
	}
	return 0;
}

stock CarnetTime(playerid)
{
	if(PlayerCache[playerid][pKarnet] > 0)
	{
		if(PlayerCache[playerid][pKarnetTime] > 0)
		{
			PlayerCache[playerid][pKarnetTime]--;

			new text[64];
			format(text,sizeof(text),"~n~~n~~n~~n~czas treningu: ~r~%i min",PlayerCache[playerid][pKarnetTime]/60);
			GameTextForPlayer(playerid, text, 1000, 5);
		}
		else
		{
			new buffer[256];
			format(buffer,sizeof(buffer),"DELETE FROM core_items WHERE item_uid=%i",PlayerCache[playerid][pKarnet]);
			mysql_query(buffer);
			
			PlayerCache[playerid][pKarnet] = 0;
			PlayerCache[playerid][pKarnetTime] = 0;
			
			if(PlayerCache[playerid][pTrain] > 0)
			{
				StopPlayerTrain(playerid);
			}
		}
	}
	
	return 1;
}

stock GetPlayerGameScore(playerid)
{
	new buffer[256];
	format(buffer,sizeof(buffer),"SELECT gamescore FROM mysa_members WHERE member_id=%i",pInfo[playerid][pGID]);
	mysql_query(buffer);
	mysql_store_result();
	new gamescore = mysql_fetch_int();
	mysql_free_result();
	return gamescore;
}

stock AddGameScore(playerid)
{
	if(PlayerCache[playerid][pOnlineSec] > 600)
	{
		//dodaj gsa
		PlayerCache[playerid][pOnlineSec] = 0;
		
		new buffer[256];
		format(buffer,sizeof(buffer),"UPDATE mysa_members SET gamescore=gamescore+1 WHERE member_id=%i",pInfo[playerid][pGID]);
		mysql_query(buffer);
		
		format(buffer,sizeof(buffer),"SELECT gamescore FROM mysa_members WHERE member_id=%i",pInfo[playerid][pGID]);
		mysql_query(buffer);
		mysql_store_result();
		
		new gamescore = mysql_fetch_int();
		
		SetPlayerScore(playerid,gamescore);
		
		mysql_free_result();
	}
	
	return 1;
}

public SetVariables(playerid)
{
	if(IsPlayerConnect(playerid))
	{
		if(!IsPlayerInAnyVehicle(playerid))
		{
			TextDrawHideForPlayer(playerid,TextDrawVehicleInfo[playerid]);
		}
		
		SpeedHack(playerid);
		
		SetMoney(playerid);
		
		PlayerCache[playerid][pOnlineSec]++;
		
		AddGameScore(playerid);
		
		CornersCheck(playerid);
		
		if(pInfo[playerid][pPremium] > 0)
		{
			SetPlayerColor(playerid,COLOR_GOLD);
		}
		
		if(PlayerCache[playerid][pCacheAccess] < PERMS_HELPER)
		{
			if(GetPlayerPing(playerid) > 600) system_kick(playerid,"Anty HP");
		}
		
		CheckPlayerStatus(playerid);
		
		AddWeaponModel(playerid);
		
		DeductAmmo(playerid);
		
		CarnetTime(playerid);
		
		CheckJP(playerid);
		
		CheckFly(playerid);
		
		new buffer[256];
		
		//format(buffer,sizeof(buffer),"UPDATE core_players SET char_time=char_time+1 WHERE char_uid=%i",GetPlayerUID(playerid));
		
		mysql_query(buffer);	
		
		GetVehicle(playerid);	
		
		//ForceLatex(playerid);
		
		SetHp(playerid);
		
		WeaponHack(playerid);
		
		BankomatTimer(playerid);
		
		if(DutyTimeToReduty[playerid] > 0)
		{
			DutyTimeToReduty[playerid]--;
		}
		
		if(PlayerCache[playerid][pIPChecked] == 0)
		{
			CheckBannedIP(playerid);
		}
		
		if(PlayerCache[playerid][pZP] > 0)
		{
			if(!IsPlayerInAnyVehicle(playerid))
			{
				PlayerCache[playerid][pZP] = 0;
				statusPlayer[playerid] = 1;
			}
		}	
		
		if(pInfo[playerid][blockChar] == 1)
		{
			Kick(playerid);
		}

		for(new i = 0 ; i < 10; i++)
		{
			if(GetPlayerVirtualWorld(playerid) <= 0)
			{
				if(IsValidObject(PozarInfo[i][pozObject]))
				{
					new Float:x,Float:y,Float:z;
					GetObjectPos(PozarInfo[i][pozObject],x,y,z);
					if(IsPlayerInRangeOfPoint(playerid,3,x,y,z))
					{
						pInfo[playerid][pHealth] = pInfo[playerid][pHealth] - 5;
					}
				}
			}
		}
		
		Streamer_Update(playerid);		// czy nie bedzie przeciazen?
		
		//kondycja
		if(IsPlayerRunning(playerid))
		{
			new Float:getCon = 0;
			if(pInfo[playerid][maxCondition] < 20)
			{
				getCon = pInfo[playerid][maxCondition] * (0.05 / 2);
				pInfo[playerid][maxCondition] = pInfo[playerid][maxCondition] + (3 * 0.000625);	//skillek
			}
			else if(pInfo[playerid][maxCondition] < 40)
			{
				getCon = pInfo[playerid][maxCondition] * (0.035 / 2);
				pInfo[playerid][maxCondition] = pInfo[playerid][maxCondition] + (3 * 0.0005);	//skillek
			}
			else if(pInfo[playerid][maxCondition] < 60)
			{
				getCon = pInfo[playerid][maxCondition] * (0.025 / 2) ;
				pInfo[playerid][maxCondition] = pInfo[playerid][maxCondition] + (3 * 0.000375);	//skillek
			}
			else if(pInfo[playerid][maxCondition] < 80)
			{
				getCon = pInfo[playerid][maxCondition] * (0.015 /2);
				pInfo[playerid][maxCondition] = pInfo[playerid][maxCondition] + (3 * 0.00025);	//skillek
			}
			else
			{
				getCon = pInfo[playerid][maxCondition] * (0.01 / 2);
				pInfo[playerid][maxCondition] = pInfo[playerid][maxCondition] + (3 * 0.000125);	//skillek
			}
			
			//new Float:getCon = pInfo[playerid][maxCondition] * 0.05;
			
			//pInfo[playerid][maxCondition] = pInfo[playerid][maxCondition] + 0.001;	//skillek
			
			pInfo[playerid][pCondition] = pInfo[playerid][pCondition] - getCon;
			SetProgressBarValue(stamina[playerid], pInfo[playerid][pCondition]);
			UpdateProgressBar(stamina[playerid], playerid);
			
			
			
			if(pInfo[playerid][pCondition] < pInfo[playerid][maxCondition] * 0.1)
			{
				//wymus akcje zmeczenia
				pInfo[playerid][pCondition] = pInfo[playerid][maxCondition] * 0.1;
				idleTime[playerid] = 5;
				SetTimerEx("SetPlayerExhausted", 1000, false, "i", playerid);
			}
		}
		else
		{
			if(pInfo[playerid][pCondition] >= pInfo[playerid][maxCondition])
			{
				//nie rob nic
			}
			else
			{
				if(PlayerCache[playerid][pTrain] == 0)
				{
					//dodaj kondyche
					new Float:dodajCon = pInfo[playerid][maxCondition] * 0.02;
					pInfo[playerid][pCondition] = pInfo[playerid][pCondition] + dodajCon;
					
					SetProgressBarValue(stamina[playerid], pInfo[playerid][pCondition]);
					UpdateProgressBar(stamina[playerid], playerid);
				}
				else
				{
					if(pInfo[playerid][pCondition] < pInfo[playerid][maxCondition] * 0.1)
					{
						//wymus akcje zmeczenia
						StopPlayerTrain(playerid);
						pInfo[playerid][pCondition] = pInfo[playerid][maxCondition] * 0.1;
						idleTime[playerid] = 5;
						SetTimerEx("SetPlayerExhausted", 1000, false, "i", playerid);
					}
				}
			}
		}
		
		if(IsPlayerInAnyArea(playerid))
		{
			//pobiera po prostu co sekunde strefe gracza
		}
		
		if(IsPlayerDuty[playerid] > 0)
		{
			if(DutyGroupType[playerid] == TYPE_LSFD || 
			DutyGroupType[playerid] == TYPE_LSPD || 
			DutyGroupType[playerid] == TYPE_LSMC || 
			DutyGroupType[playerid] == TYPE_SZAJK || 
			DutyGroupType[playerid] == TYPE_MAFIA || 
			DutyGroupType[playerid] == TYPE_ORG || 
			DutyGroupType[playerid] == TYPE_GOV ||
			DutyGroupType[playerid] == TYPE_FBI  ||
			DutyGroupType[playerid] == TYPE_SANNEWS  ||
			DutyGroupType[playerid] == TYPE_SYNDYKAT  ||
			DutyGroupType[playerid] == TYPE_TAXI  ||
			DutyGroupType[playerid] == TYPE_KARTEL 
			)
			{
				//nic nie rób, bo oni musz¹ biegaæ na zewn¹trz
			}
			else
			{
				//anuluj jak w innych drzwiach
				if(!IsPlayerInBusinnesBuilding(playerid,pGrupa[playerid][PlayerDutyGroup[playerid]][pGuid]))
				{
					statusPlayer[playerid] = 1;
					new output[128];
					format(output,128,"zakoñczy³eœ s³u¿bê - przepracowa³eœ %i minut (( wyjœcie poza teren pracy ))",PlayerDutySeconds[playerid]/60);
					duty_msg(playerid,output);
					IsPlayerDuty[playerid]  = 0;
					DutyGroupType[playerid] = EOS;
				}
			}
		}
		
		
		SetTimerEx("SetVariables", 1000, false, "i", playerid);
	}
	else
	{
		//nie polaczony
		if(PlayerCache[playerid][pKarnet] > 0)
		{
			DisableCarnet(playerid);
		}
	}
	return 1;
}

stock system_ban(target,reason[])
{
	new buffer[256],string[256];
	format(buffer,sizeof(buffer),"UPDATE core_players SET block_ban=1 , ban_reason='%s' WHERE char_uid=%i",reason,GetPlayerUID(target));
	mysql_query(buffer);
	
	//dodawanie ip
	new ip[16];
	GetPlayerIp(target, ip, sizeof(ip));
	format(buffer,sizeof(buffer),"INSERT INTO banned_ip VALUES(NULL,'%s')",ip);
	mysql_query(buffer);
	
	format(string, sizeof(string), "~r~~h~Ban~n~~w~Gracz: %s~n~Nadajacy: System~n~~y~~h~%s", pInfo[target][pName], reason);
	TextDrawSetString(Text:TextDrawPunishInfo, string);
	TextDrawShowForAll(Text:TextDrawPunishInfo);
	SetTimer("HidePunishText",6000,false);
	SetTimerEx("KickWithMessage",500,false,"i",target);
	printf("[kick] Gracz %s (UID: %i) zostal wyrzucony z serwera - powod: ban",pInfo[target][pName],GetPlayerUID(target));
	
	format(buffer,sizeof(buffer),"INSERT INTO penalties VALUES(NULL,'%s','%s',%i,0,'%s','%s',0)",getStringDate(),getStringTime(),GetPlayerUID(target),reason,"Ban");
	mysql_query(buffer);
}

stock system_msg(target,title[],giver,reason[])
{
	new string[256];
	
	if(giver == INVALID_PLAYER_ID)
	{
		format(string, sizeof(string), "~r~~h~%s~n~~w~Gracz: %s~n~Nadajacy: System~n~~y~~h~%s", title,pInfo[target][pName],reason);
	}
	else
	{
		format(string, sizeof(string), "~r~~h~%s~n~~w~Gracz: %s~n~Nadajacy: %s~n~~y~~h~%s", title,pInfo[target][pName],pInfo[giver][pName],reason);
	}
	
	TextDrawSetString(Text:TextDrawPunishInfo, string);
	TextDrawShowForAll(Text:TextDrawPunishInfo);
	SetTimer("HidePunishText",6000,false);
}

stock system_kick(target,reason[])
{
	new string[256];
	format(string, sizeof(string), "~r~~h~Kick~n~~w~Gracz: %s~n~Nadajacy: System~n~~y~~h~%s", pInfo[target][pName], reason);
	TextDrawSetString(Text:TextDrawPunishInfo, string);
	TextDrawShowForAll(Text:TextDrawPunishInfo);
	SetTimer("HidePunishText",6000,false);			
	new output[150];
	format(output,sizeof(output),"Zosta³eœ wyrzucony z serwera, powód : %s",reason);
	SendClientMessage(target,COLOR_SERVER,output);
	SetTimerEx("KickWithMessage", 500, false, "i", target);
	printf("[kick] Gracz %s (UID: %i) zostal wyrzucony z serwera - powod: System KICK",pInfo[target][pName],GetPlayerUID(target));
	
	format(string,sizeof(string),"INSERT INTO penalties VALUES(NULL,'%s','%s',%i,0,'%s','%s',1)",getStringDate(),getStringTime(),GetPlayerUID(target),reason,"Kick");
	mysql_query(string);
}

stock system_aj(player,time,reason[])
{
	new buffer[256];
	
	SetPlayerVirtualWorld(player,player+9999);
	SetPlayerPos(player,AJ_X,AJ_Y,AJ_Z);
	SetPlayerSpecialAction(player,SPECIAL_ACTION_CUFFED);
	format(buffer,sizeof(buffer),"UPDATE core_players SET char_ajtime=%i WHERE char_uid=%i",time,GetPlayerUID(player));
	mysql_query(buffer);
	new pname[MAX_PLAYER_NAME];
	GetPlayerName(playerid,pname,sizeof(pname));
	format(string, sizeof(string), "~r~~h~AdminJail~n~~w~Gracz: %s~n~Nadajacy: System~n~~y~~h~%s", pInfo[player][pName], reason);
	TextDrawSetString(Text:TextDrawPunishInfo, string);
	TextDrawShowForAll(Text:TextDrawPunishInfo);
	SetTimer("HidePunishText",6000,false);						
	AjTimer[player] = SetTimerEx("PlayerAj", 60000, false, "i", player);		
	format(buffer,sizeof(buffer),"Zosta³eœ umieszczony a AdminJail na %i minut.",time);
	SendClientMessage(player,COLOR_SERVER,buffer);
	pInfo[player][pAj] = time;
	
	format(buffer,sizeof(buffer),"INSERT INTO penalties VALUES(NULL,'%s','%s',%i,0,'%s','%s',0)",getStringDate(),getStringTime(),GetPlayerUID(target),reason,"AdminJail");
	mysql_query(buffer);
}

stock CheckBannedIP(playerid)
{
	new buffer[256];
	new ip[16];
	GetPlayerIp(playerid, ip, sizeof(ip));
	
	format(buffer,sizeof(buffer),"SELECT * FROM banned_ip WHERE ip='%s'",ip);
	mysql_query(buffer);
	mysql_store_result();
	if(mysql_num_rows() > 0)
	{
		mysql_free_result();
		printf("[kick] Gracz %s (UID: %i) zostal wyrzucony z serwera - powod: Adres IP zbanowany",pInfo[playerid][pName],GetPlayerUID(playerid));
		Kick(playerid);
		return 1;
	}
	else
	{
		mysql_free_result();
		PlayerCache[playerid][pIPChecked] = 1;
		return 0;
	}
}

stock WeaponHack(playerid)
{
	new weapon = GetPlayerWeapon(playerid);
	if(weapon != 0)
	{
		if(weapon == 22)
		{
			if(PlayerCache[playerid][pHasParalyzer] == 1)
			{
				return 0;
			}
		}
		if(weapon == 16)
		{
			if(IsPlayerHasFlashbang[playerid] == 1)
			{
				return 0;
			}
		}
		if(weapon == 46)
		{
			return 0;
		}
		if(weapon == 41 && PlayerCache[playerid][pHasLakier] == 1)
		{
			return 0;
		}
		else
		{
			new buffer[256];
			format(buffer,sizeof(buffer),"SELECT * FROM core_items WHERE item_value1=%i AND item_type=%i AND item_owneruid=%i",weapon,ITEM_WEAPON,GetPlayerUID(playerid));
			
			mysql_query(buffer);
			mysql_store_result();
			if(mysql_num_rows() <= 0)
			{
				new reason[64];
				GetWeaponName(weapon,reason,sizeof(reason));
				format(reason,sizeof(reason),"WeaponHack : %s",reason);
				system_ban(playerid,reason);		
				GuiInfo(playerid,""COL_GRAY2"Wykryto WeaponHack! Zostaniesz za to "COL_USE"zbanowany.");
				return 1;
			}
			mysql_free_result();
		}
	}
	return 0;
}

stock GetVehicle(playerid)
{
	if(IsPlayerInAnyVehicle(playerid))
	{
		new seat = GetPlayerVehicleSeat(playerid);
		if(seat == 0)
		{
			if(pInfo[playerid][blockVehicles] > 0)
			{
				RemovePlayerFromVehicle(playerid);
				GuiInfo(playerid,"Posiadasz aktywn¹ blokadê prowadzenia pojazdów.");
			}
		}
	}
}

stock SpeedHack(playerid)
{
	if(IsPlayerInAnyVehicle(playerid))
	{
		new vehicleid= GetPlayerVehicleID(playerid);
		new seat = GetPlayerVehicleSeat(playerid);
		if(seat == 0)
		{
			if(PlayerCache[playerid][pCacheAccess] > PERMS_SUPPORT)
			{
			
			}
			else
			{
				for(new i=0;i< 212; i++)
				{
					if(VehicleData[i][vModel] == GetVehicleModel(vehicleid))
					{
						if(GetVehicleSpeed(vehicleid) > VehicleData[i][vMaxSpeed])
						{
							system_kick(playerid,"SpeedHack");
							break;
						}
					}
				}
			}
		}
	}
}

stock BikeSpeed(playerid)
{
	if(IsPlayerInAnyVehicle(playerid))
	{
		new vehicleid = GetPlayerVehicleID(playerid);
		new seat = GetPlayerVehicleSeat(playerid);
		new modelid = GetVehicleModel(vehicleid);
		if(seat == 0)
		{
			if(modelid == 481 || modelid == 509 || modelid == 510)
			{
				if(GetVehicleSpeed(vehicleid) > 50)
				{
					SetVehicleSpeed(vehicleid,40);
				}
			}
		}
	}
}

stock CheckPlayerStatus(playerid)
{
	if(statusPlayer[playerid] > 0)
	{
		new string[256],finalEffect[256];
		new ilestatus = 0;
		format(string,sizeof(string),"(");
		
		if(PlayerCache[playerid][pHidden] > 0)
		{
			statusPlayer[playerid] = 0;
			
			UpdateDynamic3DTextLabelText(Text3D:PlayerNick[playerid], COLOR_WHITE, " ");
			
			return 1;
		}
		
		if(IsPlayerDuty[playerid] > 0)
		{
			if(DutyGroupType[playerid] == TYPE_ORG || DutyGroupType[playerid] == TYPE_MAFIA) { } else 
			{
				format(string,sizeof(string),"%s duty",string);
				ilestatus++;
			}
		}

		if(pInfo[playerid][pSeconds] < 18000)
		{
			format(string,sizeof(string),"%s nowa postaæ",string);
			ilestatus++;
		}
		
		if(pInfo[playerid][maxCondition] > 70)
		{
			if(pInfo[playerid][pSex] == SEX_MALE)
			{
				format(string,sizeof(string),"%s wysportowany",string);
			}
			if(pInfo[playerid][pSex] == SEX_FEMALE)
			{
				format(string,sizeof(string),"%s wysportowana",string);
			}
			ilestatus++;
		}
		
		if(pInfo[playerid][pPower] > 65)
		{
			if(pInfo[playerid][pSex] == SEX_MALE)
			{
				format(string,sizeof(string),"%s muskularny",string);
			}
			if(pInfo[playerid][pSex] == SEX_FEMALE)
			{
				format(string,sizeof(string),"%s muskularna",string);
			}
			ilestatus++;
		}
		
		if(PlayerCache[playerid][pGloves] > 0)
		{
			format(string,sizeof(string),"%s rêkawiczki",string);
			ilestatus++;
		}
		
		if(pInfo[playerid][pBw] > 0)
		{
			if(pInfo[playerid][pSex] == SEX_MALE)
			{
				format(string,sizeof(string),"%s nieprzytomny",string);
			}
			if(pInfo[playerid][pSex] == SEX_FEMALE)
			{
				format(string,sizeof(string),"%s nieprzytomna",string);
			}
			
			ilestatus++;
		}
		
		if(PlayerCache[playerid][pAfk] == true)
		{
			format(string,sizeof(string),"%s AFK",string);
			ilestatus++;
		}
		
		if(AmfetaminaTimer[playerid] > 0 || MarihuanaTimer[playerid] > 0 || KokainaTimer[playerid] > 0)
		{
			if(pInfo[playerid][pSex] == SEX_MALE)
			{
				format(string,sizeof(string),"%s naæpany",string);
			}
			if(pInfo[playerid][pSex] == SEX_FEMALE)
			{
				format(string,sizeof(string),"%s naæpana",string);
			}
			
			ilestatus++;
		}
		
		if(DrunkedTimer[playerid] > 0)
		{
			if(pInfo[playerid][pSex] == SEX_MALE)
			{
				format(string,sizeof(string),"%s pijany",string);
			}
			if(pInfo[playerid][pSex] == SEX_FEMALE)
			{	
				format(string,sizeof(string),"%s pijana",string);
			}
			
			ilestatus++;
		}
		
		if(Kamizelka[playerid] > 0)
		{
			format(string,sizeof(string),"%s kamizelka",string);
			ilestatus++;
		}		
		
		if(PlayerCache[playerid][pZP] > 0)
		{
			format(string,sizeof(string),"%s pasy",string);
			ilestatus++;
		}
		
		format(string,sizeof(string),"%s )",string);
		statusPlayer[playerid] = 0;
		
		if(pInfo[playerid][pAdmin] > 0)
		{
			if(CheckPlayerAccess(playerid) == PERMS_HELPER)
			{
				new globalNickname[128];
				format(globalNickname,sizeof(globalNickname),"%s",GetPlayerGlobalNickname(playerid));
				format(finalEffect,sizeof(finalEffect),"%s (%i)",globalNickname,playerid);
				UpdateDynamic3DTextLabelText(Text3D:PlayerNick[playerid], COLOR_ORANGE, finalEffect);
			}
			if(CheckPlayerAccess(playerid) == PERMS_SUPPORT)
			{
				new globalNickname[128];
				format(globalNickname,sizeof(globalNickname),"%s",GetPlayerGlobalNickname(playerid));
				format(finalEffect,sizeof(finalEffect),"%s (%i)",globalNickname,playerid);
				UpdateDynamic3DTextLabelText(Text3D:PlayerNick[playerid], COLOR_BLUE, finalEffect);
			}
			if(CheckPlayerAccess(playerid) == PERMS_GAMEMASTER)
			{
				new globalNickname[128];
				format(globalNickname,sizeof(globalNickname),"%s",GetPlayerGlobalNickname(playerid));
				format(finalEffect,sizeof(finalEffect),"%s (%i)",globalNickname,playerid);
				UpdateDynamic3DTextLabelText(Text3D:PlayerNick[playerid], COLOR_GM, finalEffect);
			}
			if(CheckPlayerAccess(playerid) == PERMS_ADMIN)
			{
				new globalNickname[128];
				format(globalNickname,sizeof(globalNickname),"%s",GetPlayerGlobalNickname(playerid));
				format(finalEffect,sizeof(finalEffect),"%s (%i)",globalNickname,playerid);
				UpdateDynamic3DTextLabelText(Text3D:PlayerNick[playerid], COLOR_RED, finalEffect);
			}
			if(CheckPlayerAccess(playerid) == PERMS_PROGRAMMER)
			{
				new globalNickname[128];
				format(globalNickname,sizeof(globalNickname),"%s",GetPlayerGlobalNickname(playerid));
				format(finalEffect,sizeof(finalEffect),"%s (%i)",globalNickname,playerid);
				UpdateDynamic3DTextLabelText(Text3D:PlayerNick[playerid], COLOR_PROGRAMMER, finalEffect);
			}
		}
		else if(pInfo[playerid][pMasked] > 0)
		{
			if(ilestatus <= 0)
			{
				format(finalEffect,sizeof(finalEffect),"Zamaskowany %i (%i)",PlayerCache[playerid][pMaskedNumbers],GetPlayerUID(playerid));
				UpdateDynamic3DTextLabelText(Text3D:PlayerNick[playerid], COLOR_WHITE, finalEffect);
			}
			else
			{
				format(finalEffect,sizeof(finalEffect),"Zamaskowany %i (%i)",PlayerCache[playerid][pMaskedNumbers],GetPlayerUID(playerid));
				format(finalEffect,sizeof(finalEffect),"%s\n%s",finalEffect,string);
				UpdateDynamic3DTextLabelText(Text3D:PlayerNick[playerid], COLOR_WHITE, finalEffect);
			}
		}
		else
		{	
			if(ilestatus <= 0)
			{
				new imie[32],nazwisko[32];
				sscanf(pInfo[playerid][pName],"p<_>s[32]s[32]",imie,nazwisko);
				format(finalEffect,sizeof(finalEffect),"%s %s (%i)",imie,nazwisko,playerid);
				
				if(PlayerCache[playerid][pHited] > 0)
				{
					UpdateDynamic3DTextLabelText(Text3D:PlayerNick[playerid], COLOR_RED, finalEffect);
					//timer
				}
				else
				{
					UpdateDynamic3DTextLabelText(Text3D:PlayerNick[playerid], COLOR_WHITE, finalEffect);
				}
			}
			else
			{
				new imie[32],nazwisko[32];
				sscanf(pInfo[playerid][pName],"p<_>s[32]s[32]",imie,nazwisko);
				format(finalEffect,sizeof(finalEffect),"%s %s (%i)",imie,nazwisko,playerid);
				format(finalEffect,sizeof(finalEffect),"%s\n%s",finalEffect,string);
				
				if(PlayerCache[playerid][pHited] > 0)
				{
					UpdateDynamic3DTextLabelText(Text3D:PlayerNick[playerid], COLOR_RED, finalEffect);
					//timer
				}
				else
				{
					UpdateDynamic3DTextLabelText(Text3D:PlayerNick[playerid], COLOR_WHITE, finalEffect);
				}
			}
		}
	}
	return 1;	// moze powodowac bledy
}

stock BankomatTimer(playerid)
{
	if(pInfo[playerid][pBankomat] > 0)
	{
		pInfo[playerid][pBankomat]--;
	}
}

forward ParalyzerWaitAC(playerid);
public ParalyzerWaitAC(playerid)
{
	PlayerCache[playerid][pHasParalyzer] = 0;
}

forward ParalyzerWaitFB(playerid);
public ParalyzerWaitFB(playerid)
{
	IsPlayerHasFlashbang[playerid] = 0;
}

forward Drunked(playerid);
public Drunked(playerid)
{
	if(DrunkedTimer[playerid] > 0)
	{
		DrunkedTimer[playerid]--;
		SetTimerEx("Drunked", 1000, false, "i", playerid);
	}
	else
	{
		statusPlayer[playerid] = 1;
	}
}

forward LakierWaitAC(playerid);
public LakierWaitAC(playerid)
{
	PlayerCache[playerid][pHasLakier] = 0;
}

public OnPlayerShootPlayer(shooter,target,Float:hpLoss,Float:armLoss)
{
	//snajperka znosi na jeden strza³
	if(GetPlayerWeapon(shooter) == 34)
	{
		pInfo[target][pHealth] = 0;
		SetPlayerHealth(target,0);
	}
	
	//ac weaponhack
	if(GetPlayerWeaponAmmo(shooter,GetPlayerWeapon(shooter)) > PlayerCache[shooter][pWeaponAmmo])
	{
		system_kick(shooter,"AmmoHack");
	}
	
    return 1;
}

forward SetPlayerNormalColorNick(playerid);
public SetPlayerNormalColorNick(playerid)
{
	PlayerCache[playerid][pHited] = 0;
	statusPlayer[playerid] = 1;
	return 1;
}

COMMAND:test1(playerid,params[])
{
	EnablePlayerFlashed(playerid);
	return 1;
}

COMMAND:test2(playerid,params[])
{
	DisablePlayerFlashed(playerid);
	return 1;
}

public OnPlayerTakeDamage(playerid, issuerid, Float: amount, weaponid)
{
	if(pInfo[playerid][pAdmin] > 0)
	{
		pInfo[playerid][pHealth] = 100;
		return 1;
	}
	
	/*if(weaponid == 16)
	{
		if(weaponid == 16)	//nie pytaj
		{
			//no hp nie zabieraj ale oslep
			EnablePlayerFlashed(playerid);
			SetPlayerDrunkLevel(playerid,5000);
			
			SetTimerEx("DisablePlayerFlashed", 45000, false, "i", playerid);
		}
	}*/
	
	if(issuerid != INVALID_PLAYER_ID)
	{
		//flashbang
		if(IsPlayerHasFlashbang[issuerid] == 1)
		{
			if(GetPlayerWeapon(issuerid) > 0)
			{
				EnablePlayerFlashed(playerid);
				SetPlayerDrunkLevel(playerid,5000);
				
				SetTimerEx("DisablePlayerFlashed", 45000, false, "i", playerid);
			}
		}
		
		if(pInfo[playerid][pBw] > 0)
		{
			SetPlayerHealth(playerid,pInfo[playerid][pHealth]);
			return 1;
		}
		
		if(pInfo[issuerid][pCuffed] > 0)
		{
			SetPlayerHealth(playerid,pInfo[playerid][pHealth]);
			return 1;
		}
		
		if(PlayerWeapon[issuerid] > 0)
		{
			if(PlayerWeapon[issuerid] > 10 && PlayerWeapon[playerid] < 40 && GetPlayerWeapon(issuerid) > 0)
			{
				if(GetPlayerWeapon(issuerid) == 34)
				{
					//snajpa
					amount = amount+100;
				}
				
				if(GetPlayerWeapon(issuerid) == 30 || GetPlayerWeapon(issuerid) == 31)
				{
					//snajpa
					amount = amount+40;
				}
				
				if(GetPlayerWeapon(issuerid) == 23)
				{
					//fiveseven
					amount = amount+25;
				}
				
				if(GetPlayerWeapon(issuerid) == 27)
				{
					//spas
					amount = amount+30;
				}
				
				ShootedTime[playerid] = 8;
				SetTimerEx("ApplyPlayerShooted", 1000, false, "i", playerid);
				TogglePlayerControllable(playerid,0);
				ApplyAnimation(playerid, "SWEET", "Sweet_injuredloop", 4, 0, 0, 1, 1, 0, 0);
			}
		}
		
		if(PlayerCache[issuerid][pHasParalyzer] > 0)
		{
			SetPlayerHealth(playerid,pInfo[playerid][pHealth]);

			if(PlayerWeapon[issuerid] > 0)
			{
				//sprawdzenie czy to paralyzer
				if(IsWeaponParalyzer(PlayerWeapon[issuerid]) == 1 )
				{
					ParalyzedTime[playerid] = 15;
					SetTimerEx("ApplyPlayerParalyzed", 1000, false, "i", playerid);
					TogglePlayerControllable(playerid,0);
					ApplyAnimation(playerid, "CRACK", "crckidle2", 4, 0, 0, 1, 1, 0, 0);
				}
			}	
			return 1;
		}
		
		if(PlayerCache[issuerid][pHasSpecialWeapon] == 1)
		{
			
		}
		
		if(PlayerWeapon[issuerid] <= 0)
		{
			new Float:x,Float:y,Float:z;
			GetPlayerPos(issuerid,x,y,z);
			if(IsPlayerInRangeOfPoint(playerid,5,x,y,z))			//przez lagi trzeba zwiêkszyæ
			{
				//tutaj uderzy³ i trzeba to ogarn¹æ
				new Float:bonus2 = 0;
				if(PlayerCache[playerid][pBonusPower] > 0)
				{
					bonus2 = PlayerCache[playerid][pBonusPower];
				}
				
				new Float:bonus = pInfo[issuerid][pPower] *0.1;
				pInfo[playerid][pHealth] = pInfo[playerid][pHealth] - (amount + bonus + bonus2);
				PlayerCache[playerid][pHited] = 1;
				statusPlayer[playerid] = 1;
				
				SetTimerEx("SetPlayerNormalColorNick", 1000, false, "i", playerid);
				
				return 1;
			}
			else
			{
				system_kick(issuerid,"Cheat");
				SetPlayerHealth(playerid,pInfo[playerid][pHealth]);
				return 1;
			}
		}
	}
	new buffer[256];
	new Float:hp;
	GetPlayerHealth(playerid,hp);
	hp = hp - amount;
	format(buffer,sizeof(buffer),"UPDATE core_players SET char_health=%f WHERE char_uid=%i",hp,GetPlayerUID(playerid));
	mysql_query(buffer);
	
	PlayerCache[playerid][pHited] = 1;
	statusPlayer[playerid] = 1;
	
	SetTimerEx("SetPlayerNormalColorNick", 1000, false, "i", playerid);
	
	pInfo[playerid][pHealth] = hp;
	return 1;
}

public OnPlayerWeaponShot(playerid, weaponid, hittype, hitid, Float:fX, Float:fY, Float:fZ)
{
	//	return 0 = nie zadaje obra¿eñ
	// return 1 = zadaje obra¿enia
	
	if(hittype == BULLET_HIT_TYPE_VEHICLE)
	{
		new vehicleid = hitid;
		vehicle[vehicleid][vShooted] = 1;
		SetVehicleHealth(vehicleid,vehicle[vehicleid][vhp]);
		return 1;
	}
	
	return 1;
}

forward DisablePlayerFlashed(playerid);
public DisablePlayerFlashed(playerid)
{
	TextDrawHideForPlayer(playerid, Text:TextDrawFlashbang[playerid]);
	return 1;
}

stock EnablePlayerFlashed(playerid)
{
	TextDrawShowForPlayer(playerid, Text:TextDrawFlashbang[playerid]);
	return 1;
}