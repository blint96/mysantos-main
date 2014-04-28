enum sSkinData
{
	sModel,
	sPrice,
	sName[24]
}
new SkinInfo[217][sSkinData] =
{
	{1, 40, "Flanelowa koszula"},
	{2, 60, "Bluza z kapeluszem"},
	{3, 95, "Firmowa marynarka"},
	{4, 30, "Brazowa koszulka"},
	{5, 35, "Brazowa koszulka xxl"},
	{6, 64, "Zielona kurtka"},
	{7, 68, "Kurtka jensowa"},
	{8, 70, "Strój mechanika 1"},
	{9, 50, "Ciemny ¿akiet"},
	{11, 95, "Strój elegancki"},
	{12, 110, "Czarna sukienka"},
	{13, 50, "Ciemny luzny stroj"},
	{14, 60, "Hawajska koszula"},
	{15, 35, "Koszula w krate"},
	{16, 200, "Czarny garnitur"},
	{18, 10, "Kapielowki"},
	{19, 55, "Czerwone spodnie"},
	{20, 50, "Zolta bluza"},
	{21, 60, "Granatowa koszula"},
	{22, 65, "Pomaranczowa bluza"},
	{23, 45, "Stroj skate"},
	{24, 50, "Granatowa bluza"},
	{25, 60, "Komplet Luz"},
	{28, 70, "Firmowa podkoszulka"},
	{29, 95, "Bluza z kapturem"},
	{30, 30, "Szara podkoszulka"},
	{32, 25, "Jensowa kurtka"},
	{33, 60, "Czarny p³aszcz"},
	{34, 75, "Stroj kowboja"},
	{35, 35, "Rozowa koszula"},
	{36, 25, "Biala koszulka"},
	{37, 30, "Stroj Golf"},
	{39, 25, "Stroj babciny"},
	{40, 105, "Czerwona sukienka"},
	{41, 80, "Blekitny dres"},
	{42, 60, "Stroj mechanika 2"},
	{43, 20, "Fioletowa koszulka"},
	{44, 45, "Koszulka w krate"},
	{45, 20, "Zielone bokserki"},
	{46, 110, "Elegancka koszula"},
	{47, 60, "Siwa koszula"},
	{48, 40, "Koszula w paski"},
	{49, 50, "Komplet Snake"},
	{50, 50, "Stroj mechanika 3"},
	{54, 40, "Swterek babuni"},
	{55, 75, "Sukienka i okulary"},
	{56, 65, "Komplet nastolatki"},
	{57, 150, "Elegancki garnitur 1"},
	{58, 30, "Bordowa koszula"},
	{59, 75, "Elegancka koszula"},
	{60, 55, "Komplet codzienny"},
	{62, 30, "Pizama nocna"},
	{63, 170, "Komplet prostytutki 1"},
	{64, 190, "Komplet prostytutki 2"},
	{65, 40, "Ubranie LUZ"},
	{66, 50, "Strój codzienny"},
	{67, 70, "Bluza z chusta"},
	{68, 50, "Stroj ksiedza"},
	{69, 40, "Niebieski komplet"},
	{72, 55, "Ubranie tradycyjne"},
	{73, 65, "Spodnie morro"},
	{75, 40, "Stroj wywloki"},
	{76, 55, "Zakiet granatowy"},
	{78, 10, "Komplet bezdomny 1"},
	{79, 10, "Komplet bezdomny 2"},
	{80, 90, "Komplet bokserski"},
	{81, 90, "Komplet bokserski"},
	{85, 120, "Komplet prostytutki 5"},
	{87, 90, "Komplet striptiz"},
	{90, 50, "Komplet Dres"},
	{91, 110, "Biala suknia"},
	{93, 70, "Stroj codzienny"},
	{94, 20, "Stroj dziadkowy"},
	{95, 30, "Jasna koszula"},
	{96, 55, "Stroj sportowy"},
	{97, 30, "Czerwone bokserki"},
	{98, 100, "Elegancki stroj"},
	{100, 80, "Stroj Harley"},
	{80, 75, "Komplet Standard"},
	{111, 150, "Komplet BLACK"},
	{112, 140, "Kurtka skorzana"},
	{113, 200, "Garnitur"},
	{119, 90, "Jasne spodnie"},
	{121, 60, "Komplet Army"},
	{122, 70, "Koszulka z chusta"},
	{124, 100, "Komplet LUZIO"},
	{128, 90, "Komplet HIPIS"},
	{130, 30, "Komplet babuszki"},
	{131, 50, "Stroj damski"},
	{132, 40, "Stroj farmera"},
	{133, 55, "Komplet DENO"},
	{134, 10, "Komplet bezdomnych"},
	{135, 15, "Komplet NIEOGAR"},
	{137, 5, "Stroj z kartonem"},
	{138, 15, "Komplet i okulary"},
	{139, 10, "Komplet"},
	{140, 10, "Komplet2"},
	{141, 90, "Stroj sluzbowy"},
	{142, 50, "Komplet kolorow"},
	{143, 85, "Blekitna bluza"},
	{144, 70, "Koszula z maska"},
	{146, 40, "Maseczka"},
	{147, 110, "Garnitur wyjsciowy"},
	{148, 90, "Zakiet zielony"},
	{150, 100, "Krotka sukienka"},
	{151, 70, "Sukienka jansowa"},
	{152, 200, "Stroj SEX"},
	{153, 100, "Garnitur z kaskiem"},
	{154, 5, "Galoty granatowe"},
	{155, 155, "Komplet PIZZA"},
	{156, 50, "Koszuleczka niebieska"},
	{157, 40, "Stroj FARM"},
	{158, 50, "Komplet FARM"},
	{159, 40, "Komplet FARMI"},
	{160, 20, "Komplet BIDA"},
	{161, 80, "Komplet KOWBOY"},
	{162, 5, "Spodnie JENS"},
	{165, 220, "Czarny garnitur N"},
	{167, 10, "Kogut"},
	{168, 30, "Fartuch"},
	{169, 95, "Czarna sukienka CHI"},
	{170, 80, "Komplet PIR"},
	{172, 90, "Komplet CES"},
	{176, 60, "Komplet BLUE"},
	{177, 60, "Komplet BLUE2"},
	{178, 300, "LATEX"},
	{179, 50, "Komplet MOR"},
	{180, 60, "Dluga koszulka"},
	{181, 70, "Komplet PUNK"},
	{182, 60, "Kurtka jasna"},
	{183, 70, "Ciemna krata"},
	{184, 60, "Ubranie LIZ"},
	{185, 80, "Koszula w pasy"},
	{186, 220, "Brazowy garnitur"},
	{187, 200, "Garnitur z krawatem"},
	{188, 50, "Zielona koszulka"},
	{189, 80, "Czerwona kamizelka"},
	{190, 40, "Getry i koszuleczka"},
	{191, 50, "Stroj zielony"},
	{192, 70, "Komplet GE"},
	{193, 60, "Dres i koszulka"},
	{194, 95, "Elegancja"},
	{198, 80, "Dam kowboy"},
	{200, 5, "Ogrodniczki"},
	{201, 20, "Komplet Fam-dam"},
	{202, 40, "Koszulka z czapka"},
	{203, 60, "Stroj Kimono"},
	{204, 60, "Stroj Kimono2"},
	{206, 40, "Komplet Cab"},
	{207, 60, "Komplet Gre"},
	{208, 100, "Ubranie Black"},	
	{299, 50, "Czarna kurtka ze skory"},
	{298, 25, "Bluzka na ramiaczkach"},
	{297, 70, "Komplet Madd-Dogg"},
	{296, 90, "Fioletowy Garnitur"},
	{295, 100, "Elegancki Garnitur"},
	{294, 120, "Stylowy garnitur"},
	{293, 20, "Br¹zowe Jeansy"},
	{292, 30, "Bia³y podkoszulek"},
	{291, 40, "Jeansowa kamizelka"},
	{290, 50, "Czarna marynarka"},
	{289, 35, "Zielona koszula"},
	{273, 25, "T-Shirt w paski"},
	{272, 40, "Brazowa kurtka"},
	{268, 60, "Ubranie robocze"},
	{263, 90, "Obcisla sukienka"},
	{262, 50, "Zielony sweterek"},
	{261, 60, "Koszula w krate"},
	{259, 60, "Niebieska koszula XXL"},
	{258, 60, "Szara koszula w krate"},
	{257, 80, "Komplet Nocne igraszki"},
	{256, 80, "Czarno-czerwona Mini"},
	{255, 90, "Garnitur dla szofera"},
	{254, 70, "Kamizelka ze skory"},
	{252, 15, "Bokserki na plaze"},
	{251, 20, "Stroj kapielowy"},
	{250, 35, "Bluzka z rekawami"},
	{249, 85, "Dlugi plaszcz ze skory"},
	{248, 90, "Komplet ze skory"},
	{246, 50, "Wyuzdana policjantka"},
	{245, 25, "Bluzko-biustonosz"},
	{244, 40, "Obiekt porzadania"},
	{243, 40, "Czerwona bluzka"},
	{242, 20, "Bialy podkoszulek XXL"},
	{241, 25, "Czarny T-shirt zapinany"},
	{240, 75, "Czarny bezrekawnik"},
	{239, 15, "Zaniedbana koszula"},
	{238, 70, "Obcisla sukienka"},
	{237, 40, "Bluzeczka Panterka"},
	{236, 25, "Koszula z T-Shirtem"},
	{235, 30, "Koszula z wzorkiem"},
	{234, 25, "Obcisly T-Shirt"},
	{233, 40, "Bialy sweterek"},
	{232, 20, "Bordowy kangurek"},
	{231, 20, "Jablkowy T-Shirt"},
	{230, 15, "Bluza z kapturem"},
	{229, 25, "Bialy T-shirt"},
	{228, 90, "Zwykly garnitur"},
	{227, 110, "Firmowy garnitur"},
	{226, 55, "Liliowa bluzka"},
	{225, 45, "Obcisla bluzka"},
	{224, 55, "Obcisly T-Shirt"},
	{223, 65, "Rozowa koszula"},
	{222, 30, "Luzna koszula"},
	{221, 25, "Biala koszulka Polo"},
	{220, 40, "Arabski T-Shirt"},
	{219, 95, "Damska marynarka"},
	{218, 30, "Rozowy sweter"},
	{217, 35, "Zwykly ciemny T-shirt"},
	{216, 75, "Biala sukienka"},
	{215, 65, "Zolta kurteczka"},
	{214, 125, "Komplet Afrodyta"},
	{213, 15, "Rock 'n Rollowiec"},
	{212, 15, "Uzywana koszulka"},
	{211, 55, "Bialo-czarny komplet"},
	{210, 45, "Kamizelka rybacka"},
	{209, 35, "Pracownik Fast-Food"}
};

new Text:TextDrawSkins[MAX_PLAYERS];
new SelectedSkin[MAX_PLAYERS];

stock LoadSkinsSystem()
{
	LoadSkinText();
}

stock LoadSkinText()
{
	for(new i=0; i < MAX_PLAYERS; i ++)
	{
		TextDrawSkins[i] = TextDrawCreate(450.000000, 350, "_");
		TextDrawAlignment(TextDrawSkins[i], 2);
		TextDrawBackgroundColor(TextDrawSkins[i], 255);
		TextDrawFont(TextDrawSkins[i], 1);
		TextDrawLetterSize(TextDrawSkins[i], 0.379999, 1.299999);
		TextDrawColor(TextDrawSkins[i], -1);
		TextDrawSetOutline(TextDrawSkins[i], 1);
		TextDrawSetProportional(TextDrawSkins[i], 1);
		TextDrawUseBox(TextDrawSkins[i], 1);
		TextDrawBoxColor(TextDrawSkins[i], 68);
		TextDrawTextSize(TextDrawSkins[i], 490.000000, 221.000000);
	}
}

COMMAND:ubranie(playerid,params[])
{
	new doorid = GetPlayerDoorID(playerid);
	if(DoorInfo[doorid][doorOwnerType] == DOOR_TYPE_UBRANIA)
	{
		new Float:PosX, Float:PosY, Float:PosZ;
		GetPlayerPos(playerid, PosX, PosY, PosZ);

		GetXYInFrontOfPlayer(playerid, PosX, PosY, 4.0);
		SetPlayerCameraPos(playerid, PosX, PosY, PosZ + 0.30);

		GetPlayerPos(playerid, PosX, PosY, PosZ);
		SetPlayerCameraLookAt(playerid, PosX, PosY, PosZ);
		
		
		new string[256];
		format(string,sizeof(string),"~r~ENTER~w~ by kupic wybrany skin~n~~r~LALT~w~ by przerwac wybieranie~n~Kliknij ~g~PPM ~w~aby przewijac");
		
		TextDrawSetString(TextDrawSkins[playerid], string);
		TextDrawShowForPlayer(playerid,TextDrawSkins[playerid]);
		
		SetPlayerSkin(playerid,SkinInfo[0][sModel]);
		SelectedSkin[playerid] = 0;
		TogglePlayerControllable(playerid,0);
		
		PlayerCache[playerid][pSelSkin] = true;
		
		return 1;
	}
	else
	{
		GuiInfo(playerid,"Nie jesteœ w sklepie z ubraniami...");
	}
	return 1;
}