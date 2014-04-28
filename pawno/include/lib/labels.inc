#define MAX_LABELS_SCRIPT 1000

enum lab_pars
{
	lab_uid,
	lab_text[256],
	lab_vw,
	Float:lab_posx,
	Float:lab_posy,
	Float:lab_posz,
	lab_otype,
	lab_ouid,
	lab_color,
	lab_distance,
}
new LabelInfo[MAX_LABELS][lab_pars];

stock LoadLabels()
{
	new gameid,buffer[256];
	format(buffer,sizeof(buffer),"SELECT * FROM labels");
	mysql_query(buffer);
	mysql_store_result();
	while(mysql_fetch_row(buffer,"|"))
	{
		gameid++;
		sscanf(buffer,"p<|>is[256]ifffiiii",
		LabelInfo[gameid][lab_uid],
		LabelInfo[gameid][lab_text],
		LabelInfo[gameid][lab_vw],
		LabelInfo[gameid][lab_posx],
		LabelInfo[gameid][lab_posy],
		LabelInfo[gameid][lab_posz],
		LabelInfo[gameid][lab_otype],
		LabelInfo[gameid][lab_ouid],
		LabelInfo[gameid][lab_color],
		LabelInfo[gameid][lab_distance]);
		
		
		switch(LabelInfo[gameid][lab_color])
		{
			case 0:
			{
				Create3DTextLabel(LabelInfo[gameid][lab_text],COLOR_GREEN, LabelInfo[gameid][lab_posx], LabelInfo[gameid][lab_posy], LabelInfo[gameid][lab_posz], float(LabelInfo[gameid][lab_distance]), LabelInfo[gameid][lab_vw], 1);
			}
			case 1:
			{
				Create3DTextLabel(LabelInfo[gameid][lab_text],COLOR_WHITE, LabelInfo[gameid][lab_posx], LabelInfo[gameid][lab_posy], LabelInfo[gameid][lab_posz], float(LabelInfo[gameid][lab_distance]), LabelInfo[gameid][lab_vw], 1);
			}
			case 2:
			{
				Create3DTextLabel(LabelInfo[gameid][lab_text],COLOR_RED, LabelInfo[gameid][lab_posx], LabelInfo[gameid][lab_posy], LabelInfo[gameid][lab_posz], float(LabelInfo[gameid][lab_distance]), LabelInfo[gameid][lab_vw], 1);
			}
		}
	}
	mysql_free_result();
	
	printf("[labels] zaladowano %i labeli",gameid);
	return 1;
}

stock GetFreeLabelID()
{
	new label_id;
	for (new l = 1; l < MAX_LABELS; l++)
	{
	    if(!LabelInfo[l][lab_uid])
	    {
	        label_id = l;
	        break;
	    }
	}
	return label_id;
}

stock StartLabelSystem()
{
	print("[load] ladowanie modulu labeli");
	
	LoadLabels();
	
	return 1;
}

stock LoadLabel(gameid,uid)
{
	return 1;
}

stock SpawnLabel(uid)
{
	new buffer[256];
	//new gameid = GetFreeLabelID();
	format(buffer,sizeof(buffer),"SELECT * FROM labels WHERE id= %i",uid);
	mysql_query(buffer);
	mysql_store_result();
	while(mysql_fetch_row(buffer,"|"))
	{
		new luid,text[256],vw,Float:x,Float:y,Float:z,otype,ouid,color,distance;
		sscanf(buffer,"p<|>is[256]ifffiiii",luid,text,vw,x,y,z,otype,ouid,color,distance);
		
		printf("[load] wczytuje label (UID: %i)",uid);
		
		new gameid = Create3DTextLabel(text,COLOR_WHITE, x, y,z,float(distance), vw, 1);
		
		LabelInfo[gameid][lab_uid] = luid;
		LabelInfo[gameid][lab_vw] = vw;
		LabelInfo[gameid][lab_posx] = x;
		LabelInfo[gameid][lab_posy] = y;
		LabelInfo[gameid][lab_posz] = z;
		LabelInfo[gameid][lab_otype] = otype;
		LabelInfo[gameid][lab_ouid] = ouid;
		LabelInfo[gameid][lab_color] = color;
		LabelInfo[gameid][lab_distance] = distance;
	}
	mysql_free_result();
	return 1;
}

//(text[], color, Float:X, Float:Y, Float:Z, Float:DrawDistance, virtualworld, testLOS)
stock CreateLabel(text[],color,Float:X,Float:Y,Float:Z,distance,vw,ownerType,ownerUid)
{
	new buffer[256];
	format(buffer,sizeof(buffer),"INSERT INTO labels VALUES(NULL,'%s',%i,%f,%f,%f,%i,%i,%i,%i)",text,vw,X,Y,Z,ownerType,ownerUid,color,distance);
	mysql_query(buffer);
	
	format(buffer,sizeof(buffer),"SELECT MAX(id) FROM labels");
	mysql_query(buffer);
	mysql_store_result();
	new labelUID = mysql_fetch_int();
	mysql_free_result();
	
	SpawnLabel(labelUID);
	return 1;
}

COMMAND:alabel(playerid,params[])
{
	if(CheckPlayerAccess(playerid) > PERMS_SUPPORT)
	{
		new type[32], varchar[256];//string[256];
		if(sscanf(params, "s[32]S()[256]", type, varchar))
		{
			vlive_err(playerid, "/alabel [stworz | tresc | usun | pozycja | lista]");
			return 1;
		}
		
		if(!strcmp(type, "stworz", true))
		{
			new color,distance,text[256];
			if(sscanf(varchar,"dds[256]",color,distance,text))
			{
				vlive_err(playerid,"/alabel stworz [kolor] [zasieg widzenia] [tekst]");
				return 1;
			}
			new Float:x,Float:y,Float:z;
			GetPlayerPos(playerid,x,y,z);
			
			new vw = GetPlayerVirtualWorld(playerid);
			
			CreateLabel(text,color,Float:x,Float:y,Float:z,distance,vw,0,0);
		}
		
		if(!strcmp(type, "tresc", true))
		{
		
		}
		
		if(!strcmp(type, "pozycja", true))
		{
		
		}
		
		if(!strcmp(type, "usun", true))
		{
			new labid;
			if(sscanf(varchar,"d",labid))
			{
				vlive_err(playerid,"/alabel usun [label id]");
				return 1;
			}
			
			new buffer[256];
			format(buffer,sizeof(buffer),"DELETE FROM labels WHERE id=%i",LabelInfo[labid][lab_uid]);
			mysql_query(buffer);
			
			Delete3DTextLabel(labid);
			
			LabelInfo[labid][lab_uid] = EOS;
			LabelInfo[labid][lab_posx] = EOS;
			LabelInfo[labid][lab_posy] = EOS;
			LabelInfo[labid][lab_posz] = EOS;
			
			GuiInfo(playerid,"Usun¹³eœ wskazany label.");
		}
		
		if(!strcmp(type, "lista", true))
		{
			new lista[1024];
			format(lista,sizeof(lista),"ID\t\tTekst\n###\t\t###");
			for(new i = 0 ; i < MAX_LABELS; i++)
			{
				if(IsPlayerInRangeOfPoint(playerid,10,LabelInfo[i][lab_posx],LabelInfo[i][lab_posy],LabelInfo[i][lab_posz]))
				{
					if(LabelInfo[i][lab_vw] == GetPlayerVirtualWorld(playerid))
					{
						format(lista,sizeof(lista),"%s\n%i\t\t%s",lista,i,LabelInfo[i][lab_text]);
					}
				}
			}
			
			ShowPlayerDialog(playerid,404,DIALOG_STYLE_LIST,"Lista 3d tekstów w pobli¿u",lista,"OK","");
		}
	}
	else
	{
		GuiInfo(playerid,"Brak uprawnieñ.");
	}
	return 1;
}