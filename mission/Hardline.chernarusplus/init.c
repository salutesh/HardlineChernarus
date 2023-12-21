void main()
{
	//INIT ECONOMY--------------------------------------
	Hive ce = CreateHive();
	if ( ce )
		ce.InitOffline();

	//DATE RESET AFTER ECONOMY INIT-------------------------
	int year, month, day, hour, minute;
	int reset_month = 9, reset_day = 20;
	GetGame().GetWorld().GetDate(year, month, day, hour, minute);

	if ((month == reset_month) && (day < reset_day))
	{
		GetGame().GetWorld().SetDate(year, reset_month, reset_day, hour, minute);
	}
	else
	{
		if ((month == reset_month + 1) && (day > reset_day))
		{
			GetGame().GetWorld().SetDate(year, reset_month, reset_day, hour, minute);
		}
		else
		{
			if ((month < reset_month) || (month > reset_month + 1))
			{
				GetGame().GetWorld().SetDate(year, reset_month, reset_day, hour, minute);
			}
		}
	}

	//! Spawn test train
	CustomMission.SetupTrains();
}

class CustomMission: MissionServer
{
	void SetRandomHealth(EntityAI itemEnt)
	{
		if ( itemEnt )
		{
			float rndHlt = Math.RandomFloat( 0.45, 0.65 );
			itemEnt.SetHealth01( "", "", rndHlt );
		}
	}

	override PlayerBase CreateCharacter(PlayerIdentity identity, vector pos, ParamsReadContext ctx, string characterName)
	{
		Entity playerEnt;
		playerEnt = GetGame().CreatePlayer( identity, characterName, pos, 0, "NONE" );
		Class.CastTo( m_player, playerEnt );

		GetGame().SelectPlayer( identity, m_player );

		return m_player;
	}

	override void StartingEquipSetup(PlayerBase player, bool clothesChosen)
	{
		EntityAI itemClothing;
		EntityAI itemEnt;
		ItemBase itemBs;
		float rand;

		itemClothing = player.FindAttachmentBySlotName( "Body" );
		if ( itemClothing )
		{
			SetRandomHealth( itemClothing );
			
			itemEnt = itemClothing.GetInventory().CreateInInventory( "BandageDressing" );
			player.SetQuickBarEntityShortcut(itemEnt, 2);
			
			string chemlightArray[] = { "Chemlight_White", "Chemlight_Yellow", "Chemlight_Green", "Chemlight_Red" };
			int rndIndex = Math.RandomInt( 0, 4 );
			itemEnt = itemClothing.GetInventory().CreateInInventory( chemlightArray[rndIndex] );
			SetRandomHealth( itemEnt );
			player.SetQuickBarEntityShortcut(itemEnt, 1);

			rand = Math.RandomFloatInclusive( 0.0, 1.0 );
			if ( rand < 0.35 )
				itemEnt = player.GetInventory().CreateInInventory( "Apple" );
			else if ( rand > 0.65 )
				itemEnt = player.GetInventory().CreateInInventory( "Pear" );
			else
				itemEnt = player.GetInventory().CreateInInventory( "Plum" );
			player.SetQuickBarEntityShortcut(itemEnt, 3);
			SetRandomHealth( itemEnt );
		}
		
		itemClothing = player.FindAttachmentBySlotName( "Legs" );
		if ( itemClothing )
			SetRandomHealth( itemClothing );
		
		itemClothing = player.FindAttachmentBySlotName( "Feet" );
	}

	static void SetupTrains()
    {
        string filePath = "$mission:trains-spawned.txt";
        if (FileExist(filePath))
        {
            Print("SpawnTrain skipped ...");
            return;
        }

        auto file = OpenFile(filePath, FileMode.WRITE);
        if (file)
        {
            FPrint(file, "Choo choo!");
            CloseFile(file);
        }

        Print("SpawnTrain loading ...");

        // Spawn train
        auto locomotive = HypeTrain_LocomotiveBase.Cast(HypeTrain_PartBase.CreateAt("HypeTrain_742_Locomotive_Red", "2498.584717 190.581711 5209.014648", "-97.901909 0.001385 0.000000"));
        locomotive.GetInventory().CreateAttachment("TruckBattery");
        locomotive.GetInventory().CreateAttachment("CarRadiator");
        locomotive.GetInventory().CreateAttachment("HeadlightH7");
        locomotive.GetInventory().CreateAttachment("HeadlightH7");
        locomotive.GetInventory().CreateAttachment("HypeTrain_742_Locomotive_EngineBelt");
        locomotive.GetInventory().CreateAttachment("HypeTrain_742_Locomotive_ControlUnit");
        locomotive.GetInventory().CreateAttachment("GlowPlug");
        locomotive.GetInventory().CreateAttachment("HypeTrain_742_Locomotive_CompressedAirHoses");
        locomotive.GetInventory().CreateAttachment("HypeTrain_742_Locomotive_Glass_Large");
        locomotive.GetInventory().CreateAttachment("HypeTrain_742_Locomotive_Glass_Large");
        locomotive.GetInventory().CreateAttachment("HypeTrain_742_Locomotive_Glass_Small");
        locomotive.GetInventory().CreateAttachment("HypeTrain_742_Locomotive_Glass_Small");
        locomotive.GetInventory().CreateAttachment("HypeTrain_742_Locomotive_Glass_Small");
        locomotive.GetInventory().CreateAttachment("HypeTrain_742_Locomotive_Glass_Small");
        locomotive.SetLiquidQuantity(locomotive.GetLiquidQuantityMax() * 0.95); // Refuel the locomotive to 95%

        auto wagon1 = HypeTrain_PartBase.CreateAt("HypeTrain_742_Wagon_Box", "2511.191162 190.581543 5210.760254", "-97.879120 0.000239 -0.000000");
        auto wagon2 = HypeTrain_PartBase.CreateAt("HypeTrain_742_Wagon_Tank", "2522.887207 190.581482 5212.378906", "-97.880173 0.000134 -0.000000");
        auto wagon3 = HypeTrain_PartBase.CreateAt("HypeTrain_742_Wagon_Flat", "2537.135742 190.582825 5214.351563", "-97.887115 -0.013224 -0.000000");

        TrainSimulation.s_Instance.ConnectTrainParts(locomotive, wagon1);
        TrainSimulation.s_Instance.ConnectTrainParts(wagon2, wagon1);
        TrainSimulation.s_Instance.ConnectTrainParts(wagon3, wagon2);
    }
};

Mission CreateCustomMission(string path)
{
	return new CustomMission();
}