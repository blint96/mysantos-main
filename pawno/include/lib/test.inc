forward SetVariables(playerid);

stock SetMoney(playerid)
{
	ResetPlayerMoney(playerid);
	GivePlayerMoney(playerid,pInfo[playerid][pMoney]);
}

stock SetHp(playerid)
{
	SetPlayerHealth(playerid,pInfo[playerid][pHealth]);
}

public SetVariables(playerid)
{
	if(IsPlayerConnected(playerid))
	{
		SetMoney(playerid);		
		new buffer[256];
		format(buffer,sizeof(buffer),"UPDATE core_players SET char_time=char_time+1 WHERE char_uid=%i",GetPlayerUID(playerid));
		mysql_query(buffer);	
		GetVehicle(playerid);		
		SetHp(playerid);
		WeaponHack(playerid);
		SetTimerEx("SetVariables", 1000, false, "i", playerid);
	}	
	return 1;
}

stock WeaponHack(playerid)
{
	new weapon = GetPlayerWeapon(playerid);
	if(weapon != 0)
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
			GuiInfo(playerid,""COL_GRAY2"Wykryto WeaponHack! Zostaniesz za to "COL_USE"zbanowany.");
			format(buffer,sizeof(buffer),"UPDATE core_players SET block_ban=1 , ban_reason='%s' WHERE char_uid=%i",reason,GetPlayerUID(playerid));
			mysql_query(buffer);
			SetTimerEx("KickWithMessage",2000,false,"i",playerid);
		}
		mysql_free_result();
	}
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
