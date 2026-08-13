-- |||||||||||||||||||||||||||||||||||||||||||||||||||||||||
-- Configurado por RDDLV - 97x Edition
-- |||||||||||||||||||||||||||||||||||||||||||||||||||||||||

require "System\\ScriptCore"


local SystemMain       = {};
local SystemConfigs    = {};

BridgeFunctionAttach("OnReadScript","evItemAuction_ReadScript");
BridgeFunctionAttach("OnTimerThread","evItemAuction_TimerThread");
BridgeFunctionAttach("OnCommandManager","evItemAuction_CommandManager");

function evItemAuction_ReadScript()
    SystemConfigs.Timer                     = {};               -- Hor�rio em que o evento acontecer�.
    SystemConfigs.Timer[1]                  = "21:12:00";       -- Primeiro hor�rio;    
    SystemConfigs.Timer[2]                  = "10:20:00";       -- Segundo hor�rio; 
    SystemConfigs.Timer[3]                  = "16:23:00";       -- Terceiro hor�rio;  	
    SystemConfigs.Timer[4]                  = "22:20:00";       -- Terceiro hor�rio;  
    SystemConfigs.Timer[5]                  = "10:20:00";       -- Terceiro hor�rio; 
    SystemConfigs.Timer[6]                  = "14:20:00";       -- Terceiro hor�rio; 	
 
    SystemConfigs.TimerMax                  = 5;                -- Tempo m�ximo, em minutos, para durar o leil�o caso ningu�m d� um lance.
    SystemConfigs.TimerOne                  = 10;               -- Tempo m�ximo, em segundos, para aparecer o dole 1.
    SystemConfigs.TimerTwo                  = 20;                -- Tempo m�ximo, em segundos, para aparecer o dole 2.
    SystemConfigs.TimerTree                 = 30;                -- Tempo m�ximo, em segundos, para aparecer o dole 3. [ VENDIDO! ]

    SystemConfigs.CoinName                  = "WCoinC"            -- Nome da moeda utilizada no evento.
    SystemConfigs.CoinType                  = 1;                -- Tipo da moeda utilizada no evento.
    
	
	
    SystemConfigs.nmCommandBuy              = "/lance";         -- Nome do comando utilizado para dar o lance.
    SystemConfigs.cdCommandBuy              = 92;              -- C�digo do comando utilizado para dar o lance.
    
    SystemConfigs.Items                     = {};

    SystemConfigs.Items[1]                  = {};
    SystemConfigs.Items[1]["Index"]         = 7342;
    SystemConfigs.Items[1]["Level"]         = 0;
    SystemConfigs.Items[1]["Durability"]    = 0;
    SystemConfigs.Items[1]["Option1"]       = 0;
    SystemConfigs.Items[1]["Option2"]       = 0;
    SystemConfigs.Items[1]["Option3"]       = 0;
    SystemConfigs.Items[1]["NewOption"]     = 0;
    SystemConfigs.Items[1]["SetOption"]     = 0;
    SystemConfigs.Items[1]["RewardMin"]     = 5;
    SystemConfigs.Items[1]["Description"]   = "1 BOX OF HORUS";
    
    SystemConfigs.Items[2]                  = {};
    SystemConfigs.Items[2]["Index"]         = 7342;
    SystemConfigs.Items[2]["Level"]         = 0;
    SystemConfigs.Items[2]["Durability"]    = 0;
    SystemConfigs.Items[2]["Option1"]       = 0;
    SystemConfigs.Items[2]["Option2"]       = 0;
    SystemConfigs.Items[2]["Option3"]       = 0;
    SystemConfigs.Items[2]["NewOption"]     = 0;
    SystemConfigs.Items[2]["SetOption"]     = 0;
    SystemConfigs.Items[2]["RewardMin"]     = 5;
    SystemConfigs.Items[2]["Description"]   = "1 BOX OF HORUS";
	
    SystemConfigs.Items[3]                  = {};
    SystemConfigs.Items[3]["Index"]         = 7342;
    SystemConfigs.Items[3]["Level"]         = 0;
    SystemConfigs.Items[3]["Durability"]    = 0;
    SystemConfigs.Items[3]["Option1"]       = 0;
    SystemConfigs.Items[3]["Option2"]       = 0;
    SystemConfigs.Items[3]["Option3"]       = 0;
    SystemConfigs.Items[3]["NewOption"]     = 0;
    SystemConfigs.Items[3]["SetOption"]     = 0;
    SystemConfigs.Items[3]["RewardMin"]     = 5;
    SystemConfigs.Items[3]["Description"]   = "1 BOX OF HORUS";
	
	SystemConfigs.Items[4]                  = {};
    SystemConfigs.Items[4]["Index"]         = 7342;
    SystemConfigs.Items[4]["Level"]         = 0;
    SystemConfigs.Items[4]["Durability"]    = 0;
    SystemConfigs.Items[4]["Option1"]       = 0;
    SystemConfigs.Items[4]["Option2"]       = 0;
    SystemConfigs.Items[4]["Option3"]       = 0;
    SystemConfigs.Items[4]["NewOption"]     = 0;
    SystemConfigs.Items[4]["SetOption"]     = 0;
    SystemConfigs.Items[4]["RewardMin"]     = 5;
    SystemConfigs.Items[4]["Description"]   = "1 BOX OF HORUS";
	
	SystemConfigs.Items[5]                  = {};
    SystemConfigs.Items[5]["Index"]         = 7342;
    SystemConfigs.Items[5]["Level"]         = 0;
    SystemConfigs.Items[5]["Durability"]    = 0;
    SystemConfigs.Items[5]["Option1"]       = 0;
    SystemConfigs.Items[5]["Option2"]       = 0;
    SystemConfigs.Items[5]["Option3"]       = 0;
    SystemConfigs.Items[5]["NewOption"]     = 0;
    SystemConfigs.Items[5]["SetOption"]     = 0;
    SystemConfigs.Items[5]["RewardMin"]     = 5;
    SystemConfigs.Items[5]["Description"]   = "1 BOX OF HORUS";
	
    SystemConfigs.Items[6]                  = {};
    SystemConfigs.Items[6]["Index"]         = 7342;
    SystemConfigs.Items[6]["Level"]         = 0;
    SystemConfigs.Items[6]["Durability"]    = 0;
    SystemConfigs.Items[6]["Option1"]       = 0;
    SystemConfigs.Items[6]["Option2"]       = 0;
    SystemConfigs.Items[6]["Option3"]       = 0;
    SystemConfigs.Items[6]["NewOption"]     = 0;
    SystemConfigs.Items[6]["SetOption"]     = 0;
    SystemConfigs.Items[6]["RewardMin"]     = 5;
    SystemConfigs.Items[6]["Description"]   = "1 BOX OF HORUS";
end

function evItemAuction_TimerThread()
    if (SystemMain.Stage == nil) then
        for z = 1, #SystemConfigs.Timer, 1 do
            if (os.date("%X") == SystemConfigs.Timer[z]) then
                SystemMain.Count            = 0;
                SystemMain.SelectedUser     = 0;
                SystemMain.Stage            = 1;
                SystemMain.TimerMax         = SystemConfigs.TimerMax;
                SystemMain.Randomize        = math.random(1, #SystemConfigs.Items);
                SystemMain.Randomize        = math.random(1, 9478);
                SystemMain.Randomize        = math.random(9478, 18956);
                SystemMain.Randomize        = math.random(1, #SystemConfigs.Items);
                SystemMain.Randomize        = math.random(1, #SystemConfigs.Items);
                SystemMain.SelectedPayment  = SystemConfigs.Items[SystemMain.Randomize]["RewardMin"];
                
                LogPrint("[LEIL�O_DE_ITENS]: Evento iniciado.");
            end
        end
        return;
    elseif (SystemMain.Stage == 1) then
        if (SystemMain.Count%60 == 0) then
            if (SystemMain.TimerMax == 0) then
                SystemMain.Stage            = nil;
                SystemMain.Count            = nil;
                SystemMain.TimerMax         = nil;
                SystemMain.Randomize        = nil;
                SystemMain.SelectedUser     = nil;
                SystemMain.SelectedPayment  = nil;

                LogPrint("[LEIL�O_DE_ITENS]: Encerrado. Nenhum jogador comprou o item.");
            else
                NoticeSendToAll(0, ". : [ LEIL�O DE ITENS ] : .");
                NoticeSendToAll(0, "Item: "..SystemConfigs.Items[SystemMain.Randomize]["Description"]..".");
                NoticeSendToAll(0, "Lance m�nimo: "..SystemMain.SelectedPayment.." "..SystemConfigs.CoinName..". Digite '"..SystemConfigs.nmCommandBuy.." <quantidade>' para cobrir..");
                NoticeSendToAll(1, "[Leil�o]: Fechar� em "..SystemMain.TimerMax.." minuto(s).");

                SystemMain.Count    = SystemMain.Count + 1; 
                SystemMain.TimerMax = SystemMain.TimerMax - 1;
            end
        else
            SystemMain.Count = SystemMain.Count + 1; 
        end
    elseif (SystemMain.Stage == 2) then
        if (SystemMain.Count == SystemConfigs.TimerOne) then
            NoticeSendToAll(0, ". : [ DOLE 1 ] : .");
            NoticeSendToAll(0, "Item: "..SystemConfigs.Items[SystemMain.Randomize]["Description"]..".");
            NoticeSendToAll(0, "Lance predominante ("..GetObjectName(SystemMain.SelectedUser).."): "..SystemMain.SelectedPayment.." "..SystemConfigs.CoinName..". Digite '"..SystemConfigs.nmCommandBuy.."' para cobrir..");

            SystemMain.Count = SystemMain.Count + 1; 
        elseif (SystemMain.Count == SystemConfigs.TimerTwo) then
            NoticeSendToAll(0, ". : [ DOLE 2 ] : .");
            NoticeSendToAll(0, "Item: "..SystemConfigs.Items[SystemMain.Randomize]["Description"]..".");
            NoticeSendToAll(0, "Lance predominante ("..GetObjectName(SystemMain.SelectedUser).."): "..SystemMain.SelectedPayment.." "..SystemConfigs.CoinName..". Digite '"..SystemConfigs.nmCommandBuy.."' para cobrir..");
            
            SystemMain.Count = SystemMain.Count + 1; 
        elseif (SystemMain.Count == SystemConfigs.TimerTree) then
            if (GetObjectConnected(SystemMain.SelectedUser) == 3) then 
                NoticeSendToAll(0, ". : [ LEIL�O DE ITENS ] : .");
                NoticeSendToAll(0, "Item: "..SystemConfigs.Items[SystemMain.Randomize]["Description"]..".");
                NoticeSendToAll(0, "Vendido para: "..GetObjectName(SystemMain.SelectedUser).." por "..SystemMain.SelectedPayment.." "..SystemConfigs.CoinName..".");

                ItemGiveEx(SystemMain.SelectedUser,SystemConfigs.Items[SystemMain.Randomize]["Index"],SystemConfigs.Items[SystemMain.Randomize]["Level"],SystemConfigs.Items[SystemMain.Randomize]["Durability"],SystemConfigs.Items[SystemMain.Randomize]["Option1"],SystemConfigs.Items[SystemMain.Randomize]["Option2"],SystemConfigs.Items[SystemMain.Randomize]["Option3"],SystemConfigs.Items[SystemMain.Randomize]["NewOption"],SystemConfigs.Items[SystemMain.Randomize]["SetOption"]);
                LogPrint("[LEIL�O_DE_ITENS]: Item: "..SystemConfigs.Items[SystemMain.Randomize]["Description"]..". Vendido para: "..GetObjectName(SystemMain.SelectedUser).." por "..SystemMain.SelectedPayment.." "..SystemConfigs.CoinName..".");
                
                SystemMain.Stage            = nil;
                SystemMain.Count            = nil;
                SystemMain.TimerMax         = nil;
                SystemMain.Randomize        = nil;
                SystemMain.SelectedUser     = nil;
                SystemMain.SelectedPayment  = nil;
            else
                NoticeSendToAll(0, ". : [ LEIL�O DE ITENS ] : .");
                NoticeSendToAll(0, "O player predominante saiu do jogo e n�o houve novos lances, leil�o encerrado.");

                SystemMain.Stage            = nil;
                SystemMain.Count            = nil;
                SystemMain.TimerMax         = nil;
                SystemMain.Randomize        = nil;
                SystemMain.SelectedUser     = nil;
                SystemMain.SelectedPayment  = nil;
                
				LogPrint("[LEIL�O_DE_ITENS]: O player predominante saiu do jogo e n�o houve novos lances, leil�o encerrado.");
            end
        else
            SystemMain.Count = SystemMain.Count + 1; 
        end
    end
end

function evItemAuction_CommandManager(aIndex, code, arg)
    if (code == SystemConfigs.cdCommandBuy) then
        local iValue = tonumber(CommandGetArgString(arg,0));
        if (SystemMain.Stage ~= nil) then
            if (SystemMain.Count < SystemConfigs.TimerOne) and (SystemMain.Stage == 2) then
                NoticeSend(aIndex, 1, "[Leil�o]: Aguarde alguns segundos para dar o pr�ximo lance.");          
            elseif (iValue == nil) then
                NoticeSend(aIndex, 1, "[Leil�o]: Valor inserido inv�lido. ");            
            elseif (iValue > SystemMain.SelectedPayment) then
                if (SystemMain.Buy(aIndex, iValue) ~= 0) then 
                    NoticeSendToAll(0, ". : [ LEIL�O DE ITENS ] : .");
                    NoticeSendToAll(0, "Item: "..SystemConfigs.Items[SystemMain.Randomize]["Description"]..".");
                    NoticeSendToAll(0, GetObjectName(SystemMain.SelectedUser).." deu um lance no item de "..SystemMain.SelectedPayment.." "..SystemConfigs.CoinName..".");
                else
                    NoticeSend(aIndex, 1, "[Leil�o]: Saldo insuficiente para cobrir o lance.");
                end
            else
                NoticeSend(aIndex, 1, "[Leil�o]: Valor igual ou abaixo do lance atual. ");
            end
        else
            NoticeSend(aIndex, 1, "[Leil�o]: Evento indispon�vel no momento.");
        end
        return 1;
    end
    return 0;
end

function SystemMain.Buy(aIndex, sMessage)

    SQLQuery("SELECT WCoinC FROM CashShopData WHERE AccountID = '"..GetObjectAccount(aIndex).."'");
    SQLFetch();
    local iValue = tonumber(SQLGetNumber("WCoinC"));
    SQLClose();

    if (iValue > SystemMain.SelectedPayment) and (iValue >= sMessage) then
        if (SystemMain.SelectedUser ~= 0) then 
            SQLQuery("UPDATE CashShopData SET WCoinC = WCoinC +"..SystemMain.SelectedPayment.." WHERE AccountID = '"..GetObjectAccount(SystemMain.SelectedUser).."'");				
            SQLClose();

            NoticeSend(SystemMain.SelectedUser, 1, "[Lei�o]: "..GetObjectName(aIndex).." cobriu seu lance com "..sMessage.." "..SystemConfigs.CoinName..".");
            LogPrint("[LEIL�O_DE_ITENS]: "..GetObjectName(aIndex).." cobriu o lance de "..GetObjectName(SystemMain.SelectedUser).." com "..sMessage.." "..SystemConfigs.CoinName..".");
        end

        SQLQuery("UPDATE CashShopData SET WCoinC  = WCoinC  -"..sMessage.." WHERE AccountID = '"..GetObjectAccount(aIndex).."'");
        SQLClose();
        NoticeSend(aIndex, 1, "[Leil�o]: Voc� deu um lance de "..sMessage.." "..SystemConfigs.CoinName.." pelo item.");

        SystemMain.Count            = 0;
        SystemMain.SelectedUser     = aIndex;
        SystemMain.SelectedPayment  = sMessage;

        if (SystemMain.Stage == 1) then SystemMain.Stage = 2 end;
        return 1;
    else
        return 0;
    end
end

LogColor(1, "[Script_ItemAuction] Carregado Com Sucesso!")