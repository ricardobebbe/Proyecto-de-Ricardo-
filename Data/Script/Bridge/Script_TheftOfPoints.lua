require "System\\ScriptCore"

BridgeFunctionAttach("OnReadScript","evToP_ReadScript");
BridgeFunctionAttach("OnTimerThread","evToP_TimerThread");
BridgeFunctionAttach("OnCommandManager","evToP_CommandManager");
BridgeFunctionAttach("OnCheckUserTarget","evToP_CheckTarget");
BridgeFunctionAttach("OnUserDie","evToP_UserDie");
BridgeFunctionAttach("OnUserRespawn","evToP_UserRespawn");
BridgeFunctionAttach("OnCharacterClose","evToP_CharacterClose");

local SYSTEM_MAIN    = {};
local SYSTEM_CONFIGS = {};

function evToP_ReadScript()
    SYSTEM_CONFIGS.Schedule             = {};               -- Horários em que o evento irá ocorrer;
    SYSTEM_CONFIGS.Schedule[1]          = "20:50:00";      
    SYSTEM_CONFIGS.Schedule[2]          = "12:15:00";    
    SYSTEM_CONFIGS.Schedule[3]          = "16:12:00";    	
    SYSTEM_CONFIGS.Schedule[4]          = "18:15:00"; 
    SYSTEM_CONFIGS.Schedule[5]          = "21:15:00";                                                           	

    SYSTEM_CONFIGS.Map                 	= 6;            	-- Número do mapa onde o evento ocorrerá;
    SYSTEM_CONFIGS.CordXY              	= {63, 157};    	-- Coordenadas do ponto inicial do evento;

    SYSTEM_CONFIGS.CordRespawnX 		= {};         		-- Coordenadas X dos pontos de respawn;
    SYSTEM_CONFIGS.CordRespawnY     	= {};        		-- Coordenadas Y dos pontos de respawn;
	
	SYSTEM_CONFIGS.CordRespawnX[1],SYSTEM_CONFIGS.CordRespawnY[1] = 58,144;
	SYSTEM_CONFIGS.CordRespawnX[2],SYSTEM_CONFIGS.CordRespawnY[2] = 68,144;
	SYSTEM_CONFIGS.CordRespawnX[3],SYSTEM_CONFIGS.CordRespawnY[3] = 63,149;
	SYSTEM_CONFIGS.CordRespawnX[4],SYSTEM_CONFIGS.CordRespawnY[4] = 63,161;
	SYSTEM_CONFIGS.CordRespawnX[5],SYSTEM_CONFIGS.CordRespawnY[5] = 58,175;
	SYSTEM_CONFIGS.CordRespawnX[6],SYSTEM_CONFIGS.CordRespawnY[6] = 68,176;

    SYSTEM_CONFIGS.TimerAlert       	= 2;                -- Tempo de alerta para entrada no evento;
    SYSTEM_CONFIGS.TimerLimit       	= 2;                -- Tempo de duração da fase de batalha;

    SYSTEM_CONFIGS.SecsRanking      	= 20;               -- Tempo em segundos para mostrar os 3 primeiros em pontuação;
    SYSTEM_CONFIGS.SecsPoints       	= 15;               -- Tempo em segundos para somar a quantidade de pontos abaixo definida, consecutivamente;
    SYSTEM_CONFIGS.AddPoints        	= 5;                -- Quantidade de pontos atribuída;
    
    SYSTEM_CONFIGS.MaxPlayers       	= 25;               -- Número máximo de players;
    SYSTEM_CONFIGS.MinPlayers       	= 2;                -- Número mínimo de players;
    
    SYSTEM_CONFIGS.cdCommandEntry   	= 141;              -- Código do comando para entrar;
    SYSTEM_CONFIGS.nmCommandEntry   	= "/irevento";     	-- Nome do comando utilizado para entrar;

    SYSTEM_CONFIGS.ValueReward      	= 1;              	-- Valor do pagamento;
    SYSTEM_CONFIGS.NameReward       	= "Leilão Points";  -- Nome da moeda;    
	
	LogPrint("[THEFT_OF_POINTS]: Configurações armazenadas.");
end

function evToP_TimerThread()
    if (SYSTEM_MAIN.Stage == nil) then
        for x = 1, #SYSTEM_CONFIGS.Schedule do
            if (os.date("%X") == SYSTEM_CONFIGS.Schedule[x]) then   
                SYSTEM_MAIN.Count       = 0;
                SYSTEM_MAIN.Stage       = 1;
                SYSTEM_MAIN.UserCount   = 1;
                SYSTEM_MAIN.Players     = {};
                SYSTEM_MAIN.TimerAlert  = SYSTEM_CONFIGS.TimerAlert;

				LogPrint("[THEFT_OF_POINTS]: Evento iniciado.");			
                break; 
            end
        end    
    elseif (SYSTEM_MAIN.Stage == 1) then
        if (SYSTEM_MAIN.Count%60 == 0) then
            if (SYSTEM_MAIN.TimerAlert == 0) then
                if (#SYSTEM_MAIN.Players >= SYSTEM_CONFIGS.MinPlayers) then
                    NoticeSendToAll(0, ". : [ THEFT OF POINTS ] : .");
                    NoticeSendToAll(0, "Tempo esgotado. Evento iniciado.");
                    
                    SYSTEM_MAIN.Count       = 1;
                    SYSTEM_MAIN.Stage       = 2;
                    SYSTEM_MAIN.TimerAlert  = nil;
                    SYSTEM_MAIN.UserCount   = nil;
                    SYSTEM_MAIN.TimerLimit  = SYSTEM_CONFIGS.TimerLimit - 1;
                
                    LogPrint("[THEFT_OF_POINTS]: Tempo esgotado, evento iniciado.");
                else
                    NoticeSendToAll(0, ". : [ THEFT OF POINTS ] : .");
                    NoticeSendToAll(0, "Evento encerrado. Mínimo de participantes não atingido.");

                    for x = #SYSTEM_MAIN.Players, 1, -1 do
                        MoveUserEx(GetObjectIndexByName(SYSTEM_MAIN.Players[x]["Name"]), 0, 125, 125);
                        PermissionInsert(GetObjectIndexByName(SYSTEM_MAIN.Players[x]["Name"]), 12);
                        table.remove(SYSTEM_MAIN.Players, x);
                    end

                    SYSTEM_MAIN.Count       = nil;
                    SYSTEM_MAIN.Stage       = nil;
                    SYSTEM_MAIN.TimerAlert  = nil;
                    SYSTEM_MAIN.Players     = nil;

                    LogPrint("[THEFT_OF_POINTS]: Mínimo de players não atingido, evento cancelado.");
                end
            else
                NoticeSendToAll(0, ". : [ THEFT OF POINTS ] : .");
                NoticeSendToAll(0, "O evento iniciará em "..SYSTEM_MAIN.TimerAlert.. " minuto(s).");
                NoticeSendToAll(0, "Digite "..SYSTEM_CONFIGS.nmCommandEntry.." para participar.");

                SYSTEM_MAIN.TimerAlert = SYSTEM_MAIN.TimerAlert - 1;
                SYSTEM_MAIN.Count = SYSTEM_MAIN.Count + 1;
            end
        else
            SYSTEM_MAIN.Count = SYSTEM_MAIN.Count + 1;
        end
    elseif (SYSTEM_MAIN.Stage == 2) then
        if (SYSTEM_MAIN.Count%60 == 0) then
            if (SYSTEM_MAIN.TimerLimit == 0) then
                local T1 = SYSTEM_MAIN.Ranking();

                NoticeSendToAll(0, ". : [ THEFT OF POINTS ] : . ");
                NoticeSendToAll(0, "Ganhador(a): "..T1.Name[1]..", com "..T1.Score[1].." pontos.");
                NoticeSendToAll(0, "Obrigado a todos os participantes.");
                NoticeSend(GetObjectIndexByName(T1.Name[1]), 1, "[ToP]: Você ganhou o evento e recebeu "..SYSTEM_CONFIGS.ValueReward.." "..SYSTEM_CONFIGS.NameReward..".");

                SYSTEM_MAIN.RewardS4(GetObjectIndexByName(T1.Name[1]));

                for x = #SYSTEM_MAIN.Players, 1, -1 do
                    MoveUserEx(GetObjectIndexByName(SYSTEM_MAIN.Players[x]["Name"]), 0, 125, 125);
                    PermissionInsert(GetObjectIndexByName(SYSTEM_MAIN.Players[x]["Name"]), 12);
                    table.remove(SYSTEM_MAIN.Players, x);
                end
                
                SYSTEM_MAIN.Count       = nil;
                SYSTEM_MAIN.Stage       = nil;
                SYSTEM_MAIN.TimerLimit  = nil;
                SYSTEM_MAIN.Players     = nil;

                LogPrint("[THEFT_OF_POINTS]: Ganhador(a): "..T1.Name[1]..", com "..T1.Score[1].." pontos. Evento encerrado.");
            else
                NoticeSendToAll(0, ". : [ THEFT OF POINTS ] : .");
                NoticeSendToAll(0, "O evento encerrará em "..SYSTEM_MAIN.TimerLimit.. " minuto(s).");

                SYSTEM_MAIN.TimerLimit = SYSTEM_MAIN.TimerLimit - 1;
                SYSTEM_MAIN.Count = SYSTEM_MAIN.Count + 1;
            end
        elseif (SYSTEM_MAIN.Count%SYSTEM_CONFIGS.SecsPoints == 0) then
            for x = 1, #SYSTEM_MAIN.Players, 1 do
                SYSTEM_MAIN.Players[x]["Points"] = SYSTEM_MAIN.Players[x]["Points"] + SYSTEM_CONFIGS.AddPoints;
        
                NoticeSend(GetObjectIndexByName(SYSTEM_MAIN.Players[x]["Name"]), 1, "[SYSTEM]: Você ganhou "..SYSTEM_CONFIGS.AddPoints.." pontos. TOTAL: "..SYSTEM_MAIN.Players[x]["Points"].." pontos.");
            end
    
            SYSTEM_MAIN.Count = SYSTEM_MAIN.Count + 1;
    
        elseif (SYSTEM_MAIN.Count%SYSTEM_CONFIGS.SecsRanking == 0) then
            local T1 = SYSTEM_MAIN.Ranking();

            for x = 1, #SYSTEM_MAIN.Players, 1 do
                NoticeSend(GetObjectIndexByName(SYSTEM_MAIN.Players[x]["Name"]), 0, ". : [ RANKING ] : .");
                NoticeSend(GetObjectIndexByName(SYSTEM_MAIN.Players[x]["Name"]), 0, "1º - "..T1.Name[1].."("..T1.Score[1].."), 2º - "..T1.Name[2].."("..T1.Score[2].."), 3º - "..T1.Name[3].."("..T1.Score[3]..").");
            end

            SYSTEM_MAIN.Count = SYSTEM_MAIN.Count + 1;
        else
            SYSTEM_MAIN.Count = SYSTEM_MAIN.Count + 1;
        end
    end 
end

function evToP_CommandManager(aIndex, code, arg)
    if (SYSTEM_MAIN.Stage ~= nil) then
        if (SYSTEM_MAIN.Stage == 1) then
            if (SYSTEM_CONFIGS.cdCommandEntry == code) then
                if (SYSTEM_MAIN.GetUser(aIndex) ~= 0) then
                    NoticeSend(aIndex, 1, "[ToP]: Você já está participando deste evento.");
                else
                    SYSTEM_MAIN.Players[SYSTEM_MAIN.UserCount]           	= {};
                    SYSTEM_MAIN.Players[SYSTEM_MAIN.UserCount]["Name"]   	= GetObjectName(aIndex);
                    SYSTEM_MAIN.Players[SYSTEM_MAIN.UserCount]["Points"] 	= 0;
                    SYSTEM_MAIN.UserCount                                   = SYSTEM_MAIN.UserCount + 1;

                    NoticeSend(aIndex, 0, "Você foi inscrito no evento, não saia ou perderá a vaga.");
                    NoticeSend(aIndex, 1, "[ToP]: Você foi inscrito no evento, não saia ou perderá a vaga.");

                    SYSTEM_MAIN.MoveUser(aIndex);

                    if (#SYSTEM_MAIN.Players == SYSTEM_CONFIGS.MaxPlayers) then
                        NoticeSendToAll(0, ". : [ THEFT OF POINTS ] : .");
                        NoticeSendToAll(0, "Máximo de participantes atingido. Evento iniciado.");

                        SYSTEM_MAIN.Count       = 1;
                        SYSTEM_MAIN.Stage       = 2;
                        SYSTEM_MAIN.TimerAlert  = nil;
                        SYSTEM_MAIN.UserCount   = nil;
                        SYSTEM_MAIN.TimerLimit  = SYSTEM_CONFIGS.TimerLimit - 1;

                        LogPrint("[THEFT_OF_POINTS]: Máximo de participantes atingido. Evento iniciado.");
                    end
                end

                return 1;
            end
        end
    end

    return 0;
end

function evToP_CheckTarget(aIndex, bIndex)
    if (SYSTEM_MAIN.Stage ~= nil) then
        if (SYSTEM_MAIN.Stage == 1) then
            if (SYSTEM_MAIN.GetUser(aIndex) ~= 0) and (SYSTEM_MAIN.GetUser(bIndex) ~= 0) then
                return 0;
            end
        else    
            if (SYSTEM_MAIN.GetUser(aIndex) ~= 0) and (SYSTEM_MAIN.GetUser(bIndex) ~= 0) then
                return 1;
            end
        end
    end

    return 1;
end

function evToP_UserDie(aIndex, bIndex)
    if (SYSTEM_MAIN.Stage ~= nil) then
        if (SYSTEM_MAIN.Stage == 2) then
            local x = SYSTEM_MAIN.GetUser(aIndex);
            local y = SYSTEM_MAIN.GetUser(bIndex);

            if (x ~= 0) and (y ~= 0) then
                SYSTEM_MAIN.Players[y]["Points"] = SYSTEM_MAIN.Players[y]["Points"] +  SYSTEM_MAIN.Players[x]["Points"];
                NoticeSend(bIndex, 1,"[ToP]: Você roubou "..SYSTEM_MAIN.Players[x]["Points"].." pontos. TOTAL: "..SYSTEM_MAIN.Players[y]["Points"].." pontos.");

                SYSTEM_MAIN.Players[x]["Points"] = 0;
                NoticeSend(aIndex, 1,"[ToP]: Você morreu e teve seus pontos roubados.");
            end
        end
    end
end

function evToP_UserRespawn(aIndex, KillerType)	
    if (SYSTEM_MAIN.Stage ~= nil) then
        if (SYSTEM_MAIN.GetUser(aIndex) ~= 0) then
            SYSTEM_MAIN.MoveUser(aIndex);			
        end
    end
end

function evToP_CharacterClose(aIndex)
    if (SYSTEM_MAIN.Stage ~= nil) then
        if (SYSTEM_MAIN.Stage == 1) or (SYSTEM_MAIN.Stage == 2) then
            local x = SYSTEM_MAIN.GetUser(aIndex);

            if (x ~= 0) then
                NoticeSendToAll(1, "[ToP]: "..GetObjectName(aIndex).." desconectou-se e foi removido do evento.");
                MoveUserEx(aIndex, 0, 125, 125);

                table.remove(SYSTEM_MAIN.Players, x);
                if (SYSTEM_MAIN.Stage == 1) then SYSTEM_MAIN.UserCount = SYSTEM_MAIN.UserCount - 1 end;
            end
        end
    end
end

function SYSTEM_MAIN.GetUser(aIndex)
    for n = 1, #SYSTEM_MAIN.Players, 1 do
        if (SYSTEM_MAIN.Players[n]["Name"] == GetObjectName(aIndex)) then
            return n;
        end
    end

    return 0;
end

function SYSTEM_MAIN.MoveUser(aIndex)
    if (SYSTEM_MAIN.Stage == 1) then
        MoveUserEx(aIndex, SYSTEM_CONFIGS.Map, SYSTEM_CONFIGS.CordXY[1], SYSTEM_CONFIGS.CordXY[2]);
        PermissionRemove(aIndex, 12);
    else
        local aleatory = math.random(1, #SYSTEM_CONFIGS.CordRespawnX);
		
        MoveUserEx(aIndex, SYSTEM_CONFIGS.Map, SYSTEM_CONFIGS.CordRespawnX[aleatory], SYSTEM_CONFIGS.CordRespawnY[aleatory]);
        aleatory = math.random(1, #SYSTEM_CONFIGS.CordRespawnX);
    end
end

function  SYSTEM_MAIN.Ranking()
    local T1  = {
        Name  = {"","",""},
        Score = {0,0,0}
    }

    for n = 1, #SYSTEM_MAIN.Players, 1 do

        if (SYSTEM_MAIN.Players[n]["Points"] >= T1.Score[1]) then

			T1.Score[3] = T1.Score[2];
            T1.Score[2] = T1.Score[1];
            T1.Score[1] = SYSTEM_MAIN.Players[n]["Points"];

            T1.Name[3]  = T1.Name[2];
            T1.Name[2]  = T1.Name[1];
            T1.Name[1]  = SYSTEM_MAIN.Players[n]["Name"];
            
		elseif (SYSTEM_MAIN.Players[n]["Points"] >= T1.Score[2]) then

            T1.Score[3] = T1.Score[2];
            T1.Score[2] = SYSTEM_MAIN.Players[n]["Points"];

            T1.Name[3]  = T1.Name[2];
            T1.Name[2]  = SYSTEM_MAIN.Players[n]["Name"];

		elseif (SYSTEM_MAIN.Players[n]["Points"] >= T1.Score[3]) then

            T1.Score[3] = SYSTEM_MAIN.Players[n]["Points"];

            T1.Name[3]  = SYSTEM_MAIN.Players[n]["Name"];
        end
    end  

    return T1;
end

function SYSTEM_MAIN.RewardS4(aIndex) 
    SQLQuery("UPDATE MEMB_INFO SET leilao = leilao +"..SYSTEM_CONFIGS.ValueReward.." WHERE memb___id = '"..GetObjectAccount(aIndex).."'");
    SQLClose();
end

LogColor(1, "[Script_TheftOfPoints] Loaded!")