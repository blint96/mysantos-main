// skrypcik tylko dla administracji
//
// mniej bugow
//

//
//			NAPRAWIÆ "ESC" PODCZAS EDYCJI OBIEKTÓW
//
//			OSEL ID -< nie ma cordów
//			/rotate [x | y | z] [stopnie]



#define MAX_OBJECTS_VL 30000

#define O_TYPE_OBJECT 1
#define O_TYPE_GATE 2

enum opars
{
	object_uid,
	object_sid,
	object_ownertype,
	object_ownerid,
	object_model,
	Float:oposx,
	Float:oposy,
	Float:oposz,
	Float:orx,
	Float:ory,
	Float:orz,
	object_world,
	object_interior,
	object_whatis, 		//co to jest , jeœli 1 to jest to przedmiot
	object_type,
	
	
	//bramy
	object_isgate,	//czy jest bram¹
	object_isblocade,
	Float:brOX,
	Float:brOY,
	Float:brOZ,
	isclosed,
}
new object[MAX_OBJECTS_VL][opars];

forward LoadObjectsOnInit();
forward InitObjects();

new SelectedObject[MAX_PLAYERS];
new CreateGate[MAX_PLAYERS];

new PlayerText:objEdytor[MAX_PLAYERS];
new PlayerText:objX[MAX_PLAYERS];
new PlayerText:objY[MAX_PLAYERS];
new PlayerText:objZ[MAX_PLAYERS];


COMMAND:oc(playerid,params[])
{
	new doorid = GetPlayerDoorID(playerid);
	if(IsPlayerPermsDoorsObjects(playerid,doorid) > 0)
	{
		new modelid = strval(params);
		AddObject(playerid,modelid);
	}
	else
	{
		GuiInfo(playerid,"Nie masz uprawnieñ do wykonania tej komendy.");
	}
	return 1;
}

COMMAND:ocopy(playerid,params[])			//kopiowanie aktualnie edytowanego obiektu
{
	if(SelectedObject[playerid] > 0)
	{
		CopyObject(SelectedObject[playerid] );
		SendClientMessage(playerid,COLOR_GREY,"[obiekty] Skopiowa³eœ obiekt, nowy pojawi³ siê w miejscu obecnie zaznaczonego.");
	}
	else
	{
		GuiInfo(playerid,"Nie edytujesz ¿adnego obiektu.");
	}
	return 1;
}

/*COMMAND:rx(playerid,params[])		
{
	if(SelectedObject[playerid] > 0)
	{
		new Float:rotate;
		if(sscanf(params,"f",rotate))
		{
			vlive_err(playerid,"/rx [stopnie]");
			return 1;
		}
		new Float:x,Float:y,Float:z;
		GetDynamicObjectPos(SelectedObject[playerid] ,x,y,z);
		
		SetDynamicObjectPos(SelectedObject[playerid] ,rotate,y,z);
	}
	else
	{
		GuiInfo(playerid,"Nie edytujesz ¿adnego obiektu.");
	}
	return 1;
}

COMMAND:ry(playerid,params[])		
{
	if(SelectedObject[playerid] > 0)
	{
		new Float:rotate;
		if(sscanf(params,"f",rotate))
		{
			vlive_err(playerid,"/ry [stopnie]");
			return 1;
		}
		
		new Float:x,Float:y,Float:z;
		GetDynamicObjectPos(SelectedObject[playerid] ,x,y,z);
		
		SetDynamicObjectPos(SelectedObject[playerid] ,x,rotate,z);
	}
	else
	{
		GuiInfo(playerid,"Nie edytujesz ¿adnego obiektu.");
	}
	return 1;
}

COMMAND:rz(playerid,params[])
{
	if(SelectedObject[playerid] > 0)
	{
		new Float:rotate;
		if(sscanf(params,"f",rotate))
		{
			vlive_err(playerid,"/rz [stopnie]");
			return 1;
		}
		
		new Float:x,Float:y,Float:z;
		GetDynamicObjectPos(SelectedObject[playerid] ,x,y,z);
		
		SetDynamicObjectPos(SelectedObject[playerid] ,x,y,rotate);
	}
	else
	{
		GuiInfo(playerid,"Nie edytujesz ¿adnego obiektu.");
	}
	return 1;
}*/

COMMAND:osel(playerid,params[])
{
	new doorid = GetPlayerDoorID(playerid);
	if(IsPlayerPermsDoorsObjects(playerid,doorid) > 0)
	{
		new modelid;
		if(sscanf(params,"i",modelid))
		{
			SelectObject(playerid);
			SendClientMessage(playerid,COLOR_GREY,"[obiekty] Zaznacz obiekt, który chcesz edytowaæ.");
		}
		else
		{
			GetNearObjectModel(modelid,playerid);
		}
	}
	else
	{
		GuiInfo(playerid,"Nie masz uprawnieñ do wykonania tej komendy.");
	}
	return 1;
}

public OnPlayerSelectDynamicObject(playerid, objectid, modelid, Float:x, Float:y, Float:z)
{
	//if(object[objectid][object_whatis] != 1)
	//{
	EditDynamicObject(playerid, objectid);
	ShowObjectTx(playerid);
	SelectedObject[playerid] = objectid;
	//}
	return 1;
}

public OnPlayerEditDynamicObject(playerid, objectid, response, Float:x, Float:y, Float:z, Float:rx, Float:ry, Float:rz)
{
	//SelectedObject[playerid] = objectid;
	if(response == EDIT_RESPONSE_FINAL)
	{
		SetDynamicObjectPos(objectid,x,y,z);
        SetDynamicObjectRot(objectid,rx,ry,rz);
		
		new buffer[256];
		
		SendClientMessage(playerid,COLOR_GREY,"[obiekty] Obiekt zapisany.");
		
		if(CreateGate[playerid] > 0)
		{
			format(buffer,sizeof(buffer),"UPDATE core_objects SET object_type=%i , ox=%f, oy=%f, oz=%f WHERE object_uid=%i",O_TYPE_GATE,x,y,z,object[objectid][object_uid]);
			mysql_query(buffer);
			CreateGate[playerid] = EOS;
			
			object[objectid][brOX] = x;
			object[objectid][brOY] = y;
			object[objectid][brOZ] = z;
			object[objectid][object_type] = O_TYPE_GATE;
			
			SelectedObject[playerid] = EOS;
			HideObjectTx(playerid);
			GuiInfo(playerid,"Stworzy³eœ bramê.");
			return 1;
		}
		else
		{
			format(buffer,sizeof(buffer),"UPDATE core_objects SET object_posx=%f, object_posy=%f, object_posz=%f,object_rotatex=%f,object_rotetey=%f,object_rotatez=%f WHERE object_uid=%i",x,y,z,rx,ry,rz,object[objectid][object_uid]);
			mysql_query(buffer);
		}
		object[objectid][oposx] = x;
		object[objectid][oposy] = y;
		object[objectid][oposz] = z;
		object[objectid][orx] = rx;
		object[objectid][ory] = ry;
		object[objectid][orz] = rz;
		
		
		SelectedObject[playerid] = EOS;
		HideObjectTx(playerid);
	}
	else if( response == EDIT_RESPONSE_UPDATE )
	{
		new xstring[32];
		format(xstring,sizeof(xstring),"Rotate X : %f",rx);
		PlayerTextDrawSetString(playerid, objX[playerid], xstring);
		format(xstring,sizeof(xstring),"Rotate Y : %f",ry);
		PlayerTextDrawSetString(playerid, objY[playerid], xstring);
		format(xstring,sizeof(xstring),"Rotate Z : %f",rz);
		PlayerTextDrawSetString(playerid, objZ[playerid], xstring);
	}
	else if(response == EDIT_RESPONSE_CANCEL)
	{
		SetDynamicObjectPos(objectid,object[objectid][oposx],object[objectid][oposy],object[objectid][oposz]);
        SetDynamicObjectRot(objectid,object[objectid][orx] ,object[objectid][ory] ,object[objectid][orz] );
		SelectedObject[playerid] = EOS;
		HideObjectTx(playerid);
	}
	
	return 1;
}
	
COMMAND:odel(playerid,params[])
{
	new doorid = GetPlayerDoorID(playerid);
	if(IsPlayerPermsDoorsObjects(playerid,doorid) > 0)
	{
		new buffer[256];
		format(buffer,sizeof(buffer),"DELETE FROM core_objects WHERE object_uid=%i",object[SelectedObject[playerid]][object_uid]);
		mysql_query(buffer);
		SendClientMessage(playerid,COLOR_GREY,"[obiekty] Usun¹³es obiekt.");
		
		DestroyDynamicObject(SelectedObject[playerid]);
		object[SelectedObject[playerid]][object_model] = -1;
		SelectedObject[playerid] = -1;
		
		CancelEdit(playerid);
		HideObjectTx(playerid);
	}
	else
	{
		GuiInfo(playerid,"Brak uprawnieñ.");
	}
	return 1;
}

COMMAND:ohelp(playerid,params[])
{
	new help[512];
	format(help,sizeof(help),"Dokumentacja dotycz¹ca u¿ywania In Game edytora obiektów : \n\n1. /oc [modelid] - tworzenie obiektu o podanym modelu\n2. /osel - przejœcie w tryb wybierania obiektu\n4. /odel - usuwanie obiektu podczas jego edycji	\n5. /ocopy - kopiowanie aktualnie edytowanego obiektu");
	ShowPlayerDialog(playerid,404,DIALOG_STYLE_MSGBOX,"Edytor obiektów » pomoc",help,"OK","");
	return 1;
}

COMMAND:ogate(playerid,params[])
{
	new doorid = GetPlayerDoorID(playerid);
	if(IsPlayerPermsDoorsObjects(playerid,doorid) > 0)
	{
		if(SelectedObject[playerid] > 0)
		{
			new id = SelectedObject[playerid] ;
			if(object[id][object_type] == O_TYPE_GATE)
			{
				vlive_err(playerid,"ten obiekt jest ju¿ bram¹, mo¿esz go usun¹æ");
				return 1;
			}
			else
			{
				SendClientMessage(playerid,COLOR_RED,"Tworzenie bramy: przesuñ bramê w miejsce 'zamkniêcia' i zapisz obiekt.");
				CreateGate[playerid] = id;
			}
		}
		else
		{
			vlive_err(playerid,"nie zaznaczy³eœ ¿adnego obiektu");
	}
	}
	return 1;
}

COMMAND:brama(playerid,params[])
{
	new doorid = GetPlayerDoorID(playerid);
	for(new i = 0 ; i < MAX_OBJECTS_VL; i++)
	{
		if(IsPlayerInRangeOfPoint(playerid,5.9,object[i][oposx],object[i][oposy],object[i][oposz]))
		{
			if(doorid == 0)
			{
				if(object[i][object_type] == O_TYPE_GATE)
				{
					if(object[i][object_ownertype] == DOOR_TYPE_GROUP)
					{
						if(object[i][object_ownerid] == pGrupa[playerid][PlayerDutyGroup[playerid]][pGuid])
						{
							if(object[i][isclosed] == 0)
							{
								MoveDynamicObject(i,object[i][brOX], object[i][brOY], object[i][brOZ], 5);
								object[i][isclosed] = 1;
							}
							else
							{
								MoveDynamicObject(i,object[i][oposx], object[i][oposy], object[i][oposz], 5);
								object[i][isclosed] = 0;
							}
						}
					}
				}
				return 1;
			}	
			if(IsPlayerCanOpenGates(playerid,doorid))
			{
				if(object[i][object_type] == O_TYPE_GATE)
				{
					if(object[i][isclosed] == 0)
					{
						MoveDynamicObject(i,object[i][brOX], object[i][brOY], object[i][brOZ], 5);
						object[i][isclosed] = 1;
					}
					else
					{
						MoveDynamicObject(i,object[i][oposx], object[i][oposy], object[i][oposz], 5);
						object[i][isclosed] = 0;
					}
				}
			}
			else
			{
				GuiInfo(playerid,"Nie mo¿esz otwieraæ drzwi i bram w tym budynku.");
			}
		}
	}
	return 1;
}

public LoadObjectsOnInit()
{
	new buffer[256];
	format(buffer,sizeof(buffer),"SELECT * FROM core_objects");
	mysql_query(buffer);
	mysql_store_result();
	new ile=0;
	while(mysql_fetch_row(buffer,"|"))
	{
		ile++;
		new id = CheckFreeObjectSlot();
		sscanf(buffer,"p<|>iiiifffffffffiii",
		object[id][object_uid],
		object[id][object_ownertype],
		object[id][object_ownerid],
		object[id][object_model],
		object[id][oposx],
		object[id][oposy],
		object[id][oposz],
		object[id][brOX],
		object[id][brOY],
		object[id][brOZ],
		object[id][orx],
		object[id][ory],
		object[id][orz],
		object[id][object_world],
		object[id][object_interior],
		object[id][object_type]);
		
		object[id][object_sid] = CreateDynamicObject(object[id][object_model], object[id][oposx], object[id][oposy], object[id][oposz], object[id][orx], object[id][ory],object[id][orz], object[id][object_world], object[id][object_interior], -1,  300.0);
	}
	printf("[obiekty] zaladowano %i obiektow",ile);
	mysql_free_result();
	
	new num = 0;
	for(new i =0 ; i< MAX_OBJECTS_VL; i++)
	{
		if(object[i][object_type] == O_TYPE_GATE)
		{
			num++;
		}
	}
	printf("[gates] za³adowano %i bram",num);
	return 1;
}

stock IsPlayerPermsDoorsObjects(playerid,doorid)
{
	if(CheckPlayerAccess(playerid) > PERMS_SUPPORT) return 1;
	if(DoorInfo[doorid][doorOwnerType] == DOOR_TYPE_PLAYER)
	{
		if(DoorInfo[doorid][doorOwnerUID] == pInfo[playerid][pUID])
		{
			return 1;
		}
	}
	else if(DoorInfo[doorid][doorOwnerType] == DOOR_TYPE_GROUP)
	{
		for(new gid=1; gid < MAX_PLAYER_GROUPS; gid++)
		{
			if(pGrupa[playerid][gid][pGuid] == DoorInfo[doorid][doorOwnerUID])
			{
				if(pGrupa[playerid][gid][permLeader] == 1)
				{
					return 1;
				}
			}
		}
	}

	return 0;
}

stock IsPlayerCanOpenGates(playerid,doorid)
{
	if(CheckPlayerAccess(playerid) > PERMS_SUPPORT) return 1;
	if(DoorInfo[doorid][doorOwnerType] == DOOR_TYPE_PLAYER)
	{
		if(DoorInfo[doorid][doorOwnerUID] == pInfo[playerid][pUID])
		{
			return 1;
		}
	}
	else if(DoorInfo[doorid][doorOwnerType] == DOOR_TYPE_GROUP)
	{
		for(new gid=1; gid < MAX_PLAYER_GROUPS; gid++)
		{
			if(pGrupa[playerid][gid][pGuid] == DoorInfo[doorid][doorOwnerUID])
			{
				if(pGrupa[playerid][gid][permDoors] == 1)
				{
					return 1;
				}
			}
		}
	}
	return 0;
}

stock AddObject(playerid,modelid)
{
	if(modelid != 0)
	{
		new buffer[256];
		new Float:x,Float:y,Float:z;
		new worldid = GetPlayerVirtualWorld(playerid);
		new interiorid = GetPlayerInterior(playerid);
		GetPlayerPos(playerid,x,y,z);
		x = x + 2;
		y = y + 2;
		format(buffer,sizeof(buffer),"INSERT INTO core_objects VALUES(NULL,0,0,%i,%f,%f,%f,0,0,0,0,0,0,%i,%i,1)",modelid,x,y,z,worldid,interiorid);
		mysql_query(buffer);
		format(buffer,sizeof(buffer),"SELECT MAX(object_uid) FROM core_objects");
		mysql_query(buffer);
		mysql_store_result();
		new maxid = mysql_fetch_int();
		mysql_free_result();
		
		format(buffer,sizeof(buffer),"SELECT * FROM core_objects WHERE object_uid=%i",maxid);
		mysql_query(buffer);
		mysql_store_result();
		while(mysql_fetch_row(buffer,"|"))
		{
			new id = CheckFreeObjectSlot();
			sscanf(buffer,"p<|>iiiifffffffffiii",
			object[id][object_uid],
			object[id][object_ownertype],
			object[id][object_ownerid],
			object[id][object_model],
			object[id][oposx],
			object[id][oposy],
			object[id][oposz],
			object[id][brOX],
			object[id][brOY],
			object[id][brOZ],
			object[id][orx],
			object[id][ory],
			object[id][orz],
			object[id][object_world],
			object[id][object_interior],
			object[id][object_type]);
			
			object[id][object_sid] = CreateDynamicObject(object[id][object_model], object[id][oposx], object[id][oposy], object[id][oposz], object[id][orx], object[id][ory],object[id][orz], object[id][object_world], object[id][object_interior], -1,  300.0);
		}
		mysql_free_result();	
		
		Streamer_Update(playerid);
	}
}

stock CopyObject(objectid)
{
	if(object[objectid][object_model] > 0)
	{
		//new newobject = CheckFreeObjectSlot();
		new buffer[256];
		//object[newobject][object_sid] = CreateDynamicObject(object[objectid][object_model], object[objectid][oposx], object[objectid][oposy], object[objectid][oposz], object[objectid][orx], object[objectid][ory],object[objectid][orz], object[objectid][object_world], object[objectid][object_interior], -1,  300.0);
		format(buffer,sizeof(buffer),"INSERT INTO core_objects VALUES(NULL,0,0,%i,%f,%f,%f,0,0,0,%f,%f,%f,%i,%i,1)",
		object[objectid][object_model],
		object[objectid][oposx],
		object[objectid][oposy],
		object[objectid][oposz],
		object[objectid][orx],
		object[objectid][ory],
		object[objectid][orz],	
		object[objectid][object_world],
		object[objectid][object_interior]);
		mysql_query(buffer);
		
		format(buffer,sizeof(buffer),"SELECT MAX(object_uid) FROM core_objects");
		mysql_query(buffer);
		mysql_store_result();
		new maxid = mysql_fetch_int();
		mysql_free_result();
		
		format(buffer,sizeof(buffer),"SELECT * FROM core_objects WHERE object_uid=%i",maxid);
		mysql_query(buffer);
		mysql_store_result();
		while(mysql_fetch_row(buffer,"|"))
		{
			new id = CheckFreeObjectSlot();
			sscanf(buffer,"p<|>iiiifffffffffiii",
			object[id][object_uid],
			object[id][object_ownertype],
			object[id][object_ownerid],
			object[id][object_model],
			object[id][oposx],
			object[id][oposy],
			object[id][oposz],
			object[id][brOX],
			object[id][brOY],
			object[id][brOZ],
			object[id][orx],
			object[id][ory],
			object[id][orz],
			object[id][object_world],
			object[id][object_interior],
			object[id][object_type]);
			
			object[id][object_sid] = CreateDynamicObject(object[id][object_model], object[id][oposx], object[id][oposy], object[id][oposz], object[id][orx], object[id][ory],object[id][orz], object[id][object_world], object[id][object_interior], -1,  300.0);
		}
		mysql_free_result();
	}
}

stock LoadPlayerObjectTextdraws(playerid)
{
	objEdytor[playerid] = CreatePlayerTextDraw(playerid,370,330,"Koordynaty obiektu");
	objX[playerid] = CreatePlayerTextDraw(playerid,370,340,"Rotate X : 0");
	objY[playerid] = CreatePlayerTextDraw(playerid,370,350,"Rotate Y : 0");
	objZ[playerid] = CreatePlayerTextDraw(playerid,370,360,"Rotate Z : 0");

	PlayerTextDrawSetShadow(playerid,objEdytor[playerid],0);
	PlayerTextDrawFont(playerid,objEdytor[playerid],2);
	
	PlayerTextDrawSetShadow(playerid,objX[playerid] ,0);	//x
	PlayerTextDrawFont(playerid,objX[playerid] ,2);
	
	PlayerTextDrawSetShadow(playerid,objY[playerid] ,0);	//y
	PlayerTextDrawFont(playerid,objY[playerid] ,2);
	
	PlayerTextDrawSetShadow(playerid,objZ[playerid] ,0);	//z
	PlayerTextDrawFont(playerid,objZ[playerid] ,2);
}

stock CheckFreeObjectSlot()
{
	new id;
	for(new i = 1; i < MAX_OBJECTS_VL; i++)
	{
		if(object[i][object_model] > 0)
		{
			//brak, objekt istnieje
		}
		else
		{
			id = i;
			break;
		}
	}
	return id;
}

stock GetNearObjectModel(modelid,playerid)
{
	new Float:x,Float:y,Float:z;
	for (new i= 0;i< MAX_OBJECTS_VL;i++)
	{
		if(object[i][object_model] == modelid)
		{
			GetDynamicObjectPos(object[i][object_sid], x, y, z);
			if(IsPlayerInRangeOfPoint(playerid, 15,x,y,z))
			{
				if(object[i][object_world] == GetPlayerVirtualWorld(playerid))	//zmien
				{
					EditDynamicObject(playerid,object[i][object_sid]);
					ShowObjectTx(playerid);
					SelectedObject[playerid] = object[i][object_sid];
					break;
				}
			}
		}
	}
}

stock CountObjects(playerid)
{
	if(GetPlayerAccess(playerid) > 2)
	{
		new worldid = GetPlayerVirtualWorld(playerid);
		new liczba = 0;
		if(worldid == 0)
		{
			return -1;
		}
		for(new i=0;i<MAX_OBJECTS_VL;i++)
		{
			if(object[i][object_world] == worldid)
			{
				liczba++;
			}
		}
		return liczba;
	}
	else
	{
		GuiInfo(playerid,"Brak uprawnieñ.");
		return -1;
	}
}


stock ShowObjectTx(playerid)
{
	PlayerTextDrawShow(playerid,objEdytor[playerid]);
	PlayerTextDrawShow(playerid,objX[playerid]);
	PlayerTextDrawShow(playerid,objY[playerid]);
	PlayerTextDrawShow(playerid,objZ[playerid]);
}

stock HideObjectTx(playerid)
{
	PlayerTextDrawHide(playerid,objEdytor[playerid]);
	PlayerTextDrawHide(playerid,objX[playerid]);
	PlayerTextDrawHide(playerid,objY[playerid]);
	PlayerTextDrawHide(playerid,objZ[playerid]);
}

stock DeleteAllObjects()
{
	for(new i = 1;i < MAX_OBJECTS_VL; i++)
	{
		DestroyDynamicObject(i);
		object[i][object_model] = -1;
	}
}

public InitObjects()
{
	printf("[objects] system wystartowal");
	
	LoadObjectsOnInit();
	return 1;
}

COMMAND:agate(playerid,params[])
{
	if(CheckPlayerAccess(playerid) > PERMS_SUPPORT)
	{
		new groupid;
		if(sscanf(params,"d",groupid))
		{
			vlive_err(playerid,"/agate [groupid]");
			return 1;
		}
		
		if(grupa[groupid][Guid] <= 0)
		{
			GuiInfo(playerid,"Nie ma takiej grupy.");
			return 1;
		}
		
		if(SelectedObject[playerid] <= 0)
		{
			vlive_err(playerid,"b³¹d");
			return 1;
		}
		
		new buffer[256];
		format(buffer,sizeof(buffer),"UPDATE core_objects SET object_ownerid = %i , object_ownertype = %i WHERE object_uid=%i",grupa[groupid][Guid],1,object[SelectedObject[playerid]][object_uid]);
		mysql_query(buffer);
		
		object[SelectedObject[playerid]][object_ownerid] = grupa[groupid][Guid];
		object[SelectedObject[playerid]][object_ownertype] = 1;
		
		GuiInfo(playerid,"Przypisa³eœ bramê pod grupê.");
	}
	return 1;
}

COMMAND:nob(playerid,params[])
{
	new lista[1024];
	new Float:x,Float:y,Float:z;
	GetPlayerPos(playerid,x,y,z);
	format(lista,sizeof(lista),"ID\t\tModel\n###\t\t###");
	for(new i = 0 ; i <MAX_OBJECTS_VL; i++)
	{
		if(object[i][object_world] == GetPlayerVirtualWorld(playerid))
		{
			if(IsPlayerInRangeOfPoint(playerid,10,object[i][oposx],object[i][oposy],object[i][oposz]))
			{
				format(lista,sizeof(lista),"%s\n%i\t\t%i",lista,i,object[i][object_model]);
			}
		}
	}
	
	ShowPlayerDialog(playerid,404,DIALOG_STYLE_LIST,"Obiekty w pobli¿u",lista,"OK","");
	return 1;
}

stock IsPlayerInRangeOfObject(playerid,model,Float:distance)
{
	for(new i = 0 ; i < MAX_OBJECTS_VL; i++)
	{
		if(object[i][object_model] == model)
		{
			if(object[i][object_world] == GetPlayerVirtualWorld(playerid))
			{
				if(IsPlayerInRangeOfPoint(playerid,distance,object[i][oposx],object[i][oposy],object[i][oposz]))
				{
					return 1;
				}
			}
		}
	}
	
	return 0;
}