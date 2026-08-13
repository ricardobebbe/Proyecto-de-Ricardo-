-- |||||||||||||||||||||||||||||||||||||||||||||||||||||||||
-- Configurado por RDDLV - 97x Edition
-- |||||||||||||||||||||||||||||||||||||||||||||||||||||||||

require "System\\ScriptCore"

BridgeFunctionAttach("OnCommandManager", "Coin_CommandManager");

Coin_CommandManager = function(aIndex, code, arg)	

	if code == 130 then
		SQLQuery("SELECT leilao FROM MEMB_INFO WHERE memb___id = '"..GetObjectAccount(aIndex).."'");
		SQLFetch();
		local aValue = tonumber(SQLGetNumber("leilao"));
		SQLClose();
	
		NoticeSend(aIndex, 1, string.format("[Leil�o]: Voc� possui %d moedas de Leil�o.", aValue));
		return 1;
		
	end

	return 0;
end

LogColor(1, "[Script_CommandCoin] Carregado Com Sucesso!")