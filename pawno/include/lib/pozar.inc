#define MAX_PLOMIENIE 25

enum pozar_info
{
	Float:pozX,
	Float:pozY,
	Float:pozZ,
	Float:pozHP,
	pozObject,
}
new PozarInfo[MAX_PLOMIENIE][pozar_info];

forward PozarTimer();
public PozarTimer()
{

}

COMMAND:apozar(playerid,params[])
{
	if(CheckPlayerAccess(playerid) > PERMS_ADMIN)
	{
		new Float:x,Float:y,Float:z;
		GetPlayerPos(playerid,x,y,z);
		
		new pozid;
		if(sscanf(params,"d",pozid))
		{
			vlive_err(playerid,"/apozar [1-10]");
			return 1;
		}
		
		if(pozid > 10 || pozid < 0)
		{
			vlive_err(playerid,"nieprawidłowy pozarid");
			return 1;
		}
		
		if(!IsValidObject(PozarInfo[pozid][pozObject]))
		{
			//stworz ten plomien pierdolony
			new obid = random(5);
			switch(obid)
			{
				case 0:
				{
					PozarInfo[pozid][pozObject] = CreateObject(18692, x-1.5, y-1.5, z-2.6, 0, 0, 0);
				}
				case 1:
				{
					PozarInfo[pozid][pozObject] = CreateObject(18691, x-1.5, y-1.5, z-2.6, 0, 0, 0);
				}
				case 2:
				{
					PozarInfo[pozid][pozObject] = CreateObject(18688, x-1.5, y-1.5, z-2.6, 0, 0, 0);
				}
				case 3:
				{
					PozarInfo[pozid][pozObject] = CreateObject(18692, x-1.5, y-1.5, z-2.6, 0, 0, 0);
				}
				case 4:
				{
					PozarInfo[pozid][pozObject] = CreateObject(18688, x-1.5, y-1.5, z-2.6, 0, 0, 0);
				}
			}
		}
		else
		{
			DestroyObject(PozarInfo[pozid][pozObject]);
		}
	}
	else
	{
		GuiInfo(playerid,"Brak uprawnień.");
	}
	return 1;
}