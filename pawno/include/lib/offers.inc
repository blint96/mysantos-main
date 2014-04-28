#define OFFER_TYPE_CAR 1
#define OFFER_TYPE_ITEM 2
#define OFFER_TYPE_HEALING 3
#define OFFER_TYPE_MANDAT 4
#define OFFER_TYPE_YO 5
#define OFFER_TYPE_REPAIR	6
#define OFFER_TYPE_PRAWKO	7
#define OFFER_TYPE_REPAINT	8
#define OFFER_TYPE_REJESTRACJA	9
//#define OFFER_TYPE_UNBLOCK	10
#define OFFER_TYPE_CARDEALER	20
//#define OFFER_TYPE_REPAIR_NON	21

#define PRAWKO_CENA	350
#define REJESTRACJA_CENA	150

forward Healing(playerid);



stock LoadOffers()
{
	LoadOffersTextdraws();
	print("[oferty] za³adowano system ofert");
}

stock LoadOffersTextdraws()
{
	for (new i= 0 ; i < MAX_PLAYERS ; i++)
	{
		//oferty
		TextDrawOffers[i] = TextDrawCreate(326.000000, 330.000000, "_");
		TextDrawAlignment(TextDrawOffers[i], 2);
		TextDrawBackgroundColor(TextDrawOffers[i], 255);
		TextDrawFont(TextDrawOffers[i], 1);
		TextDrawLetterSize(TextDrawOffers[i], 0.379999, 1.299999);
		TextDrawColor(TextDrawOffers[i], -1);
		TextDrawSetOutline(TextDrawOffers[i], 1);
		TextDrawSetProportional(TextDrawOffers[i], 1);
		TextDrawUseBox(TextDrawOffers[i], 1);
		TextDrawBoxColor(TextDrawOffers[i], 68);
		TextDrawTextSize(TextDrawOffers[i], 490.000000, 221.000000);
	}
}

stock GetVehicleModelPrice(modelid)
{
	new price = 0;
	for(new i = 0 ; i < 212; i ++)
	{
		if(VehicleData[i][vModel] == modelid)
		{
			price = VehicleData[i][vPrice];
			break;
		}
	}
	
	return price;
}

//==================
//
//		Komendki
//
//==================

COMMAND:o(playerid,params[])
{
	if(pInfo[playerid][pBw] > 0)
	{
		GuiInfo(playerid,"Nie mo¿esz nic oferowaæ jeœli jesteœ nieprzytomny.");
		return 1;
	}
	new type[32], varchar[64];
	if(sscanf(params, "s[32]S()[64]", type, varchar))
	{
		new offerlist[512];
		format(offerlist, sizeof(offerlist), "Dostêpne typy ofert :\n» pojazd");	    
	    strcat(offerlist, "\n» przedmiot", sizeof(offerlist));
     	strcat(offerlist, "\n» drzwi", sizeof(offerlist));
     	strcat(offerlist, "\n» holowanie", sizeof(offerlist));
     	strcat(offerlist, "\n» vcard", sizeof(offerlist));
		strcat(offerlist, "\n» seks", sizeof(offerlist));
		strcat(offerlist, "\n» leczenie", sizeof(offerlist));
		strcat(offerlist, "\n» naprawa", sizeof(offerlist));
		strcat(offerlist, "\n» tankowanie", sizeof(offerlist));
		strcat(offerlist, "\n» mandat", sizeof(offerlist));
		strcat(offerlist, "\n» lakierowanie", sizeof(offerlist));
		strcat(offerlist, "\n» montaz", sizeof(offerlist));
		strcat(offerlist, "\n» paintjob", sizeof(offerlist));
		strcat(offerlist, "\n» odblokowanie", sizeof(offerlist));
		strcat(offerlist, "\n» przejazd", sizeof(offerlist));
		strcat(offerlist, "\n» karnet", sizeof(offerlist));
		//strcat(offerlist, "\n» biznes", sizeof(offerlist));				//wychodze z zalozenia ze to administracja nadaje grupe
		
		ShowPlayerDialog(playerid,DIAL_O,DIALOG_STYLE_LIST,"Dostêpne typy ofert",offerlist,"Wybierz","Zamknij");
		return 1;
	}
	
	if(!strcmp(type, "karnet", true))
	{
		new target,price;
		if(sscanf(varchar,"dd",target,price))
		{
			vlive_err(playerid,"/o karnet [playerid] [koszt karnetu]");
			return 1;
		}
		
		if(DutyGroupType[playerid] != TYPE_GYM)
		{
			GuiInfo(playerid,"Nie masz uprawnieñ do wykonania tej komendy.");
			return 1;
		}
		
		if(pGrupa[playerid][PlayerDutyGroup[playerid]][permOffers] <= 0)	
		{
			GuiInfo(playerid,"Nie masz uprawnieñ do wykonania tej komendy.");
			return 1;
		}
		
		if(IsPlayerInRangeOfPlayer(playerid,target,5))
		{

			Sprzedawca[target] = playerid;
			OfferPrice[target] = price;
			OfferType[target] = OFFER_TYPE_KARNET;		
			ConstructOffer(playerid,target,OFFER_TYPE_KARNET,"",price);
		}
		else
		{
			GuiInfo(playerid,"Gracz poza zasiêgiem.");
		}
	}
	
	if(!strcmp(type, "pojazdsalon", true))
	{
		new target,nazwa[64];
		if(sscanf(varchar,"ds[64]",target,nazwa))
		{
			vlive_err(playerid,"/o pojazdsalon [playerid] [nazwa pojazdu]");
			return 1;
		}
		
		if(DutyGroupType[playerid] != TYPE_CARDEALER)
		{
			GuiInfo(playerid,"Nie masz uprawnieñ do wykonania tej komendy.");
			return 1;
		}
		
		if(pGrupa[playerid][PlayerDutyGroup[playerid]][permOffers] <= 0)	
		{
			GuiInfo(playerid,"Nie masz uprawnieñ do wykonania tej komendy.");
			return 1;
		}
		
		if(IsPlayerInRangeOfPlayer(playerid,target,5))
		{
			for(new i = 0 ; i < 300; i ++)
			{
				if(!strcmp(nazwa, VehicleData[i][vName], true))
				{
					new price = VehicleData[i][vPrice];
					if(pInfo[target][pMoney] < price)
					{
						return GuiInfo(playerid,"Gracz nie posiada takiej iloœci gotówki.");
					}
					else
					{
						//oferta
						Sprzedawca[target] = playerid;
						OfferPrice[target] = price;
						OfferType[target] = OFFER_TYPE_CARDEALER;		
						OfferColor1[target] = i;
						ConstructOffer(playerid,target,OFFER_TYPE_CARDEALER,"",price);
					}
					return 1;
				}
			}
			
			GuiInfo(playerid,"Nie znaleziono pojazdu o takiej nazwie.");
		}
		else
		{
			GuiInfo(playerid,"Gracz poza zasiêgiem.");
		}
	}
	
	if(!strcmp(type, "przejazd", true))
	{
		//price - cena za 100 metrów
		new target,price;
		if(sscanf(varchar,"dd",target,price))
		{
			vlive_err(playerid, "/o przejazd [playerid] [cena za przejazd]");
			return 1;
		}
		
		if(DutyGroupType[playerid] != TYPE_TAXI)
		{
			GuiInfo(playerid,"Nie masz uprawnieñ do wykonania tej komendy.");
			return 1;
		}
		
		if(pGrupa[playerid][PlayerDutyGroup[playerid]][permOffers] <= 0)	
		{
			GuiInfo(playerid,"Nie masz uprawnieñ do wykonania tej komendy.");
			return 1;
		}
		
		if(IsPlayerInRangeOfPlayer(playerid,target,5))
		{
			if(pInfo[target][pMoney] >= price)
			{
				Sprzedawca[target] = playerid;
				OfferPrice[target] = price;
				OfferType[target] = OFFER_TYPE_PRZEJAZD;					
				ConstructOffer(playerid,target,OFFER_TYPE_PRZEJAZD,"",price);
			}
			else
			{
				GuiInfo(playerid,"Nie ma tyle hajsu.");
			}
		}
	}
	
	if(!strcmp(type, "montaz", true))
	{
		new odbiorca, partID, price;
		if(sscanf(varchar,"ddd",odbiorca, partID, price))
		{
			vlive_err(playerid, "/o montaz [playerid] [UID przedmiotu] [Cena]");
			return 1;
		}
		
		if(DutyGroupType[playerid] != TYPE_WORKSHOP)
		{
			GuiInfo(playerid,"Nie masz uprawnieñ do wykonania tej komendy.");
			return 1;
		}
		
		if(pGrupa[playerid][PlayerDutyGroup[playerid]][permOffers] <= 0)	
		{
			GuiInfo(playerid,"Nie masz uprawnieñ do wykonania tej komendy.");
			return 1;
		}
		
		if(IsPlayerInAnyVehicle(odbiorca))
		{
			if(pInfo[odbiorca][pMoney] < price)
			{
				GuiInfo(playerid,"Ten gracz ma za ma³o hajsów.");
			}
			else
			{
				if(IsPlayerItemOwner(playerid,partID))
				{
					if(IsPlayerInRangeOfPlayer(playerid,odbiorca,5))
					{
						SoldItemUID[odbiorca] = partID;
						Sprzedawca[odbiorca] = playerid;
						Kupiec[playerid] = odbiorca;
						OfferPrice[odbiorca] = price;
						OfferType[odbiorca] = OFFER_TYPE_MONTAZ;					
						OfferColor1[odbiorca] = GetPlayerVehicleID(odbiorca);
						ConstructOffer(playerid,odbiorca,OFFER_TYPE_MONTAZ,"",price);
					}
					else
					{
						GuiInfo(playerid,"Gracz poza zasiêgiem.");
					}
				}
				else
				{
					GuiInfo(playerid,"Nie jesteœ posiadaczem tego przedmiotu.");
				}
			}
		}
		else
		{
			GuiInfo(playerid,"Gracz musi byæ w pojeŸdzie.");
		}
	}
	
	if(!strcmp(type, "rejestracja", true))
	{
		new target,vehicleid;
		if(sscanf(varchar,"dd",target,vehicleid))
		{
			vlive_err(playerid,"/o rejestracja [gracz] [SampID pojazdu]");
			return 1;
		}
		
		if(DutyGroupType[playerid] != TYPE_GOV)
		{
			GuiInfo(playerid,"Nie masz uprawnieñ do wykonania tej komendy.");
			return 1;
		}
		
		if(IsPlayerCarOwner(target,vehicleid))
		{
			if(IsPlayerInRangeOfPlayer(playerid,target,5))
			{
				SoldItemUID[target] = vehicleid;
				Sprzedawca[target] = playerid;
				Kupiec[playerid] = target;
				OfferPrice[target] = REJESTRACJA_CENA;
				OfferType[target] = OFFER_TYPE_REJESTRACJA;
				
				ConstructOffer(playerid,target,OFFER_TYPE_REJESTRACJA,"",REJESTRACJA_CENA);
			}
			else
			{
				GuiInfo(playerid,"Gracz nie jest posiadaczem tego pojazdu.");
			}
		}
		else
		{
			GuiInfo(playerid,"Gracz nie jest posiadaczem tego pojazdu.");
		}
	}
	
	if(!strcmp(type, "mandat", true))
	{
		new targetid,cost,reason[64],points;
		if(sscanf(varchar,"ddds[64]",targetid,cost,points,reason))
		{
			vlive_err(playerid,"/o mandat [gracz] [koszt] [pkt karne] [powod]");
			return 1;
		}
		
		if(DutyGroupType[playerid] == TYPE_LSPD)
		{
			if(pGrupa[playerid][PlayerDutyGroup[playerid]][permOffers] <= 0)
			{
				GuiInfo(playerid,"Nie masz uprawnieñ do oferowania.");
				return 1;
			}
			
			if(IsPlayerInRangeOfPlayer(playerid,targetid,5))
			{
			
				//SoldItemUID[targetid] = vehicleid;
				Sprzedawca[targetid] = playerid;
				Kupiec[playerid] = targetid;
				OfferPrice[targetid] = cost;
				OfferType[targetid] = OFFER_TYPE_MANDAT;
				OfferColor1[targetid] = points;
				
				format(PlayerCache[targetid][pStringVal],128,"%s",reason);
				ConstructOffer(playerid,targetid,OFFER_TYPE_MANDAT,"",cost);
			}
			else
			{
				GuiInfo(playerid,"Gracz poza zasiêgiem.");
			}
		}
		else
		{
			GuiInfo(playerid,"Brak uprawnieñ.");
		}
	}
	
	if(!strcmp(type, "lakierowanie", true))
	{
		new odbiorca, col1, col2, cena;
		if(sscanf(varchar,"dddd",odbiorca,col1,col2,cena))
		{
			vlive_err(playerid, "/o lakierowanie [ID gracza] [Kolor 1] [Kolor 2] [Cena]");
			return 1;
		}
		
		if(DutyGroupType[playerid] != TYPE_WORKSHOP)
		{
			GuiInfo(playerid,"Nie masz uprawnieñ do wykonania tej komendy.");
			return 1;
		}
		
		if(pGrupa[playerid][PlayerDutyGroup[playerid]][permOffers] <= 0)	
		{
			GuiInfo(playerid,"Nie masz uprawnieñ do wykonania tej komendy.");
			return 1;
		}
		
		if(IsPlayerInAnyVehicle(odbiorca))
		{
			new vehicleid = GetPlayerVehicleID(odbiorca);
			if(IsPlayerInRangeOfPlayer(playerid,odbiorca,5))
			{
				if(col1 < 0 || col1 > 255)
				{
					GuiInfo(playerid,"Podano z³¹ kolorystykê.");
					return 1;
				}
				
				if(cena < 0)
				{
					GuiInfo(playerid,"Nie wprowadzono ceny.");
					return 1;
				}
				
				if(odbiorca == playerid)
				{
					GuiInfo(playerid,"Nie mo¿esz oferowaæ sobie czegoœ.");
					return 1;
				}
				
				if(IsPlayerCarOwner(odbiorca,vehicleid))
				{
					SoldItemUID[odbiorca] = vehicleid;
					Sprzedawca[odbiorca] = playerid;
					Kupiec[playerid] = odbiorca;
					OfferPrice[odbiorca] = cena;
					OfferType[odbiorca] = OFFER_TYPE_REPAINT;
					OfferColor1[odbiorca] = col1;
					OfferColor2[odbiorca] = col2;
					
					ConstructOffer(playerid,odbiorca,OFFER_TYPE_REPAINT,"",cena);
				}
				else
				{
					GuiInfo(playerid,"Wskazana osoba nie jest w³aœcicielem tego pojazdu.");
					return 1;
				}
			}
			else
			{
				GuiInfo(playerid,"Gracz poza zasiêgiem");
			}
		}
		else
		{
			GuiInfo(playerid,"Ten gracz nie jest w ¿adnym pojeŸdzie.");
		}
	}
	
	if(!strcmp(type, "pojazd", true))
	{
		new target,cena;
		if(sscanf(varchar, "dd", target, cena))
	    {
	        vlive_err(playerid, "/o pojazd [ID gracza] [Cena]");
	        return 1;
	    }
		if(IsPlayerInAnyVehicle(playerid))
		{
			new vehicleid = GetPlayerVehicleID(playerid);
			new seat = GetPlayerVehicleSeat(playerid);
			
			if(seat == 0)
			{
				if(vehicleid > 0)
				{
					if(IsPlayerCarOwner(playerid,vehicleid))
					{
						SoldVehicle[target] = vehicle[vehicleid][vuid];
						Sprzedawca[target] = playerid;
						Kupiec[playerid] = target;
						OfferPrice[target] = cena;
						OfferType[target] = OFFER_TYPE_CAR;
						
						if(vehicle[vehicleid][vStarted] > 0)
						{
							GuiInfo(playerid,"Nie mo¿esz sprzedawaæ pojazdu startowego.");
							return 1;
						}
						
						if(target == playerid)
						{
							GuiInfo(playerid,"Nie mo¿esz oferowaæ sobie czegoœ.");
							return 1;
						}
						new Float:x,Float:y,Float:z;
						GetPlayerPos(playerid,x,y,z);
						new vw = GetPlayerVirtualWorld(playerid);
						if(vw == GetPlayerVirtualWorld(target))
						{
							if(IsPlayerInRangeOfPoint(target,10,x,y,z))
							{
								ConstructOffer(playerid,target,OFFER_TYPE_CAR,"",cena);
							}
							else
							{
								GuiInfo(playerid,"Gracz poza zasiêgiem.");
							}
						}
						else
						{
							GuiInfo(playerid,"Gracz poza zasiêgiem.");
						}
						//oferty					
					}
					else
					{
						GuiInfo(playerid,"Nie jesteœ w³aœcicielem tego pojazdu.");
					}
				}
			}
		}
		else
		{
			GuiInfo(playerid,"Musisz byæ w pojeŸdzie by u¿yæ tej komendy.");
		}
	}
	if(!strcmp(type, "przedmiot", true))
	{
		new target,cena,uid;
		if(sscanf(varchar, "ddd", target,uid, cena))
	    {
	        vlive_err(playerid, "/o przedmiot [ID gracza] [UID przedmiotu] [Cena]");
	        return 1;
	    }
		
		new Float:Pos[3];
		GetPlayerPos(playerid,Pos[0],Pos[1],Pos[2]);
		new playervw = GetPlayerVirtualWorld(playerid);
		if(playervw == GetPlayerVirtualWorld(target))
		{
			if(IsPlayerInRangeOfPoint(target,5,Pos[0],Pos[1],Pos[2]))
			{
				if(IsPlayerItemOwner(playerid,uid))
				{
					if(target == playerid)
					{
						GuiInfo(playerid,"Nie mo¿esz oferowaæ sobie czegoœ.");
						return 1;
					}
					SoldItemUID[target] = uid;
					Sprzedawca[target] = playerid;
					Kupiec[playerid] = target;
					OfferPrice[target] = cena;
					OfferType[target] = OFFER_TYPE_ITEM;
					
					ConstructOffer(playerid,target,OFFER_TYPE_ITEM,"",cena);
					
				}
				else
				{
					GuiInfo(playerid,"Nie jesteœ w³aœcicielem takiego przedmiotu.");
					return 1;
				}
			}
			else
			{
				GuiInfo(playerid,"Gracz poza zasiêgiem.");
			}
		}
		else
		{
			GuiInfo(playerid,"Gracz poza zasiêgiem.");
		}
	}
	if(!strcmp(type, "leczenie", true))
	{
		new target,cena;
		if(sscanf(varchar,"dd",target,cena))
		{
			vlive_err(playerid," /o leczenie [gracz] [cena]");
			return 1;
		}
		
		new czy_moze = 0;
		if(DutyGroupType[playerid] == TYPE_LSMC)
		{
			czy_moze = 1;
		}
		
		if(pGrupa[playerid][PlayerDutyGroup[playerid]][permOffers] <= 0)			//czy na pewno dobre sprawdzenie?
		{
			GuiInfo(playerid,"Nie masz uprawnieñ do oferowania czegokolwiek.");
			return 1;
		}
		
		if(czy_moze == 0)
		{
			GuiInfo(playerid,"Brak uprawnieñ.");
			return 1;
		}
		
		new Float:Pos[3];
		GetPlayerPos(playerid,Pos[0],Pos[1],Pos[2]);
		new playervw = GetPlayerVirtualWorld(playerid);
		
		if(playervw == GetPlayerVirtualWorld(target))
		{
			if(IsPlayerInRangeOfPoint(target,5,Pos[0],Pos[1],Pos[2]))
			{
				Sprzedawca[target] = playerid;
				Kupiec[playerid] = target;
				OfferPrice[target] = cena;
				OfferType[target] = OFFER_TYPE_HEALING;					
				ConstructOffer(playerid,target,OFFER_TYPE_HEALING,"",cena);
			}
		}		
	}
	if(!strcmp(type, "naprawe", true))
	{
		new target;
		if(sscanf(varchar,"d",target))
		{
			vlive_err(playerid," /o naprawe [gracz]");
			return 1;
		}
		
		if(DutyGroupType[playerid] == TYPE_WORKSHOP)
		{
			if(PlayerCache[playerid][pRepairCar] != 0)
			{
				GuiInfo(playerid,"Naprawiasz ju¿ jakiœ pojazd.");
				return 1;
			}
			new Float:Pos[3];
			GetPlayerPos(playerid,Pos[0],Pos[1],Pos[2]);
			new playervw = GetPlayerVirtualWorld(playerid);
			
			if(playervw == GetPlayerVirtualWorld(target))
			{
				if(IsPlayerInRangeOfPoint(target,5,Pos[0],Pos[1],Pos[2]))
				{
					if(IsPlayerInAnyVehicle(target))
					{
						new vehicleid = GetPlayerVehicleID(target);
						new seat = GetPlayerVehicleSeat(target);
						if(seat == 0)
						{
							new Float:VehHP;
							GetVehicleHealth(vehicleid,VehHP);
							if(VehHP >= 1000)
							{
								GuiInfo(playerid,"Ten pojazd jest w pe³ni sprawny.");
								return 1;
							}
							new vehicleModel = GetVehicleModel(vehicleid);
							new Float:hp = 1000 - VehHP;
							new cena = floatround(GetVehicleModelPrice(vehicleModel) * 0.00005 * hp);
							Sprzedawca[target] = playerid;
							Kupiec[playerid] = target;
							OfferPrice[target] = cena;
							OfferType[target] = OFFER_TYPE_REPAIR;					
							ConstructOffer(playerid,target,OFFER_TYPE_REPAIR,"",cena);
						}
						else
						{
							GuiInfo(playerid,"Gracz musi byæ kierowc¹ pojazdu aby oferowaæ mu naprawê.");
						}
					}
					else
					{
						GuiInfo(playerid,"Ten gracz nie jest w pojeŸdzie.");
					}
				}
			}
			else
			{
				GuiInfo(playerid,"Brak uprawnieñ.");
			}
		}
	}
	if(!strcmp(type, "prawko", true))
	{
		new typ[64], target;
		if(sscanf(varchar,"s[64]d",typ,target))
		{
			vlive_err(playerid,"/o prawko [a | b | c | ce | d] [playerid]");
			return 1;
		}
		
		if(DutyGroupType[playerid] != TYPE_GOV)
		{
			GuiInfo(playerid,"Nie masz uprawnieñ do wykonania tej komendy.");
			return 1;
		}
		
		if(IsPlayerInRangeOfPlayer(playerid,target,5))
		{
			SoldItemUID[target] = 0;
			Sprzedawca[target] = playerid;
			Kupiec[playerid] = target;
			OfferPrice[target] = PRAWKO_CENA;
			OfferType[target] = OFFER_TYPE_PRAWKO;
			
			if(!strcmp(typ, "a", true))
			{
				SoldItemUID[target] = 1;
				if(pInfo[target][pHasPrawkoA] == 1)
				{
					GuiInfo(playerid,"Ta osoba posiada ju¿ prawo jazdy tej kategorii.");
					return 1;
				}
			}
			
			if(!strcmp(typ, "b", true))
			{
				SoldItemUID[target] = 2;
				if(pInfo[target][pHasPrawkoB] == 1)
				{
					GuiInfo(playerid,"Ta osoba posiada ju¿ prawo jazdy tej kategorii.");
					return 1;
				}
			}
			
			if(!strcmp(typ, "c", true))
			{
				SoldItemUID[target] = 3;
				if(pInfo[target][pHasPrawkoC] == 1)
				{
					GuiInfo(playerid,"Ta osoba posiada ju¿ prawo jazdy tej kategorii.");
					return 1;
				}
			}
			
			if(!strcmp(typ, "ce", true))
			{
				SoldItemUID[target] = 4;
				if(pInfo[target][pHasPrawkoCE] == 1)
				{
					GuiInfo(playerid,"Ta osoba posiada ju¿ prawo jazdy tej kategorii.");
					return 1;
				}
			}
			
			if(!strcmp(typ, "d", true))
			{
				SoldItemUID[target] = 5;
				if(pInfo[target][pHasPrawkoD] == 1)
				{
					GuiInfo(playerid,"Ta osoba posiada ju¿ prawo jazdy tej kategorii.");
					return 1;
				}
			}
			
			ConstructOffer(playerid,target,OFFER_TYPE_PRAWKO,"",PRAWKO_CENA);
		}
		else
		{
			GuiInfo(playerid,"Gracz nie jest w Twoim zasiêgu.");
		}
	}
	return 1;
}

stock ConstructOffer(playerid,kupiec,offertype,stringg[],price)
{
	stringg[0] = EOS;
	new string[256];
	// ustawiæ variables
	switch(offertype)
	{
		case OFFER_TYPE_BITEM:
		{
			format(string,sizeof(string),"~r~Oferta ~>~ ~w~przedmiot");
			
			//nazwa by sie przydala
			new buffer[256];
			format(buffer,sizeof(buffer),"SELECT item_name FROM core_items WHERE item_uid=%i",SoldItemUID[kupiec]);
			mysql_query(buffer);
			mysql_store_result();
			new itemName[32];
			mysql_fetch_string(itemName);
			mysql_free_result();
			
			format(string,sizeof(string),"%s~n~Nazwa: ~g~%s~w~",string,itemName);
			format(string,sizeof(string),"%s~n~Cena: ~g~%i$~w~",string,price);
			format(string,sizeof(string),"%s~n~Oferujacy: ~g~%s~w~",string,pInfo[playerid][pName]);
		}
		case OFFER_TYPE_KARNET:
		{
			format(string,sizeof(string),"~r~Oferta ~>~ ~w~karnet 10 min");
			format(string,sizeof(string),"%s~n~Cena: ~g~%i$~w~",string,price);
			format(string,sizeof(string),"%s~n~Oferujacy: ~g~%s~w~",string,pInfo[playerid][pName]);
		}
		case OFFER_TYPE_REPAIR_NON:
		{
			format(string,sizeof(string),"~r~Oferta ~>~ ~w~naprawa");
			format(string,sizeof(string),"%s~n~Cena: ~g~%i$~w~",string,price);
			format(string,sizeof(string),"%s~n~Oferujacy: ~g~%s~w~",string,pInfo[playerid][pName]);
		}
		case OFFER_TYPE_CARDEALER:
		{
			format(string,sizeof(string),"~r~Oferta ~>~ ~w~pojazd %s",VehicleData[OfferColor1[kupiec]][vName]);
			format(string,sizeof(string),"%s~n~Cena: ~g~%i$~w~",string,price);
			format(string,sizeof(string),"%s~n~Oferujacy: ~g~%s~w~",string,pInfo[playerid][pName]);
		}
		case OFFER_TYPE_PRZEJAZD:
		{
			format(string,sizeof(string),"~r~Oferta ~>~ ~w~przejazd");
			format(string,sizeof(string),"%s~n~Cena: ~g~%i$/100m~w~",string,price);
			format(string,sizeof(string),"%s~n~Oferujacy: ~g~%s~w~",string,pInfo[playerid][pName]);
		}
		case OFFER_TYPE_MONTAZ:
		{
			format(string,sizeof(string),"~r~Oferta ~>~ ~w~montaz");
			format(string,sizeof(string),"%s~n~Cena: ~g~%i$~w~",string,price);
			format(string,sizeof(string),"%s~n~Oferujacy: ~g~%s~w~",string,pInfo[playerid][pName]);
		}
		case OFFER_TYPE_MANDAT:
		{
			format(string,sizeof(string),"~r~Oferta ~>~ ~w~mandat");
			format(string,sizeof(string),"%s~n~Cena: ~g~%i$~w~",string,price);
			format(string,sizeof(string),"%s~n~Oferujacy: ~g~%s~w~",string,pInfo[playerid][pName]);
		}
		case OFFER_TYPE_UNBLOCK:
		{
			format(string,sizeof(string),"~r~Oferta ~>~ ~w~odblokowanie pojazdu ");
			format(string,sizeof(string),"%s~n~Cena: ~g~%i$~w~",string,price);
			format(string,sizeof(string),"%s~n~Oferujacy: ~g~%s~w~",string,pInfo[playerid][pName]);
		}
		case OFFER_TYPE_REJESTRACJA:
		{
			format(string,sizeof(string),"~r~Oferta ~>~ ~w~rejestracja ");
			format(string,sizeof(string),"%s~n~Cena: ~g~%i$~w~",string,price);
			format(string,sizeof(string),"%s~n~Oferujacy: ~g~%s~w~",string,pInfo[playerid][pName]);
		}
		case OFFER_TYPE_REPAINT:
		{
			format(string,sizeof(string),"~r~Oferta ~>~ ~w~lakierowanie %i/%i",OfferColor1[kupiec],OfferColor2[kupiec]);
			format(string,sizeof(string),"%s~n~Cena: ~g~%i$~w~",string,price);
			format(string,sizeof(string),"%s~n~Oferujacy: ~g~%s~w~",string,pInfo[playerid][pName]);
		}
		case OFFER_TYPE_PRAWKO:
		{
			// do dokonczenias
			switch(SoldItemUID[kupiec])
			{
				case 1:
				{
					format(string,sizeof(string),"~r~Oferta ~>~ ~w~prawo jazdy kat. A");
				}
				case 2:
				{
					format(string,sizeof(string),"~r~Oferta ~>~ ~w~prawo jazdy kat. B");
				}
				case 3:
				{
					format(string,sizeof(string),"~r~Oferta ~>~ ~w~prawo jazdy kat. C");
				}
				case 4:
				{
					format(string,sizeof(string),"~r~Oferta ~>~ ~w~prawo jazdy kat. C+E");
				}
				case 5:
				{
					format(string,sizeof(string),"~r~Oferta ~>~ ~w~prawo jazdy kat. D");
				}
			}
			//format(string,sizeof(string),"~r~Oferta ~>~ ~w~prawo jazdy");
			format(string,sizeof(string),"%s~n~Cena: ~g~%i$~w~",string,price);
			format(string,sizeof(string),"%s~n~Oferujacy: ~g~%s~w~",string,pInfo[playerid][pName]);
		}
		case OFFER_TYPE_CAR:
		{
			format(string,sizeof(string),"~r~Oferta ~>~ ~w~pojazd");
			if(price <= 0)
			{
				vlive_err(playerid,"nie poda³eœ ceny");
				return 1;
			}
			else
			{
				format(string,sizeof(string),"%s~n~Cena: ~g~%i$~w~",string,price);
				format(string,sizeof(string),"%s~n~Oferujacy: ~g~%s~w~",string,pInfo[playerid][pName]);
			}
		}
		case OFFER_TYPE_ITEM:
		{
			new itemid = 0;
			for(new i = 0; i < MAX_PLAYER_ITEMS; i++)
			{
				if(itemInfo[playerid][i][iUID] == SoldItemUID[kupiec])
				{
					itemid = i;
					break;
				}
			}
			format(string,sizeof(string),"~r~Oferta ~>~ ~w~przedmiot");
			format(string,sizeof(string),"%s~n~~r~Nazwa ~>~ ~w~%s",string,itemInfo[playerid][itemid][iName]);
			if(price <= 0)
			{
				vlive_err(playerid,"nie poda³eœ ceny");
				return 1;
			}
			else
			{
				format(string,sizeof(string),"%s~n~Cena: ~g~%i$~w~",string,price);
				format(string,sizeof(string),"%s~n~Oferujacy: ~g~%s~w~",string,pInfo[playerid][pName]);
			}
		}
		case OFFER_TYPE_HEALING:
		{
			if(price <= 0)
			{
				vlive_err(playerid,"nie poda³eœ ceny");
				return 1;
			}
			else
			{
				format(string,sizeof(string),"~r~Oferta ~>~ ~w~leczenie");
				format(string,sizeof(string),"%s~n~Cena: ~g~%i$~w~",string,price);
				format(string,sizeof(string),"%s~n~Oferujacy: ~g~%s~w~",string,pInfo[playerid][pName]);
			}
		}
		case OFFER_TYPE_REPAIR:
		{
			if(price <= 0)
			{
				vlive_err(playerid,"pojazd jest ca³kowicie naprawiony");
				return 1;
			}
			else
			{
				format(string,sizeof(string),"~r~Oferta ~>~ ~w~naprawa");
				format(string,sizeof(string),"%s~n~Cena: ~g~%i$~w~",string,price);
				format(string,sizeof(string),"%s~n~Oferujacy: ~g~%s~w~",string,pInfo[playerid][pName]);
			}
		}
	}
	
	TextDrawHideForPlayer(kupiec,TextDrawVehicleInfo[kupiec]);
	ShowPlayerDialog(kupiec,DIAL_OFFER,DIALOG_STYLE_MSGBOX,"Oferta","Czy akceptujesz t¹ ofertê?","Tak","Nie");
	TextDrawSetString(TextDrawOffers[kupiec],string);
	TextDrawShowForPlayer(kupiec, TextDrawOffers[kupiec]);
	GuiInfo(playerid,"Wys³a³eœ ofertê dla gracza, poczekaj na jego odpowiedŸ.");
	return 1;
}

//czy coœ jest oferowane temu graczowi
stock IsPlayerOffered(playerid)
{
	for(new i = 0; i < MAX_PLAYERS; i++)
	{
		if(Kupiec[i] == playerid)
		{
			return 1;
		}
	}
	return 0;
}

stock TransferVehicle(playerid,vehicleid)
{
	new owneruid = pInfo[playerid][pUID];
	new buffer[256];
	format(buffer,sizeof(buffer),"UPDATE vehicles_list SET veh_owner=%i WHERE veh_uid=%i",owneruid,vehicle[vehicleid][vuid]);
	mysql_query(buffer);
	vehicle[vehicleid][vowneruid] = owneruid;
}

stock TransferItem(playerid,itemuid)
{
	new owneruid = pInfo[playerid][pUID];
	new buffer[256];
	format(buffer,sizeof(buffer),"UPDATE core_items SET item_owneruid=%i AND item_whereis=%i WHERE item_uid=%i",owneruid,WHEREIS_GRACZ,itemuid);
	mysql_query(buffer);
}

forward Healing(playerid);
public Healing(playerid)
{
	if(pInfo[playerid][pHealth] < 100)
	{
		pInfo[playerid][pHealth] = pInfo[playerid][pHealth] + 5;
		SetPlayerHealth(playerid,pInfo[playerid][pHealth]);
		SetTimerEx("Healing", 3000, 0, "d", playerid);
	}
	else
	{
		pInfo[playerid][pHealth] = 100;
		SetPlayerHealth(playerid,100);
	}
}

forward PlayerRepairVehicle(playerid,vehicleid);
public PlayerRepairVehicle(playerid,vehicleid)
{
	if(RepairTime[playerid] > 0)
	{
		new Float:x,Float:y,Float:z;
		GetVehiclePos(vehicleid,x,y,z);
		if(IsPlayerInRangeOfPoint(playerid,5,x,y,z))
		{
			new output[64];
			format(output,sizeof(output),"do konca naprawy: ~g~%i sek",RepairTime[playerid]);
			GameTextForPlayer(playerid, output, 1000, 5);
			RepairTime[playerid]--;
			SetTimerEx("PlayerRepairVehicle", 1000, false, "ii", playerid,vehicleid);
		}
		else
		{
			new output[64];
			format(output,sizeof(output),"jestes zbyt daleko od pojazdu",RepairTime[playerid]);
			GameTextForPlayer(playerid, output, 1000, 5);
			SetTimerEx("PlayerRepairVehicle", 1000, false, "ii", playerid,vehicleid);
		}
	}
	else
	{
		new bufferr[256];
		RepairTime[playerid] = 0;
		PlayerCache[playerid][pRepairCar] = 0;
		format(bufferr,sizeof(bufferr),"UPDATE vehicles_list SET veh_hp=1000, dm_panels = 0, dm_doors = 0,dm_lights = 0, dm_tires = 0 WHERE veh_uid=%i",vehicle[vehicleid][vuid]);
		mysql_query(bufferr);
		vehicle[vehicleid][vhp] = 1000;
		vehicle[vehicleid][vdoors] = 0;
		vehicle[vehicleid][vtires] = 0;
		vehicle[vehicleid][vpanels] = 0;
		vehicle[vehicleid][vlights] = 0;
		SetVehicleHealth(vehicleid,1000);
		UpdateVehicleDamageStatus(vehicleid, 0, 0, 0, 0);
	}
}

forward PlayerRepaintVehicle(playerid,vehicleid,col1,col2,ticks);
public PlayerRepaintVehicle(playerid,vehicleid,col1,col2,ticks)
{
	if(IsPlayerConnect(playerid))
	{
		if(IsPlayerInRangeOfVehicle(playerid,vehicleid,10))
		{
			if(IsPlayerFacingVehicle(playerid,vehicleid))
			{
				if(PlayerCache[playerid][pHasLakier] > 0 && GetPlayerWeapon(playerid) == 41)
				{
					if(RepaintTime[playerid] > 0)
					{
						new keysa,uda,lra;
						GetPlayerKeys(playerid, keysa, uda, lra);
						if(keysa != KEY_FIRE)
						{
							SetTimerEx("PlayerRepaintVehicle", 1000, false, "iiiii", playerid,vehicleid,col1,col2,ticks);
						}
						else
						{
							ticks = PlayerCache[playerid][lakierTicks];
							new output[64];
							format(output,sizeof(output),"do konca lakierowania: ~g~%i sek",RepaintTime[playerid]);
							GameTextForPlayer(playerid, output, 1000, 5);
							RepaintTime[playerid]--;
							SetTimerEx("PlayerRepaintVehicle", 1000, false, "iiiii", playerid,vehicleid,col1,col2,ticks);
						}
					}
					else
					{
						RepaintTime[playerid] = 0;
						new buffer[256];
						format(buffer,sizeof(buffer),"UPDATE vehicles_list SET veh_col1 = %i, veh_col2 = %i WHERE veh_uid=%i",col1,col2,vehicle[vehicleid][vuid]);
						mysql_query(buffer);
						
						ChangeVehicleColor(vehicleid, col1, col2);
						
						PlayerCache[playerid][pLakieruje] = false;
						PlayerCache[playerid][lakierTicks] = 0;
						PlayerCache[playerid][lastLakierTick] = 0;
					}
				}
				else
				{
					//wstrzymaj zegar
					new output[64];
					format(output,sizeof(output),"wyciagnij lakier");
					GameTextForPlayer(playerid, output, 1000, 5);
					SetTimerEx("PlayerRepaintVehicle", 1000, false, "iiiii", playerid,vehicleid,col1,col2,ticks);
				}
			}
			else
			{
				//wstrzymaj zegar
				new output[64];
				format(output,sizeof(output),"celuj w pojazd");
				GameTextForPlayer(playerid, output, 1000, 5);
				SetTimerEx("PlayerRepaintVehicle", 1000, false, "iiiii", playerid,vehicleid,col1,col2,ticks);
			}
		}
		else
		{
			//wstrzymaj zegar
			new output[64];
			format(output,sizeof(output),"jestes zbyt daleko od pojazdu");
			GameTextForPlayer(playerid, output, 1000, 5);
			SetTimerEx("PlayerRepaintVehicle", 1000, false, "iiiii", playerid,vehicleid,col1,col2,ticks);
		}
	}
}

forward DorywczoRepairVehicle(playerid,vehicleid);
public DorywczoRepairVehicle(playerid,vehicleid)
{
	if(RepairTime[playerid] > 0)
	{
		new Float:x,Float:y,Float:z;
		GetVehiclePos(vehicleid,x,y,z);
		if(IsPlayerInRangeOfPoint(playerid,5,x,y,z))
		{
			new output[64];
			format(output,sizeof(output),"do konca naprawy: ~g~%i sek",RepairTime[playerid]);
			GameTextForPlayer(playerid, output, 1000, 5);
			RepairTime[playerid]--;
			SetTimerEx("DorywczoRepairVehicle", 1000, false, "ii", playerid,vehicleid);
		}
		else
		{
			new output[64];
			format(output,sizeof(output),"jestes zbyt daleko od pojazdu",RepairTime[playerid]);
			GameTextForPlayer(playerid, output, 1000, 5);
			SetTimerEx("DorywczoRepairVehicle", 1000, false, "ii", playerid,vehicleid);
		}
	}
	else
	{
		new bufferr[256];
		RepairTime[playerid] = 0;
		PlayerCache[playerid][pRepairCar] = 0;
		format(bufferr,sizeof(bufferr),"UPDATE vehicles_list SET veh_hp=1000 WHERE veh_uid=%i",vehicle[vehicleid][vuid]);
		mysql_query(bufferr);
		vehicle[vehicleid][vhp] = 1000;
		SetVehicleHealth(vehicleid,1000);
	}
}