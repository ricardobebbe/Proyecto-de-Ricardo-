-- |||||||||||||||||||||||||||||||||||||||||||||||||||||||||
-- Configurado por RDDLV - 97x Edition
-- |||||||||||||||||||||||||||||||||||||||||||||||||||||||||

Zoombie = {}
Zoombie.Zoombie_Ativado = 1   -- Aqui Liga Desliga Event

OBJECT_ONLINE = 3
OBJECT_USER = 1

-- Tempos em minutos
DuracaoEvento = 3
TempoParaInicioFinalizacao = 1

-- Premiação bônus para quem não se infecta
-- o premio é dividido entre os que não se infectarem, logo 500 dividido pelos jogadores que não se infectaram
PremiacaoBonus=5000

-- Bônus por infecção dado ao jogador que sair infectando os demais jogadores
-- Logo bonusinfeccao vezes a quantidade de jogadores infectados
bonusinfeccao=100

Finalizou = 0
zoombieTime = 0
zoombieUsers = {}
 
function Zoombie.ZoombieAddUser(aIndex)
	if Finalizou == 1 then
		return 0
	end
	
	if zoombieUsers[aIndex] ~= nil then
		NoticeSend(aIndex, 1, "Voce ja esta registrado no evento!")
		return 0
	end
	
	NoticeSend(aIndex, 1, "Voce foi adicionado ao evento!")
	MoveUserEx(aIndex, 6, 30, 166)
	SkinChangeSend(aIndex, -1)
 
	zoombieUsers[aIndex] = {isZombie=0}
 
	return 1
end
 
function Zoombie.ZoombieRemoveUser(aIndex)
	if zoombieUsers[aIndex] ~= nil then
		zoombieUsers[aIndex] = nil
		
		EffectDel(aIndex, 1)
		EffectDel(aIndex, 8)
		EffectDel(aIndex, 73)
		EffectDel(aIndex, 143)
		EffectDel(aIndex, 144)
		SkinChangeSend(aIndex,-1)
		
		MoveUserEx(aIndex, 0, 125, 125)
		NoticeSend(aIndex, 1, "Voce saiu do evento!")
	end
end
  
function Zoombie.ZoombieOnTimerThread()
	if Finalizou == 1 then return 0	end
	
	--LogColor(1, "[LUA] zoombie rodando...")
 
	local totalZombies = 0
	local totalPlayers = 0
	
	-- se o tempo for menor ou igual que 60 segundos
	if zoombieTime <=  TempoParaInicioFinalizacao * 60 then 
		-- Usado para exibir contagem quando tempo inferior a 10 segundos
		local t = 1 * 60 - zoombieTime
		
		-- notifica a cada 15 segundos ou quando menor ou igual que 10 segundos
		if ((zoombieTime % 15 == 0) or (t < 11)) then			
			NoticeSendToAll(0, 'Ataque Zumbi irá começar em '..t..' segundos')
			NoticeSendToAll(0, 'Utilize /zoombie para participar')
		end 
		
		-- Incrementa 1 segundo a variável
		zoombieTime = zoombieTime+1 
		return 0
	else
		-- Se passou do tempo limite de 60 segundos
		-- Conta quantas entradas tem no array zoombieUsers e se menor que 4 cancela o evento 
		if Funcoes.CountTable(zoombieUsers) < 4 then
			Zoombie.ZoombieCancel()
			return 0
		end	
		
		-- Incrementa 1 segundo a variável
		zoombieTime = zoombieTime+1 
	end	
  
	for k,v in pairs(zoombieUsers) do 
		local typ = GetObjectType(k)
		local con = GetObjectConnected(k)
		
		if(typ == OBJECT_USER and con == OBJECT_ONLINE) then			
			local map = GetObjectMap(k)
			
			-- Se o jogador sair do mapa remove ele do evento
			if map ~= 6 then
				Zoombie.ZoombieRemoveUser(k)
			end
 
			-- Conta a quantidade de zumbis e jogadores
			if zoombieUsers[k].isZombie == 1 then
				totalZombies = totalZombies+1
			else 
				totalPlayers = totalPlayers+1
			end 
		else
			-- Se não estiver online remove do evento
			Zoombie.ZoombieRemoveUser(k) 
		end 
	end
	
	-- Se não tiver pelo menos 4 jogadores sai da função
	if Funcoes.CountTable(zoombieUsers) < 4 then
		return 0
	end

	-- Infecta alguém para dar inicio
	if totalZombies < 1 and totalPlayers > 1 then
		Zoombie.ZoombieInfectRandom()
	end

	-- Notifica a cada 30 segundos
	if zoombieTime % 30 == 0 then
		NoticeSendToAll(0, 'Restam '..totalPlayers..' players a serem infectados!')
	end
	-- Notifica a cada 15 segundos
	if zoombieTime % 30 == 15 then
		NoticeSendToAll(0, 'Há '..totalZombies..' zumbis pelo mapa!')
	end		

	-- Se restou apenas 1 player ele é o vencedor e portanto termina o evento
	if (totalPlayers <= 1 or zoombieTime >= DuracaoEvento * 60) then
		Zoombie.ZoombieFinish()
	end
end
 
function Zoombie.ZoombieInfectRandom()
	-- Variavel para saber o total de jogadores
 	local total = 0
 
	-- Percorre o array zoombieUsers
	for k,v in pairs(zoombieUsers) do
		-- se não for zumbi adiciona 1 a variavel total
		if v.isZombie == 0 then
			total = total+1
		end 
	end
 
	if total < 1 then
		-- se não tiver jogador que é zumbi sai da função
		return 0
	end
 
	-- Array para adicionarmos quem não é zumbi
	local p = {}
 
	-- k = index e v chave do index em questão
	for k,v in pairs(zoombieUsers) do 
		if v.isZombie == 0 then
			-- Adiciona a index "k" do jogador que não é zumbi ao array p
			table.insert(p, k)
		end 
	end
 
	-- Infecta algum jogador que não é zumbi aleatório
	local aleatorio = p[math.random(#p)]
	
	Zoombie.ZoombieInfect(aleatorio)	
	NoticeSendToAll(0, string.format("O virus foi liberado, o jogador %s foi infectado CORRAM!", GetObjectName(aleatorio)))
 end
 
function Zoombie.ZoombieFinish()
 	local total = 0
	local id
 
	-- Conta a quantidade de jogadores para dividir a premiação do bônus
	for k,v in pairs(zoombieUsers) do 
		if zoombieUsers[k].isZombie == 0 then
			total = total+1
			id = k
		end 
	end
 
	-- Divide o bônus pela quantidade de jogador que não foi infectado
	local bonus = math.ceil(PremiacaoBonus/total)
	
	if total == 1 then
		local name = GetObjectName(id)
		NoticeSendToAll(0, string.format("Apenas o jogador %s conseguiu sobreviver!", name))
	elseif total > 1 then
		NoticeSendToAll(0, string.format("%d jogadores conseguiram sobreviver!", total))
	end
 
	for k,v in pairs(zoombieUsers) do
		if v.isZombie == 0 then
			--CashShopAddPoint(k, 0, bonus, 0)
			NoticeSend(k, 1, "Você recebeu "..bonus.." mPoints por não ter se infectado!")
		end
		Zoombie.ZoombieRemoveUser(k)
	end 
	
	NoticeSendToAll(0, "Evento Ataque Zumbi finalizado!")	
	Finalizou = 1
end
 
function Zoombie.ZoombieCancel() 
	for k,v in pairs(zoombieUsers) do
		Zoombie.ZoombieRemoveUser(k)
	end 
	
	NoticeSendToAll(0, "Evento zumbi cancelado devido a falta de jogadores necessários!")
	Finalizou = 1
end
 
function Zoombie.ZoombieOnUserDie(aIndex,bIndex)  
	if zoombieUsers[aIndex] ~= nil and zoombieUsers[bIndex] ~= nil then 
		if zoombieUsers[aIndex].isZombie == 0 and zoombieUsers[bIndex].isZombie == 1 then 
			Zoombie.ZoombieInfect(aIndex)
			
			--CashShopAddPoint(bIndex, 0, 50, 0)
			NoticeSend(bIndex, 1, "Você recebeu "..bonusinfeccao.." mPoints por infectar um jogador!") 
		end 
	end 
end
 
function Zoombie.ZoombieOnCharacterClose(aIndex)
	Zoombie.ZoombieRemoveUser(aIndex)
end
 
function Zoombie.ZoombieInfect(aIndex) 
	if zoombieUsers[aIndex] ~= nil and zoombieUsers[aIndex].isZombie == 0 then
		local UserName = GetObjectName(aIndex)
		NoticeSend(aIndex,0, string.format("%s você foi infectado!", UserName))
	
		zoombieUsers[aIndex].isZombie = 1
		
		--EffectAdd(aIndex,0,73,DuracaoEvento*60,0,0,0,0) -- Apenas para efeito, sem valor
		EffectAdd(aIndex,0,1,DuracaoEvento*60,10000,0,0,0)
		EffectAdd(aIndex,0,8,DuracaoEvento*60,1000,0,0,0)
		EffectAdd(aIndex,0,143,DuracaoEvento*60,10000,0,0,0) 
		EffectAdd(aIndex,0,144,DuracaoEvento*60,10000,0,0,0) 
		
		SkinChangeSend(aIndex,16)
	end 
end

LogColor(1, "[Script_Zoombie] Loaded!")
return Zoombie

