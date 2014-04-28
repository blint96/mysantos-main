stock SendPlayerRadioToNearPlayers(playerid,text[])
{
	new sending[256],pname[MAX_PLAYER_NAME],imie[24],nazwisko[24],Float:x,Float:y,Float:z;
	GetPlayerName(playerid,pname,sizeof(pname));
	text[0] = toupper(text[0]);
	GetPlayerPos(playerid,x,y,z);
	sscanf(pname,"p<_>s[24]s[24]",imie,nazwisko);
	
	format(sending,sizeof(sending),"%s %s mówi (przez radio)%s",imie,nazwisko,text);
	
	new sendervw = GetPlayerVirtualWorld(playerid);
	
	for(new i = 0; i < MAX_PLAYERS+1;i++)
	{
		if(IsPlayerInRangeOfPoint(i,1,x,y,z))
		{	
			new gainervw = GetPlayerVirtualWorld(i);
			if(sendervw == gainervw)
			{
				//SendClientMessage(i,COLOR_1m,sending);
				SendCustomPlayerMessage(i, COLOR_1m, sending);
			}
		}
		else if(IsPlayerInRangeOfPoint(i,2,x,y,z))
		{	
			new gainervw = GetPlayerVirtualWorld(i);
			if(sendervw == gainervw)
			{
				//SendClientMessage(i,COLOR_2m,sending);
				SendCustomPlayerMessage(i, COLOR_2m, sending);
			}
		}
		else if(IsPlayerInRangeOfPoint(i,4,x,y,z))
		{	
			new gainervw = GetPlayerVirtualWorld(i);
			if(sendervw == gainervw)
			{
				//SendClientMessage(i,COLOR_4m,sending);
				SendCustomPlayerMessage(i, COLOR_4m, sending);
			}
		}
		else if(IsPlayerInRangeOfPoint(i,6,x,y,z))
		{	
			new gainervw = GetPlayerVirtualWorld(i);
			if(sendervw == gainervw)
			{
				//SendClientMessage(i,COLOR_6m,sending);
				SendCustomPlayerMessage(i, COLOR_6m, sending);
			}
		}
		else if(IsPlayerInRangeOfPoint(i,8,x,y,z))
		{	
			new gainervw = GetPlayerVirtualWorld(i);
			if(sendervw == gainervw)
			{
				//SendClientMessage(i,COLOR_8m,sending);
				SendCustomPlayerMessage(i, COLOR_8m, sending);
			}
		}
		else if(IsPlayerInRangeOfPoint(i,9,x,y,z))
		{	
			new gainervw = GetPlayerVirtualWorld(i);
			if(sendervw == gainervw)
			{
				//SendClientMessage(i,COLOR_9m,sending);
				SendCustomPlayerMessage(i, COLOR_9m, sending);
			}
		}
		//TalkAnimation(playerid);
	}
}

stock SendRadioTalking(playerid,text[])
{
	new sending[256],pname[MAX_PLAYER_NAME],imie[24],nazwisko[24],Float:x,Float:y,Float:z;
	GetPlayerName(playerid,pname,sizeof(pname));
	text[0] = toupper(text[0]);
	GetPlayerPos(playerid,x,y,z);
	sscanf(pname,"p<_>s[24]s[24]",imie,nazwisko);
	
	format(sending,sizeof(sending),"%s %s mówi (przez radio): %s",imie,nazwisko,text);
	
	new sendervw = GetPlayerVirtualWorld(playerid);
	
	for(new i = 0; i < MAX_PLAYERS+1;i++)
	{
		if(IsPlayerInRangeOfPoint(i,1,x,y,z))
		{	
			new gainervw = GetPlayerVirtualWorld(i);
			if(sendervw == gainervw)
			{
				//SendClientMessage(i,COLOR_1m,sending);
				SendCustomPlayerMessage(i, COLOR_1m, sending);
			}
		}
		else if(IsPlayerInRangeOfPoint(i,2,x,y,z))
		{	
			new gainervw = GetPlayerVirtualWorld(i);
			if(sendervw == gainervw)
			{
				//SendClientMessage(i,COLOR_2m,sending);
				SendCustomPlayerMessage(i, COLOR_2m, sending);
			}
		}
		else if(IsPlayerInRangeOfPoint(i,4,x,y,z))
		{	
			new gainervw = GetPlayerVirtualWorld(i);
			if(sendervw == gainervw)
			{
				//SendClientMessage(i,COLOR_4m,sending);
				SendCustomPlayerMessage(i, COLOR_4m, sending);
			}
		}
		else if(IsPlayerInRangeOfPoint(i,6,x,y,z))
		{	
			new gainervw = GetPlayerVirtualWorld(i);
			if(sendervw == gainervw)
			{
				//SendClientMessage(i,COLOR_6m,sending);
				SendCustomPlayerMessage(i, COLOR_6m, sending);
			}
		}
		else if(IsPlayerInRangeOfPoint(i,8,x,y,z))
		{	
			new gainervw = GetPlayerVirtualWorld(i);
			if(sendervw == gainervw)
			{
				//SendClientMessage(i,COLOR_8m,sending);
				SendCustomPlayerMessage(i, COLOR_8m, sending);
			}
		}
		else if(IsPlayerInRangeOfPoint(i,9,x,y,z))
		{	
			new gainervw = GetPlayerVirtualWorld(i);
			if(sendervw == gainervw)
			{
				//SendClientMessage(i,COLOR_9m,sending);
				SendCustomPlayerMessage(i, COLOR_9m, sending);
			}
		}
		//TalkAnimation(playerid);
	}
}

stock SendPlayerPhoneToNearPlayers(playerid,text[])
{
	new sending[256],pname[MAX_PLAYER_NAME],imie[24],nazwisko[24],Float:x,Float:y,Float:z;
	GetPlayerName(playerid,pname,sizeof(pname));
	text[0] = toupper(text[0]);
	GetPlayerPos(playerid,x,y,z);
	sscanf(pname,"p<_>s[24]s[24]",imie,nazwisko);
	
	format(sending,sizeof(sending),"%s %s mówi (przez telefon)%s",imie,nazwisko,text);
	
	new sendervw = GetPlayerVirtualWorld(playerid);
	
	for(new i = 0; i < MAX_PLAYERS+1;i++)
	{
		if(IsPlayerInRangeOfPoint(i,1,x,y,z))
		{	
			new gainervw = GetPlayerVirtualWorld(i);
			if(sendervw == gainervw)
			{
				//SendClientMessage(i,COLOR_1m,sending);
				SendCustomPlayerMessage(i, COLOR_1m, sending);
			}
		}
		else if(IsPlayerInRangeOfPoint(i,2,x,y,z))
		{	
			new gainervw = GetPlayerVirtualWorld(i);
			if(sendervw == gainervw)
			{
				//SendClientMessage(i,COLOR_2m,sending);
				SendCustomPlayerMessage(i, COLOR_2m, sending);
			}
		}
		else if(IsPlayerInRangeOfPoint(i,4,x,y,z))
		{	
			new gainervw = GetPlayerVirtualWorld(i);
			if(sendervw == gainervw)
			{
				//SendClientMessage(i,COLOR_4m,sending);
				SendCustomPlayerMessage(i, COLOR_4m, sending);
			}
		}
		else if(IsPlayerInRangeOfPoint(i,6,x,y,z))
		{	
			new gainervw = GetPlayerVirtualWorld(i);
			if(sendervw == gainervw)
			{
				//SendClientMessage(i,COLOR_6m,sending);
				SendCustomPlayerMessage(i, COLOR_6m, sending);
			}
		}
		else if(IsPlayerInRangeOfPoint(i,8,x,y,z))
		{	
			new gainervw = GetPlayerVirtualWorld(i);
			if(sendervw == gainervw)
			{
				//SendClientMessage(i,COLOR_8m,sending);
				SendCustomPlayerMessage(i, COLOR_8m, sending);
			}
		}
		else if(IsPlayerInRangeOfPoint(i,9,x,y,z))
		{	
			new gainervw = GetPlayerVirtualWorld(i);
			if(sendervw == gainervw)
			{
				//SendClientMessage(i,COLOR_9m,sending);
				SendCustomPlayerMessage(i, COLOR_9m, sending);
			}
		}
		//TalkAnimation(playerid);
	}
}

stock RandPhoneNumber()
{
	losowanie:
	new nr1 = random(9);
	new nr2 = random(9);
	new nr3 = random(9);
	new nr4 = random(9);
	new nr5 = random(9);
	new nr6 = random(9);
	new numer[12];
	format(numer,sizeof(numer),"%i%i%i%i%i%i",nr1,nr2,nr3,nr4,nr5,nr6);
	
	new numer_int = strval(numer);
	
	if(numer_int <= 0)
	{
		goto losowanie;
	}
	
	new buffer[256];
	format(buffer,sizeof(buffer),"SELECT item_uid FROM core_items WHERE item_value1 = %i AND item_type=%i",numer_int,ITEM_PHONE);
	mysql_query(buffer);
	mysql_store_result();
	if(mysql_num_rows() > 0)
	{
		mysql_free_result();
		goto losowanie;
	}
	else
	{
		return numer_int;
	}	
	return numer_int;
}

stock TakePlayerMoney(playerid,amount)
{
	new buffer[256];
	format(buffer,sizeof(buffer),"UPDATE core_players SET char_money=char_money-%i WHERE char_uid=%i",amount,GetPlayerUID(playerid));
	mysql_query(buffer);
	pInfo[playerid][pMoney] = pInfo[playerid][pMoney] - amount;
}

stock AddPlayerMoney(playerid,amount)
{
	new buffer[256];
	format(buffer,sizeof(buffer),"UPDATE core_players SET char_money=char_money+%i WHERE char_uid=%i",amount,GetPlayerUID(playerid));
	mysql_query(buffer);
	pInfo[playerid][pMoney] = pInfo[playerid][pMoney] + amount;
}

#define D_TYPE_Q 1
#define D_TYPE_QS	2
#define D_TYPE_TIMEOUT	3

stock SendDisconnectMsg(playerid,type)
{
	new Float:x,Float:y,Float:z;
	GetPlayerPos(playerid,x,y,z);
	
	for(new i = 0 ; i < MAX_PLAYERS; i++)
	{
		if(GetPlayerVirtualWorld(playerid) == GetPlayerVirtualWorld(i))
		{
			if(IsPlayerInRangeOfPoint(i,10,x,y,z))
			{
				new sending[256];
				switch(type)
				{
					case 1:
					{
						format(sending,sizeof(sending),"(( %s - /q ))",pInfo[playerid][pName]);
					}
					case 2:
					{
						format(sending,sizeof(sending),"(( %s - /qs lub kick ))",pInfo[playerid][pName]);
					}
					case 3:
					{
						format(sending,sizeof(sending),"(( %s - timeout ))",pInfo[playerid][pName]);
					}
				}
				
				SendClientMessage(i,COLOR_GREY,sending);
			}
		}
	}
}

