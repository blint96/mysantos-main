#define MAX_PLANTS 500
#define MIN_PLANT_TIME	192000

new AmfetaminaTimer[MAX_PLAYERS];
new MarihuanaTimer[MAX_PLAYERS];
new KokainaTimer[MAX_PLAYERS];
new LSDTimer[MAX_PLAYERS];

//
//
//		dynamic, nie zwykly bo na wszystkich worldach jest ;/
//
//

enum plants_params
{
	plant_id,
	Text3D:plant_txt,
	plant_obj,
	Float:plant_x,
	Float:plant_y,
	Float:plant_z,
	plant_vw,
	plant_time,
}
new Plant[MAX_PLANTS][plants_params];


forward AmfetaminaEffect(playerid);
public AmfetaminaEffect(playerid)
{
	if(IsPlayerConnect(playerid))
	{
		if(AmfetaminaTimer[playerid] > 0)
		{
			if(AmfetaminaTimer[playerid] > 260)
			{
				if(AmfetaminaTimer[playerid] == 290)
				{
					SetPlayerDrunkLevel(playerid,4000);
					NarrateToPlayer(playerid,"Czujesz srogi przyp³yw si³y, nerwowo rozgl¹dasz siê wokó³ siebie w poszukiwaniu potencjalnych wrogów.");
				}
			}
			else if(AmfetaminaTimer[playerid] > 200)
			{
				if(AmfetaminaTimer[playerid] == 201)
				{
					SetPlayerDrunkLevel(playerid,4000);
					NarrateToPlayer(playerid,"Nie mo¿esz ustaæ w miejscu, co chwilê masz ochotê przemieœciæ siê lub biec.");
				}
			}
			else if(AmfetaminaTimer[playerid] > 150)
			{
				if(AmfetaminaTimer[playerid] == 151)
				{
					SetPlayerDrunkLevel(playerid,4000);
					NarrateToPlayer(playerid,"Mocno siê pocisz i czujesz niesamowicie szybkie bicie serca.");
				}
			}
			else if(AmfetaminaTimer[playerid] > 100)
			{
				if(AmfetaminaTimer[playerid] == 101)
				{
					SetPlayerDrunkLevel(playerid,4000);
					NarrateToPlayer(playerid,"Dzisiejszej nocy raczej nie przeœpisz.");
				}
			}
			else if(AmfetaminaTimer[playerid] > 20)
			{
				if(AmfetaminaTimer[playerid] == 21)
				{
					SetPlayerDrunkLevel(playerid,4000);
					NarrateToPlayer(playerid,"Doskwiera Ci strasznie upierdliwa suchoœæ w gardle.");
				}
			}
			
			AmfetaminaTimer[playerid]--;
			SetTimerEx("AmfetaminaEffect", 1000, 0, "d", playerid);
		}
		else
		{
			AmfetaminaTimer[playerid] = 0;
			PlayerCache[playerid][pBonusPower] = 0;
			SetPlayerDrunkLevel(playerid,0);
			statusPlayer[playerid] = 1;
		}
	}
}

forward MarihuanaEffect(playerid);
public MarihuanaEffect(playerid)
{
	if(IsPlayerConnect(playerid))
	{
		if(MarihuanaTimer[playerid] > 0)
		{
			if(MarihuanaTimer[playerid] > 230)
			{
				if(MarihuanaTimer[playerid] == 245)
				{
					NarrateToPlayer(playerid,"Czujesz coraz to wiêksze rozluŸnienie.");
					SetPlayerWeather(playerid,250);
					SetPlayerDrunkLevel(playerid,10000);
				}
			}
			else if(MarihuanaTimer[playerid] > 190)
			{
			
			}
			else if(MarihuanaTimer[playerid] > 90)
			{
				if(MarihuanaTimer[playerid] == 100)
				{
					NarrateToPlayer(playerid,"Tracisz orientacjê w przestrzeni, nie mo¿esz sobie przypomnieæ co sta³o siê chwilê temu.");
					//SetPlayerWeather(playerid,100);
					SetPlayerDrunkLevel(playerid,5000);
				}
			}
			MarihuanaTimer[playerid]--;
			SetTimerEx("MarihuanaEffect", 1000, 0, "d", playerid);
		}
		else
		{
			NarrateToPlayer(playerid,"Powoli wracasz do stanu wzglêdnej przytomnoœci.");
			SetPlayerWeather(playerid,1);
			MarihuanaTimer[playerid] = 0;
			statusPlayer[playerid] = 1;
		}
	}
}

forward KokainaEffect(playerid);
public KokainaEffect(playerid)
{
	if(IsPlayerConnect(playerid))
	{
	
	}
}

forward LSDEffect(playerid);
public LSDEffect(playerid)
{
	if(IsPlayerConnect(playerid))
	{
	
	}
}

stock CreatePlant(playerid)
{
	new Float:x,Float:y,Float:z;
	GetPlayerPos(playerid,x,y,z);
	
	new buffer[256];
	format(buffer,sizeof(buffer),"INSERT INTO core_plants VALUES(NULL,%f,%f,%f,%i,%i)",x,y,z,GetPlayerVirtualWorld(playerid),0);
	mysql_query(buffer);
	
	format(buffer,sizeof(buffer),"SELECT MAX(id) FROM core_plants");
	mysql_query(buffer);
	mysql_store_result();
	new uid = mysql_fetch_int();
	mysql_free_result();
	
	format(buffer,sizeof(buffer),"SELECT * FROM core_plants WHERE id= %i",uid);
	mysql_query(buffer);
	mysql_store_result();
	
	new num = GetFreePlantID();
	
	while(mysql_fetch_row(buffer,"|"))
	{
		sscanf(buffer,"p<|>ifffii",
		Plant[num][plant_id],
		Plant[num][plant_x],
		Plant[num][plant_y],
		Plant[num][plant_z],
		Plant[num][plant_vw],
		Plant[num][plant_time]);
		
		new info[128];
		new Float:percent = 100*Plant[num][plant_time]/MIN_PLANT_TIME;
		format(info,sizeof(info),"(( Krzak marihuany: %i/100 ))",floatround(percent));
		Plant[num][plant_txt] = Create3DTextLabel(info,COLOR_WHITE,Plant[num][plant_x],Plant[num][plant_y],Plant[num][plant_z] , 5, Plant[num][plant_vw], 1);
		Plant[num][plant_obj] = CreateObject(19473,Plant[num][plant_x],Plant[num][plant_y],Plant[num][plant_z]-0.75,0,0,0,50);
	}
	
	mysql_free_result();
	return 1;
}

stock GetFreePlantID()
{
	new plantid = 0 ;
	for(new i = 0; i < MAX_PLANTS; i++)
	{
		if(Plant[i][plant_id] <= 0 )
		{
			plantid = i;
			break;
		}
	}
	return plantid;
}	

stock LoadPlants()
{
	new buffer[256];
	format(buffer,sizeof(buffer),"SELECT * FROM core_plants");
	mysql_query(buffer);
	mysql_store_result();
	
	new num = 0;
	
	while(mysql_fetch_row(buffer,"|"))
	{
		sscanf(buffer,"p<|>ifffii",
		Plant[num][plant_id],
		Plant[num][plant_x],
		Plant[num][plant_y],
		Plant[num][plant_z],
		Plant[num][plant_vw],
		Plant[num][plant_time]);
		
		new info[128];
		new Float:percent = 100*Plant[num][plant_time]/MIN_PLANT_TIME;
		format(info,sizeof(info),"(( Krzak marihuany: %i/100 ))",floatround(percent));
		Plant[num][plant_txt] = Create3DTextLabel(info,COLOR_WHITE,Plant[num][plant_x],Plant[num][plant_y],Plant[num][plant_z] , 5, Plant[num][plant_vw], 1);
		Plant[num][plant_obj] = CreateObject(19473,Plant[num][plant_x],Plant[num][plant_y],Plant[num][plant_z]-0.75,0,0,0,50);
		num++;
	}
	
	printf("[load] zaladowano %i drzewek marihuany",num);
	
	mysql_free_result();
	return 1;
}

stock SystemNarkotykow()
{
	//ladowanie systemu
	LoadPlants();
	return 1;
}

stock SavePlants()
{
	printf("[save] zapisuje krzaki marihuany");
	//zapisywanie wszystkich krzaczkow
	for(new i = 0 ; i < MAX_PLANTS; i++)
	{
		if(Plant[i][plant_id] > 0)
		{
			new buffer[256];
			format(buffer,sizeof(buffer),"UPDATE core_plants SET time = %i WHERE id=%i",Plant[i][plant_time],Plant[i][plant_id] );
			mysql_query(buffer);
		}
	}
	return 1;
}

forward PlantsTimer();
public PlantsTimer()
{
	//print("[test] plant timer - tick");
	for(new i = 0 ; i < MAX_PLANTS; i++)
	{
		if(Plant[i][plant_time] >= MIN_PLANT_TIME)
		{
			new info[128];
			format(info,sizeof(info),"(( Krzak marihuany gotowy do zebrania ))");				
			Update3DTextLabelText(Plant[i][plant_txt], COLOR_SUPPORT, info);
			
			continue;
		}
		
		Plant[i][plant_time] = Plant[i][plant_time]+60;
		
		new info[128];
		new Float:percent = 100*Plant[i][plant_time]/MIN_PLANT_TIME;
		format(info,sizeof(info),"(( Krzak marihuany: %i/100 ))",floatround(percent));	
		
		Update3DTextLabelText(Plant[i][plant_txt], COLOR_WHITE, info);
	}
}

COMMAND:zbierz(playerid,params[])
{
	for(new i = 0 ; i < MAX_PLANTS ; i++)
	{
		if(IsPlayerInRangeOfPoint(playerid,5,Plant[i][plant_x],Plant[i][plant_y],Plant[i][plant_z]))
		{
			if(GetPlayerVirtualWorld(playerid) == Plant[i][plant_vw])
			{
				if(Plant[i][plant_time] >= MIN_PLANT_TIME)
				{
					//zbierz
					// problem ze wszystkimi roslinami, ale break chyba to poprawil
					
					new ilosc = random(15);
					new buffer[256];
					
					
					if(ilosc <= 0)
					{
						if(IsValidObject(Plant[i][plant_obj]))
						{
							DestroyObject(Plant[i][plant_obj]);
						}
						
						Delete3DTextLabel(Text3D:Plant[i][plant_txt]);
						
						format(buffer,sizeof(buffer),"DELETE FROM core_plants WHERE id=%i",Plant[i][plant_id]);
						mysql_query(buffer);
						
						Plant[i][plant_id] = EOS;
						Plant[i][plant_x] = EOS;
						Plant[i][plant_y] = EOS;
						Plant[i][plant_z] = EOS;
						Plant[i][plant_time] = EOS;
						
						GuiInfo(playerid,"Roœlinka by³a niezadbana, nie masz po¿ytku z jej zebrania.");
						
						ApplyAnimation(playerid,"BOMBER","BOM_Plant",4.1,0,0,0,0,0,1);
						break;
					}
					else
					{
						new seeds = random(3);
						format(buffer,sizeof(buffer),"INSERT INTO core_items(item_owneruid,item_whereis,item_value1,item_value2,item_type,item_name) VALUES (%i,%i,%i,%i,%i,'%s')",GetPlayerUID(playerid),0,ilosc,0,ITEM_MARIHUANA,"Marihuana");
						mysql_query(buffer);
						
						for(new s= 0 ; s< seeds ;s++)
						{
							format(buffer,sizeof(buffer),"INSERT INTO core_items(item_owneruid,item_whereis,item_value1,item_value2,item_type,item_name) VALUES (%i,%i,%i,%i,%i,'%s')",GetPlayerUID(playerid),0,1,0,ITEM_SEEDS,"Nasiona marihuany");
							mysql_query(buffer);
						}
					}
					
					if(IsValidObject(Plant[i][plant_obj]))
					{
						DestroyObject(Plant[i][plant_obj]);
					}
					
					Delete3DTextLabel(Text3D:Plant[i][plant_txt]);
					
					format(buffer,sizeof(buffer),"DELETE FROM core_plants WHERE id=%i",Plant[i][plant_id]);
					mysql_query(buffer);
					
					Plant[i][plant_id] = EOS;
					Plant[i][plant_x] = EOS;
					Plant[i][plant_y] = EOS;
					Plant[i][plant_z] = EOS;
					Plant[i][plant_time] = EOS;
					
					GuiInfo(playerid,"Zebra³eœ liœcie marihuany, masz je teraz w swoim ekwipunku.");
					
					ApplyAnimation(playerid,"BOMBER","BOM_Plant",4.1,0,0,0,0,0,1);
					break;
				}
				else
				{
					PlayerCache[playerid][pSelPlant] = i;
					//GuiInfo(playerid,"Ta roœlina nie jest jeszcze dojrza³a.");
					//potem moze usuwanie tu dodac
					ShowPlayerDialog(playerid,DIAL_GANJA_NIEDOJRZALA,DIALOG_STYLE_MSGBOX,"Informacja","Ta roœlina nie jest na tyle dojrza³a by rozpocz¹æ zbiory, czy chcesz j¹ zakopaæ?","Tak","Nie");
					break;
				}
			}
		}
	}
	return 1;
}