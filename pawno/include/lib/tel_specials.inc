#define MAX_WORKSHOP_ITEMS 50

#define MAG_X	90.263961
#define MAG_Y	-305.207000
#define MAG_Z	1.578125

// czyli typ grupy który jest u¿ywany, przydatne
new WhatTypeOffer[MAX_PLAYERS];


enum pinfoo
{
	pItemVal1,
	pItemVal2,
	pItemPrice,
	pItemName[32],
	pType,
	pHowmuch,
}
new PurchasedItem[MAX_PLAYERS][pinfoo];

enum workshop_params
{
	wItemID,
	wItemName[32],
	wItemValue1,
	wItemValue2,				
	wItemCost,					//cena hurtowa
	wType,
}
new WorkshopItems[15][workshop_params] =
{
	{0,"Felgi Shadow",1073,0,1500,ITEM_COMPONENT },
	{1,"Felgi Mega",1074,0,1600,ITEM_COMPONENT },
	{2,"Felgi Rimshine",1075,0,1200,ITEM_COMPONENT },
	{3,"Felgi Wires",1076,0,1600,ITEM_COMPONENT },
	{4,"Felgi Classic",1077,0,2300,ITEM_COMPONENT },
	{5,"Felgi Twist",1078,0,2600,ITEM_COMPONENT },
	{6,"Felgi Cutter",1079,0,2400,ITEM_COMPONENT },
	{7,"Felgi Switch",1080,0,1700,ITEM_COMPONENT },
	{8,"Felgi Grove",1081,0,1950,ITEM_COMPONENT },
	{9,"Felgi Import",1082,0,2000,ITEM_COMPONENT },
	{10,"Felgi Dollar",1083,0,3100,ITEM_COMPONENT },
	{11,"Felgi Trance",1084,0,2100,ITEM_COMPONENT },
	{12,"Felgi Atomic",1085,0,1800,ITEM_COMPONENT },
	{13,"Lakier",41,5000,310,ITEM_LAKIER},
	{14,"Skrzynka z narzedziami",0,0,150,ITEM_NARZEDZIA}
};

enum restaurant_params
{
	rItemID,
	rItemName[32],
	rItemValue1,
	rItemValue2,
	rItemCost,
	rType
}
new RestaurantItems[12][restaurant_params] = 
{
	{0,"Piwo",0,0,4,ITEM_WINO},
	{1,"Wódka",0,0,15,ITEM_WINO},
	{2,"Whisky",0,0,30,ITEM_WINO},
	{3,"Woda",0,0,2,ITEM_SPRUNK},
	{4,"Sok",0,0,5,ITEM_SPRUNK},
	{5,"Spaghetti",4,0,12,ITEM_FOOD},
	{6,"Zupa",3,0,8,ITEM_FOOD},
	{7,"Kurczak z ziemniakami",5,0,15,ITEM_FOOD},
	{8,"Miêso wo³owe",4,0,13,ITEM_FOOD},
	{9,"Szarlotka",2,0,10,ITEM_FOOD},
	{10,"Cola",0,0,3,ITEM_SPRUNK},
	{11,"Makaron z serem",5,0,20,ITEM_FOOD}
};

enum lspd_params
{
	lItemID,
	lItemName[32],
	lItemValue1,
	lItemValue2,
	lItemCost,
	lType
}
new LspdItems[12][lspd_params]=
{
	{0,"Colt 9mm",22,34,200,ITEM_WEAPON},
	{1,"Amunicja Colt 9mm",22,34,50,ITEM_AMMO},
	{2,"Deagle",24,14,500,ITEM_WEAPON},
	{3,"Amunicja Deagle",24,28,100,ITEM_AMMO},
	{4,"Palka policyjna",3,1,20,ITEM_WEAPON},
	{5,"Shotgun",25,40,750,ITEM_WEAPON},
	{6,"Amunicja Shotgun",25,80,200,ITEM_AMMO},
	{7,"Gaz pieprzowy",41,5000,80,ITEM_WEAPON},
	{8,"Kamizelka",100,0,150,ITEM_KAMIZELKA},
	{9,"Megafon",0,0,50,ITEM_MEGAFON},
	{10,"Kajdanki policyjne",0,0,10,ITEM_KAJDANKI},
	{11,"Paralizator",22,1000,50,ITEM_PARALIZATOR}
};

enum lsfd_params
{
	fItemID,
	fItemName[32],
	fItemValue1,
	fItemValue2,
	fItemCost,
	fType
}
new FDItems[2][lsfd_params] = 
{
	{0,"Gasnica",42,5000,150,ITEM_WEAPON},
	{1,"Pila lancuchowa",9,1,2100,ITEM_WEAPON}
};

enum fbi_params
{
	bItemID,
	bItemName[32],
	bItemValue1,
	bItemValue2,
	bItemCost,
	bType
}
new FBIitems[13][fbi_params] = 
{
	{0,"Colt 9mm",22,34,200,ITEM_WEAPON},
	{1,"Amunicja Colt 9mm",22,34,50,ITEM_AMMO},
	{2,"Deagle",24,14,500,ITEM_WEAPON},
	{3,"Amunicja Deagle",24,28,100,ITEM_AMMO},
	{4,"Kamizelka",100,0,150,ITEM_KAMIZELKA},
	{5,"Megafon",0,0,50,ITEM_MEGAFON},
	{6,"Kajdanki policyjne",0,0,10,ITEM_KAJDANKI},
	{7,"Colt z tlumikiem",23,34,250,ITEM_WEAPON},
	{8,"Amunicja Colt(tlumik)",23,34,50,ITEM_AMMO},
	{9,"M4A1",31,500,5000,ITEM_WEAPON},
	{10,"M4A1 Amunicja",31,500,500,ITEM_AMMO},
	{11,"Wytrych",0,0,250,ITEM_WYTRYCH_DRZWI},
	{12,"Granat flashbang",16,10,1050,ITEM_FLASHBANG}
};

enum mafia_params
{
	mItemID,
	mItemName[32],
	mItemValue1,
	mItemValue2,
	mItemCost,
	mType
}
new MafiaItems[3][mafia_params] = 
{
	{0,"Colt 9mm",22,34,2000,ITEM_WEAPON},
	{1,"Amunicja Colt 9mm",22,34,800,ITEM_AMMO},
	{2,"Rekawiczki",0,0,800,ITEM_GLOVES}
	//{2,"Uzi",28,250,4500,ITEM_WEAPON},
	//{3,"Amunicja Uzi",28,250,2500,ITEM_AMMO},
	//{4,"Deagle",24,14,3500,ITEM_WEAPON},
	//{5,"Amunicja Deagle",24,28,1500,ITEM_AMMO},
	//{6,"AK47",30,500,10000,ITEM_WEAPON},
	//{7,"AK47 Amunicja",30,500,2500,ITEM_AMMO},
	//{8,"Tec 9",32,250,4500,ITEM_WEAPON},
	//{9,"Amunicja Tec 9",32,250,450,ITEM_AMMO},
	//{10,"Barrett M82A1",34,12,25000,ITEM_WEAPON},
	//{11,"Amunicja M82A1",34,12,25000,ITEM_AMMO}
};

enum ochroniarze_params
{
	oItemID,
	oItemName[32],
	oItemValue1,
	oItemValue2,
	oItemCost,
	oType
}
new OchroniarzeItems[4][ochroniarze_params] = 
{
	{0,"Paralizator",22,1000,500,ITEM_PARALIZATOR},
	{1,"Palka",3,1,230,ITEM_WEAPON},
	{2,"Gaz pieprzowy",41,5000,310,ITEM_WEAPON},
	{3,"Colt 9mm",22,34,2000,ITEM_WEAPON}
};

enum organizacje_params
{
	orgItemID,
	orgItemName[32],
	orgItemValue1,
	orgItemValue2,
	orgItemCost,
	orgType
}
new OrganizacjeItems[5][organizacje_params] = 
{
	{0,"Kij Baseballowy",5,1,300,ITEM_WEAPON},
	{1,"Kastet",1,1,400,ITEM_WEAPON},
	{2,"Amfetamina (10g)",10,0,1500,ITEM_AMFETAMINA},
	{3,"Nasiona marihuany",1,0,350, ITEM_SEEDS},			//zmienic na nasionka
	{4,"Maska",0,0,300, ITEM_MASK}
	//{2,"Colt 9mm",22,34,1500,ITEM_WEAPON},
	//{5,"Kokaina (10g)",10,0,500, ITEM_KOKAINA},
	//{6,"Tabletka LSD",1,0,190, ITEM_LSD}
};

enum sciganci_params
{
	sItemID,
	sItemName[64],
	sItemValue1,
	sItemValue2,
	sItemCost,
	sType
}
new SciganciItems[194][sciganci_params] = 
{
        {0,"Spoiler Pro",1000,0,2500,ITEM_COMPONENT},
        {1,"Spoiler Win",1001,0,2500,ITEM_COMPONENT},
        {2,"Spoiler Drag",1002,0,2500,ITEM_COMPONENT},
        {3,"Spoiler Alpha",1003,0,2500,ITEM_COMPONENT},
        {4,"Hood Champ Scoop",1004,0,1500,ITEM_COMPONENT},
        {5,"Hood Fury Scoop",1005,0,1500,ITEM_COMPONENT},
        {6,"Roof Scoop",1006,0,1500,ITEM_COMPONENT},
        {7,"Right Sideskirt",1007,0,1750,ITEM_COMPONENT},
        {8,"Nitro x5",1008,0,45000,ITEM_COMPONENT},
        {9,"Nitro x2",1009,0,20000,ITEM_COMPONENT},
        {10,"Nitro x10",1010,0,85000,ITEM_COMPONENT},
        {11,"Hood Race Scoop",1011,0,1500,ITEM_COMPONENT},
        {12,"Hood Worx Scoop",1012,0,1500,ITEM_COMPONENT},
        {13,"Lamps Round Fog",1013,0,1000,ITEM_COMPONENT},
        {14,"Spoiler Champ",1014,0,2500,ITEM_COMPONENT},
        {15,"Spoiler Race",1015,0,2500,ITEM_COMPONENT},
        {16,"Spoiler Worx",1016,0,2500,ITEM_COMPONENT},
        {17,"Left Sideskirt",1017,0,1750,ITEM_COMPONENT},
        {18,"Exhaust Upswept",1018,0,1250,ITEM_COMPONENT},
        {19,"Exhaust Twin",1019,0,1250,ITEM_COMPONENT},
        {20,"Exhaust Large",1020,0,1250,ITEM_COMPONENT},
        {21,"Exhaust Medium",1021,0,1250,ITEM_COMPONENT},
        {22,"Exhaust SMall",1022,0,1250,ITEM_COMPONENT},
        {23,"Spoiler Fury",1023,0,2500,ITEM_COMPONENT},
        {24,"Lamps Square Fog",1024,0,1000,ITEM_COMPONENT},
        {25,"Wheels Offroad",1025,0,900,ITEM_COMPONENT},
        {26,"Sultan Right Alien Sideskirt",1026,0,7500,ITEM_COMPONENT},
        {27,"Sultan Left Alien Sideskirt",1027,0,7500,ITEM_COMPONENT},
        {28,"Sultan Exhaust Alien",1028,0,3500,ITEM_COMPONENT},
        {29,"Sultan Exhaust X-Flow",1029,0,3500,ITEM_COMPONENT},
        {30,"Sultan Left X-Flow Sideskirt",1030,0,7500,ITEM_COMPONENT},
        {31,"Sultan Right X-Flow Sideskirt",1031,0,7500,ITEM_COMPONENT},
        {32,"Sultan Alien Roof Vent",1032,0,5000,ITEM_COMPONENT},
        {33,"Sultan X-Flow Roof Vent",1033,0,5000,ITEM_COMPONENT},
        {34,"Elegy Exhaust Alien",1034,0,3500,ITEM_COMPONENT},
        {35,"Elegy X-Flow Roof Vent",1035,0,5000,ITEM_COMPONENT},
        {36,"Elegy Right Alien Sideskirt",1036,0,7500,ITEM_COMPONENT},
        {37,"Elegy Exhaust X-Flow",1037,0,3500,ITEM_COMPONENT},
        {38,"Elegy Alien Roof Vent",1038,0,5000,ITEM_COMPONENT},
        {39,"Elegy Left X-Flow Sideskirt",1039,0,7500,ITEM_COMPONENT},
        {40,"Elegy Left Alien Sideskirt",1040,0,7500,ITEM_COMPONENT},
        {41,"Elegy Right X-Flow Sideskirt",1041,0,7500,ITEM_COMPONENT},
        {42,"Broadway Right Chrome Sideskirt",1042,0,6000,ITEM_COMPONENT},
        {43,"Broadway Exhaust Slamin",1043,0,4000,ITEM_COMPONENT},
        {44,"Broadway Exhaust Chrome",1044,0,4000,ITEM_COMPONENT},
        {45,"Flash Exhaust X-Flow",1045,0,3500,ITEM_COMPONENT},
        {46,"Flash Exhaust Alien",1046,0,3500,ITEM_COMPONENT},
        {47,"Flash Right Alien Sideskirt",1047,0,7500,ITEM_COMPONENT},
        {48,"Flash Right X-Flow Sideskirt",1048,0,7500,ITEM_COMPONENT},
        {49,"Flash Spoiler Alien",1049,0,6500,ITEM_COMPONENT},
        {50,"Flash Spoiler X-Flow",1050,0,6500,ITEM_COMPONENT},
        {51,"Flash Left Alien Sideskirt",1051,0,7500,ITEM_COMPONENT},
        {52,"Flash Left X-Flow Sideskirt",1052,0,7500,ITEM_COMPONENT},
        {53,"Flash Roof X-Flow",1053,0,5000,ITEM_COMPONENT},
        {54,"Flash Roof Alien",1054,0,5000,ITEM_COMPONENT},
        {55,"Stratum Roof Alien",1055,0,5000,ITEM_COMPONENT},
        {56,"Stratum Right Alien Sideskirt",1056,0,7500,ITEM_COMPONENT},
        {57,"Stratum Right X-Flow Sideskirt",1057,0,7500,ITEM_COMPONENT},
        {58,"Stratum Spoiler Alien",1058,0,6500,ITEM_COMPONENT},
        {59,"Stratum Exhaust X-Flow",1059,0,3500,ITEM_COMPONENT},
        {60,"Stratum Spoiler X-Flow",1060,0,6500,ITEM_COMPONENT},
        {61,"Stratum Roof X-Flow",1061,0,5000,ITEM_COMPONENT},
        {62,"Stratum Left Alien Sideskirt",1062,0,7500,ITEM_COMPONENT},
        {63,"Stratum Left X-Flow Sideskirt",1063,0,7500,ITEM_COMPONENT},
        {64,"Stratum Exhaust Alien",1064,0,3500,ITEM_COMPONENT},
        {65,"Jester Exhaust Alien",1065,0,3500,ITEM_COMPONENT},
        {66,"Jester Exhaust X-Flow",1066,0,3500,ITEM_COMPONENT},
        {67,"Jester Roof Alien",1067,0,5000,ITEM_COMPONENT},
        {68,"Jester Roof X-Flow",1068,0,5000,ITEM_COMPONENT},
        {69,"Jester Right Alien Sideskirt",1069,0,7500,ITEM_COMPONENT},
        {70,"Jester Right X-Flow Sideskirt",1070,0,7500,ITEM_COMPONENT},
        {71,"Jester Left Alien Sideskirt",1071,0,7500,ITEM_COMPONENT},
        {72,"Jester Left X-Flow Sideskirt",1072,0,7500,ITEM_COMPONENT},
        {73,"Wheels Shadow",1073,0,1700,ITEM_COMPONENT},
        {74,"Wheels Mega",1074,0,1900,ITEM_COMPONENT},
        {75,"Wheels Rimshine",1075,0,2100,ITEM_COMPONENT},
        {76,"Wheels Wires",1076,0,2200,ITEM_COMPONENT},
        {77,"Wheels Classic",1077,0,2400,ITEM_COMPONENT},
        {78,"Wheels Twist",1078,0,2300,ITEM_COMPONENT},
        {79,"Wheels Cutter",1079,0,2700,ITEM_COMPONENT},
        {80,"Wheels Switch",1080,0,3200,ITEM_COMPONENT},
        {81,"Wheels Grove",1081,0,2500,ITEM_COMPONENT},
        {82,"Wheels Import",1082,0,2000,ITEM_COMPONENT},
        {83,"Wheels Dollar",1083,0,3500,ITEM_COMPONENT},
        {84,"Wheels Trance",1084,0,2600,ITEM_COMPONENT},
        {85,"Wheels Atomic",1085,0,1800,ITEM_COMPONENT},
        {86,"Stereo",1086,0,8000,ITEM_COMPONENT},
        {87,"Hydraulics",1087,0,15000,ITEM_COMPONENT},
        {88,"Uranus Roof Alien",1088,0,5000,ITEM_COMPONENT},
        {89,"Uranus Exhaust X-Flow",1089,0,3500,ITEM_COMPONENT},
        {90,"Uranus Right Alien Sideskirt",1090,0,7500,ITEM_COMPONENT},
        {91,"Uranus Roof X-Flow",1091,0,5000,ITEM_COMPONENT},
        {92,"Uranus Exhaust Alien",1092,0,3500,ITEM_COMPONENT},
        {93,"Uranus Right X-Flow Sideskirt",1093,0,7500,ITEM_COMPONENT},
        {94,"Uranus Left Alien Sideskirt",1094,0,7500,ITEM_COMPONENT},
        {95,"Uranus Right X-Flow Sideskirt",1095,0,7500,ITEM_COMPONENT},
        {96,"Wheels Ahab",1096,0,1500,ITEM_COMPONENT},
        {97,"Wheels Virtual",1097,0,2800,ITEM_COMPONENT},
        {98,"Wheels Access",1098,0,2350,ITEM_COMPONENT},
        {99,"Broadway Left Chrome Sideskirt",1099,0,6000,ITEM_COMPONENT},
        {100,"Remington Chrome Grill",1100,0,4500,ITEM_COMPONENT},
        {101,"Remington Left Chrome Flames Sideskirt",1101,0,6000,ITEM_COMPONENT},
        {102,"Savanna Left Chrome Strip Sideskirt",1102,0,6000,ITEM_COMPONENT},
        {103,"Blade Roof Covertible",1103,0,10000,ITEM_COMPONENT},
        {104,"Blade Exhaust Chrome",1104,0,4000,ITEM_COMPONENT},
        {105,"Blade Exhaust Slamin",1105,0,4000,ITEM_COMPONENT},
        {106,"Remington Right Chrome Arches",1106,0,6000,ITEM_COMPONENT},
        {107,"Blade Left Chrome Strip Sideskirt",1107,0,6000,ITEM_COMPONENT},
        {108,"Blade Right Chrome Strip Sideskirt",1108,0,6000,ITEM_COMPONENT},
        {109,"Slamvan Rear Bullbars Chrome",1109,0,5000,ITEM_COMPONENT},
        {110,"Slamvan Rear Bullbars Slamin",1110,0,5000,ITEM_COMPONENT},
        {111,"Slamvan Front Sign? Little Sign?",1111,0,3500,ITEM_COMPONENT},
        {112,"Slamvan Front Sign? Little Sign?",1112,0,3500,ITEM_COMPONENT},
        {113,"Slamvan Exhaust Chrome",1113,0,4000,ITEM_COMPONENT},
        {114,"Slamvan Exhaust Slamin",1114,0,4000,ITEM_COMPONENT},
        {115,"Slamvan Front Bullbars Chrome",1115,0,5000,ITEM_COMPONENT},
        {116,"Slamvan Front Bullbars Slamin",1116,0,5000,ITEM_COMPONENT},
        {117,"Slamvan Front Bumper Chrome",1117,0,5000,ITEM_COMPONENT},
        {118,"Slamvan Right Chrome Trim Sideskirt",1118,0,6000,ITEM_COMPONENT},
        {119,"Slamvan Right Whellcovers Sideskirt",1119,0,6000,ITEM_COMPONENT},
        {120,"Slamvan Left Chrome Trim Sideskirt",1120,0,6000,ITEM_COMPONENT},
        {121,"Slamvan Left Whellcovers Sideskirt",1121,0,6000,ITEM_COMPONENT},
        {122,"Remington Right Chrome Flames Sideskirt",1122,0,6000,ITEM_COMPONENT},
        {123,"Remington Bullbar Chrome Bars",1123,0,4500,ITEM_COMPONENT},
        {124,"Remington Left Chrome Arches Sideskirt",1124,0,6000,ITEM_COMPONENT},
        {125,"Remington Bullbar Chrome Lights",1125,0,4500,ITEM_COMPONENT},
        {126,"Remington Exhaust Chrome",1126,0,4500,ITEM_COMPONENT},
        {127,"Remington Exhaust Slamin",1127,0,4500,ITEM_COMPONENT},
        {128,"Blade Roff Vinyl Hardtop",1128,0,10000,ITEM_COMPONENT},
        {129,"Savanna Exhaust Chrome",1129,0,4500,ITEM_COMPONENT},
        {130,"Savanna Roof Hardtop",1130,0,10000,ITEM_COMPONENT},
        {131,"Savanna Roof Softtop",1131,0,10000,ITEM_COMPONENT},
        {132,"Savanna Exhaust Slamin",1132,0,4500,ITEM_COMPONENT},
        {133,"Savanna Right Chrome Strip Sideskirt",1133,0,6000,ITEM_COMPONENT},
        {134,"Tornado Right Chrome Strip Sideskirt",1134,0,6000,ITEM_COMPONENT},
        {135,"Tornado Exhaust Slamin",1135,0,4500,ITEM_COMPONENT},
        {136,"Tornado Exhaust Chrome",1136,0,4500,ITEM_COMPONENT},
        {137,"Tornado Left Chrome Strip Sideskirt",1137,0,6000,ITEM_COMPONENT},
        {138,"Sultan Spoiler Alien",1138,0,6500,ITEM_COMPONENT},
        {139,"Sultan Spoiler X-Flow",1139,0,6500,ITEM_COMPONENT},
        {140,"Sultan Rear Bumper X-Flow",1140,0,10000,ITEM_COMPONENT},
        {141,"Sultan Rear Bumper Alien",1141,0,10000,ITEM_COMPONENT},
        {142,"Left Oval Vents",1142,0,2000,ITEM_COMPONENT},
        {143,"Right Oval Vents",1143,0,2000,ITEM_COMPONENT},
        {144,"Left Square Vents",1144,0,2000,ITEM_COMPONENT},
        {145,"Right Square Vents",1145,0,2000,ITEM_COMPONENT},
        {146,"Elegy Spoiler X-Flow",1146,0,6500,ITEM_COMPONENT},
        {147,"Elegy Spoiler Alien",1147,0,6500,ITEM_COMPONENT},
        {148,"Elegy Rear Bumper X-Flow",1148,0,10000,ITEM_COMPONENT},
        {149,"Elegy Rear Bumper Alien",1149,0,10000,ITEM_COMPONENT},
        {150,"Flash Rear Bumper Alien",1150,0,10000,ITEM_COMPONENT},
        {151,"Flash Rear Bumper X-Flow",1151,0,10000,ITEM_COMPONENT},
        {152,"Flash Front Bumper X-Flow",1152,0,10000,ITEM_COMPONENT},
        {153,"Flash Front Bumper Alien",1153,0,10000,ITEM_COMPONENT},
        {154,"Stratum Rear Bumper Alien",1154,0,10000,ITEM_COMPONENT},
        {155,"Stratum Front Bumper Alien",1155,0,10000,ITEM_COMPONENT},
        {156,"Stratum Rear Bumper X-Flow",1156,0,10000,ITEM_COMPONENT},
        {157,"Stratum Front Bumper X-Flow",1157,0,10000,ITEM_COMPONENT},
        {158,"Jester Spoiler X-Flow",1158,0,6500,ITEM_COMPONENT},
        {159,"Jester Rear Bumper Alien",1159,0,10000,ITEM_COMPONENT},
        {160,"Jester Front Bumper Alien",1160,0,10000,ITEM_COMPONENT},
        {161,"Jester Rear Bumper X-Flow",1161,0,10000,ITEM_COMPONENT},
        {162,"Jester Spoiler Alien",1162,0,6500,ITEM_COMPONENT},
        {163,"Uranus Spoiler X-Flow",1163,0,6500,ITEM_COMPONENT},
        {164,"Uranus Spoiler Alien",1164,0,6500,ITEM_COMPONENT},
        {165,"Uranus Front Bumper X-Flow",1165,0,10000,ITEM_COMPONENT},
        {166,"Uranus Front Bumper Alien",1166,0,10000,ITEM_COMPONENT},
        {167,"Uranus Rear Bumper X-Flow",1167,0,10000,ITEM_COMPONENT},
        {168,"Uranus Rear Bumper Alien",1168,0,10000,ITEM_COMPONENT},
        {169,"Sultan Front Bumper Alien",1169,0,10000,ITEM_COMPONENT},
        {170,"Sultan Front Bumper X-Flow",1170,0,10000,ITEM_COMPONENT},
        {171,"Elegy Front Bumper Alien",1171,0,10000,ITEM_COMPONENT},
        {172,"Elegy Front Bumper X-Flow",1172,0,10000,ITEM_COMPONENT},
        {173,"Jester Front Bumper X-Flow",1173,0,10000,ITEM_COMPONENT},
        {174,"Broadway Front Bumper Chrome",1174,0,5000,ITEM_COMPONENT},
        {175,"Broadway Front Bumper Slamin",1175,0,5000,ITEM_COMPONENT},
        {176,"Broadway Rear Bumper Chrome",1176,0,5000,ITEM_COMPONENT},
        {177,"Broadway Rear Bumper Slamin",1177,0,5000,ITEM_COMPONENT},
        {178,"Remington Rear Bumper Slamin",1178,0,5000,ITEM_COMPONENT},
        {179,"Remington Front Bumper Chrome",1179,0,5000,ITEM_COMPONENT},
        {180,"Remington Rear Bumper Chrome",1180,0,5000,ITEM_COMPONENT},
        {181,"Blade Front Bumper Slamin",1181,0,5000,ITEM_COMPONENT},
        {182,"Blade Front Bumper Chrome",1182,0,5000,ITEM_COMPONENT},
        {183,"Blade Rear Bumper Slamin",1183,0,5000,ITEM_COMPONENT},
        {184,"Blade Rear Bumper Chrome",1184,0,5000,ITEM_COMPONENT},
        {185,"Remington Front Bumper Slamin",1185,0,5000,ITEM_COMPONENT},
        {186,"Savanna Rear Bumper Slamin",1186,0,5000,ITEM_COMPONENT},
        {187,"Savanna Rear Bumper Chrome",1187,0,5000,ITEM_COMPONENT},
        {188,"Savanna Front Bumper Slamin",1188,0,5000,ITEM_COMPONENT},
        {189,"Savanna Front Bumper Chrome",1189,0,5000,ITEM_COMPONENT},
        {190,"Tornado Front Bumper Slamin",1190,0,5000,ITEM_COMPONENT},
        {191,"Tornado Front Bumper Chrome",1191,0,5000,ITEM_COMPONENT},
        {192,"Tornado Rear Bumper Chrome",1192,0,5000,ITEM_COMPONENT},
        {193,"Tornado Rear Bumper Slamin",1193,0,5000,ITEM_COMPONENT}
};

enum coffee_pars
{
	cItemID,
	cItemName[64],
	cItemValue1,
	cItemValue2,
	cItemCost,
	cType
}
new CoffeeItems[][coffee_pars] = 
{
	{0,"Wino",0,0,8,ITEM_WINO},
	{1,"Marihuana (10g)",10,0,500, ITEM_MARIHUANA},
	{2,"Ciastko z haszyszem",5,0,3,ITEM_FOOD},
	{3,"Lizak z haszyszem",3,0,2,ITEM_FOOD},
	{4,"Czekolada z haszyszem",7,0,5,ITEM_FOOD},
	{5,"Guma z haszyszem",2,0,1,ITEM_FOOD},
	{6,"Chrupki z haszyszem",7,0,5,ITEM_FOOD},
	{7,"Paluszki z haszyszem",4,0,3,ITEM_FOOD},
	{8,"Baton z haszyszem",3,0,2,ITEM_FOOD},
	{9,"Cola",0,0,3,ITEM_SPRUNK},
	{10,"Pepsi",0,0,3,ITEM_SPRUNK},
	{11,"Sprite",0,0,3,ITEM_SPRUNK},
	{12,"Fanta",0,0,3,ITEM_SPRUNK},
	{13,"Energetyk",0,0,3,ITEM_SPRUNK},
	{14,"Woda",0,0,3,ITEM_SPRUNK},
	{15,"Sok malinowy",0,0,3,ITEM_SPRUNK},
	{16,"Sok jablkowy",0,0,3,ITEM_SPRUNK},
	{17,"Sok pomaranczowy",0,0,3,ITEM_SPRUNK},
	{18,"Sok winogronowy",0,0,3,ITEM_SPRUNK},
	{19,"Woda z haszyszem",0,0,3,ITEM_SPRUNK},
	{20,"Jack Daniels",0,0,30,ITEM_WINO},
	{21,"Ballantines",0,0,30,ITEM_WINO},
	{22,"Finlandia",0,0,15,ITEM_WINO},
	{23,"Wyborowa",0,0,12,ITEM_WINO},
	{24,"Sobieski",0,0,10,ITEM_WINO},
	{25,"Krupnik",0,0,10,ITEM_WINO},
	{26,"Miodowo lipowe",0,0,3,ITEM_WINO},
	{27,"Spaghetti",5,0,10,ITEM_FOOD},
	{28,"Ryz z kurczakiem",5,0,10,ITEM_FOOD},
	{29,"Szarlotka",2,0,10,ITEM_FOOD},
	{30,"Frytki",3,0,5,ITEM_FOOD},
	{31,"Pizza",5,0,9,ITEM_FOOD},
	{32,"Bagietka",4,0,6,ITEM_FOOD}
};

enum aptekap
{
	aItemID,
	aItemName[32],
	aItemValue1,
	aItemValue2,
	aItemCost,
	aType
}
new AptekaItems[4][aptekap] = 
{
	{0,"Aspiryna",20,0,20,ITEM_FOOD},
	{1,"Syrop acodin",12,0,12,ITEM_FOOD},
	{2,"Witamina C",8,0,8,ITEM_FOOD},
	{3,"Pusta strzykawka",1,0,5,ITEM_EMPTY_STRZYKAWKA}
};

enum nvehssp
{
	Float:vx,
	Float:vy,
	Float:vz,
}
new NewVehiclePos[16][nvehssp] = 
{
	{838.588012 ,-1207.657470, 16.976562},
	{845.586730, -1208.207885, 16.976562},
	{853.809631 ,-1207.817993, 16.976562},
	{861.536621, -1208.396972 ,16.976562},
	{868.284057 ,-1209.216064 ,16.976562},
	{891.3287,-1208.8417,16.9766},
	{900.9679,-1209.9056,16.9832},
	{911.3836,-1209.7113,16.9832},
	{875.1639,-1268.5784,14.7785},
	{881.3693,-1268.8708,14.7389},
	{891.7123,-1268.9886,14.7496},
	{898.4431,-1268.9055,14.7499},
	{905.4354,-1268.8053,14.5911},
	{867.2003,-1296.6016,13.7403},
	{859.4458,-1296.6993,13.6607},
	{851.3958,-1296.6473,13.6035}
};

new CarryPackage[MAX_PLAYERS];			//uid wiezionej paczki

stock ShowRequestViaPhone(playerid)
{
	new doorid = GetPlayerDoorID(playerid);
	if(DoorInfo[doorid][doorOwnerType] == DOOR_TYPE_GROUP)
	{
		new guid = DoorInfo[doorid][doorOwnerUID];
		for(new i=1;i<MAX_PLAYER_GROUPS;i++)
		{
			if(pGrupa[playerid][i][pGuid] == guid)
			{
				if(pGrupa[playerid][i][permZamawianie] != 1)
				{
					GuiInfo(playerid,"Brak Ci odpowiednich uprawnieñ.");
					return 1;
				}
				//znalaz³a siê grupa o podanym uid
				new gType = 0;
				for(new groupid = 0; groupid < MAX_GROUPS; groupid++)
				{
					if(grupa[groupid][Guid] == guid)
					{
						gType = grupa[groupid][Gtype];
						break;
					}
				}
				// koniec fora
				if(gType == TYPE_NONE)
				{
					GuiInfo(playerid,"Twoja grupa jest bez okreœlonego typu, wiêc nie ma okreœlonych mo¿liwoœci zamawiania.");
					return 1;
				}
				if(gType == TYPE_WORKSHOP)
				{
					//warsztat
					new lista[512];
					format(lista, sizeof(lista), "Mo¿liwe do zamówienia produkty :\n» Felgi Shadow");	 
					strcat(lista, "\n» Felgi Mega", sizeof(lista));
					strcat(lista, "\n» Felgi Rimshine", sizeof(lista));
					strcat(lista, "\n» Felgi Wires", sizeof(lista));
					strcat(lista, "\n» Felgi Classic", sizeof(lista));
					strcat(lista, "\n» Felgi Twist", sizeof(lista));
					strcat(lista, "\n» Felgi Cutter", sizeof(lista));
					strcat(lista, "\n» Felgi Switch", sizeof(lista));
					strcat(lista, "\n» Felgi Grove", sizeof(lista));
					strcat(lista, "\n» Felgi Import", sizeof(lista));
					strcat(lista, "\n» Felgi Dollar", sizeof(lista));
					strcat(lista, "\n» Felgi Trance", sizeof(lista));
					strcat(lista, "\n» Felgi Atomic", sizeof(lista));
					strcat(lista, "\n» Lakier", sizeof(lista));
					strcat(lista, "\n» Skrzynka z narzedziami", sizeof(lista));
					
					WhatTypeOffer[playerid] = TYPE_WORKSHOP;
					
					ShowPlayerDialog(playerid,DIAL_ZAMAWIANIE,DIALOG_STYLE_LIST,"** Zamawianie w hurtowni **",lista,"Wybierz","Zamknij");
					return 1;
				}
				if(gType == TYPE_RESTAURANT)
				{
					//bary, restauracje itp
					new lista[512];
					format(lista, sizeof(lista), "Mo¿liwe do zamówienia produkty :\n» Piwo");	 
					strcat(lista, "\n» Wódka", sizeof(lista));
					strcat(lista, "\n» Whisky", sizeof(lista));
					strcat(lista, "\n» Woda", sizeof(lista));
					strcat(lista, "\n» Sok", sizeof(lista));
					strcat(lista, "\n» Spaghetti", sizeof(lista));
					strcat(lista, "\n» Zupa", sizeof(lista));
					strcat(lista, "\n» Kurczak z ziemniakami", sizeof(lista));
					strcat(lista, "\n» Miêso wo³owe", sizeof(lista));
					strcat(lista, "\n» Szarlotka", sizeof(lista));
					strcat(lista, "\n» Cocacola", sizeof(lista));
					strcat(lista, "\n» Makaron z serem", sizeof(lista));
					
					WhatTypeOffer[playerid] = TYPE_RESTAURANT;
					ShowPlayerDialog(playerid,DIAL_ZAMAWIANIE,DIALOG_STYLE_LIST,"** Zamawianie w hurtowni **",lista,"Wybierz","Zamknij");
					return 1;
				}
				if(gType == TYPE_CARDEALER)
				{
					GuiInfo(playerid,"Co mia³byœ zamawiaæ do tego typu biznesu?");
					return 1;
				}
				if(gType == TYPE_LSPD)
				{
					//bary, restauracje itp
					new lista[512];
					format(lista, sizeof(lista), "Mo¿liwe do zamówienia produkty :\n» Colt 9mm");	 
					strcat(lista, "\n» Amunicja Colt 9mm", sizeof(lista));
					strcat(lista, "\n» Deagle", sizeof(lista));
					strcat(lista, "\n» Amunicja Deagle", sizeof(lista));
					strcat(lista, "\n» Pa³ka policyjna", sizeof(lista));
					strcat(lista, "\n» Shotgun", sizeof(lista));
					strcat(lista, "\n» Amunicja Shotgun", sizeof(lista));
					strcat(lista, "\n» Gaz pieprzowy", sizeof(lista));
					strcat(lista, "\n» Kamizelka", sizeof(lista));
					strcat(lista, "\n» Megafon", sizeof(lista));
					strcat(lista, "\n» Kajdanki policyjne", sizeof(lista));
					strcat(lista, "\n» Paralizator", sizeof(lista));
					
					WhatTypeOffer[playerid] = TYPE_LSPD;
					ShowPlayerDialog(playerid,DIAL_ZAMAWIANIE,DIALOG_STYLE_LIST,"** Zamawianie w hurtowni **",lista,"Wybierz","Zamknij");
					return 1;
				}
				if(gType == TYPE_LSFD)
				{
					//bary, restauracje itp
					new lista[512];
					format(lista, sizeof(lista), "Mo¿liwe do zamówienia produkty :\n» Gasnica");	 
					strcat(lista, "\n» Pila lancuchowa", sizeof(lista));
					
					WhatTypeOffer[playerid] = TYPE_LSFD;
					ShowPlayerDialog(playerid,DIAL_ZAMAWIANIE,DIALOG_STYLE_LIST,"** Zamawianie w hurtowni **",lista,"Wybierz","Zamknij");
				}
				if(gType == TYPE_FBI)
				{
					//bary, restauracje itp
					new lista[512];
					format(lista, sizeof(lista), "Mo¿liwe do zamówienia produkty :\n» Colt 9mm");	 
					strcat(lista, "\n» Amunicja Colt 9mm", sizeof(lista));
					strcat(lista, "\n» Deagle", sizeof(lista));
					strcat(lista, "\n» Amunicja Deagle", sizeof(lista));
					strcat(lista, "\n» Kamizelka", sizeof(lista));
					strcat(lista, "\n» Megafon", sizeof(lista));
					strcat(lista, "\n» Kajdanki policyjne", sizeof(lista));
					strcat(lista, "\n» Colt 9mm z t³umikiem", sizeof(lista));
					strcat(lista, "\n» Amunicja Colt 9mm z t³umikiem", sizeof(lista));
					strcat(lista, "\n» M4A1", sizeof(lista));
					strcat(lista, "\n» Amunicja M4A1", sizeof(lista));
					strcat(lista, "\n» Wytrych", sizeof(lista));
					strcat(lista, "\n» Granat flashbang", sizeof(lista));
					
					WhatTypeOffer[playerid] = TYPE_FBI;
					ShowPlayerDialog(playerid,DIAL_ZAMAWIANIE,DIALOG_STYLE_LIST,"** Zamawianie w hurtowni **",lista,"Wybierz","Zamknij");
					return 1;
				}
				if(gType == TYPE_MAFIA)
				{
					//bary, restauracje itp
					new lista[512];
					format(lista, sizeof(lista), "Mo¿liwe do zamówienia produkty :\n» Colt 9mm");	 
					strcat(lista, "\n» Amunicja Colt 9mm", sizeof(lista));
					strcat(lista, "\n» Rêkawiczki", sizeof(lista));
			/*		strcat(lista, "\n» Amunicja Uzi", sizeof(lista));
					strcat(lista, "\n» Deagle", sizeof(lista));
					strcat(lista, "\n» Amunicja Deagle", sizeof(lista));
					strcat(lista, "\n» AK47", sizeof(lista));
					strcat(lista, "\n» Amunicja AK47", sizeof(lista));
					strcat(lista, "\n» Tec 9", sizeof(lista));
					strcat(lista, "\n» Amunicja Tec 9", sizeof(lista));
					strcat(lista, "\n» Barrett M82A1", sizeof(lista));
					strcat(lista, "\n» Amunicja M82A1", sizeof(lista));*/
					
					WhatTypeOffer[playerid] = TYPE_MAFIA;
					ShowPlayerDialog(playerid,DIAL_ZAMAWIANIE,DIALOG_STYLE_LIST,"** Zamawianie w hurtowni **",lista,"Wybierz","Zamknij");
				}
				if(gType == TYPE_ORG)
				{
					//bary, restauracje itp
					new lista[512];
					format(lista, sizeof(lista), "Mo¿liwe do zamówienia produkty :\n» Kij Baseballowy");	 
					strcat(lista, "\n» Kastet", sizeof(lista));
					strcat(lista, "\n» Amfetamina (10g)", sizeof(lista));
					strcat(lista, "\n» Marihuana (10g)", sizeof(lista));
					strcat(lista, "\n» Maska", sizeof(lista));
					//strcat(lista, "\n» Kokaina (10g)", sizeof(lista));
					//strcat(lista, "\n» Tabletka LSD", sizeof(lista));
					
					WhatTypeOffer[playerid] = TYPE_ORG;
					ShowPlayerDialog(playerid,DIAL_ZAMAWIANIE,DIALOG_STYLE_LIST,"** Zamawianie w hurtowni **",lista,"Wybierz","Zamknij");
				}
				if(gType == TYPE_OCHRONIARZE)
				{
					//bary, restauracje itp
					new lista[512];
					format(lista, sizeof(lista), "Mo¿liwe do zamówienia produkty :\n» Paralizator");	 
					strcat(lista, "\n» Palka", sizeof(lista));
					strcat(lista, "\n» Gaz pieprzowy", sizeof(lista));
					strcat(lista, "\n» Colt 9mm", sizeof(lista));
					
					WhatTypeOffer[playerid] = TYPE_OCHRONIARZE;
					ShowPlayerDialog(playerid,DIAL_ZAMAWIANIE,DIALOG_STYLE_LIST,"** Zamawianie w hurtowni **",lista,"Wybierz","Zamknij");
				}
				if(gType == TYPE_COFFESHOP)
				{
					//bary, restauracje itp
					new lista[512];
					format(lista, sizeof(lista), "Mo¿liwe do zamówienia produkty :\n» Wino");	 
					strcat(lista, "\n» Marihuana", sizeof(lista));
					strcat(lista, "\n» Ciastko z haszyszem", sizeof(lista));
					strcat(lista, "\n» Lizak z haszyszem", sizeof(lista));
					strcat(lista, "\n» Czekolada z haszyszem", sizeof(lista));
					strcat(lista, "\n» Guma z haszyszem", sizeof(lista));
					strcat(lista, "\n» Chrupki z haszyszem", sizeof(lista));
					strcat(lista, "\n» Paluszki z haszyszem", sizeof(lista));
					strcat(lista, "\n» Baton z haszyszem", sizeof(lista));
					strcat(lista, "\n» Cola", sizeof(lista));
					strcat(lista, "\n» Pepsi", sizeof(lista));
					strcat(lista, "\n» Sprite", sizeof(lista));
					strcat(lista, "\n» Fanta", sizeof(lista));
					strcat(lista, "\n» Energetyk", sizeof(lista));
					strcat(lista, "\n» Woda", sizeof(lista));
					strcat(lista, "\n» Sok malinowy", sizeof(lista));
					strcat(lista, "\n» Sok jablkowy", sizeof(lista));
					strcat(lista, "\n» Sok pomaranczowy", sizeof(lista));
					strcat(lista, "\n» Sok winogronowy", sizeof(lista));
					strcat(lista, "\n» Woda z haszyszem", sizeof(lista));
					strcat(lista, "\n» Jack Daniels", sizeof(lista));
					strcat(lista, "\n» Ballantines", sizeof(lista));
					strcat(lista, "\n» Finlandia", sizeof(lista));
					strcat(lista, "\n» Wyborowa", sizeof(lista));
					strcat(lista, "\n» Sobieski", sizeof(lista));
					strcat(lista, "\n» Krupnik", sizeof(lista));
					strcat(lista, "\n» Miodowo lipowe", sizeof(lista));
					strcat(lista, "\n» Spaghetti", sizeof(lista));
					strcat(lista, "\n» Ryz z kurczakiem", sizeof(lista));
					strcat(lista, "\n» Szarlotka", sizeof(lista));
					strcat(lista, "\n» Frytki", sizeof(lista));
					strcat(lista, "\n» Pizza", sizeof(lista));
					strcat(lista, "\n» Bagietka", sizeof(lista));
					
					WhatTypeOffer[playerid] = TYPE_COFFESHOP;
					ShowPlayerDialog(playerid,DIAL_ZAMAWIANIE,DIALOG_STYLE_LIST,"** Zamawianie w hurtowni **",lista,"Wybierz","Zamknij");
				}
				if(gType == TYPE_SCIGANT)
				{
					new lista[2048];
					format(lista, sizeof(lista), "Mo¿liwe do zamówienia produkty :\n» Spoiler Pro");
					strcat(lista, "\n» Spoiler Win", sizeof(lista));
					strcat(lista, "\n» Spoiler Drag", sizeof(lista));
					strcat(lista, "\n» Spoiler Alpha", sizeof(lista));
					strcat(lista, "\n» Hood Champ Scoop", sizeof(lista));
					strcat(lista, "\n» Hood Fury Scoop", sizeof(lista));
					strcat(lista, "\n» Roof Scoop", sizeof(lista));
					strcat(lista, "\n» Right Sideskirt", sizeof(lista));
					strcat(lista, "\n» Nitro x5", sizeof(lista));
					strcat(lista, "\n» Nitro x2", sizeof(lista));
					strcat(lista, "\n» Nitro x10", sizeof(lista));
					strcat(lista, "\n» Hood Race Scoop", sizeof(lista));
					strcat(lista, "\n» Hood Worx Scoop", sizeof(lista));
					strcat(lista, "\n» Lamps Round Fog", sizeof(lista));
					strcat(lista, "\n» Spoiler Champ", sizeof(lista));
					strcat(lista, "\n» Spoiler Race", sizeof(lista));
					strcat(lista, "\n» Spoiler Worx", sizeof(lista));
					strcat(lista, "\n» Left Sideskirt", sizeof(lista));
					strcat(lista, "\n» Exhaust Upswept", sizeof(lista));
					strcat(lista, "\n» Exhaust Twin", sizeof(lista));
					strcat(lista, "\n» Exhaust Large", sizeof(lista));
					strcat(lista, "\n» Exhaust Medium", sizeof(lista));
					strcat(lista, "\n» Exhaust SMall", sizeof(lista));
					strcat(lista, "\n» Spoiler Fury", sizeof(lista));
					strcat(lista, "\n» Lamps Square Fog", sizeof(lista));
					strcat(lista, "\n» Wheels Offroad", sizeof(lista));
					strcat(lista, "\n» Sultan Right Alien Sideskirt", sizeof(lista));
					strcat(lista, "\n» Sultan Left Alien Sideskirt", sizeof(lista));
					strcat(lista, "\n» Sultan Exhaust Alien", sizeof(lista));
					strcat(lista, "\n» Sultan Exhaust X-Flow", sizeof(lista));
					strcat(lista, "\n» Sultan Left X-Flow Sideskirt", sizeof(lista));
					strcat(lista, "\n» Sultan Right X-Flow Sideskirt", sizeof(lista));
					strcat(lista, "\n» Sultan Alien Roof Vent", sizeof(lista));
					strcat(lista, "\n» Sultan X-Flow Roof Vent", sizeof(lista));
					strcat(lista, "\n» Elegy Exhaust Alien", sizeof(lista));
					strcat(lista, "\n» Elegy X-Flow Roof Vent", sizeof(lista));
					strcat(lista, "\n» Elegy Right Alien Sideskirt", sizeof(lista));
					strcat(lista, "\n» Elegy Exhaust X-Flow", sizeof(lista));
					strcat(lista, "\n» Elegy Alien Roof Vent", sizeof(lista));
					strcat(lista, "\n» Elegy Left X-Flow Sideskirt", sizeof(lista));
					strcat(lista, "\n» Elegy Left Alien Sideskirt", sizeof(lista));
					strcat(lista, "\n» Elegy Right X-Flow Sideskirt", sizeof(lista));
					strcat(lista, "\n» Broadway Right Chrome Sideskirt", sizeof(lista));
					strcat(lista, "\n» Broadway Exhaust Slamin", sizeof(lista));
					strcat(lista, "\n» Broadway Exhaust Chrome", sizeof(lista));
					strcat(lista, "\n» Flash Exhaust X-Flow", sizeof(lista));
					strcat(lista, "\n» Flash Exhaust Alien", sizeof(lista));
					strcat(lista, "\n» Flash Right Alien Sideskirt", sizeof(lista));
					strcat(lista, "\n» Flash Right X-Flow Sideskirt", sizeof(lista));
					strcat(lista, "\n» Flash Spoiler Alien", sizeof(lista));
					strcat(lista, "\n» Flash Spoiler X-Flow", sizeof(lista));
					strcat(lista, "\n» Flash Left Alien Sideskirt", sizeof(lista));
					strcat(lista, "\n» Flash Left X-Flow Sideskirt", sizeof(lista));
					strcat(lista, "\n» Flash Roof X-Flow", sizeof(lista));
					strcat(lista, "\n» Flash Roof Alien", sizeof(lista));
					strcat(lista, "\n» Stratum Roof Alien", sizeof(lista));
					strcat(lista, "\n» Stratum Right Alien Sideskirt", sizeof(lista));
					strcat(lista, "\n» Stratum Right X-Flow Sideskirt", sizeof(lista));
					strcat(lista, "\n» Stratum Spoiler Alien", sizeof(lista));
					strcat(lista, "\n» Stratum Exhaust X-Flow", sizeof(lista));
					strcat(lista, "\n» Stratum Spoiler X-Flow", sizeof(lista));
					strcat(lista, "\n» Stratum Roof X-Flow", sizeof(lista));
					strcat(lista, "\n» Stratum Left Alien Sideskirt", sizeof(lista));
					strcat(lista, "\n» Stratum Left X-Flow Sideskirt", sizeof(lista));
					strcat(lista, "\n» Stratum Exhaust Alien", sizeof(lista));
					strcat(lista, "\n» Jester Exhaust Alien", sizeof(lista));
					strcat(lista, "\n» Jester Exhaust X-Flow", sizeof(lista));
					strcat(lista, "\n» Jester Roof Alien", sizeof(lista));
					strcat(lista, "\n» Jester Roof X-Flow", sizeof(lista));
					strcat(lista, "\n» Jester Right Alien Sideskirt", sizeof(lista));
					strcat(lista, "\n» Jester Right X-Flow Sideskirt", sizeof(lista));
					strcat(lista, "\n» Jester Left Alien Sideskirt", sizeof(lista));
					strcat(lista, "\n» Jester Left X-Flow Sideskirt", sizeof(lista));
					strcat(lista, "\n» Wheels Shadow", sizeof(lista));
					strcat(lista, "\n» Wheels Mega", sizeof(lista));
					strcat(lista, "\n» Wheels Rimshine", sizeof(lista));
					strcat(lista, "\n» Wheels Wires", sizeof(lista));
					strcat(lista, "\n» Wheels Classic", sizeof(lista));
					strcat(lista, "\n» Wheels Twist", sizeof(lista));
					strcat(lista, "\n» Wheels Cutter", sizeof(lista));
					strcat(lista, "\n» Wheels Switch", sizeof(lista));
					strcat(lista, "\n» Wheels Grove", sizeof(lista));
					strcat(lista, "\n» Wheels Import", sizeof(lista));
					strcat(lista, "\n» Wheels Dollar", sizeof(lista));
					strcat(lista, "\n» Wheels Trance", sizeof(lista));
					strcat(lista, "\n» Wheels Atomic", sizeof(lista));
					strcat(lista, "\n» Stereo", sizeof(lista));
					strcat(lista, "\n» Hydraulics", sizeof(lista));
					strcat(lista, "\n» Uranus Roof Alien", sizeof(lista));
					strcat(lista, "\n» Uranus Exhaust X-Flow", sizeof(lista));
					strcat(lista, "\n» Uranus Right Alien Sideskirt", sizeof(lista));
					strcat(lista, "\n» Uranus Roof X-Flow", sizeof(lista));
					strcat(lista, "\n» Uranus Exhaust Alien", sizeof(lista));
					strcat(lista, "\n» Uranus Right X-Flow Sideskirt", sizeof(lista));
					strcat(lista, "\n» Uranus Left Alien Sideskirt", sizeof(lista));
					strcat(lista, "\n» Uranus Right X-Flow Sideskirt", sizeof(lista));
					strcat(lista, "\n» Wheels Ahab", sizeof(lista));
					strcat(lista, "\n» Wheels Virtual", sizeof(lista));
					strcat(lista, "\n» Wheels Access", sizeof(lista));
					strcat(lista, "\n» Broadway Left Chrome Sideskirt", sizeof(lista));
					strcat(lista, "\n» Remington Chrome Grill", sizeof(lista));
					strcat(lista, "\n» Remington Left Chrome Flames Sideskirt", sizeof(lista));
					strcat(lista, "\n» Savanna Left Chrome Strip Sideskirt", sizeof(lista));
					strcat(lista, "\n» Blade Roof Covertible", sizeof(lista));
					strcat(lista, "\n» Blade Exhaust Chrome", sizeof(lista));
					strcat(lista, "\n» Blade Exhaust Slamin", sizeof(lista));
					strcat(lista, "\n» Remington Right Chrome Arches", sizeof(lista));
					strcat(lista, "\n» Blade Left Chrome Strip Sideskirt", sizeof(lista));
					strcat(lista, "\n» Blade Right Chrome Strip Sideskirt", sizeof(lista));
					strcat(lista, "\n» Slamvan Rear Bullbars Chrome", sizeof(lista));
					strcat(lista, "\n» Slamvan Rear Bullbars Slamin", sizeof(lista));
					strcat(lista, "\n» Slamvan Front Sign? Little Sign?", sizeof(lista));
					strcat(lista, "\n» Slamvan Front Sign? Little Sign?", sizeof(lista));
					strcat(lista, "\n» Slamvan Exhaust Chrome", sizeof(lista));
					strcat(lista, "\n» Slamvan Exhaust Slamin", sizeof(lista));
					strcat(lista, "\n» Slamvan Front Bullbars Chrome", sizeof(lista));
					strcat(lista, "\n» Slamvan Front Bullbars Slamin", sizeof(lista));
					strcat(lista, "\n» Slamvan Front Bumper Chrome", sizeof(lista));
					strcat(lista, "\n» Slamvan Right Chrome Trim Sideskirt", sizeof(lista));
					strcat(lista, "\n» Slamvan Right Whellcovers Sideskirt", sizeof(lista));
					strcat(lista, "\n» Slamvan Left Chrome Trim Sideskirt", sizeof(lista));
					strcat(lista, "\n» Slamvan Left Whellcovers Sideskirt", sizeof(lista));
					strcat(lista, "\n» Remington Right Chrome Flames Sideskirt", sizeof(lista));
					strcat(lista, "\n» Remington Bullbar Chrome Bars", sizeof(lista));
					strcat(lista, "\n» Remington Left Chrome Arches Sideskirt", sizeof(lista));
					strcat(lista, "\n» Remington Bullbar Chrome Lights", sizeof(lista));
					strcat(lista, "\n» Remington Exhaust Chrome", sizeof(lista));
					strcat(lista, "\n» Remington Exhaust Slamin", sizeof(lista));
					strcat(lista, "\n» Blade Roff Vinyl Hardtop", sizeof(lista));
					strcat(lista, "\n» Savanna Exhaust Chrome", sizeof(lista));
					strcat(lista, "\n» Savanna Roof Hardtop", sizeof(lista));
					strcat(lista, "\n» Savanna Roof Softtop", sizeof(lista));
					strcat(lista, "\n» Savanna Exhaust Slamin", sizeof(lista));
					strcat(lista, "\n» Savanna Right Chrome Strip Sideskirt", sizeof(lista));
					strcat(lista, "\n» Tornado Right Chrome Strip Sideskirt", sizeof(lista));
					strcat(lista, "\n» Tornado Exhaust Slamin", sizeof(lista));
					strcat(lista, "\n» Tornado Exhaust Chrome", sizeof(lista));
					strcat(lista, "\n» Tornado Left Chrome Strip Sideskirt", sizeof(lista));
					strcat(lista, "\n» Sultan Spoiler Alien", sizeof(lista));
					strcat(lista, "\n» Sultan Spoiler X-Flow", sizeof(lista));
					strcat(lista, "\n» Sultan Rear Bumper X-Flow", sizeof(lista));
					strcat(lista, "\n» Sultan Rear Bumper Alien", sizeof(lista));
					strcat(lista, "\n» Left Oval Vents", sizeof(lista));
					strcat(lista, "\n» Right Oval Vents", sizeof(lista));
					strcat(lista, "\n» Left Square Vents", sizeof(lista));
					strcat(lista, "\n» Right Square Vents", sizeof(lista));
					strcat(lista, "\n» Elegy Spoiler X-Flow", sizeof(lista));
					strcat(lista, "\n» Elegy Spoiler Alien", sizeof(lista));
					strcat(lista, "\n» Elegy Rear Bumper X-Flow", sizeof(lista));
					strcat(lista, "\n» Elegy Rear Bumper Alien", sizeof(lista));
					strcat(lista, "\n» Flash Rear Bumper Alien", sizeof(lista));
					strcat(lista, "\n» Flash Rear Bumper X-Flow", sizeof(lista));
					strcat(lista, "\n» Flash Front Bumper X-Flow", sizeof(lista));
					strcat(lista, "\n» Flash Front Bumper Alien", sizeof(lista));
					strcat(lista, "\n» Stratum Rear Bumper Alien", sizeof(lista));
					strcat(lista, "\n» Stratum Front Bumper Alien", sizeof(lista));
					strcat(lista, "\n» Stratum Rear Bumper X-Flow", sizeof(lista));
					strcat(lista, "\n» Stratum Front Bumper X-Flow", sizeof(lista));
					strcat(lista, "\n» Jester Spoiler X-Flow", sizeof(lista));
					strcat(lista, "\n» Jester Rear Bumper Alien", sizeof(lista));
					strcat(lista, "\n» Jester Front Bumper Alien", sizeof(lista));
					strcat(lista, "\n» Jester Rear Bumper X-Flow", sizeof(lista));
					strcat(lista, "\n» Jester Spoiler Alien", sizeof(lista));
					strcat(lista, "\n» Uranus Spoiler X-Flow", sizeof(lista));
					strcat(lista, "\n» Uranus Spoiler Alien", sizeof(lista));
					strcat(lista, "\n» Uranus Front Bumper X-Flow", sizeof(lista));
					strcat(lista, "\n» Uranus Front Bumper Alien", sizeof(lista));
					strcat(lista, "\n» Uranus Rear Bumper X-Flow", sizeof(lista));
					strcat(lista, "\n» Uranus Rear Bumper Alien", sizeof(lista));
					strcat(lista, "\n» Sultan Front Bumper Alien", sizeof(lista));
					strcat(lista, "\n» Sultan Front Bumper X-Flow", sizeof(lista));
					strcat(lista, "\n» Elegy Front Bumper Alien", sizeof(lista));
					strcat(lista, "\n» Elegy Front Bumper X-Flow", sizeof(lista));
					strcat(lista, "\n» Jester Front Bumper X-Flow", sizeof(lista));
					strcat(lista, "\n» Broadway Front Bumper Chrome", sizeof(lista));
					strcat(lista, "\n» Broadway Front Bumper Slamin", sizeof(lista));
					strcat(lista, "\n» Broadway Rear Bumper Chrome", sizeof(lista));
					strcat(lista, "\n» Broadway Rear Bumper Slamin", sizeof(lista));
					strcat(lista, "\n» Remington Rear Bumper Slamin", sizeof(lista));
					strcat(lista, "\n» Remington Front Bumper Chrome", sizeof(lista));
					strcat(lista, "\n» Remington Rear Bumper Chrome", sizeof(lista));
					strcat(lista, "\n» Blade Front Bumper Slamin", sizeof(lista));
					strcat(lista, "\n» Blade Front Bumper Chrome", sizeof(lista));
					strcat(lista, "\n» Blade Rear Bumper Slamin", sizeof(lista));
					strcat(lista, "\n» Blade Rear Bumper Chrome", sizeof(lista));
					strcat(lista, "\n» Remington Front Bumper Slamin", sizeof(lista));
					strcat(lista, "\n» Savanna Rear Bumper Slamin", sizeof(lista));
					strcat(lista, "\n» Savanna Rear Bumper Chrome", sizeof(lista));
					strcat(lista, "\n» Savanna Front Bumper Slamin", sizeof(lista));
					strcat(lista, "\n» Savanna Front Bumper Chrome", sizeof(lista));
					strcat(lista, "\n» Tornado Front Bumper Slamin", sizeof(lista));
					strcat(lista, "\n» Tornado Front Bumper Chrome", sizeof(lista));
					strcat(lista, "\n» Tornado Rear Bumper Chrome", sizeof(lista));
					strcat(lista, "\n» Tornado Rear Bumper Slamin", sizeof(lista));
					
					WhatTypeOffer[playerid] = TYPE_SCIGANT;
					ShowPlayerDialog(playerid,DIAL_ZAMAWIANIE,DIALOG_STYLE_LIST,"** Zamawianie w hurtowni **",lista,"Wybierz","Zamknij");
				}
				//koniec skryptu
				break;
			}
		}
	}
	return 0;
}

COMMAND:kurier(playerid,params[])
{
	if(pInfo[playerid][pCarryPackage] == 0)
	{
		SetPlayerCheckpoint(playerid, MAG_X, MAG_Y, MAG_Z, 3.0);
		pInfo[playerid][pCarryPackage] = 1;
		GuiInfo(playerid,"Udaj siê we wskazane miejsce by odebraæ paczkê.");
		vlive_err(playerid,"kiedy bêdziesz ju¿ w magazynie u¿yj komendy /paczka");
	}
	else
	{
		pInfo[playerid][pCarryPackage] = 0;
		GuiInfo(playerid,"Anulowa³eœ przejazd.");
		CarryPackage[playerid] = 0;
		DisablePlayerCheckpoint(playerid);
	}
	return 1;
}

COMMAND:dostarcz(playerid,params[])
{
	if(pInfo[playerid][pCarryPackage] > 0)
	{
		if(CarryPackage[playerid] > 0)
		{
			if(IsPlayerInCheckpoint(playerid))
			{
				new uid = CarryPackage[playerid];
				new buffer[256];
				format(buffer,sizeof(buffer),"SELECT val1,val2,name,type,doorid,howmuch FROM purchases WHERE uid=%i",uid);
				mysql_query(buffer);
				mysql_store_result();
				new val1,val2,name[32],type,doorid,howmuch;
				while(mysql_fetch_row(buffer,"|"))
				{
					sscanf(buffer,"p<|>iis[32]iii",val1,val2,name,type,doorid,howmuch);				
				}
				mysql_free_result();
				
				format(buffer,256,"INSERT INTO core_items (item_name,item_value1,item_value2,item_type,item_owneruid,item_whereis,item_worldid) VALUES ('%s',%i,%i,%i,%i,%i,%i)",name,val1,val2,type,doorid,WHEREIS_SCHOWEK,howmuch);
				mysql_query(buffer);
				
				format(buffer,sizeof(buffer),"DELETE FROM purchases WHERE uid=%i",uid);
				mysql_query(buffer);
				
				CarryPackage[playerid] = 0;
				pInfo[playerid][pCarryPackage] = 0;
				
				GuiInfo(playerid,"Dostarczy³eœ paczkê.");
				
				//dodaj kase graczowi
				new Float:distance = GetPlayerDistanceFromPoint(playerid,MAG_X,MAG_Y,MAG_Z);
				new dist_int = floatround(distance);
				dist_int = dist_int/30;		//w metrach
				pInfo[playerid][pMoney] = pInfo[playerid][pMoney] + dist_int;
				
				format(buffer,256,"UPDATE core_players SET char_money=%i WHERE char_uid=%i",pInfo[playerid][pMoney],GetPlayerUID(playerid));
				mysql_query(buffer);
				
				DisablePlayerCheckpoint(playerid);
			}
			else
			{
				GuiInfo(playerid,"Nie dotar³es na miejsce.");
			}
		}
	}
	return 1;
}

COMMAND:paczka(playerid,params[])
{
	if (IsPlayerInCheckpoint(playerid))
	{
		new buffer[256],lista[1024];
		format(buffer,256,"SELECT uid,doorid FROM purchases");
		mysql_query(buffer);
		mysql_store_result();
		if(mysql_num_rows() > 0)
		{
			while(mysql_fetch_row(buffer,"|"))
			{
				new uid,dooruid,doorname[64];
				sscanf(buffer,"p<|>ii",uid,dooruid);
				new doorid = 0;
				for(new i=0;i < MAX_DOORS; i++)
				{
					if(DoorInfo[i][doorUID] == dooruid)
					{
						doorid = i;
						break;
					}
				}
				format(doorname,64,"%s",DoorInfo[doorid][doorName]);
				format(lista,sizeof(lista),"%s\n%i\t\t%s",lista,uid,doorname);
			}
			mysql_free_result();
			
			ShowPlayerDialog(playerid,DIAL_MAGAZYN,DIALOG_STYLE_LIST,"Magazyn",lista,"Wybierz","Zamknij");
		}
		else
		{
			mysql_free_result();
			GuiInfo(playerid,"Magazyn jest pusty, nie ma obecnie ¿adnych paczek do rozwiezienia.");
		}
	}
	else
	{
		GuiInfo(playerid,"Nie jesteœ w magazynie..");
	}
	return 1;
}