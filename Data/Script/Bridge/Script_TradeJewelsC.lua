-- |||||||||||||||||||||||||||||||||||||||||||||||||||||||||
-- Configurado por RDDLV - 97x Edition
-- |||||||||||||||||||||||||||||||||||||||||||||||||||||||||

require "System\\ScriptCore"



SystemMain      = {};
SystemConfigs   = {};

BridgeFunctionAttach("OnReadScript","evTradeJewels_ReadScript");

function evTradeJewels_ReadScript()
    SystemConfigs.cdCommandTrade        = 136;

    SystemConfigs.RewardName            = "AuctionCoins";

    SystemConfigs.Items                 = {};

    SystemConfigs.Items[1]              = {};
    SystemConfigs.Items[1]["Index"]     = 7190;
    SystemConfigs.Items[1]["Qntd"]      = 5;
    SystemConfigs.Items[1]["Coin"]      = 1;
    SystemConfigs.Items[1]["Name"]      = "Creation";
    
    SystemConfigs.Items[2]              = {};
    SystemConfigs.Items[2]["Index"]     = 7184;
    SystemConfigs.Items[2]["Qntd"]      = 5;
    SystemConfigs.Items[2]["Coin"]      = 1;
    SystemConfigs.Items[2]["Name"]      = "Life";
	
	SystemConfigs.Items[3]              = {};
    SystemConfigs.Items[3]["Index"]     = 7181;
    SystemConfigs.Items[3]["Qntd"]      = 10;
    SystemConfigs.Items[3]["Coin"]      = 1;
    SystemConfigs.Items[3]["Name"]      = "Bless";
	
	SystemConfigs.Items[4]              = {};
    SystemConfigs.Items[4]["Index"]     = 7182;
    SystemConfigs.Items[4]["Qntd"]      = 10;
    SystemConfigs.Items[4]["Coin"]      = 1;
    SystemConfigs.Items[4]["Name"]      = "Soul";
end

BridgeFunctionAttach("OnCommandManager","evTradeJewels_CommandManager");

function evTradeJewels_CommandManager(aIndex, code, arg)
    if (SystemConfigs.cdCommandTrade == code) then
        local a = CommandGetArgString(arg, 0);
        local b = CommandGetArgNumber(arg, 1);

		if (b > 0) then
			for z = 1, #SystemConfigs.Items, 1 do	
				if (string.upper(SystemConfigs.Items[z]["Name"]) == string.upper(a)) then	
					local c = InventoryGetItemCount(aIndex, SystemConfigs.Items[z]["Index"], -1);
					
					if (c >= b) then						
						local d = 0;

                        if b%SystemConfigs.Items[z]["Qntd"] == 0 then
						    for x = 12, 107, 1 do                            
						    	if (InventoryGetItemIndex(aIndex, x) == SystemConfigs.Items[z]["Index"]) then                                
						    		InventoryDelItemIndex(aIndex,x);
						    		d = d + 1;
                                
						    		if (d == b) then                                    
						    			break;                                    
						    		end                                
						    	end                          
						    end
                        
						    SystemMain.Reward(aIndex, SystemConfigs.Items[z]["Coin"]*(b/SystemConfigs.Items[z]["Qntd"]));
						    NoticeSend(aIndex, 1, "[TradeJewels]: Troca aceita. Voc� recebeu "..(b/SystemConfigs.Items[z]["Qntd"]).." "..SystemConfigs.RewardName..".");
                        else
                            NoticeSend(aIndex, 1, string.format("[TradeJewels]: Troca recusada, o valor deve ser divisor de %d.", SystemConfigs.Items[z]["Qntd"]));
                        end                     
					else
						NoticeSend(aIndex, 1, "[TradeJewels]: Troca recusada. Quantidade de j�ias insuficiente.");
					end	
					break;	
				elseif (z == #SystemConfigs.Items) then	
					NoticeSend(aIndex, 1, "[TradeJewels]: Troca recusada. Nome inv�lido.");	
				end	
			end
		else
			NoticeSend(aIndex, 1, "[TradeJewels]: Voc� precisa digitar um n�mero de j�ias v�lido.");
		end
        return 1;
    end
    return 0;
end

function SystemMain.Reward(aIndex, aValue) 
	SQLQuery("SELECT leilao FROM MEMB_INFO WHERE memb___id = '"..GetObjectAccount(aIndex).."'");
	SQLFetch();
	local bValue = tonumber(SQLGetNumber("leilao"))+aValue;
	SQLClose();

    SQLQuery("UPDATE MEMB_INFO SET leilao = "..bValue.." WHERE memb___id = '"..GetObjectAccount(aIndex).."'");
    SQLClose();
end

LogColor(1, "[Script_TradeJewelsC] Carregado Com Sucesso!")