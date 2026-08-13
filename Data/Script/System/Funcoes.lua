--[[
//=======================================================================================================
// Configurado por RDDLV - 97x Edition
//========================================================================================================


	Exemplo que inicia uma janela com um texto definido no Text.bmd
	ServerCommandSend(aIndex,1,1,0) -- 0 = linha 650 no Text.bmd, 1 = linha 651 e assim por diante.
]]--

Funcoes = {}

--[[	
	Você pode definir funções neste arquivo e chamar no ScriptMain através da chamada Funcoes.NomeFuncao()
	
	Função: function Funcoes.CheckGameMasterLevel(name)
	
	Explicação: Exemplo de estrutura array onde o nome "LEGENDARY" equivale ao index 0 do array GAME_MASTER_LEVEL.
	A rotina em questao CheckGameMasterLevel(name) verifica se existe algum valor no array com o nome informado e retorna a informação "level" do array em questão.
	
	se não encontrar retorna 0
	
	|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
]]--

GAME_MASTER_LEVEL = {}
GAME_MASTER_LEVEL['ZEUS-ADM'] = {level = 2}

function Funcoes.CheckGameMasterLevel(name)
    local gamemaster = GAME_MASTER_LEVEL[name]

    if gamemaster ~= nil
    then
		return gamemaster.level 
    end

    return 0
end

function Funcoes.CountTable(table)
	count = 0
	
	for i in pairs(table) do
		count = count + 1
	end
	
	return count
end

function Funcoes.average(t)
  local sum = 0
  for _,v in pairs(t) do
    sum = sum + v
  end
  return sum / #t
end

LogColor(1, "[Script_Funcoes] Carregado Com Sucesso!")

return Funcoes