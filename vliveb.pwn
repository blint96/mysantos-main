/*
	mySantos.pl - pierwotny vLive.pl autorstwa blinta, skrypt przystosowany do wspó³pracy z IP.Board 3.4.5
	
	Last update: 18 marzec 2014 r.
	
	Warto poprawiæ b³êdy wyrzucane podczas korzystania ze schowka, dodatkowo mo¿e byæ jakiœ b³¹d
	przy timerach u gracza, server_log siê cholernie zasypuje; mapa na chodzie! :)
*/

#include <a_samp>
#include <mysql>
#include <sscanf2>
#include <vlive_kolorki>
#include <zcmd>
#include <streamer>
#include <mSelection>
#include <audio>
#include <md5>
#include <onplayershootplayer>
#include <progress>		//nowe hudy
#include <speedcap>
#include <fly>

#undef MAX_PLAYERS
#define MAX_PLAYERS 100

#define ENGINE_BASE_COST 0
#define ENGINE_MODIFY_COST 2400
#define ENGINE_SPORT_COST 8000

#define BENEFIT_AMOUNT 150


//
// -- Define -- //
// »
//
//		- warningi przy wyci¹ganiu ze schowka...
//		- czyszczenie Days z bazy last_pd na koniec miesiaca


//	Error CODE : 7374 - wiêcej ni¿ jeden telefon w u¿yciu
//  Error CODE : 4046 - wiêcej itemów w ekwipunku ni¿ dopuszczalne
//


#define DIAL_P 1		// cmd : /p
#define DIAL_L 3		// logowanie na serwer
#define DIAL_PUSE 4		// u¿ywanie itema z p
#define DIAL_PP 6		//podnoszenie itemów
#define DIAL_NOTEPAD 7	//tworzenie notatki
#define DIAL_G	8	//lista grup
#define DIAL_V 9	//lista pojazdow
#define DIAL_VSEL 10	//wybrany pojazd
#define DIAL_AVMENU 11			//admin gui vehicles
#define DIAL_ACP 12				//acp
#define DIAL_AVOLIST 13 		//lista bezpanskich aut
#define DIAL_AVACTION 14    // akcja na wybrtanym
#define DIAL_UNSPAWN 15	//dial unspawnowania
#define DIAL_GUSE 16
#define DIAL_SG	18			//selectgroup
#define DIAL_RG 19				//rejestracja in game
#define DIAL_RGC 20			//tworzenie
#define DIAL_O	21				//dial oferowania
#define DIAL_PHONE 22			//telefon
#define DIAL_STATS 23
#define DIAL_CONTACTS 24		//kontakty
#define DIAL_APRZEDMIOT 25
#define DIAL_VAL1	26
#define DIAL_VAL2	27
#define DIAL_NAME	28
#define DIAL_OWNER	29
#define DIAL_PICKUP 30
#define DIAL_BANKOMAT 31
#define DIAL_BKWYPLAC	32
#define DIAL_BKWPLAC		33
#define DIAL_SCHOWEK 34
#define DIAL_911 35
#define DIAL_911SEL	36
#define DIAL_ZAMAWIANIE	37
#define DIAL_ZAMSEL	38
#define DIAL_MAGAZYN	39
#define DIAL_MAGWYJMIJ	40
#define DIAL_ILE 41
#define DIAL_KOKPIT 42
#define DIAL_G_TYPE 43
#define DIAL_PURCHASE_CLOATH	44
#define DIAL_HELP	45
#define DIAL_BANK	46
#define DIAL_BANK_PRZELEW_ILOSC	47
#define DIAL_BANK_WPLAC	48
#define DIAL_BANK_WYPLAC	49
#define DIAL_BANK_PRZELEW	50
#define DIAL_HELP_SEL	51
#define DIAL_DOOR_OPTIONS	52
#define DIAL_DOOR_SET_ENTER	53
#define DIAL_DOOR_SET_MUZYKA	54
#define DIAL_DOOR_SET_NAME	55
#define DIAL_OFFER	56
#define DIAL_BUS	57
#define DIAL_DOOR_SET_PRZEJAZD	58
#define DIAL_AS 59
#define DIAL_KUP 60
#define DIAL_OPIS	61
#define DIAL_OPIS_SELECT 62
#define DIAL_OPIS_LISTA 63
#define DIAL_OPIS_DELETE 64
#define DIAL_OPIS_CREATE	65
#define DIAL_OPIS_CREATE_2	66
#define DIAL_SET_SPAWN	67
#define DIAL_TUNING	68
#define DIAL_SHOW_GROUPS	69
#define DIAL_ATTACH_MENU	70
#define DIAL_PURCHASE_ATTACH	71
#define DIAL_PURCHASE_LSPD_SPECIALS	72
#define DIAL_GANJA_NIEDOJRZALA	73
#define DIAL_LIST_VEH_ITEMS	74
#define DIAL_VEH_ITEM_SELECTED	75
#define DIAL_SEL_STARTED 76
#define DIAL_MODDING_VEHICLE	77
#define DIAL_SHOW_HOUSE_MENU 78
#define DIAL_HOUSE_INVITES 79
#define DIAL_HOUSE_UNINVITE 80
#define DIAL_HOUSE_LISTING 81
#define DIAL_HOUSE_SELECT_SPAWN 82
#define DIAL_STYLE 83
#define DIAL_CICON 84
#define DIAL_SELECT_WEIGHT 85
#define DIAL_STYLE_STATS 86
#define DIAL_KUP_APTEKA 87


//wersje, kilka ustawieñ
#define VERSION "1.3.8"				//wersja skryptu
#define SERVERNAME "mySantos"	//nazwa serwera
#define MYBB_TABLE "mysa_members"					//tabela uzytkownikow IP.Board
#define REGISTER_IN_GAME 0		// rejestracja InGame
#define KEY_AIM KEY_HANDBRAKE


// limity
#define MAX_PLAYER_ITEMS 50
#define MAX_DROPPED_ITEMS 1000
#define MAX_OBJECTS2 3000
#define MAX_PLAYER_GROUPS 6
#define MAX_ANIMATIONS      200
#define MAX_BANKOMATS 100
#define MAX_PETROLS 100
#define MAX_LOGIN_TRY 3
#define MAX_GROUPS 700
#define MAX_BLOCKADES	10

#define OFFER_TYPE_UNBLOCK	10
#define OFFER_TYPE_PRZEJAZD	11
#define OFFER_TYPE_MONTAZ	12
#define OFFER_TYPE_REPAIR_NON	13
#define OFFER_TYPE_KARNET 14
#define OFFER_TYPE_BITEM 15

#define CAR_DELAY -1
#define CZAS_BW 10
#define PETROL_COST_PER_LITER	2			//dwa dolce za litra benzyny


#define GROUP_TYPE_RUBBISH_UID 58
#define GROUP_TYPE_PIZZA_UID 109


//uprawnienia
#define PERMS_PROGRAMMER 5
#define PERMS_ADMIN 4
#define PERMS_GAMEMASTER 3
#define PERMS_SUPPORT 2
#define PERMS_HELPER 1 
#define PEMRS_PLAYER 0


//obiekty przyczepialne
#define SLOT_CUFFS 1
#define SLOT_WEAPON 2
#define SLOT_PHONE 3
#define SLOT_PLAYERONE 4
#define SLOT_PLAYERTWO 5
#define SLOT_CACHE 6
#define SLOT_CHUJWDUPIE	7
#define SLOT_TEST	8
#define SLOT_GYM 9


//kary
#define BLOCK_OOC 1
#define BLOCK_VEHICLES 1
#define BLOCK_CHAR 1
#define BLOCK_BAN 1


// MySQL settings
#define M_HOST ""
#define M_LOGIN ""
#define M_PASS ""
#define M_DB ""

// -- DEFINE ITEMTYPES -- //
#define ITEM_NONE 0				//brak
#define ITEM_WEAPON 1			//broñ
#define ITEM_FOOD 2				//¿arcie
#define ITEM_AMMO 3				//amunicja
#define ITEM_NOTEPAD 4			//notatnik
#define ITEM_PC 5				//komputer (?)
#define ITEM_PHONE 6			//telefon							(val1 = numer, val2 = w³/wy³)
#define ITEM_PAPERSHEET 7		//kartka z notatnika
#define ITEM_CLOCK 8			//zegarek
#define ITEM_CASH 9				//kasa
#define ITEM_WINO 10			//picie
#define ITEM_CLOTH 11			//ubranie
#define ITEM_KANISTER 12		//kanister
#define ITEM_MASK 13			//maska
#define ITEM_COMPONENT 14		//czêœci do tuningu auta 			(value1 = model komponentu)
#define ITEM_KAJDANKI 15		//no kajdanki
#define ITEM_JOINT 16			//joincik
#define ITEM_BLETKA 17			//bletki
#define ITEM_ATTACH 18			//przyczepialne
#define ITEM_DRUG 19 			//narkotyki
#define ITEM_STRZYKAWA 20		//strzykawka dla narkusów
#define ITEM_PAPIEROS	21
#define ITEM_SPRUNK	22				//picie ogolnie
#define ITEM_KAMIZELKA	23			//kamizelka kuloodporna
#define ITEM_MEGAFON 24
#define ITEM_PARALIZATOR 25
#define ITEM_CORPSE	30
#define ITEM_TORBA	31
#define ITEM_LAKIER		32
#define ITEM_SEEDS	33	
#define ITEM_SPECIALWEAP	34	//no te specjalne bronie
#define ITEM_NARZEDZIA	35
#define ITEM_KARNET 36
#define ITEM_EMPTY_STRZYKAWKA 37
#define ITEM_FLASHBANG 38
#define ITEM_WYTRYCH_DRZWI 39
#define ITEM_SWAT_CAMERA 40
#define ITEM_GLOVES 41

//narkotyki
#define ITEM_MARIHUANA 26
#define ITEM_AMFETAMINA 27
#define ITEM_KOKAINA 28
#define ITEM_LSD	29


// -- GUI COLORS -- //
#define COL_GRAY "{8C8C8C}"
#define COL_GREEN "{00EB00}"
#define COL_USE "{0000FC}" // niebieski
#define COL_WHITE "{FFFFFF}"	//bialy
#define COL_GREEN2 "{00F000}" 
#define COL_GRAY2 "{C7C7C7}" 		//szary
#define COL_RED "{FF0000}"
#define COL_SERV "{00B2E7}"
#define COL_SUPPORT "{ABBDEB}"
#define COL_GM	"{00BF00}"
#define COL_DUTY "{2E5AA6}"
#define COL_EQ "{5E5EEB}"
#define COL_ORANGE "{FF9900}"

#define SEX_MALE 1
#define SEX_FEMALE 2

#define SPAWN_TYPE_CITY	0
#define SPAWN_TYPE_HOUSE	1

#define SPAWN_TYPE_METRO	3
#define SPAWN_TYPE_IDLEWOOD	4

//logowanie hajsu
#define MONEY_SEND	1
#define MONEY_TAKE	2
#define MONEY_PLAYER_FOR_PLAYER	1
#define MONEY_GROUP_FOR_PLAYER 2
#define MONEY_PLAYER_FOR_GROUP	3

#define SPAWN_IDLE_X 1920.0217
#define SPAWN_IDLE_Y -1763.5162
#define SPAWN_IDLE_Z 13.5391

#define SW_ROTX	94.9
#define SW_ROTY -92
#define SW_ROTZ 0
#define SW_POSX 0.061999
#define SW_BONE 6

new debugMode = 0;
new RunningTime = 0;
new testingMode = 0;


// -- Tablice na graczy/pojazdy/grupy/frakcje -- //

enum playerParams
{
	pUID,			//unikalny id z bazy
	pSID,			//samp id
	pName[32],		//imiê gracza
	pGID,		//uid globalnego konta
	pOnline,		//czy jest online
	pLogged,
	Float:pHealth,		//ilosc hp
	pMoney,			//ilosc kasy
	pSkin,			//skin
	bool:pAFK,		//czy jest afk
	Float:pX,		//posx
	Float:pY,		//posy
	Float:pZ,		//posz
	Float:pA,		//player angle	
	pBlock,
	pBw,
	pAj,
	blockVehicles,
	blockOOC,
	blockChar,
	pFreeze,
	pCuffed,			//czy jest skuty
	pCuffPlayer,		//czy sku³ kogoœ
	pPhoneInUse,
	pBank,				//ile kasy w banku
	pWarns,			//liczba warnów
	pGamescore,		//gsy
	pWiek,				//wiek postaci
	pSex,				//plec
	pBankomat,		// czy uzywal bankomat
	pBankNR,			//numer konta bankowego
	pMasked,			//czy zamaskowany
	pTogW,				//czy w³¹czone MSG
	pSpectate,			//czy specuje
	pAdmin,
	pHasDowod,		//czy ma dowód osobisty
	pHasPrawkoA,	//motory
	pHasPrawkoB,	//auto
	pHasPrawkoC,	//ciezarowe
	pHasPrawkoCE,	//plus przyczepa i ciezarowe
	pHasPrawkoD,	//autobus
	pCarryPackage,		//czy paczke wiezie
	pFirstLogin,			//póki co nieu¿ywane
	pWorld,
	pInterior,
	pSpawnType,
	Float:pCondition,			//kondycja gracza
	Float:maxCondition,
	pSeconds,
	pVehMask,
	pPremium,	//czy ma on konto premium
	pGMCars,
	pGMDoors,
	pGMGroups,
	pGMItems,
	Float:pPower,
	fightingStyle,
	pSpecPossible,
	Float:pShootskill,
}
new pInfo[MAX_PLAYERS][playerParams];

enum binfo
{
	bkUID,
	Float:bkX,
	Float:bkY,
	Float:bkZ,
	Float:bkVW,
}
new bankomat[MAX_BANKOMATS][binfo];

enum pinfo
{
	pUID,
	Float:peX,
	Float:peY,
	Float:peZ,
	peVW,
}
new petrol[MAX_PETROLS][pinfo];

enum sAnimData
{
	aUID,
	
	aCommand[12],
	aLib[16],
	aName[24],
	
	Float:aSpeed,
	
	aOpt1,
	aOpt2,
	aOpt3,
	aOpt4,
	aOpt5,
	
	aAction
}
new AnimInfo[MAX_ANIMATIONS][sAnimData];

enum pgpars
{
	pGuid,
	pCharuid,
	pRank[32],
	pPayday,
	pSkin,
	pPerms[32],
	pDutytime,
	pGname[32],
	permDoors,
	permLeader,
	permVehicles,
	permOoc,
	permOffers,
	permZamawianie,
}
new pGrupa[MAX_PLAYERS][MAX_PLAYER_GROUPS][pgpars];

enum gpars
{
	Guid,
	Gname[64],
	Gtype,
	Gmoney,
	Gcolor,
	Gtag[6],
}
new grupa[MAX_GROUPS][gpars];

enum itemParams
{
	iUID,
	iOwner,
	iName[64],
	iValue1,
	iValue2,
	iType,
	Float:iX,
	Float:iY,
	Float:iZ,
	iObject,
	iWhereis,
	iUsed,
	iWorldid,
}
new itemInfo[MAX_PLAYERS][MAX_PLAYER_ITEMS][itemParams];

enum pcache
{
	Float:pCX,
	Float:pCY,
	Float:pCZ,
	bool:pPlayAnim,
	playerUnmask[MAX_PLAYER_NAME],
	
	Float:pSpecX,
	Float:pSpecY,
	Float:pSpecZ,
	pSpecClose,
	pSpecVW,
	bool:pSelSkin,
	pWeaponID,			//id broni
	pWeaponAmmo,		//ammo tej borni
	
	Float:pDeadX,
	Float:pDeadY,
	Float:pDeadZ,
	pHasParalyzer,
	
	pAudioHandle,
	
	pEditLabel,
	
	pHasInterview,			//czy koleœ jest teraz w wywiadzie
	pRepairCar,
	
	pStringVal[128],			//pomocnicza wartoœæ string
	pHasLakier,
	pVehCheckpoint,
	
	pColourCar,				//czy maluje auto
	
	pEnterDoors,
	
	bool:pAfk,
	bool:pLakieruje,
	lakierTicks,
	lastLakierTick,
	
	pIPChecked,
	
	pZP,			//zapiete pasy
	pMetersRide,
	bool:pTaxed,
	
	Float:takX,
	Float:takY,
	Float:takZ,
	
	pTaxedDriver,
	pTaxedPrice,
	
	pRubbishDrive,
	bool:pRubbishCheckpoint,
	
	pMaskedNumbers,
	pGroupListPage,
	
	pSelectedAttachment,
	pEditAttachment,
	
	pBuyAttach,
	pSelAttach,
	
	pSelPlant,
	
	pHited,
	
	pShootedWeapon,
	pShootedUID,
	pShootedType,
	
	pCacheAccess,
	
	pSelStarted,
	pHasSpecialWeapon,	//specjalna bron
	pSpecialWeaponDamage,
	
	pSelectedLokator,
	pSelectedHouse,
	
	pArea,
	
	pTrain,
	pTrainTime,
	
	pGymFaza,
	pGymType,
	pGymVal1,
	pGymVal2,
	Float:pGymPowerPerTick,
	pGymHantleType,
	
	pKarnet,
	pKarnetTime,
	pKarnetBiznes,
	
	pDoorsDuty,
	
	pPizzaTicks,
	
	pOnlineSec,
	
	Float:pBonusPower,
	
	pUseSWATCam,
	
	pGPS,
	
	pGPSCP,
	
	pGloves,
	
	pHidden,
	
	pCornered,
	pCornerID,
	pCornerTime,
}
new PlayerCache[MAX_PLAYERS][pcache];

enum p_pars
{
	attachmentItemUid,
}
new PlayerAttachment[2][p_pars];

enum sVehicleData
{
	vModel,
	vName[32],
	vMaxSpeed,
	vPrice,
	vFuel,
}
new VehicleData[212][sVehicleData] = {
    {
        400, "Landstalker", 140, 12000, 90
    }, {
        401, "Bravura", 131, 6700, 60
    }, {
        402, "Buffalo", 166, 85000, 70
    }, {
        403, "Linerunner", 98, 45000, 190
    }, {
        404, "Pereniel", 118, 4100, 50
    }, {
        405, "Sentinel", 146, 26000, 70
    }, {
        406, "Dumper", 98, 50000, 290
    }, {
        407, "Firetruck", 132, 50000, 300
    }, {
        408, "Trashmaster", 89, 50000, 300
    }, {
        409, "Stretch", 140, 67000, 100
    }, {
        410, "Manana", 115, 3700, 40
    }, {
        411, "Infernus", 197, 400000, 90
    }, {
        412, "Voodoo", 150, 14000, 60
    }, {
        413, "Pony", 98, 9400, 120
    }, {
        414, "Mule", 94, 17000, 210
    }, {
        415, "Cheetah", 171, 350000, 70
    }, {
        416, "Ambulance", 137, 50000, 130
    }, {
        417, "Leviathan", 399, 1000000, 300
    }, {
        418, "Moonbeam", 103, 5400, 100
    }, {
        419, "Esperanto", 133, 4700, 60
    }, {
        420, "Taxi", 129, 21000, 100
    }, {
        421, "Washington", 137, 100000, 90
    }, {
        422, "Bobcat", 124, 6800, 85
    }, {
        423, "Mr Whoopee", 88, 45000, 100
    }, {
        424, "BF Injection", 120, 34000, 40
    }, {
        425, "Hunter", 399, 1000000, 300
    }, {
        426, "Premier", 154, 25000, 90
    }, {
        427, "Enforcer", 147, 1000000, 100
    }, {
        428, "Securicar", 139, 65000, 200
    }, {
        429, "Banshee", 179, 210000, 75
    }, {
        430, "Predator", 399, 1000000, 100
    }, {
        431, "Bus", 116, 60000, 200
    }, {
        432, "Rhino", 84, 2000000, 400
    }, {
        433, "Barracks", 98, 200000, 210
    }, {
        434, "Hotknife", 148, 100000, 90
    }, {
        435, "Trailer", 0, 0, 0
    }, {
        436, "Previon", 133, 3200, 50
    }, {
        437, "Coach", 140, 55000, 290
    }, {
        438, "Cabbie", 127, 19000, 90
    }, {
        439, "Stallion", 150, 12000, 60
    }, {
        440, "Rumpo", 121, 4900, 100
    }, {
        441, "RC Bandit", 67, 0, 0
    }, {
        442, "Romero", 124, 43000, 90
    }, {
        443, "Packer", 112, 67000, 190
    }, {
        444, "Monster Truck A", 98, 0, 290
    }, {
        445, "Admiral", 146, 14000, 70
    }, {
        446, "Squalo", 399, 190000, 200
    }, {
        447, "Seasparrow", 399, 430000, 200
    }, {
        448, "Pizzaboy", 162, 1200, 9
    }, {
        449, "Tram", 399, 0, 0
    }, {
        450, "Trailer", 399, 0, 0
    }, {
        451, "Turismo", 172, 1000000, 90
    }, {
        452, "Speeder", 399, 230000, 200
    }, {
        453, "Reefer", 399, 21000, 200
    }, {
        454, "Tropic", 399, 94000, 200
    }, {
        455, "Flatbed", 140, 60000, 200
    }, {
        456, "Yankee", 94, 23000, 190
    }, {
        457, "Caddy", 84, 9000, 20
    }, {
        458, "Solair", 140, 7300, 50
    }, {
        459, "Berkley's RC Van", 121, 0, 0
    }, {
        460, "Skimmer", 399, 34000, 200
    }, {
        461, "PCJ-600", 180, 16000, 30
    }, {
        462, "Faggio", 155, 900, 7
    }, {
        463, "Freeway", 180, 8400, 25
    }, {
        464, "RC Baron", 399, 0, 0
    }, {
        465, "RC Raider", 399, 0, 0
    }, {
        466, "Glendale", 131, 5100, 45
    }, {
        467, "Oceanic", 125, 8100, 60
    }, {
        468, "Sanchez", 164, 10000, 40
    }, {
        469, "Sparrow", 399, 275000, 200
    }, {
        470, "Patriot", 139, 1000000, 200
    }, {
        471, "Quad", 98, 6500, 30
    }, {
        472, "Coastguard", 399, 430000, 200
    }, {
        473, "Dinghy", 399, 3100, 20
    }, {
        474, "Hermes", 133, 9000, 60
    }, {
        475, "Sabre", 154, 14500, 70
    }, {
        476, "Rustler", 399, 90000, 200
    }, {
        477, "ZR-350", 166, 65000, 100
    }, {
        478, "Walton", 105, 4300, 90
    }, {
        479, "Regina", 124, 5100, 80
    }, {
        480, "Comet", 164, 210000, 95
    }, {
        481, "BMX", 86, 600, 0
    }, {
        482, "Burrito", 139, 8500, 120
    }, {
        483, "Camper", 109, 3200, 70
    }, {
        484, "Marquis", 399, 340000, 200
    }, {
        485, "Baggage", 88, 40000, 20
    }, {
        486, "Dozer", 56, 200000, 200
    }, {
        487, "Maverick", 399, 150000, 200
    }, {
        488, "News Chopper", 399, 70000, 100
    }, {
        489, "Rancher", 124, 14000, 80
    }, {
        490, "FBI Rancher", 139, 500000, 100
    }, {
        491, "Virgo", 132, 9600, 90
    }, {
        492, "Greenwood", 125, 4900, 40
    }, {
        493, "Jetmax", 399, 1000000, 200
    }, {
        494, "Hotring", 191, 0, 100
    }, {
        495, "Sandking", 157, 0, 120
    }, {
        496, "Blista Compact", 145, 7200, 60
    }, {
        497, "Police Maverick", 399, 0, 200
    }, {
        498, "Boxville", 96, 5400, 120
    }, {
        499, "Benson", 109, 16700, 100
    }, {
        500, "Mesa", 125, 16000, 80
    }, {
        501, "RC Goblin", 399, 0, 0
    }, {
        502, "Hotring Racer", 191, 0, 100
    }, {
        503, "Hotring Racer", 191, 0, 100
    }, {
        504, "Bloodring Banger", 154, 0, 100
    }, {
        505, "Rancher", 124, 12000, 120
    }, {
        506, "Super GT", 159, 250000, 80
    }, {
        507, "Elegant", 148, 21000, 70
    }, {
        508, "Journey", 96, 23000, 100
    }, {
        509, "Bike", 93, 250, 0
    }, {
        510, "Mountain Bike", 117, 350, 0
    }, {
        511, "Beagle", 399, 240000, 200
    }, {
        512, "Cropdust", 399, 78000, 200
    }, {
        513, "Stunt", 399, 340000, 200
    }, {
        514, "Tanker", 107, 120000, 200
    }, {
        515, "RoadTrain", 126, 240000, 250
    }, {
        516, "Nebula", 140, 13400, 70
    }, {
        517, "Majestic", 140, 8400, 70
    }, {
        518, "Buccaneer", 146, 8100, 55
    }, {
        519, "Shamal", 399, 320000, 200
    }, {
        520, "Hydra", 399, 1000000, 200
    }, {
        521, "FCR-900", 190, 24500, 60
    }, {
        522, "NRG-500", 200, 34500, 55
    }, {
        523, "HPV1000", 172, 0, 45
    }, {
        524, "Cement Truck", 116, 0, 120
    }, {
        525, "Tow Truck", 143, 18500, 145
    }, {
        526, "Fortune", 140, 6800, 50
    }, {
        527, "Cadrona", 133, 8600, 60
    }, {
        528, "FBI Truck", 157, 0, 120
    }, {
        529, "Willard", 133, 22500, 90
    }, {
        530, "Forklift", 54, 0, 20
    }, {
        531, "Tractor", 62, 12000, 140
    }, {
        532, "Combine", 98, 0, 290
    }, {
        533, "Feltzer", 148, 56000, 110
    }, {
        534, "Remington", 150, 16500, 60
    }, {
        535, "Slamvan", 140, 23000, 50
    }, {
        536, "Blade", 154, 14500, 60
    }, {
        537, "Freight", 399, 0, 120
    }, {
        538, "Streak", 399, 0, 120
    }, {
        539, "Vortex", 89, 0, 60
    }, {
        540, "Vincent", 136, 15600, 75
    }, {
        541, "Bullet", 180, 260000, 80
    }, {
        542, "Clover", 146, 3400, 40
    }, {
        543, "Sadler", 134, 6400, 45
    }, {
        544, "Firetruck", 132, 50000, 120
    }, {
        545, "Hustler", 131, 14300, 60
    }, {
        546, "Intruder", 133, 19500, 70
    }, {
        547, "Primo", 127, 5600, 85
    }, {
        548, "Cargobob", 399, 0, 200
    }, {
        549, "Tampa", 136, 4100, 50
    }, {
        550, "Sunrise", 128, 24300, 100
    }, {
        551, "Merit", 140, 31400, 85
    }, {
        552, "Utility", 108, 12300, 100
    }, {
        553, "Nevada", 399, 125000, 200
    }, {
        554, "Yosemite", 128, 12300, 120
    }, {
        555, "Windsor", 141, 95000, 75
    }, {
        556, "Monster Truck B", 98, 0, 100
    }, {
        557, "Monster Truck C", 98, 0, 100
    }, {
        558, "Uranus", 170, 29000, 80
    }, {
        559, "Jester", 158, 210000, 80
    }, {
        560, "Sultan", 150, 140000, 100
    }, {
        561, "Stratum", 137, 12300, 70
    }, {
        562, "Elegy", 158, 190000, 80
    }, {
        563, "Raindance", 399, 0, 200
    }, {
        564, "RC Tiger", 79, 0, 0
    }, {
        565, "Flash", 146, 24000, 80
    }, {
        566, "Tahoma", 142, 9100, 75
    }, {
        567, "Savanna", 154, 14500, 100
    }, {
        568, "Bandito", 130, 34500, 120
    }, {
        569, "Freight", 399, 0, 100
    }, {
        570, "Trailer", 399, 0, 100
    }, {
        571, "Kart", 83, 45000, 25
    }, {
        572, "Mower", 54, 2100, 10
    }, {
        573, "Duneride", 98, 45000, 210
    }, {
        574, "Sweeper", 53, 21000, 20
    }, {
        575, "Broadway", 140, 100000, 80
    }, {
        576, "Tornado", 140, 14500, 70
    }, {
        577, "AT-400", 399, 1000000, 200
    }, {
        578, "DFT-30", 116, 26500, 120
    }, {
        589, "Huntley", 140, 41000, 140
    }, {
        580, "Stafford", 136, 53000, 80
    }, {
        581, "BF-400", 170, 6500, 20
    }, {
        582, "Newsvan", 121, 54000, 190
    }, {
        583, "Tug", 76, 34000, 30
    }, {
        584, "Trailer", 399, 0, 0
    }, {
        585, "Emperor", 136, 18500, 85
    }, {
        586, "Wayfarer", 175, 13900, 40
    }, {
        587, "Euros", 147, 27000, 75
    }, {
        588, "Hotdog", 96, 11000, 100
    }, {
        589, "Club", 145, 10500, 55
    }, {
        590, "Trailer", 399, 0, 0
    }, {
        591, "Trailer", 399, 0, 0
    }, {
        592, "Andromada", 399, 0, 200
    }, {
        593, "Dodo", 399, 56000, 200
    }, {
        594, "RC Cam", 54, 0, 0
    }, {
        595, "Launch", 399, 0, 200
    }, {
        596, "Police Car", 156, 0, 100
    }, {
        597, "Police Car", 156, 0, 100
    }, {
        598, "Police Car", 156, 0, 100
    }, {
        599, "Police Ranger", 140, 0, 100
    }, {
        600, "Picador", 134, 9100, 60
    }, {
        601, "S.W.A.T. Van", 98, 0, 250
    }, {
        602, "Alpha", 150, 28000, 90
    }, {
        603, "Phoenix", 152, 24000, 70
    }, {
        604, "Glendale", 131, 0, 100
    }, {
        605, "Sadler", 134, 0, 100
    }, {
        606, "Luggage Trailer", 399, 0, 0
    }, {
        607, "Luggage Trailer", 399, 0, 0
    }, {
        608, "Stair Trailer", 399, 0, 0
    }, {
        609, "Boxville", 96, 12400, 120
    }, {
        610, "Farm Plow", 399, 0, 0
    }, {
        611, "Utility Trailer", 399, 0, 0
    }
};

new Float:CamSWATPos[MAX_PLAYERS][3];
new Float:CamSWATIndoorPos[MAX_PLAYERS][3];
new CamSWATVW[MAX_PLAYERS];

new PodnoszoneLista[MAX_PLAYERS][50];

new SelectedItem[MAX_PLAYERS];
new PlayerWeapon[MAX_PLAYERS];
new NotatnikUID[MAX_PLAYERS];
new SelectedPhone[MAX_PLAYERS];
new Selected911[MAX_PLAYERS];				//który numer wybra³ z alarmowych
new LogTry[MAX_PLAYERS];
new Logged[MAX_PLAYERS];
new Kamizelka[MAX_PLAYERS];					//kamizelka która ma na sobie ziom
new SendMoneyTo[MAX_PLAYERS];
new chatstat;
new PlayerDutyGroup[MAX_PLAYERS];

new AFK[MAX_PLAYERS];

new statusPlayer[MAX_PLAYERS];			//czy zmieni³ siê staus gracza
new DutyTimeToReduty[MAX_PLAYERS];
new PoliceBlockade[5];
new PoliceKolczatka[10];

new DrunkedTimer[MAX_PLAYERS];

new ToWho[MAX_PLAYERS];	// z kim rozmawia gracz
new CzyRozmawia[MAX_PLAYERS]; 	// czy gracz rozmawia

new PlayerBwTimer[MAX_PLAYERS];		//liczba sekund BW

new Preloaded[MAX_PLAYERS];
new FirstSpawn[MAX_PLAYERS];
new GetPlayerVehicle[MAX_PLAYERS];
new RepaintTime[MAX_PLAYERS];

new VehicleUnspawnTimer[MAX_VEHICLES];		//timer do usuniecia pojazdu

//oferty i ich zmienne
new SoldVehicle[MAX_PLAYERS];
new SoldItemUID[MAX_PLAYERS];
new OfferPrice[MAX_PLAYERS];
new OfferType[MAX_PLAYERS];
new Kupiec[MAX_PLAYERS];
new Sprzedawca[MAX_PLAYERS];
new Text:TextDrawOffers[MAX_PLAYERS];
new OfferColor1[MAX_PLAYERS];
new OfferColor2[MAX_PLAYERS];

new ModdingVehicle[MAX_PLAYERS];


new Text3D:PlayerNick[MAX_PLAYERS];
new Text3D:PlayerDesc[MAX_PLAYERS];
new Text3D:VehicleDesc[MAX_PLAYERS];

new Text:TextDrawSpecialPlayer[MAX_PLAYERS];
//new Text3D:PlayerStatus[MAX_PLAYERS];
new Text:TextDrawPunishInfo;
new Text:TextDrawAdminLog;
new Text:TextDrawSanNews;
new Text:ServerName;

//zmienne do tworzenia przedmiotow
new ItemValue1[MAX_PLAYERS];
new ItemValue2[MAX_PLAYERS];
new ItemOwner[MAX_PLAYERS];
new ItemType[MAX_PLAYERS];

new SelectedAdminDoors[MAX_PLAYERS];

new SelectedVehicleItem[MAX_PLAYERS];

new DutyGroupType[MAX_PLAYERS];

new VehicleKogut[MAX_VEHICLES];

new IsPlayerHasFlashbang[MAX_PLAYERS];

new PlayerGID[MAX_PLAYERS];

forward GetPlayerInfo(playerid);
forward SavePlayerInfo(playerid);
forward GuiInfo(playerid,text[]);
forward GetPlayerUID(playerid);
forward CheckPassword(playerid,password[]);
forward GetPlayerItems(playerid);
forward LogPlayerLogin(playerid,success);
forward setGunSkill(playerid);
forward SetPlayerNick(playerid);
forward QuickSave(playerid);
forward CitySpawn(playerid);
forward DropPlayerItem(playerid,itemid);
forward GetDroppedItem();
forward PickupNearbyItems(playerid);
forward PublicMe(playerid,tresc[]);				//me, np przy uzywaniu zegarka
forward IsPlayerConnect(playerid);				// 1 = log 0 = nielog
forward PlayerNarracja(playerid,tresc[]);		//** Coœ siê sta³o siê.
forward timelog();
forward datelog();
forward GetPlayerIDbyUID(uid);
forward CleanPlayerTable(playerid);
// GetPlayerGlobalNickname(playerid)			//zwraca globalny nickname
forward GetPlayerAccess(playerid);				//zwraca globalny access
forward CheckPlayerLogged(playerid);				//zwraca czy zalogowany
forward CheckAFK();
forward IsPlayerBanned(playerid); 				//1 = jest , 0 = nie jest
forward LoadAnims();

new idleTime[MAX_PLAYERS];

new busEnabled = 1;
new mysqlPerformed = 0;

//--	libS	--//
#include <lib\doors>
#include <lib\statystyki>
#include <lib\vehicles>
#include <lib\groups>
#include <lib\obiekty>
#include <lib\admin_alpha>
#include <lib\duty>
#include <lib\changeskin>
#include <lib\items>
#include <lib\narkotyki>
#include <lib\pozar>
#include <lib\areas>
#include <lib\ac>
#include <lib\offers>
#include <lib\computer>
#include <lib\shit>
#include <lib\tel_specials>
#include <lib\pomoc>
#include <lib\bus>
#include <lib\pd>
#include <lib\dorywcze>
#include <lib\labels>
#include <lib\motels>
#include <lib\przestepcze>
//--						--//

main () 
{

}

// -- CALLBACKS -- //

forward SystemTimer();
public SystemTimer()
{
	RunningTime++;
	// respawnowanie œmieciarek
	for(new vid = 0 ; vid < MAX_VEHICLES ; vid++)
	{
		if(vehicle[vid][vownertype] == 1)
		{
			if(vehicle[vid][vowneruid] == GROUP_TYPE_RUBBISH_UID || vehicle[vid][vowneruid] == GROUP_TYPE_PIZZA_UID)
			{
				if(!IsVehicleOccupied(vid))
				{
					if(vehicle[vid][vRubbishUnspawn] >= 60)
					{
						vehicle[vid][vRubbishUnspawn] = 0 ;
						RespawnVehicle(vid);
					}
					else
					{
						vehicle[vid][vRubbishUnspawn]++;
					}
				}
			}
		}
	}
	
	
	
	// czyszczenie opisow niezalogownych postaci
	for(new i = 0 ; i < MAX_PLAYERS; i++)
	{
		if(!IsPlayerConnect(i))
		{
			UpdateDynamic3DTextLabelText(Text3D:PlayerDesc[i], COLOR_ME, " ");
		}
	}
}

forward VehicleTimer();
public VehicleTimer()
{
	for(new i = 0 ; i < MAX_VEHICLES; i++)
	{
		new Float:VehHP;
		GetVehicleHealth(i,VehHP);
		if(VehHP < 350)
		{
			vehicle[i][vhp] = 350;
			SetVehicleHealth(i,350);
		}
		
		//offroad
		new felgi = GetVehicleComponentInSlot(i, CARMODTYPE_WHEELS);
		if(felgi == 1025)
		{
			new panels,doors,lights,tires;
			GetVehicleDamageStatus(i,panels,doors,lights,tires);
			
			UpdateVehicleDamageStatus(i,panels,doors,lights,0);
		}
	}
}

forward SetServerTime();
public SetServerTime()
{
	new hour,minute,sec;
	gettime(hour,minute,sec);
	SetWorldTime(hour);
	
	// dotacja spo³eczna
	new buffer[256];
	format(buffer,sizeof(buffer),"UPDATE srv_settings SET val=val-3600 WHERE type=1");
	mysql_query(buffer);
	
	
	format(buffer,sizeof(buffer),"SELECT val FROM srv_settings WHERE type=1");
	mysql_query(buffer);
	mysql_store_result();
	new val = mysql_fetch_int();
	mysql_free_result();
	
	if(val <= 0)
	{
		//dodaj kazdemu
		format(buffer,sizeof(buffer),"UPDATE srv_settings SET val = 86400 WHERE type=1");
		mysql_query(buffer);
		
		format(buffer,sizeof(buffer),"UPDATE core_players SET char_bank=char_bank+%i",BENEFIT_AMOUNT);
		mysql_query(buffer);
				
	}
}

public OnGameModeInit()
{
    new MySQL:connection = mysql_init(LOG_ONLY_ERRORS, 1);
	mysql_connect(M_HOST, M_LOGIN, M_PASS, M_DB, connection, 1);
	
	InitVehicleSystem();
	
//	vehiclelist = LoadModelSelectionMenu("vehicles.txt");
	
	Audio_CreateTCPServer(7777);
	
	SetTimer("CheckAFK", 1000, true);
	
	printf("[system] uruchamiam system timer");
	SetTimer("SystemTimer", 1000, true);
	
	SetTimer("VehicleTimer", 200, true);
	
	SetTimer("forceGVR", 10000, false);			//moze do wycofania
	
	SetTimer("SetServerTime", 3600000, true);
	
	SetTimer("PlantsTimer", 60000, true);			//timerek plantsów
	
	InitGroups();
	
	LoadBusSystem();
	
	SystemNarkotykow();
	
	LoadStatsSystem();
	
	LoadDutyText();
	
	StartLabelSystem();
	
	LoadProperties();
	
	InitAdmins();
	
	LoadSkinsSystem();
	
	InitDoors();
	
	LoadPetrols();
	
	LoadComputerSystem();
	
	LoadBankomats();
	
	LoadCrimeSys();
	
	LoadAnims();
	
	InitObjects();
	
	AntyDeAMX();
	
	chatstat = 0;
	
	vehiclelist = LoadModelSelectionMenu("vehicles.txt");

	mysql_query("UPDATE core_players SET char_logged=0");	//czyszczenie komórki czy_zalogowany
	
	mysql_query("UPDATE vehicles_list SET veh_spawned= 0, veh_sampid = 0");
	
	ShowPlayerMarkers(0);
    ShowNameTags(0);
    DisableInteriorEnterExits();
    EnableStuntBonusForAll(0);
	EnableZoneNames(1);
	
	printf("%s : trwa uruchamianie skryptu...",VERSION);
	new gamemodetext[128];
	format(gamemodetext,sizeof(gamemodetext),"%s %s",SERVERNAME,VERSION);
	SetGameModeText(gamemodetext);
	
	mysql_query("UPDATE core_items SET item_used=0");
	
	for(new i = 0 ; i < MAX_PLAYERS ; i ++)
	{
		PlayerNick[i] = CreateDynamic3DTextLabel(" ", COLOR_WHITE, 0.0, 0.0, 0.2, 10.0, i, INVALID_VEHICLE_ID, 1);
		PlayerDesc[i] = CreateDynamic3DTextLabel(" ", COLOR_ME, 0.0, 0.0, -0.6, 10.0, i, INVALID_VEHICLE_ID, 1);
		
		TextDrawSpecialPlayer[i] = Text:TextDrawCreate(492, 425.000000, " ");
		TextDrawFont(Text:TextDrawSpecialPlayer[i] , 3);
		TextDrawColor(Text:TextDrawSpecialPlayer[i] , COLOR_GREY);
		TextDrawSetOutline(Text:TextDrawSpecialPlayer[i] , 1);
		TextDrawLetterSize(Text:TextDrawSpecialPlayer[i] , 0.40, 0.90);
		TextDrawSetProportional(Text:TextDrawSpecialPlayer[i] , 1);
	}
	
	printf("[load] za³adowano 3d texty dla graczy");
	
	for(new i= 0 ; i < MAX_VEHICLES; i++)
	{
		//VehicleDesc[i] = CreateDynamic3DTextLabel(" ", COLOR_DO, 0.0, 0.0, -0.6, 10.0, -1, i, 1);
		VehicleDesc[i] = CreateDynamic3DTextLabel(" ", COLOR_DO, 0, 0, 0, 18.5, INVALID_PLAYER_ID, i ,  1);
	}
	
	print("[load] zaladowano 3dtexty dla pojazdow");
	
	//tutaj wstawiæ czyszczenie last_pd jeœli dzieñ nie taki
	
	return 1;
}

public OnGameModeExit()
{
	print("[gamemode] Wy³¹czam wszystkie modu³y...");
	print("[vehicles] Usuwam wszystkie pojazdy z mapy");
	Audio_DestroyTCPServer();
	UnloadProperties();
	SavePlants();
	test2();
	return 1;
}

public OnPlayerCommandPerformed(playerid, cmdtext[], success)
{
	new Float:x,Float:y,Float:z;
	GetPlayerPos(playerid,x,y,z);
	if(!success) 
	{
		//vlive_err(playerid,"takiej komendy nawet najstarsi indianie jeszcze nie wymyœlili");	
		PlayerPlaySound(playerid, 5205, x, y, z);
	}
	return 1;
}

forward GMX();
public GMX()
{
	SendRconCommand("gmx");
	return 1;
}

forward TurnOff();
public TurnOff()
{
	SendRconCommand("exit");
	return 1;
}

stock FormatDesc(text[])
{
	new string[256];
	format(string,sizeof(string),"%s",text);
    if(strlen(text) > 64)
    {
        new text1[65],
            text2[65];
            
        strmid(text2, text, 64, 128);
        strmid(text1, text, 0, 64);
		
		new final[256];
		format(final,sizeof(final),"%s\n%s",text1,text2);
        return final;
    }
    return string;
}

forward GetStartedVehicle(playerid);
public GetStartedVehicle(playerid)
{
	ShowModelSelectionMenu(playerid, vehiclelist, "Wybierz pojazd");
}

public OnDialogResponse(playerid,dialogid,response,listitem,inputtext[])
{
	switch(dialogid)
	{
		case DIAL_KUP_APTEKA:
		{
			if(!response) { }
			else
			{
				if(pInfo[playerid][pMoney] >= AptekaItems[listitem][aItemCost])
				{
					new buffer[256];
					format(buffer,sizeof(buffer),"INSERT INTO core_items(item_owneruid,item_whereis,item_value1,item_value2,item_type,item_name) VALUES (%i,%i,%i,%i,%i,'%s')",GetPlayerUID(playerid),0,AptekaItems[listitem][aItemValue1],0,AptekaItems[listitem][aType],AptekaItems[listitem][aItemName]);
					mysql_query(buffer);
					
					TakePlayerMoney(playerid, AptekaItems[listitem][aItemCost]);
				}
				else
				{
					GuiInfo(playerid,"Nie staæ Cie na ten przedmiot.");
				}
			}
		}
		case DIAL_STYLE_STATS:
		{
			if(!response) { }
			else
			{
				new buffer[256];
				switch(listitem)
				{
					case 0:	//normal
					{
						format(buffer,sizeof(buffer),"UPDATE core_players SET char_fs=%i WHERE char_uid=%i",FIGHT_STYLE_NORMAL,pInfo[playerid][pUID]);
						pInfo[playerid][fightingStyle] = FIGHT_STYLE_NORMAL;
						SetPlayerFightingStyle (playerid, FIGHT_STYLE_NORMAL);
					}
					case 1:	//boks
					{
						format(buffer,sizeof(buffer),"UPDATE core_players SET char_fs=%i WHERE char_uid=%i",FIGHT_STYLE_BOXING,pInfo[playerid][pUID]);
						pInfo[playerid][fightingStyle] = FIGHT_STYLE_BOXING;
						SetPlayerFightingStyle (playerid, FIGHT_STYLE_BOXING);
					}
					case 2:	//kongfu
					{
						format(buffer,sizeof(buffer),"UPDATE core_players SET char_fs=%i WHERE char_uid=%i",FIGHT_STYLE_KUNGFU,pInfo[playerid][pUID]);
						pInfo[playerid][fightingStyle] = FIGHT_STYLE_KUNGFU;
						SetPlayerFightingStyle (playerid, FIGHT_STYLE_KUNGFU);
					}
				}
				
				mysql_query(buffer);
				GuiInfo(playerid,"Zmieni³eœ swój styl walki.");
			}
		}
		case DIAL_STATS:
		{
			if(!response) { }
			else
			{
				switch(listitem)
				{
					case 12:	//fighting style
					{
						if(pInfo[playerid][pPower] < 60)
						{
							GuiInfo(playerid,"Nie masz wystarczaj¹co wytrenowanej postaci by móc wybieraæ styl walki.");
							return 1;
						}
						else
						{
							ShowPlayerDialog(playerid,DIAL_STYLE_STATS,DIALOG_STYLE_LIST,"Wybierz styl","1. Normalny\n2. Boks\n3. Kungfu","Wybierz","Anuluj");
						}
					}
				}
			}
		}
		case DIAL_SELECT_WEIGHT:
		{
			if(!response) { }
			else
			{
				switch(listitem)
				{
					case 0:	//20
					{
						//startuj
						PlayerCache[playerid][pGymPowerPerTick] =  (4 * 0.000625);
						PlayerCache[playerid][pGymHantleType] = HANTLE_20;
						
						SetPlayerTrain(playerid,TRAIN_TYPE_HANTLE);
					}
					case 1:	//40
					{
						if(pInfo[playerid][pPower]  < 30)
						{
							//nie rozpoczynaj bo za malo sily
							GuiInfo(playerid,"Twoja postaæ jest zbyt s³aba by unosiæ taki ciê¿ar.");
						}
						else
						{
							//rozpocznij
							PlayerCache[playerid][pGymPowerPerTick] =  (4 * 0.0005) ;
							PlayerCache[playerid][pGymHantleType] = HANTLE_40;
							SetPlayerTrain(playerid,TRAIN_TYPE_HANTLE);
						}
					}
					case 2:	//60
					{
						if(pInfo[playerid][pPower] < 45)
						{
							//nie rozpoczynaj bo za malo sily
							GuiInfo(playerid,"Twoja postaæ jest zbyt s³aba by unosiæ taki ciê¿ar.");
						}
						else
						{
							//rozpocznij
							PlayerCache[playerid][pGymPowerPerTick] =  (4 * 0.000375) ;
							PlayerCache[playerid][pGymHantleType] = HANTLE_60;
							SetPlayerTrain(playerid,TRAIN_TYPE_HANTLE);
						}
					}
					case 3: //80
					{
						if(pInfo[playerid][pPower]  < 65)
						{
							//nie rozpoczynaj bo za malo sily
							GuiInfo(playerid,"Twoja postaæ jest zbyt s³aba by unosiæ taki ciê¿ar.");
						}
						else
						{
							//rozpocznij
							PlayerCache[playerid][pGymPowerPerTick] =  (4 * 0.00025) ;
							PlayerCache[playerid][pGymHantleType] = HANTLE_80;
							SetPlayerTrain(playerid,TRAIN_TYPE_HANTLE);
						}
					}
					case 4: //100
					{
						if(pInfo[playerid][pPower] < 80)
						{
							//nie rozpoczynaj bo za malo sily
							GuiInfo(playerid,"Twoja postaæ jest zbyt s³aba by unosiæ taki ciê¿ar.");
						}
						else
						{
							//rozpocznij
							PlayerCache[playerid][pGymPowerPerTick] =  (4 * 0.000125) ;
							PlayerCache[playerid][pGymHantleType] = HANTLE_100;
							SetPlayerTrain(playerid,TRAIN_TYPE_HANTLE);
						}
					}
				}
			}	
		}
		case DIAL_CICON:
		{
			if(!response) { }
			else
			{
				new icontype = 0;
				switch(listitem)
				{
					case 0:	//restauracja
					{
						icontype = 10;
					}
					case 1:	//warsztat
					{
						icontype = 27;
					}
					case 2: //cardealer
					{
						icontype = 55;
					}
					case 3: //dolar
					{
						icontype = 52;
					}
					case 4: //ubranie
					{
						icontype = 45;
					}
				}
				
				new buffer[256];
				new Float:Pos[3];
				GetPlayerPos(playerid,Pos[0],Pos[1],Pos[2]);
				format(buffer,sizeof(buffer),"INSERT INTO icons(x,y,z,markertype) VALUES(%f,%f,%f,%i)",Pos[0],Pos[1],Pos[2],icontype);
				mysql_query(buffer);
				
				GuiInfo(playerid,"Doda³eœ ikonê, bêdzie widoczna po relogowaniu.");
			}
		}
		case DIAL_STYLE:
		{
			if(!response) { }
			else
			{
				switch(listitem)
				{
					case 0:
					{
						SetPlayerFightingStyle (playerid, FIGHT_STYLE_NORMAL);
					}
					case 1:
					{
						SetPlayerFightingStyle (playerid, FIGHT_STYLE_BOXING);
					}
					case 2:
					{
						SetPlayerFightingStyle (playerid, FIGHT_STYLE_KUNGFU);
					}
					case 3:
					{
						SetPlayerFightingStyle (playerid, FIGHT_STYLE_KNEEHEAD);
					}
					case 4:
					{
						SetPlayerFightingStyle (playerid, FIGHT_STYLE_GRABKICK);
					}
					case 5:
					{
						SetPlayerFightingStyle (playerid, FIGHT_STYLE_ELBOW);
					}
				}
			}
		}
		case DIAL_HOUSE_SELECT_SPAWN:
		{
			if(!response) { }
			else
			{
				SetPlayerSpawning(playerid,PlayerCache[playerid][pSelectedHouse]);
				GuiInfo(playerid,"Ustawi³eœ nowe miejsce spawnu.");
			}
		}
		case DIAL_HOUSE_LISTING:
		{
			if(!response) { }
			else
			{
				//pokaz liste mieszkan i poxniej wybierz tam spawnl
				new buffer[256];
				new num = 0 ;
				format(buffer,sizeof(buffer),"SELECT * FROM houseinvites WHERE charuid= %i ", GetPlayerUID(playerid));
				mysql_query(buffer);
				mysql_store_result();
				if(mysql_num_rows() <= 0)
				{
					mysql_free_result();
					return 1;
				}
				while(mysql_fetch_row(buffer,"|"))
				{
					//id duid, chuid, chname
					new none,dooruid,charuid,charname[32];
					sscanf(buffer,"p<|>iiis[32]",none,dooruid,charuid,charname);
					if(num == listitem)
					{
						PlayerCache[playerid][pSelectedHouse] = dooruid;
						ShowPlayerDialog(playerid,DIAL_HOUSE_SELECT_SPAWN,DIALOG_STYLE_MSGBOX,"Dom » wybierz spawn","Czy chcesz wybraæ ten dom jako swój spawn?","Tak","Nie");
					}
					num++;
				}
				
				mysql_free_result();
			}
		}
		case DIAL_HOUSE_UNINVITE:
		{
			if(!response) { }
			else
			{
				new buffer[256];
				format(buffer,sizeof(buffer),"DELETE FROM houseinvites WHERE id = %i",PlayerCache[playerid][pSelectedLokator] );
				mysql_query(buffer);
				
				GuiInfo(playerid,"Usun¹³eœ t¹ osobê jako lokatora tego domu.");
			}
		}
		case DIAL_HOUSE_INVITES:
		{
			if(!response) { }
			else
			{
				//listitem = lokator
				new dooruid = DoorInfo[GetPlayerDoorID(playerid)][doorUID];
				new buffer[256];
				
				format(buffer,sizeof(buffer),"SELECT id FROM houseinvites WHERE dooruid=%i",dooruid);
				mysql_query(buffer);
				mysql_store_result();
				new num = 0;
				
				while(mysql_fetch_row(buffer,"|"))
				{
					if(listitem-1 == num)
					{
						PlayerCache[playerid][pSelectedLokator] = mysql_fetch_int();
						ShowPlayerDialog(playerid,DIAL_HOUSE_UNINVITE,DIALOG_STYLE_MSGBOX,"Dom » wyproœ","Czy chcesz wyprosiæ t¹ osobê z domu?","Tak","Nie");
					}
					num++;
				}
				
				mysql_free_result();
			}
		}
		case DIAL_SHOW_HOUSE_MENU:
		{
			if(!response) { }
			else
			{
				//menu mieszkan
				switch(listitem)
				{
					case 0:	//lista mieszkan
					{
						//pokaz liste mieszkan i poxniej wybierz tam spawnl
						new buffer[256];
						format(buffer,sizeof(buffer),"SELECT * FROM houseinvites WHERE charuid= %i ", GetPlayerUID(playerid));
						mysql_query(buffer);
						new lista[512];
						mysql_store_result();
						if(mysql_num_rows() <= 0)
						{
							mysql_free_result();
							return GuiInfo(playerid,"Nie jesteœ lokatorem ¿adnego mieszkania.");
						}
						
						while(mysql_fetch_row(buffer,"|"))
						{
							//id duid, chuid, chname
							new none,dooruid,charuid,charname[32];
							sscanf(buffer,"p<|>iiis[32]",none,dooruid,charuid,charname);
							format(lista,sizeof(lista),"%s\n%s",lista,DoorInfo[GetDoorIDbyUID(dooruid)][doorName]);
						}
						
						ShowPlayerDialog(playerid,DIAL_HOUSE_LISTING,DIALOG_STYLE_LIST,"Mieszkania w których jesteœ lokatorem",lista,"Wybierz","Anuluj");
						mysql_free_result();
					}
					
					case 1:	//lokatorzy
					{
						new doorid = GetPlayerDoorID(playerid);
						if(doorid <= 0)
						{
							GuiInfo(playerid,"Musisz byæ w mieszkaniu, by u¿yæ tej opcji.");
						}
						else
						{
							if(DoorInfo[doorid][doorOwnerType] != 2)
							{
								GuiInfo(playerid,"Musisz byæ w mieszkaniu by u¿yæ tej komendy.");
								return 1;
							}
							
							if(IsPlayerPermsDoors(playerid,doorid))
							{
								new dooruid = DoorInfo[doorid][doorUID];
								new buffer[256];
								format(buffer,sizeof(buffer),"SELECT * FROM houseinvites WHERE dooruid=%i",dooruid);
								mysql_query(buffer);
								mysql_store_result();
								
								if(mysql_num_rows() <= 0)
								{
									//nie ma lokatorow
									GuiInfo(playerid,"Brak lokatorów dla tego mieszkania.");
								}
								else
								{
									new invited[512];
									while(mysql_fetch_row(buffer,"|"))
									{
										new none,dooruidd,charuid,charname[32];
										sscanf(buffer,"p<|>iiis[32]",none,dooruidd,charuid,charname);
										format(invited,sizeof(invited),"%s\n%i\t\t%s",invited,charuid,charname);
									}
									
									ShowPlayerDialog(playerid,DIAL_HOUSE_INVITES,DIALOG_STYLE_LIST,"Lista lokatorów",invited,"Wybierz","Anuluj");
								}
								
								mysql_free_result();
							}
							else
							{
								GuiInfo(playerid,"Brak uprawnieñ do wpisania tej komendy.");
							}
						}
					}
				}
			}
		}
		case DIAL_MODDING_VEHICLE:
		{
			if(!response) { }
			else
			{
				new modvehicle = ModdingVehicle[playerid];
				
				switch(listitem)
				{
					case 0:		//firmowy
					{
						if(vehicle[modvehicle][vEngine] == ENGINE_TYPE_BASE)
						{
							return GuiInfo(playerid,"Pojazd posiada ju¿ zamontowany ten typ silnika.");
						}
						else
						{
							if(pInfo[playerid][pMoney] >= ENGINE_BASE_COST)
							{
								new buffer[256];
								format(buffer,sizeof(buffer),"UPDATE vehicles_list SET veh_engine = 1 WHERE veh_uid=%i",vehicle[modvehicle][vuid]);
								mysql_query(buffer);
								GuiInfo(playerid,"Zamontowa³eœ silnik typu firmowego.");
								vehicle[modvehicle][vEngine] = ENGINE_TYPE_BASE;
							}
							else
							{
								GuiInfo(playerid,"Nie staæ Cie na wmontowanie tego silnika.");
							}
						}
					}
					
					case 1:		//modyfikowany
					{
						if(vehicle[modvehicle][vEngine] == ENGINE_TYPE_MODIFY)
						{
							return GuiInfo(playerid,"Pojazd posiada ju¿ zamontowany ten typ silnika.");
						}
						else
						{
							if(pInfo[playerid][pMoney] >= ENGINE_MODIFY_COST)
							{
								TakePlayerMoney(playerid,ENGINE_MODIFY_COST);
								new buffer[256];
								format(buffer,sizeof(buffer),"UPDATE vehicles_list SET veh_engine = 2 WHERE veh_uid=%i",vehicle[modvehicle][vuid]);
								mysql_query(buffer);
								GuiInfo(playerid,"Zamontowa³eœ silnik typu modyfikowanego.");
								vehicle[modvehicle][vEngine] = ENGINE_TYPE_MODIFY;
							}
							else
							{
								GuiInfo(playerid,"Nie staæ Cie na wmontowanie tego silnika.");
							}
						}
					}
					
					case 2:		//sportowy
					{
						if(DutyGroupType[playerid] != TYPE_SCIGANT )
						{
							GuiInfo(playerid,"Brak uprawnieñ.");
							return 1;
						}
						
						
						if(vehicle[modvehicle][vEngine] == ENGINE_TYPE_SPORT)
						{
							return GuiInfo(playerid,"Pojazd posiada ju¿ zamontowany ten typ silnika.");
						}
						else
						{
							if(pInfo[playerid][pMoney] >= ENGINE_SPORT_COST)
							{
								TakePlayerMoney(playerid,ENGINE_SPORT_COST);
								new buffer[256];
								format(buffer,sizeof(buffer),"UPDATE vehicles_list SET veh_engine = 3 WHERE veh_uid=%i",vehicle[modvehicle][vuid]);
								mysql_query(buffer);
								GuiInfo(playerid,"Zamontowa³eœ silnik typu sportowego.");
								vehicle[modvehicle][vEngine] = ENGINE_TYPE_SPORT;
							}
							else
							{
								GuiInfo(playerid,"Nie staæ Cie na wmontowanie tego silnika.");
							}
						}
					}
				}
			}
		}
		case DIAL_SEL_STARTED:
		{
			if(!response)
			{
				//anulowal wybieranie
				PlayerCache[playerid][pSelStarted] = 1;
			}
			if(response)
			{
				SetTimerEx("GetStartedVehicle", 1000, false, "i", playerid);
				//ShowModelSelectionMenu(playerid, vehiclelist, "Wybierz pojazd");
			}
		}
		case DIAL_VEH_ITEM_SELECTED:
		{
			if(!response) { }
			else
			{
				new itemuid = SelectedVehicleItem[playerid];
				new buffer[256];
				format(buffer,sizeof(buffer),"UPDATE core_items SET item_owneruid = %i, item_whereis=%i WHERE item_uid=%i",GetPlayerUID(playerid),WHEREIS_GRACZ,itemuid);
				mysql_query(buffer);
				GuiInfo(playerid,"Wyci¹gn¹³eœ przedmiot ze schowka.");
				PublicMe(playerid,"wyci¹ga przedmiot ze schowka w pojeŸdzie.");
			}
		}
		case DIAL_LIST_VEH_ITEMS:
		{
			if(!response) { }
			else
			{
				new uid,name[64];
				new buffer[256];
				new vehicleid = GetPlayerVehicleID(playerid);
				format(buffer,sizeof(buffer),"SELECT item_uid,item_name FROM core_items WHERE item_whereis=%i AND item_owneruid=%i",WHEREIS_AUTO,vehicle[vehicleid][vuid]);
				mysql_query(buffer);
				mysql_store_result();
				new num = 0;
				while(mysql_fetch_row(buffer,"|"))
				{
					if(num == listitem-2)	//tutaj moze byc errorek
					{
						sscanf(buffer,"p<|>is[64]",uid,name);
						break;
					}
					num++;
				}
				mysql_free_result();
				
				// listitem+2
				SelectedVehicleItem[playerid] = uid;
				new msg[126];
				format(msg,sizeof(msg),"Czy na pewno chcesz wyci¹gn¹æ %s ze schowka?",name);
				ShowPlayerDialog(playerid,DIAL_VEH_ITEM_SELECTED,DIALOG_STYLE_MSGBOX,"Schowek » wyci¹ganie",msg,"Tak","Nie");
			}
		}
		case DIAL_GANJA_NIEDOJRZALA:
		{
			if(!response) { }
			else
			{
				new ganjaid = PlayerCache[playerid][pSelPlant] ;
				
				if(IsValidObject(Plant[ganjaid][plant_obj]))
				{
					DestroyObject(Plant[ganjaid][plant_obj]);
				}
					
				Delete3DTextLabel(Text3D:Plant[ganjaid][plant_txt]);
				
				new buffer[256];
				format(buffer,sizeof(buffer),"DELETE FROM core_plants WHERE id=%i",Plant[ganjaid][plant_id]);
				mysql_query(buffer);
					
				Plant[ganjaid][plant_id] = EOS;
				Plant[ganjaid][plant_x] = EOS;
				Plant[ganjaid][plant_y] = EOS;
				Plant[ganjaid][plant_z] = EOS;
				Plant[ganjaid][plant_time] = EOS;
				
				GuiInfo(playerid,"Zakopa³eœ roœlinkê, szkoda.");
			}
		}
		case DIAL_PURCHASE_LSPD_SPECIALS:
		{
			if(!response) { }
			else
			{
				if(pInfo[playerid][pMoney] < LspdSpecials[listitem][spd_price])
				{
					GuiInfo(playerid,"Nie posiadasz tyle pieniêdzy.");
					return 1;
				}
				else
				{
					TakePlayerMoney(playerid,LspdSpecials[listitem][spd_price]);
					CreateAttachedItem(playerid,LspdSpecials[listitem][spd_model],LspdSpecials[listitem][spd_bone],LspdSpecials[listitem][spd_name]);
					GuiInfo(playerid,"Wybrany przedmiot pojawi³ siê w Twoim ekwipunku.");
				}
			}
		}
		case DIAL_ATTACH_MENU:
		{
			if(!response) { }
			else
			{
				switch(listitem)
				{
					case 0:	//use
					{
						new buffer[256];
						
						new slot_do_czyszczenia = 0;
						
						format(buffer,sizeof(buffer),"SELECT item_value2 FROM core_items WHERE item_uid=%i AND item_used = 1",PlayerCache[playerid][pSelectedAttachment]);
						mysql_query(buffer);
						mysql_store_result();
						if(mysql_num_rows() > 0)
						{
							slot_do_czyszczenia = mysql_fetch_int();
							switch(slot_do_czyszczenia)
							{
								case 1:
								{
									RemovePlayerAttachedObject(playerid,SLOT_PLAYERONE);
								}
								case 2:
								{
									RemovePlayerAttachedObject(playerid,SLOT_PLAYERTWO);
								}
							}
						}
						mysql_free_result();
						
						if(slot_do_czyszczenia > 0)
						{
							format(buffer,sizeof(buffer),"UPDATE core_items SET item_value2 = 0, item_used = 0 WHERE item_uid=%i",PlayerCache[playerid][pSelectedAttachment]);
							mysql_query(buffer);
							return 1;			
						}


						format(buffer,sizeof(buffer),"SELECT * FROM core_items WHERE item_owneruid=%i AND item_whereis= %i AND item_type= %i AND item_used = 1",GetPlayerUID(playerid),WHEREIS_GRACZ,ITEM_ATTACH);
						mysql_query(buffer);
						mysql_store_result();
						if(mysql_num_rows() >= 2)
						{
							mysql_free_result();
							GuiInfo(playerid,"Obydwa sloty zajête.");
							return 1;
						}
						mysql_free_result();
						
						format(buffer,sizeof(buffer),"SELECT item_value1 FROM core_items WHERE item_uid=%i",PlayerCache[playerid][pSelectedAttachment]);
						mysql_query(buffer);
						mysql_store_result();
						new modelid = mysql_fetch_int();
						mysql_free_result();
						

						format(buffer,sizeof(buffer),"SELECT * FROM core_attachments WHERE item_uid=%i",PlayerCache[playerid][pSelectedAttachment]);
						mysql_query(buffer);
						mysql_store_result();

						new id, iuid, bone, Float:posx,Float:posy,Float:posz,Float:rotx,Float:roty,Float:rotz,Float:sx,Float:sy,Float:sz;
						while(mysql_fetch_row(buffer,"|"))
						{
							sscanf(buffer,"p<|>iiifffffffff",id,iuid,bone,posx,posy,posz,rotx,roty,rotz,sx,sy,sz);
							
							if(IsPlayerAttachedObjectSlotUsed(playerid,SLOT_PLAYERONE))
							{
								if(IsPlayerAttachedObjectSlotUsed(playerid,SLOT_PLAYERTWO))
								{
									GuiInfo(playerid,"Brak wolnego slotu dla tego przedmiotu.");
									return 1;
								}
								else
								{
									//PlayerAttachment[1][attachmentItemUid] = PlayerCache[playerid][pSelectedAttachment];
									format(buffer,sizeof(buffer),"UPDATE core_items SET item_used = 1, item_value2 = 2 WHERE item_uid=%i",PlayerCache[playerid][pSelectedAttachment]);
									mysql_query(buffer);
									SetPlayerAttachedObject(playerid, SLOT_PLAYERTWO, modelid, bone, posx,posy,posz,rotx,roty,rotz,sx,sy,sz);
								}
							}
							else
							{
								//PlayerAttachment[0][attachmentItemUid] = PlayerCache[playerid][pSelectedAttachment];
								format(buffer,sizeof(buffer),"UPDATE core_items SET item_used = 1, item_value2 = 1 WHERE item_uid=%i",PlayerCache[playerid][pSelectedAttachment]);
								mysql_query(buffer);
								SetPlayerAttachedObject(playerid, SLOT_PLAYERONE, modelid, bone, posx,posy,posz,rotx,roty,rotz,sx,sy,sz);
							}
							
						}
						mysql_free_result();
					}
					case 1:	//edit
					{
						new buffer[256];
						format(buffer,sizeof(buffer),"SELECT item_value1 FROM core_items WHERE item_uid=%i",PlayerCache[playerid][pSelectedAttachment]);
						mysql_query(buffer);
						mysql_store_result();
						new modelid = mysql_fetch_int();
						mysql_free_result();
						

						format(buffer,sizeof(buffer),"SELECT * FROM core_attachments WHERE item_uid=%i",PlayerCache[playerid][pSelectedAttachment]);
						mysql_query(buffer);
						mysql_store_result();
						
						new id, iuid, bone, Float:posx,Float:posy,Float:posz,Float:rotx,Float:roty,Float:rotz,Float:sx,Float:sy,Float:sz;
						while(mysql_fetch_row(buffer,"|"))
						{
							sscanf(buffer,"p<|>iiifffffffff",id,iuid,bone,posx,posy,posz,rotx,roty,rotz,sx,sy,sz);
							
							if(IsPlayerAttachedObjectSlotUsed(playerid,SLOT_PLAYERONE))
							{
								if(IsPlayerAttachedObjectSlotUsed(playerid,SLOT_PLAYERTWO))
								{
									GuiInfo(playerid,"Brak wolnego slotu dla tego przedmiotu.");
									return 1;
								}
								else
								{
									//doczep
									//PlayerAttachment[1][attachmentItemUid] = PlayerCache[playerid][pSelectedAttachment];
									SetPlayerAttachedObject(playerid, SLOT_PLAYERTWO, modelid, bone, posx,posy,posz,rotx,roty,rotz,sx,sy,sz);
									EditAttachedObject(playerid, SLOT_PLAYERTWO);
									PlayerCache[playerid][pEditAttachment] = 1;
								}
							}
							else
							{
								//doczep
								//PlayerAttachment[0][attachmentItemUid] = PlayerCache[playerid][pSelectedAttachment];
								SetPlayerAttachedObject(playerid, SLOT_PLAYERONE, modelid, bone, posx,posy,posz,rotx,roty,rotz,sx,sy,sz);
								EditAttachedObject(playerid, SLOT_PLAYERONE);
								PlayerCache[playerid][pEditAttachment] = 0;
							}
							
						}
						mysql_free_result();
					}
				}
			}
		}
		case DIAL_SHOW_GROUPS:
		{
			if(!response) { }
			else
			{
				
				if(listitem == 9)
				{
					new lista[512];
					for(new i=PlayerCache[playerid][pGroupListPage];i<PlayerCache[playerid][pGroupListPage]+10;i++)
					{
						if(i == PlayerCache[playerid][pGroupListPage]+10)
						{
							format(lista,1024,"%s\n###########\n>>>>>>>>>>>>",lista);
						}
						else
						{
							format(lista,1024,"%s\n%i\t\t%s",lista,i,grupa[i][Gname]);
						}
					}
					ShowPlayerDialog(playerid,DIAL_SHOW_GROUPS,DIALOG_STYLE_LIST,"Lista grup",lista,"OK","");
					//next page
					PlayerCache[playerid][pGroupListPage] = PlayerCache[playerid][pGroupListPage]+10;
				}
				else if(listitem == 10)
				{
					//prev page
					PlayerCache[playerid][pGroupListPage] = PlayerCache[playerid][pGroupListPage]-10;
				}
			}
		}
		case DIAL_TUNING:
		{
			if(!response) { }
			else
			{
				//ShowPlayerDialog(playerid,DIAL_TUNING_OPTIONS,DIALOG_STYLE_MSGBOX,"Akcja","Czy na pewno chcesz wymontowaæ t¹ czêœæ z pojazdu?","Tak","Nie");
				new vehicleid = GetPlayerVehicleID(playerid);
				new vehicleUID = vehicle[vehicleid][vuid];
				new buffer[256];
				format(buffer,sizeof(buffer),"SELECT item_uid,item_name,item_value1 FROM core_items WHERE item_owneruid = 0 AND item_value2 = %i",vehicleUID);
				mysql_query(buffer);
				mysql_store_result();
				new num = 0 ;
				while(mysql_fetch_row(buffer,"|"))
				{
					if(num == listitem)
					{
						new uid,nazwa[64],value;
						sscanf(buffer,"p<|>is[64]i",uid,nazwa,value);
						mysql_free_result();
						
						RemoveVehicleComponent(vehicleid,value);
						
						format(buffer,sizeof(buffer),"UPDATE core_items SET item_owneruid = %i, item_value2 = 0 , item_whereis = 0 WHERE item_uid = %i",GetPlayerUID(playerid),uid);
						mysql_query(buffer);
					}
					num++;
				}

				
			}
		}
		case DIAL_SET_SPAWN:
		{
			if(!response)  { }
			else
			{
				switch(listitem)
				{
					case 0:
					{
						// miasto
						new buffer[256];
						format(buffer,sizeof(buffer),"UPDATE core_players SET char_spawn = 0 WHERE char_uid=%i",GetPlayerUID(playerid));
						mysql_query(buffer);
						GuiInfo(playerid,"Zmieni³eœ typ spawnu na Market.");
					}
					case 2:
					{
						// dom
						new buffer[256];
						format(buffer,sizeof(buffer),"UPDATE core_players SET char_spawn = 1 WHERE char_uid=%i",GetPlayerUID(playerid));
						mysql_query(buffer);
						GuiInfo(playerid,"Zmieni³eœ typ spawnu na Mieszkanie.");
					}
					case 1:
					{
						//i-wood
						new buffer[256];
						format(buffer,sizeof(buffer),"UPDATE core_players SET char_spawn = %i WHERE char_uid=%i",SPAWN_TYPE_IDLEWOOD,GetPlayerUID(playerid));
						mysql_query(buffer);
						GuiInfo(playerid,"Zmieni³eœ typ spawnu na Idlewood.");
					}
				}
			}
		}
		case DIAL_OPIS_SELECT:
		{
			if(!response) {}
			else
			{
				//ustaw opis PlayerCache[playerid][pStringVal]
				//UpdateDynamic3DTextLabelText(Text3D:PlayerDesc[playerid], COLOR_ME, PlayerCache[playerid][pStringVal]);
				//PlayerDesc[playerid] = CreateDynamic3DTextLabel(" ", COLOR_ME, 0.0, 0.0, -0.6, 10.0, playerid, INVALID_VEHICLE_ID, 1);
				UpdateDynamic3DTextLabelText(Text3D:PlayerDesc[playerid], COLOR_ME, FormatDesc(PlayerCache[playerid][pStringVal]));
			}
		}
		case DIAL_OPIS_LISTA:
		{
			if(!response) { }
			else
			{
				new num = 0;
				new buffer[256];
				format(buffer,sizeof(buffer),"SELECT tresc,id FROM char_opisy WHERE owner=%i",GetPlayerUID(playerid));
				mysql_query(buffer);
				mysql_store_result();
				while(mysql_fetch_row(buffer,"|"))
				{
					if(num == listitem)
					{
						new msg[256];
						new tresc[128],id;
						sscanf(buffer,"p<|>s[128]i",tresc,id);
						format(msg,sizeof(msg),"Czy chcesz ustawiæ opis: \n%s?",tresc);
						ShowPlayerDialog(playerid,DIAL_OPIS_SELECT,DIALOG_STYLE_MSGBOX,"PotwierdŸ wybór",msg,"Tak","Nie");
						format(PlayerCache[playerid][pStringVal],128,"%s",tresc);
						break;	
					}
					num++;
				}
				mysql_free_result();
			}
		}
		case DIAL_OPIS_DELETE:
		{
			if(!response)
			{
			
			}
			else
			{
				new num = 0;
				new buffer[256];
				format(buffer,sizeof(buffer),"SELECT id FROM char_opisy WHERE owner=%i",GetPlayerUID(playerid));
				mysql_query(buffer);
				mysql_store_result();
				while(mysql_fetch_row(buffer,"|"))
				{
					if(num == listitem)
					{
						format(buffer,sizeof(buffer),"DELETE FROM char_opisy WHERE id=%i",mysql_fetch_int());
						mysql_query(buffer);
						GuiInfo(playerid,"Usun¹³eœ ten opis.");
						break;	
					}
					num++;
				}
			}
		}
		case DIAL_OPIS_CREATE_2:
		{
			if(!response) { }
			else
			{
				new string[128];
				new text1[60],text2[60];
				if(strval(inputtext) > 120)
				{
					strmid(text1,inputtext,0,60);
					strmid(text2,inputtext,61,120);
					format(string,sizeof(string),"%s\n%s",text1,text2);
				}
				else
				{
					format(string,sizeof(string),"%s",inputtext);
				}
				new buffer[256];
				new nonQuery[128];
				mysql_real_escape_string(string,nonQuery);
				format(buffer,sizeof(buffer),"INSERT INTO char_opisy VALUES(NULL,%i,'%s','%s')",GetPlayerUID(playerid),nonQuery,PlayerCache[playerid][pStringVal]);
				mysql_query(buffer);
				GuiInfo(playerid,"Doda³eœ opis.");
			}
		}
		case DIAL_OPIS_CREATE:
		{
			if(!response) { }
			else
			{
				new nonQuery[128];
				mysql_real_escape_string(inputtext,nonQuery);
				format(PlayerCache[playerid][pStringVal],128,"%s",nonQuery);
				ShowPlayerDialog(playerid,DIAL_OPIS_CREATE_2,DIALOG_STYLE_INPUT,"Opis","WprowadŸ treœæ opisu.","Ok","Anuluj");
			}
		}
		case DIAL_OPIS:
		{
			if(!response) { }
			else
			{
				switch(listitem)
				{
					case 0:		//wybierz opis
					{
						new buffer[256];
						format(buffer,sizeof(buffer),"SELECT id,title FROM char_opisy WHERE owner=%i",GetPlayerUID(playerid));
						mysql_query(buffer);
						mysql_store_result();
						if(mysql_num_rows() <=0)
						{
							mysql_free_result();
							GuiInfo(playerid,"Nie posiadasz ¿adnego opisu.");
							return 1;
						}
						else
						{
							new lista[512];
							while(mysql_fetch_row(buffer,"|"))
							{
								new id,title[32];
								sscanf(buffer,"p<|>is[32]",id,title);
								format(lista,sizeof(lista),"%s\n%i\t\t%s",lista,id,title);
							}
							mysql_free_result();
							ShowPlayerDialog(playerid,DIAL_OPIS_LISTA,DIALOG_STYLE_LIST,"System opisów » wybierz",lista,"Wybierz","Zamknij");
						}
					}
					case 1:		//dodaj opis
					{
						ShowPlayerDialog(playerid,DIAL_OPIS_CREATE,DIALOG_STYLE_INPUT,"Tytu³ opisu","WprowadŸ tytu³ opisu","Gotowe","Anuluj");
					}
					case 2:		//usun opis
					{
						new buffer[256];
						format(buffer,sizeof(buffer),"SELECT id,title FROM char_opisy WHERE owner=%i",GetPlayerUID(playerid));
						mysql_query(buffer);
						mysql_store_result();
						if(mysql_num_rows() <=0)
						{
							mysql_free_result();
							GuiInfo(playerid,"Nie posiadasz ¿adnego opisu.");
							return 1;
						}
						else
						{
							new lista[512];
							while(mysql_fetch_row(buffer,"|"))
							{
								new id,title[32];
								sscanf(buffer,"p<|>is[32]",id,title);
								format(lista,sizeof(lista),"%s\n%i\t\t%s",lista,id,title);
							}
							mysql_free_result();
							ShowPlayerDialog(playerid,DIAL_OPIS_DELETE,DIALOG_STYLE_LIST,"System opisów » usuñ",lista,"Wybierz","Zamknij");
						}
					}
					case 3:
					{
						UpdateDynamic3DTextLabelText(Text3D:PlayerDesc[playerid], COLOR_ME, " ");
						GuiInfo(playerid,"Usun¹³eœ opis.");
					}	
				}
			}
		}
		case DIAL_KUP:
		{
			if(!response) { }
			else
			{
				switch(listitem)
				{
					case 0:		//telefon
					{
						if(pInfo[playerid][pMoney] >= 150)
						{
							new telNumber = RandPhoneNumber();
							TakePlayerMoney(playerid,150);
							
							new buffer[256];
							format(buffer,sizeof(buffer),"INSERT INTO core_items(item_owneruid,item_whereis,item_value1,item_value2,item_type,item_name) VALUES (%i,%i,%i,%i,%i,'%s')",GetPlayerUID(playerid),0,telNumber,1,ITEM_PHONE,"Telefon");
							mysql_query(buffer);
							
							GuiInfo(playerid,"Kupi³eœ telefon.");
						}
						else
						{
							GuiInfo(playerid,"Nie staæ Cie na ten przedmiot.");
						}
					}
					case 1:		//papierosy
					{
						if(pInfo[playerid][pMoney] >= 15)
						{
							TakePlayerMoney(playerid,15);
							
							new buffer[256];
							format(buffer,sizeof(buffer),"INSERT INTO core_items (item_type,item_value1,item_value2,item_name,item_owneruid) VALUES(%i,%i,%i,'%s',%i)",ITEM_PAPIEROS,20,0,"Papierosy",GetPlayerUID(playerid));
							mysql_query(buffer);
						}
						else
						{
							GuiInfo(playerid,"Nie staæ Cie na ten przedmiot.");
						}
					}
					case 2:		//piwo 
					{
						if(pInfo[playerid][pMoney] >= 8)
						{
							TakePlayerMoney(playerid,8);
							new buffer[256];
							format(buffer,sizeof(buffer),"INSERT INTO core_items (item_type,item_value1,item_value2,item_name,item_owneruid) VALUES(%i,%i,%i,'%s',%i)",ITEM_WINO,1,0,"Piwo",GetPlayerUID(playerid));
							mysql_query(buffer);
						}
						else
						{
							GuiInfo(playerid,"Nie staæ Cie na ten przedmiot.");
						}
					}
					case 3:		//zegarek
					{
						if(pInfo[playerid][pMoney] >= 100)
						{
							TakePlayerMoney(playerid,100);
							new buffer[256];
							format(buffer,sizeof(buffer),"INSERT INTO core_items (item_type,item_value1,item_value2,item_name,item_owneruid) VALUES(%i,%i,%i,'%s',%i)",ITEM_CLOCK,0,0,"Zegarek",GetPlayerUID(playerid));
							mysql_query(buffer);
						}
						else
						{
							GuiInfo(playerid,"Nie staæ Cie na ten przedmiot.");
						}
					}
					case 4:		//torba
					{
						if(pInfo[playerid][pMoney] >= 80)
						{
							TakePlayerMoney(playerid,80);
							
							new buffer[256];
							format(buffer,sizeof(buffer),"INSERT INTO core_items (item_type,item_value1,item_value2,item_name,item_owneruid) VALUES(%i,%i,%i,'%s',%i)",ITEM_TORBA,1,0,"Torba",GetPlayerUID(playerid));
							mysql_query(buffer);
						}
						else
						{
							GuiInfo(playerid,"Nie staæ Cie na ten przedmiot.");
						}
					}
					case 5: //chleb
					{
						if(pInfo[playerid][pMoney] >= 5)
						{
							TakePlayerMoney(playerid,5);
							
							new buffer[256];
							format(buffer,sizeof(buffer),"INSERT INTO core_items (item_type,item_value1,item_value2,item_name,item_owneruid) VALUES(%i,%i,%i,'%s',%i)",ITEM_FOOD,3,0,"Chleb",GetPlayerUID(playerid));
							mysql_query(buffer);
						}
						else
						{
							GuiInfo(playerid,"Nie staæ Cie na ten przedmiot.");
						}
					}
				}
			}
		}
		case DIAL_AS:
		{
			if(!response)
			{
				//
			}
			else
			{
				new nieboszczyk[MAX_PLAYER_NAME+10];
				new imie[32],nazwisko[32];
				sscanf(pInfo[playerid][pName],"p<_>s[32]s[32]",imie,nazwisko);
				new buffer[256];
				
				if(PlayerCache[playerid][pShootedWeapon] <= 0)
				{
					new msg[128];
					format(msg,sizeof(msg),"%s\nPrawdopodobnie pobity na œmieræ",inputtext);
					format(nieboszczyk,sizeof(nieboszczyk),"Zwloki %s %s",imie,nazwisko);
					format(buffer,sizeof(buffer),"INSERT INTO core_items (item_type,item_value1,item_value2,item_name,item_stacjafm,item_owneruid) VALUES(%i,%i,%i,'%s','%s',%i)",ITEM_CORPSE,0,0,nieboszczyk,msg,GetPlayerUID(playerid));
				}
				else
				{
					// postrzelony
					new weapon[32];
					GetWeaponName(PlayerCache[playerid][pShootedType],weapon,sizeof(weapon));
					new msg[128];
					format(msg,sizeof(msg),"%s\nPostrza³y sugeruj¹ u¿ycie broni typu %s (( UID: %i ))",inputtext,weapon,PlayerCache[playerid][pShootedUID]);
					format(nieboszczyk,sizeof(nieboszczyk),"Zwloki %s %s",imie,nazwisko);
					format(buffer,sizeof(buffer),"INSERT INTO core_items (item_type,item_value1,item_value2,item_name,item_stacjafm,item_owneruid) VALUES(%i,%i,%i,'%s','%s',%i)",ITEM_CORPSE,0,0,nieboszczyk,msg,GetPlayerUID(playerid));
				}
				
				mysql_query(buffer);
				
				
				new Float:x,Float:y,Float:z;
				GetPlayerPos(playerid,x,y,z);
				
				for(new i = 0 ; i < MAX_PLAYER_ITEMS; i++)
				{
					if(itemInfo[playerid][i][iType] == ITEM_CORPSE)
					{
						DropPlayerItem(playerid,i);
						break;
					}
				}
				
				//wywalanie pozosta³ych rzeczy
				format(buffer,sizeof(buffer),"UPDATE core_items SET item_owneruid = 0 , item_whereis=%i,item_posx = %f,item_posy = %f,item_posz = %f WHERE item_owneruid = %i",1,x,y,z-1,GetPlayerUID(playerid));
				mysql_query(buffer);
				
				system_ck(playerid,"Zaakceptowana smierc");
			}
		}
		case DIAL_BUS:
		{
			if(!response) { }
			else
			{
				SetPlayerVirtualWorld(playerid,0);
				SetPlayerPos(playerid,BusStop[listitem][busPosX],BusStop[listitem][busPosY],BusStop[listitem][busPosZ]);
				TakePlayerMoney(playerid,BUS_COST);
			}
		}
		case DIAL_OFFER:
		{
			if(!response)
			{
				TextDrawHideForPlayer(playerid,TextDrawOffers[playerid]);
				SendClientMessage(Sprzedawca[playerid],COLOR_LIME,"Gracz odrzuci³ Twoj¹ ofertê.");
			}
			else
			{
				SendClientMessage(Sprzedawca[playerid],COLOR_LIME,"Gracz akceptowa³ Twoj¹ ofertê.");
				//akceptowal
				switch(OfferType[playerid])
				{
					case OFFER_TYPE_BITEM:
					{
						new price = OfferPrice[playerid] ;
						new sprzedawca = Sprzedawca[playerid];
						
						if(pInfo[playerid][pMoney] >= price)
						{
							new Float:workerCash = price*0.2;
							new Float:groupCash = price*0.8;
							
							TakePlayerMoney(playerid,floatround(price));
							
							AddPlayerMoney(sprzedawca,floatround(workerCash));		

							AddGroupMoney(pGrupa[sprzedawca][PlayerDutyGroup[sprzedawca]][pGuid],floatround(groupCash));	
							
							new buffer[256];
							format(buffer,sizeof(buffer),"UPDATE core_items SET item_owneruid=%i,item_whereis = 0 WHERE item_uid=%i",GetPlayerUID(playerid),SoldItemUID[playerid]);
							mysql_query(buffer);
						}
						else
						{
							SendClientMessage(Sprzedawca[playerid],COLOR_LIME,"Gracz nie ma tyle pieniêdzy.");
						}
					}
					case OFFER_TYPE_KARNET:
					{
						new price = OfferPrice[playerid] ;
						new sprzedawca = Sprzedawca[playerid];
						

						if(pInfo[playerid][pMoney] >= price)
						{
							new Float:workerCash = price*0.2;
							new Float:groupCash = price*0.8;
								
							TakePlayerMoney(playerid,price);
								
							AddPlayerMoney(sprzedawca,floatround(workerCash));		

							AddGroupMoney(pGrupa[sprzedawca][PlayerDutyGroup[sprzedawca]][pGuid],floatround(groupCash));	

							new karnetName[32];
							format(karnetName,sizeof(karnetName),"Karnet %s",pGrupa[sprzedawca][PlayerDutyGroup[sprzedawca]][pGname]);
							new silowniaUID = pGrupa[sprzedawca][PlayerDutyGroup[sprzedawca]][pGuid];
								
								
							new buffer[256];
							/*format(buffer,sizeof(buffer),"INSERT INTO core_items VALUES(NULL,%i,'%s',600,%i,'brak',%i,0,0,0,0,0,0)",
							GetPlayerUID(playerid),
							karnetName,
							silowniaUID,
							ITEM_KARNET);*/
							
							format(buffer,sizeof(buffer),"INSERT INTO core_items (item_type,item_value1,item_value2,item_name,item_owneruid) VALUES(%i,%i,%i,'%s',%i)",
							ITEM_KARNET,600,silowniaUID,karnetName,GetPlayerUID(playerid));
							
							mysql_query(buffer);
								

							TextDrawHideForPlayer(playerid,TextDrawOffers[playerid]);
							TextDrawHideForPlayer(sprzedawca,TextDrawOffers[sprzedawca]);
						}
						else
						{
							SendClientMessage(Sprzedawca[playerid],COLOR_LIME,"Gracz nie ma tyle pieniêdzy.");
						}				
					}
					case OFFER_TYPE_REPAIR_NON:
					{
						new price = OfferPrice[playerid] ;
						new sprzedawca = Sprzedawca[playerid];
						if(IsPlayerOffered(playerid))
						{
							if(pInfo[playerid][pMoney] >= price)
							{
								new Float:workerCash = price*0.2;
								//new Float:groupCash = price*0.8;
								
								TakePlayerMoney(playerid,price);
								
								AddPlayerMoney(sprzedawca,floatround(workerCash));
								
								RepairTime[sprzedawca ] = 180;
								PlayerCache[sprzedawca][pRepairCar] = 1;
								
								new vehicleid = GetPlayerVehicleID(playerid);

								SetTimerEx("DorywczoRepairVehicle", 1000, false, "ii", sprzedawca,vehicleid);

								TextDrawHideForPlayer(playerid,TextDrawOffers[playerid]);
								TextDrawHideForPlayer(sprzedawca,TextDrawOffers[sprzedawca]);
							}
							else
							{
								SendClientMessage(Sprzedawca[playerid],COLOR_LIME,"Gracz nie ma tyle pieniêdzy.");
							}
						}
						else
						{
							GuiInfo(sprzedawca,"Goœciu nie ma tyle kasy!");
						}
					}
					case OFFER_TYPE_CARDEALER:
					{
						new tablicaPojazdu = OfferColor1[playerid];
						new przychod = OfferPrice[playerid];
						new sprzedawca = Sprzedawca[playerid];
						
						if(pInfo[playerid][pMoney] >= przychod)
						{		
							TakePlayerMoney(playerid,przychod);
							
							//zmniejszanie zarobku
							przychod = floatround(przychod * 0.2);
							
							new Float:employeeCash = przychod*0.2;
							new Float:groupCash = przychod*0.8;
							
							AddPlayerMoney(sprzedawca,floatround(employeeCash));
							AddGroupMoney(pGrupa[sprzedawca][PlayerDutyGroup[sprzedawca]][pGuid],floatround(groupCash));
							
							new id = random(5);
							new buffer[256];
							
							format(buffer,sizeof(buffer),"INSERT INTO vehicles_list VALUES (NULL,%i,1000,%i,2,0,'%s',%i,%f,%f,%f,0,1,1,3,'',10,0,0,0,0,0,0,0,0,1)",0,GetPlayerUID(playerid),VehicleData[tablicaPojazdu][vName],VehicleData[tablicaPojazdu][vModel],NewVehiclePos[id][vx],NewVehiclePos[id][vy],NewVehiclePos[id][vz]);
							mysql_query(buffer);
						}
						else
						{
							SendClientMessage(Sprzedawca[playerid],COLOR_LIME,"Gracz nie ma tyle pieniêdzy.");
						}
					}
					case OFFER_TYPE_PRZEJAZD:
					{
						new przychod = OfferPrice[playerid];
						new sprzedawca = Sprzedawca[playerid];
						PlayerCache[playerid][pTaxedDriver] = Sprzedawca[playerid];
						PlayerCache[playerid][pTaxed] = true;
						
						if(pInfo[playerid][pMoney] >= przychod)
						{
							new Float:employeeCash = przychod*0.2;
							new Float:groupCash = przychod*0.8;
													
							TakePlayerMoney(playerid,przychod);
							AddPlayerMoney(sprzedawca,floatround(employeeCash));
							AddGroupMoney(pGrupa[sprzedawca][PlayerDutyGroup[sprzedawca]][pGuid],floatround(groupCash));
						}
						else
						{
							SendClientMessage(Sprzedawca[playerid],COLOR_LIME,"Gracz nie ma tyle pieniêdzy.");
						}
					}
					case OFFER_TYPE_MONTAZ:
					{
						new sprzedawca = Sprzedawca[playerid];
						new przychod = OfferPrice[playerid];
						new partUID = SoldItemUID[playerid];
						new vehicleid = OfferColor1[playerid];
						
						if(pInfo[playerid][pMoney] >= przychod)
						{
							new buffer[256];
							format(buffer,sizeof(buffer),"UPDATE core_items SET item_owneruid = 0 , item_value2 = %i WHERE item_uid=%i",vehicle[vehicleid][vuid],partUID);
							mysql_query(buffer);
							
							format(buffer,sizeof(buffer),"SELECT item_value1 FROM core_items WHERE item_uid=%i",partUID);
							mysql_query(buffer);
							mysql_store_result();
							new component = mysql_fetch_int();
							mysql_free_result();
							
							AddVehicleComponent(vehicleid, component);
							
							TakePlayerMoney(playerid,przychod);
							
							new Float:employeeCash = przychod*0.2;
							new Float:groupCash = przychod*0.8;
							
							AddPlayerMoney(sprzedawca,floatround(employeeCash));
							AddGroupMoney(pGrupa[sprzedawca][PlayerDutyGroup[sprzedawca]][pGuid],floatround(groupCash));
						}
						else
						{
							SendClientMessage(Sprzedawca[playerid],COLOR_LIME,"Gracz nie ma tyle pieniêdzy.");
						}
					}
					case OFFER_TYPE_MANDAT:
					{
						new price = OfferPrice[playerid];
						if(pInfo[playerid][pMoney] >= price)
						{
							new sprzedawca = Sprzedawca[playerid];
							new pktKarne = OfferColor1[playerid];
							//reason stringval cache player
							
							new przychod = OfferPrice[playerid];
							new Float:employeeCash = przychod*0.2;
							new Float:groupCash = przychod*0.8;
							
							AddPlayerMoney(sprzedawca,floatround(employeeCash));
							TakePlayerMoney(playerid,przychod);
							AddGroupMoney(pGrupa[sprzedawca][PlayerDutyGroup[sprzedawca]][pGuid],floatround(groupCash));
							
							new buffer[256];
							format(buffer,sizeof(buffer),"INSERT INTO lspd_kartoteka VALUES(NULL,%i,'%s',%i,%i)",GetPlayerUID(playerid),PlayerCache[playerid][pStringVal],przychod,pktKarne);
							mysql_query(buffer);
						}
						else
						{
							GuiInfo(playerid,"Nie masz tyle kasy.");
							SendClientMessage(Sprzedawca[playerid],COLOR_LIME,"Gracz nie ma tyle pieniêdzy.");
							
						}
					}
					case OFFER_TYPE_UNBLOCK:
					{
						new price = OfferPrice[playerid];
						if(pInfo[playerid][pMoney] >= price)
						{
							new vehicleid = SoldItemUID[playerid];
							new sprzedawca = Sprzedawca[playerid];
							
							new przychod = OfferPrice[playerid];
							new Float:employeeCash = przychod*0.2;
							new Float:groupCash = przychod*0.8;
							
							AddPlayerMoney(sprzedawca,floatround(employeeCash));
							TakePlayerMoney(playerid,przychod);
							AddGroupMoney(pGrupa[sprzedawca][PlayerDutyGroup[sprzedawca]][pGuid],floatround(groupCash));
							
							new buffer[256];
							format(buffer,sizeof(buffer),"DELETE FROM veh_blocks WHERE uid=%i",vehicle[vehicleid][vuid]);
							mysql_query(buffer);
							
							GuiInfo(sprzedawca,"Blokada zdjêta.");
						}
					}
					case OFFER_TYPE_REJESTRACJA:
					{
						new price = OfferPrice[playerid];
						if(pInfo[playerid][pMoney] >= price)
						{
							new vehicleid = SoldItemUID[playerid];
							new sprzedawca = Sprzedawca[playerid];
							
							new przychod = OfferPrice[playerid];
							new Float:employeeCash = przychod*0.2;
							new Float:groupCash = przychod*0.8;
							
							AddPlayerMoney(sprzedawca,floatround(employeeCash));
							TakePlayerMoney(playerid,przychod);
							AddGroupMoney(pGrupa[sprzedawca][PlayerDutyGroup[sprzedawca]][pGuid],floatround(groupCash));
							
							new buffer[256];
							new vehicleuid = vehicle[vehicleid][vuid];
							
							new rejestracja[32];
							format(rejestracja,sizeof(rejestracja),"LS%i",vehicleuid);
							format(buffer,sizeof(buffer),"UPDATE vehicles_list SET veh_plates='%s' WHERE veh_uid=%i",rejestracja,vehicleuid);
							mysql_query(buffer);
						}
						
						new sprzedawca2 = Sprzedawca[playerid];
						TextDrawHideForPlayer(playerid,TextDrawOffers[playerid]);
						TextDrawHideForPlayer(sprzedawca2,TextDrawOffers[sprzedawca2]);
					}
					case OFFER_TYPE_REPAINT:
					{
						new price = OfferPrice[playerid];
						if(pInfo[playerid][pMoney] >= price)
						{
							new col1 = OfferColor1[playerid];
							new col2 = OfferColor2[playerid];
							new sprzedawca = Sprzedawca[playerid];
							
							new Float:groupMoney = price*0.85;
							new Float:playerMoney = price *0.15;
							
							TakePlayerMoney(playerid,price);
							AddPlayerMoney(sprzedawca,floatround(playerMoney));
							AddGroupMoney(pGrupa[sprzedawca][PlayerDutyGroup[sprzedawca]][pGuid],floatround(groupMoney));
							
							PlayerCache[playerid][pLakieruje] = true;
							
							RepaintTime[sprzedawca] = 100;
							
							SetTimerEx("PlayerRepaintVehicle", 1000, false, "iiii", sprzedawca,SoldItemUID[playerid],col1,col2);
							
						}
						else
						{
							GuiInfo(playerid,"Nie masz tyle kasy.");
						}
						new sprzedawca2 = Sprzedawca[playerid];
						TextDrawHideForPlayer(playerid,TextDrawOffers[playerid]);
						TextDrawHideForPlayer(sprzedawca2,TextDrawOffers[sprzedawca2]);
					}
					case OFFER_TYPE_PRAWKO:
					{
						new prawkoType = SoldItemUID[playerid];			//czemu akurat ta zmienna? nie wiem, po prostu nie chcialem tworzyæ kolejnej
						new price = OfferPrice[playerid];
						new sprzedawca = Sprzedawca[playerid];
						
						if(IsPlayerOffered(playerid))
						{
							if(pInfo[playerid][pMoney] >= price)
							{
								//sprzedano
								TakePlayerMoney(playerid,price);
								new Float:sprzedawcaMoney = price*0.2;
								new Float:groupMoney = price*0.8;
								
								AddPlayerMoney(sprzedawca,floatround(sprzedawcaMoney));
								AddGroupMoney(pGrupa[sprzedawca][PlayerDutyGroup[sprzedawca]][pGuid],floatround(groupMoney));
								
								//nadaj prawko
								new buffer[256];
								switch(prawkoType)
								{
									case 1:	//a
									{
										pInfo[playerid][pHasPrawkoA] = 1;
										format(buffer,sizeof(buffer),"UPDATE core_players SET prawkoa = 1 WHERE char_uid=%i",GetPlayerUID(playerid));
									}
									
									case 2:	//b
									{
										pInfo[playerid][pHasPrawkoB] = 1;
										format(buffer,sizeof(buffer),"UPDATE core_players SET prawkob = 1 WHERE char_uid=%i",GetPlayerUID(playerid));
									}
									
									case 3:	//c
									{
										pInfo[playerid][pHasPrawkoC] = 1;
										format(buffer,sizeof(buffer),"UPDATE core_players SET prawkoc = 1 WHERE char_uid=%i",GetPlayerUID(playerid));
									}
									
									case 4:	//ce
									{
										pInfo[playerid][pHasPrawkoCE] = 1;
										format(buffer,sizeof(buffer),"UPDATE core_players SET prawkoce = 1 WHERE char_uid=%i",GetPlayerUID(playerid));
									}
									
									case 5:	//d
									{
										pInfo[playerid][pHasPrawkoD] = 1;
										format(buffer,sizeof(buffer),"UPDATE core_players SET prawkod = 1 WHERE char_uid=%i",GetPlayerUID(playerid));
									}
								}
								
								TextDrawHideForPlayer(playerid,TextDrawOffers[playerid]);
								TextDrawHideForPlayer(sprzedawca,TextDrawOffers[sprzedawca]);
								mysql_query(buffer);
							}
							else
							{
								GuiInfo(playerid,"Nie masz tyle pieniêdzy.");
							}
						}
					}
					case OFFER_TYPE_ITEM:
					{
						new itemuid = SoldItemUID[playerid];
						new price = OfferPrice[playerid];
						new sprzedawca = Sprzedawca[playerid];
						
						if(IsPlayerOffered(playerid))
						{
							if(pInfo[playerid][pMoney] >= price)
							{
								TakePlayerMoney(playerid,price);
								AddPlayerMoney(sprzedawca,price);
								//new vehicleid = 0;
								
								GuiInfo(playerid,"Kupileœ przedmiot.");
								GuiInfo(sprzedawca,"Sprzeda³eœ przedmiot.");						
								
								printf("To who: %i || Item UID: %i",playerid,itemuid);
								new owneruid = pInfo[playerid][pUID];
								printf("UID NOWEGO W£AŒÆ: %i",owneruid);
								new buffer[256];
								format(buffer,sizeof(buffer),"UPDATE core_items SET item_owneruid=%i , item_whereis=%i WHERE item_uid=%i",owneruid,WHEREIS_GRACZ,itemuid);
								mysql_query(buffer);
								
								TextDrawHideForPlayer(playerid,TextDrawOffers[playerid]);
								TextDrawHideForPlayer(sprzedawca,TextDrawOffers[sprzedawca]);
							}
							else
							{
								GuiInfo(playerid,"Nie masz tyle pieniêdzy.");
								return 1;
							}
						}
					}
					case OFFER_TYPE_CAR:
					{
						new vehuid = SoldVehicle[playerid];
						new price = OfferPrice[playerid] ;
						new sprzedawca = Sprzedawca[playerid];
						
						if(IsPlayerOffered(playerid))
						{
							if(pInfo[playerid][pMoney] >= price)
							{
								TakePlayerMoney(playerid,price);
								AddPlayerMoney(sprzedawca,price);
								new vehicleid = 0;
								for(new i = 0;i < MAX_VEHICLES; i++)
								{
									if(vehicle[i][vuid] == vehuid)
									{
										vehicleid = i;
										break;
									}
								}
								vehicle[vehicleid][vowneruid] = GetPlayerUID(playerid);
								TransferVehicle(playerid,vehicleid);
								ClearPlayerVehiclesTable(playerid);
								ClearPlayerVehiclesTable(sprzedawca);
								
								SpawnedVehicle[playerid] = SearchSpawnedVehicle(playerid);
								SpawnedVehicle[sprzedawca] = SearchSpawnedVehicle(sprzedawca);
								
								GuiInfo(playerid,"Kupi³eœ pojazd.");
								GuiInfo(sprzedawca,"Sprzeda³eœ pojazd.");
								
								TextDrawHideForPlayer(playerid,TextDrawOffers[playerid]);
								TextDrawHideForPlayer(sprzedawca,TextDrawOffers[sprzedawca]);
							}
							else
							{
								GuiInfo(playerid,"Nie masz tyle pieniêdzy.");
								return 1;
							}
						}
					}
					case OFFER_TYPE_HEALING:
					{
						new price = OfferPrice[playerid] ;
						if(IsPlayerOffered(playerid))
						{
							if(pInfo[playerid][pMoney] >= price)
							{
								new sprzedawca = Sprzedawca[playerid];
								TakePlayerMoney(playerid,price);
								AddPlayerMoney(sprzedawca,price);
								SetTimerEx("Healing", 3000, 0, "d", playerid);
								GameTextForPlayer(playerid, "~g~proces leczenia trwa", 5000, 5);
								TextDrawHideForPlayer(playerid,TextDrawOffers[playerid]);
								TextDrawHideForPlayer(sprzedawca,TextDrawOffers[sprzedawca]);
							}
							else
							{
								GuiInfo(playerid,"Nie masz tyle pieniêdzy.");
								return 1;
							}
						}
					}
					case OFFER_TYPE_REPAIR:
					{
						new price = OfferPrice[playerid] ;
						if(IsPlayerOffered(playerid))
						{
							if(pInfo[playerid][pMoney] >= price)
							{
								new sprzedawca = Sprzedawca[playerid];			//id mechanika
								new Float:workerCash = price*0.1;
								new Float:groupCash = price*0.9;

								
								TakePlayerMoney(playerid,price);
								AddPlayerMoney(sprzedawca,floatround(workerCash));
								AddGroupMoney(pGrupa[sprzedawca][PlayerDutyGroup[sprzedawca]][pGuid],floatround(groupCash));
								
								GuiInfo(playerid,"Zap³aci³eœ za naprawê, mechanik rozpoczyna w³aœnie naprawê.");
								GuiInfo(sprzedawca,"Otrzyma³eœ zap³atê, przejdŸ teraz do naprawy.");
								
								new vehicleid = GetPlayerVehicleID(playerid);
								
								RepairTime[sprzedawca ] = 180;
								PlayerCache[sprzedawca][pRepairCar] = 1;
								SetTimerEx("PlayerRepairVehicle", 1000, false, "ii", sprzedawca,vehicleid);
								
								TextDrawHideForPlayer(playerid,TextDrawOffers[playerid]);
								TextDrawHideForPlayer(sprzedawca,TextDrawOffers[sprzedawca]);
							}
							else
							{
								GuiInfo(playerid,"Nie masz tyle pieniêdzy.");
								return 1;
							}
						}
					}
				}
				new sprzedawca2 = Sprzedawca[playerid];
				TextDrawHideForPlayer(playerid,TextDrawOffers[playerid]);
				TextDrawHideForPlayer(sprzedawca2,TextDrawOffers[sprzedawca2]);
			}
		}
		case DIAL_DOOR_SET_NAME:
		{
			if(!response) { }
			else
			{
				new doorid = GetPlayerDoorID(playerid);
				if(IsPlayerPermsDoors(playerid,doorid))
				{
					new buffer[256];
					new antyMySQL[128];
					mysql_real_escape_string(inputtext,antyMySQL);
					if(!inputtext[0])
					{
						GuiInfo(playerid,"No jak¹œ nazwê to powinny jednak mieæ.");
						return 1;
					}
					
					format(buffer,sizeof(buffer),"UPDATE doors SET name='%s' WHERE uid=%i",antyMySQL,DoorInfo[doorid][doorUID]);
					mysql_query(buffer);
					format(DoorInfo[doorid][doorName],64,"%s",inputtext);
					GuiInfo(playerid,"Zmieni³eœ nazwê tych drzwi.");
				}
			}
		}
		case DIAL_DOOR_SET_MUZYKA:
		{
			if(!response) { }
			else
			{
				new doorid = GetPlayerDoorID(playerid);
				if(IsPlayerPermsDoors(playerid,doorid))
				{
					if(!inputtext[0])
					{
						new buffer[256];
						format(buffer,sizeof(buffer),"UPDATE doors SET url='brak' WHERE uid=%i",DoorInfo[doorid][doorUID]);
						mysql_query(buffer);
						GuiInfo(playerid,"Wy³¹czy³eœ muzykê w drzwiach.");
						format(DoorInfo[doorid][doorMuzyka],128,"brak");
					}
					else
					{
						new buffer[256];
						new antyMySQL[128];
						mysql_real_escape_string(inputtext,antyMySQL);
						format(buffer,sizeof(buffer),"UPDATE doors SET url='%s' WHERE uid=%i",antyMySQL,DoorInfo[doorid][doorUID]);
						mysql_query(buffer);
						GuiInfo(playerid,"W³¹czy³eœ muzykê w drzwiach.");
						format(DoorInfo[doorid][doorMuzyka],128,inputtext);
					}
				}
			}
		}
		case DIAL_DOOR_SET_ENTER:
		{
			if(!response) { }
			else
			{
				new doorid = GetPlayerDoorID(playerid);
				if(IsPlayerPermsDoors(playerid,doorid))
				{
					new amount = strval(inputtext);
					if(amount <= 0)
					{
						new buffer[256];
						format(buffer,sizeof(buffer),"UPDATE doors SET entrycost = 0 WHERE uid=%i",DoorInfo[doorid][doorUID]);
						mysql_query(buffer);
						GuiInfo(playerid,"Usuniêto cenê wejœcia do Twoich drzwi.");
						DoorInfo[doorid][doorEntry] = 0;
					}
					else
					{
						new buffer[256];
						format(buffer,sizeof(buffer),"UPDATE doors SET entrycost = %i WHERE uid=%i",amount,DoorInfo[doorid][doorUID]);
						mysql_query(buffer);
						format(buffer,sizeof(buffer),"Nowa cena wejœciówki to: %i$",amount);
						GuiInfo(playerid,buffer);
						DoorInfo[doorid][doorEntry] = amount;
					}
				}
			}
		}
		case DIAL_DOOR_SET_PRZEJAZD:
		{
			if(!response) { }
			else
			{
				new doorid = GetPlayerDoorID(playerid);
				if(IsPlayerPermsDoors(playerid,doorid))
				{
					switch(listitem)
					{
						case 0:		//wl
						{
							new buffer[256];
							format(buffer,sizeof(buffer),"UPDATE doors SET cars = 1 WHERE uid=%i",DoorInfo[doorid][doorUID]);
							mysql_query(buffer);
							DoorInfo[doorid][doorCars] = 1;
							GuiInfo(playerid,"W³¹czy³eœ mo¿liwoœæ przejazdu dla pojazdów.");
						}
						case 1:			//wyl
						{
							new buffer[256];
							format(buffer,sizeof(buffer),"UPDATE doors SET cars = 0 WHERE uid=%i",DoorInfo[doorid][doorUID]);
							mysql_query(buffer);
							DoorInfo[doorid][doorCars] = 0;
							GuiInfo(playerid,"Wy³¹czy³eœ mo¿liwoœæ przejazdu dla pojazdów.");
						}
					}
				}
			}
		}
		case DIAL_DOOR_OPTIONS:
		{
			if(!response) { }
			else
			{
				new doorid = GetPlayerDoorID(playerid);
				if(IsPlayerPermsDoors(playerid,doorid))
				{
					switch(listitem)
					{
						case 0:			//nazwa
						{
							ShowPlayerDialog(playerid,DIAL_DOOR_SET_NAME,DIALOG_STYLE_INPUT,"Nazwa drzwi","Ustal now¹ nazwê dla tych drzwi.","Gotowe","Zakoncz");
						}
						case 1:			//cena
						{
							ShowPlayerDialog(playerid,DIAL_DOOR_SET_ENTER,DIALOG_STYLE_INPUT,"Wejœciówka","Ustal cenê wejœciówki.","Gotowe","Zakoncz");
						}
						case 2:			//muzyka
						{
							ShowPlayerDialog(playerid,DIAL_DOOR_SET_MUZYKA,DIALOG_STYLE_INPUT,"Muzyka w interiorze","WprowadŸ adres do audio streama.","Gotowe","Zakoncz");
						}
						case 3:			//przejazd
						{	
							ShowPlayerDialog(playerid,DIAL_DOOR_SET_PRZEJAZD,DIALOG_STYLE_LIST,"Przejazd przez drzwi","1. W³¹cz\n2. Wy³¹cz","Gotowe","Zakoncz");
						}
					}
				}
			}
		}
		case DIAL_HELP:
		{
			if(!response) { }
			else
			{
				new text[512];
				switch(listitem)
				{
					case 0:			//ogólne
					{
						format(text,sizeof(text),"To poradnik dotycz¹cy u¿ywania oprogramowania na tym serwerze, poszczególne kategorie wyjaœni¹ Ci podstawowe zagadnienia dot. komend.");
						ShowPlayerDialog(playerid,DIAL_HELP_SEL,DIALOG_STYLE_MSGBOX,"Pomoc - ogólne",text,"OK","");
					}
					case 1:			//obiekty
					{
						return cmd_ohelp(playerid,text);
					}
					case 2:			//pojazdy
					{
						format(text,sizeof(text),"Komenda przydatna w obs³udze systemu pojazdów:\n /v [lista | zamknij | zaparkuj]\nMo¿esz posiadaæ maksymalnie piêæ pojazdów w tym jeden zespawnowany.");
						ShowPlayerDialog(playerid,DIAL_HELP_SEL,DIALOG_STYLE_MSGBOX,"Pomoc - pojazdy",text,"OK","");
					}
					case 3:			//grupy
					{
						format(text,sizeof(text),"Komenda przydatna w obs³udze systemu grup:\n /g [lista | zapros | wypros]\nMaksymalnie mo¿esz posiadaæ piêæ grup.");
						ShowPlayerDialog(playerid,DIAL_HELP_SEL,DIALOG_STYLE_MSGBOX,"Pomoc - grupy",text,"OK","");
					}
					case 4:			//drzwi
					{
						format(text,sizeof(text),"Komenda przydatna w obs³udze systemu drzwi:\n /drzwi [zamknij | opcje | schowek]\nW jednym momencie mo¿esz posiadaæ jedno mieszkanie, wiêcej przypisanych pod Ciebie mo¿e powodowaæ b³êdy.");
						ShowPlayerDialog(playerid,DIAL_HELP_SEL,DIALOG_STYLE_MSGBOX,"Pomoc - drzwi",text,"OK","");
					}
					case 5:			//przedmioty
					{
						format(text,sizeof(text),"Komenda przydatna w obs³udze systemu przedmiotów:\n /p [podnies]]\nMo¿esz posiadaæ maksymalnie 30 przedmiotów.");
						ShowPlayerDialog(playerid,DIAL_HELP_SEL,DIALOG_STYLE_MSGBOX,"Pomoc - przedmioty",text,"OK","");
					}
					case 6:			//ac
					{
					
					}
					case 7:			// o gamemode
					{
						//format(text,sizeof(text),"Autorem tego oprogramowania jest blint.\n"COL_GM"mySantos 1.0dev");
						//ShowPlayerDialog(playerid,DIAL_HELP_SEL,DIALOG_STYLE_MSGBOX,"Autor",text,"OK","");
					}
				}
			}
		}
		case DIAL_BANK_WPLAC:
		{
			if(!response) { }
			else
			{
				new amount = strval(inputtext);
				if(amount <=0 || strlen(inputtext) > 11)
				{	
					GuiInfo(playerid,"Wprowadzono nieprawid³ow¹ kwotê.");
					return 1;
				}
				if(amount > pInfo[playerid][pMoney])
				{
					GuiInfo(playerid,"Nie posiadasz przy sobie takiej iloœci gotówki.");
					return 1;
				}
				else
				{
					new buffer[256];
					format(buffer,sizeof(buffer),"UPDATE core_players SET char_money=char_money-%i ,char_bank=char_bank+%i WHERE char_uid=%i",amount,amount,GetPlayerUID(playerid));
					mysql_query(buffer);
					pInfo[playerid][pMoney] = pInfo[playerid][pMoney] - amount;
					ResetPlayerMoney(playerid);
					GivePlayerMoney(playerid,pInfo[playerid][pMoney]);
					pInfo[playerid][pBank] = pInfo[playerid][pBank]+amount;
					new ou[128];
					format(ou,sizeof(ou),"Wp³aci³eœ do banku %i$, nowy stan konta to : %i$",amount,pInfo[playerid][pBank]);
					GuiInfo(playerid,ou);
					return 1;
				}
			}
		}
		case DIAL_BANK_WYPLAC:
		{
			if(!response) { } 
			else
			{
				new amount = strval(inputtext);
				if(amount <=0 || strlen(inputtext) > 11)
				{
					GuiInfo(playerid,"Wprowadzono nieprawid³ow¹ kwotê.");
					return 1;
				}
				if(amount > pInfo[playerid][pBank])
				{
					GuiInfo(playerid,"Nie posiadasz w banku takiej iloœci gotówki.");
					return 1;
				}
				else
				{
					new buffer[256];
					pInfo[playerid][pMoney] = pInfo[playerid][pMoney] + amount;
					pInfo[playerid][pBank] = pInfo[playerid][pBank] - amount;
					ResetPlayerMoney(playerid);
					GivePlayerMoney(playerid,pInfo[playerid][pMoney]);
					format(buffer,sizeof(buffer),"UPDATE core_players SET char_money=char_money+%i , char_bank = char_bank - %i WHERE char_uid=%i",amount,amount,GetPlayerUID(playerid));
					mysql_query(buffer);
					new ou[128];
					format(ou,sizeof(ou),"Wyp³aci³eœ z banku %i$, nowy stan konta to : %i$",amount,pInfo[playerid][pBank]);
					GuiInfo(playerid,ou);
					return 1;
				}
			}
		}
		case DIAL_BANK_PRZELEW_ILOSC:
		{
			if(!response) { }
			else
			{
				new buffer[256];
				format(buffer,sizeof(buffer),"SELECT char_time FROM core_players WHERE char_uid=%i",GetPlayerUID(playerid));
				mysql_query(buffer);
				mysql_store_result();
				new time = mysql_fetch_int();
				mysql_free_result();
				if(time < 10800)
				{
					GuiInfo(playerid,"Nie masz trzech godzin wymaganych do u¿ycia tej komendy.");
					return 1;
				}
				
				new ilosc = strval(inputtext);
				if(ilosc > pInfo[playerid][pBank])
				{
					GuiInfo(playerid,"Nie posiadasz tyle pieniêdzy w banku.");
					return 1;
				}
				if(ilosc <= 0)
				{
					GuiInfo(playerid,"Wprowadzono nieprawid³ow¹ kwotê.");
					return 1;
				}
				
				//new buffer[256];
				format(buffer,sizeof(buffer),"SELECT char_uid FROM core_players WHERE char_banknr=%i",SendMoneyTo[playerid]);
				mysql_query(buffer);
				mysql_store_result();
				new uid = mysql_fetch_int();
				mysql_free_result();
				
				new player = 0;
				
				for(new i = 0;i < MAX_PLAYERS; i++)
				{
					if(pInfo[i][pBankNR] == SendMoneyTo[playerid])
					{
						player = i;
						break;
					}
				}
				
				if(player > 0)
				{
					pInfo[player][pBank] = pInfo[player][pBank] + ilosc;
				}
				
				format(buffer,256,"UPDATE core_players SET char_bank=char_bank+%i WHERE char_uid=%i",ilosc,uid);
				mysql_query(buffer);
				
				format(buffer,256,"UPDATE core_players SET char_bank=char_bank-%i WHERE char_uid=%i",ilosc,pInfo[playerid][pUID]);
				mysql_query(buffer);
				pInfo[playerid][pBank] = pInfo[playerid][pBank] - ilosc;
				
				GuiInfo(playerid,"Dokona³eœ przelewu.");
			}
		}
		case DIAL_BANK_PRZELEW:
		{
			if(!response) { }
			else
			{
				new banknr = strval(inputtext);
				if(banknr <= 0)
				{
					GuiInfo(playerid,"Wprowadzono z³y numer konta bankowego.");
					return 1;
				}
				if(banknr == pInfo[playerid][pBankNR])
				{
					GuiInfo(playerid,"Sam do siebie nie mo¿esz przelewaæ kasy.");
					return 1;
				}
				if(banknr < 0)
				{
					GuiInfo(playerid,"Wprowadzono nieprawid³owy numer konta.");
					return 1;
				}
				else
				{
					new buffer[256];
					format(buffer,sizeof(buffer),"SELECT * FROM core_players WHERE char_banknr=%i",banknr);
					mysql_query(buffer);
					mysql_store_result();
					if(mysql_num_rows() > 0)
					{
						mysql_free_result();
						ShowPlayerDialog(playerid,DIAL_BANK_PRZELEW_ILOSC,DIALOG_STYLE_INPUT,"Przelew","Wpisz poni¿ej kwotê pieniêdzy, które chcesz przelaæ.","Gotowe","Anuluj");
						SendMoneyTo[playerid] = banknr;
					}
					else
					{
						mysql_free_result();
						GuiInfo(playerid,"Takie konto nie istnieje.");
					}
				}
			}
		}
		case DIAL_BANK:
		{
			if(!response) { }
			else
			{
				switch(listitem)
				{
					case 0:		//wplac
					{
						ShowPlayerDialog(playerid,DIAL_BANK_WPLAC,DIALOG_STYLE_INPUT,"Bank » wp³aæ pieni¹dze","Wpisz poni¿ej kwotê jak¹ chcesz wp³aciæ do banku...","Gotowe","Anuluj");
					}
					case 1:		//wyplac
					{
						ShowPlayerDialog(playerid,DIAL_BANK_WYPLAC,DIALOG_STYLE_INPUT,"Bank » wyp³aæ pieni¹dze","Wpisz poni¿ej kwotê jak¹ chcesz wyp³aciæ z banku...","Gotowe","Anuluj");
					}
					case 2:		//przelew
					{
						ShowPlayerDialog(playerid,DIAL_BANK_PRZELEW,DIALOG_STYLE_INPUT,"Bank » dokonaj przelewu","Wpisz poni¿ej numer konta bankowego, na który chcesz przelaæ pieni¹dze.","Gotowe","Zakoncz");
					}
					case 3:		//stan konta
					{
						new stan[256];
						format(stan,sizeof(stan),"Stan konta: ~g~%i$",pInfo[playerid][pBank]);
						GameTextForPlayer(playerid, stan, 7000, 5);
					}
				}
			}
		}
		case DIAL_PURCHASE_ATTACH:
		{
			if(!response) { }
			else
			{
				new attid = PlayerCache[playerid][pSelAttach] ;
				new price = AttachItems[attid][aPrice];
				if(pInfo[playerid][pMoney] >= price)
				{
					TakePlayerMoney(playerid,price);
					CreateAttachedItem(playerid,AttachItems[attid][aModel],AttachItems[attid][aBone],AttachItems[attid][aName]);
				}
				else
				{
					GuiInfo(playerid,"Nie posiadasz tyle pieniêdzy.");
					return 1;
				}
				
				vlive_err(playerid,"zakupi³eœ to akcesorium");
				SetCameraBehindPlayer(playerid);
				TogglePlayerControllable(playerid,1);
				PlayerCache[playerid][pSelAttach] = 0;
				TextDrawHideForPlayer(playerid,TextDrawSkins[playerid]);
				RemovePlayerAttachedObject(playerid,SLOT_CACHE);
				PlayerCache[playerid][pBuyAttach] = 0;
				
				TextDrawHideForPlayer(playerid,TextDrawSkins[playerid]);
				TogglePlayerControllable(playerid,1);
			}
		}
		case DIAL_PURCHASE_CLOATH:
		{
			if(!response) { }
			else
			{
				new price = SkinInfo[SelectedSkin[playerid]][sPrice];
				if(pInfo[playerid][pMoney] >= price)
				{
					pInfo[playerid][pMoney] = pInfo[playerid][pMoney] - price;
					new buffer[256];
					format(buffer,sizeof(buffer),"UPDATE core_players SET char_money=%i WHERE char_uid=%i",pInfo[playerid][pMoney],GetPlayerUID(playerid));
					mysql_query(buffer);
					
					format(buffer,sizeof(buffer),"INSERT INTO core_items(item_owneruid,item_whereis,item_value1,item_type,item_name) VALUES(%i,%i,%i,%i,'%s')",GetPlayerUID(playerid),0,SkinInfo[SelectedSkin[playerid]][sModel],ITEM_CLOTH,SkinInfo[SelectedSkin[playerid]][sName]);
					mysql_query(buffer);
				}
				else
				{
					GuiInfo(playerid,"Nie posiadasz tyle pieniêdzy.");
					return 1;
				}
				
				vlive_err(playerid,"kupi³eœ to ubranie, pojawi³o siê w Twoim ekwipunku");
				
				PlayerCache[playerid][pSelSkin] = false;
				SetCameraBehindPlayer(playerid);
				SetPlayerSkin(playerid,pInfo[playerid][pSkin]);
				TextDrawHideForPlayer(playerid,TextDrawSkins[playerid]);
				TogglePlayerControllable(playerid,1);
			}
		}
		case DIAL_G_TYPE:
		{
			if(!response) { }
			else
			{
				new guid = SelectedAdminGroup[playerid];
				switch(listitem)
				{
					case 0:	//warsztat
					{
						new buffer[256];
						format(buffer,sizeof(buffer),"UPDATE groups_list SET group_type=%i WHERE group_uid=%i",TYPE_WORKSHOP,guid);
						mysql_query(buffer);
					}
					case 1:	//restauracja
					{
						new buffer[256];
						format(buffer,sizeof(buffer),"UPDATE groups_list SET group_type=%i WHERE group_uid=%i",TYPE_RESTAURANT,guid);
						mysql_query(buffer);
					}
					case 2:	//salon
					{
						new buffer[256];
						format(buffer,sizeof(buffer),"UPDATE groups_list SET group_type=%i WHERE group_uid=%i",TYPE_CARDEALER,guid);
						mysql_query(buffer);
					}
					case 3:	//silka
					{
						new buffer[256];
						format(buffer,sizeof(buffer),"UPDATE groups_list SET group_type=%i WHERE group_uid=%i",TYPE_GYM,guid);
						mysql_query(buffer);
					}
					case 4:	//fbi
					{
						new buffer[256];
						format(buffer,sizeof(buffer),"UPDATE groups_list SET group_type=%i WHERE group_uid=%i",TYPE_FBI,guid);
						mysql_query(buffer);
					}
					case 5:	//san news
					{
						new buffer[256];
						format(buffer,sizeof(buffer),"UPDATE groups_list SET group_type=%i WHERE group_uid=%i",TYPE_SANNEWS,guid);
						mysql_query(buffer);
					}
					case 6:	//mafia
					{
						new buffer[256];
						format(buffer,sizeof(buffer),"UPDATE groups_list SET group_type=%i WHERE group_uid=%i",TYPE_MAFIA,guid);
						mysql_query(buffer);
					}
					case 7:	//org
					{
						new buffer[256];
						format(buffer,sizeof(buffer),"UPDATE groups_list SET group_type=%i WHERE group_uid=%i",TYPE_ORG,guid);
						mysql_query(buffer);
					}
					case 8: //sciganci
					{
						new buffer[256];
						format(buffer,sizeof(buffer),"UPDATE groups_list SET group_type=%i WHERE group_uid=%i",TYPE_SCIGANT,guid);
						mysql_query(buffer);
					}
					case 9: //szajka
					{
						new buffer[256];
						format(buffer,sizeof(buffer),"UPDATE groups_list SET group_type=%i WHERE group_uid=%i",TYPE_SZAJK,guid);
						mysql_query(buffer);
					}
					case 10:	//lsfd
					{
						new buffer[256];
						format(buffer,sizeof(buffer),"UPDATE groups_list SET group_type=%i WHERE group_uid=%i",TYPE_LSFD,guid);
						mysql_query(buffer);
					}
					case 11:	//lspd
					{
						new buffer[256];
						format(buffer,sizeof(buffer),"UPDATE groups_list SET group_type=%i WHERE group_uid=%i",TYPE_LSPD,guid);
						mysql_query(buffer);
					}
					case 12://lsmc
					{
						new buffer[256];
						format(buffer,sizeof(buffer),"UPDATE groups_list SET group_type=%i WHERE group_uid=%i",TYPE_LSMC,guid);
						mysql_query(buffer);
					}
					case 13://gov
					{
						new buffer[256];
						format(buffer,sizeof(buffer),"UPDATE groups_list SET group_type=%i WHERE group_uid=%i",TYPE_GOV,guid);
						mysql_query(buffer);
					}
					case 14:	//brak
					{
						new buffer[256];
						format(buffer,sizeof(buffer),"UPDATE groups_list SET group_type=%i WHERE group_uid=%i",TYPE_NONE,guid);
						mysql_query(buffer);
					}
					case 15:	//ochroniarze
					{
						new buffer[256];
						format(buffer,sizeof(buffer),"UPDATE groups_list SET group_type=%i WHERE group_uid=%i",TYPE_OCHRONIARZE,guid);
						mysql_query(buffer);
					}
					case 16:	//taxi
					{
						new buffer[256];
						format(buffer,sizeof(buffer),"UPDATE groups_list SET group_type=%i WHERE group_uid=%i",TYPE_TAXI,guid);
						mysql_query(buffer);
					}
					case 17:	//coffee
					{
						new buffer[256];
						format(buffer,sizeof(buffer),"UPDATE groups_list SET group_type=%i WHERE group_uid=%i",TYPE_COFFESHOP,guid);
						mysql_query(buffer);
					}
					case 19:	//kartel
					{
						new buffer[256];
						format(buffer,sizeof(buffer),"UPDATE groups_list SET group_type=%i WHERE group_uid=%i",TYPE_KARTEL,guid);
						mysql_query(buffer);
					}
					case 20:	//syndykat
					{
						new buffer[256];
						format(buffer,sizeof(buffer),"UPDATE groups_list SET group_type=%i WHERE group_uid=%i",TYPE_SYNDYKAT,guid);
						mysql_query(buffer);
					}
				}
				vlive_err(playerid,"zmieni³eœ typ grupy");
				CleanGroups();
				LoadGroupsOnStart();	//przeladowanie grup
			}
		}
		case DIAL_KOKPIT:
		{
			if(!response) { }
			else
			{
				switch(listitem)
				{
					case 0:			//restart grup
					{
						CleanGroups();
						LoadGroupsOnStart();
						kokpit_msg(playerid,"prze³adowano grupy");
					}
					case 1:			//restart pojazdów
					{
						//test1(playerid);
						//LoadGroupsVehicles();
						//kokpit_msg(playerid,"usuniêto wszystkie pojazdy (1/2)");
						//kokpit_msg(playerid,"prze³adowano pojazdy grup (2/2)");
						kokpit_msg(playerid,"ta funkcja jest najbardziej skurwysyñskim miejscem w skrypcie, buguje siê jak nogi kurwy");
					}
					case 2:			//restart drzwi
					{
						ReloadDoors();
						kokpit_msg(playerid,"prze³adowano wszystkie drzwi");
					}
					case 3:			//restart serwera
					{
						SendClientMessageToAll(COLOR_GOLD,"[GAMEMODE] Serwer zostanie zrestartowany za 15 sek...");
						SetTimer("GMX", 15000, false);
					}
					case 4:			//wylacz skrypt
					{
						SendClientMessageToAll(COLOR_GOLD,"[GAMEMODE] Skrypt zostanie wy³¹czony za 10 sekund.");
						SetTimer("TurnOff", 10000, false);
					}
					case 5:		//czyszczenie paczek
					{
					
					}
					case 6:		//reload obiektów
					{
						DeleteAllObjects();
						LoadObjectsOnInit();
					}
					case 7:
					{
						ReloadBusStops();
						GuiInfo(playerid,"Prze³adowa³eœ wszystkie przystanki");
					}
					case 8:			//3d texty
					{

						GuiInfo(playerid,"Prze³adowa³eœ wszystkie 3D Texty");
					}
					case 9:		//debug mode
					{
						if(debugMode == 1)
						{
							debugMode = 0;
							SendClientMessageToAll(COLOR_GREEN, "[Debug] Tryb debugowania zosta³ wy³¹czony");
						}	
						else
						{
							debugMode = 1;
							SendClientMessageToAll(COLOR_RED, "[Debug] Tryb debugowania zosta³ w³¹czony");
							SendClientMessageToAll(COLOR_RED, "[Debug] Od tej pory gracze bez uprawnieñ nie bêd¹ mogli po³¹czyæ siê z serwerem");
						}
					}
				}
			}
		}
		case DIAL_MAGAZYN:
		{
			if(!response) { }
			else
			{
				new buffer[256];
				new loop = 0;
				format(buffer,sizeof(buffer),"SELECT uid,doorid FROM purchases");
				mysql_query(buffer);
				mysql_store_result();
				while(mysql_fetch_row(buffer,"|"))
				{
					if(loop == listitem)
					{
						new uid,dooruid;
						sscanf(buffer,"p<|>ii",uid,dooruid);
						for(new i = 0; i< MAX_PLAYERS; i++)
						{
							if(CarryPackage[i] == uid)
							{
								vlive_err(playerid,"ktoœ ju¿ wiezie tê paczkê");
								return 1;
							}
						}
						DisablePlayerCheckpoint(playerid);
						
						new doorid = 0;
						for(new i = 0 ; i < MAX_DOORS;i++)
						{
							if(DoorInfo[i][doorUID] == dooruid)
							{
								doorid = i;
								break;
							}
						}
						
						if(DoorInfo[doorid][doorUID] > 0)
						{
							SetPlayerCheckpoint(playerid, DoorInfo[doorid][doorEnterX], DoorInfo[doorid][doorEnterY], DoorInfo[doorid][doorEnterZ], 6.0);
							CarryPackage[playerid] = uid;
							pInfo[playerid][pCarryPackage] = 1;
							vlive_err(playerid,"kiedy dojdziesz we wskazane miejsce u¿yj komendy /dostarcz");
						}
						else
						{
							vlive_err(playerid,"takie drzwi nie istniej¹");
						}
						break;
					}
					loop++;
				}
				mysql_free_result();
			}
		}
		case DIAL_ZAMSEL:
		{
			if(!response) { }
			else
			{
				if(pInfo[playerid][pMoney] >= PurchasedItem[playerid][pItemPrice] )
				{
					//stac go
					if(GetPlayerVirtualWorld(playerid) != 0)
					{
						new doorid = GetPlayerDoorID(playerid);
						new dooruid = DoorInfo[doorid][doorUID];
						TakePlayerMoney(playerid,PurchasedItem[playerid][pItemPrice]);
						new buffer[256];
						format(buffer,sizeof(buffer),"INSERT INTO purchases (val1,val2,name,doorid,type,howmuch) VALUES (%i,%i,'%s',%i,%i,%i)",PurchasedItem[playerid][pItemVal1],PurchasedItem[playerid][pItemVal2],PurchasedItem[playerid][pItemName],dooruid,PurchasedItem[playerid][pType],PurchasedItem[playerid][pHowmuch]);
						mysql_query(buffer);
						format(buffer,256,"Zamówi³eœ %s za który zap³aci³eœ %i$, przesy³kê mo¿esz odebraæ w magazynie.",PurchasedItem[playerid][pItemName],PurchasedItem[playerid][pItemPrice]);
						
						GuiInfo(playerid,buffer);
					}
					else
					{
						vlive_err(playerid,"b³¹d przy zamawianiu.");
					}
					
				}
				else
				{
					GuiInfo(playerid,"Nie masz przy sobie tyle gotówki.");
					return 1;
				}
			}
		}
		case DIAL_ILE:
		{
			if(!response) { }
			else
			{
				new title[128];
				format(title,sizeof(title),"Zamawianie » %s",PurchasedItem[playerid][pItemName]);
				new text[128];
				new ile = strval(inputtext);
				if(ile <= 0 || ile > 25)
				{
					GuiInfo(playerid,"Wprowadzono z³¹ wartoœæ");
					return 1;
				}
				new totalcost = PurchasedItem[playerid][pItemPrice]*ile;
				if(pInfo[playerid][pMoney] >= totalcost)
				{
					PurchasedItem[playerid][pItemPrice] = totalcost;
					//PurchasedItem[playerid][pItemVal2] = ile;
					PurchasedItem[playerid][pHowmuch] = ile;
					//pInfo[playerid][pMoney] = pInfo[playerid][pMoney] - totalcost;
					format(text,sizeof(text),"Czy na pewno chcesz kupiæ %ix%s za %i$?",ile,PurchasedItem[playerid][pItemName],PurchasedItem[playerid][pItemPrice]);
					ShowPlayerDialog(playerid,DIAL_ZAMSEL,DIALOG_STYLE_MSGBOX,title,text,"Tak","Nie");
					return 1;
				}
				else
				{
					GuiInfo(playerid,"Nie masz przy sobie tyle gotówki.");
					return 1;
				}
			}
		}
		case DIAL_ZAMAWIANIE:
		{
			if(!response) { }
			else
			{
				switch(WhatTypeOffer[playerid])
				{
					case TYPE_WORKSHOP:			//zamawianie dla warsztatow
					{
						new title[128];
						format(title,sizeof(title),"Zamawianie » %s",WorkshopItems[listitem-1][wItemName]);
						//format(text,sizeof(text),"Czy na pewno chcesz kupiæ %s za %i$?",WorkshopItems[listitem-1][wItemName],WorkshopItems[listitem-1][wItemCost]);
						//ShowPlayerDialog(playerid,DIAL_ILE,DIALOG_STYLE_MSGBOX,title,text,"Tak","Nie");
						ShowPlayerDialog(playerid,DIAL_ILE,DIALOG_STYLE_INPUT,title,"Ile sztuk tego chcesz kupiæ?","Gotowe","Anuluj");
						
						PurchasedItem[playerid][pItemVal1] = WorkshopItems[listitem-1][wItemValue1];
						PurchasedItem[playerid][pItemVal2] = WorkshopItems[listitem-1][wItemValue2];
						PurchasedItem[playerid][pItemPrice] = WorkshopItems[listitem-1][wItemCost];
						PurchasedItem[playerid][pType] = WorkshopItems[listitem-1][wType];
						
						format(PurchasedItem[playerid][pItemName],32,"%s",WorkshopItems[listitem-1][wItemName]);
					}
					case TYPE_RESTAURANT:
					{
						new title[128];
						format(title,sizeof(title),"Zamawianie » %s",RestaurantItems[listitem-1][rItemName]);
						//format(text,sizeof(text),"Czy na pewno chcesz kupiæ %s za %i$?",WorkshopItems[listitem-1][wItemName],WorkshopItems[listitem-1][wItemCost]);
						//ShowPlayerDialog(playerid,DIAL_ILE,DIALOG_STYLE_MSGBOX,title,text,"Tak","Nie");
						ShowPlayerDialog(playerid,DIAL_ILE,DIALOG_STYLE_INPUT,title,"Ile sztuk tego chcesz kupiæ?","Gotowe","Anuluj");
						
						PurchasedItem[playerid][pItemVal1] = RestaurantItems[listitem-1][rItemValue1];
						PurchasedItem[playerid][pItemPrice] = RestaurantItems[listitem-1][rItemCost];
						PurchasedItem[playerid][pType] = RestaurantItems[listitem-1][rType];
						
						format(PurchasedItem[playerid][pItemName],32,"%s",RestaurantItems[listitem-1][rItemName]);
					}
					case TYPE_LSPD:
					{
						new title[128];
						format(title,sizeof(title),"Zamawianie » %s",LspdItems[listitem-1][lItemName]);
						//format(text,sizeof(text),"Czy na pewno chcesz kupiæ %s za %i$?",WorkshopItems[listitem-1][wItemName],WorkshopItems[listitem-1][wItemCost]);
						//ShowPlayerDialog(playerid,DIAL_ILE,DIALOG_STYLE_MSGBOX,title,text,"Tak","Nie");
						ShowPlayerDialog(playerid,DIAL_ILE,DIALOG_STYLE_INPUT,title,"Ile sztuk tego chcesz kupiæ?","Gotowe","Anuluj");
						
						PurchasedItem[playerid][pItemVal1] = LspdItems[listitem-1][lItemValue1];
						PurchasedItem[playerid][pItemVal2] = LspdItems[listitem-1][lItemValue2];
						PurchasedItem[playerid][pItemPrice] = LspdItems[listitem-1][lItemCost];
						PurchasedItem[playerid][pType] = LspdItems[listitem-1][lType];
						
						format(PurchasedItem[playerid][pItemName],32,"%s",LspdItems[listitem-1][lItemName]);
					}
					case TYPE_LSFD:
					{
						new title[128];
						format(title,sizeof(title),"Zamawianie » %s",FDItems[listitem-1][fItemName]);
						//format(text,sizeof(text),"Czy na pewno chcesz kupiæ %s za %i$?",WorkshopItems[listitem-1][wItemName],WorkshopItems[listitem-1][wItemCost]);
						//ShowPlayerDialog(playerid,DIAL_ILE,DIALOG_STYLE_MSGBOX,title,text,"Tak","Nie");
						ShowPlayerDialog(playerid,DIAL_ILE,DIALOG_STYLE_INPUT,title,"Ile sztuk tego chcesz kupiæ?","Gotowe","Anuluj");
						
						PurchasedItem[playerid][pItemVal1] = FDItems[listitem-1][fItemValue1];
						PurchasedItem[playerid][pItemVal2] = FDItems[listitem-1][fItemValue2];
						PurchasedItem[playerid][pItemPrice] = FDItems[listitem-1][fItemCost];
						PurchasedItem[playerid][pType] = FDItems[listitem-1][fType];
						
						format(PurchasedItem[playerid][pItemName],32,"%s",FDItems[listitem-1][fItemName]);
					}
					case TYPE_FBI:
					{
						new title[128];
						format(title,sizeof(title),"Zamawianie » %s",FBIitems[listitem-1][bItemName]);
						//format(text,sizeof(text),"Czy na pewno chcesz kupiæ %s za %i$?",WorkshopItems[listitem-1][wItemName],WorkshopItems[listitem-1][wItemCost]);
						//ShowPlayerDialog(playerid,DIAL_ILE,DIALOG_STYLE_MSGBOX,title,text,"Tak","Nie");
						ShowPlayerDialog(playerid,DIAL_ILE,DIALOG_STYLE_INPUT,title,"Ile sztuk tego chcesz kupiæ?","Gotowe","Anuluj");
						
						PurchasedItem[playerid][pItemVal1] = FBIitems[listitem-1][bItemValue1];
						PurchasedItem[playerid][pItemVal2] = FBIitems[listitem-1][bItemValue2];
						PurchasedItem[playerid][pItemPrice] = FBIitems[listitem-1][bItemCost];
						PurchasedItem[playerid][pType] = FBIitems[listitem-1][bType];
						
						format(PurchasedItem[playerid][pItemName],32,"%s",FBIitems[listitem-1][bItemName]);
					}
					case TYPE_MAFIA:
					{
						new title[128];
						format(title,sizeof(title),"Zamawianie » %s",MafiaItems[listitem-1][mItemName]);
						//format(text,sizeof(text),"Czy na pewno chcesz kupiæ %s za %i$?",WorkshopItems[listitem-1][wItemName],WorkshopItems[listitem-1][wItemCost]);
						//ShowPlayerDialog(playerid,DIAL_ILE,DIALOG_STYLE_MSGBOX,title,text,"Tak","Nie");
						ShowPlayerDialog(playerid,DIAL_ILE,DIALOG_STYLE_INPUT,title,"Ile sztuk tego chcesz kupiæ?","Gotowe","Anuluj");
						
						PurchasedItem[playerid][pItemVal1] = MafiaItems[listitem-1][mItemValue1];
						PurchasedItem[playerid][pItemVal2] = MafiaItems[listitem-1][mItemValue2];
						PurchasedItem[playerid][pItemPrice] = MafiaItems[listitem-1][mItemCost];
						PurchasedItem[playerid][pType] = MafiaItems[listitem-1][mType];
						
						format(PurchasedItem[playerid][pItemName],32,"%s",MafiaItems[listitem-1][mItemName]);
					}
					case TYPE_ORG:
					{
						new title[128];
						format(title,sizeof(title),"Zamawianie » %s",OrganizacjeItems[listitem-1][orgItemName]);
						//format(text,sizeof(text),"Czy na pewno chcesz kupiæ %s za %i$?",WorkshopItems[listitem-1][wItemName],WorkshopItems[listitem-1][wItemCost]);
						//ShowPlayerDialog(playerid,DIAL_ILE,DIALOG_STYLE_MSGBOX,title,text,"Tak","Nie");
						ShowPlayerDialog(playerid,DIAL_ILE,DIALOG_STYLE_INPUT,title,"Ile sztuk tego chcesz kupiæ?","Gotowe","Anuluj");
						
						PurchasedItem[playerid][pItemVal1] = OrganizacjeItems[listitem-1][orgItemValue1];
						PurchasedItem[playerid][pItemVal2] = OrganizacjeItems[listitem-1][orgItemValue2];
						PurchasedItem[playerid][pItemPrice] = OrganizacjeItems[listitem-1][orgItemCost];
						PurchasedItem[playerid][pType] = OrganizacjeItems[listitem-1][orgType];
						
						format(PurchasedItem[playerid][pItemName],32,"%s",OrganizacjeItems[listitem-1][orgItemName]);
					}
					case TYPE_OCHRONIARZE:
					{
						new title[128];
						format(title,sizeof(title),"Zamawianie » %s",OchroniarzeItems[listitem-1][oItemName]);
						//format(text,sizeof(text),"Czy na pewno chcesz kupiæ %s za %i$?",WorkshopItems[listitem-1][wItemName],WorkshopItems[listitem-1][wItemCost]);
						//ShowPlayerDialog(playerid,DIAL_ILE,DIALOG_STYLE_MSGBOX,title,text,"Tak","Nie");
						ShowPlayerDialog(playerid,DIAL_ILE,DIALOG_STYLE_INPUT,title,"Ile sztuk tego chcesz kupiæ?","Gotowe","Anuluj");
						
						PurchasedItem[playerid][pItemVal1] = OchroniarzeItems[listitem-1][oItemValue1];
						PurchasedItem[playerid][pItemVal2] = OchroniarzeItems[listitem-1][oItemValue2];
						PurchasedItem[playerid][pItemPrice] = OchroniarzeItems[listitem-1][oItemCost];
						PurchasedItem[playerid][pType] = OchroniarzeItems[listitem-1][oType];
						
						format(PurchasedItem[playerid][pItemName],32,"%s",OchroniarzeItems[listitem-1][oItemName]);
					}
					case TYPE_SCIGANT:
					{
						new title[128];
						format(title,sizeof(title),"Zamawianie » %s",SciganciItems[listitem-1][sItemName]);
						//format(text,sizeof(text),"Czy na pewno chcesz kupiæ %s za %i$?",WorkshopItems[listitem-1][wItemName],WorkshopItems[listitem-1][wItemCost]);
						//ShowPlayerDialog(playerid,DIAL_ILE,DIALOG_STYLE_MSGBOX,title,text,"Tak","Nie");
						ShowPlayerDialog(playerid,DIAL_ILE,DIALOG_STYLE_INPUT,title,"Ile sztuk tego chcesz kupiæ?","Gotowe","Anuluj");
						
						PurchasedItem[playerid][pItemVal1] = SciganciItems[listitem-1][sItemValue1];
						PurchasedItem[playerid][pItemVal2] = SciganciItems[listitem-1][sItemValue2];
						PurchasedItem[playerid][pItemPrice] = SciganciItems[listitem-1][sItemCost];
						PurchasedItem[playerid][pType] = SciganciItems[listitem-1][sType];
						
						format(PurchasedItem[playerid][pItemName],32,"%s",SciganciItems[listitem-1][sItemName]);
					}
					case TYPE_COFFESHOP:
					{
						new title[128];
						format(title,sizeof(title),"Zamawianie » %s",CoffeeItems[listitem-1][cItemName]);
						//format(text,sizeof(text),"Czy na pewno chcesz kupiæ %s za %i$?",WorkshopItems[listitem-1][wItemName],WorkshopItems[listitem-1][wItemCost]);
						//ShowPlayerDialog(playerid,DIAL_ILE,DIALOG_STYLE_MSGBOX,title,text,"Tak","Nie");
						ShowPlayerDialog(playerid,DIAL_ILE,DIALOG_STYLE_INPUT,title,"Ile sztuk tego chcesz kupiæ?","Gotowe","Anuluj");
						
						PurchasedItem[playerid][pItemVal1] = CoffeeItems[listitem-1][cItemValue1];
						PurchasedItem[playerid][pItemVal2] = CoffeeItems[listitem-1][cItemValue2];
						PurchasedItem[playerid][pItemPrice] = CoffeeItems[listitem-1][cItemCost];
						PurchasedItem[playerid][pType] = CoffeeItems[listitem-1][cType];
						
						format(PurchasedItem[playerid][pItemName],32,"%s",CoffeeItems[listitem-1][cItemName]);
					}
				}
			}
		}
		case DIAL_911SEL:
		{
			if(!response)
			{
				DeletePhoneAnim(playerid);
			}
			else
			{
				//tu rozsylamy
				if(Selected911[playerid] == 0)				//pd
				{
					new centrInfo[128],deptMsg[256];
					format(centrInfo,sizeof(centrInfo),"Centrala: Do wszystkich jednostek, otrzymaliœmy zg³oszenie. ((%i))",pInfo[playerid][pPhoneInUse]);
					format(deptMsg,sizeof(deptMsg),"Treœæ: %s",inputtext);
					
					for(new i = 0 ; i < MAX_PLAYERS; i++)
					{
						if(DutyGroupType[i] == TYPE_LSPD)
						{
							SendCustomPlayerMessage(i,COLOR_GREEN,centrInfo);
							SendCustomPlayerMessage(i,COLOR_GREEN,deptMsg);
						}
					}
					DeletePhoneAnim(playerid);
					SendCustomPlayerMessage(playerid,COLOR_YELLOW,"Dziêkujemy za zg³oszenie!");
				}
				if(Selected911[playerid] == 1)				//fd
				{
					new centrInfo[128],deptMsg[256];
					format(centrInfo,sizeof(centrInfo),"Centrala: Do wszystkich jednostek, otrzymaliœmy zg³oszenie. ((%i))",pInfo[playerid][pPhoneInUse]);
					format(deptMsg,sizeof(deptMsg),"Treœæ: %s",inputtext);
					
					for(new i = 0 ; i < MAX_PLAYERS; i++)
					{
						if(DutyGroupType[i] == TYPE_LSFD || DutyGroupType[i] == TYPE_LSMC )
						{
							SendCustomPlayerMessage(i,COLOR_GREEN,centrInfo);
							SendCustomPlayerMessage(i,COLOR_GREEN,deptMsg);
						}
					}
					DeletePhoneAnim(playerid);
					SendCustomPlayerMessage(playerid,COLOR_YELLOW,"Dziêkujemy za zg³oszenie!");
				}
				if(Selected911[playerid] == 2)				//medical
				{
					new centrInfo[128],deptMsg[256];
					format(centrInfo,sizeof(centrInfo),"Centrala: Do wszystkich jednostek, otrzymaliœmy zg³oszenie. ((%i))",pInfo[playerid][pPhoneInUse]);
					format(deptMsg,sizeof(deptMsg),"Treœæ: %s",inputtext);
					
					for(new i = 0 ; i < MAX_PLAYERS; i++)
					{
						if(DutyGroupType[i] == TYPE_LSMC || DutyGroupType[i] == TYPE_LSFD)
						{
							SendCustomPlayerMessage(i,COLOR_GREEN,centrInfo);
							SendCustomPlayerMessage(i,COLOR_GREEN,deptMsg);
						}
					}
					DeletePhoneAnim(playerid);
					SendCustomPlayerMessage(playerid,COLOR_YELLOW,"Dziêkujemy za zg³oszenie!");
					
					new phoneMsg[256];
					format(phoneMsg,sizeof(phoneMsg),": %s",inputtext);
					SendPlayerPhoneToNearPlayers(playerid,phoneMsg);
				}
				if(Selected911[playerid] == 3)
				{
					new centrInfo[128],deptMsg[256];
					format(centrInfo,sizeof(centrInfo),"Centrala: Do wszystkich jednostek, otrzymaliœmy wezwanie. ((%i))",pInfo[playerid][pPhoneInUse]);
					format(deptMsg,sizeof(deptMsg),"Treœæ: %s",inputtext);
					
					for(new i = 0 ; i < MAX_PLAYERS; i++)
					{
						if(DutyGroupType[i] == TYPE_TAXI)
						{
							SendCustomPlayerMessage(i,COLOR_GREEN,centrInfo);
							SendCustomPlayerMessage(i,COLOR_GREEN,deptMsg);
						}
					}
					DeletePhoneAnim(playerid);
					SendCustomPlayerMessage(playerid,COLOR_YELLOW,"Dziêkujemy za zg³oszenie!");
					
					new phoneMsg[256];
					format(phoneMsg,sizeof(phoneMsg),": %s",inputtext);
					SendPlayerPhoneToNearPlayers(playerid,phoneMsg);
				}				
			}
		}
		case DIAL_911:
		{
			if(!response)
			{
				DeletePhoneAnim(playerid);
			}
			else
			{
				Selected911[playerid] = listitem;
				ShowPlayerDialog(playerid,DIAL_911SEL,DIALOG_STYLE_INPUT,"Numer alarmowy » zg³oszenie","Centrala departamentowa, s³ucham?","Gotowe","Zamknij");
			}
		}
		case DIAL_SCHOWEK:
		{
			if(!response) { }
			else
			{
				new what = 0;
				new num = 0;
				new buffer[256],lista[1024];
				new doorid = GetPlayerDoorID(playerid);
				new dooruid = 0;
				for(new i = 0 ; i < MAX_DOORS; i++)
				{
					if(DoorInfo[i][doorUID] == DoorInfo[doorid][doorUID])
					{
						dooruid = i;
					}
				}
				format(buffer,sizeof(buffer),"SELECT item_uid, item_name FROM core_items WHERE item_whereis=%i AND item_owneruid=%i",WHEREIS_SCHOWEK,DoorInfo[dooruid][doorUID]);
				mysql_query(buffer);
				mysql_store_result();
				while(mysql_fetch_row(buffer,"|"))
				{
					new uid,name[32];
					sscanf(buffer,"p<|>is[32]",uid,name);
					format(lista,sizeof(lista),"%s\n%i\t\t%s",lista,uid,name);
					if(listitem == num)
					{
						what = uid;
						break;
					}
					num++;
				}
				mysql_free_result();
				// num == listitem 
				//what == uid itema
				format(buffer,256,"SELECT item_worldid FROM core_items WHERE item_uid=%i",what);
				mysql_query(buffer);
				mysql_store_result();
				new val2 = mysql_fetch_int();
				mysql_free_result();
				if(val2 > 1)
				{
					format(buffer,sizeof(buffer),"UPDATE core_items SET item_worldid=item_worldid-1 WHERE item_uid=%i",what);
					mysql_query(buffer);
					format(buffer,256,"SELECT item_value1,item_value2,item_type,item_name FROM core_items WHERE item_uid=%i",what);
					mysql_query(buffer);
					mysql_store_result();
					while(mysql_fetch_row(buffer,"|"))
					{
						new val1,value2,type,name[32];
						sscanf(buffer,"p<|>iiis[32]",val1,value2,type,name);
						mysql_free_result();
						format(buffer,256,"INSERT INTO core_items ( item_value1,item_value2,item_type,item_name,item_owneruid,item_whereis) VALUES (%i,%i,%i,'%s',%i,%i)",val1,value2,type,name,GetPlayerUID(playerid),WHEREIS_GRACZ);
						mysql_query(buffer);
					}
				}
				else
				{
					//1 lub mniej
					if(CountPlayerItems(playerid) >= 30)
					{
						vlive_err(playerid,"masz za du¿o przedmiotów w swoim ekwipunku");
						return 1;
					}
					format(buffer,sizeof(buffer),"UPDATE core_items SET item_owneruid=%i, item_whereis=%i WHERE item_uid=%i",GetPlayerUID(playerid),WHEREIS_GRACZ,what);
					mysql_query(buffer);
				}
				
				GuiInfo(playerid,"Wyci¹gn¹³eœ przedmiot ze schowka.");
			}
		}
		case DIAL_BKWYPLAC:
		{
			if(!response) { } 
			else
			{
				new amount = strval(inputtext);
				if(amount <=0 || strlen(inputtext) > 11)
				{
					GuiInfo(playerid,"Wprowadzono nieprawid³ow¹ kwotê.");
					return 1;
				}
				if(amount > pInfo[playerid][pBank])
				{
					GuiInfo(playerid,"Nie posiadasz w banku takiej iloœci gotówki.");
					return 1;
				}
				else
				{
					new buffer[256];
					pInfo[playerid][pMoney] = pInfo[playerid][pMoney] + amount;
					pInfo[playerid][pBank] = pInfo[playerid][pBank] - amount;
					ResetPlayerMoney(playerid);
					GivePlayerMoney(playerid,pInfo[playerid][pMoney]);
					format(buffer,sizeof(buffer),"UPDATE core_players SET char_money=char_money+%i , char_bank = char_bank - %i WHERE char_uid=%i",amount,amount,GetPlayerUID(playerid));
					mysql_query(buffer);
					new ou[128];
					format(ou,sizeof(ou),"Wyp³aci³eœ z banku %i$, nowy stan konta to : %i$",amount,pInfo[playerid][pBank]);
					GuiInfo(playerid,ou);
					pInfo[playerid][pBankomat] = 10*60;
					return 1;
				}
			}
		}
		case DIAL_BKWPLAC:
		{
			if(!response) { }
			else
			{
				new amount = strval(inputtext);
				if(amount <=0 || strlen(inputtext) > 11)
				{	
					GuiInfo(playerid,"Wprowadzono nieprawid³ow¹ kwotê.");
					return 1;
				}
				if(amount > pInfo[playerid][pMoney])
				{
					GuiInfo(playerid,"Nie posiadasz przy sobie takiej iloœci gotówki.");
					return 1;
				}
				else
				{
					new buffer[256];
					format(buffer,sizeof(buffer),"UPDATE core_players SET char_money=char_money-%i ,char_bank=char_bank+%i WHERE char_uid=%i",amount,amount,GetPlayerUID(playerid));
					mysql_query(buffer);
					pInfo[playerid][pMoney] = pInfo[playerid][pMoney] - amount;
					ResetPlayerMoney(playerid);
					GivePlayerMoney(playerid,pInfo[playerid][pMoney]);
					pInfo[playerid][pBank] = pInfo[playerid][pBank]+amount;
					new ou[128];
					format(ou,sizeof(ou),"Wp³aci³eœ do banku %i$, nowy stan konta to : %i$",amount,pInfo[playerid][pBank]);
					GuiInfo(playerid,ou);
					return 1;
				}
			}
		}
		case DIAL_BANKOMAT:
		{
			if(!response) { }
			else
			{
				switch(listitem)
				{
					case 0:				//stan konta
					{
						new stan[256];
						format(stan,sizeof(stan),"Stan konta: ~g~%i$",pInfo[playerid][pBank]);
						GameTextForPlayer(playerid, stan, 7000, 5);
					}
					case 1:				//wyplac
					{
						ShowPlayerDialog(playerid,DIAL_BKWYPLAC,DIALOG_STYLE_INPUT,"Bankomat » wyp³aæ pieni¹dze","Wpisz poni¿ej kwotê jak¹ chcesz wyp³aciæ z bankomatu...","Gotowe","Anuluj");
					}
					case 2:				//wplac
					{
						ShowPlayerDialog(playerid,DIAL_BKWPLAC,DIALOG_STYLE_INPUT,"Bankomat » wp³aæ pieni¹dze","Wpisz poni¿ej kwotê jak¹ chcesz wp³aciæ na Twoje konto bankowe.","Gotowe","Anuluj");
					}
				}
			}
		}
		case DIAL_O:
		{
			switch(listitem)
			{
				case 6:
				{
					GuiInfo(playerid,"¯artowa³em! No bez jaj, jak ju¿ musisz to po prostu to odegraj!");
				}
			}
		}
		case DIAL_PICKUP:
		{
			if(!response) { }
			else
			{
				new doorid = SelectedAdminDoors[playerid];
				new buffer[256];
				switch(listitem)
				{

					case 0:			//dolar
					{
						format(buffer,sizeof(buffer),"UPDATE doors SET model=1274 WHERE uid=%i",DoorInfo[doorid][doorUID]);
						mysql_query(buffer);
						
					}
					case 1:			//informacja
					{
						format(buffer,sizeof(buffer),"UPDATE doors SET model=1239 WHERE uid=%i",DoorInfo[doorid][doorUID]);
						mysql_query(buffer);
					}
					case 2:			//serce
					{
						format(buffer,sizeof(buffer),"UPDATE doors SET model=1240 WHERE uid=%i",DoorInfo[doorid][doorUID]);
						mysql_query(buffer);
					}
					case 3:			//niebieski domek
					{
						format(buffer,sizeof(buffer),"UPDATE doors SET model=1272 WHERE uid=%i",DoorInfo[doorid][doorUID]);
						mysql_query(buffer);
					}
					case 4:			//zielony domek
					{
						format(buffer,sizeof(buffer),"UPDATE doors SET model=1273 WHERE uid=%i",DoorInfo[doorid][doorUID]);
						mysql_query(buffer);
					}
					case 5:			//bia³a strza³ka
					{
						format(buffer,sizeof(buffer),"UPDATE doors SET model=1318 WHERE uid=%i",DoorInfo[doorid][doorUID]);
						mysql_query(buffer);
					}
					case 6:			//gwiazdka
					{
						format(buffer,sizeof(buffer),"UPDATE doors SET model=1247 WHERE uid=%i",DoorInfo[doorid][doorUID]);
						mysql_query(buffer);
					}
				}
				new load = DoorInfo[doorid][doorUID];
				UnloadDoor(DoorInfo[doorid][doorSampID]);
				LoadDoor(load);
			}
		}
		case DIAL_NAME:
		{
			if(!response) { }
			{
				new buffer[256];
				format(buffer,sizeof(buffer),"INSERT INTO core_items (item_type,item_value1,item_value2,item_name,item_owneruid) VALUES(%i,%i,%i,'%s',%i)",ItemType[playerid],ItemValue1[playerid],ItemValue2[playerid],inputtext,ItemOwner[playerid]);
				mysql_query(buffer);
				GuiInfo(playerid,"Stworzy³eœ przedmiot, pojawi³ siê w ekwipunku wskazanego gracza.");
				
				new adminLog[128];
				format(adminLog,sizeof(adminLog),"~r~[item]~y~ Administrator %s ~g~(UID: %i | SID: %i)~y~ stworzyl przedmiot %s (%i|%i)",pInfo[playerid][pName],GetPlayerUID(playerid),playerid,inputtext,ItemValue1[playerid],ItemValue2[playerid]);
				LogAdminAction(adminLog);
			}
		}
		case DIAL_OWNER:				//do poprawy
		{
			if(!response) { }
			else
			{
				ItemOwner[playerid] = strval(inputtext);
				//GuiInfo(playerid,"Stworzy³eœ przedmiot, pojawi³ siê w ekwipunku wskazanego gracza.");
				ShowPlayerDialog(playerid,DIAL_NAME,DIALOG_STYLE_INPUT,"Przedmiot » tworzenie (Krok #4)","Wpisz nazwê tworzonego w³aœnie przedmiotu.","Gotowe","Anuluj");
			}
		}
		case DIAL_VAL2:
		{
			if(!response) { }
			else
			{
				ItemValue2[playerid] = strval(inputtext);
				ShowPlayerDialog(playerid,DIAL_OWNER,DIALOG_STYLE_INPUT,"Przedmiot » tworzenie (Krok #3)","Wpisz UID osoby, która ma otwrzymaæ ten przedmiot.","Gotowe","Anuluj");
			}
		}
		case DIAL_VAL1:
		{
			if(!response) { }
			else
			{
				ItemValue1[playerid] = strval(inputtext);
				ShowPlayerDialog(playerid,DIAL_VAL2,DIALOG_STYLE_INPUT,"Przedmiot » tworzenie (Krok #2)","Wpisz poni¿ej wartoœæ drug¹ danego przedmiotu, dla broni jest to iloœæ amunicji.","Gotowe","Anuluj");
			}
		}
		case DIAL_APRZEDMIOT:
		{
			if(!response) { }
			else
			{
				switch(listitem)
				{					
					case 1:
					{
						ShowPlayerDialog(playerid,DIAL_VAL1,DIALOG_STYLE_INPUT,"Przedmiot » tworzenie (Krok #1)","Wpisz poni¿ej wartoœæ pierwsz¹ tworzonego przedmiotu, dla ubrania jest to skinid, dla broni gunid.","Gotowe","Anuluj");
						ItemType[playerid] = ITEM_WEAPON;
					}
					case 2:
					{
						ShowPlayerDialog(playerid,DIAL_VAL1,DIALOG_STYLE_INPUT,"Przedmiot » tworzenie (Krok #1)","Wpisz poni¿ej wartoœæ pierwsz¹ tworzonego przedmiotu, dla ubrania jest to skinid, dla broni gunid.","Gotowe","Anuluj");
						ItemType[playerid] = ITEM_AMMO;
					}
					case 3:
					{
						ShowPlayerDialog(playerid,DIAL_VAL1,DIALOG_STYLE_INPUT,"Przedmiot » tworzenie (Krok #1)","Wpisz poni¿ej wartoœæ pierwsz¹ tworzonego przedmiotu, dla ubrania jest to skinid, dla broni gunid.","Gotowe","Anuluj");
						ItemType[playerid] = ITEM_CLOTH;
					}
					case 4:
					{
						ShowPlayerDialog(playerid,DIAL_VAL1,DIALOG_STYLE_INPUT,"Przedmiot » tworzenie (Krok #1)","Wpisz poni¿ej wartoœæ pierwsz¹ tworzonego przedmiotu, dla ubrania jest to skinid, dla broni gunid.","Gotowe","Anuluj");
						ItemType[playerid] = ITEM_FOOD;
					}
					case 5:
					{
						ShowPlayerDialog(playerid,DIAL_VAL1,DIALOG_STYLE_INPUT,"Przedmiot » tworzenie (Krok #1)","Wpisz poni¿ej wartoœæ pierwsz¹ tworzonego przedmiotu, dla ubrania jest to skinid, dla broni gunid.","Gotowe","Anuluj");
						ItemType[playerid] = ITEM_COMPONENT;
					}
					case 6:
					{
						ShowPlayerDialog(playerid,DIAL_VAL1,DIALOG_STYLE_INPUT,"Przedmiot » tworzenie (Krok #1)","Wpisz poni¿ej wartoœæ pierwsz¹ tworzonego przedmiotu, dla ubrania jest to skinid, dla broni gunid.","Gotowe","Anuluj");
						ItemType[playerid] = ITEM_CLOCK;
					}
					case 7:
					{
						ShowPlayerDialog(playerid,DIAL_VAL1,DIALOG_STYLE_INPUT,"Przedmiot » tworzenie (Krok #1)","Wpisz poni¿ej wartoœæ pierwsz¹ tworzonego przedmiotu, dla ubrania jest to skinid, dla broni gunid.","Gotowe","Anuluj");
						ItemType[playerid] = ITEM_NOTEPAD;
					}
					case 8:
					{
						ShowPlayerDialog(playerid,DIAL_VAL1,DIALOG_STYLE_INPUT,"Przedmiot » tworzenie (Krok #1)","Wpisz poni¿ej wartoœæ pierwsz¹ tworzonego przedmiotu, dla ubrania jest to skinid, dla broni gunid.","Gotowe","Anuluj");
						ItemType[playerid] = ITEM_PHONE;
					}
					case 9:
					{
						ShowPlayerDialog(playerid,DIAL_VAL1,DIALOG_STYLE_INPUT,"Przedmiot » tworzenie (Krok #1)","Wpisz poni¿ej wartoœæ pierwsz¹ tworzonego przedmiotu, dla ubrania jest to skinid, dla broni gunid.","Gotowe","Anuluj");
						ItemType[playerid] = ITEM_CASH;
					}
					case 10:
					{
						ShowPlayerDialog(playerid,DIAL_VAL1,DIALOG_STYLE_INPUT,"Przedmiot » tworzenie (Krok #1)","Wpisz poni¿ej wartoœæ pierwsz¹ tworzonego przedmiotu, dla ubrania jest to skinid, dla broni gunid.","Gotowe","Anuluj");
						ItemType[playerid] = ITEM_KAJDANKI;
					}
					case 11:
					{
						ShowPlayerDialog(playerid,DIAL_VAL1,DIALOG_STYLE_INPUT,"Przedmiot » tworzenie (Krok #1)","Wpisz poni¿ej wartoœæ pierwsz¹ tworzonego przedmiotu, dla ubrania jest to skinid, dla broni gunid.","Gotowe","Anuluj");
						ItemType[playerid] = ITEM_WINO;
					}
					case 12:
					{
						ShowPlayerDialog(playerid,DIAL_VAL1,DIALOG_STYLE_INPUT,"Przedmiot » tworzenie (Krok #1)","Wpisz poni¿ej wartoœæ pierwsz¹ tworzonego przedmiotu, dla ubrania jest to skinid, dla broni gunid.","Gotowe","Anuluj");
						ItemType[playerid] = ITEM_PAPIEROS;
					}
					case 13:
					{
						ShowPlayerDialog(playerid,DIAL_VAL1,DIALOG_STYLE_INPUT,"Przedmiot » tworzenie (Krok #1)","Wpisz poni¿ej wartoœæ pierwsz¹ tworzonego przedmiotu, dla ubrania jest to skinid, dla broni gunid.","Gotowe","Anuluj");
						ItemType[playerid] = ITEM_MASK;
					}
					case 14:
					{
						ShowPlayerDialog(playerid,DIAL_VAL1,DIALOG_STYLE_INPUT,"Przedmiot » tworzenie (Krok #1)","Wpisz poni¿ej wartoœæ pierwsz¹ tworzonego przedmiotu, dla ubrania jest to skinid, dla broni gunid.","Gotowe","Anuluj");
						ItemType[playerid] = ITEM_KAMIZELKA;
					}
					case 15:
					{
						ShowPlayerDialog(playerid,DIAL_VAL1,DIALOG_STYLE_INPUT,"Przedmiot » tworzenie (Krok #1)","Wpisz poni¿ej wartoœæ pierwsz¹ tworzonego przedmiotu, dla ubrania jest to skinid, dla broni gunid.","Gotowe","Anuluj");
						ItemType[playerid] = ITEM_MEGAFON;
					}
					case 16:
					{
						ShowPlayerDialog(playerid,DIAL_VAL1,DIALOG_STYLE_INPUT,"Przedmiot » tworzenie (Krok #1)","Wpisz poni¿ej wartoœæ pierwsz¹ tworzonego przedmiotu, dla ubrania jest to skinid, dla broni gunid.","Gotowe","Anuluj");
						ItemType[playerid] = ITEM_PARALIZATOR;
					}
					case 17:	//maryska
					{
						ShowPlayerDialog(playerid,DIAL_VAL1,DIALOG_STYLE_INPUT,"Przedmiot » tworzenie (Krok #1)","Wpisz poni¿ej wartoœæ pierwsz¹ tworzonego przedmiotu, dla ubrania jest to skinid, dla broni gunid.","Gotowe","Anuluj");
						ItemType[playerid] = ITEM_MARIHUANA;
					}
					case 18:	//amfa
					{
						ShowPlayerDialog(playerid,DIAL_VAL1,DIALOG_STYLE_INPUT,"Przedmiot » tworzenie (Krok #1)","Wpisz poni¿ej wartoœæ pierwsz¹ tworzonego przedmiotu, dla ubrania jest to skinid, dla broni gunid.","Gotowe","Anuluj");
						ItemType[playerid] = ITEM_AMFETAMINA;
					}
					case 19:	//lakier
					{
						ShowPlayerDialog(playerid,DIAL_VAL1,DIALOG_STYLE_INPUT,"Przedmiot » tworzenie (Krok #1)","Wpisz poni¿ej wartoœæ pierwsz¹ tworzonego przedmiotu, dla ubrania jest to skinid, dla broni gunid.","Gotowe","Anuluj");
						ItemType[playerid] = ITEM_LAKIER;
					}
					case 20:	//przyczepialny
					{
						ShowPlayerDialog(playerid,DIAL_VAL1,DIALOG_STYLE_INPUT,"Przedmiot » tworzenie (Krok #1)","Wpisz poni¿ej wartoœæ pierwsz¹ tworzonego przedmiotu, dla ubrania jest to skinid, dla broni gunid.","Gotowe","Anuluj");
						ItemType[playerid] = ITEM_ATTACH;
					}
					case 21:	//seeds
					{
						ShowPlayerDialog(playerid,DIAL_VAL1,DIALOG_STYLE_INPUT,"Przedmiot » tworzenie (Krok #1)","Wpisz poni¿ej wartoœæ pierwsz¹ tworzonego przedmiotu, dla ubrania jest to skinid, dla broni gunid.","Gotowe","Anuluj");
						ItemType[playerid] = ITEM_SEEDS;
					}
					case 22:	//sww
					{
						ShowPlayerDialog(playerid,DIAL_VAL1,DIALOG_STYLE_INPUT,"Przedmiot » tworzenie (Krok #1)","Wpisz poni¿ej wartoœæ pierwsz¹ tworzonego przedmiotu, dla ubrania jest to skinid, dla broni gunid.","Gotowe","Anuluj");
						ItemType[playerid] = ITEM_SPECIALWEAP;
					}
					case 23:	//karnet
					{
						ShowPlayerDialog(playerid,DIAL_VAL1,DIALOG_STYLE_INPUT,"Przedmiot » tworzenie (Krok #1)","Wpisz poni¿ej wartoœæ pierwsz¹ tworzonego przedmiotu, dla ubrania jest to skinid, dla broni gunid.","Gotowe","Anuluj");
						ItemType[playerid] = ITEM_KARNET;
					}
					case 24:
					{
						ShowPlayerDialog(playerid,DIAL_VAL1,DIALOG_STYLE_INPUT,"Przedmiot » tworzenie (Krok #1)","Wpisz poni¿ej wartoœæ pierwsz¹ tworzonego przedmiotu, dla ubrania jest to skinid, dla broni gunid.","Gotowe","Anuluj");
						ItemType[playerid] = ITEM_WYTRYCH_DRZWI;
					}
					case 25:
					{
						ShowPlayerDialog(playerid,DIAL_VAL1,DIALOG_STYLE_INPUT,"Przedmiot » tworzenie (Krok #1)","Wpisz poni¿ej wartoœæ pierwsz¹ tworzonego przedmiotu, dla ubrania jest to skinid, dla broni gunid.","Gotowe","Anuluj");
						ItemType[playerid] = ITEM_SWAT_CAMERA;
					}
				}
			}
		}
		case DIAL_CONTACTS:
		{
			if(!response) { }
			else
			{
				//wybrany kontakt
			}
		}
		case DIAL_PHONE:
		{
			if(GetPlayerPhoneInUse(playerid) == 2)
			{
				GuiInfo(playerid,"Wykryto b³¹d w systemie przedmiotów, musisz zg³osiæ siê do administratora. \n Error CODE : 7374");
				return 0;
			}
			else
			{
				switch(listitem)
				{
					case 0:						//kontakty
					{
						ShowPlayerContacts(playerid);
					}
					case 1:						//dodaj do kontaktow
					{
					
					}
					case 2:						//wy³¹cz
					{
						new buffer[256];
						format(buffer,sizeof(buffer),"SELECT item_value2 FROM core_items WHERE item_value1=%i AND item_type=%i",SelectedPhone[playerid],ITEM_PHONE);
						mysql_query(buffer);
						mysql_store_result();
						new status = mysql_fetch_int();
						mysql_free_result();
						if(status != 0)
						{							
							format(buffer,sizeof(buffer),"UPDATE core_items SET item_value2 = 0 WHERE item_value1=%i AND item_type=%i",pInfo[playerid][pPhoneInUse],ITEM_PHONE);
							mysql_query(buffer);
							pInfo[playerid][pPhoneInUse] = 0;
							SendClientMessage(playerid,COLOR_GREY,"Wy³¹czy³eœ swój telefon komórkowy.");
						}
						else
						{
							if(pInfo[playerid][pPhoneInUse] > 0)
							{
								vlive_err(playerid,"ju¿ masz jeden telefon w u¿yciu, wy³¹cz go by aktywowaæ inny");
								return 1;
							}
							format(buffer,sizeof(buffer),"UPDATE core_items SET item_value2 = 1 WHERE item_value1=%i AND item_type=%i",SelectedPhone[playerid],ITEM_PHONE);
							mysql_query(buffer);
							pInfo[playerid][pPhoneInUse] = SelectedPhone[playerid];
							SelectedPhone[playerid] = 0;
							SendClientMessage(playerid,COLOR_GREY,"W³¹czy³eœ swój telefon komórkowy.");
						}
					}
				}
			}
		}
		case DIAL_GUSE:
		{
			
		}
		case DIAL_G:
		{
			if(!response) { }
			else
			{
				new title[128],msg[512];
				format(title,sizeof(title),"System Grup » %s",pGrupa[playerid][listitem+1][pGname]);
				format(msg,sizeof(msg),""COL_WHITE"Nazwa grupy : "COL_GREEN"%s\n"COL_WHITE"Twoja ranga : "COL_GREEN"%s\n"COL_WHITE"Wyp³ata : "COL_GREEN"%i\n"COL_WHITE"Ubranie s³u¿bowe : "COL_GREEN"%i",pGrupa[playerid][listitem+1][pGname],pGrupa[playerid][listitem+1][pRank],pGrupa[playerid][listitem+1][pPayday],pGrupa[playerid][listitem+1][pSkin]);
				ShowPlayerDialog(playerid,DIAL_GUSE,DIALOG_STYLE_MSGBOX,title,msg,"Zamknij","");
			}
		}
		case DIAL_UNSPAWN:
		{
			if(!response) { }
			{
				DeleteVehicle(strval(inputtext));
			}
		}
		case DIAL_AVACTION:
		{
			if(!response) { }
			else
			{
				switch(listitem)
				{
					case 0:				//spawn
					{
						new vehuid = ovehicle[SelectedNoneVeh[playerid]][ovuid];
						SpawnVehicle(vehuid);
						
						for(new i = 0 ; i < MAX_VEHICLES; i++)
						{
							if(vehicle[i][vuid] == vehuid)
							{
								new Float:x,Float:y,Float:z;
								GetPlayerPos(playerid,x,y,z);
								SetVehiclePos(i,x,y,z);
								break;
							}
						}
					}
					case 1:				//unspawn
					{
						
					}
				}
			}
		}
		case DIAL_AVOLIST:
		{
			if(!response) { }
			else
			{
				ShowPlayerDialog(playerid,DIAL_AVACTION,DIALOG_STYLE_LIST,ovehicle[listitem+1][ovname],"1. Spawn\n2. Unspawn","wybierz","zamknij");
				SelectedNoneVeh[playerid] = listitem+1;
			}
		}
		case DIAL_ACP:
		{
			if(!response) { }
			else
			{
				switch(listitem)
				{
					case 0:				//pojazdow
					{
						ShowPlayerDialog(playerid,DIAL_AVMENU,DIALOG_STYLE_LIST,"Admin Control Panel » Vehicles","1. Lista pojazdów bez w³aœciciela \n2. Unspawn pojazdu o konkretnym SampID","Wybierz","Zamknij");
					}
					case 1:		
					{
						//respawn pojazdow grupowych
						for(new i = 0 ; i < MAX_VEHICLES; i++)
						{
							if(vehicle[i][vownertype] == 1)
							{
								new uid = vehicle[i][vuid];
								UnspawnVehicle(i);
								SpawnVehicle(uid);
							}
						}
						
						GuiInfo(playerid,"Odspawnowa³eœ wszystkie pojazdy grupowe.");
					}
					case 2:
					{
						//respawn WSZYSTKICH pojazdów
						for(new i = 0 ; i < MAX_VEHICLES ; i++)
						{
							if(vehicle[i][vownertype] == 1)
							{
								new uid = vehicle[i][vuid];
								UnspawnVehicle(i);
								SpawnVehicle(uid);
							}
							else if(vehicle[i][vownertype] == 2 || vehicle[i][vownertype] == 0)
							{
								UnspawnVehicle(i);
								
							}
						}
						
						GuiInfo(playerid,"Respawn wszystkich pojazdów przebieg³ pomyœlnie.");
					}
					case 3:
					{
						// naprawa globalna wozow
						for(new i = 0 ; i < MAX_VEHICLES ; i++)
						{
							vehicle[i][vhp] = 1000;
							SetVehicleHealth(i,1000);
							UpdateVehicleDamageStatus(i, 0, 0, 0, 0);
						}
					}
				}
			}
		}
		case DIAL_AVMENU:
		{
			if(!response) { }
			else
			{
				switch(listitem)
				{
					case 0:			//lista pustych pojazdów
					{
						ShowOwnerlessVehs(playerid);
					}
					case 1:			//unspawn wybranego pojazdu po ID
					{
						ShowPlayerDialog(playerid,DIAL_UNSPAWN,DIALOG_STYLE_INPUT,"Admin Control Panel » Vehicle unspawn","Wpisz sampid pojazdu","Okey","Zakoñcz");
					}
				}
			}
		}
		case DIAL_VSEL:
		{
			if (!response) { }
			else
			{
				switch(listitem)
				{
					case 0:			//spawn unspawn
					{
						SpawnedVehicle[playerid] = SearchSpawnedVehicle(playerid);
						if(SpawnedVehicle[playerid] > 0)
						{
							for(new i = 0; i < MAX_VEHICLES ; i++)
							{	
								if(vehicle[i][vuid] == SpawnedVehicle[playerid])
								{
									UnspawnVehicle(i);
									SpawnedVehicle[playerid] = EOS;
									GuiInfo(playerid,"Odspawnowa³eœ pojazd.");
									break;
								}
							}
						}
						else
						{
							SpawnVehicle(pveh[playerid][SelectedVehicle[playerid]][car_uid]);
							SpawnedVehicle[playerid] = pveh[playerid][SelectedVehicle[playerid]][car_uid];
							GuiInfo(playerid,"Zespawnowa³eœ pojazd.");
						}
					}
				}
			}
		}
		case DIAL_V:
		{
			if (!response) { }
			else
				{
				//wybieranie pojazdu
				new title[128];
				format(title,sizeof(title),"%s » akcja",pveh[playerid][listitem+1][car_name]);
				ShowPlayerDialog(playerid,DIAL_VSEL,DIALOG_STYLE_LIST,title,"1. Spawn/Unspawn\n2. Informacje\n3. Sprzedaj","Wybierz","Zamknij");
				SelectedVehicle[playerid] = listitem+1;
			}
		}
		case DIAL_NOTEPAD:
		{
			if (!response) { }
			else
			{
				new query[256],czas[24],data[24],autor[MAX_PLAYER_NAME],godzina,minuta,sekunda,dzien,miesiac,rok,calosc[256];
				gettime(godzina,minuta,sekunda);
				getdate(rok,miesiac,dzien);
				format(data,sizeof(data),"%i/%i/%i",rok,miesiac,dzien);
				format(czas,sizeof(czas),"%i : %i",godzina,minuta);
				
				GetPlayerName(playerid,autor,sizeof(autor));
				
				format(calosc,sizeof(calosc),"%s\n\nAutor : %s\nData : %s \nGodzina : %s",inputtext,autor,data,czas);
				
				format(query,sizeof(query),"INSERT INTO core_items VALUES(NULL,'%i','%s','%i','%i','%s','%i',0,0,0,0,0,0,0)",GetPlayerUID(playerid),"Karteczka",0,0,calosc,ITEM_PAPERSHEET);
				mysql_query(query);
				
				format(query,sizeof(query),"UPDATE core_items SET item_value1=item_value1-1 WHERE item_uid=%i",NotatnikUID[playerid]);
				
				GuiInfo(playerid,"Karteczka pojawi³a siê w Twoim ekwipunku.");
				PublicMe(playerid,"wyrwa³ karteczkê z notesu.");
				
				NotatnikUID[playerid] = -1;
			}
		}
		case DIAL_L:
		{
			if(!response)
			{
				Kick(playerid);
				printf("[kick] Gracz %s (UID: %i) zostal wyrzucony z serwera - powod: anulowal logowanie",pInfo[playerid][pName],GetPlayerUID(playerid));
			}
			else
			{
				CheckPassword(playerid,inputtext);
			}
		}
		case DIAL_P:
		{
			if(!response) { }
			else
			{
				SelectedItem[playerid] = listitem+1;
				new itemid = SelectedItem[playerid];
				//do dokonczenia
				if(itemInfo[playerid][itemid][iUsed] == 1)
				{
					ShowPlayerDialog(playerid,DIAL_PUSE,DIALOG_STYLE_LIST,itemInfo[playerid][itemid][iName],"1. Schowaj\n2. Upuœæ na ziemiê\n3. Informacje","Wybierz","Zamknij");
				}
				else
				{
					ShowPlayerDialog(playerid,DIAL_PUSE,DIALOG_STYLE_LIST,itemInfo[playerid][itemid][iName],""COL_EQ"1. U¿yj\n2. Upuœæ na ziemiê\n3. Informacje\n4. Wrzuæ do schowka\n5. Podziel","Wybierz","Zamknij");
				}
			}
		}
		case DIAL_PUSE:
		{
			new itemid = SelectedItem[playerid];
			if (response)
			{
				switch(listitem)
				{
					case 0:				//use
					{
						if(itemInfo[playerid][itemid][iType] == ITEM_SPECIALWEAP)		//bron specjalna
						{
							new query[256];
							if(IsPlayerInAnyVehicle(playerid))
							{
								GuiInfo(playerid,"Nie mo¿esz siedzieæ w pojeŸdzie wyci¹gaj¹c lub chowaj¹c broñ.");
								return 1;
							}
							
							new weapDMG = itemInfo[playerid][itemid][iValue2];
							
							if(PlayerCache[playerid][pHasSpecialWeapon] > 0)
							{
								//zdejmij
								PlayerCache[playerid][pHasSpecialWeapon] = 0;
								RemovePlayerAttachedObject(playerid,SLOT_TEST);
								
								format(query,sizeof(query),"UPDATE core_items SET item_used=0 WHERE item_uid=%i",itemInfo[playerid][itemid][iUID]);
								mysql_query(query);
											
								PlayerWeapon[playerid] = -1 ;
							}
							else
							{
								//zaloz
								if(PlayerWeapon[playerid] == -1)
								{
									PlayerCache[playerid][pHasSpecialWeapon] = 1;
									PlayerCache[playerid][pSpecialWeaponDamage] = weapDMG;
									
									SetPlayerAttachedObject(playerid, SLOT_TEST, itemInfo[playerid][itemid][iValue1], SW_BONE, SW_POSX,0,0,SW_ROTX,SW_ROTY,SW_ROTZ,1,1,1);
									
									format(query,sizeof(query),"UPDATE core_items SET item_used=1 WHERE item_uid=%i",itemInfo[playerid][itemid][iUID]);
									mysql_query(query);
									
									PlayerWeapon[playerid] = itemInfo[playerid][itemid][iUID] ;
								}
								else
								{
									GuiInfo(playerid,"Posiadasz ju¿ jak¹œ broñ w u¿yciu.");
								}
								//slot CHUJWDUPIE
							}
						}
						
						if(itemInfo[playerid][itemid][iType] == ITEM_WEAPON)		//broñ
						{
							if(IsPlayerInAnyVehicle(playerid))
							{
								GuiInfo(playerid,"Nie mo¿esz siedzieæ w pojeŸdzie wyci¹gaj¹c lub chowaj¹c broñ.");
								return 1;
							}
							
							if(PlayerWeapon[playerid] == -1)
							{
								if(itemInfo[playerid][itemid][iValue2] == 0)
								{
									GuiInfo(playerid,"Nie masz amunicji w tej broni.");
								}
								else
								{
									GivePlayerWeapon(playerid,itemInfo[playerid][itemid][iValue1],itemInfo[playerid][itemid][iValue2]);
									PlayerWeapon[playerid] = itemInfo[playerid][itemid][iUID] ;
									new query[128];
									UsedWeaponID[playerid] = itemInfo[playerid][itemid][iValue1];
									format(query,sizeof(query),"UPDATE core_items SET item_used=1 WHERE item_uid=%i",itemInfo[playerid][itemid][iUID]);
									mysql_query(query);
									//new output[128];
									
									
									PlayerCache[playerid][pWeaponID] = itemInfo[playerid][itemid][iValue1];
									PlayerCache[playerid][pWeaponAmmo] =  itemInfo[playerid][itemid][iValue2];
								}
							}
							else
							{
								if(PlayerWeapon[playerid] != itemInfo[playerid][itemid][iUID])
								{
									GuiInfo(playerid,"Posiadasz ju¿ inn¹ broñ w u¿yciu.");
									return 1;
								}
								new query[128];
								format(query,sizeof(query),"UPDATE core_items SET item_used=0,item_value2=%i WHERE item_uid=%i",PlayerCache[playerid][pWeaponAmmo] ,itemInfo[playerid][itemid][iUID]);
								mysql_query(query);
								ResetPlayerWeapons(playerid);
								PlayerWeapon[playerid] = -1;
								UsedWeaponID[playerid] = EOS;
								
								//new output[128];
								//format(output,128,"chowa %s do kabury.",itemInfo[playerid][itemid][iName]);
								//PublicMe(playerid,output);
								PlayerCache[playerid][pWeaponID] = EOS;
								PlayerCache[playerid][pWeaponAmmo] = EOS;
							}
						}
						if(itemInfo[playerid][itemid][iType] == ITEM_FOOD)			//jedzenie
						{
							new Float:jaksienazresz,Float:teraz,query[128];
							GetPlayerHealth(playerid,teraz);
							jaksienazresz = teraz + float(itemInfo[playerid][itemid][iValue1]);
							
							if(jaksienazresz > 100)
							{
								jaksienazresz = 100;
							}
							
							SetPlayerHealth(playerid,jaksienazresz);
							pInfo[playerid][pHealth] = jaksienazresz;
							
							new query2[128];
							format(query2,sizeof(query2),"UPDATE core_players SET char_health=%f WHERE char_uid=%i",jaksienazresz,GetPlayerUID(playerid));
							mysql_query(query2);
							
							format(query,sizeof(query),"DELETE FROM core_items WHERE item_uid=%i",itemInfo[playerid][itemid][iUID]);
							mysql_query(query);
							
							SendClientMessage(playerid,COLOR_SERVER,"Spo¿y³eœ pokarm, twój pasek ¿ycia powinien siê w³aœnie powiêkszyæ.");
							
							new msg[128];
							format(msg,sizeof(msg),"spo¿ywa %s.",itemInfo[playerid][itemid][iName]);
							PublicMe(playerid,msg);
						}
						if(itemInfo[playerid][itemid][iType] == ITEM_AMMO)			//amunicja
						{
							GuiInfo(playerid,"Aby prze³adowaæ broñ u¿yj komendy : "COL_GREEN"/przeladuj gunid ammoid\n\n""gunid - uid broni, któr¹ chcesz prze³adowaæ\nammoid - uid magazynka, który powinieneœ mieæ w ekwipunku ");
						}
						if(itemInfo[playerid][itemid][iType] == ITEM_NOTEPAD)		//notatnik
						{
							if(itemInfo[playerid][itemid][iValue1] <= 0)
							{
								new query[256];
								format(query,sizeof(query),"DELETE FROM core_items WHERE item_uid=%i",itemInfo[playerid][itemid][iUID]);
								mysql_query(query);
							}
							else
							{
								NotatnikUID[playerid] = itemInfo[playerid][itemid][iUID];
								ShowPlayerDialog(playerid,DIAL_NOTEPAD,DIALOG_STYLE_INPUT,"Tworzenie notatki","Wpisz poni¿ej treœæ notatki.","Napisz","Anuluj");
							}
						}
						if(itemInfo[playerid][itemid][iType] == ITEM_PC)			//komputer
						{
							//nieu¿ywany narazie
						}
						if(itemInfo[playerid][itemid][iType] == ITEM_PHONE)			//telefon
						{
							SelectedPhone[playerid] = itemInfo[playerid][itemid][iValue1];
							ShowPlayerDialog(playerid,DIAL_PHONE,DIALOG_STYLE_LIST,"Telefon » Menu","1. Twoje kontakty\n2. Dodaj numer\n3. Wy³¹cz/W³¹cz","Wybierz","Zamknij");
						}
						if(itemInfo[playerid][itemid][iType] == ITEM_PAPERSHEET)	//kartka papieru
						{
							new query[256];
							format(query,sizeof(query),"SELECT item_stacjafm FROM core_items WHERE item_uid=%i",itemInfo[playerid][itemid][iUID]);
							mysql_query(query);
							mysql_store_result();
							new zawartosc[512];
							mysql_fetch_string(zawartosc);
							mysql_free_result();
							GuiInfo(playerid,zawartosc);
						}
						if(itemInfo[playerid][itemid][iType] == ITEM_CLOCK)			//zegarek
						{
							new string[256];
							new hour, minute, second;
							gettime(hour, minute, second);

							format(string, sizeof(string), "~w~Godzina:~w~ ~y~%02d:%02d:%02d~y~~w~.", hour, minute, second);
							GameTextForPlayer(playerid,string,5000,5);
							
							PublicMe(playerid,"spojrza³ na zegarek.");
						}
						if(itemInfo[playerid][itemid][iType] == ITEM_CASH)			//kasa
						{
							new query[256];
							format(query,sizeof(query),"UPDATE core_players SET char_money=char_money+%i WHERE char_uid=%i",itemInfo[playerid][itemid][iValue1],GetPlayerUID(playerid));
							mysql_query(query);
							new ilema = GetPlayerMoney(playerid);
							pInfo[playerid][pMoney] = pInfo[playerid][pMoney] + itemInfo[playerid][itemid][iValue1];
							ResetPlayerMoney(playerid);
							GivePlayerMoney(playerid,ilema+itemInfo[playerid][itemid][iValue1]);
							
							format(query,sizeof(query),"DELETE FROM core_items WHERE item_uid=%i",itemInfo[playerid][itemid][iUID]);
							mysql_query(query);
						}
						if(itemInfo[playerid][itemid][iType] == ITEM_CLOTH)			//ubranie
						{
							SetPlayerSkin(playerid,itemInfo[playerid][itemid][iValue1]);
							pInfo[playerid][pSkin] = itemInfo[playerid][itemid][iValue1];
						}
						if(itemInfo[playerid][itemid][iType] == ITEM_COMPONENT)			//tuning auta
						{
							GuiInfo(playerid,"To jest komponent, który mo¿esz wmontowaæ do posiadanego auta, po prostu udaj siê do mechanika.");
						}
						if(itemInfo[playerid][itemid][iType] == ITEM_KAJDANKI)			//kajdanki
						{
							GuiInfo(playerid,"To s¹ kajdanki, aby ich u¿yæ u¿yj komendy /skuj.");
						}
						if(itemInfo[playerid][itemid][iType] == ITEM_WINO)			//wino
						{
							new buffer[256];
							format(buffer,sizeof(buffer),"DELETE FROM core_items WHERE item_uid=%i",itemInfo[playerid][itemid][iUID]);
							mysql_query(buffer);
							SetPlayerSpecialAction(playerid,SPECIAL_ACTION_DRINK_WINE);
							DrunkedTimer[playerid] = 150;
							SetTimerEx("Drunked", 1000, false, "i", playerid);
						}
						if(itemInfo[playerid][itemid][iType] == ITEM_PAPIEROS)			//papieros
						{
							if(itemInfo[playerid][itemid][iValue1] == 1)
							{
								new buffer[256];
								format(buffer,sizeof(buffer),"DELETE FROM core_items WHERE item_uid=%i",itemInfo[playerid][itemid][iUID]);
								mysql_query(buffer);
								SetPlayerSpecialAction(playerid,SPECIAL_ACTION_SMOKE_CIGGY);
							}
							else
							{
								new buffer[256];
								format(buffer,sizeof(buffer),"UPDATE core_items SET item_value1=item_value1-1 WHERE item_uid=%i",itemInfo[playerid][itemid][iUID]);
								mysql_query(buffer);
								SetPlayerSpecialAction(playerid,SPECIAL_ACTION_SMOKE_CIGGY);
							}
						}
						if(itemInfo[playerid][itemid][iType] == ITEM_SPRUNK)			//papieros
						{
							new buffer[256];
							format(buffer,sizeof(buffer),"DELETE FROM core_items WHERE item_uid=%i",itemInfo[playerid][itemid][iUID]);
							mysql_query(buffer);
							SetPlayerSpecialAction(playerid,SPECIAL_ACTION_DRINK_SPRUNK);
						}
						if(itemInfo[playerid][itemid][iType] == ITEM_MASK)			//papieros
						{
							GuiInfo(playerid,"To jest maska, któr¹ mo¿esz za³o¿yæ na twarz by ukryæ swoje oblicze. Aby jej u¿yæ wpisz: /zamaskuj.");
						}
						if(itemInfo[playerid][itemid][iType] == ITEM_MEGAFON)		
						{
							GuiInfo(playerid,"Aby u¿yæ megafonu skorzystaj z komendy /m");
						}
						if(itemInfo[playerid][itemid][iType] == ITEM_KAMIZELKA)			//kamizelka
						{
							if(Kamizelka[playerid] > 0)
							{
								if(Kamizelka[playerid] != itemInfo[playerid][itemid][iUID])
								{
									GuiInfo(playerid,"Posiadasz inn¹ kamizelkê w u¿yciu.");
									return 1;
								}
								
								PublicMe(playerid,"œci¹ga kamizelkê kuloodporn¹.");
								new Float:armour;
								GetPlayerArmour(playerid,armour);
								new armor = floatround(armour);
								new buffer[256];
								format(buffer,sizeof(buffer),"UPDATE core_items SET item_value1 = %i , item_used=0 WHERE item_uid=%i",armor,itemInfo[playerid][itemid][iUID]);
								mysql_query(buffer);
								
								SetPlayerArmour(playerid,0);
								
								Kamizelka[playerid] = EOS;
								statusPlayer[playerid] = 1;
							}
							else
							{
								if(itemInfo[playerid][itemid][iValue1] <= 0)
								{
									GuiInfo(playerid,"Ta kamizelka jest kompletnie zniszczona.");
									return 1;
								}
								
								new Float:armour = float(itemInfo[playerid][itemid][iValue1]);
								PublicMe(playerid,"zak³ada kamizelkê kuloodporn¹.");
								SetPlayerArmour(playerid,armour);
								
								new buffer[256];
								format(buffer,sizeof(buffer),"UPDATE core_items SET  item_used=1 WHERE item_uid=%i",itemInfo[playerid][itemid][iUID]);
								mysql_query(buffer);
								Kamizelka[playerid] = itemInfo[playerid][itemid][iUID];
								statusPlayer[playerid] = 1;
							}
						}
						if(itemInfo[playerid][itemid][iType] == ITEM_PARALIZATOR)	
						{
							if(PlayerWeapon[playerid] == -1)
							{
								if(itemInfo[playerid][itemid][iValue2] == 0)
								{
									GuiInfo(playerid,"Musisz na³adowaæ ten paralizator.");
								}
								else
								{
									UsedWeaponID[playerid] = itemInfo[playerid][itemid][iValue1];
									GivePlayerWeapon(playerid,itemInfo[playerid][itemid][iValue1],itemInfo[playerid][itemid][iValue2]);
									PlayerWeapon[playerid] = itemInfo[playerid][itemid][iUID] ;
									new query[128];
									format(query,sizeof(query),"UPDATE core_items SET item_used=1 WHERE item_uid=%i",itemInfo[playerid][itemid][iUID]);
									mysql_query(query);
									
									new output[128];
									format(output,sizeof(output),"wyci¹ga %s z kabury.",itemInfo[playerid][itemid][iName]);
									PublicMe(playerid,output);
									PlayerCache[playerid][pWeaponID] = itemInfo[playerid][itemid][iValue1];
									PlayerCache[playerid][pWeaponAmmo] =  itemInfo[playerid][itemid][iValue2];
									PlayerCache[playerid][pHasParalyzer] = 1;
								}
							}
							else
							{
								if(PlayerWeapon[playerid] != itemInfo[playerid][itemid][iUID])
								{
									GuiInfo(playerid,"Posiadasz ju¿ inn¹ broñ w u¿yciu.");
									return 1;
								}
								new query[128];
								format(query,sizeof(query),"UPDATE core_items SET item_used=0,item_value2=%i WHERE item_uid=%i",PlayerCache[playerid][pWeaponAmmo],itemInfo[playerid][itemid][iUID]);
								mysql_query(query);
								ResetPlayerWeapons(playerid);
								PlayerWeapon[playerid] = -1;
								UsedWeaponID[playerid] = EOS;
								
								new output[128];
								format(output,128,"chowa %s do kabury.",itemInfo[playerid][itemid][iName]);
								PublicMe(playerid,output);
								PlayerCache[playerid][pWeaponID] = EOS;
								PlayerCache[playerid][pWeaponAmmo] = EOS;
								SetTimerEx("ParalyzerWaitAC", 1000, false, "i", playerid);
							}
						}
						if(itemInfo[playerid][itemid][iType] == ITEM_MARIHUANA)	
						{
							if(itemInfo[playerid][itemid][iValue1] > 1)
							{
								GuiInfo(playerid,"Nie mo¿esz za¿yæ wiêcej ni¿ 1g naraz.");
							}
							else
							{
								new buffer[256];
								format(buffer,sizeof(buffer),"DELETE FROM core_items WHERE item_uid=%i",itemInfo[playerid][itemid][iUID]);
								mysql_query(buffer);
								
								MarihuanaTimer[playerid] = 300;
								SetTimerEx("MarihuanaEffect", 1000, 0, "d", playerid);
								SetPlayerSpecialAction(playerid,SPECIAL_ACTION_SMOKE_CIGGY);
								statusPlayer[playerid] = 1;
							}
						}
						if(itemInfo[playerid][itemid][iType] == ITEM_KOKAINA)	
						{
							if(itemInfo[playerid][itemid][iValue1] > 1)
							{
								GuiInfo(playerid,"Nie mo¿esz za¿yæ wiêcej ni¿ 1g naraz.");
							}
							else
							{
								/*new buffer[256];
								format(buffer,sizeof(buffer),"DELETE FROM core_items WHERE item_uid=%i",itemInfo[playerid][itemid][iUID]);
								mysql_query(buffer);*/
								
								/*AmfetaminaTimer[playerid] = 300;
								SetTimerEx("AmfetaminaEffect", 1000, 0, "d", playerid);
								PublicMe(playerid,"wci¹ga jak¹œ substancjê do nosa.");*/
								GuiInfo(playerid,"To jest wycofany przedmiot, mo¿esz oddaæ go administratorowi.");
								
								
								statusPlayer[playerid] = 1;
							}
						}
						if(itemInfo[playerid][itemid][iType] == ITEM_AMFETAMINA)
						{
							if(itemInfo[playerid][itemid][iValue1] > 1)
							{
								GuiInfo(playerid,"Nie mo¿esz za¿yæ wiêcej ni¿ 1g naraz.");
							}
							else
							{
								new buffer[256];
								format(buffer,sizeof(buffer),"DELETE FROM core_items WHERE item_uid=%i",itemInfo[playerid][itemid][iUID]);
								mysql_query(buffer);
								
								AmfetaminaTimer[playerid] = 300;
								SetTimerEx("AmfetaminaEffect", 1000, 0, "d", playerid);
								PublicMe(playerid,"wci¹ga jak¹œ substancjê do nosa.");
								
								pInfo[playerid][pHealth] = pInfo[playerid][pHealth] + 25;
								if(pInfo[playerid][pHealth] > 100)
								{
									pInfo[playerid][pHealth] = 100;
								}
								
								PlayerCache[playerid][pBonusPower] = 10.0;
								
								statusPlayer[playerid] = 1;
							}
						}
						if(itemInfo[playerid][itemid][iType] == ITEM_CORPSE)
						{
							new output[256];
							format(output,sizeof(output),"SELECT item_stacjafm FROM core_items WHERE item_uid=%i",itemInfo[playerid][itemid][iUID]);
							mysql_query(output);
							mysql_store_result();
							new smierc[128];
							mysql_fetch_string(smierc);
							mysql_free_result();
							
							format(output,sizeof(output),"Opis œmierci: %s",smierc);
							
							ShowPlayerDialog(playerid,404,DIALOG_STYLE_MSGBOX,itemInfo[playerid][itemid][iName],output,"OK","");
						}
						if(itemInfo[playerid][itemid][iType] == ITEM_LAKIER)			//jedzenie		
						{
							if(PlayerWeapon[playerid] == -1)
							{
								if(itemInfo[playerid][itemid][iValue2] == 0)
								{
									GuiInfo(playerid,"To pusta puszka po lakierze.");
								}
								else
								{
									UsedWeaponID[playerid] = itemInfo[playerid][itemid][iValue1];
									GivePlayerWeapon(playerid,itemInfo[playerid][itemid][iValue1],itemInfo[playerid][itemid][iValue2]);
									PlayerWeapon[playerid] = itemInfo[playerid][itemid][iUID] ;
									new query[128];
									format(query,sizeof(query),"UPDATE core_items SET item_used=1 WHERE item_uid=%i",itemInfo[playerid][itemid][iUID]);
									mysql_query(query);
									
									new output[128];
									format(output,sizeof(output),"wyci¹ga %s zza paska.",itemInfo[playerid][itemid][iName]);
									PublicMe(playerid,output);
									PlayerCache[playerid][pWeaponID] = itemInfo[playerid][itemid][iValue1];
									PlayerCache[playerid][pWeaponAmmo] =  itemInfo[playerid][itemid][iValue2];
									PlayerCache[playerid][pHasLakier] = 1;
								}
							}
							else
							{
								if(PlayerWeapon[playerid] != itemInfo[playerid][itemid][iUID])
								{
									GuiInfo(playerid,"Posiadasz ju¿ inn¹ broñ w u¿yciu.");
									return 1;
								}
								new query[128];
								format(query,sizeof(query),"UPDATE core_items SET item_used=0,item_value2=%i WHERE item_uid=%i",PlayerCache[playerid][pWeaponAmmo],itemInfo[playerid][itemid][iUID]);
								mysql_query(query);
								ResetPlayerWeapons(playerid);
								PlayerWeapon[playerid] = -1;
								UsedWeaponID[playerid] = EOS;
								
								new output[128];
								format(output,128,"przypina %s do paska.",itemInfo[playerid][itemid][iName]);
								PublicMe(playerid,output);
								PlayerCache[playerid][pWeaponID] = EOS;
								PlayerCache[playerid][pWeaponAmmo] = EOS;
								SetTimerEx("LakierWaitAC", 1000, false, "i", playerid);
							}
						}
						if(itemInfo[playerid][itemid][iType] == ITEM_ATTACH)
						{
							new buffer[256];
							format(buffer,sizeof(buffer),"SELECT * FROM core_attachments WHERE item_uid=%i",itemInfo[playerid][itemid][iUID] );
							mysql_query(buffer);
							mysql_store_result();
							if(mysql_num_rows() <= 0)
							{
								mysql_free_result();
								format(buffer,sizeof(buffer),"INSERT INTO core_attachments VALUES(NULL,%i,2,0,0,0,0,0,0,1,1,1)",itemInfo[playerid][itemid][iUID] );
								mysql_query(buffer);
							}
							
							
							PlayerCache[playerid][pSelectedAttachment] = itemInfo[playerid][itemid][iUID]; 

							ShowPlayerDialog(playerid,DIAL_ATTACH_MENU,DIALOG_STYLE_LIST,"Co chcesz zrobiæ?","1. U¿yj\n2. Edytuj","Wybierz","Anuluj");
						}
						if(itemInfo[playerid][itemid][iType] == ITEM_SEEDS)
						{
							new doorid  = GetPlayerDoorID(playerid);
							
							if(doorid > 0)
							{
								GuiInfo(playerid,"(( Póki co roœliny mo¿na zasadzaæ tylko na podwórzu. ))");
								return 1;
							}
							
							ApplyAnimation(playerid,"BOMBER","BOM_Plant",4.1,0,0,0,0,0,1);
							CreatePlant(playerid);
							
							new buffer[256];
							format(buffer,sizeof(buffer),"DELETE FROM core_items WHERE item_uid=%i",itemInfo[playerid][itemid][iUID]);
							mysql_query(buffer);
							
							GuiInfo(playerid,"Zasadzi³eœ roœlinkê, teraz musisz poczekaæ a¿ uroœnie aby j¹ zebraæ.");
						}
						if(itemInfo[playerid][itemid][iType] == ITEM_KARNET)
						{
							new doorid  = GetPlayerDoorID(playerid);
							
							if(PlayerCache[playerid][pKarnet] > 0)
							{
								DisableCarnet(playerid);
								return 1;
							}
							
							if(doorid > 0)
							{
								//uzyj karnetu
								EnableCarnet(playerid,itemInfo[playerid][itemid][iUID]);
								PlayerCache[playerid][pKarnetBiznes] = itemInfo[playerid][itemid][iValue2];
							}
							else
							{
								return GuiInfo(playerid,"Nie mo¿esz u¿ywaæ karnetu bêd¹c na zewn¹trz.");
							}
						}
						if(itemInfo[playerid][itemid][iType] == ITEM_EMPTY_STRZYKAWKA)
						{
							GuiInfo(playerid,"To nowy przedmiot, jeszcze nie u¿ywany.");
						}
						if(itemInfo[playerid][itemid][iType] == ITEM_FLASHBANG)
						{
							if(PlayerWeapon[playerid] == -1)
							{
								if(itemInfo[playerid][itemid][iValue2] == 0)
								{
									GuiInfo(playerid,"Nie masz ju¿ granatów.");
								}
								else
								{
									UsedWeaponID[playerid] = itemInfo[playerid][itemid][iValue1];
									GivePlayerWeapon(playerid,itemInfo[playerid][itemid][iValue1],itemInfo[playerid][itemid][iValue2]);
									PlayerWeapon[playerid] = itemInfo[playerid][itemid][iUID] ;
									new query[128];
									format(query,sizeof(query),"UPDATE core_items SET item_used=1 WHERE item_uid=%i",itemInfo[playerid][itemid][iUID]);
									mysql_query(query);
									

									PlayerCache[playerid][pWeaponID] = itemInfo[playerid][itemid][iValue1];
									PlayerCache[playerid][pWeaponAmmo] =  itemInfo[playerid][itemid][iValue2];
									
									IsPlayerHasFlashbang[playerid] = 1;
								}
							}
							else
							{
								if(PlayerWeapon[playerid] != itemInfo[playerid][itemid][iUID])
								{
									GuiInfo(playerid,"Posiadasz ju¿ inn¹ broñ w u¿yciu.");
									return 1;
								}
								new query[128];
								format(query,sizeof(query),"UPDATE core_items SET item_used=0,item_value2=%i WHERE item_uid=%i",PlayerCache[playerid][pWeaponAmmo],itemInfo[playerid][itemid][iUID]);
								mysql_query(query);
								ResetPlayerWeapons(playerid);
								PlayerWeapon[playerid] = -1;
								UsedWeaponID[playerid] = EOS;
								IsPlayerHasFlashbang[playerid] = 0;
								

								PlayerCache[playerid][pWeaponID] = EOS;
								PlayerCache[playerid][pWeaponAmmo] = EOS;
								SetTimerEx("ParalyzerWaitFB", 1000, false, "i", playerid);
							}
						}
						if(itemInfo[playerid][itemid][iType] == ITEM_WYTRYCH_DRZWI)
						{
							new nearDoors = -1;
							for(new i = 0 ; i < MAX_DOORS; i++)
							{
								if(GetPlayerVirtualWorld(playerid) == DoorInfo[i][doorEnterVW])
								{
									if(IsPlayerInRangeOfPoint(playerid,2,DoorInfo[i][doorEnterX],DoorInfo[i][doorEnterY],DoorInfo[i][doorEnterZ]))
									{
										nearDoors = i;
										break;
									}
								}
							}
							
							if(nearDoors == -1)
							{
								GuiInfo(playerid,"Brak jakichkolwiek drzwi w pobli¿u byœ móg³ u¿yæ wytrychu.");
							}
							else
							{
								//otworz te drzwi w poblizu i usun wytrych
								if(DoorInfo[nearDoors][doorClose] == 0)
								{
									GuiInfo(playerid,"Te drzwi s¹ ju¿ otwarte.");
									return 1;
								}
								
								new equals = 0;
								
								new szansa = random(3);
								if(szansa == 2)
								{
									//otworz
									DoorInfo[nearDoors][doorClose] = 0;
									equals = 1;
								}
								else
								{
									//zostaw
									equals = 0;
								}
								
								PublicMe(playerid,"wciska wytrych pomiêdzy zapadki w drzwiach i zaczyna nim krêciæ.");
								
								switch(equals)
								{
									case 0:
									{
										GuiInfo(playerid,"Z³ama³eœ wytrych, zamek okaza³ siê zbyt mocny.");
									}
									case 1:
									{
										//PublicMe(playerid,"wciska wytrych pomiêdzy zapadki w drzwiach i zaczyna nim krêciæ");
										GuiInfo(playerid,"Otworzy³eœ drzwi.");
									}
								}
								
								new buffer[256];
								format(buffer,sizeof(buffer),"DELETE FROM core_items WHERE item_uid=%i",itemInfo[playerid][itemid][iUID]);
								mysql_query(buffer);
							}
						}
						if(itemInfo[playerid][itemid][iType] == ITEM_SWAT_CAMERA)
						{
							if(PlayerCache[playerid][pUseSWATCam] == 1)
							{
								//przestan
								PlayerCache[playerid][pUseSWATCam] = 0;
								SetCameraBehindPlayer(playerid);
									
								SetPlayerVirtualWorld(playerid,CamSWATVW[playerid]);
								SetPlayerPos(playerid,CamSWATPos[playerid][0],CamSWATPos[playerid][1],CamSWATPos[playerid][2]);
								
								return 1;
							}
							
							new nearDoors = -1;
							for(new i = 0 ; i < MAX_DOORS; i++)
							{
								if(GetPlayerVirtualWorld(playerid) == DoorInfo[i][doorEnterVW])
								{
									if(IsPlayerInRangeOfPoint(playerid,2,DoorInfo[i][doorEnterX],DoorInfo[i][doorEnterY],DoorInfo[i][doorEnterZ]))
									{
										nearDoors = i;
										break;
									}
								}
							}
							
							if(nearDoors == -1)
							{
								GuiInfo(playerid,"Brak jakichkolwiek drzwi w pobli¿u byœ móg³ u¿yæ wytrychu.");
							}
							else
							{
								//aplikuj kamere
								if(PlayerCache[playerid][pUseSWATCam] == 0)
								{
									CamSWATVW[playerid] = GetPlayerVirtualWorld(playerid);
									GetPlayerPos(playerid,CamSWATPos[playerid][0],CamSWATPos[playerid][1],CamSWATPos[playerid][2]);
									
									//zacznij ogladac
									PlayerCache[playerid][pUseSWATCam] = 1;
									
									//SetPlayerPos(playerid,DoorInfo[i][doorExitX],DoorInfo[i][doorExitY],DoorInfo[i][doorExitZ]);
									SetPlayerVirtualWorld(playerid,DoorInfo[nearDoors][doorExitVW]);
									SetPlayerInterior(playerid,DoorInfo[nearDoors][doorExitINT]);
									SetPlayerCameraPos(playerid, DoorInfo[nearDoors][doorExitX],DoorInfo[nearDoors][doorExitY],DoorInfo[nearDoors][doorExitZ]);
									SetPlayerCameraLookAt(playerid, DoorInfo[nearDoors][doorExitX]+random(5),DoorInfo[nearDoors][doorExitY]+random(5),DoorInfo[nearDoors][doorExitZ]);
									
									SetTimerEx("RotateCameraIndoor",1000,false,"i",playerid);
									
									CamSWATIndoorPos[playerid][0] = DoorInfo[nearDoors][doorExitX];
									CamSWATIndoorPos[playerid][1] = DoorInfo[nearDoors][doorExitY];
									CamSWATIndoorPos[playerid][2] = DoorInfo[nearDoors][doorExitZ];
									//SetPlayerFacingAngle( playerid, DoorInfo[i][doorExitA]);
								}
								else
								{
									PlayerCache[playerid][pUseSWATCam] = 0;
									SetCameraBehindPlayer(playerid);
									
									SetPlayerVirtualWorld(playerid,CamSWATVW[playerid]);
									SetPlayerPos(playerid,CamSWATPos[playerid][0],CamSWATPos[playerid][1],CamSWATPos[playerid][2]);
								}
							}
						}
						if(itemInfo[playerid][itemid][iType] == ITEM_GLOVES)
						{
							if(PlayerCache[playerid][pGloves] != 0)
							{
								//ma zalozone
								PlayerCache[playerid][pGloves] = 0;
								
								PublicMe(playerid,"œci¹ga rêkawiczki.");
							}
							else
							{
								//nie ma
								PlayerCache[playerid][pGloves] = 1;
								
								PublicMe(playerid,"zak³ada rêkawiczki.");
							}
						}
					}
					case 1:				//drop 		
					{
						if(IsPlayerInAnyVehicle(playerid))
						{
							new vehicleid = GetPlayerVehicleID(playerid);
							if(IsPlayerPermVehicle(playerid,vehicleid))
							{
								//odrzuc item do wozu
								
								if(IsItemUsed( itemInfo[playerid][itemid][iUID]))
								{
									GuiInfo(playerid,"Nie mo¿esz schowaæ u¿ywanego przedmiotu.");
									return 1;
								}
								
								new itemuid = itemInfo[playerid][itemid][iUID];
								DropItemToVehicle(playerid,itemuid);
								PublicMe(playerid,"wrzuca przedmiot do schowka w pojeŸdzie.");
								ApplyAnimation(playerid,"BOMBER","BOM_Plant",4.1,0,0,0,0,0,1);
							}
							else
							{
								GuiInfo(playerid,"Nie masz uprawnieñ do chowania przedmiotów w tym pojeŸdzie.");
							}
						}
						else
						{
							PublicMe(playerid,"wyrzuca coœ na ziemiê.");
							ApplyAnimation(playerid,"BOMBER","BOM_Plant",4.1,0,0,0,0,0,1);
							DropPlayerItem(playerid,itemid);
						}
					}
					case 2:				//info
					{
					
					}
					case 3:				//schowaj do schowka
					{
						new doorid = GetPlayerDoorID(playerid);
						if(doorid > 0)
						{
							PutItemIntoDoors(playerid,itemInfo[playerid][itemid][iUID]);
							GuiInfo(playerid,"Schowa³eœ przedmiot do schowka.");
						}
						else
						{
							GuiInfo(playerid,"Musisz byæ w budynku by chowaæ coœ do schowka.");
						}
					}
					case 4:				//podziel
					{
						if(itemInfo[playerid][itemid][iType] == ITEM_MARIHUANA || itemInfo[playerid][itemid][iType] == ITEM_AMFETAMINA || itemInfo[playerid][itemid][iType] == ITEM_KOKAINA)
						{
							if(itemInfo[playerid][itemid][iValue1] > 1)
							{
								if(CountPlayerItems(playerid) > 30)
								{
									GuiInfo(playerid,"Masz za ma³o miejsca w ekwipunku aby podzieliæ przedmioty.");
									return 1;
								}
								else
								{
									switch(itemInfo[playerid][itemid][iType])
									{
										case ITEM_MARIHUANA:
										{
											new buffer[256],newName[32];
											format(newName,sizeof(newName),"Marihuana (%ig)",itemInfo[playerid][itemid][iValue1] - 1);
											format(buffer,sizeof(buffer),"UPDATE core_items SET item_value1 = item_value1 - 1, item_name='%s' WHERE item_uid=%i",newName,itemInfo[playerid][itemid][iUID]);
											mysql_query(buffer);
											
											format(buffer,sizeof(buffer),"INSERT INTO core_items(item_owneruid,item_whereis,item_value1,item_type,item_name) VALUES(%i,%i,%i,%i,'Marihuana (1g)')",GetPlayerUID(playerid),WHEREIS_GRACZ,1,ITEM_MARIHUANA);
											mysql_query(buffer);
											GuiInfo(playerid,"Podzieli³eœ przedmioty.");
											
										}
										case ITEM_AMFETAMINA:
										{
											new buffer[256],newName[32];
											format(newName,sizeof(newName),"Amfetamina (%ig)",itemInfo[playerid][itemid][iValue1] - 1);
											format(buffer,sizeof(buffer),"UPDATE core_items SET item_value1 = item_value1 - 1, item_name='%s' WHERE item_uid=%i",newName,itemInfo[playerid][itemid][iUID]);
											mysql_query(buffer);
											
											format(buffer,sizeof(buffer),"INSERT INTO core_items(item_owneruid,item_whereis,item_value1,item_type,item_name) VALUES(%i,%i,%i,%i,'Amfetamina (1g)')",GetPlayerUID(playerid),WHEREIS_GRACZ,1,ITEM_AMFETAMINA);
											mysql_query(buffer);
											GuiInfo(playerid,"Podzieli³eœ przedmioty.");
										}
									}
								}
							}
							else
							{
								GuiInfo(playerid,"Za ma³o sztuk tego przedmiotu, by go podzieliæ.");
							}
						}
						else
						{
							GuiInfo(playerid,"Nie da siê podzieliæ tego przedmiotu.");
						}
					}
				}
			}
			else { }
			SelectedItem[playerid] = -1;
		}
		case DIAL_PP:
		{
			if (!response) { }
			else
			{
				new query[512];
				format(query,sizeof(query),"SELECT * FROM core_items WHERE item_owneruid=%i",GetPlayerUID(playerid));
				mysql_query(query);
				mysql_store_result();
				if(mysql_num_rows() > 30)
				{
					GuiInfo(playerid,"Nie masz wiêcej miejsca w ekwipunku.");
					return 1;
				}
				mysql_free_result();
				
				//podnoszenie itema
				format(query,sizeof(query),"UPDATE core_items SET item_owneruid=%i, item_whereis=0, item_posx = 0,item_posy = 0, item_posz = 0 WHERE item_uid=%i",GetPlayerUID(playerid),PodnoszoneLista[playerid][listitem]);
				mysql_query(query);
				
				new query_usun[256];
				format(query_usun,sizeof(query_usun),"SELECT item_uid,item_objectid FROM core_items WHERE item_uid=%i",PodnoszoneLista[playerid][listitem]);
				mysql_query(query_usun);
				mysql_store_result();
				new resline[256],itemuid,objectid;
				while(mysql_fetch_row(resline,"|"))
				{
					sscanf(resline,"p<|>ii",itemuid,objectid);
				}
				PublicMe(playerid,"podnosi coœ z ziemi.");
				ApplyAnimation(playerid,"BOMBER","BOM_Plant",4.1,0,0,0,0,0,1);
				DestroyDynamicObject(objectid);
				object[objectid][object_model] = -1;
				new czystka[256];
				format(czystka,sizeof(czystka),"UPDATE core_items SET item_objectid=0 WHERE item_uid=%i",PodnoszoneLista[playerid][listitem]);
				mysql_query(czystka);
			}
		}
	}
	return 0;
}

forward CheckConnectPing(playerid);
public CheckConnectPing(playerid)
{
	if(GetPlayerPing(playerid) > 1500) Kick(playerid);
	return 1;
}

public OnPlayerClickMap(playerid, Float:fX, Float:fY, Float:fZ)
{
	if(pInfo[playerid][pAdmin] > 0)
	{
		SetPlayerPosFindZ(playerid, fX, fY, fZ); 
	}
	
    return 1;
}

public OnPlayerConnect(playerid)
{
	//SetTimerEx("CheckConnectPing", 650, false, "i", playerid);
	
	idleTime[playerid] = 0;
	CheckArrested[playerid] = 0;
	ArrestTime[playerid] = 0 ;
	ClearPlayerVehiclesTable(playerid);
	pInfo[playerid][pUID] = 0;
	pInfo[playerid][pName][0]= EOS;
	pInfo[playerid][pMoney] =  0 ;
	pInfo[playerid][pHealth]  =0;
	pInfo[playerid][pX] =0;
	pInfo[playerid][pY] = 0;
	pInfo[playerid][pZ] = 0;
	pInfo[playerid][pA]= 0;
	pInfo[playerid][pLogged] =0; 
	pInfo[playerid][pGID] = 0;
	pInfo[playerid][pSkin] = 0;
	pInfo[playerid][pBw] = 0;
	pInfo[playerid][pAj] = 0;
	pInfo[playerid][pBank] =0; 
	pInfo[playerid][pWarns] = 0;
	pInfo[playerid][pGamescore] = 0;
	pInfo[playerid][pWiek]= 0;
	pInfo[playerid][pBankNR]= 0;
	pInfo[playerid][pHasDowod] = 0;
	pInfo[playerid][pHasPrawkoA] = 0;
	pInfo[playerid][pHasPrawkoB] = 0;
	pInfo[playerid][pHasPrawkoC] = 0;
	pInfo[playerid][pHasPrawkoCE] =0;
	pInfo[playerid][pHasPrawkoD] =0;
	pInfo[playerid][pSex] =0;
	pInfo[playerid][pWorld] =0;
	pInfo[playerid][pInterior] = 0;
	pInfo[playerid][pSpawnType] = 0;
	pInfo[playerid][blockVehicles] = 0;
	pInfo[playerid][blockOOC] = 0;
	pInfo[playerid][blockChar] = 0;
	pInfo[playerid][pVehMask] = 0;
	pInfo[playerid][pPremium] = 0;
	pInfo[playerid][pGMCars] = 0;
	pInfo[playerid][pGMGroups] = 0;
	pInfo[playerid][pGMItems] = 0;
	pInfo[playerid][pGMDoors] = 0;
	pInfo[playerid][fightingStyle] = 0;
	pInfo[playerid][pSpecPossible] = 0;
	pInfo[playerid][pShootskill] = 0;
	
	
	//przyczepialne
	PlayerAttachment[0][attachmentItemUid] = 0;
	PlayerAttachment[1][attachmentItemUid] = 0;
	
	AmfetaminaTimer[playerid] = 0;
	MarihuanaTimer[playerid] = 0;
	KokainaTimer[playerid] = 0;
	LSDTimer[playerid] = 0;
	
	
	Logged[playerid] = 0;
	LogTry[playerid] = 0;
	
	new masked1 = random(9);
	new masked2 =	random(9);
	new masked3 = random(9);
	new masked4 = random(9);
	
	new fs[12];
	format(fs,sizeof(fs),"%i%i%i%i",masked1,masked2,masked3,masked4);
	new final = strval(fs);
	//new final = masked1+masked2+masked3+masked4;
	
	PlayerCache[playerid][pMaskedNumbers] = final;
	
	TextDrawHideForPlayer(playerid, Text:TextDrawDutyTime[playerid]);
	
	SelectedVehicleItem[playerid] = 0;
	CamSWATVW[playerid] = 0;

	
	IsPlayerHasFlashbang[playerid] = 0;
	SoldVehicle[playerid] = EOS;
	Sprzedawca[playerid] = EOS;
	Kupiec[playerid] = EOS;
	OfferPrice[playerid] = EOS;
	IsPlayerDuty[playerid] = EOS;
	PlayerDutyGroup[playerid] = EOS;
	DutyGroupType[playerid] = EOS;
	PlayerCache[playerid][pEditLabel] = 0;
	PlayerCache[playerid][pHasInterview] = 0;
	PlayerCache[playerid][pRepairCar] = 0;
	PlayerCache[playerid][pVehCheckpoint] = 0;
	PlayerCache[playerid][pColourCar] = 0;
	PlayerCache[playerid][lakierTicks] = 0;
	PlayerCache[playerid][lastLakierTick] = 0;
	PlayerCache[playerid][pIPChecked] = 0;
	PlayerCache[playerid][pZP] = 0;
	PlayerCache[playerid][pMetersRide] = 0;
	PlayerCache[playerid][pTaxed] = false;
	PlayerCache[playerid][pRubbishDrive] = 0;
	PlayerCache[playerid][pRubbishCheckpoint] = false;
	PlayerCache[playerid][pGroupListPage] = 0;
	PlayerCache[playerid][pSelectedAttachment] = 0;
	PlayerCache[playerid][pEditAttachment] = 0;
	PlayerCache[playerid][pBuyAttach] = 0;
	PlayerCache[playerid][pSelAttach] = 0;
	PlayerCache[playerid][pHited] = 0;
	PlayerCache[playerid][pShootedWeapon] = 0;
	PlayerCache[playerid][pShootedUID] = 0;
	PlayerCache[playerid][pShootedType] = 0;
	PlayerCache[playerid][pCacheAccess] = 0;
	PlayerCache[playerid][pSelStarted] = 0;
	PlayerCache[playerid][pHasSpecialWeapon] = 0;
	PlayerCache[playerid][pArea] = 0;
	PlayerCache[playerid][pTrain] = 0;
	PlayerCache[playerid][pTrainTime] = 0;
	PlayerCache[playerid][pGymFaza] = 0;
	PlayerCache[playerid][pGymType] = 0;
	PlayerCache[playerid][pKarnet] = 0;
	PlayerCache[playerid][pKarnetTime] = 0;
	PlayerCache[playerid][pKarnetBiznes] = 0;
	PlayerCache[playerid][pDoorsDuty] = 0;
	PlayerCache[playerid][pOnlineSec] = 0;
	PlayerCache[playerid][pBonusPower] = 0;
	PlayerCache[playerid][pUseSWATCam] = 0;
	PlayerCache[playerid][pGPS] = 1;
	PlayerCache[playerid][pGPSCP] = 0;
	PlayerCache[playerid][pGloves] = 0;
	PlayerCache[playerid][pHidden] = 0;	
	PlayerCache[playerid][pCornered] = 0;
	PlayerCache[playerid][pCornerID] = -1;
	PlayerCache[playerid][pCornerTime] = 0;
	
	
	RepairTime[playerid] = 0;
	PlayerBwTimer[playerid] = 0;
	RepaintTime[playerid] = EOS;
	
	DrunkedTimer[playerid] = EOS;
	
	PlayerDutySeconds[playerid] = EOS;
	
	if(!IsPlayerNPC(playerid))
	{
		SetTimerEx("IsPlayerLogged", 30000, false, "i", playerid);
	}
	
	SetTimerEx("CountPlayerSecondsInGame", 60000, false, "i", playerid);
	
	//CheckPlayerLogged(playerid);
	
	SelectedItem[playerid] = -1 ;
	
	pInfo[playerid][pBankomat] = 0;
	
	pInfo[playerid][pAdmin] = 0;
	
	CarryPackage[playerid] = 0;
	
	pInfo[playerid][pCarryPackage] = 0;
	
	pInfo[playerid][pSpectate] = 0;
	
	pInfo[playerid][pTogW] = 0;
	
	FirstSpawn[playerid] = 1;
	
	//PlayerDesc[playerid] = CreateDynamic3DTextLabel(" ", COLOR_ME, 0.0, 0.0, -0.6, 10.0, playerid, INVALID_VEHICLE_ID, 1);
	
	/*PlayerStatus[playerid] = CreateDynamic3DTextLabel(" ", COLOR_WHITE, 0.0, 0.0, 0.1, 10.0, playerid, INVALID_VEHICLE_ID, 1);*/
	
	pInfo[playerid][pCuffPlayer] = -1;
	
	pInfo[playerid][pCuffed] = 0;
	
	CzyRozmawia[playerid] = 0;
	
	IsPlayerComputerUse[playerid] = 0;
	
	InitFly(playerid);
	
	ToWho[playerid] = -1;
	
	SelectedVehicle[playerid] = -1;
	
	SelectedAdminDoors[playerid] = 0;
	
	LoadPlayerObjectTextdraws(playerid);
	
	SpawnedVehicle[playerid] = -1;
	
	NotatnikUID[playerid] = -1;
	
	new hour,minute;
	gettime(hour, minute);
	
	SetPlayerTime(playerid,hour,minute);
	
	PlayerWeapon[playerid] = -1;
	
	GetPlayerVehicles(playerid);
	
	Preloaded[playerid] = 0;
	
	pInfo[playerid][pCuffed] = 0;
	
	pInfo[playerid][pMasked] = 0;
	
	setGunSkill(playerid);
	
	SetPlayerColor(playerid,COLOR_GREY);
	
	pInfo[playerid][pSID] = playerid;
	
	TextDrawShowForPlayer(playerid, Text:TextDrawSanNews);
	
	TextDrawShowForPlayer(playerid, Text:ServerName);
	
	ClearPlayerGroupsTable(playerid);
	
	LoadGroupsOnConnect(playerid);
	
	return 1;
}

public OnPlayerUpdate(playerid)
{
	// If the player just came back from being afk, the AFK variable is most likely more than 3/5 (vary this based on your gamemode and experience)
    if(AFK[playerid] > 0.5)
    {
		// This player just came back from being AFK
		PlayerCache[playerid][pAfk] = false;
		statusPlayer[playerid] = 1;
    }
    AFK[playerid] = 0;
	
	for(new i = 0 ; i < 10; i++)
	{
		if(IsValidDynamicObject(object[PoliceKolczatka[i]][object_sid]))
		{
			new Float:Pos[3];
			if(IsPlayerInAnyVehicle(playerid))
			{
				new seat = GetPlayerVehicleSeat(playerid);
				if(seat == 0)
				{
					GetDynamicObjectPos(object[PoliceKolczatka[i]][object_sid],Pos[0],Pos[1],Pos[2]);
					
					if(IsPlayerInRangeOfPoint(playerid,2.5,Pos[0],Pos[1],Pos[2]))
					{
						new vehicleid = GetPlayerVehicleID(playerid);
						
						new opony = random(16);
						
						new panels,doors,lights,tires;
						GetVehicleDamageStatus(vehicleid,panels,doors,lights,tires);
						
						UpdateVehicleDamageStatus(vehicleid,panels,doors,lights,opony);
						vehicle[vehicleid][vtires] = opony;
					}
				}
			}
		}
	}
	
	//zapisywanie pozycji
	new Float:x,Float:y,Float:z;
	GetPlayerPos(playerid,x,y,z);
	pInfo[playerid][pX] = x;
	pInfo[playerid][pY] = y;
	pInfo[playerid][pZ] = z;
	
	if(PlayerCache[playerid][pTrain] > 0)
	{
		new keysa, uda, lra;
		GetPlayerKeys(playerid, keysa, uda, lra);
		
		if(PlayerCache[playerid][pGymType] == TRAIN_TYPE_HANTLE)
		{
			//hantelki jedziemy
			if(keysa & KEY_SPRINT)
			{
				if(PlayerCache[playerid][pGymFaza] == 1)
				{
					PlayerCache[playerid][pGymFaza] = 2;
					ApplyAnimation(playerid, "FREEWEIGHTS", "gym_free_A", 4.0, 0, 0, 0, 1, 0, 1);
					
					SetTimerEx("TrainTick", 1000, false, "i", playerid);
				}
			}
			
			if(keysa & KEY_SECONDARY_ATTACK)
			{
				if(PlayerCache[playerid][pGymFaza] == 1)
				{
					PlayerCache[playerid][pGymFaza] = 0;
					//ApplyAnimation(playerid, "FREEWEIGHTS", "gym_free_putdown", 4.0, 0, 0, 0, 0, 0, 1);
					
					StopPlayerTrain(playerid);
				}
			}
		}
	}
	
	if(PlayerCache[playerid][pBuyAttach] > 0)
	{
		new Keys,ud,lr;
		GetPlayerKeys(playerid,Keys,ud,lr);
		GetPlayerPos(playerid,x,y,z);
			
		if(lr == KEY_LEFT)
		{
			//przewin w lewo
			if(PlayerCache[playerid][pSelAttach] == MAX_ATTACHMENTS)
			{
				PlayerPlaySound(playerid, 5205, x, y, z);
			}
			else
			{
				PlayerCache[playerid][pSelAttach] = PlayerCache[playerid][pSelAttach] + 1;			
				RemovePlayerAttachedObject(playerid,SLOT_CACHE);
					
				new attid = PlayerCache[playerid][pSelAttach] ;
					
				SetPlayerAttachedObject(playerid, SLOT_CACHE, AttachItems[attid][aModel], AttachItems[attid][aBone], AttachItems[attid][aPosX],AttachItems[attid][aPosY],AttachItems[attid][aPosZ],AttachItems[attid][aRotX],AttachItems[attid][aRotY],AttachItems[attid][aRotZ],AttachItems[attid][aScaleX],AttachItems[attid][aScaleY],AttachItems[attid][aScaleZ]);
				new koszt[16];
				format(koszt,sizeof(koszt),"koszt: ~g~%i$",AttachItems[attid][aPrice]);
				GameTextForPlayer(playerid, koszt, 5000, 6);
			}
		}
			
		if(lr == KEY_RIGHT)
		{
			//przewin w prawo
			if(PlayerCache[playerid][pSelAttach] <= 0)
			{
				PlayerPlaySound(playerid, 5205, x, y, z);
			}
			else
			{
				PlayerCache[playerid][pSelAttach] = PlayerCache[playerid][pSelAttach] -1;			
				RemovePlayerAttachedObject(playerid,SLOT_CACHE);
					
				new attid = PlayerCache[playerid][pSelAttach] ;
					
				SetPlayerAttachedObject(playerid, SLOT_CACHE, AttachItems[attid][aModel], AttachItems[attid][aBone], AttachItems[attid][aPosX],AttachItems[attid][aPosY],AttachItems[attid][aPosZ],AttachItems[attid][aRotX],AttachItems[attid][aRotY],AttachItems[attid][aRotZ],AttachItems[attid][aScaleX],AttachItems[attid][aScaleY],AttachItems[attid][aScaleZ]);
				new koszt[16];
				format(koszt,sizeof(koszt),"koszt: ~g~%i$",AttachItems[attid][aPrice]);
				GameTextForPlayer(playerid, koszt, 5000, 6);
			}
		}
	}
	
	//miotanie celownikiem jak celuje
	if(GetPlayerWeapon(playerid) > 10)
	{
		new Keys,ud,lr;
		GetPlayerKeys(playerid,Keys,ud,lr);
		GetPlayerPos(playerid,x,y,z);
		
		if(Keys == KEY_AIM)
		{
			if(pInfo[playerid][pShootskill] < 20)
			{
				if(GetPlayerDrunkLevel(playerid) < 5000)
				{
					SetPlayerDrunkLevel(playerid,5000);
				}
			}
			else if(pInfo[playerid][pShootskill] < 40)
			{
				if(GetPlayerDrunkLevel(playerid) < 4000)
				{
					SetPlayerDrunkLevel(playerid,4000);
				}
			}
			else if(pInfo[playerid][pShootskill] < 60)
			{
				if(GetPlayerDrunkLevel(playerid) < 2500)
				{
					SetPlayerDrunkLevel(playerid,2500);
				}
			}
			else if(pInfo[playerid][pShootskill] < 80)
			{
				if(GetPlayerDrunkLevel(playerid) < 500)
				{
					SetPlayerDrunkLevel(playerid,500);
				}
			}
			else
			{
				//nie bujaj wcale
				SetPlayerDrunkLevel(playerid,0);
			}
		}
		
		if(Keys == KEY_AIM + KEY_FIRE)
		{
			pInfo[playerid][pShootskill] = pInfo[playerid][pShootskill] + 0.008;
		}
		
		SetTimerEx("StopScreenMoveOnFire", 1800, false, "i", playerid);
	}
	
	return 1;
}

forward StopScreenMoveOnFire(playerid);
public StopScreenMoveOnFire(playerid)
{
	new Keys,ud,lr;
	GetPlayerKeys(playerid,Keys,ud,lr);
	
	if(Keys == KEY_AIM)
	{
		return 1;
	}
	
	SetPlayerDrunkLevel(playerid,0);
	
	return 1;
}

forward SetPlayerExhausted(playerid);
public SetPlayerExhausted(playerid)
{
	if(idleTime[playerid] > 0)
	{
		idleTime[playerid]--;
		ApplyAnimation(playerid, "PED", "IDLE_tired", 4, 1, 0, 0, 0, 0, 0);
		SetTimerEx("SetPlayerExhausted", 1000, false, "i", playerid);
	}
	else
	{
		ApplyAnimation(playerid, "CARRY", "crry_prtial", 4.1, 0, 0, 0, 0, 0);
		idleTime[playerid] = 0;
	}
}

public OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
	new Float:x,Float:y,Float:z;
	GetPlayerPos(playerid,x,y,z);
	
	if(newkeys & KEY_LOOK_BEHIND)
	{
		if(IsPlayerInAnyVehicle(playerid))
		{
			new vehicleid = GetPlayerVehicleID(playerid);
			if(IsValidObject(VehicleKogut[vehicleid]))
			{
				new rand = random(2);
				switch(rand)
				{
					case 0:
					{
						DestroyObject(VehicleKogut[vehicleid]);
						VehicleKogut[vehicleid] = CreateObject(19419, 0,0,0, 0.0, 0.0, 96.0); 
						AttachObjectToVehicle(VehicleKogut[vehicleid], vehicleid, 0.0, 0.0, 0.85, 0.0, 0.0, 0.0);
					}
					case 1:
					{
						DestroyObject(VehicleKogut[vehicleid]);
						VehicleKogut[vehicleid] = CreateObject(19420, 0,0,0, 0.0, 0.0, 96.0); 
						AttachObjectToVehicle(VehicleKogut[vehicleid], vehicleid, 0.0, 0.0, 0.85, 0.0, 0.0, 0.0);
					}
				}
			}
		}
	}
	
	if(newkeys == KEY_LEFT)
	{
		if(PlayerCache[playerid][pSelSkin] == true)
		{
			if(SelectedSkin[playerid] == 0)
			{
				PlayerPlaySound(playerid, 5205, x, y, z);
			}
			else
			{
				SelectedSkin[playerid]--;
				SetPlayerSkin(playerid,SkinInfo[SelectedSkin[playerid]][sModel]);

			}
		}
	}
	
	/*if(newkeys & KEY_LEFT)
	{
		if(PlayerCache[playerid][pBuyAttach] > 0)
		{
			//przewin w lewo
			if(PlayerCache[playerid][pSelAttach] == MAX_ATTACHMENTS)
			{
				PlayerPlaySound(playerid, 5205, x, y, z);
			}
			else
			{
				PlayerCache[playerid][pSelAttach] = PlayerCache[playerid][pSelAttach] + 1;			
				RemovePlayerAttachedObject(playerid,SLOT_CACHE);
				
				new attid = PlayerCache[playerid][pSelAttach] ;
				
				SetPlayerAttachedObject(playerid, SLOT_CACHE, AttachItems[attid][aModel], AttachItems[attid][aBone], AttachItems[attid][aPosX],AttachItems[attid][aPosY],AttachItems[attid][aPosZ],AttachItems[attid][aRotX],AttachItems[attid][aRotY],AttachItems[attid][aRotZ],AttachItems[attid][aScaleX],AttachItems[attid][aScaleY],AttachItems[attid][aScaleZ]);
				new koszt[16];
				format(koszt,sizeof(koszt),"koszt: ~g~%i$",AttachItems[attid][aPrice]);
				GameTextForPlayer(playerid, koszt, 5000, 6);
			}
		}
	}
	
	if(newkeys & KEY_RIGHT)
	{
		if(PlayerCache[playerid][pBuyAttach] > 0)
		{
			//przewin w prawo
			if(PlayerCache[playerid][pSelAttach] <= 0)
			{
				PlayerPlaySound(playerid, 5205, x, y, z);
			}
			else
			{
				PlayerCache[playerid][pSelAttach] = PlayerCache[playerid][pSelAttach] -1;			
				RemovePlayerAttachedObject(playerid,SLOT_CACHE);
				
				new attid = PlayerCache[playerid][pSelAttach] ;
				
				SetPlayerAttachedObject(playerid, SLOT_CACHE, AttachItems[attid][aModel], AttachItems[attid][aBone], AttachItems[attid][aPosX],AttachItems[attid][aPosY],AttachItems[attid][aPosZ],AttachItems[attid][aRotX],AttachItems[attid][aRotY],AttachItems[attid][aRotZ],AttachItems[attid][aScaleX],AttachItems[attid][aScaleY],AttachItems[attid][aScaleZ]);
				new koszt[16];
				format(koszt,sizeof(koszt),"koszt: ~g~%i$",AttachItems[attid][aPrice]);
				GameTextForPlayer(playerid, koszt, 5000, 6);
			}
		}
	}*/
	
	if(newkeys == KEY_YES)
	{
		return cmd_brama(playerid,"");
	}
	
	if(newkeys == KEY_RIGHT)
	{
		if(PlayerCache[playerid][pSelSkin] == true)
		{
			if(SelectedSkin[playerid] > 299)
			{
				PlayerPlaySound(playerid, 5205, x, y, z);
			}
			else
			{
				SelectedSkin[playerid]++;
				SetPlayerSkin(playerid,SkinInfo[SelectedSkin[playerid]][sModel]);
			}
		}
	}
	
	if(newkeys & KEY_WALK)
	{
		if(PlayerCache[playerid][pBuyAttach] > 0)
		{
			SetCameraBehindPlayer(playerid);
			TogglePlayerControllable(playerid,1);
			PlayerCache[playerid][pSelAttach] = 0;
			PlayerCache[playerid][pBuyAttach] = 0;
			TextDrawHideForPlayer(playerid,TextDrawSkins[playerid]);
			RemovePlayerAttachedObject(playerid,SLOT_CACHE);
		}
		
		if(PlayerCache[playerid][pSelSkin] == true)
		{
			PlayerCache[playerid][pSelSkin] = false;
			SetCameraBehindPlayer(playerid);
			SetPlayerSkin(playerid,pInfo[playerid][pSkin]);
			TextDrawHideForPlayer(playerid,TextDrawSkins[playerid]);
			TogglePlayerControllable(playerid,1);
		}
	}
		
	if(newkeys & KEY_AIM)
	{
		if(pInfo[playerid][pCuffed] == 0)
		{
			if(pInfo[playerid][pBw] == 0)
			{
				if(CzyRozmawia[playerid] == 0)
				{
					if(GetPlayerWeapon(playerid) <= 0)
					{
						if(PlayerCache[playerid][pPlayAnim] == true)
						{
							ApplyAnimation(playerid, "CARRY", "crry_prtial", 4.1, 0, 0, 0, 0, 0);
							PlayerCache[playerid][pPlayAnim] = false;
						}
					}
				}
			}
		}
	}
	
	if(newkeys & KEY_SECONDARY_ATTACK)
	{
		if(pInfo[playerid][pBw] == 0)
		{
			if(PlayerCache[playerid][pSelSkin] == true)
			{
				
				new msg[256];
				format(msg,256,"Czy na pewno chcesz kupiæ to ubranie za "COL_GREEN"%i$?",SkinInfo[SelectedSkin[playerid]][sPrice]);
				ShowPlayerDialog(playerid,DIAL_PURCHASE_CLOATH,DIALOG_STYLE_MSGBOX,"Informacja",msg,"Tak","Nie");
			}
			
			if(PlayerCache[playerid][pBuyAttach] > 0)
			{
				new msg[256];
				format(msg,256,"Czy na pewno chcesz kupiæ to akcesorium za "COL_GREEN"%i$?",AttachItems[PlayerCache[playerid][pSelAttach]][aPrice]);
				ShowPlayerDialog(playerid,DIAL_PURCHASE_ATTACH,DIALOG_STYLE_MSGBOX,"Informacja",msg,"Tak","Nie");
			}
			
			if(pInfo[playerid][pCuffed] > 0)
			{
				ClearAnimations(playerid);
			}
		}
	}
	
	if(newkeys & KEY_FIRE)
	{
		if(IsPlayerInAnyVehicle(playerid))
		{
			new vehicleid = GetPlayerVehicleID(playerid);
			new seat = GetPlayerVehicleSeat(playerid);
			if(seat == 0)
			{
				new engine, lights, alarm, doors, bonnet, boot, objective;
				GetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, bonnet, boot, objective);
				
				if(engine == 1 || engine == 0)
				{
					if(lights == 0)
					{
						SetVehicleParamsEx(vehicleid, engine, 1, alarm, doors, bonnet, boot, objective);
					}
					else
					{
						SetVehicleParamsEx(vehicleid, engine, 0, alarm, doors, bonnet, boot, objective);
					}
				}
			}
		}
		
		if(pInfo[playerid][pBw] == 0)
		{
			if(PlayerCache[playerid][pSelSkin] == true)
			{
				//anuluj wybieranie
			}
			if(pInfo[playerid][pCuffed] > 0)
			{
				ClearAnimations(playerid);
			}
			else
			{
				if(PlayerCache[playerid][pHasSpecialWeapon] > 0)
				{
					new anim = random(4);
					switch(anim)
					{
						case 0:
						{
							ApplyAnimation(playerid, "BASEBALL", "Bat_1",4, 0, 1, 1, 0, 0, 1);
						}
						case 1:
						{
							ApplyAnimation(playerid, "BASEBALL", "Bat_2",4, 0, 1, 1, 0, 0, 1);
						}
						case 2:
						{
							ApplyAnimation(playerid, "BASEBALL", "Bat_3",4, 0, 1, 1, 0, 0, 1);
						}
						case 3:
						{
							ApplyAnimation(playerid, "BASEBALL", "Bat_4",4, 0, 1, 1, 0, 0, 1);
						}
					}
					
					for(new i=0; i < MAX_PLAYERS; i++)
					{
						if(IsPlayerFacingPlayer(playerid, i, 3))
						{
							pInfo[i][pHealth] = pInfo[i][pHealth]  - PlayerCache[playerid][pSpecialWeaponDamage];
							
							new animVictim = random(3);
							switch(animVictim)
							{
								case 0:
								{
									ApplyAnimation(i, "BASEBALL", "Bat_Hit_1",4, 0, 1, 1, 0, 0, 1);
								}
								case 1:
								{
									ApplyAnimation(i, "BASEBALL", "Bat_Hit_2",4, 0, 1, 1, 0, 0, 1);
								}
								case 2:
								{
									ApplyAnimation(i, "BASEBALL", "Bat_Hit_3",4, 0, 1, 1, 0, 0, 1);
								}
							}
						}
					}
				}
			}
			
			if(PlayerCache[playerid][pHasLakier] > 0 && GetPlayerWeapon(playerid) == 41)
			{
				PlayerCache[playerid][lakierTicks]++;
			}
		}
		
		
	}
	
	
	if(newkeys & KEY_JUMP)
	{
		if(pInfo[playerid][pBw] == 0)
		{
			if(pInfo[playerid][pCuffed] > 0)
			{
				ApplyAnimation(playerid, "GYMNASIUM", "gym_jog_falloff", 4.1, 0, 0, 0, 0, 0);
			}
		}
	}
	
	if(newkeys == KEY_SPRINT + KEY_WALK)
	{
		new vw = GetPlayerVirtualWorld(playerid);
		for (new i = 0 ; i< MAX_DOORS; i ++ )
		{
			if(IsPlayerInRangeOfPoint(playerid,2.0,DoorInfo[i][doorEnterX],DoorInfo[i][doorEnterY],DoorInfo[i][doorEnterZ]))
			{
				if(vw != DoorInfo[i][doorEnterVW])
				{
					continue;
					//return 1;
				}
				if(DoorInfo[i][doorClose] == 0)
				{
					if(DoorInfo[i][doorEntry] > 0)
					{
						if(pInfo[playerid][pMoney] >= DoorInfo[i][doorEntry])
						{
							pInfo[playerid][pMoney] = pInfo[playerid][pMoney] - DoorInfo[i][doorEntry];
							TakePlayerMoney(playerid,DoorInfo[i][doorEntry]);
						}
						else
						{
							GuiInfo(playerid,"Nie posiadasz tyle pieniêdzy.");
							return 1;
						}
					}
					SetPlayerPos(playerid,DoorInfo[i][doorExitX],DoorInfo[i][doorExitY],DoorInfo[i][doorExitZ]);
					SetPlayerVirtualWorld(playerid,DoorInfo[i][doorExitVW]);
					SetPlayerFacingAngle( playerid, DoorInfo[i][doorExitA]);
					SetPlayerInterior(playerid,DoorInfo[i][doorExitINT]);
					OnEnterToDoor(playerid);
					PlayerCache[playerid][pAudioHandle] = Audio_PlayStreamed(playerid, DoorInfo[i][doorMuzyka]);
				}
				else
				{
					GameTextForPlayer(playerid, "~r~Drzwi sa zamkniete", 5000, 5);
				}
			}
			else if(IsPlayerInRangeOfPoint(playerid,2.0,DoorInfo[i][doorExitX],DoorInfo[i][doorExitY],DoorInfo[i][doorExitZ]))
			{
				if(vw != DoorInfo[i][doorExitVW])
				{
					continue;
					//return 1;
				}
				if(DoorInfo[i][doorClose] == 0)
				{
					SetPlayerPos(playerid,DoorInfo[i][doorEnterX],DoorInfo[i][doorEnterY],DoorInfo[i][doorEnterZ]);
					SetPlayerVirtualWorld(playerid,DoorInfo[i][doorEnterVW]);
					SetPlayerFacingAngle( playerid, DoorInfo[i][doorEnterA]);
					SetPlayerInterior(playerid,DoorInfo[i][doorEnterINT]);
					OnEnterToDoor(playerid);
					Audio_Stop(playerid, PlayerCache[playerid][pAudioHandle]);
				}
				else
				{
					GameTextForPlayer(playerid, "~r~Drzwi sa zamkniete", 5000, 5);
				}
			}
		}
	}
	return 1;
}

public OnPlayerRequestSpawn(playerid)
{
	return 0;
}

public OnPlayerRequestClass(playerid,classid)
{
	SetSpawnInfo(playerid, 0,0,0,0,0,0,0,0,0,0,0,0);
	SpawnPlayer(playerid);
}

public OnPlayerDisconnect(playerid,reason)
{
	if(reason == 0)
	{
		SendDisconnectMsg(playerid,D_TYPE_TIMEOUT);
	}
	
	if(reason == 1)
	{
		SendDisconnectMsg(playerid,D_TYPE_Q);
	}
	
	if(reason == 2)
	{
		//SendDisconnectMsg(playerid,D_TYPE_QS);
		QuickSave(playerid);
		SavePlayerInfo(playerid);
		Kick(playerid);
	}
	
	LogTry[playerid] = 0;
	new query2[256];
	format(query2,sizeof(query2),"UPDATE core_players SET char_logged=0 WHERE char_uid=%i",GetPlayerUID(playerid));
	mysql_query(query2);
	
	UpdateDynamic3DTextLabelText(Text3D:PlayerDesc[playerid], COLOR_ME, " ");
	
	if(reason == 2)
	{
		//QuickSave(playerid);
		//SavePlayerInfo(playerid);
	}
	else
	{
		SavePlayerInfo(playerid);
		new query[128];
		format(query,sizeof(query),"UPDATE core_players SET char_posx='%f',char_posy='%f',char_posz='%f' WHERE char_uid=%i",0,0,0,GetPlayerUID(playerid));
		mysql_query(query);
	}
	
	new buffer[256];
	format(buffer,sizeof(buffer),"UPDATE core_items SET item_used = 0 WHERE item_owneruid=%i",GetPlayerUID(playerid));
	mysql_query(buffer);
	
	SavePlayerStats(playerid);
	
	PlayerWeapon[playerid] = -1;
	SetPlayerVehicleToUnspawn(playerid);
	
	SoldVehicle[playerid] = 0;
	Sprzedawca[playerid] = 0;
	Kupiec[playerid] = 0;
	OfferPrice[playerid] = 0;
	OfferType[playerid] = 0;
	pInfo[playerid][pUID] = EOS;
	
	pInfo[playerid][pAdmin] = 0;
	

	if(PlayerBwTimer[playerid] > 0)
	{
		new Float:x,Float:y,Float:z;
		GetPlayerPos(playerid,x,y,z);
		format(buffer,sizeof(buffer),"UPDATE core_players SET char_posx=%f, char_posy=%f,char_posz=%f WHERE char_uid=%i",pInfo[playerid][pX],pInfo[playerid][pY],pInfo[playerid][pZ],GetPlayerUID(playerid));
		mysql_query(buffer);
	}
	
	pInfo[playerid][pAdmin] = 0;
	
	CarryPackage[playerid] = 0 ;
	
	//czyszczenie tablicy admina jesli jest na duty
	for(new i=0; i < MAX_ADMINS;i++)
	{
		if(Admins[i][aSid] == playerid)
		{
			Admins[i][aSid] = EOS;
			break;
		}
	}
	
	if(PlayerCache[playerid][pKarnet] > 0)
	{
		DisableCarnet(playerid);
	}
	
	ResetPlayerWeapons(playerid);
	
	CzyRozmawia[playerid] = 0;
	
	ClearPlayerGroupsTable(playerid);
	
	ToWho[playerid] = -1;
	
	ResetPlayerMoney(playerid);
	
	KillTimer(AjTimer[playerid]);
	
	CleanPlayerTable(playerid);
	
	ClearPlayerVehiclesTable(playerid);
	
	TextDrawHideForPlayer(playerid, Text:TextDrawSanNews);
	
	TextDrawHideForPlayer(playerid, Text:ServerName);
	
	ClearPlayerGroupsTable(playerid);
	
	return 1;
}

public OnPlayerSpawn(playerid)
{
	UpdateDynamic3DTextLabelText(Text3D:PlayerDesc[playerid], COLOR_ME, " ");
	UpdateDynamic3DTextLabelText(Text3D:PlayerNick[playerid], COLOR_WHITE, " ");
	
	statusPlayer[playerid] = 1;
	
	if(!IsPlayerNPC(playerid))
	{
		if(Logged[playerid] == 0)
		{
			TogglePlayerSpectating(playerid,1);
			ShowPlayerDialog(playerid,DIAL_L,DIALOG_STYLE_PASSWORD,"Logowanie","Wpisz swoje has³o, aby po³¹czyæ siê z serwerem.\nOD TERAZ U¯YWAJ HAS£A DO KONTA GLOBALNEGO!","Gotowe","Wyloguj");
			//Logged[playerid] = 1;
			return 1;
		}
	}
	//reszta
	
	if(pInfo[playerid][blockChar] > 0)
	{
		printf("[kick]Gracz %s (UID: %i) zosta³ wyrzucony z serwera - postac zablokowana",pInfo[playerid][pName],GetPlayerUID(playerid));
		Kick(playerid);
	}
	
	if(CheckPlayerAccess(playerid) < PERMS_SUPPORT)
	{
		if(debugMode > 0)
		{
			if(pInfo[playerid][pPremium] <= 0)
			{
				Kick(playerid);
			}
		}
	}
	
	UsedWeaponID[playerid] = EOS;

	
	PlayerCache[playerid][pWeaponID] = EOS;
	PlayerCache[playerid][pWeaponAmmo] = EOS;
	PlayerCache[playerid][pHasParalyzer] = EOS;
	PlayerCache[playerid][pHasLakier] = EOS;
	
	SpawnedVehicle[playerid] = SearchSpawnedVehicle(playerid);
	
	//ShowPlayerProgressBar(playerid, stamina[playerid]);
	//SetProgressBarValue(stamina[playerid], 100.0);
	
	PlayerWeapon[playerid] = -1;
	
	PlayerCache[playerid][pSelSkin] = false;
	
	Kamizelka[playerid] = 0;
	
	IsPlayerBanned(playerid);			//sprawdzanie czy ziomek zbanowany jest
	CitySpawn(playerid);		// nowa func
	//SetTimerEx("HouseSpawnIfExist", 3000, 0, "d", playerid);
	GivePlayerMoney(playerid,pInfo[playerid][pMoney]);
	SetPlayerHealth(playerid,pInfo[playerid][pHealth]);
	SetPlayerSkin(playerid,pInfo[playerid][pSkin]);
	SetPlayerColor(playerid,COLOR_GREY);
	CheckIsJailed(playerid);
	//SetTimerEx("SetVariables", 1000, false, "i", playerid);
	GetPlayerPhoneInUse(playerid);
	SetPlayerScore(playerid,GetPlayerGameScore(playerid));
	
	if(PlayerCache[playerid][pSpecClose] != 0)
	{
		SetPlayerPos(playerid,PlayerCache[playerid][pSpecX],PlayerCache[playerid][pSpecY],PlayerCache[playerid][pSpecZ]);
		SetPlayerVirtualWorld(playerid,PlayerCache[playerid][pSpecVW]);
		PlayerCache[playerid][pSpecClose] = 0;
	}
	
	
	if(Preloaded[playerid] == 0)
	{
		/*PreloadAnimLib(playerid, "PED");
		PreloadAnimLib(playerid, "CARRY");
		PreloadAnimLib(playerid, "BOMBER");
		PreloadAnimLib(playerid, "INT_HOUSE");*/
		
		PreloadAnimLib(playerid, "FREEWEIGHTS");
		PreloadAnimLib(playerid, "BOMBER");
   		PreloadAnimLib(playerid, "RAPPING");
    	PreloadAnimLib(playerid, "SHOP");
   		PreloadAnimLib(playerid, "BEACH");
   		PreloadAnimLib(playerid, "SMOKING");
    	PreloadAnimLib(playerid, "FOOD");
    	PreloadAnimLib(playerid, "ON_LOOKERS");
    	PreloadAnimLib(playerid, "DEALER");
		PreloadAnimLib(playerid, "CRACK");
		PreloadAnimLib(playerid, "CARRY");
		PreloadAnimLib(playerid, "COP_AMBIENT");
		PreloadAnimLib(playerid, "PARK");
		PreloadAnimLib(playerid, "INT_HOUSE");
		PreloadAnimLib(playerid, "INT_OFFICE");
		PreloadAnimLib(playerid, "FOOD");
		PreloadAnimLib(playerid, "PED");
		PreloadAnimLib(playerid, "POLICE");
		PreloadAnimLib(playerid, "CAR");
		PreloadAnimLib(playerid, "CAR_CHAT");
		PreloadAnimLib(playerid, "MEDIC");
		PreloadAnimLib(playerid, "GANGS");
		PreloadAnimLib(playerid, "BENCHPRESS");
		PreloadAnimLib(playerid, "HEIST9");
		PreloadAnimLib(playerid, "MISC");
		PreloadAnimLib(playerid, "OTB");
		PreloadAnimLib(playerid, "PAULNMAC");
		PreloadAnimLib(playerid, "SWEET");
		PreloadAnimLib(playerid, "GRAFFITI");
		PreloadAnimLib(playerid, "FIGHT_C");
		PreloadAnimLib(playerid, "FIGHT_B");
		PreloadAnimLib(playerid, "FIGHT_D");
		PreloadAnimLib(playerid, "WAYFARER");
		PreloadAnimLib(playerid, "BASEBALL");
		PreloadAnimLib(playerid, "GRENADE");
		PreloadAnimLib(playerid, "BSKTBALL");
		PreloadAnimLib(playerid, "RIOT");
		Preloaded[playerid] = 1;
	}
	
	GetPlayerSpawnedVehicle(playerid);
	
	if(pInfo[playerid][pPremium] > 0)
	{
		SetPlayerColor(playerid,COLOR_GOLD);
	}
	
	if(FirstSpawn[playerid] == 1)
	{
		//GivePlayerStartVehicle(playerid);
		CheckPlayerBW(playerid);
		
		SetTimerEx("SetVariables", 1000, false, "i", playerid);
		
		ShowProgressBarForPlayer(playerid, stamina[playerid]);
		SetProgressBarMaxValue(stamina[playerid], pInfo[playerid][pCondition]);
		SetProgressBarValue(stamina[playerid], pInfo[playerid][pCondition]);
		UpdateProgressBar(stamina[playerid], playerid);
		
		PlayerCache[playerid][pCacheAccess] = CheckPlayerAccess(playerid);
		
		if(CheckPlayerAccess(playerid) > 0)
		{
			switch(CheckPlayerAccess(playerid))
			{
				case 1:
				{
					TextDrawSetString(Text:TextDrawSpecialPlayer[playerid], "Ranga: ~h~~r~Helper");
				}
				case 2:
				{
					TextDrawSetString(Text:TextDrawSpecialPlayer[playerid], "Ranga: ~h~~b~Supporter");
				}
				case 3:
				{
					TextDrawSetString(Text:TextDrawSpecialPlayer[playerid], "Ranga: ~g~Gamemaster");
				}
				case 4:
				{
					TextDrawSetString(Text:TextDrawSpecialPlayer[playerid], "Ranga: ~h~~r~Moderator Gry");
				}
				case 5:
				{
					TextDrawSetString(Text:TextDrawSpecialPlayer[playerid], "Ranga: ~r~Programista");
				}
			}
			TextDrawShowForPlayer(playerid,TextDrawSpecialPlayer[playerid]);
		}
		else
		{
			TextDrawHideForPlayer(playerid,TextDrawSpecialPlayer[playerid]);
		}
		
		if(pInfo[playerid][pPremium] > 0)
		{
			TextDrawSetString(Text:TextDrawSpecialPlayer[playerid], "Konto ~g~premium");
			TextDrawShowForPlayer(playerid,TextDrawSpecialPlayer[playerid]);
		}
		
		FirstSpawn[playerid] = 0;
		
		//wiadomosc
		if(testingMode == 1)
		{
			new msg[512];
			format(msg,sizeof(msg),"Tryb testowy jest uruchomiony.\n"COL_RED"Nie twórz tutaj interiorów, pojazdów etc. - to nie zostanie zapisane!");
			GuiInfo(playerid,msg);
		}
	}
	
	
	new access = GetPlayerAccess(playerid);	
	switch(access)
	{
		case 1:
		{
			GameTextForPlayer(playerid, "Uprawnienia ~h~~r~helpera.", 5000, 5);
		}
		case 2:
		{
			GameTextForPlayer(playerid, "Uprawnienia ~b~supportera.", 5000, 5);
		}
		case 3:
		{
			GameTextForPlayer(playerid, "Uprawnienia ~g~gamemastera.", 5000, 5);
		}
		case 4:
		{
			GameTextForPlayer(playerid, "Uprawnienia ~y~moderatora.", 5000, 5);
		}	
		case 5:
		{
			GameTextForPlayer(playerid, "Uprawnienia ~r~programisty.", 5000, 5);
		}
	}
	
	if(pInfo[playerid][pBw] != 0)
	{
		SetPlayerBW(playerid);
		ApplyAnimation(playerid, "CRACK", "crckidle2", 4, 0, 0, 1, 1, 0, 0);
	}
	
	setGunSkill(playerid);
	
	ApplyDildo(playerid);
	
	SetPlayerFightingStyle(playerid,pInfo[playerid][fightingStyle]);
	
	LoadIcons(playerid);
	
	//tutaj by³ problem - CJ na blueberry i tam sobie siedzia³ bez staminy
	//ShowProperties(playerid);
	
	if(CheckArrested[playerid] <= 0)
	{
		if(IsPlayerArrested(playerid))
		{
			//ustaw do pierdla
			new buffer[256];
			format(buffer,sizeof(buffer),"SELECT minutes FROM lspd_arrests WHERE gainer = %i",GetPlayerUID(playerid));
			mysql_query(buffer);
			mysql_store_result();
			ArrestTime[playerid] = mysql_fetch_int();
			mysql_free_result();
			
			new jail = random(5);
			SetPlayerVirtualWorld(playerid,3169);
			SetPlayerPos(playerid,ArrestSpot[jail][aX],ArrestSpot[jail][aY],ArrestSpot[jail][aZ]);
			OnEnterToDoor(playerid);
			CheckArrested[playerid] = 1;
			SetTimerEx("PoliceArrest", 500, false, "i", playerid);
		}
	}
	
	return 1;
}

public OnPlayerDeath(playerid, killerid, reason)
{

	if(GetPlayerWeapon(killerid) > 0)
	{
		PlayerCache[playerid][pShootedWeapon] = 1;
		new weapuid = PlayerWeapon[killerid] ;
		PlayerCache[playerid][pShootedUID] = weapuid; 
		PlayerCache[playerid][pShootedType] = GetPlayerWeapon(killerid);
	}
	else
	{
		PlayerCache[playerid][pShootedWeapon] = 0;
	}
	
	GetPlayerPos(playerid,PlayerCache[playerid][pDeadX],PlayerCache[playerid][pDeadY],PlayerCache[playerid][pDeadZ]);
	SetTimerEx("TimerBW", 500, false, "i", playerid);
	
	if(killerid == INVALID_PLAYER_ID )
	{
		new adminLog[128];
		format(adminLog,sizeof(adminLog),"~r~[smierc]~y~ Gracz %s ~g~(UID: %i | SID: %i)~y~ popelnil samobojstwo lub spadl z wysokosci",pInfo[playerid][pName],GetPlayerUID(playerid),playerid);
		LogAdminAction(adminLog);
	}
	else
	{
		new adminLog[128];
		format(adminLog,sizeof(adminLog),"~r~[smierc]~y~ Gracz %s ~g~(UID: %i | SID: %i)~y~ zostal zabity przez gracza %s (UID: %i | SID: %i)",pInfo[playerid][pName],GetPlayerUID(playerid),playerid,pInfo[killerid][pName],GetPlayerUID(killerid),playerid);
		LogAdminAction(adminLog);
	}

	return 1;
}

public OnPlayerModelSelection(playerid, response, listid, modelid)
{
    if(listid == vehiclelist)
    {
        if(response)
        {
			new buffer[256];
			
			new vehicleID;
			for(new i = 0 ; i < 300; i++)
			{
				if(VehicleData[i][vModel] == modelid)
				{
					vehicleID = i;
					break;
				}
			}
			
			//nadaj pojazd o modelid
			new color1 = random(200);
			new color2 = random(200);
			new fuel = random(30);
			new owner = GetPlayerUID(playerid);
			
			new vid = random(15);
			new Float:x = NewVehiclePos[vid][vx];
			new Float:y = NewVehiclePos[vid][vy];
			new Float:z = NewVehiclePos[vid][vz];
			
			
			format(buffer,sizeof(buffer),"INSERT INTO vehicles_list VALUES (NULL,%i,1000,%i,%i,0,'%s',%i,%f,%f,%f,0,%i,%i,3,'',%i,0,0,0,0,0,0,1,0,1)",0,owner,TYPE_PLAYER,VehicleData[vehicleID][vName],modelid,x,y,z,color1,color2,fuel);
			mysql_query(buffer);
			
			GuiInfo(playerid,"Pojazd zosta³ dodany do listy.");
		}
        return 1;
    }
    return 1;
}

public OnPlayerEnterCheckpoint(playerid)
{
	if(PlayerCache[playerid][pGPSCP] == 1)
	{
		DisablePlayerCheckpoint(playerid);
		return 1;
	}
	
    if(PlayerCache[playerid][pVehCheckpoint] == 1)
	{
		PlayerCache[playerid][pVehCheckpoint] = 0;
		DisablePlayerCheckpoint(playerid);
		
		return 1;
	}
	
	if(PlayerCache[playerid][pRubbishCheckpoint] == true)
	{
		if(!IsPlayerInAnyVehicle(playerid))
		{
			vlive_err(playerid,"musisz byæ w pojeŸdzie by dostarczyæ pizzê");
			return 1;
		}
		
		if(GetVehicleModel(GetPlayerVehicleID(playerid)) != 448)
		{
			vlive_err(playerid,"musisz byæ w pojeŸdzie do rozwo¿enia pizzy");
			return 1;
		}
		
		PlayerCache[playerid][pRubbishCheckpoint] = false;
		RandPlayerCash(playerid);
		DisablePlayerCheckpoint(playerid);
		PlayerCache[playerid][pRubbishDrive] = 0;
	}
    return 1;
}

public OnMysqlQuery(resultid, spareid, MySQL:handle)
{
	mysqlPerformed++;
	return 1;
}

stock GivePlayerStartVehicle(playerid)
{
	new buffer[256];
	format(buffer,sizeof(buffer),"SELECT char_time FROM core_players WHERE char_uid=%i",GetPlayerUID(playerid));
	mysql_query(buffer);
	mysql_store_result();
	new char_time = mysql_fetch_int();
	mysql_free_result();
	
	if(char_time <= 3600)
	{
		if(!IsPlayerHasStartVehicle(playerid))
		{
			ShowModelSelectionMenu(playerid, vehiclelist, "Wybierz pojazd startowy");
		}
	}
}

stock GetPlayerHours(playerid)
{
	new buffer[256];
	format(buffer,sizeof(buffer),"SELECT char_time FROM core_players WHERE char_uid=%i",GetPlayerUID(playerid));
	mysql_query(buffer);
	mysql_store_result();
	new char_time = mysql_fetch_int();
	mysql_free_result();
	
	new Float:time = float(char_time);
	time = time/60/60;
	
	return floatround(time); // <-- zwraca godziny!
}

forward TimerBW(playerid);
public TimerBW(playerid)
{
	new buffer[256];
	format(buffer,sizeof(buffer),"UPDATE core_players SET char_health=14, char_bwtime=%i WHERE char_uid=%i",CZAS_BW,GetPlayerUID(playerid));
	mysql_query(buffer);
	
	new Float:x,Float:y,Float:z;
	GetPlayerPos(playerid,x,y,z);
	PlayerCache[playerid][pCX] = x;
	PlayerCache[playerid][pCY] = y;
	PlayerCache[playerid][pCZ] = z;
	
	format(buffer,sizeof(buffer),"UPDATE core_items SET item_used = 0 WHERE item_uid=%i",PlayerWeapon[playerid]);
	mysql_query(buffer);
	
	PlayerWeapon[playerid] = -1;
	
	PlayerCache[playerid][pWeaponID] = EOS;
	
	ResetPlayerWeapons(playerid);
	
	ResetPlayerMoney(playerid);
	
	PlayerBwTimer[playerid] = CZAS_BW*60;
	
	SetPlayerBW(playerid);
	
	pInfo[playerid][pBw] = 1;
	
	statusPlayer[playerid] = 1;
	
	SetTimerEx("IsPlayerBW", 1000, false, "i", playerid);
	
	GetPlayerPos(playerid,PlayerCache[playerid][pDeadX],PlayerCache[playerid][pDeadY],PlayerCache[playerid][pDeadZ]);
	
	//na pewno?
	SetSpawnInfo(playerid, NO_TEAM, pInfo[playerid][pSkin], PlayerCache[playerid][pDeadX],PlayerCache[playerid][pDeadY],PlayerCache[playerid][pDeadZ], 0, 0, 0, 0, 0, 0, 0);	
	SpawnPlayer(playerid);
	
	return 1;
}

public OnPlayerText(playerid,text[])
{
	if(pInfo[playerid][pBw] > 0)
	{
		return 0;
	}
	
	//emotki
	if(text[0] == ':')
	{
		if(text[1] == '(')
		{
			PublicMe(playerid,"robi smutn¹ minê.");
			return 0;
		}
		if(text[1] == ')')
		{
			PublicMe(playerid,"uœmiecha siê.");
			return 0;
		}
		if(text[1] == 'D' || text[1] == 'd' )
		{
			PublicMe(playerid,"œmieje siê.");
			return 0;
		}
		if(text[1] == 'o' || text[1] == 'O')
		{
			PublicMe(playerid,"robi zdziwion¹ minê.");
			return 0;
		}
		if(text[1] == 'P' || text[1] == 'p' )
		{
			PublicMe(playerid,"wystawia jêzyk.");
			return 0;
		}
		if(text[1] == '/' )
		{
			PublicMe(playerid,"krzywi siê.");
			return 0;
		}
	}
	

	//animki
	if(text[0] == '.')
	{
		if(pInfo[playerid][pBw] != 0)
		{
			GuiInfo(playerid,"Nie mo¿esz odzywaæ siê bêd¹c nieprzytomnym.");
			return 1;
		}
		
		if(PlayerCache[playerid][pTrain] != 0)
		{
			GuiInfo(playerid,"Nie mo¿esz odzywaæ siê podczas treningu.");
			return 1;
		}
		
		
		//new bool:found = false;
	    for(new anim_id = 1; anim_id < MAX_ANIMATIONS; anim_id++)
	    {
			if(!isnull(AnimInfo[anim_id][aCommand]))
			{
	        	if(!strcmp(text, AnimInfo[anim_id][aCommand], true))
	        	{
	        	    if(AnimInfo[anim_id][aAction] == 0)
	        	    {
	        	    	ApplyAnimation(playerid, AnimInfo[anim_id][aLib], AnimInfo[anim_id][aName], AnimInfo[anim_id][aSpeed], AnimInfo[anim_id][aOpt1], AnimInfo[anim_id][aOpt2], AnimInfo[anim_id][aOpt3], AnimInfo[anim_id][aOpt4], AnimInfo[anim_id][aOpt5], 1);
					}
					else
					{
                        SetPlayerSpecialAction(playerid, AnimInfo[anim_id][aAction]);
					}
					PlayerCache[playerid][pPlayAnim] = true;
					//found = true;
					return 0;
	        	}
	        }
			else
			{
				vlive_err(playerid,"nie ma takiej animacji");
				return 0;
			}
	    }
		
	}
	
	
	//zwykle gadanie IC
	if(pInfo[playerid][pBw] != 0)
	{
		GuiInfo(playerid,"Nie mo¿esz odzywaæ siê bêd¹c nieprzytomnym.");
		return 0;
	}
	if (chatstat == 1)
	{
		return chatstat;
	}
	if (chatstat == 0)
	{
		new sending[256],pname[MAX_PLAYER_NAME],imie[24],nazwisko[24],Float:x,Float:y,Float:z;
		GetPlayerName(playerid,pname,sizeof(pname));
		text[0] = toupper(text[0]);
		GetPlayerPos(playerid,x,y,z);
		if(pInfo[playerid][pAdmin] > 0)
		{
			format(sending,sizeof(sending),"%s mówi: %s",GetPlayerGlobalNickname(playerid),text);
		}
		else
		{
			if(pInfo[playerid][pMasked] > 0)
			{
				new maskedMsg[32];
				format(maskedMsg,sizeof(maskedMsg),"Zamaskowany %i",PlayerCache[playerid][pMaskedNumbers]);
				format(sending,sizeof(sending),"%s mówi: %s",maskedMsg,text);
			}
			else
			{
				sscanf(pname,"p<_>s[24]s[24]",imie,nazwisko);	
				format(sending,sizeof(sending),"%s %s mówi: %s",imie,nazwisko,text);
			}
		}
		
		new sendervw = GetPlayerVirtualWorld(playerid);
		
		new buffer[256];
		format(buffer,sizeof(buffer),"INSERT INTO log_texting VALUES(NULL,'%s',%i,'%s','%s')",sending,GetPlayerUID(playerid),datelog(),timelog());
		mysql_query(buffer);
		
		if(IsPlayerInAnyVehicle(playerid))
		{
			new vehicleid = GetPlayerVehicleID(playerid);
			
			if(vehicle[vehicleid][vSzyba] == 1)
			{
				for(new i = 0 ; i < MAX_PLAYERS; i++)
				{
					if(IsPlayerInVehicle(i,vehicleid))
					{
						SendCustomPlayerMessage(i, COLOR_1m, sending);
					}
				}
				
				return 0;
			}
		}
		
		for(new i = 0; i < MAX_PLAYERS+1;i++)
		{
			if(IsPlayerInAnyVehicle(i))
			{
				new vehicleid = GetPlayerVehicleID(i);
				if(vehicle[vehicleid][vSzyba] == 1)
				{
					continue;
				}
			}
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
	return chatstat;
}

// -- FUNC -- //
stock GetPlayerPremium(playerid)
{
	new buffer[256];
	format(buffer,sizeof(buffer),"SELECT premium FROM mysa_members WHERE member_id=%i",pInfo[playerid][pGID]);
	mysql_query(buffer);
	mysql_store_result();
	if(mysql_fetch_int() > 0)
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

public GetPlayerInfo(playerid)
{
	new query[512];
	new pname[MAX_PLAYER_NAME];
	GetPlayerName(playerid,pname,sizeof(pname));
	format(query,sizeof(query),"SELECT `char_uid`,`char_name`,`char_money`,`char_health`,`char_posx`,`char_posy`,`char_posz`,`char_angle`,`char_logged`,`char_gid`,`char_skin`,`char_bwtime`,`char_ajtime`,`char_bank`,`char_warns`,`char_gamescore`,`char_wiek`,`char_banknr`,`dowod`,`prawkoa`,`prawkob`,`prawkoc`,`prawkoce`,`prawkod`,`sex`,`char_world`,`char_interior`,`char_spawn`,`char_kondycja`,`char_time`,`char_sila`,`char_fs`,`char_shootskill` FROM core_players WHERE char_name='%s'",pname);
	mysql_query(query);
	mysql_store_result();
	new resline[512];
	while(mysql_fetch_row(resline,"|"))
	{
		sscanf(resline,"p<|>is[32]ifffffiiiiiiiiiiiiiiiiiiiififif",
		pInfo[playerid][pUID],
		pInfo[playerid][pName],
		pInfo[playerid][pMoney],
		pInfo[playerid][pHealth],
		pInfo[playerid][pX],
		pInfo[playerid][pY],
		pInfo[playerid][pZ],
		pInfo[playerid][pA],
		pInfo[playerid][pLogged],
		pInfo[playerid][pGID],
		pInfo[playerid][pSkin],
		pInfo[playerid][pBw],
		pInfo[playerid][pAj],
		pInfo[playerid][pBank],
		pInfo[playerid][pWarns],
		pInfo[playerid][pGamescore],
		pInfo[playerid][pWiek],
		pInfo[playerid][pBankNR],
		pInfo[playerid][pHasDowod],
		pInfo[playerid][pHasPrawkoA],
		pInfo[playerid][pHasPrawkoB],
		pInfo[playerid][pHasPrawkoC],
		pInfo[playerid][pHasPrawkoCE],
		pInfo[playerid][pHasPrawkoD],
		pInfo[playerid][pSex],
		pInfo[playerid][pWorld],
		pInfo[playerid][pInterior],
		pInfo[playerid][pSpawnType],
		pInfo[playerid][pCondition],
		pInfo[playerid][pSeconds],
		pInfo[playerid][pPower],
		pInfo[playerid][fightingStyle],
		pInfo[playerid][pShootskill]);
		
		pInfo[playerid][maxCondition] = pInfo[playerid][pCondition];		// potem moze edycja

		printf("[testing] Typ spawnu gracza %s - [%i]",pInfo[playerid][pName],pInfo[playerid][pSpawnType]);
	}
	printf("\n%s %s : gracz %s pomyœlnie siê zalogowa³\n",SERVERNAME,VERSION,pInfo[playerid][pName]);
	mysql_free_result();
	
	if(CheckPlayerAccess(playerid) > 0 )
	{
		GetAdminPermissions(playerid);
	}
	
	pInfo[playerid][pPremium] = GetPlayerPremium(playerid);
	
	return 1;
}

public SavePlayerInfo(playerid)
{
	//zapisywanie informacji o graczu
	new query[512];
	format(query,sizeof(query),"UPDATE core_players SET char_money=%i, char_health=%f, char_skin=%i WHERE char_uid=%i",pInfo[playerid][pMoney],pInfo[playerid][pHealth],GetPlayerSkin(playerid),GetPlayerUID(playerid));
	mysql_query(query);
	return 1;
}

stock SavePlayerStats(playerid)
{
	//zapis statystyk postaci
	new Float:kondycja = pInfo[playerid][maxCondition];
	new Float:sila = pInfo[playerid][pPower];
	new Float:shoot = pInfo[playerid][pShootskill] ;
	
	
	//zapis kodycji
	new buffer[256];
	if(kondycja > 100)
	{
		format(buffer,sizeof(buffer),"UPDATE core_players SET char_kondycja = 100 WHERE char_uid=%i",GetPlayerUID(playerid));
	}
	else
	{
		format(buffer,sizeof(buffer),"UPDATE core_players SET char_kondycja = %f WHERE char_uid=%i",kondycja,GetPlayerUID(playerid));
	}
	mysql_query(buffer);
	
	//zapis si³y
	if(sila > 100)
	{
		format(buffer,sizeof(buffer),"UPDATE core_players SET char_sila = 100 WHERE char_uid=%i",GetPlayerUID(playerid));
	}
	else
	{
		format(buffer,sizeof(buffer),"UPDATE core_players SET char_sila = %f WHERE char_uid=%i",sila,GetPlayerUID(playerid));
	}
	mysql_query(buffer);
	
	//zapis si³y
	if(shoot > 100)
	{
		format(buffer,sizeof(buffer),"UPDATE core_players SET char_shootskill = 100 WHERE char_uid=%i",GetPlayerUID(playerid));
	}
	else
	{
		format(buffer,sizeof(buffer),"UPDATE core_players SET char_shootskill = %f WHERE char_uid=%i",shoot,GetPlayerUID(playerid));
	}
	mysql_query(buffer);
	
	return 1;
}

public GuiInfo(playerid,text[])
{
	//szybkie tworzenie okienka z informacja
	//example : GuiInfo(playerid,"Jesteœ na serwerze");
	ShowPlayerDialog(playerid,404,DIALOG_STYLE_MSGBOX,"Informacja",text,"Okej","");
	return 1;
}

stock IsPlayerRunning(playerid)
{
    new keys,ud,lr;

    GetPlayerKeys(playerid, keys, ud, lr);
	
     if(keys == KEY_WALK + KEY_SPRINT) 
        return true;
		
	if(keys == KEY_AIM)
	{
		if(GetPlayerWeapon(playerid) > 10)
		{
			return false;
		}
	}
	
    if(keys == KEY_WALK) 
        return false;

    if(ud == 0 && lr == 0)
        return false;
		
	if(IsPlayerInAnyVehicle(playerid))
		return false;
	if(PlayerCache[playerid][pBuyAttach] > 0)
		return false;

    return true;
}

stock HudText(playerid,text[])
{
	GameTextForPlayer(playerid, text, 5000, 5);
	return 1;
}

public IsPlayerBanned(playerid)
{
	new buffer[256],char_block,ban_reason[64];
	format(buffer,sizeof(buffer),"SELECT block_ban,ban_reason FROM core_players WHERE char_uid=%i",GetPlayerUID(playerid));
	mysql_query(buffer);
	mysql_store_result();
	while(mysql_fetch_row(buffer,"|"))
	{
		sscanf(buffer,"p<|>is[64]",char_block,ban_reason);
		if(char_block == 1)
		{
			mysql_free_result();
			format(buffer,sizeof(buffer),"Twoja postaæ jest zbanowana, powód: {0000FC}%s",ban_reason);
			SendClientMessage(playerid,COLOR_SERVER,buffer);
			SetTimerEx("KickWithMessage", 500, false, "i", playerid);
			printf("[kick] Gracz %s (UID: %i) zostal wyrzucony z serwera - powod: aktywny ban",pInfo[playerid][pName],GetPlayerUID(playerid));
			return 1;
		}
		else
		{
			mysql_free_result();
			return 0;
		}
	}
	return 2;
}

/*public GetPlayerUID(playerid)					//pobieranie UID gracza (NIE SAMPID!!!)
{
	new query[128];
	new pname[MAX_PLAYER_NAME];
	GetPlayerName(playerid,pname,sizeof (pname));
	format(query,sizeof(query),"SELECT char_uid FROM core_players WHERE char_name='%s'",pname);
	mysql_query(query);
	mysql_store_result();
	new uid = mysql_fetch_int();
	mysql_free_result();
	return uid;
}*/

public GetPlayerUID(playerid)
{
	return pInfo[playerid][pUID];
}

stock GetPlayerID(const Name[])
{
    for(new i; i<MAX_PLAYERS; i++)
    {
      if(IsPlayerConnected(i))
      {
        new pname[MAX_PLAYER_NAME];
        GetPlayerName(i, pname, sizeof(pname));
        if(strcmp(Name, pname, true)==0)
        {
          return i;
        }
      }
    }
    return -1;
}

public CheckPassword(playerid,password[])		//sprawdzanie czy gracz wpisal poprawnie haslo
{
	new buffer[256],pname[MAX_PLAYER_NAME];
	GetPlayerName(playerid,pname,sizeof(pname));
	format(buffer,sizeof(buffer),"SELECT char_gid FROM core_players WHERE char_name='%s'",pname);
	mysql_query(buffer);
	mysql_store_result();
	PlayerGID[playerid] = mysql_fetch_int();
	mysql_free_result();
	
	format(buffer,sizeof(buffer),"SELECT members_pass_salt FROM mysa_members WHERE member_id=%i",PlayerGID[playerid] );
	mysql_query(buffer);
	mysql_store_result();
	new memberPassSalt[5];
	mysql_fetch_string(memberPassSalt);
	mysql_free_result();

	new password_e[128];
	mysql_real_escape_string(password,password_e);
	format(buffer,sizeof(buffer),"SELECT name FROM mysa_members WHERE member_id=%i AND members_pass_hash=md5(CONCAT(md5('%s'),md5('%s')))",PlayerGID[playerid] ,memberPassSalt,password_e);
	mysql_query(buffer);
	
	
	mysql_store_result();
	if(mysql_num_rows() == 0)
	{
		TogglePlayerSpectating(playerid,1);
		LogTry[playerid]++;

		mysql_free_result();
		ShowPlayerDialog(playerid,DIAL_L,DIALOG_STYLE_PASSWORD,"Logowanie","Wpisz swoje has³o, aby po³¹czyæ siê z serwerem.\n"COL_RED"* - Ÿle wpisa³eœ has³o, spróbuj jeszcze raz","Gotowe","Wyloguj");
		return 0;
	}
	else
	{
	    mysql_free_result();		
		TogglePlayerSpectating(playerid,0);
		GetPlayerInfo(playerid);
		LogPlayerLogin(playerid,1);
		Logged[playerid] = 1;
		SetSpawnInfo(playerid, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);	
		SpawnPlayer(playerid);
		
		GetPlayerGroups(playerid);
		
		GetPlayerBlocks(playerid);		//do tej pory dziala
		
	    new welcome[256],name[24],surname[24];
	    sscanf(pname,"p<_>s[24]s[24]",name,surname);
	    format(welcome,sizeof(welcome),"Witaj, %s na %s %s. ¯yczymy Ci przyjemnej gry :)",GetPlayerGlobalNickname(playerid),SERVERNAME,VERSION);
		SetSpawnInfo(playerid, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
		SpawnPlayer(playerid);
		//Attach3DTextLabelToPlayer(PlayerNick[playerid], playerid, 0.0, 0.0, 0.2);
	    SendClientMessage(playerid,COLOR_SERVER,welcome);
		new query2[256];
		format(query2,sizeof(query2),"UPDATE core_players SET char_logged=1 WHERE char_uid=%i",GetPlayerUID(playerid));
		mysql_query(query2);
		return 1;
	}
}

stock LogPlayerLogin(playerid,success)
{
	new query[256],ip[16],pname[MAX_PLAYER_NAME],rok,miesiac,dzien,godzina,minuta,sekunda,data[16],czas[16],uwagi[128];
	GetPlayerName(playerid,pname,sizeof(pname));
	GetPlayerIp(playerid,ip,sizeof(ip));
	getdate(rok,miesiac,dzien);
	gettime(godzina,minuta,sekunda);
	format(data,sizeof(data),"%i-%i-%i",rok,miesiac,dzien);
	format(czas,sizeof(czas),"%i:%i:%i",godzina,minuta,sekunda);
	
	if(success == 0)
	{
		uwagi = "Bad password.";
	}
	else
	{
		uwagi = "brak";
	}
	
	format(query,sizeof(query),"INSERT INTO log_logowania VALUES(NULL,%i,%i,'%s','%s','%s','%s')",GetPlayerUID(playerid),success,data,czas,uwagi,ip);
	mysql_query(query);
	return 1;
}

public setGunSkill(playerid)
{
	if(pInfo[playerid][pShootskill] < 20)
	{
		for (new i=0;i < 10; i++)
		{
			SetPlayerSkillLevel(playerid, i, 200);
		}
	}
	else if(pInfo[playerid][pShootskill] < 40)
	{
		for (new i=0;i < 10; i++)
		{
			SetPlayerSkillLevel(playerid, i, 400);
		}
	}
	else if(pInfo[playerid][pShootskill] < 60)
	{
		for (new i=0;i < 10; i++)
		{
			SetPlayerSkillLevel(playerid, i, 600);
		}
	}
	else if(pInfo[playerid][pShootskill] < 80)
	{
		for (new i=0;i < 10; i++)
		{
			SetPlayerSkillLevel(playerid, i, 800);
		}
	}
	else
	{
		for (new i=0;i < 10; i++)
		{
			SetPlayerSkillLevel(playerid, i, 1000);
		}
	}
	
	return 1;
}

public GetPlayerAccess(playerid)
{
	new buffer[256];
	format(buffer,sizeof(buffer),"SELECT `gmaccess` FROM %s WHERE `member_id`=%i",MYBB_TABLE,pInfo[playerid][pGID]);
	mysql_query(buffer);
	mysql_store_result();

	new access = mysql_fetch_int();
	
	mysql_free_result();
	return access;
}

stock GetPlayerGlobalNickname(playerid)
{
	new buffer[256];
	format(buffer,sizeof(buffer),"SELECT `name` FROM %s WHERE `member_id`=%i",MYBB_TABLE,pInfo[playerid][pGID]);
	mysql_query(buffer);
	mysql_store_result();	
	new nickname[128];
	mysql_fetch_string(nickname);
	mysql_free_result();
	return nickname;
}

stock GetPlayerGlobalID(playerid)
{
	new buffer[256];
	new pname[MAX_PLAYER_NAME];
	GetPlayerName(playerid,pname,sizeof(pname));
	format(buffer,sizeof(buffer),"SELECT char_gid FROM core_players WHERE char_name='%s'",pname);
	mysql_query(buffer);
	mysql_store_result();
	new gid = mysql_fetch_int();
	mysql_free_result();
	return gid;
}

/*stock SetVehicleSpeed(vehicleid, Float:Speed)
{
	new	Float:POS[4];
	GetVehicleZAngle(vehicleid, POS[3]);
	Speed = Speed/170;
    POS[0] = Speed * floatsin(-POS[3], degrees);
    POS[1] = Speed * floatcos(-POS[3], degrees);
    SetVehicleVelocity(vehicleid, POS[0], POS[1], POS[2]);
}*/

stock IsPlayerFacingVehicle(playerid, vehicleid)
{
	new Float:plPosX, Float:plPosZ, Float:plPosY, Float:vePosX, Float:vePosY, Float:vePosZ, Float:MainAngle;

	GetVehiclePos(vehicleid, vePosX, vePosY, vePosZ);
	GetPlayerPos(playerid, plPosX, plPosY, plPosZ);

	if( vePosY > plPosY ) MainAngle = (-acos((vePosX - plPosX) / floatsqroot((vePosX - plPosX) * (vePosX - plPosX) + (vePosY - plPosY) * (vePosY - plPosY))) - 90.0);
	else if( vePosY < plPosY && vePosX < plPosX ) MainAngle = (acos((vePosX - plPosX) / floatsqroot((vePosX - plPosX) * (vePosX - plPosX) + (vePosY - plPosY) * (vePosY - plPosY))) - 450.0);
	else if( vePosY < plPosY ) MainAngle = (acos((vePosX - plPosX) / floatsqroot((vePosX - plPosX) * (vePosX - plPosX) + (vePosY - plPosY) * (vePosY - plPosY))) - 90.0);

	if(vePosX > plPosX) MainAngle = (floatabs(floatabs(MainAngle) + 180.0));
	else MainAngle = (floatabs(MainAngle) - 180.0);

	new Float:plAngle;
	GetPlayerFacingAngle(playerid, plAngle);
	
	if(MainAngle - plAngle < -130 || MainAngle - plAngle > 130)
	{
		return 0;
	}
	return 1;
}

//facingi stocki
stock Float:PlayerAngle(playerid)
{
	new Float:Angle;
	if(IsPlayerInAnyVehicle(playerid)) GetVehicleZAngle(GetPlayerVehicleID(playerid), Angle);
	else GetPlayerFacingAngle(playerid, Angle);
	return Angle;
}

stock bool:IsPlayerFacingPoint(playerid, Float:PointX, Float:PointY, Float:Range = 10.0)
{
	new Float:FacingAngle = PlayerAngle(playerid), Float:Angle;
	new Float:X, Float:Y, Float:Z;
	GetPlayerPos(playerid, X, Y, Z);
	if(X > PointX && Y > PointY) Angle = floatabs(atan2(floatsub(PointX, X), floatsub(PointY, Y)));
	if(X > PointX && Y <= PointY) Angle = floatadd(floatabs(atan2(floatsub(Y, PointY), floatsub(PointX, X))), 270.0);
	if(X <= PointX && Y > PointY) Angle = floatadd(floatabs(atan2(floatsub(PointY, Y), floatsub(X, PointX))), 90.0);
	if(X <= PointX && Y <= PointY) Angle = floatadd(floatabs(atan2(floatsub(X, PointX), floatsub(Y, PointY))), 180.0);
	Range /= 2.0;
	return floatabs(floatsub(FacingAngle, Angle)) <= floatabs(Range) || floatabs(floatsub(floatadd(FacingAngle, 360.0), Angle)) <= floatabs(Range);
}

forward bool:IsPlayerFacingPlayer(playerid, faceplayerid, Float:Range);
public bool:IsPlayerFacingPlayer(playerid, faceplayerid, Float:Range)
{
	if(playerid == faceplayerid) return false;
	new Float:X, Float:Y, Float:Z;
	GetPlayerPos(faceplayerid, X, Y, Z);
	return IsPlayerFacingPoint(playerid, X, Y, Range);
}

stock Float:SetPlayerFaceToPoint(playerid, Float:PointX, Float:PointY)
{
	new Float:X, Float:Y, Float:Z;
	new Float:Angle;
	GetPlayerPos(playerid, X, Y, Z);
	if(X > PointX && Y > PointY) Angle = floatabs(atan2(floatsub(PointX, X), floatsub(PointY, Y)));
	if(X > PointX && Y <= PointY) Angle = floatadd(floatabs(atan2(floatsub(Y, PointY), floatsub(PointX, X))), 270.0);
	if(X <= PointX && Y > PointY) Angle = floatadd(floatabs(atan2(floatsub(PointY, Y), floatsub(X, PointX))), 90.0);
	if(X <= PointX && Y <= PointY) Angle = floatadd(floatabs(atan2(floatsub(X, PointX), floatsub(Y, PointY))), 180.0);
	if(GetPlayerState(playerid) == PLAYER_STATE_DRIVER) SetVehicleZAngle(GetPlayerVehicleID(playerid), Angle);
	else SetPlayerFacingAngle(playerid, Angle);
	return Angle;
}

stock Float:SetPlayerFaceFromPoint(playerid, Float:PointX, Float:PointY)
{
	new Float:Angle = floatadd(SetPlayerFaceToPoint(playerid, PointX, PointY), 180.0);
	if(GetPlayerState(playerid) == PLAYER_STATE_DRIVER) SetVehicleZAngle(GetPlayerVehicleID(playerid), Angle);
	else SetPlayerFacingAngle(playerid, Angle);
	return Angle;
}

stock Float:SetPlayerFaceToPlayer(playerid, faceplayerid)
{
	new Float:X, Float:Y, Float:Z;
	GetPlayerPos(faceplayerid, X, Y, Z);
	return SetPlayerFaceToPoint(playerid, X, Y);
}

stock Float:SetPlayerFaceFromPlayer(playerid, faceplayerid)
{
	new Float:X, Float:Y, Float:Z;
	GetPlayerPos(faceplayerid, X, Y, Z);
	return SetPlayerFaceFromPoint(playerid, X, Y);
}

public CheckAFK()
{
	for(new i = 0; i != MAX_PLAYERS; i++)
    {
        AFK[i] ++;
        if(AFK[i] == 3)
        {
			// The player most likely just went AFK!
			PlayerCache[i][pAfk] = true;
			statusPlayer[i] = 1;
        }
    }
	return 1;
}

forward IsPlayerConnectedUID(uid);
public IsPlayerConnectedUID(uid)
{
	new buffer[256];
	format(buffer,sizeof(buffer),"SELECT char_name FROM core_players WHERE char_uid=%i",uid);
	mysql_query(buffer);
	mysql_store_result();
	if(mysql_num_rows() > 0)
	{
		new pname[MAX_PLAYER_NAME];
		mysql_fetch_string(pname);
		mysql_free_result();
		
		for(new i=0; i < MAX_PLAYERS; i ++)
		{
			if(IsPlayerConnect(i))
			{
				if(!strcmp(pInfo[i][pName],pname))
				{
					return i;
				}
			}
		}
	}
	return -1;
}

public QuickSave(playerid)
{
	new Float:x,Float:y,Float:z,query[256];
	GetPlayerPos(playerid,x,y,z);
	
	new vw = GetPlayerVirtualWorld(playerid);
	new interior = GetPlayerInterior(playerid);
	
	format(query,sizeof(query),"UPDATE core_players SET char_posx='%f',char_posy='%f',char_posz='%f',char_world=%i,char_interior=%i WHERE char_uid=%i",x,y,z,vw,interior,GetPlayerUID(playerid));
	mysql_query(query);	
	return 1;
}

stock IsPlayerExist(playerid)
{
	new buffer[256],pname[MAX_PLAYER_NAME];
	GetPlayerName(playerid,pname,sizeof(pname));
	format(buffer,sizeof(buffer),"SELECT * FROM core_players WHERE char_name='%s'",pname);
	//format(PlayerRegister[playerid][pName],64,"%s",pname);
	mysql_query(buffer);
	mysql_store_result();
	if(mysql_num_rows() > 0)
	{
		return 1;
	}
	else
	{
		//ShowPlayerDialog(playerid,DIAL_REGISTER,DIALOG_STYLE_MSGBOX,"Informacja","Postaæ z takim imieniem i nazwiskiem nie istnieje w naszej bazie danych, czy chcesz j¹ zarejestrowaæ?","Tak","Nie");
		return 0;
	}
}

forward testIdlewood(playerid);
public testIdlewood(playerid)
{
	SetPlayerVirtualWorld(playerid,0);
	SetPlayerPos(playerid,SPAWN_IDLE_X,SPAWN_IDLE_Y,SPAWN_IDLE_Z);
}

forward testHomeLocator(playerid);
public testHomeLocator(playerid)
{
	new dooruid = pInfo[playerid][pSpawnType];
	new doorid = -1;
	for(new i = 0 ; i < MAX_DOORS; i++)
	{
		if(DoorInfo[i][doorUID] == dooruid)
		{
			doorid = i;
			break;
		}
	}
	
	SetPlayerPos(playerid,DoorInfo[doorid][doorExitX],DoorInfo[doorid][doorExitY],DoorInfo[doorid][doorExitZ]);
	SetPlayerVirtualWorld(playerid,DoorInfo[doorid][doorExitVW]);
	SetPlayerInterior(playerid,DoorInfo[doorid][doorExitINT]);
}

public CitySpawn(playerid)
{
	new query[256];
	format(query,sizeof(query),"SELECT char_posx FROM core_players WHERE char_uid=%i",GetPlayerUID(playerid));
	mysql_query(query);
	mysql_store_result();
	new Float:result = mysql_fetch_float();
	if(floatround(result) == 0)
	{
		if(pInfo[playerid][pSpawnType] == SPAWN_TYPE_IDLEWOOD)
		{
			if(FirstSpawn[playerid] == 1)
			{
				//HouseSpawnIfExist(playerid);
				SetTimerEx("testIdlewood", 500, false, "i", playerid);
				return 1;
			}
		}
		if(pInfo[playerid][pSpawnType] == SPAWN_TYPE_HOUSE)
		{
			if(FirstSpawn[playerid] == 1)
			{
				SetTimerEx("testHouse", 500, false, "i", playerid);
				return 1;
			}
		}
		if(IsPlayerHouseLocator(playerid,GetDoorIDbyUID(pInfo[playerid][pSpawnType])))
		{
			SetTimerEx("testHomeLocator", 500, false, "i", playerid);
			return 1;
		}
		else
		{
			new Float:myRandomFloat = float(random(20000)/10000);
			SetPlayerPos(playerid,820.849426+myRandomFloat,-1349.170654,13.525882);
		}
	}
	else
	{
		format(query,sizeof(query),"SELECT char_posx, char_posy, char_posz,char_world,char_interior FROM core_players WHERE char_uid=%i",GetPlayerUID(playerid));
		mysql_query(query);
		mysql_store_result();
		new Float:x,Float:y,Float:z;
		new world,interior;
		while(mysql_fetch_row(query,"|"))
		{
			sscanf(query,"p<|>fffii",x,y,z,world);
		}
		//printf("testing one : %f , %f , %f",pInfo[playerid][pX],pInfo[playerid][pY],pInfo[playerid][pZ]);
		SetPlayerPos(playerid,x,y,z);
		SetPlayerVirtualWorld(playerid,world);
		SetPlayerInterior(playerid,interior);
	}
	mysql_free_result();
	return 1;
}

forward testHouse(playerid);
public testHouse(playerid)
{
	HouseSpawnIfExist(playerid);
}

stock getStringDate()
{
	new date[16];
	new Year, Month, Day;
	getdate(Year, Month, Day);
	format(date,sizeof(date),"%i-%i-%i",Year,Month,Day);
	return date;
}

stock getStringTime()
{
	new time[16];
	new Hour, Minute, Second;
	gettime(Hour, Minute, Second);
	format(time,sizeof(time),"%02d:%02d:%02d", Hour, Minute, Second);
	return time;
}

stock HouseSpawnIfExist(playerid)
{
	if(pInfo[playerid][pSpawnType] == 1)
	{
		new ile_wynikow = 0;
		new doorid;
		for(new i = 0 ; i < MAX_DOORS; i++)
		{
			if(DoorInfo[i][doorOwnerType] == 2)
			{
				if(DoorInfo[i][doorOwnerUID] == GetPlayerUID(playerid))
				{
					doorid = i;
					SetPlayerPos(playerid,DoorInfo[doorid][doorExitX],DoorInfo[doorid][doorExitY],DoorInfo[doorid][doorExitZ]);
					SetPlayerVirtualWorld(playerid,DoorInfo[doorid][doorExitVW]);
					SetPlayerInterior(playerid,DoorInfo[doorid][doorExitINT]);
					ile_wynikow++;
					break;
				}
			}
		}
		
		if(ile_wynikow == 0)
		{
			new Float:myRandomFloat = float(random(20000)/10000);
			SetPlayerPos(playerid,820.849426+myRandomFloat,-1349.170654,13.525882);
		}
		
	}
}

public PublicMe(playerid,tresc[])
{
	new send[256],nhhbe[32],e_imie[24],e_nazwisko[24],Float:x,Float:y,Float:z;
	GetPlayerName(playerid,nhhbe,sizeof nhhbe);
	sscanf(nhhbe,"p<_>s[24]s[24]",e_imie,e_nazwisko);
	
	if(pInfo[playerid][pMasked] > 0)
	{
		new maskedMsg[32];
		format(maskedMsg,sizeof(maskedMsg),"Zamaskowany %i",PlayerCache[playerid][pMaskedNumbers]);
		format(send,sizeof send,"** %s %s",maskedMsg,tresc);
	}
	else
	{
		format(send,sizeof send,"* %s %s %s",e_imie,e_nazwisko,tresc);
	}
	
	//format(send,sizeof send,"* %s %s %s",e_imie,e_nazwisko,tresc);
	GetPlayerPos(playerid,x,y,z);
	
	for (new i=0; i <= MAX_PLAYERS+1 ; i++)
	{
		if(GetPlayerVirtualWorld(playerid) == GetPlayerVirtualWorld(i))
		{
			if(IsPlayerInRangeOfPoint(i,10,x,y,z))
			{
				SendClientMessage(i,COLOR_ME,send);
			}
		}
	}
	return 1;
}

stock vlive_err(playerid,tresc[])
{
	new output[256];
	format(output,sizeof(output),""COL_SERV"%s %s:"COL_GRAY" %s",SERVERNAME,VERSION,tresc);
	SendClientMessage(playerid,COLOR_GREY,output);
	return 1;
}

stock kokpit_msg(playerid,tresc[])
{
	new output[256];
	format(output,sizeof(output),""COL_SERV"kokpit :"COL_GRAY" %s",tresc);
	SendClientMessage(playerid,COLOR_GREY,output);
}

public IsPlayerConnect(playerid)							// 1 = po³¹czony 0 = niepo³¹czony
{
	new query[256],pname[MAX_PLAYER_NAME],returnval;
	GetPlayerName(playerid,pname,sizeof(pname));
	format(query,sizeof(query),"SELECT char_uid,char_logged FROM core_players WHERE char_name='%s'",pname);
	mysql_query(query);
	mysql_store_result();
	new resline[128],chuid,chl;
	while(mysql_fetch_row(resline,"|"))
	{
		sscanf(resline,"p<|>ii",chuid,chl);
	}
	mysql_free_result();
	
	if(chl == 1)
	{
		returnval = chl;
	}
	if(chl == 0)
	{
		returnval = chl;
	}
	return returnval;
}

public PlayerNarracja(playerid,tresc[])					//** Coœ siê sta³o siê.
{
	new send[256],Float:x,Float:y,Float:z;
	new imie[32],nazwisko[32];
	new pname[MAX_PLAYER_NAME];
	GetPlayerName(playerid,pname,sizeof(pname));
	sscanf(pname,"p<_>s[32]s[32]",imie,nazwisko);
	format(send,sizeof send,"* %s (( %s %s ))",tresc,imie,nazwisko);
	GetPlayerPos(playerid,x,y,z);
	
	for (new i=0; i <= MAX_PLAYERS+1 ; i++)
	{
		if(IsPlayerInRangeOfPoint(i,10,x,y,z))
		{
		    SendClientMessage(i,COLOR_DO,send);
		}
	}
	return 1;
}

stock NarrateToPlayer(playerid,tresc[])
{
	new output[256];
	format(output,sizeof(output),"* %s",tresc);
	SendCustomPlayerMessage(playerid,COLOR_DO,output);
}

stock mysa_LogMoney(playerid,amount,type,giver)
{
	new buffer[256];
	format(buffer,sizeof(buffer),"INSERT INTO log_money VALUES(NULL,%i,%i,%i,%i,%i,'%s','%s')",GetPlayerUID(playerid),pInfo[playerid][pMoney],pInfo[playerid][pMoney] + amount,type,giver,datelog(),timelog());
	mysql_query(buffer);
}

public GetPlayerIDbyUID(uid)
{
	new buffer[256];
	format(buffer,sizeof(buffer),"SELECT char_name FROM core_players WHERE char_uid=%i",uid);
	mysql_query(buffer);
	mysql_store_result();
	new nick[MAX_PLAYER_NAME];
	mysql_fetch_string(nick);
	new playerid = GetPlayerID(nick);
	mysql_free_result();
	return playerid;
}

stock CleanPlayerTable(playerid)
{
	pInfo[playerid][pUID] = EOS;
	pInfo[playerid][pSID] = EOS;
	pInfo[playerid][pName][0] = EOS;
	pInfo[playerid][pGID] = EOS;
	pInfo[playerid][pLogged] = EOS;
	pInfo[playerid][pMoney] = EOS;	
	pInfo[playerid][pPhoneInUse] = EOS;
	pInfo[playerid][pMasked] = EOS;
}

public CheckPlayerLogged(playerid)
{
	new buffer[256],pname[MAX_PLAYER_NAME];
	GetPlayerName(playerid,pname,sizeof(pname));
	format(buffer,sizeof(buffer),"SELECT char_logged FROM core_players WHERE char_name='%s'",pname);
	mysql_query(buffer);
	mysql_store_result();
	new result = mysql_fetch_int();
	mysql_free_result();
	
	if (result > 0)
	{
		Kick(playerid);
	}
}

stock timelog()
{
	new godzina,minuta,sekunda,czas[16];
	gettime(godzina,minuta,sekunda);
	format(czas,sizeof(czas),"%i:%i:%i",godzina,minuta,sekunda);
	
	return czas;
}

stock datelog()
{
	new rok,miesiac,dzien,data[16];
	getdate(rok,miesiac,dzien);
	format(data,sizeof(data),"%i-%i-%i",rok,miesiac,dzien);
	return data;
}

stock ShowPlayerContacts(playerid)
{
	if(GetPlayerPhoneInUse(playerid) == 2)
	{
		GuiInfo(playerid,"Wykryto b³¹d w systemie przedmiotów, musisz zg³osiæ siê do administratora. \n Error CODE : 7374");
		return 1;
	}
	new buffer[256];
	format(buffer,sizeof(buffer),"SELECT c_number,c_nazwa FROM tel_contacts WHERE numer=%i",pInfo[playerid][pPhoneInUse]);
	mysql_query(buffer);
	mysql_store_result();
	if(mysql_num_rows() > 0)
	{
		new kontakty[512];
		while(mysql_fetch_row(buffer,"|"))
		{
			new numer,nazwa[32];
			sscanf(buffer,"p<|>is[32]",numer,nazwa);
			format(kontakty,sizeof(kontakty),"%s\n%i\t\t%s",kontakty,numer,nazwa);
		}
		mysql_free_result();
		ShowPlayerDialog(playerid,DIAL_CONTACTS,DIALOG_STYLE_LIST,"Telefon » kontakty",kontakty,"Wybierz","Zamknij");
	}
	else
	{
		GuiInfo(playerid,"Nie posiadasz kontaktów w tym telefonie.");
	}
	return 1;
}

stock IsPlayerInRangeOfPlayer(one,two,Float:odleglosc)
{
	new Float:x,Float:y,Float:z;
	GetPlayerPos(one,x,y,z);
	new onevw = GetPlayerVirtualWorld(one);
	
	if(onevw == GetPlayerVirtualWorld(two))
	{
		if(IsPlayerInRangeOfPoint(two,odleglosc,x,y,z))
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
	else
	{
		return 0;
	}
}

stock GetPlayerPhoneInUse(playerid)
{
	new buffer[256];
	format(buffer,sizeof(buffer),"SELECT item_value1 FROM core_items WHERE item_owneruid=%i AND item_whereis = 0 AND item_type=%i AND item_value2=1",GetPlayerUID(playerid),ITEM_PHONE);
	mysql_query(buffer);
	mysql_store_result();
	new ile = mysql_num_rows();
	if(ile > 1)
	{
		GuiInfo(playerid,"Wykryto b³¹d w systemie przedmiotów, musisz zg³osiæ siê do administratora. \n Error CODE : 7374");
		return 2;
	}
	if(ile == 1)
	{
		new pnumber = mysql_fetch_int();
		mysql_free_result();
		pInfo[playerid][pPhoneInUse] = pnumber;
		return 1;
	}
	return 0;
}

stock TalkAnimation(playerid)
{
	if(pInfo[playerid][pCuffed] > 0 || pInfo[playerid][pBw] > 0 || PlayerCache[playerid][pPlayAnim] == true)
	{
	    return 1;
	}
 	ApplyAnimation(playerid, "PED", "IDLE_CHAT", 4.1, 0, 1, 1, 1, 1, 1);
  	SetTimerEx("KillAnimation", 3000, 0, "d", playerid);
	return 1;
}

forward KillAnimation(playerid);
public KillAnimation(playerid)
{
	PlayerCache[playerid][pPlayAnim] = false;
	ApplyAnimation(playerid, "CARRY", "crry_prtial", 4.1, 0, 0, 0, 0, 0);
	//SetTimerEx("KillAnimation2", 3000, 0, "d", playerid);
}

forward KillAnimation2(playerid);
public KillAnimation2(playerid)
{
	PlayerCache[playerid][pPlayAnim] = false;
	ApplyAnimation(playerid, "CARRY", "crry_prtial", 4.1, 0, 0, 0, 0, 0);
}

forward IsPlayerBW(playerid);
public IsPlayerBW(playerid)
{
	if(PlayerBwTimer[playerid] > 0)
	{	
		PlayerBwTimer[playerid]--;
		
		SetTimerEx("IsPlayerBW", 1000, false, "i", playerid);
		
		pInfo[playerid][pBw] = 1;
		
		pInfo[playerid][pHealth ] = 100;
		
		SetPlayerHealth(playerid,100);
		
		new output[128];
		new timeleft = PlayerBwTimer[playerid] / 60;
		format(output,sizeof(output),"Pozostaly czas bw~r~ %i min",timeleft);
		
		GameTextForPlayer(playerid, output, 1000, 5);
	}
	else
	{
		TogglePlayerControllable(playerid, 1);
		pInfo[playerid][pBw] = EOS;
		PlayerBwTimer[playerid] = EOS;
		SetCameraBehindPlayer(playerid);
		
		statusPlayer[playerid] = 1;
		
		pInfo[playerid][pHealth ] = 14;
		
		SetPlayerHealth(playerid,pInfo[playerid][pHealth]);
		new buffer[256];
		format(buffer,sizeof(buffer),"UPDATE core_players SET char_bwtime=0 WHERE char_uid=%i",GetPlayerUID(playerid));
		mysql_query(buffer);
		SetPlayerSkin(playerid,pInfo[playerid][pSkin]);
		SetTimerEx("testSkin", 1000, false, "i", playerid);
	}
	return 1;
}

forward testSkin(playerid);
public testSkin(playerid)
{
	SetPlayerSkin(playerid,pInfo[playerid][pSkin]);
}

forward SetPlayerBW(playerid);
public SetPlayerBW(playerid)
{
	SetPlayerPos(playerid,PlayerCache[playerid][pDeadX],PlayerCache[playerid][pDeadY],PlayerCache[playerid][pDeadZ]);		
	SetPlayerSkin(playerid,pInfo[playerid][pSkin]);
	//SetPlayerInterior(playerid, PlayerCache[playerid][pInt]);
	//SetPlayerVirtualWorld(playerid, PlayerCache[playerid][pVW]);			
	SetPlayerCameraPos(playerid, PlayerCache[playerid][pCX]+3, PlayerCache[playerid][pCY]+4, PlayerCache[playerid][pCZ]+7);
	SetPlayerCameraLookAt(playerid, PlayerCache[playerid][pCX],PlayerCache[playerid][pCY],PlayerCache[playerid][pCZ]);
	TogglePlayerControllable(playerid, 0);
	pInfo[playerid][pHealth] = 14;
	SetPlayerHealth(playerid,14);
	ApplyAnimation(playerid, "CRACK", "crckidle2", 4, 0, 0, 1, 1, 0, 0);
	SetTimerEx("SetPlayerBWAnim", 2000, false, "i", playerid);
	pInfo[playerid][pBw] = 1;
	//PreloadAnimLib(playerid, "CRACK");
}

stock CheckPlayerBW(playerid)
{
	new buffer[256];
	format(buffer,sizeof(buffer),"SELECT char_bwtime,char_posx,char_posy,char_posz FROM core_players WHERE char_uid=%i",GetPlayerUID(playerid));
	mysql_query(buffer);
	mysql_store_result();
	new bwtime;
	while (mysql_fetch_row(buffer,"|"))
	{
		sscanf(buffer,"p<|>ifff",bwtime,PlayerCache[playerid][pCX],PlayerCache[playerid][pCY],PlayerCache[playerid][pCZ]);
	}
	mysql_free_result();
	if(bwtime > 0)
	{
		PlayerBwTimer[playerid] = bwtime*60;
		SetTimerEx("IsPlayerBW", 1000, false, "i", playerid);
		//SetPlayerBW(playerid);
		SetPlayerPos(playerid,PlayerCache[playerid][pCX],PlayerCache[playerid][pCY],PlayerCache[playerid][pCZ]);		
		SetPlayerSkin(playerid,pInfo[playerid][pSkin]);
		//SetPlayerInterior(playerid, PlayerCache[playerid][pInt]);
		//SetPlayerVirtualWorld(playerid, PlayerCache[playerid][pVW]);			
		SetPlayerCameraPos(playerid, PlayerCache[playerid][pCX] + 3,PlayerCache[playerid][pCY] + 4,PlayerCache[playerid][pCZ] + 7);
		SetPlayerCameraLookAt(playerid, PlayerCache[playerid][pCX],PlayerCache[playerid][pCY],PlayerCache[playerid][pCZ]);
		TogglePlayerControllable(playerid, 0);
		pInfo[playerid][pHealth] = 100;
		SetPlayerHealth(playerid,100);
		ApplyAnimation(playerid, "CRACK", "crckidle2", 4, 0, 0, 1, 1, 0, 0);
		SetTimerEx("SetPlayerBWAnim", 2000, false, "i", playerid);
		pInfo[playerid][pBw] = 1;
		statusPlayer[playerid] = 1;
		//PreloadAnimLib(playerid, "CRACK");
	}
}

stock LoadBankomats()
{
	printf("[bankomaty] Rozpoczynam wczytywanie bankomatów...");
	new buffer[256];
	format(buffer,sizeof(buffer),"SELECT * FROM bankomaty");
	mysql_query(buffer);
	mysql_store_result();
	new num = 0;
	while(mysql_fetch_row(buffer,"|"))
	{
		sscanf(buffer,"p<|>ifffi",
		bankomat[num][bkUID],
		bankomat[num][bkX],
		bankomat[num][bkY],
		bankomat[num][bkZ],
		bankomat[num][bkVW]);
		num++;
	}
	mysql_free_result();
	
	printf("[bankomaty] Pomyœlnie za³adowano %i maszyn",num);
}

stock NewBankomatID()
{
	new id;
	for(new i = 0 ; i < MAX_BANKOMATS ; i++)
	{
		if(bankomat[i][bkUID] != 0)
		{
		
		}
		else
		{
			id = i;
			break;
		}
	}
	return id;
}

forward SetPlayerBWAnim(playerid);
public SetPlayerBWAnim(playerid)
{
	ApplyAnimation(playerid, "CRACK", "crckidle2", 4, 0, 0, 1, 1, 0, 0);
}

stock ApplyPhoneAnim(playerid)
{
	if(pInfo[playerid][pBw] == 0)
	{
		if(pInfo[playerid][pCuffed] == 0)
		{
			SetPlayerSpecialAction(playerid,SPECIAL_ACTION_USECELLPHONE);
			SetPlayerAttachedObject(playerid, SLOT_PHONE, 330, 6, 0.013744 ,0.007423 ,-0.010019 ,-2.759115, 8.353719, 9.100319 ,1.000000 ,1.000000 ,1.000000);
		}
	}
}

public LoadAnims()
{
	new data[128], anim_id;
	mysql_query("SELECT * FROM `vlive_anim`");
	
	print("[load] Rozpoczynam proces wczytywania wszystkich animacji z bazy...");
	
	mysql_store_result();
	while(mysql_fetch_row(data, "|"))
	{
		anim_id ++;
		
		sscanf(data, "p<|>ds[12]s[16]s[24]fdddddd",
		AnimInfo[anim_id][aUID],
		AnimInfo[anim_id][aCommand],
		AnimInfo[anim_id][aLib],
		AnimInfo[anim_id][aName],
		AnimInfo[anim_id][aSpeed],
		AnimInfo[anim_id][aOpt1],
		AnimInfo[anim_id][aOpt2],
		AnimInfo[anim_id][aOpt3],
		AnimInfo[anim_id][aOpt4],
		AnimInfo[anim_id][aOpt5],
		AnimInfo[anim_id][aAction]);
	}
	mysql_free_result();
	
	printf("[load] Wczytano %d animacji/e.", anim_id);
	return 1;
}

forward CountPlayerSecondsInGame(playerid);
public CountPlayerSecondsInGame(playerid)
{
	if(IsPlayerConnect(playerid))
	{
		if(PlayerCache[playerid][pAfk] == false)
		{
			new buffer[256];
			format(buffer,sizeof(buffer),"UPDATE core_players SET char_time=char_time+60  WHERE char_uid=%i",GetPlayerUID(playerid));
			mysql_query(buffer);
			
			PlayerCache[playerid][pPizzaTicks] = 0;
		}
		
		setGunSkill(playerid);
		
		SetTimerEx("CountPlayerSecondsInGame", 60000, false, "i", playerid);
	}
}

stock LoadPetrols()
{
	printf("[petrols] Rozpoczynam wczytywanie stacji benzynowych...");
	new buffer[256];
	format(buffer,sizeof(buffer),"SELECT * FROM petrolstation");
	mysql_query(buffer);
	mysql_store_result();
	new num = 0;
	while(mysql_fetch_row(buffer,"|"))
	{
		sscanf(buffer,"p<|>ifffi",
		petrol[num][pUID],
		petrol[num][peX],
		petrol[num][peY],
		petrol[num][peZ],
		petrol[num][peVW]);
		num++;
	}
	mysql_free_result();
	printf("[petrols] Wczytano %i stacji benzynowych",num);
}

stock CheckFreePetrolSlot()
{
	new id;
	for(new i= 0; i < MAX_PETROLS; i ++)
	{
		if(petrol[i][pUID] > 0) { }
		else
		{
			id = i;
			break;
		}
	}
	return id;
}

stock PreloadAnimLib(playerid, animlib[])
{
	ApplyAnimation(playerid, animlib, "null", 0.0, 0, 0, 0, 0, 0);
	return 1;
}

forward IsPlayerLogged(playerid);
public IsPlayerLogged(playerid)
{
	if(Logged[playerid] != 0)
	{
		//zalogowa³ siê, wiêc nic...
	}
	else
	{
		//nie zalogowal sie
		printf("[kick] Gracz %s (UID: %i) zostal wyrzucony z serwera - 30 sek bez zalogowania",pInfo[playerid][pName],GetPlayerUID(playerid));
		Kick(playerid);
	}
	return 0;
}

stock GetXYInFrontOfPlayer(playerid, &Float:x, &Float:y, Float:distance) //by Y_Less
{
 	new Float:a;
 	GetPlayerPos(playerid, x, y, a);
 	GetPlayerFacingAngle(playerid, a);
 	
 	x += (distance * floatsin(-a, degrees));
 	y += (distance * floatcos(-a, degrees));
}

stock bankrandom(target)
{
	losowanie:
	new nr1 = random(10);
	new nr2 = random(10);
	new nr3 = random(10);
	new nr4 = random(10);
	new nr5 = random(10);
	new nr6 = random(10);
	new nr7 = random(10);
	new nr8 = random(10);
	new nr9 = random(10);
	new nr10 = random(10);
	new bank[11];
	format(bank,sizeof(bank),"%i%i%i%i%i%i%i%i%i%i",nr1,nr2,nr3,nr4,nr5,nr6,nr7,nr8,nr9,nr10);
	new bank_int = strval(bank);
	if(bank_int <= 0)
	{
		goto losowanie;
	}
	new buffer[256];
	format(buffer,sizeof(buffer),"SELECT * FROM core_players WHERE char_banknr=%i",bank_int);
	mysql_query(buffer);
	mysql_store_result();
	if(mysql_num_rows() > 0)
	{
		mysql_free_result();
		goto losowanie;
	}
	else
	{
		mysql_free_result();
		format(buffer,sizeof(buffer),"UPDATE core_players SET char_banknr=%i WHERE char_uid=%i",bank_int,GetPlayerUID(target));
		mysql_query(buffer);
		//GuiInfo(playerid,"Zmieni³eœ numer konta bankowego graczowi.");
		pInfo[target][pBankNR] = bank_int;
	}
}


//
//
//
//
COMMAND:tog(playerid,params[])
{
	new id;
	if(sscanf(params,"d",id))
	{
		vlive_err(playerid,"/tog [1 - wiadomosci | 2 - czat ooc grupy]");
		return 1;
	}
	switch(id)
	{
		case 1:
		{
			if(pInfo[playerid][pTogW] > 0)
			{
				pInfo[playerid][pTogW] = 0;
				GuiInfo(playerid,"W³¹czy³eœ prywatne wiadomoœci.");
			}
			else
			{
				pInfo[playerid][pTogW] = 1;
				GuiInfo(playerid,"Wy³¹czy³eœ prywatne wiadomoœci.");
			}
		}
	}
	return 1;
}

COMMAND:w(playerid,params[])
{
	new receiver,text[256];
	if(sscanf(params,"ds[256]",receiver,text))
	{
		vlive_err(playerid,"/w [playerid] [wiadomosc] - wysy³a prywatn¹ wiadomoœæ");
		return 1;
	}
	
	if(pInfo[playerid][blockOOC] > 0)
	{
		GuiInfo(playerid,"Posiadasz aktywn¹ blokadê czatu OOC.");
		return 1;
	}
	
	if(IsPlayerConnected(receiver))
	{
		if(pInfo[receiver][pTogW] != 0)
		{
			vlive_err(playerid,"ten gracz ma wy³¹czone wiadomoœci prywatne");
			return 1;
		}
		else	
		{
			if(playerid == receiver)
			{
				vlive_err(playerid,"no chyba masz kolegów, czy serio chcesz rozmawiaæ sam ze sob¹?");
				return 1;
			}
			
			if(PlayerCache[receiver][pAfk] == true)
			{
				HudText(playerid,"Ten gracz jest AFK");
			}
			
			new sending[256];
			text[0] = toupper(text[0]);
			//do adresata
			if(pInfo[playerid][pAdmin] > 0)
			{
				//jest wysylaj¹cy jest adminem
				new pname[MAX_PLAYER_NAME];
				GetPlayerName(playerid,pname,sizeof(pname));
				format(sending,sizeof(sending),"(( %s [%i]: %s ))",GetPlayerGlobalNickname(playerid),playerid,text);
				SendCustomPlayerMessage(receiver,COLOR_GOT_PW,sending);	//receiver
				
				new buffer[256];
				format(buffer,sizeof(buffer),"INSERT INTO log_privates VALUES(NULL,'%s',%i,%i,'%s','%s')",sending,GetPlayerUID(playerid),GetPlayerUID(receiver),datelog(),timelog());
				mysql_query(buffer);
				
				//do wysylajacego
				GetPlayerName(receiver,pname,sizeof(pname));
				format(sending,sizeof(sending),"(( » %s [%i]: %s ))",pname,receiver,text);
				SendCustomPlayerMessage(playerid,COLOR_SEND_PW,sending);	//sender
			}
			else
			{
				if(pInfo[receiver][pAdmin] > 0)
				{
					//jest wysylaj¹cy jest adminem
					new pname[MAX_PLAYER_NAME];
					GetPlayerName(playerid,pname,sizeof(pname));
					format(sending,sizeof(sending),"(( %s [%i]: %s ))",pname,playerid,text);
					SendCustomPlayerMessage(receiver,COLOR_GOT_PW,sending);
					
					new buffer[256];
					format(buffer,sizeof(buffer),"INSERT INTO log_privates VALUES(NULL,'%s',%i,%i,'%s','%s')",sending,GetPlayerUID(playerid),GetPlayerUID(receiver),datelog(),timelog());
					mysql_query(buffer);
					
					//do wysylajacego
					GetPlayerName(receiver,pname,sizeof(pname));
					format(sending,sizeof(sending),"(( » %s [%i]: %s ))",GetPlayerGlobalNickname(receiver),receiver,text);
					SendCustomPlayerMessage(playerid,COLOR_SEND_PW,sending);
				}
				else
				{
					new pname[MAX_PLAYER_NAME],imie[32],nazwisko[32];
					GetPlayerName(playerid,pname,sizeof(pname));
					sscanf(pname,"p<_>s[32]s[32]",imie,nazwisko);
					format(sending,sizeof(sending),"(( %s %s [%i]: %s ))",imie,nazwisko,playerid,text);
					SendCustomPlayerMessage(receiver,COLOR_GOT_PW,sending);
					
					new buffer[256];
					format(buffer,sizeof(buffer),"INSERT INTO log_privates VALUES(NULL,'%s',%i,%i,'%s','%s')",sending,GetPlayerUID(playerid),GetPlayerUID(receiver),datelog(),timelog());
					mysql_query(buffer);
					
					//do wysylajacego
					GetPlayerName(receiver,pname,sizeof(pname));
					sscanf(pname,"p<_>s[32]s[32]",imie,nazwisko);
					format(sending,sizeof(sending),"(( » %s %s [%i]: %s ))",imie,nazwisko,receiver,text);
					SendCustomPlayerMessage(playerid,COLOR_SEND_PW,sending);
				}
			}
			
			return 1;
		}
	}
	else
	{
		vlive_err(playerid,"ten gracz nie jest pod³¹czony");
	}
	return 1;
}

CMD:k(playerid,params[])
{
	if(pInfo[playerid][pBw] > 0)
	{
		return 1;
	}
	new text[256];
	if(sscanf(params,"s[256]",text))
	{
		vlive_err(playerid,"/k [wiadomosc] - krzyk");
		return 1;
	}
	new Float:x,Float:y,Float:z;
	GetPlayerPos(playerid,x,y,z);
	text[0] = toupper(text[0]);
	new sendervw = GetPlayerVirtualWorld(playerid);
	new imie[32],nazwisko[32];
	sscanf(pInfo[playerid][pName],"p<_>s[32]s[32]",imie,nazwisko);
	
	if(pInfo[playerid][pMasked] > 0)
	{
		new maskedMsg[32];
		format(maskedMsg,sizeof(maskedMsg),"Zamaskowany %i",PlayerCache[playerid][pMaskedNumbers]);
		format(text,sizeof(text),"%s krzyczy: %s",maskedMsg,text);
	}
	else
	{
		format(text,sizeof(text),"%s %s krzyczy: %s",imie,nazwisko,text);
	}
	
	for(new i = 0 ; i < MAX_PLAYERS; i++)
	{
		if(IsPlayerInRangeOfPoint(i,10,x,y,z))
		{	
			new gainervw = GetPlayerVirtualWorld(i);
			if(sendervw == gainervw)
			{
				SendCustomPlayerMessage(i, COLOR_1m, text);
			}
		}
		else if(IsPlayerInRangeOfPoint(i,15,x,y,z))
		{	
			new gainervw = GetPlayerVirtualWorld(i);
			if(sendervw == gainervw)
			{
				SendCustomPlayerMessage(i, COLOR_2m, text);
			}
		}
		else if(IsPlayerInRangeOfPoint(i,20,x,y,z))
		{	
			new gainervw = GetPlayerVirtualWorld(i);
			if(sendervw == gainervw)
			{
				SendCustomPlayerMessage(i, COLOR_4m, text);
			}
		}
		else if(IsPlayerInRangeOfPoint(i,25,x,y,z))
		{	
			new gainervw = GetPlayerVirtualWorld(i);
			if(sendervw == gainervw)
			{
				SendCustomPlayerMessage(i, COLOR_9m, text);
			}
		}
	}
	return 1;
}

CMD:m(playerid,params[])
{
	if(IsPlayerHasMegafon(playerid))
	{
		new output[256];
		if(sscanf(params,"s[256]",output))
		{
			vlive_err(playerid,"/m [wiadomosc]");
			return 1;
		}
		output[0] = toupper(output[0]);
		new sending[256];
		new pimie[32],pnazwisko[32];
		sscanf(pInfo[playerid][pName],"p<_>s[32]s[32]",pimie,pnazwisko);
		format(sending,sizeof(sending),"%s %s (megafon): %s",pimie,pnazwisko,output);
		new Float:x,Float:y,Float:z;
		GetPlayerPos(playerid,x,y,z);
		new sendervw = GetPlayerVirtualWorld(playerid);
		for(new i=0;i<MAX_PLAYERS;i++)
		{
			new gainervw = GetPlayerVirtualWorld(i);
			if(gainervw == sendervw)
			{
				if(IsPlayerInRangeOfPoint(i,50,x,y,z))
				{
					SendCustomPlayerMessage(i, COLOR_MEGAFON, sending);
				}
			}
		}
	}
	else
	{
		GuiInfo(playerid,"Nie posiadasz megafonu.");
	}
	return 1;
}

CMD:s(playerid,params[])
{
	if(pInfo[playerid][pBw] > 0)
	{
		return 1;
	}
	new text[256];
	if(sscanf(params,"s[256]",text))
	{
		vlive_err(playerid,"/s [wiadomosc] - szept");
		return 1;
	}
	new Float:x,Float:y,Float:z;
	GetPlayerPos(playerid,x,y,z);
	new sendervw = GetPlayerVirtualWorld(playerid);
	text[0] = toupper(text[0]);
	new imie[32],nazwisko[32];
	sscanf(pInfo[playerid][pName],"p<_>s[32]s[32]",imie,nazwisko);
	format(text,sizeof(text),"%s %s szepcze: %s",imie,nazwisko,text);
	for(new i = 0 ; i < MAX_PLAYERS; i++)
	{
		if(IsPlayerInRangeOfPoint(i,1,x,y,z))
		{	
			new gainervw = GetPlayerVirtualWorld(i);
			if(sendervw == gainervw)
			{
				SendCustomPlayerMessage(i, COLOR_1m, text);
			}
		}
		else if(IsPlayerInRangeOfPoint(i,2,x,y,z))
		{	
			new gainervw = GetPlayerVirtualWorld(i);
			if(sendervw == gainervw)
			{
				SendCustomPlayerMessage(i, COLOR_2m, text);
			}
		}
		else if(IsPlayerInRangeOfPoint(i,3,x,y,z))
		{	
			new gainervw = GetPlayerVirtualWorld(i);
			if(sendervw == gainervw)
			{
				SendCustomPlayerMessage(i, COLOR_4m, text);
			}
		}
		else if(IsPlayerInRangeOfPoint(i,4,x,y,z))
		{	
			new gainervw = GetPlayerVirtualWorld(i);
			if(sendervw == gainervw)
			{
				SendCustomPlayerMessage(i, COLOR_9m, text);
			}
		}
	}
	return 1;
}

CMD:phone(playerid,params[])
{
	ApplyPhoneAnim(playerid);
	return 1;
}

CMD:unphone(playerid,params[])
{
	DeletePhoneAnim(playerid);
	return 1;
}

CMD:ephone(playerid,params[])
{
	EditAttachedObject(playerid, SLOT_PHONE);
	return 1;
}

public OnPlayerEditAttachedObject(playerid, response, index, modelid, boneid, Float:fOffsetX, Float:fOffsetY, Float:fOffsetZ, Float:fRotX, Float:fRotY, Float:fRotZ, Float:fScaleX, Float:fScaleY, Float:fScaleZ)
{
	if(response)
	{
		new buffer[256];
		// PlayerCache[playerid][pSelectedAttachment]
		format(buffer,sizeof(buffer),"UPDATE core_attachments SET posx = %f, posy=%f, posz=%f,rotx = %f, roty = %f, rotz = %f, scalex = %f, scaley = %f, scalez = %f WHERE item_uid = %i",fOffsetX,fOffsetY,fOffsetZ,fRotX,fRotY,fRotZ,fScaleX,fScaleY,fScaleZ,PlayerCache[playerid][pSelectedAttachment]);
		mysql_query(buffer);
		
		switch(PlayerCache[playerid][pEditAttachment])
		{
			case 0:
			{
				RemovePlayerAttachedObject(playerid,SLOT_PLAYERONE);
			}
			case 1:
			{
				RemovePlayerAttachedObject(playerid,SLOT_PLAYERTWO);
			}
		}
		GuiInfo(playerid,"Zapisa³eœ pozycjê obiektu.");
		PlayerCache[playerid][pEditAttachment] = 0;
	}
	else
	{
		switch(PlayerCache[playerid][pEditAttachment])
		{
			case 0:
			{
				RemovePlayerAttachedObject(playerid,SLOT_PLAYERONE);
			}
			case 1:
			{
				RemovePlayerAttachedObject(playerid,SLOT_PLAYERTWO);
			}
		}
		GuiInfo(playerid,"Anulowa³eœ edycjê obiektu.");
		PlayerCache[playerid][pEditAttachment] = 0;
	}
	return 1;
}

stock DeletePhoneAnim(playerid)
{
	if(pInfo[playerid][pBw] == 0)
	{
		if(pInfo[playerid][pCuffed] == 0)
		{
			SetPlayerSpecialAction(playerid,SPECIAL_ACTION_STOPUSECELLPHONE);
			RemovePlayerAttachedObject(playerid, SLOT_PHONE);
		}
	}
}

//
//
// -- COMMANDS -- //
//
//

COMMAND:v(playerid,params[])
{
	if(GetPlayerHours(playerid) > 3)
	{
		if(!IsPlayerHasStartVehicle(playerid))
		{
			if(PlayerCache[playerid][pSelStarted] == 0)
			{
				ShowPlayerDialog(playerid,DIAL_SEL_STARTED,DIALOG_STYLE_MSGBOX,"Informacja","Nie posiadasz pojazdu startowego, czy chcesz go teraz wybraæ?","Tak","Nie");
			}
		}
	}
	
	new param[32];
	if(sscanf(params,"s[32]",param)) return SendClientMessage(playerid,COLOR_GREY,"U¿ycie komendy : /v (lista, zamknij, tuning, sprzedaj, zaparkuj, info, namierz)");
	
	
	if(!strcmp(param,"lista"))
    {
		ShowPlayerVehicles(playerid);
    }
	if(!strcmp(param,"namierz"))
	{
		NavigateVehicle(playerid);
	}
	if(!strcmp(param,"zamknij"))
    {
		CloseNearVehicle(playerid);
    }
	if(!strcmp(param,"z"))
    {
		CloseNearVehicle(playerid);
    }
	if(!strcmp(param,"tuning"))
    {
		ShowVehicleUpgrades(playerid);
    }
	if(!strcmp(param,"sprzedaj"))
    {
		GuiInfo(playerid,"/o pojazd");
    }
	if(!strcmp(param,"zaparkuj"))
    {
		VehiclePark(playerid);
    }
	if(!strcmp(param,"info"))
    {
		GetOccupiedVehicleInfo(playerid);
    }
	
	return 1;
}

COMMAND:info(playerid,params[])
{
	new Float:running = RunningTime/60/60;
	new output[128];
	format(output,sizeof(output),"%s %s\nUptime: %i h\nDebug mode: %i",SERVERNAME,VERSION,floatround(running),debugMode);
	GuiInfo(playerid,output);
	return 1;
}

COMMAND:p(playerid,cmdtext[])
{
	if(pInfo[playerid][pBw] > 0)
	{
		vlive_err(playerid,"nie mo¿esz u¿ywaæ ekwipunku podczas nieprzytomnoœci");
		return 1;
	}
	if (strcmp(cmdtext,"podnies",true) == 0)
	{
		//print("podnoszenie");
		if(IsPlayerInAnyVehicle(playerid))
		{
			new vehicleid = GetPlayerVehicleID(playerid);
			
			if(IsPlayerPermVehicle(playerid,vehicleid))
			{
				if(!ListVehicleItems(playerid,vehicleid))
				{
					GuiInfo(playerid,"Nie ma przedmiotów w schowku tego pojazdu.");
				}
			}
			else
			{
				GuiInfo(playerid,"Brak uprawnieñ do przegl¹dania schowka w tym pojeŸdzie.");
			}
			
			return 1;
		}
		PickupNearbyItems(playerid);
	}
	else
	{
		GetPlayerItems(playerid);
	}
	return 1;
}

COMMAND:me(playerid,params[])
{
	new doo[128];
	if(sscanf(params,"s[128]",doo))
	{
		vlive_err(playerid,"/me [sytuacja]");
		return 1;
	}
	new tresc[256],nhhbe[32],e_imie[24],e_nazwisko[24],Float:x,Float:y,Float:z;
	GetPlayerName(playerid,nhhbe,sizeof nhhbe);
	sscanf(nhhbe,"p<_>s[24]s[24]",e_imie,e_nazwisko);
	
	if(pInfo[playerid][pMasked] > 0)
	{
		new maskedMsg[32];
		format(maskedMsg,sizeof(maskedMsg),"Zamaskowany %i",PlayerCache[playerid][pMaskedNumbers]);
		format(tresc,sizeof tresc,"** %s %s",maskedMsg,doo);
	}
	else
	{
		format(tresc,sizeof tresc,"** %s %s %s",e_imie,e_nazwisko,doo);
	}
	
	GetPlayerPos(playerid,x,y,z);
	
	new sendervw = GetPlayerVirtualWorld(playerid);
	
	for (new i=0; i <= MAX_PLAYERS+1 ; i++)
	{
		if(IsPlayerInRangeOfPoint(i,10,x,y,z))
		{
			new gainervw = GetPlayerVirtualWorld(i);
			if(sendervw == gainervw)
			{
				//SendClientMessage(i,COLOR_ME,tresc);
				SendCustomPlayerMessage(i, COLOR_ME, tresc);
			}
		}
	}
	return 1;
}

stock SendCustomPlayerMessage(playerid, color, text[])
{
	new string[256];
    if(strlen(text) > 64)
    {
        new text1[65],
            text2[65];
            
        strmid(text2, text, 64, 128);
        strmid(text1, text, 0, 64);
                
        format(string, 256, "%s...", text1);
        SendClientMessage(playerid, color, string);
        
        format(string, 256, "...%s", text2);
        SendClientMessage(playerid, color, string);
    }
    else SendClientMessage(playerid, color, text);
}

stock FormatPlayerDescription(text[])
{
	new string[120];
	if(strlen(text) > 40)
	{
		new text1[40];
		new text2[80];
		strmid(text1, text, 0, 40);
		strmid(text2, text, 41, 120);
		
		if(strlen(text2) > 40)
		{
			new text3[40];
			new text4[40];
			strmid(text3,text2,0,40);
			strmid(text4,text2,41,80);
			
			format(string,120,"%s\n%s\n%s",text1,text3,text4);
			return string;
		}
		else
		{
			format(string,120,"%s\n%s",text1,text2);
			return string;
		}
		
	}
	return string;
}


COMMAND:do(playerid,params[])
{
	new doo[128];
	if(sscanf(params,"s[128]",doo))
	{
		vlive_err(playerid,"/do [spostrze¿enia]");
		return 1;
	}
	new tresc[256],ghghgh[32],e_imie[24],e_nazwisko[24],Float:x,Float:y,Float:z;
	GetPlayerName(playerid,ghghgh,sizeof ghghgh);
	sscanf(ghghgh,"p<_>s[24]s[24]",e_imie,e_nazwisko);
	doo[0] = toupper(doo[0]);
	
	if(pInfo[playerid][pMasked] > 0)
	{
		//zamaskowany
		new maskedMsg[32];
		format(maskedMsg,sizeof(maskedMsg),"Zamaskowany %i",PlayerCache[playerid][pMaskedNumbers]);
		format(tresc,sizeof tresc,"** %s ** (( %s ))",doo,maskedMsg);
	}
	else
	{
		format(tresc,sizeof tresc,"** %s ** (( %s %s ))",doo,e_imie,e_nazwisko);
	}
	
	GetPlayerPos(playerid,x,y,z);
	
	new sendervw = GetPlayerVirtualWorld(playerid);

	for (new i=0; i <= MAX_PLAYERS+1 ; i++)
	{
		if(IsPlayerInRangeOfPoint(i,10,x,y,z))
		{
			new gainervw = GetPlayerVirtualWorld(i);
			if(sendervw == gainervw)
			{
				//SendClientMessage(i,COLOR_DO,tresc);
				SendCustomPlayerMessage(i, COLOR_DO, tresc);
			}
		}
	}
	return 1;
}

COMMAND:akceptujsmierc(playerid,params[])
{
	if(pInfo[playerid][pBw] > 0)
	{
		new buffer[256];
		format(buffer,sizeof(buffer),"SELECT char_time FROM core_players WHERE char_uid=%i",GetPlayerUID(playerid));
		mysql_query(buffer);
		mysql_store_result();
		new time = mysql_fetch_int();
		mysql_free_result();
		if(time < 36000)
		{
			GuiInfo(playerid,"Nie masz dziesiêciu godzin.");
			return 1;
		}
		
		ShowPlayerDialog(playerid,DIAL_AS,DIALOG_STYLE_INPUT,"Powód œmierci","Wpisz poni¿ej okolicznoœci œmierci Twojej postaci.","Gotowe","Anuluj");
	}
	else
	{
		GuiInfo(playerid,"Musisz byæ nieprzytomny, by u¿yæ tej komendy.");
	}
	return 1;
}

COMMAND:b(playerid,params[])
{
	new doo[128];
	if(sscanf(params,"s[128]",doo))
	{
		vlive_err(playerid,"/b [wiadomosc]");
		return 1;
	}
	if(pInfo[playerid][blockOOC] > 0)
	{
		GuiInfo(playerid,"Posiadasz aktywn¹ blokadê czatu OOC.");
	}
	else
	{
		new tresc[256],hghghg[32],e_imie[24],e_nazwisko[24],Float:x,Float:y,Float:z;
		GetPlayerName(playerid,hghghg,sizeof hghghg);
		sscanf(hghghg,"p<_>s[24]s[24]",e_imie,e_nazwisko);
		if(pInfo[playerid][pAdmin] > 0)
		{
			format(tresc,sizeof tresc,"(( [%i] %s: %s ))",playerid,GetPlayerGlobalNickname(playerid),doo);
		}
		else
		{
			if(pInfo[playerid][pMasked] > 0)
			{
				//zamaskowany
				new maskedMsg[32];
				format(maskedMsg,sizeof(maskedMsg),"Zamaskowany %i",PlayerCache[playerid][pMaskedNumbers]);
				format(tresc,sizeof tresc,"(( %s: %s ))",maskedMsg,doo);
			}
			else
			{
				format(tresc,sizeof tresc,"(( [%i] %s %s: %s ))",playerid,e_imie,e_nazwisko,doo);
			}
		}
		GetPlayerPos(playerid,x,y,z);
		
		new sendervw = GetPlayerVirtualWorld(playerid);

		for (new i=0; i <= MAX_PLAYERS+1 ; i++)
		{
			if(IsPlayerInRangeOfPoint(i,10,x,y,z))
			{
				new gainervw = GetPlayerVirtualWorld(i);
				if(sendervw == gainervw)
				{
					//SendClientMessage(i,COLOR_GREY,tresc);
					SendCustomPlayerMessage(i, COLOR_GREY, tresc);
				}
			}
		}
	}
	return 1;
}

COMMAND:vtest(playerid,params[])
{
	#pragma unused params
	//kolejna cmd ALPHA
	new modelid,kolor,Float:x,Float:y,Float:z;
	GetPlayerPos(playerid,x,y,z);
	sscanf(params,"ii",modelid,kolor);
	
	CreateVehicle(modelid,x+2,y,z,0,kolor,kolor,CAR_DELAY);
	return 1;
}

COMMAND:qs(playerid,params[])
{
	#pragma unused params
	QuickSave(playerid);
	SavePlayerInfo(playerid);
	Kick(playerid);
	return 1;
}

COMMAND:stats(playerid,pars[])
{
	new buffer[256],output[512],title[256];
	format(buffer,sizeof(buffer),"SELECT char_time FROM core_players WHERE char_uid=%i",GetPlayerUID(playerid));
	mysql_query(buffer);
	mysql_store_result();
	new time = mysql_fetch_int();
	mysql_free_result();
	time = time/60/60;
	format(buffer,sizeof(buffer),"SELECT member_id FROM %s WHERE name='%s'",MYBB_TABLE,GetPlayerGlobalNickname(playerid));
	mysql_query(buffer);
	mysql_store_result();
	new gid = mysql_fetch_int();
	mysql_free_result();
	format(title,sizeof(title),"%s [SampID: %i] [UID: %i] [GID: %i]",pInfo[playerid][pName],playerid,pInfo[playerid][pUID],gid);
		
	format(output, sizeof(output), "Konto globalne : "COL_GREEN2"\t\t%s",GetPlayerGlobalNickname(playerid));
	format(output, sizeof(output), "%s\nCzas gry postaci : "COL_GREEN2"\t\t%i godzin",output,time);
	format(output, sizeof(output), "%s\nPunkty ¿ycia : "COL_GREEN2"\t\t%f",output,pInfo[playerid][pHealth]);
	format(output, sizeof(output), "%s\nPieni¹dze w portfelu : "COL_GREEN2"\t\t%i$",output,pInfo[playerid][pMoney]);
	format(output, sizeof(output), "%s\nAktualny skin : "COL_GREEN2"\t\t%i",output,pInfo[playerid][pSkin]);
	format(output, sizeof(output), "%s\nLiczba warnów : "COL_GREEN2"\t\t%i/5",output,pInfo[playerid][pWarns]);
	format(output, sizeof(output), "%s\nNumer konta bankowego : "COL_GREEN2"\t\t%i",output,pInfo[playerid][pBankNR]);
	format(output, sizeof(output), "%s\nStan konta bankowego : "COL_GREEN2"\t\t%i$",output,pInfo[playerid][pBank]);
	format(output, sizeof(output), "%s\nPKT kondycji : "COL_GREEN2"\t\t%f",output,pInfo[playerid][maxCondition] );
	format(output, sizeof(output), "%s\nPKT si³y : "COL_GREEN2"\t\t%f",output,pInfo[playerid][pPower] );
	format(output, sizeof(output), "%s\nPKT celnoœci : "COL_GREEN2"\t\t%f",output,pInfo[playerid][pShootskill] );
	format(output, sizeof(output), "%s\n-------",output);
	format(output, sizeof(output), "%s\n"COL_GRAY2"[ "COL_ORANGE"ustaw styl walki "COL_GRAY2"]",output);
		
	ShowPlayerDialog(playerid,DIAL_STATS,DIALOG_STYLE_LIST,title,output,"OK","");
	return 1;
}

COMMAND:przeladuj(playerid,pars[])											//przeladuj gunid ammoid
{
	new uid_bron,uid_amunicja;
	if(sscanf(pars,"ii",uid_bron,uid_amunicja))
	{
		vlive_err(playerid,"/przeladuj [uid broni] [uid amunicji]");
		return 1;
	}
	new query[256];
	format(query,sizeof(query),"SELECT item_uid,item_name FROM core_items WHERE item_uid=%i AND item_owneruid=%i",uid_bron,GetPlayerUID(playerid));
	mysql_query(query);
	mysql_store_result();
	if(mysql_num_rows() > 0)
	{
		mysql_free_result();
		new query2[256];
		format(query2,sizeof(query2),"SELECT item_value1 FROM core_items WHERE item_uid=%i",uid_bron);
		mysql_query(query2);
		mysql_store_result();
		new type_gun = mysql_fetch_int();
		mysql_free_result();
		new query3[256];
		format(query3,sizeof(query3),"SELECT item_value1,item_value2 FROM core_items WHERE item_uid=%i AND item_type=%i",uid_amunicja,3);
		mysql_query(query3);
		mysql_store_result();
		new resline[256],ilosc_ammo,type_ammo;
		while(mysql_fetch_row(resline,"|"))
		{
			sscanf(resline,"p<|>ii",type_ammo,ilosc_ammo);
		}
		mysql_free_result();
		
		if(IsItemUsed(uid_bron))
		{	
			GuiInfo(playerid,"Broñ musi byæ schowana by j¹ prze³adowaæ.");
			return 1;
		}
		
		if(type_gun == type_ammo)
		{
			SendClientMessage(playerid,COLOR_RED,"(WEAPON SYSTEM) Broñ zosta³a prze³adowana.");
			new query4[256];
			format(query4,sizeof(query4),"UPDATE core_items SET item_value2=item_value2+%i WHERE item_uid=%i",ilosc_ammo,uid_bron);
			mysql_query(query4);
			
			new query5[256];
			format(query5,sizeof(query5),"DELETE FROM core_items WHERE item_uid=%i",uid_amunicja);
			mysql_query(query5);
		}
		else
		{
			GuiInfo(playerid,"U¿y³eœ amunicjê, która nie pasuje do tej broni.");
		}
	}
	return 1;
}

COMMAND:kasa(playerid,p[])
{
	new ile = strval(p);
	if(!p[0])
	{
		GuiInfo(playerid,"U¿ycie : /kasa "COL_GREEN"iloœæ");
	}
	else
	{
		new query[256],item[32];
		//sprawdzanie czy ma tyle hajsu
		format(query,sizeof(query),"SELECT char_money,char_uid FROM core_players WHERE char_uid=%i",GetPlayerUID(playerid));
		mysql_query(query);
		mysql_store_result();
		new resline[64],money,uid;
		while(mysql_fetch_row(resline,"|"))
		{
			sscanf(resline,"p<|>ii",money,uid);
		}
		if(ile < 0) { }
		else
		{
			if(ile == 0 )	{ }
			else
			{
				if (money - ile > 0)
				{
					new pozostalosc = money-ile;
					format(item,sizeof(item),"Kasa (%i $)",ile);
				
					//dodawanie itema
					format(query,sizeof(query),"INSERT INTO core_items VALUES(NULL,'%i','%s','%i','%i','brak','%i','%f','%f','%f','%i','%i','%i','0')",GetPlayerUID(playerid),item,ile,0,9,0,0,0,0,0,0);
					mysql_query(query);
					
					//odejmowanie hajsu z konta gracza
					//printf("POZOSTALOSC : %i",pozostalosc);
					format(query,sizeof(query),"UPDATE core_players SET char_money=%i WHERE char_uid=%i",pozostalosc,GetPlayerUID(playerid));
					mysql_query(query);
					pInfo[playerid][pMoney] = pozostalosc;
					
					ResetPlayerMoney(playerid);
					GivePlayerMoney(playerid,pozostalosc);
					
				}
			}
		}
	}
	return 1;
}

COMMAND:przejscie(playerid,params[])
{
	// komenda do przechodzenia przez drzwi jeœli gostek jest zakuty
	if(pInfo[playerid][pCuffed] > 0)
	{
		GameTextForPlayer(playerid, "Probujesz otworzyc drzwi", 3000, 5);
		SetTimerEx("EnterDoorIfCuffed", 3000, false, "i", playerid);
		
	}
	else
	{
		vlive_err(playerid,"aby u¿yæ tej komendy musisz byæ skuty kajdankami");
	}
	return 1;
}

COMMAND:tel(playerid,params[])
{
	if(GetPlayerPhoneInUse(playerid) == 2)
	{
		GuiInfo(playerid,"Wykryto b³¹d w systemie przedmiotów, musisz zg³osiæ siê do administratora. \n Error CODE : 7374");
		return 1;
	}
	GetPlayerPhoneInUse(playerid);
	new buffer[256];
	format(buffer,sizeof(buffer),"SELECT * FROM core_items WHERE item_owneruid=%i AND item_value2=1 AND item_type=%i",GetPlayerUID(playerid),ITEM_PHONE);
	mysql_query(buffer);
	mysql_store_result();
	if(mysql_num_rows() > 0)
	{
		new numer;
		if(sscanf(params,"d",numer))
		{
			ShowPlayerContacts(playerid);
			return 1;
		}
		if(numer == 911)
		{
			ApplyPhoneAnim(playerid);
			ShowPlayerDialog(playerid,DIAL_911,DIALOG_STYLE_LIST,"Numer alarmowy","1. Los Santos Police Department\n2. Los Santos Fire Department\n3. Los Santos Medical Center\n4. Taxi","Wybierz","Zamknij");
			return 1;
		}
		if(numer == 333)
		{
			ShowRequestViaPhone(playerid);
			return 1;
		}
		if(numer == pInfo[playerid][pPhoneInUse])
		{
			vlive_err(playerid,"wska¿ mi sieæ w której mo¿esz OOC zadzwoniæ sam do siebie to udostêpniê t¹ opcjê.");
		}
		else
		{
			if(CzyRozmawia[playerid] > 0)
			{
				vlive_err(playerid,"rozumiem, ¿e chcesz prowadziæ dwie rozmowy jednoczeœnie?");
			}
			else
			{
				IsPhoneOwnerOnline(numer,playerid);
			}
		}
	}
	else
	{
		GuiInfo(playerid,"Nie masz ¿adnego telefonu w u¿yciu.");
	}
	mysql_free_result();
	return 1;
}

COMMAND:sms(playerid,params[])
{
	return 1;
}

COMMAND:zakoncz(playerid,params[])
{
	//zakoncz rozmowe
	if(CzyRozmawia[playerid] > 0)
	{
		DeletePhoneAnim(playerid);
		DeletePhoneAnim(ToWho[playerid] );
		SendClientMessage(ToWho[playerid],COLOR_YELLOW,"Osoba z któr¹ rozmawia³eœ roz³¹czy³a siê.");
		SendClientMessage(playerid,COLOR_YELLOW,"Roz³¹czy³eœ siê.");
		CzyRozmawia[playerid] = 0;
		CzyRozmawia[ToWho[playerid]] = 0;
		ToWho[ToWho[playerid]] = -1;
		ToWho[playerid] = -1;
	}
	else
	{
		vlive_err(playerid,"z nikim teraz nie rozmawiasz przez telefon");
	}
	return 1;
}

COMMAND:odbierz(playerid,params[])
{
	if(CzyRozmawia[playerid] == 0)
	{
		for(new i= 0; i < MAX_PLAYERS; i++)
		{
			if(IsPlayerConnect(i))
			{
				
				if(ToWho[i] == playerid)
				{
					//tu jest problem
					ToWho[playerid] = i;
					CzyRozmawia[playerid] = 1;
					ApplyPhoneAnim(playerid);
					//SetTimerEx("TalkTimer", 1000, false, "i", playerid,i);
					SendClientMessage(playerid,COLOR_YELLOW,"Odebra³eœ po³¹czenie, aby rozmawiaæ przez telefon wpisz /t [wiadomosc] lub /zakoncz by j¹ zakoñczyæ.");
					SendClientMessage(i,COLOR_YELLOW,"Osoba do której dzwonisz podnios³a s³uchawkê, napisz /t [wiadomosc] by rozpocz¹æ rozmowê lub /zakoncz by j¹ zakoñczyæ.");
					break;
				}
			}
		}
	}
	else
	{
		vlive_err(playerid,"nikt nie próbuje siê z Tob¹ po³¹czyæ, lub jesteœ w³aœnie w trakcie rozmowy");
	}
	return 1;
}

COMMAND:t(playerid,params[])
{
	if(CzyRozmawia[playerid] > 0)
	{
		new output[256],wiadomosc[128];
		if (sscanf(params,"s[128]",wiadomosc))
		{
			vlive_err(playerid,"nie mo¿esz 'nic nie mówiæ' do s³uchawki");
			return 1;
		}
		format(output,sizeof(output),"Telefon : %s",wiadomosc);
		if(CzyRozmawia[ToWho[playerid]] > 0)
		{
			SendClientMessage(ToWho[playerid],COLOR_YELLOW,output);
			new sending[256],pname[MAX_PLAYER_NAME],imie[24],nazwisko[24],Float:x,Float:y,Float:z;
			GetPlayerName(playerid,pname,sizeof(pname));
			GetPlayerPos(playerid,x,y,z);
			sscanf(pname,"p<_>s[24]s[24]",imie,nazwisko);
			
			format(sending,sizeof(sending),"%s %s mówi (Telefon): %s",imie,nazwisko,params);
			
			for(new i = 0; i < MAX_PLAYERS+1;i++)
			{
				if(IsPlayerInRangeOfPoint(i,10,x,y,z))
				{
					SendClientMessage(i,COLOR_WHITE,sending);
				}
			}
		}
		else
		{
			vlive_err(playerid,"przecie¿ ten telefon nie zosta³ jeszcze odebrany");
		}
	}
	else
	{
		vlive_err(playerid,"z nikim teraz nie rozmawiasz przez telefon");
	}
	return 1;
}

COMMAND:anim(playerid,p[])
{
	new buffer[256],anims[2048];
	format(buffer,sizeof(buffer),"SELECT anim_command,anim_uid FROM vlive_anim");
	mysql_query(buffer);
	mysql_store_result();
	while(mysql_fetch_row(buffer,"|"))
	{
		new animid,animname[12];
		sscanf(buffer,"p<|>s[12]i",animname,animid);
		format(anims,sizeof(anims),"%s\n%i\t\t%s",anims,animid,animname);
	}
	mysql_free_result();
	ShowPlayerDialog(playerid,404,DIALOG_STYLE_LIST,"Lista animacji",anims,"OK","");
	return 1;
}

COMMAND:skuj(playerid,params[])
{
	new buffer[256];
	format(buffer,sizeof(buffer),"SELECT item_uid FROM core_items WHERE item_owneruid=%i AND item_type=%i AND item_whereis=0",GetPlayerUID(playerid),ITEM_KAJDANKI);
	mysql_query(buffer);
	mysql_store_result();
	if(mysql_num_rows() > 0)
	{
		new target,Float:x,Float:y,Float:z;
		if(sscanf(params,"d",target))
		{
			vlive_err(playerid,"/skuj [gracz]");
			return 1;
		}
		if(IsPlayerConnect(target))
		{
			if(target == playerid)
			{
				GuiInfo(playerid,"Lubisz skuwaæ sam siebie? ");
				return 1;
			}
			GetPlayerPos(playerid,x,y,z);
			new playervw = GetPlayerVirtualWorld(playerid);
			new targetvw = GetPlayerVirtualWorld(target);
			if(IsPlayerInRangeOfPoint(target, 5, x,y,z))
			{
				if(playervw == targetvw)
				{
					if(pInfo[playerid][pCuffed] > 0)
					{
						GuiInfo(playerid,"Jakim cudem chcesz zakuæ osobê samemu bêd¹c skutym?");
						return 1;
					}
					if(pInfo[target][pCuffed] > 0)
					{
						if(pInfo[playerid][pCuffPlayer] == target)
						{
							pInfo[target][pCuffed] = 0;
							pInfo[playerid][pCuffPlayer] = 0;
							SetPlayerSpecialAction(target,SPECIAL_ACTION_NONE);
							RemovePlayerAttachedObject(target,SLOT_CUFFS);
						}
						else
						{
							GuiInfo(playerid,"Nie posiadasz kluczy do tych kajdanek.");
						}
					}
					else
					{
						pInfo[target][pCuffed] = 1;
						pInfo[playerid][pCuffPlayer] = target;
						SetPlayerSpecialAction(target,SPECIAL_ACTION_CUFFED);
						SetPlayerAttachedObject(target, SLOT_CUFFS, 19418, 6, -0.011000, 0.028000, -0.022000, -15.600012, -33.699977, -81.700035, 0.891999, 1.000000, 1.168000);
						new wiad[128];
						format(wiad,sizeof(wiad),"Zosta³eœ skuty przez gracza: %s (SampID: %i)",pInfo[playerid][pName],playerid);
						SendClientMessage(target,COLOR_GOLD,wiad);
					}
				}
				else
				{
					vlive_err(playerid,"gracz poza zasiêgiem");
				}
			}
			else
			{
				vlive_err(playerid,"gracz jest poza zasiêgiem");
			}
		}
		else
		{
			vlive_err(playerid,"ten gracz nie jest pod³¹czony do serwera");
		}
	}
	else
	{
		GuiInfo(playerid,"Nie posiadasz kajdanek w ekwipunku.");
	}
	mysql_free_result();
	return 1;
}

CMD:d(playerid,params[])
{
	new groupType = DutyGroupType[playerid];
	if(groupType == TYPE_LSMC || groupType == TYPE_LSFD || groupType == TYPE_LSPD || groupType == TYPE_FBI )
	{
		new deptMsg[256],groupTag[10];
		if(sscanf(params,"s[256]",deptMsg))
		{
			vlive_err(playerid,"/d [wiadomosc]");
			return 1;
		}
		
		for(new i = 0; i < MAX_GROUPS; i++)
		{
			if(grupa[i][Guid] == pGrupa[playerid][PlayerDutyGroup[playerid]][pGuid])
			{
				format(groupTag,sizeof(groupTag),"%s",grupa[i][Gtag]);
				break;
			}
		}
		
		new imie[32],nazwisko[32];
		sscanf(pInfo[playerid][pName],"p<_>s[32]s[32]",imie,nazwisko);
		new radioMsg[256];
		format(radioMsg,sizeof(radioMsg),"** %s %s [%s]: %s",imie,nazwisko,groupTag,deptMsg);
		
		//wysy³anie na ca³y teren Los Santos do graczy na duty departamentowych frakcji
		for(new i = 0; i < MAX_PLAYERS; i++)
		{
			if(DutyGroupType[i] == TYPE_LSMC || DutyGroupType[i] == TYPE_LSFD || DutyGroupType[i] == TYPE_LSPD || DutyGroupType[i] == TYPE_FBI)
			{
				SendCustomPlayerMessage(i, COLOR_DEPARTMENT, radioMsg);
			}
		}
		
		format(deptMsg,sizeof(deptMsg),": %s",deptMsg);
		//wysy³anie w okolicy 10m do graczy w poblizu
		SendPlayerRadioToNearPlayers(playerid,deptMsg);
	}
	else
	{
		GuiInfo(playerid,"Brak odpowiednich uprawnieñ.");
		return 1;
	}
	return 1;
}

CMD:l(playerid,params[])
{
	new msg[256];
	if(sscanf(params,"s[256]",msg))
	{
		vlive_err(playerid,"/l wiadomosc");
		return 1;
	}
	
	if(pInfo[playerid][pBw] != 0)
	{
		GuiInfo(playerid,"Nie mo¿esz odzywaæ siê bêd¹c nieprzytomnym.");
		return 0;
	}
	if (chatstat == 1)
	{
		return chatstat;
	}
	if (chatstat == 0)
	{
		new sending[256],pname[MAX_PLAYER_NAME],imie[24],nazwisko[24],Float:x,Float:y,Float:z;
		GetPlayerName(playerid,pname,sizeof(pname));
		msg[0] = toupper(msg[0]);
		GetPlayerPos(playerid,x,y,z);
		if(pInfo[playerid][pAdmin] > 0)
		{
			format(sending,sizeof(sending),"%s mówi: %s",GetPlayerGlobalNickname(playerid),msg);
		}
		else
		{
			sscanf(pname,"p<_>s[24]s[24]",imie,nazwisko);	
			format(sending,sizeof(sending),"%s %s mówi: %s",imie,nazwisko,msg);
		}
		
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
	return 1;
}

CMD:sprobuj(playerid, params[])
{
	new text[128], string[256];
	if(sscanf(params, "s[128]", text))
	{
		vlive_err(playerid, "/sprobuj [akcja]");
	    return 1;
	}
	new loss = random(2);
	new imie[32],nazwisko[32],Float:x,Float:y,Float:z;
	
	sscanf(pInfo[playerid][pName],"p<_>s[32]s[32]",imie,nazwisko);
	
	switch(loss)
	{
		case 0:
		{
			if(pInfo[playerid][pMasked] > 0)
			{
				new maskedMsg[32];
				format(maskedMsg,sizeof(maskedMsg),"Zamaskowany %i",PlayerCache[playerid][pMaskedNumbers]);
				format(string, sizeof(string), "* %s zawiód³ próbuj¹c %s",maskedMsg,text);
			}
			else
			{
				format(string, sizeof(string), "* %s %s zawiód³ próbuj¹c %s", imie,nazwisko,text);
			}
			//format(string, sizeof(string), "* %s %s zawiód³ próbuj¹c %s", imie,nazwisko,text);
			GetPlayerPos(playerid,x,y,z);
	
			new sendervw = GetPlayerVirtualWorld(playerid);
			
			for (new i=0; i <= MAX_PLAYERS+1 ; i++)
			{
				if(IsPlayerInRangeOfPoint(i,10,x,y,z))
				{
					new gainervw = GetPlayerVirtualWorld(i);
					if(sendervw == gainervw)
					{
						SendCustomPlayerMessage(i, COLOR_ME, string);
					}
				}
			}
		}
		case 1:
		{
			if(pInfo[playerid][pMasked] > 0)
			{
				new maskedMsg[32];
				format(maskedMsg,sizeof(maskedMsg),"Zamaskowany %i",PlayerCache[playerid][pMaskedNumbers]);
				format(string, sizeof(string), "* %s zawiód³ próbuj¹c %s",maskedMsg,text);
			}
			else
			{
				format(string, sizeof(string), "* %s %s odniós³ sukces próbuj¹c %s",imie,nazwisko,text);
			}
			
			GetPlayerPos(playerid,x,y,z);
	
			new sendervw = GetPlayerVirtualWorld(playerid);
			
			for (new i=0; i <= MAX_PLAYERS+1 ; i++)
			{
				if(IsPlayerInRangeOfPoint(i,10,x,y,z))
				{
					new gainervw = GetPlayerVirtualWorld(i);
					if(sendervw == gainervw)
					{
						//SendClientMessage(i,COLOR_ME,tresc);
						SendCustomPlayerMessage(i, COLOR_ME, string);
					}
				}
			}
		}
	}
	return 1;
}

COMMAND:bankomat(playerid,params[])
{
	new bkid = 0;
	new vw = GetPlayerVirtualWorld(playerid);
	for(new i = 0 ; i < MAX_BANKOMATS;i++)
	{
		if(vw == bankomat[i][bkVW])
		{
			if(IsPlayerInRangeOfPoint(playerid,3,bankomat[i][bkX],bankomat[i][bkY],bankomat[i][bkZ]))
			{
				bkid = i;
				break;
			}
		}
	}
	if(bkid > 0)
	{
		if(pInfo[playerid][pBankomat] > 0)
		{
			GuiInfo(playerid,"Z bankomatu mo¿na korzystaæ tylko raz na dziesieæ minut.");
		}
		else
		{
			ShowPlayerDialog(playerid,DIAL_BANKOMAT,DIALOG_STYLE_LIST,"Bankomat","1. Stan konta\n2. Wyp³aæ pieni¹dze\n3. Wp³aæ pieni¹dze","Wybierz","Zamknij");
		}
	}
	else
	{
		GuiInfo(playerid,"Nie znaleziono ¿adnego bankomatu w pobli¿u.");
	}
	return 1;
}

COMMAND:tankuj(playerid,params[])
{
	new ilosc;
	if(sscanf(params,"d",ilosc))
	{
		vlive_err(playerid,"/tankuj [ilosc litrow]");
		return 1;
	}
	new stacjaid = 0;
	new vw = GetPlayerVirtualWorld(playerid);
	for(new i = 0 ; i < MAX_PETROLS ; i ++)
	{
		if(IsPlayerInRangeOfPoint(playerid,5,petrol[i][peX],petrol[i][peY],petrol[i][peZ]))
		{
			if(petrol[i][peVW] == vw)
			{
				//jest stacja
				stacjaid = i;
				break;
			}
		}
	}
	
	new test = 1;
	
	if(stacjaid != 0)
	{
		for(new i=0; i < MAX_VEHICLES; i++)
		{
			new Float:x,Float:y,Float:z;
			GetVehiclePos(i,x,y,z);
			if(IsPlayerInRangeOfPoint(playerid,5,x,y,z))
			{
				new vehvw = GetVehicleVirtualWorld(i);
				if(vehvw == vw)
				{
					if(/*vehicle[i][vownertype] == 2*/ test == 1)
					{
						if(/*vehicle[i][vowneruid] == GetPlayerUID(playerid)*/ test == 1)
						{
							//tankowanie
							if(sscanf(params,"d",ilosc))
							{
								vlive_err(playerid,"/tankuj [ilosc]");
								return 1;
							}
							else
							{
								new vehicleModel = GetVehicleModel(i);
								new vehicleMaxFuel = 0;
								for(new t = 0 ; t < 300; t++)
								{
									if(vehicleModel == VehicleData[t][vModel])
									{
										vehicleMaxFuel = VehicleData[t][vFuel];
										break;
									}
								}
								new pustemiejsce = vehicleMaxFuel -floatround(vehicle[i][vfuel]);
								if(ilosc > pustemiejsce)
								{
									GuiInfo(playerid,"Nie ma tyl miejsca w baku!");
									break;
								}
								else if ( ilosc <= 0)
								{
									GuiInfo(playerid,"Chcesz wlaæ 'nic' paliwa?");
									break;
								}
								else
								{
									new ilekasy = ilosc*PETROL_COST_PER_LITER;
									if(pInfo[playerid][pMoney] >= ilekasy)
									{
										//plac
										new buffer[256];
										format(buffer,sizeof(buffer),"UPDATE core_players SET char_money=char_money-%i WHERE char_uid=%i",ilekasy,GetPlayerUID(playerid));
										mysql_query(buffer);
										pInfo[playerid][pMoney] = pInfo[playerid][pMoney]-ilekasy;
										vehicle[i][vfuel] = vehicle[i][vfuel] + ilosc;
										PublicMe(playerid,"wk³ada w¹¿ do baku.");
										new output[256];
										format(output,sizeof(output),"Zap³aci³eœ %i$ za %i litrów paliwa.",ilekasy,ilosc);
										SendClientMessage(playerid,COLOR_CRIMSON,output);
										ApplyAnimation(playerid, "INT_HOUSE", "wash_up",4.1, 0, 0, 0, 0, 0, 1);
										break;
									}
									else
									{
										GuiInfo(playerid,"Nie masz tyle pieniêdzy!");
										break;
									}
								}
							}
						}
					}
				}
			}
		}
	}
	else
	{
		GuiInfo(playerid,"Nie znaleziono ¿adnej stacji benzynowej w pobli¿u.");
	}
	return 1;
}

COMMAND:zamaskuj(playerid,params[])
{	
	new buffer[256];
	format(buffer,sizeof(buffer),"SELECT item_stacjafm FROM core_items WHERE item_type=%i AND item_owneruid=%i",ITEM_MASK,GetPlayerUID(playerid));
	mysql_query(buffer);
	mysql_store_result();
	if(mysql_num_rows() > 0)
	{
		if(pInfo[playerid][pMasked] > 0)
		{
			mysql_free_result();		
			PublicMe(playerid,"œci¹ga maskê z twarzy.");
			pInfo[playerid][pMasked] = 0;	
			statusPlayer[playerid] = 1;
		}
		else
		{
			mysql_free_result();			
			PublicMe(playerid,"zak³ada maskê na twarz.");
			pInfo[playerid][pMasked] = 1;	
			statusPlayer[playerid] = 1;
		}
	}
	else
	{
		mysql_free_result();
		GuiInfo(playerid,"Nie posiadasz maski.");
	}
	return 1;
}

CMD:opis(playerid,params[])
{
	ShowPlayerDialog(playerid,DIAL_OPIS,DIALOG_STYLE_LIST,"System opisów","1. Wybierz opis\n2. WprowadŸ nowy opis\n3. Usuñ opis z listy\n4. Usuñ opis postaci","Wybierz","Anuluj");
	return 1;
}

forward PreloadDesc(playerid,desc[]);
public PreloadDesc(playerid,desc[])
{
	UpdateDynamic3DTextLabelText(Text3D:PlayerDesc[playerid], COLOR_ME, FormatPlayerDescription(desc));
}

COMMAND:pokaz(playerid,params[])
{
	new co[32],komu;
	if(sscanf(params,"s[32]d",co,komu))
	{
		vlive_err(playerid,"/pokaz [dowod | prawko] [id]");
		return 1;
	}
	//tu pokazywanie
	if(!strcmp(co, "dowod", true))
	{
		if(IsPlayerConnected(komu))
		{
			new Float:x,Float:y,Float:z;
			GetPlayerPos(playerid,x,y,z);
			if(IsPlayerInRangeOfPoint(komu,3,x,y,z))
			{
				if(pInfo[playerid][pHasDowod] == 0)
				{
					GuiInfo(playerid,"Nie posiadasz dowodu osobistego.");
					return 1;
				}
				new output[128];
				format(output,128,"Imiê i nazwisko: %s\nWiek: %i",pInfo[playerid][pName],pInfo[playerid][pWiek]);
				new targetvw = GetPlayerVirtualWorld(komu);
				new playervw = GetPlayerVirtualWorld(playerid);
				if(targetvw == playervw)
				{
					ShowPlayerDialog(komu,404,DIALOG_STYLE_MSGBOX,"** Dowód osobisty **",output,"Zamknij","");
					PublicMe(playerid,"pokazuje dowód osobisty.");
				}
			}
		}
	}
	if(!strcmp(co, "prawko", true))
	{
		if(IsPlayerConnected(komu))
		{
			new Float:x,Float:y,Float:z;
			GetPlayerPos(playerid,x,y,z);
			if(IsPlayerInRangeOfPoint(komu,3,x,y,z))
			{
				new output[128];
				new ile = 0;
				format(output,128,"Imiê i nazwisko: %s",pInfo[playerid][pName]);
				if(pInfo[playerid][pHasPrawkoA] == 1)
				{
					format(output,128,"%s\nKategoria A",output);
					ile++;
				}
				if(pInfo[playerid][pHasPrawkoB] == 1)
				{
					format(output,128,"%s\nKategoria B",output);
					ile++;
				}
				if(pInfo[playerid][pHasPrawkoC] == 1)
				{
					format(output,128,"%s\nKategoria C",output);
					ile++;
				}
				if(pInfo[playerid][pHasPrawkoCE] == 1)
				{
					format(output,128,"%s\nKategoria C+E",output);
					ile++;
				}
				if(pInfo[playerid][pHasPrawkoD] == 1)
				{
					format(output,128,"%s\nKategoria D",output);
					ile++;
				}
				if(ile <= 0)
				{
					GuiInfo(playerid,"Nie posiadasz prawa jazdy.");
					return 1;
				}
				new targetvw = GetPlayerVirtualWorld(komu);
				new playervw = GetPlayerVirtualWorld(playerid);
				if(targetvw == playervw)
				{
					ShowPlayerDialog(komu,404,DIALOG_STYLE_MSGBOX,"** Prawo jazdy **",output,"Zamknij","");
					PublicMe(playerid,"pokazuje prawo jazdy.");
				}
			}
		}
	}
	return 1;
}

COMMAND:bank(playerid,params[])
{
	new doorid = GetPlayerDoorID(playerid);
	if(doorid != 0 && DoorInfo[doorid][doorOwnerType] == DOOR_TYPE_BANK)
	{
		if(pInfo[playerid][pBankNR] <= 0)
		{
			bankrandom(playerid);		
		}
		ShowPlayerDialog(playerid,DIAL_BANK,DIALOG_STYLE_LIST,"Bank","1. Wp³aæ gotówkê\n2. Wyp³aæ gotówkê\n3. Dokonaj przelewu\n4. Stan konta","Wybierz","Zamknij");
	}
	else
	{
		GuiInfo(playerid,"Nie jesteœ w banku.");
	}
	return 1;
}

COMMAND:mysql(playerid,params[])
{
	new msg[128];
	format(msg,sizeof(msg),"Wykonano %i zapytañ od czasu uruchomienia pluginu.",mysqlPerformed);
	ShowPlayerDialog(playerid,4004,DIALOG_STYLE_MSGBOX,"Info",msg,"OK","");
	return 1;
}

CMD:reanimuj(playerid,params[])
{
	new target;
	if(sscanf(params,"d",target))
	{
		vlive_err(playerid,"/reanimuj [id]");
		return 1;
	}
	
	new gid = PlayerDutyGroup[playerid];
	new czy_moze = 0;
	
	for(new i = 0 ; i< MAX_GROUPS; i++)
	{
		if(grupa[i][Gtype] == TYPE_LSMC)
		{
			if(grupa[i][Guid] == pGrupa[playerid][gid][pGuid])
			{
				czy_moze = 1;
				break;
			}
		}
	}
	
	if(czy_moze == 0)
	{
		GuiInfo(playerid,"Brak uprawnieñ.");
		return 1;
	}
	
	new Float:x,Float:y,Float:z;
	GetPlayerPos(playerid,x,y,z);
	
	if(IsPlayerInRangeOfPoint(target,10,x,y,z))
	{
		if(pInfo[target][pBw] > 0)
		{
			PlayerBwTimer[target] = 1;
		}
		else
		{
			vlive_err(playerid,"ten gracz nie ma BW");
		}
	}
	else
	{
		vlive_err(playerid,"gracz poza zasiêgiem");
	}
	return 1;
}

CMD:przeszukaj(playerid,params[])
{
	new groupType = DutyGroupType[playerid];
	if(groupType == TYPE_LSPD || groupType == TYPE_FBI || groupType == TYPE_ORG)
	{
		new target;
		if(sscanf(params,"d",target))
		{
			vlive_err(playerid,"/przeszukaj [id]");
			return 1;
		}
		
		if(IsPlayerInRangeOfPlayer(playerid,target,5))
		{
			ShowPlayerItemsToPlayer(target,playerid);
		}
		else
		{
			GuiInfo(playerid,"Gracz poza zasiêgiem");
		}
	}
	else
	{
		GuiInfo(playerid,"Brak uprawnieñ.");
	}
	return 1;
}

CMD:kolczatka(playerid, params[])
{
	new groupType = DutyGroupType[playerid];
	if(groupType == TYPE_LSMC || groupType == TYPE_LSFD || groupType == TYPE_LSPD || groupType == TYPE_FBI )
	{
		
	}
	else
	{
		GuiInfo(playerid,"Brak uprawnieñ.");
		return 1;
	}
	
  	new blockade_id;
  	if(sscanf(params, "d", blockade_id))
  	{
  	    vlive_err(playerid, "/kolczatka [nr kolczatki 1-10]");
  	    return 1;
  	}
  	if(blockade_id <= 0 || blockade_id > 10)
  	{
  	    GuiInfo(playerid,"Nieprawid³owe ID kolczatki.");
  	    return 1;
  	}
  	if(IsPlayerInAnyVehicle(playerid))
  	{
  	    GuiInfo(playerid,"Nie mo¿esz stawiaæ kolczatki bêd¹c w pojeŸdzie.");
  	    return 1;
  	}
  	if(IsValidDynamicObject(object[PoliceKolczatka[blockade_id]][object_sid]))
  	{
  	    DestroyDynamicObject(object[PoliceKolczatka[blockade_id]][object_sid]);
  	    GameTextForPlayer(playerid, "~n~~n~~n~~n~~n~~n~~n~~r~Usunieto ~y~kolczatke", 3000, 3);
		object[PoliceKolczatka[blockade_id]][object_sid] = EOS;
		object[PoliceKolczatka[blockade_id]][object_model] = EOS; 
  	    return 1;
  	}
  	new Float:PosX, Float:PosY, Float:PosZ, Float:PosA;
  	
  	GetPlayerPos(playerid, PosX, PosY, PosZ);
  	GetPlayerFacingAngle(playerid, PosA);  	
	new freeslot = CheckFreeObjectSlot();	
	GetXYInFrontOfPlayer(playerid, PosX, PosY, 5.0);
	object[freeslot][object_sid] = CreateDynamicObject(1593, PosX, PosY, PosZ - 0.8, 0.0, 0.0, PosA);
	object[freeslot][object_model] = 1593;
	
	PoliceKolczatka[blockade_id] = freeslot;

	Streamer_Update(playerid);
	
	GameTextForPlayer(playerid, "~n~~n~~n~~n~~n~~n~~n~~g~Postawiono ~y~kolczatke", 3000, 3);
	return 1;
}

CMD:blokada(playerid, params[])
{
	new groupType = DutyGroupType[playerid];
	if(groupType == TYPE_LSMC || groupType == TYPE_LSFD || groupType == TYPE_LSPD || groupType == TYPE_FBI )
	{
		
	}
	else
	{
		GuiInfo(playerid,"Brak uprawnieñ.");
		return 1;
	}
	
  	new blockade_id;
  	if(sscanf(params, "d", blockade_id))
  	{
  	    vlive_err(playerid, "/blokada [nr blokady 1-5]");
  	    return 1;
  	}
  	if(blockade_id <= 0 || blockade_id > 5)
  	{
  	    GuiInfo(playerid,"Nieprawid³owe ID blokady.");
  	    return 1;
  	}
  	if(IsPlayerInAnyVehicle(playerid))
  	{
  	    GuiInfo(playerid,"Nie mo¿esz stawiaæ blokady bêd¹c w pojeŸdzie.");
  	    return 1;
  	}
  	if(IsValidDynamicObject(object[PoliceBlockade[blockade_id]][object_sid]))
  	{
  	    DestroyDynamicObject(object[PoliceBlockade[blockade_id]][object_sid]);
  	    GameTextForPlayer(playerid, "~n~~n~~n~~n~~n~~n~~n~~r~Usunieto ~y~blokade", 3000, 3);
		object[PoliceBlockade[blockade_id]][object_sid] = EOS;
		object[PoliceBlockade[blockade_id]][object_model] = EOS; 
  	    return 1;
  	}
  	new Float:PosX, Float:PosY, Float:PosZ, Float:PosA;
  	
  	GetPlayerPos(playerid, PosX, PosY, PosZ);
  	GetPlayerFacingAngle(playerid, PosA);  	
	new freeslot = CheckFreeObjectSlot();	
	GetXYInFrontOfPlayer(playerid, PosX, PosY, 5.0);
	object[freeslot][object_sid] = CreateDynamicObject(3578, PosX, PosY, PosZ - 0.5, 0.0, 0.0, PosA);
	object[freeslot][object_model] = 3578;
	
	PoliceBlockade[blockade_id] = freeslot;

	Streamer_Update(playerid);
	
	GameTextForPlayer(playerid, "~n~~n~~n~~n~~n~~n~~n~~g~Postawiono ~y~blokade", 3000, 3);
	return 1;
}

CMD:spawn(playerid,params[])
{
	new buffer[256];
	format(buffer,sizeof(buffer),"SELECT * FROM doors WHERE otype = 2 AND ouid=%i",GetPlayerUID(playerid));
	mysql_query(buffer);
	mysql_store_result();
	new nums = mysql_num_rows();
	mysql_free_result();
	
	if (nums > 0)
	{
		ShowPlayerDialog(playerid,DIAL_SET_SPAWN,DIALOG_STYLE_LIST,"Miejsce spawnu","1. Metro\n2. Idlewood\n3. Mieszkanie","Wybierz","Anuluj");
	}
	else
	{
		ShowPlayerDialog(playerid,DIAL_SET_SPAWN,DIALOG_STYLE_LIST,"Miejsce spawnu","1. Metro\n2. Idlewood","Wybierz","Anuluj");
	}
	return 1;
}

CMD:plac(playerid,params[])
{
	new odbiorca,amount;
	if(sscanf(params,"dd",odbiorca,amount))
	{
		vlive_err(playerid,"/plac [playerid] [ilosc]");
		return 1;
	}
	
	new buffer[256];
	format(buffer,sizeof(buffer),"SELECT char_time FROM core_players WHERE char_uid=%i",GetPlayerUID(playerid));
	mysql_query(buffer);
	mysql_store_result();
	new time = mysql_fetch_int();
	mysql_free_result();
	if(time < 10800)
	{
		GuiInfo(playerid,"Nie masz trzech godzin wymaganych do u¿ycia tej komendy.");
		return 1;
	}
	
	if(amount <= 0 || amount > pInfo[playerid][pMoney])
	{
		GuiInfo(playerid,"Nieprawid³owa kwota.");
		return 1;
	}
	

	if(!IsPlayerInRangeOfPlayer(playerid,odbiorca,10))
	{
		GuiInfo(playerid,"Gracz poza zasiêgiem.");
		return 1;
	}
	
	mysa_LogMoney(odbiorca,amount,MONEY_PLAYER_FOR_PLAYER,playerid);
	
	AddPlayerMoney(odbiorca,amount);
	TakePlayerMoney(playerid,amount);
	
	new adminLog[128];
	format(adminLog,sizeof(adminLog),"~r~[money]~y~ Gracz %s ~g~(UID: %i)~y~ podal ~g~%i$ graczowi~y~ %s (UID: %i)",pInfo[playerid][pName],GetPlayerUID(playerid),amount,pInfo[odbiorca][pName],GetPlayerUID(odbiorca));
	LogAdminAction(adminLog);
	
	new msg[128];
	format(msg,sizeof(msg),"poda³eœ %i$ graczowi %s",amount,pInfo[odbiorca][pName]);
	vlive_err(playerid,msg);
	
	format(msg,sizeof(msg),"otrzyma³eœ %i$ od gracza %s",amount,pInfo[playerid][pName]);
	vlive_err(odbiorca,msg);
	
	return 1;
}

CMD:pay(playerid,params[])
{
	return cmd_plac(playerid,params);
}

// -- 					--	//
// -- 					--	//
// -- 					--	//
// -- 					--	//
// -- KOMENDY TYMCZASOWE -- //
// -- 					--	//
// -- 					--	//
// -- 					--	//

CMD:pojazdstartowy(playerid,params[])
{
	if(IsPlayerHasStartVehicle(playerid))
	{
		GuiInfo(playerid,"Posiadasz ju¿ pojazd startowy.");
		return 1;
	}
	else
	{
		new buffer[256];
		format(buffer,sizeof(buffer),"SELECT char_time FROM core_players WHERE char_uid=%i",GetPlayerUID(playerid));
		mysql_query(buffer);
		mysql_store_result();
		new time = mysql_fetch_int();
		mysql_free_result();
		if(time < 10800)
		{
			GuiInfo(playerid,"Nie masz przegranych 3h by u¿yæ tej komendy.");
			return 1;
		}
		
		
		ShowModelSelectionMenu(playerid, vehiclelist, "Wybierz pojazd");
		return 1;
	}
}

COMMAND:chatstat(playerid,p[])
{
	if (chatstat == 1)
	{
		chatstat = 0;
		SendClientMessageToAll(COLOR_SERVER,"(Server) Chat In Character zosta³ w³¹czony");
	}
	else
	{
		chatstat = 1 ;
		SendClientMessageToAll(COLOR_SERVER,"(Server) Chat In Character zosta³ wy³¹czony");
	}
	return 1;
}

COMMAND:clear(playerid,p[])
{
	for(new i= 0; i < 15; i++)
	{
		SendClientMessage(playerid,COLOR_WHITE,"");
	}
	return 1;
}

COMMAND:setcamera(playerid,params[])
{
	if(CheckPlayerAccess(playerid) > PERMS_GAMEMASTER)
	{
		new Float:px,Float:py,Float:pz;
		GetPlayerPos(playerid,px,py,pz);
		new Float:x,Float:y,Float:z;
		if(sscanf(params,"fff",x,y,z))
		{
			vlive_err(playerid,"/setcamera [x] [y] [z]");
			return 1;
		}
		SetPlayerCameraPos(playerid, px+x, py+y, pz+z);
		SetPlayerCameraLookAt(playerid, px,py,pz);
	}
	else
	{
		GuiInfo(playerid,"To testowa komenda, u¿yæ jej mo¿e tylko programista.");
	}
	return 1;
}

COMMAND:playercam(playerid,params[])
{
	if(CheckPlayerAccess(playerid) > PERMS_GAMEMASTER)
	{
		SetCameraBehindPlayer(playerid);
	}
	else
	{
		GuiInfo(playerid,"To testowa komenda, u¿yæ jej mo¿e tylko programista.");
	}
	return 1;
}

AntyDeAMX()
{
	new amx[][] =
	{
		"Unarmed (Fist)",
		"Brass K"
	};
	#pragma unused amx
}