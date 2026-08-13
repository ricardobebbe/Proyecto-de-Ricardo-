-- |||||||||||||||||||||||||||||||||||||||||||||||||||||||||
-- Configurado por RDDLV - 97x Edition
-- |||||||||||||||||||||||||||||||||||||||||||||||||||||||||

require "System\\ScriptCore"

BridgeFunctionAttach("OnCharacterEntry", "CharacterEntry");
--AQUI PODE SER UMA NOTICIA ASIM QUE PLAYER LOGAR BY: ZEUS REVOLUTIONS 
CharacterEntry = function(aIndex)		
	--local UserName = GetObjectName(aIndex)
	--local UserAccountLevel = GetObjectAccountLevel(aIndex)
	--local UserAccountExpireDate = GetObjectAccountExpireDate(aIndex)

	--NoticeSend(aIndex,0,string.format(MessageGet(154), UserName))

	--if UserAccountLevel == 0 then 
	--	NoticeSend(aIndex,1,string.format(MessageGet(155), UserAccountExpireDate))
	--elseif UserAccountLevel == 1 then
	--	NoticeSend(aIndex,1,string.format(MessageGet(156), UserAccountExpireDate))
	--elseif UserAccountLevel == 2 then
	--	NoticeSend(aIndex,1,string.format(MessageGet(157), UserAccountExpireDate))
	--elseif UserAccountLevel == 3 then
	--	NoticeSend(aIndex,1,string.format(MessageGet(158), UserAccountExpireDate))
	--end
end

LogColor(1, "[Script_CharacterEntry] Carregado Com Sucesso!")