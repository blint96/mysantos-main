COMMAND:pomoc(playerid,params[])
{
	new help_category[256];
	format(help_category,256,"1. Ogólne\n2. Obiekty\n3. Pojazdy\n4. Grupy\n5. Drzwi\n6. Przedmioty\n7. Antycheat\n8. O gamemode");
	ShowPlayerDialog(playerid,DIAL_HELP,DIALOG_STYLE_LIST,"Pomoc",help_category,"Wybierz","Zamknij");
	return 1;
}

COMMAND:createe(playerid,params[])
{
	SetPlayerAttachedObject(playerid, SLOT_TEST, 373, 1, 0.013744 ,0.007423 ,-0.010019 ,-2.759115, 8.353719, 9.100319 ,1.000000 ,1.000000 ,1.000000);
	return 1;
}

COMMAND:poss(playerid,params[])
{
	EditAttachedObject(playerid, SLOT_TEST);
	return 1;
}
