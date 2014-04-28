//
//
//
//			ITEMS VLIVE 1

#define WHEREIS_ZIEMIA 1
#define WHEREIS_GRACZ 0
#define WHEREIS_SCHOWEK 3
#define WHEREIS_AUTO 4

#define DEFAULT_MODELID 328
#define COLT_MODELID	

#define ATTACH_CZAPKA 2
#define ATTACH_GLASSES 2
#define ATTACH_KREGOSLUP 1
#define ATTACH_REKA 13

new ParalyzedTime[MAX_PLAYERS];
new ShootedTime[MAX_PLAYERS];

#define MAX_ATTACHMENTS	36

enum sAccessData
{
	aModel,
	aPrice,
	aName[24],
	
	aBone,
	
	Float:aPosX,
	Float:aPosY,
	Float:aPosZ,
	
	Float:aRotX,
	Float:aRotY,
	Float:aRotZ,
	
	Float:aScaleX,
	Float:aScaleY,
	Float:aScaleZ
}
new AttachItems[36][sAccessData] = 
{
	{371, 120, "Plecak 1",  ATTACH_KREGOSLUP, 0.465664, 0.048355, 0.004090, 87.055976, 70.951591, 0.000000, 1.0, 1.0, 1.0},
	{18645, 310, "Kask typ 1",  ATTACH_CZAPKA, 0.465664, 0.048355, 0.004090, 87.055976, 70.951591, 0.000000, 1.0, 1.0, 1.0},
	{18976, 310, "Kask typ 2",  ATTACH_CZAPKA, 0.465664, 0.048355, 0.004090, 87.055976, 70.951591, 0.000000, 1.0, 1.0, 1.0},
	{18977, 325, "Kask typ 3",  ATTACH_CZAPKA, 0.465664, 0.048355, 0.004090, 87.055976, 70.951591, 0.000000, 1.0, 1.0, 1.0},
	{18978, 325, "Kask typ 4",  ATTACH_CZAPKA, 0.465664, 0.048355, 0.004090, 87.055976, 70.951591, 0.000000, 1.0, 1.0, 1.0},
	{18979, 325, "Kask typ 5",  ATTACH_CZAPKA, 0.465664, 0.048355, 0.004090, 87.055976, 70.951591, 0.000000, 1.0, 1.0, 1.0},
	{19006, 110, "Okulary typ 1",  ATTACH_GLASSES, 0.498841, 0.079501, -0.004798, 0.476614, 89.116958, 86.275207, 1.0, 1.0, 1.0},
	{19007, 110, "Okulary typ 2",  ATTACH_GLASSES, 0.498841, 0.079501, -0.004798, 0.476614, 89.116958, 86.275207, 1.0, 1.0, 1.0},
	{19008, 110, "Okulary typ 3",  ATTACH_GLASSES, 0.498841, 0.079501, -0.004798, 0.476614, 89.116958, 86.275207, 1.0, 1.0, 1.0},
	{19009, 110, "Okulary typ 4",  ATTACH_GLASSES, 0.498841, 0.079501, -0.004798, 0.476614, 89.116958, 86.275207, 1.0, 1.0, 1.0},
	{19010, 110, "Okulary typ 5",  ATTACH_GLASSES, 0.498841, 0.079501, -0.004798, 0.476614, 89.116958, 86.275207, 1.0, 1.0, 1.0},
	{19011, 110, "Okulary typ 6",  ATTACH_GLASSES, 0.498841, 0.079501, -0.004798, 0.476614, 89.116958, 86.275207, 1.0, 1.0, 1.0},
	{19012, 110, "Okulary typ 7",  ATTACH_GLASSES, 0.498841, 0.079501, -0.004798, 0.476614, 89.116958, 86.275207, 1.0, 1.0, 1.0},
	{19013, 110, "Okulary typ 8",  ATTACH_GLASSES, 0.498841, 0.079501, -0.004798, 0.476614, 89.116958, 86.275207, 1.0, 1.0, 1.0},
	{19014, 110, "Okulary typ 9",  ATTACH_GLASSES, 0.498841, 0.079501, -0.004798, 0.476614, 89.116958, 86.275207, 1.0, 1.0, 1.0},
	{19015, 110, "Okulary typ 10",  ATTACH_GLASSES, 0.498841, 0.079501, -0.004798, 0.476614, 89.116958, 86.275207, 1.0, 1.0, 1.0},
	{19016, 110, "Okulary typ 11",  ATTACH_GLASSES, 0.498841, 0.079501, -0.004798, 0.476614, 89.116958, 86.275207, 1.0, 1.0, 1.0},
	{19017, 110, "Okulary typ 12",  ATTACH_GLASSES, 0.498841, 0.079501, -0.004798, 0.476614, 89.116958, 86.275207, 1.0, 1.0, 1.0},
	{19018, 110, "Okulary typ 13",  ATTACH_GLASSES, 0.498841, 0.079501, -0.004798, 0.476614, 89.116958, 86.275207, 1.0, 1.0, 1.0},
	{19019, 110, "Okulary typ 14",  ATTACH_GLASSES, 0.498841, 0.079501, -0.004798, 0.476614, 89.116958, 86.275207, 1.0, 1.0, 1.0},
	{19020, 110, "Okulary typ 15",  ATTACH_GLASSES, 0.498841, 0.079501, -0.004798, 0.476614, 89.116958, 86.275207, 1.0, 1.0, 1.0},
	{19021, 110, "Okulary typ 16",  ATTACH_GLASSES, 0.498841, 0.079501, -0.004798, 0.476614, 89.116958, 86.275207, 1.0, 1.0, 1.0},
	{19022, 110, "Okulary typ 17",  ATTACH_GLASSES, 0.498841, 0.079501, -0.004798, 0.476614, 89.116958, 86.275207, 1.0, 1.0, 1.0},
	{19023, 110, "Okulary typ 18",  ATTACH_GLASSES, 0.498841, 0.079501, -0.004798, 0.476614, 89.116958, 86.275207, 1.0, 1.0, 1.0},
	{19024, 110, "Okulary typ 19",  ATTACH_GLASSES, 0.498841, 0.079501, -0.004798, 0.476614, 89.116958, 86.275207, 1.0, 1.0, 1.0},
	{19025, 110, "Okulary typ 20",  ATTACH_GLASSES, 0.498841, 0.079501, -0.004798, 0.476614, 89.116958, 86.275207, 1.0, 1.0, 1.0},
	{19026, 110, "Okulary typ 21",  ATTACH_GLASSES, 0.498841, 0.079501, -0.004798, 0.476614, 89.116958, 86.275207, 1.0, 1.0, 1.0},
	{19027, 110, "Okulary typ 22",  ATTACH_GLASSES, 0.498841, 0.079501, -0.004798, 0.476614, 89.116958, 86.275207, 1.0, 1.0, 1.0},
	{19028, 110, "Okulary typ 23",  ATTACH_GLASSES, 0.498841, 0.079501, -0.004798, 0.476614, 89.116958, 86.275207, 1.0, 1.0, 1.0},
	{19029, 110, "Okulary typ 24",  ATTACH_GLASSES, 0.498841, 0.079501, -0.004798, 0.476614, 89.116958, 86.275207, 1.0, 1.0, 1.0},
	{19030, 110, "Okulary typ 25",  ATTACH_GLASSES, 0.498841, 0.079501, -0.004798, 0.476614, 89.116958, 86.275207, 1.0, 1.0, 1.0},
	{19031, 110, "Okulary typ 26",  ATTACH_GLASSES, 0.498841, 0.079501, -0.004798, 0.476614, 89.116958, 86.275207, 1.0, 1.0, 1.0},
	{19032, 110, "Okulary typ 27",  ATTACH_GLASSES, 0.498841, 0.079501, -0.004798, 0.476614, 89.116958, 86.275207, 1.0, 1.0, 1.0},
	{19033, 110, "Okulary typ 28",  ATTACH_GLASSES, 0.498841, 0.079501, -0.004798, 0.476614, 89.116958, 86.275207, 1.0, 1.0, 1.0},
	{19034, 110, "Okulary typ 29",  ATTACH_GLASSES, 0.498841, 0.079501, -0.004798, 0.476614, 89.116958, 86.275207, 1.0, 1.0, 1.0},
	{19035, 110, "Okulary typ 30",  ATTACH_GLASSES, 0.498841, 0.079501, -0.004798, 0.476614, 89.116958, 86.275207, 1.0, 1.0, 1.0}
};

public GetDroppedItem()
{
	new query[256];
	new num = 0;
	format(query,sizeof(query),"SELECT item_uid,item_objectid,item_posx,item_posy,item_posz FROM core_items WHERE item_whereis=1");
	mysql_query(query);
	mysql_store_result();
	new resline[256],uid,objectid,Float:x,Float:y,Float:z;
	while(mysql_fetch_row(resline,"|"))
	{
		num++;
		sscanf(resline,"p<|>iifff",uid,objectid,x,y,z);
	}
	mysql_free_result();
	printf("\n[System przedmiotów] Za³adowano wyrzuconych przedmiotów : %i\n",num);
	return 1;
}

stock DropItemToVehicle(playerid,itemuid)
{
	new vehicleid = GetPlayerVehicleID(playerid);
	new buffer[256];
	format(buffer,sizeof(buffer),"UPDATE core_items SET item_whereis = %i, item_owneruid= %i WHERE item_uid=%i",WHEREIS_AUTO,vehicle[vehicleid][vuid],itemuid);
	mysql_query(buffer);
	return 1;
}

stock ListVehicleItems(playerid,vehicleid)
{
	new buffer[256];
	format(buffer,sizeof(buffer),"SELECT item_uid,item_name FROM core_items WHERE item_whereis=%i AND item_owneruid=%i",WHEREIS_AUTO,vehicle[vehicleid][vuid]);
	mysql_query(buffer);
	mysql_store_result();
	new lista[1024];
	
	if(mysql_num_rows() <= 0)
	{
		mysql_free_result();
		return 0;
	}
	
	format(lista,sizeof(lista),"UID\t\tNazwa przedmiotu\n###\t\t###");
	
	while(mysql_fetch_row(buffer,"|"))
	{
		new uid,name[64];
		sscanf(buffer,"p<|>is[64]",uid,name);
		format(lista,sizeof(lista),"%s\n%i\t\t%s",lista,uid,name);
	}
	
	ShowPlayerDialog(playerid,DIAL_LIST_VEH_ITEMS,DIALOG_STYLE_LIST,"Przedmioty w schowku",lista,"Wybierz","Zamknij");
	
	mysql_free_result();
	return 1;
}

stock CountPlayerItems(playerid)
{
	new buffer[256];
	format(buffer,sizeof(buffer),"SELECT * FROM core_items WHERE item_owneruid=%i AND item_whereis=%i",GetPlayerUID(playerid),WHEREIS_GRACZ);
	mysql_query(buffer);
	mysql_store_result();
	new ile = mysql_num_rows();
	mysql_free_result();
	return ile;
}

forward IsPhoneOwnerOnline(number,playerid);			//number - w³aœciciela // playerid- tego kto chce dzwonic
public IsPhoneOwnerOnline(number,playerid)
{
	new buffer[256];
	format(buffer,sizeof(buffer),"SELECT item_owneruid FROM core_items WHERE item_value1=%i AND item_type=%i",number,ITEM_PHONE);
	mysql_query(buffer);
	mysql_store_result();
	if(mysql_num_rows() > 0)
	{
		new uid = mysql_fetch_int();
		mysql_free_result();
		if(IsPlayerConnectedUID(uid) >= 0)
		{
			if(CzyRozmawia[IsPlayerConnectedUID(uid)] > 0)
			{
				//PlayerNarracja(playerid,"S³ychaæ sygna³ zajêtoœci...");
				NarrateToPlayer(playerid,"S³ychaæ sygna³ zajêtoœci...");
				DeletePhoneAnim(playerid);
				CzyRozmawia[playerid] = 0;
				ToWho[playerid] = -1;
			}
			else
			{
				//jest pod³¹czony, numer podany dzia³a, mo¿na wykonaæ telefon
				ApplyPhoneAnim(playerid);
				SendClientMessage(playerid,COLOR_YELLOW,"Wykonujesz po³¹czenie, czekaj a¿ rozmówca odbierze...");
				new mg[256];
				format(mg,sizeof(mg),"Ktoœ z numerem %i próbuje siê z tob¹ skontaktowaæ, wpisz /odbierz jeœli chcesz z nim porozmawiaæ.",pInfo[playerid][pPhoneInUse]);
				SendClientMessage(IsPlayerConnectedUID(uid),COLOR_YELLOW,mg);
				ToWho[playerid] = IsPlayerConnectedUID(uid);
				CzyRozmawia[playerid] = 1;
				//SetTimerEx("TalkTimer", 1000, false, "i", playerid,IsPlayerConnectedUID(uid));
			}
		}
		if(IsPlayerConnectedUID(uid) == -1)
		{
			//PlayerNarracja(playerid,"Telefon o podanym numerze jest poza zasiêgiem sieci...");	
			NarrateToPlayer(playerid,"Telefon o podanym numerze jest poza zasiêgiem sieci...");
		}
	}
	else
	{
		//PlayerNarracja(playerid,"Telefon o podanym numerze nie jest zarejestrowany w sieci...");	
		NarrateToPlayer(playerid,"Telefon o podanym numerze nie jest zarejestrowany w sieci...");
		return 0;
	}
	return 1;
}

forward TalkTimer(playerid,rozmowcaid);
public TalkTimer(playerid,rozmowcaid)
{
	if(CzyRozmawia[playerid] == 1)
	{
		if(CzyRozmawia[ToWho[playerid]])
		{
			//SetTimerEx("TalkTimer", 1000, false, "i", playerid,ToWho[playerid]);
		}
		else
		{
			SendClientMessage(ToWho[playerid],COLOR_YELLOW,"Osoba z któr¹ rozmawia³eœ roz³¹czy³a siê.");
			SendClientMessage(playerid,COLOR_YELLOW,"Roz³¹czy³eœ siê.");
			DeletePhoneAnim(playerid);
			DeletePhoneAnim(ToWho[playerid] );
			ToWho[playerid] = -1;
			ToWho[ToWho[playerid]] = -1;
			CzyRozmawia[ToWho[playerid]] = 0;
			CzyRozmawia[playerid] = 0;
		}
	}
	else
	{
		SendClientMessage(ToWho[playerid],COLOR_YELLOW,"Osoba z któr¹ rozmawia³eœ roz³¹czy³a siê.");
		SendClientMessage(playerid,COLOR_YELLOW,"Roz³¹czy³eœ siê.");
		DeletePhoneAnim(playerid);
		ToWho[playerid] = -1;
		ToWho[ToWho[playerid]] = -1;
		CzyRozmawia[ToWho[playerid]] = 0;
		CzyRozmawia[playerid] = 0;
	}
}

public PickupNearbyItems(playerid)
{
	new query[256],Float:x,Float:y,Float:z,listitem[512],worldid;
	worldid = GetPlayerVirtualWorld(playerid);
	format(query,sizeof(query),"SELECT item_uid,item_name,item_posx,item_posy,item_posz FROM core_items WHERE item_whereis=1 AND item_owneruid=0 AND item_worldid=%i",worldid);
	mysql_query(query);
	mysql_store_result();
	new resline[256];
	new itemuid,itemname[32];
	new num = 0;
	while(mysql_fetch_row(resline,"|"))
	{
		sscanf(resline,"p<|>is[32]fff",itemuid,itemname,x,y,z);
		if(IsPlayerInRangeOfPoint(playerid,5,x,y,z))
		{
			format(listitem,sizeof(listitem),"%s\n%i\t%s",listitem,itemuid,itemname);
			PodnoszoneLista[playerid][num] = itemuid;
			num++;
		}
		//print(resline);
	}
	if(!listitem[0])
	{
		GuiInfo(playerid,"Brak przedmiotów w pobli¿u.");
	}
	else
	{
		ShowPlayerDialog(playerid,DIAL_PP,DIALOG_STYLE_LIST,"Przedmioty w pobli¿u",listitem,"Wybierz","Zamknij");
	}
	mysql_free_result();
	return 1;
}

stock PutItemIntoDoors(playerid,itemid)
{
	new buffer[256];
	new doorid = GetPlayerDoorID(playerid);
	new dooruid = DoorInfo[doorid][doorUID];
	format(buffer,sizeof(buffer),"SELECT * FROM core_items WHERE item_whereis=%i AND item_owneruid=%i",WHEREIS_SCHOWEK,dooruid);
	mysql_query(buffer);
	mysql_store_result();
	new ile = mysql_fetch_int();
	mysql_free_result();
	
	if(ile > 100)
	{
		GuiInfo(playerid,"Schowek jest przepe³niony, nie mo¿esz w³o¿yæ do niego wiêcej przedmiotów.");
		return 0;
	}
	if(!IsItemUsed(itemid))
	{
		format(buffer,sizeof(buffer),"UPDATE core_items SET item_whereis=%i,item_owneruid=%i,item_worldid=1  WHERE item_owneruid=%i AND item_uid=%i",WHEREIS_SCHOWEK,dooruid,GetPlayerUID(playerid),itemid);
		mysql_query(buffer);
		return 1;
	}
	else
	{
		GuiInfo(playerid,"Nie mo¿esz wyrzuæ u¿ywanego w³aœnie przedmiotu.");
		return 0;
	}
}

stock GetItemFromDoors(playerid,itemid)
{

}

stock ListDoorItems(playerid,doorid)
{
	new buffer[256],lista[1024];
	new dooruid = DoorInfo[doorid][doorUID];
	format(buffer,sizeof(buffer),"SELECT item_uid, item_value2, item_name,item_worldid FROM core_items WHERE item_whereis=%i AND item_owneruid=%i",WHEREIS_SCHOWEK,dooruid);
	mysql_query(buffer);
	mysql_store_result();
	while(mysql_fetch_row(buffer,"|"))
	{
		new uid,name[32],val2,ileich;
		sscanf(buffer,"p<|>iis[32]i",uid,val2,name,ileich);
		format(lista,sizeof(lista),"%s\nx%i\t%i\t\t%i\t\t%s",lista,ileich,uid,val2,name);
	}
	ShowPlayerDialog(playerid,DIAL_SCHOWEK,DIALOG_STYLE_LIST,"Przedmioty w schowku",lista,"Wybierz","Zamknij");
	mysql_free_result();
}

stock IsItemInDoors(itemuid,doorid)
{
	new buffer[256];
	new dooruid = DoorInfo[doorid][doorUID];
	
	format(buffer,sizeof(buffer),"SELECT * FROM core_items WHERE item_whereis=%i AND item_owneruid=%i AND item_uid=%i",WHEREIS_SCHOWEK,dooruid,itemuid);
	mysql_query(buffer);
	mysql_store_result();
	if(mysql_num_rows() <= 0)
	{
		mysql_free_result();
		return 0;
	}
	else
	{
		mysql_free_result();
		return 1;
	}
}

stock ShowMagazyn(playerid,doorid)
{
	new buffer[256],lista[1024];
	new dooruid = DoorInfo[doorid][doorUID];
	format(buffer,sizeof(buffer),"SELECT item_uid, item_value2, item_name,item_worldid FROM core_items WHERE item_whereis=%i AND item_owneruid=%i",WHEREIS_SCHOWEK,dooruid);
	mysql_query(buffer);
	mysql_store_result();
	while(mysql_fetch_row(buffer,"|"))
	{
		new uid,name[32],val2,ileich;
		sscanf(buffer,"p<|>iis[32]i",uid,val2,name,ileich);
		format(lista,sizeof(lista),"%s\nx%i\t%i\t\t%i\t\t%s",lista,ileich,uid,val2,name);
	}
	ShowPlayerDialog(playerid,404,DIALOG_STYLE_LIST,"Przedmioty w magazynie",lista,"OK","");
	mysql_free_result();
}

stock IsItemUsed(itemid)
{
	new check_one[256];
	format(check_one,sizeof(check_one),"SELECT item_used FROM core_items WHERE item_uid=%i",itemid);
	mysql_query(check_one);
	mysql_store_result();
	new used = mysql_fetch_int();
	mysql_free_result();
	return used;
}

stock DropPlayerItem(playerid,itemid)
{
	new uid = itemInfo[playerid][itemid][iUID];
	new check_one[256];
	format(check_one,sizeof(check_one),"SELECT item_used,item_uid FROM core_items WHERE item_uid=%i",uid);
	mysql_query(check_one);
	mysql_store_result();
	new resline[256],used,uajdi;
	while(mysql_fetch_row(resline,"|"))
	{
		sscanf(resline,"p<|>ii",used,uajdi);
	}
	mysql_free_result();
	
	if (used == 1)
	{
		GuiInfo(playerid,"Nie mo¿esz wyrzuciæ przedmiotu, którego w³aœnie u¿ywasz.");
	}
	else
	{
		//wyrzucenie przedmiotuggta
		new query[256],Float:x,Float:y,Float:z;
		GetPlayerPos(playerid,x,y,z);

		

		new worldid = GetPlayerVirtualWorld(playerid);
		new freeslot = CheckFreeObjectSlot();
		new interiorid = GetPlayerInterior(playerid);
		format(query,sizeof(query),"UPDATE core_items SET item_whereis=1,item_owneruid=0, item_posx=%f,item_posy=%f,item_posz=%f,item_objectid=%i,item_worldid=%i WHERE item_owneruid=%i AND item_uid=%i",x,y,z-1.1,freeslot,worldid,GetPlayerUID(playerid),uid);
		mysql_query(query);
		object[freeslot][object_world] = worldid;
		object[freeslot][oposx] = x;
		object[freeslot][oposy] = y;
		object[freeslot][oposz] = z;
		object[freeslot][object_whatis] = 1;
		object[freeslot][object_interior] = interiorid;
		if(itemInfo[playerid][itemid][iType] == ITEM_WEAPON)
		{
			switch(itemInfo[playerid][itemid][iValue1])
			{
				case 3:
				{
					object[freeslot][object_model] = 334;
					object[freeslot][object_sid] = CreateDynamicObject(object[freeslot][object_model],
					object[freeslot][oposx],
					object[freeslot][oposy],
					object[freeslot][oposz]-1.0,
					0,0,0,
					object[freeslot][object_world],
					object[freeslot][object_interior],
					-1,200);
					
					SetDynamicObjectRot(freeslot,90,0 ,0 );
				}
				case 4:
				{
					object[freeslot][object_model] = 335;
					object[freeslot][object_sid] = CreateDynamicObject(object[freeslot][object_model],
					object[freeslot][oposx],
					object[freeslot][oposy],
					object[freeslot][oposz]-1.0,
					0,0,0,
					object[freeslot][object_world],
					object[freeslot][object_interior],
					-1,200);
					
					SetDynamicObjectRot(freeslot,90,0 ,0 );
				}
				case 5:
				{
					object[freeslot][object_model] = 336;
					object[freeslot][object_sid] = CreateDynamicObject(object[freeslot][object_model],
					object[freeslot][oposx],
					object[freeslot][oposy],
					object[freeslot][oposz]-1.0,
					0,0,0,
					object[freeslot][object_world],
					object[freeslot][object_interior],
					-1,200);
					
					SetDynamicObjectRot(freeslot,90,0 ,0 );
				}
				case 9:
				{
					object[freeslot][object_model] = 341;
					object[freeslot][object_sid] = CreateDynamicObject(object[freeslot][object_model],
					object[freeslot][oposx],
					object[freeslot][oposy],
					object[freeslot][oposz]-1.0,
					0,0,0,
					object[freeslot][object_world],
					object[freeslot][object_interior],
					-1,200);
					
					SetDynamicObjectRot(freeslot,90,0 ,0 );
				}
				case 22:
				{
					object[freeslot][object_model] = 346;
					object[freeslot][object_sid] = CreateDynamicObject(object[freeslot][object_model],
					object[freeslot][oposx],
					object[freeslot][oposy],
					object[freeslot][oposz]-1.0,
					0,0,0,
					object[freeslot][object_world],
					object[freeslot][object_interior],
					-1,200);
					
					SetDynamicObjectRot(freeslot,90,0 ,0 );
				}
				case 23:
				{
					object[freeslot][object_model] = 347;
					object[freeslot][object_sid] = CreateDynamicObject(object[freeslot][object_model],
					object[freeslot][oposx],
					object[freeslot][oposy],
					object[freeslot][oposz]-1.0,
					0,0,0,
					object[freeslot][object_world],
					object[freeslot][object_interior],
					-1,200);
					
					SetDynamicObjectRot(freeslot,90,0 ,0 );
				}
				case 24:
				{
					object[freeslot][object_model] = 348;
					object[freeslot][object_sid] = CreateDynamicObject(object[freeslot][object_model],
					object[freeslot][oposx],
					object[freeslot][oposy],
					object[freeslot][oposz]-1.0,
					0,0,0,
					object[freeslot][object_world],
					object[freeslot][object_interior],
					-1,200);
					
					SetDynamicObjectRot(freeslot,90,0 ,0 );
				}
				case 25:
				{
					object[freeslot][object_model] = 349;
					object[freeslot][object_sid] = CreateDynamicObject(object[freeslot][object_model],
					object[freeslot][oposx],
					object[freeslot][oposy],
					object[freeslot][oposz]-1.0,
					0,0,0,
					object[freeslot][object_world],
					object[freeslot][object_interior],
					-1,200);
					
					SetDynamicObjectRot(freeslot,90,0 ,0 );
				}
				case 26:
				{
					object[freeslot][object_model] = 350;
					object[freeslot][object_sid] = CreateDynamicObject(object[freeslot][object_model],
					object[freeslot][oposx],
					object[freeslot][oposy],
					object[freeslot][oposz]-1.0,
					0,0,0,
					object[freeslot][object_world],
					object[freeslot][object_interior],
					-1,200);
					
					SetDynamicObjectRot(freeslot,90,0 ,0 );
				}
				case 27:
				{
					object[freeslot][object_model] = 351;
					object[freeslot][object_sid] = CreateDynamicObject(object[freeslot][object_model],
					object[freeslot][oposx],
					object[freeslot][oposy],
					object[freeslot][oposz]-1.0,
					0,0,0,
					object[freeslot][object_world],
					object[freeslot][object_interior],
					-1,200);
					
					SetDynamicObjectRot(freeslot,90,0 ,0 );
				}
				case 28:
				{
					object[freeslot][object_model] = 352;
					object[freeslot][object_sid] = CreateDynamicObject(object[freeslot][object_model],
					object[freeslot][oposx],
					object[freeslot][oposy],
					object[freeslot][oposz]-1.0,
					0,0,0,
					object[freeslot][object_world],
					object[freeslot][object_interior],
					-1,200);
					
					SetDynamicObjectRot(freeslot,90,0 ,0 );
				}
				case 29:
				{
					object[freeslot][object_model] = 353;
					object[freeslot][object_sid] = CreateDynamicObject(object[freeslot][object_model],
					object[freeslot][oposx],
					object[freeslot][oposy],
					object[freeslot][oposz]-1.0,
					0,0,0,
					object[freeslot][object_world],
					object[freeslot][object_interior],
					-1,200);
					
					SetDynamicObjectRot(freeslot,90,0 ,0 );
				}
				case 30:
				{
					object[freeslot][object_model] = 355;
					object[freeslot][object_sid] = CreateDynamicObject(object[freeslot][object_model],
					object[freeslot][oposx],
					object[freeslot][oposy],
					object[freeslot][oposz]-1.0,
					0,0,0,
					object[freeslot][object_world],
					object[freeslot][object_interior],
					-1,200);
					
					SetDynamicObjectRot(freeslot,90,0 ,0 );
				}
				case 31:
				{
					object[freeslot][object_model] = 356;
					object[freeslot][object_sid] = CreateDynamicObject(object[freeslot][object_model],
					object[freeslot][oposx],
					object[freeslot][oposy],
					object[freeslot][oposz]-1.0,
					0,0,0,
					object[freeslot][object_world],
					object[freeslot][object_interior],
					-1,200);
					
					SetDynamicObjectRot(freeslot,90,0 ,0 );
				}
				case 32:
				{
					object[freeslot][object_model] = 372;
					object[freeslot][object_sid] = CreateDynamicObject(object[freeslot][object_model],
					object[freeslot][oposx],
					object[freeslot][oposy],
					object[freeslot][oposz]-1.0,
					0,0,0,
					object[freeslot][object_world],
					object[freeslot][object_interior],
					-1,200);
					
					SetDynamicObjectRot(freeslot,90,0 ,0 );
				}
				case 33:
				{
					object[freeslot][object_model] = 357;
					object[freeslot][object_sid] = CreateDynamicObject(object[freeslot][object_model],
					object[freeslot][oposx],
					object[freeslot][oposy],
					object[freeslot][oposz]-1.0,
					0,0,0,
					object[freeslot][object_world],
					object[freeslot][object_interior],
					-1,200);
					
					SetDynamicObjectRot(freeslot,90,0 ,0 );
				}
				case 34:
				{
					object[freeslot][object_model] = 358;
					object[freeslot][object_sid] = CreateDynamicObject(object[freeslot][object_model],
					object[freeslot][oposx],
					object[freeslot][oposy],
					object[freeslot][oposz]-1.0,
					0,0,0,
					object[freeslot][object_world],
					object[freeslot][object_interior],
					-1,200);
					
					SetDynamicObjectRot(freeslot,90,0 ,0 );
				}
				case 41:
				{
					object[freeslot][object_model] = 365;
					object[freeslot][object_sid] = CreateDynamicObject(object[freeslot][object_model],
					object[freeslot][oposx],
					object[freeslot][oposy],
					object[freeslot][oposz]-1.0,
					0,0,0,
					object[freeslot][object_world],
					object[freeslot][object_interior],
					-1,200);
					
					SetDynamicObjectRot(freeslot,90,0 ,0 );
				}
				case 42:
				{
					object[freeslot][object_model] = 366;
					object[freeslot][object_sid] = CreateDynamicObject(object[freeslot][object_model],
					object[freeslot][oposx],
					object[freeslot][oposy],
					object[freeslot][oposz]-1.0,
					0,0,0,
					object[freeslot][object_world],
					object[freeslot][object_interior],
					-1,200);
					
					SetDynamicObjectRot(freeslot,90,0 ,0 );
				}
			}			
		}
		else if(itemInfo[playerid][itemid][iType] == ITEM_CLOTH)
		{
			object[freeslot][object_model] = 2843;
			object[freeslot][object_sid] = CreateDynamicObject(object[freeslot][object_model],
			object[freeslot][oposx],
			object[freeslot][oposy],
			object[freeslot][oposz]-1.0,
			0,0,0,
			object[freeslot][object_world],
			object[freeslot][object_interior],
			-1,200);
			
		}
		else if(itemInfo[playerid][itemid][iType] == ITEM_CORPSE)
		{
			object[freeslot][object_model] = 2060;
			object[freeslot][object_sid] = CreateDynamicObject(object[freeslot][object_model],
			object[freeslot][oposx],
			object[freeslot][oposy],
			object[freeslot][oposz]-1.0,
			0,0,0,
			object[freeslot][object_world],
			object[freeslot][object_interior],
			-1,200);
		}
		else
		{
			object[freeslot][object_model] = 328;
			object[freeslot][object_sid] = CreateDynamicObject(object[freeslot][object_model],
			object[freeslot][oposx],
			object[freeslot][oposy],
			object[freeslot][oposz]-1.0,
			0,0,0,
			object[freeslot][object_world],
			object[freeslot][object_interior],
			-1,200);
			
			SetDynamicObjectRot(freeslot,90,0 ,0 );
		}
	}
	return 1;
}

public GetPlayerItems(playerid)
{
	new query[256],itemlist[512];
	format(query,sizeof(query),"SELECT `item_uid`,`item_owneruid`,`item_name`,`item_value1`,`item_value2`,`item_type`,`item_posx`,`item_posy`,`item_posz`,`item_objectid`,`item_whereis`,`item_used` FROM core_items WHERE item_owneruid=%i AND item_whereis=%i",GetPlayerUID(playerid),WHEREIS_GRACZ);
	mysql_query(query);
	mysql_store_result();
	if(mysql_num_rows() > MAX_PLAYER_ITEMS)
	{
		GuiInfo(playerid,"Error 4026 : Zg³oœ siê do administratora.");
		return 1;
	}
	new resline[256];
	new num=1;
	while(mysql_fetch_row(resline,"|"))
	{
		sscanf(resline,"p<|>iis[32]iiifffiii",
		itemInfo[playerid][num][iUID],
		itemInfo[playerid][num][iOwner],
		itemInfo[playerid][num][iName],
		itemInfo[playerid][num][iValue1],
		itemInfo[playerid][num][iValue2],
		itemInfo[playerid][num][iType],
		itemInfo[playerid][num][iX],
		itemInfo[playerid][num][iY],
		itemInfo[playerid][num][iZ],
		itemInfo[playerid][num][iObject],
		itemInfo[playerid][num][iWhereis],
		itemInfo[playerid][num][iUsed]);
		if(!itemlist[0])
		{
			if(itemInfo[playerid][num][iUsed] == 1)
			{
				format(itemlist,sizeof(itemlist),"%i\t%i\t%i\t"COL_USE"%s",itemInfo[playerid][num][iUID],itemInfo[playerid][num][iValue1],itemInfo[playerid][num][iValue2],itemInfo[playerid][num][iName]);
			}
			else
			{
				format(itemlist,sizeof(itemlist),"%i\t%i\t%i\t%s",itemInfo[playerid][num][iUID],itemInfo[playerid][num][iValue1],itemInfo[playerid][num][iValue2],itemInfo[playerid][num][iName]);
			}
		}
		else
		{
			if(itemInfo[playerid][num][iUsed] == 1)
			{
				format(itemlist,sizeof(itemlist),"%s\n%i\t%i\t%i\t"COL_USE"%s",itemlist,itemInfo[playerid][num][iUID],itemInfo[playerid][num][iValue1],itemInfo[playerid][num][iValue2],itemInfo[playerid][num][iName]);
			}
			else
			{
				format(itemlist,sizeof(itemlist),"%s\n%i\t%i\t%i\t%s",itemlist,itemInfo[playerid][num][iUID],itemInfo[playerid][num][iValue1],itemInfo[playerid][num][iValue2],itemInfo[playerid][num][iName]);
			}
		}
		num++;
	}
	if(!itemlist[0])
	{
		GuiInfo(playerid,"Brak przedmiotów w ekwipunku.");
	}
	else
	{
		ShowPlayerDialog(playerid,DIAL_P,DIALOG_STYLE_LIST,"Przedmioty",itemlist,"Wybierz","Zamknij");
	}
	return 1;
}

stock ShowPlayerItemsToPlayer(playerid,whowatch)
{
	new query[256],itemlist[512];
	format(query,sizeof(query),"SELECT `item_uid`,`item_owneruid`,`item_name`,`item_value1`,`item_value2`,`item_type`,`item_posx`,`item_posy`,`item_posz`,`item_objectid`,`item_whereis`,`item_used` FROM core_items WHERE item_owneruid=%i AND item_whereis=%i",GetPlayerUID(playerid),WHEREIS_GRACZ);
	mysql_query(query);
	mysql_store_result();
	if(mysql_num_rows() > MAX_PLAYER_ITEMS)
	{
		GuiInfo(playerid,"Error 4026 : Zg³oœ siê do administratora.");
		return 1;
	}
	new resline[256];
	new num=1;
	while(mysql_fetch_row(resline,"|"))
	{
		sscanf(resline,"p<|>iis[32]iiifffiii",
		itemInfo[playerid][num][iUID],
		itemInfo[playerid][num][iOwner],
		itemInfo[playerid][num][iName],
		itemInfo[playerid][num][iValue1],
		itemInfo[playerid][num][iValue2],
		itemInfo[playerid][num][iType],
		itemInfo[playerid][num][iX],
		itemInfo[playerid][num][iY],
		itemInfo[playerid][num][iZ],
		itemInfo[playerid][num][iObject],
		itemInfo[playerid][num][iWhereis],
		itemInfo[playerid][num][iUsed]);
		if(!itemlist[0])
		{
			if(itemInfo[playerid][num][iUsed] == 1)
			{
				format(itemlist,sizeof(itemlist),"%i\t%i\t%i\t"COL_USE"%s",itemInfo[playerid][num][iUID],itemInfo[playerid][num][iValue1],itemInfo[playerid][num][iValue2],itemInfo[playerid][num][iName]);
			}
			else
			{
				format(itemlist,sizeof(itemlist),"%i\t%i\t%i\t%s",itemInfo[playerid][num][iUID],itemInfo[playerid][num][iValue1],itemInfo[playerid][num][iValue2],itemInfo[playerid][num][iName]);
			}
		}
		else
		{
			if(itemInfo[playerid][num][iUsed] == 1)
			{
				format(itemlist,sizeof(itemlist),"%s\n%i\t%i\t%i\t"COL_USE"%s",itemlist,itemInfo[playerid][num][iUID],itemInfo[playerid][num][iValue1],itemInfo[playerid][num][iValue2],itemInfo[playerid][num][iName]);
			}
			else
			{
				format(itemlist,sizeof(itemlist),"%s\n%i\t%i\t%i\t%s",itemlist,itemInfo[playerid][num][iUID],itemInfo[playerid][num][iValue1],itemInfo[playerid][num][iValue2],itemInfo[playerid][num][iName]);
			}
		}
		num++;
	}
	if(!itemlist[0])
	{
		GuiInfo(whowatch,"Brak przedmiotów w ekwipunku.");
	}
	else
	{
		ShowPlayerDialog(whowatch,404,DIALOG_STYLE_LIST,"Przedmioty",itemlist,"Wybierz","Zamknij");
		new money[64];
		format(money,sizeof(money),"ilosc dolanow: ~g~%i$",pInfo[playerid][pMoney]);
		HudText(whowatch,money);
	}
	return 1;
}

stock IsPlayerHasMegafon(playerid)
{
	new buffer[256];
	format(buffer,sizeof(buffer),"SELECT * FROM core_items WHERE item_owneruid=%i AND item_whereis=%i AND item_type=%i",GetPlayerUID(playerid),0,ITEM_MEGAFON);
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
	}
	return 0;
}

stock IsPlayerHasNarzedzia(playerid)
{
	new buffer[256];
	format(buffer,sizeof(buffer),"SELECT * FROM core_items WHERE item_owneruid=%i AND item_whereis=%i AND item_type=%i",GetPlayerUID(playerid),0,ITEM_NARZEDZIA);
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
	}
	return 0;
}

forward ApplyPlayerParalyzed(playerid);
public ApplyPlayerParalyzed(playerid)
{
	if(ParalyzedTime[playerid] == 0)
	{
		//nic
		ParalyzedTime[playerid] = 0;
		TogglePlayerControllable(playerid,1);
	}
	else
	{
		//ApplyAnimation(playerid, "CRACK", "crckidle2", 4, 0, 0, 1, 1, 0, 0);
		ParalyzedTime[playerid]--;
		SetTimerEx("ApplyPlayerParalyzed", 1000, false, "i", playerid);
	}
	return 1;
}

forward ApplyPlayerShooted(playerid);
public ApplyPlayerShooted(playerid)
{
	if(ShootedTime[playerid] == 0)
	{
		//nic
		ShootedTime[playerid] = 0;
		TogglePlayerControllable(playerid,1);
	}
	else
	{
		ShootedTime[playerid]--;
		SetTimerEx("ApplyPlayerShooted", 1000, false, "i", playerid);
	}
	return 1;
}

stock IsWeaponParalyzer(weapuid)
{
	new buffer[256];
	format(buffer,sizeof(buffer),"SELECT item_type FROM core_items WHERE item_uid=%i",weapuid);
	mysql_query(buffer);
	mysql_store_result();
	new typ = mysql_fetch_int();
	mysql_free_result();
	if(typ == ITEM_PARALIZATOR)
	{
		return 1;
	}
	return 0;
}

stock CreateAttachedItem(playerid,modelid,bone,name[])
{
	new buffer[256];					
	format(buffer,sizeof(buffer),"INSERT INTO core_items(item_owneruid,item_whereis,item_value1,item_type,item_name) VALUES(%i,%i,%i,%i,'%s')",GetPlayerUID(playerid),0,modelid,ITEM_ATTACH,name);
	mysql_query(buffer);
	
	format(buffer,sizeof(buffer),"SELECT MAX(item_uid) FROM core_items");
	mysql_query(buffer);
	mysql_store_result();
	new itemUid = mysql_fetch_int();
	mysql_free_result();
	
	format(buffer,sizeof(buffer),"INSERT INTO core_attachments VALUES(NULL,%i,%i,0,0,0,0,0,0,1,1,1)",itemUid ,bone);
	mysql_query(buffer);
	
	return 1;
}

stock IsPlayerItemOwner(playerid,itemuid)
{
	new buffer[256];
	format(buffer,sizeof(buffer),"SELECT * FROM core_items WHERE item_owneruid=%i AND item_whereis=%i AND item_uid=%i",GetPlayerUID(playerid),WHEREIS_GRACZ,itemuid);
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

CMD:kup(playerid,params[])
{
	new doorid = GetPlayerDoorID(playerid);
	if(!doorid)
	{
		GuiInfo(playerid,"Musisz byæ w sklepie by u¿yæ tej komendy.");
	}
	else
	{
		if(DoorInfo[doorid][doorOwnerType] == DOOR_TYPE_247)
		{
			new artykuly[512];
			format(artykuly,sizeof(artykuly),"#1 Telefon \t\t 150$\n#2 Papierosy \t\t 15$\n#3 Piwo \t\t 8$\n#4 Zegarek \t\t 100$\n#5 Torba \t\t80$\n#6 Chleb \t\t5$");
			ShowPlayerDialog(playerid,DIAL_KUP,DIALOG_STYLE_LIST,"Artyku³y w sklepie",artykuly,"Wybierz","Anuluj");
		}
		else if(DoorInfo[doorid][doorOwnerType] == DOOR_TYPE_APTEKA)
		{
			new artykuly[512];
			format(artykuly,sizeof(artykuly),"#1 Aspiryna \t\t 15$\n#2 Syrop Acodin \t\t 10$\n#3 Witamina C \t\t 18$\n#4 Pusta strzykawka \t\t 5$");
			ShowPlayerDialog(playerid,DIAL_KUP_APTEKA,DIALOG_STYLE_LIST,"Artyku³y w aptece",artykuly,"Wybierz","Anuluj");
		}
	}
	return 1;
}

CMD:akcesoria(playerid,params[])
{
	new doorid = GetPlayerDoorID(playerid);
	if(!doorid)
	{
		GuiInfo(playerid,"Musisz byæ w sklepie by u¿yæ tej komendy.");
	}
	else
	{
		if(DoorInfo[doorid][doorOwnerType] == DOOR_TYPE_ATTACHMENTS)
		{
			PlayerCache[playerid][pBuyAttach] = 1;
			TogglePlayerControllable(playerid,0);
			
			new Float:PosX, Float:PosY, Float:PosZ;
			GetPlayerPos(playerid, PosX, PosY, PosZ);

			GetXYInFrontOfPlayer(playerid, PosX, PosY, 4.0);
			SetPlayerCameraPos(playerid, PosX, PosY, PosZ + 0.30);

			GetPlayerPos(playerid, PosX, PosY, PosZ);
			SetPlayerCameraLookAt(playerid, PosX, PosY, PosZ);
			
			new string[256];
			format(string,sizeof(string),"~r~ENTER~w~ by kupic wybrane akcesorium~n~~r~LALT~w~ by przerwac wybieranie~n~Kliknij ~g~~<~~w~lub~g~~>~ ~w~aby przewijac");
			
			TextDrawSetString(TextDrawSkins[playerid], string);
			TextDrawShowForPlayer(playerid,TextDrawSkins[playerid]);
		}
	}
	return 1;
}

enum lspd_specials
{
	spd_name[32],
	spd_model,
	spd_price,
	spd_bone,
}
new LspdSpecials[4][lspd_specials] = 
{
	//{"Kamizelka",19142,200,1},		//bo ta niby biala
	{"Kamizelka",19515,200,1},
	{"Czapka PD",18636,25,2},
	{"Helm SWAT",19141,105,2},
	{"Odznaka",19347,30,1}
};

CMD:specjalne(playerid,params[])
{
	new string[32];
	if(sscanf(params,"s[32]",string))
	{
		vlive_err(playerid,"/specjalne [lspd | lsfd | fbi]");
		return 1;
	}
	
	if(!strcmp(string, "lspd", true))
	{
		new doorid = GetPlayerDoorID(playerid);
		if(IsPlayerPermsDoors(playerid,doorid))
		{
			new lista[512];
			format(lista,sizeof(lista),"1. Kamizelka\n2. Czapka PD\n3. Helm SWAT\n4. Odznaka");
			ShowPlayerDialog(playerid,DIAL_PURCHASE_LSPD_SPECIALS,DIALOG_STYLE_LIST,"Obiekty przyczepialne dla LSPD",lista,"Wybierz","Anuluj");
		}
		else
		{
			GuiInfo(playerid,"Brak uprawnieñ.");
		}
	}
	
	if(!strcmp(string, "fbi", true))
	{
		new doorid = GetPlayerDoorID(playerid);
		if(IsPlayerPermsDoors(playerid,doorid))
		{
			new lista[512];
			format(lista,sizeof(lista),"1. Kamizelka\n2. Czapka PD\n3. Helm SWAT\n4. Odznaka");
			ShowPlayerDialog(playerid,DIAL_PURCHASE_LSPD_SPECIALS,DIALOG_STYLE_LIST,"Obiekty przyczepialne dla FBI",lista,"Wybierz","Anuluj");
		}
		else
		{
			GuiInfo(playerid,"Brak uprawnieñ.");
		}
	}
	
	return 1;
}