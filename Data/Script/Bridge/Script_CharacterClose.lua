-- |||||||||||||||||||||||||||||||||||||||||||||||||||||||||
-- Configurado por RDDLV - 97x Edition
-- |||||||||||||||||||||||||||||||||||||||||||||||||||||||||

require "System\\ScriptCore"

BridgeFunctionAttach("OnCharacterClose", "CharacterClose");
--AQUI  NOTICIA ASIM QUE PLAYER DESLOGAR DA CONTA BY: ZEUS REVOLUTIONS 
CharacterClose = function(aIndex)		
	local UserName = GetObjectName(aIndex)
	if UserName ~= "" then NoticeSend(aIndex,0, string.format("Saiu do char %s", UserName)) end
end

LogColor(1, "[Script_CharacterClose] Carregado Com Sucesso!")