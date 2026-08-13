-- |||||||||||||||||||||||||||||||||||||||||||||||||||||||||
-- Configurado por RDDLV - 97x Edition
-- |||||||||||||||||||||||||||||||||||||||||||||||||||||||||

require "System\\ScriptCore"

BridgeFunctionAttach("OnCommandManager", "CommandManager");

-- Retorna 1 para cancelar a execu��o no GS
-- Caso contr�rio ir� continuar a execu��o do OnCommandManager abaixo
-- arg � o texto enviado no jogo ou parametro ap�s o comando

CommandManager = function(aIndex,code,arg)		
	-- Exemplo: Comando /teste <arg>
	if code == 131 then	
		argumento = CommandGetArgString(arg, 0)
		
		PostSend(0, 10, GetObjectName(aIndex), argumento)
	end
	
	return 0
end

LogColor(1, "[Script_Command] Carregado Com Sucesso!")