-- |||||||||||||||||||||||||||||||||||||||||||||||||||||||||
-- Configurado por RDDLV - 97x Edition
-- |||||||||||||||||||||||||||||||||||||||||||||||||||||||||

require "System\\ScriptCore"

BridgeFunctionAttach("OnCommandManagerDirect", "CommandDirect");

-- Retorna 1 para cancelar a execu��o no GS
-- Caso contr�rio ir� continuar a execu��o do OnCommandManager abaixo

CommandDirect = function(aIndex, str)		
	local comando = CommandGetArgString(str, 0)
		
	if comando == "/novo" then 
		SendTimeLimit(aIndex, 15, "Evento P. DEV !")
		NoticeSend(aIndex, 0, "Permite a cria��o de novos comandos diretos, sem passar pela command.txt")
		return 1
	end

	return 0;
end

LogColor(1, "[Script_CommandDirect] Carregado Com Sucesso!")