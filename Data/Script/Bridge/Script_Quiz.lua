-- |||||||||||||||||||||||||||||||||||||||||||||||||||||||||
-- Configurado por RDDLV - 97x Edition
-- |||||||||||||||||||||||||||||||||||||||||||||||||||||||||

require "System\\ScriptCore"

local LQuiz = { Phase = nil, Configs = {} };

BridgeFunctionAttach("OnReadScript","Quiz_ReadScript");

Quiz_ReadScript = function()
    LQuiz.Configs["RewardName"]     = "WCoinC";
    LQuiz.Configs["CommandCode"]    = 203;             -- Aqui NUMERO DO COMANDO 
    LQuiz.Configs["CommandName"]    = "/r";            -- NOME DO COMANDO EVENT
   
  -- LQuiz.Reward = { 
  --[[ Reward = { 
   
   WCoinC = 10,
   RandomWCoinC = {},
   }--]]
   
    LQuiz.Configs["Schedule"]  = {    
        [1] = { wDay = nil, Hour = "20:06:00", NumberOfQuestions = 5 },   -- Aqui Horarios Event
        [2] = { wDay = nil, Hour = "14:45:00", NumberOfQuestions = 5 },  
		[3] = { wDay = nil, Hour = "16:45:00", NumberOfQuestions = 5 },
        [4] = { wDay = nil, Hour = "18:45:00", NumberOfQuestions = 5 },
		[5] = { wDay = nil, Hour = "21:52:00", NumberOfQuestions = 5 },
        [6] = { wDay = nil, Hour = "22:45:00", NumberOfQuestions = 5 }
		
    };
    LQuiz.Configs["Questions"] = {
        [1]  = {"Qual atual time do CR7?"   ,                                          "Juventus"       , 15}, -- Aqui Muda Valor Dos Premium WCoinC
        [2]  = {"Como se chama o personagem Adolf...?" ,                               "Hitler"         , 15},
		[3]  = {"Como se escreve o n�mero 55 em n�meros romanos?" ,                    "LV"             , 15},
        [4]  = {"Qual o n�mero m�nimo de jogadores numa partida de futebol?" ,         "7"              , 15},       
        [5]  = {"Palavra PERDER em ingl�s ?"  ,                                        "lose"           , 15},
        [6]  = {"Quanto � 58 x 9 + 78?"  ,                                             "600"            , 15},
        [7]  = {"Quanto � 165 + 9 x 1 x 0?"  ,                                         "0"              , 15},
        [8]  = {"Qual o melhor amigo do homem?"   ,                                    "Cachorro"       , 15},
        [9]  = {"Uma marca de Video Games ?"  ,                                        "Sony"           , 15},
	    [10] = {"Qual o ant�nimo de forte?"  ,                                         "fraco"          , 15},
        [11] = {"De que pa�s � a inven��o do chuveiro el�trico?"   ,                   "Brasil"         , 15},
        [12] = {"Como � PORCO em ingl�s?"   ,                                          "Pig"            , 15},
        [13] = {"Quantas edi��es j� tivemos da libertadores ?"  ,                      "60"             , 15},
	    [14] = {"Nome de uma fruta que coma�a com P, 4 letras?"  ,                     "Pera"           , 15},
	    [15] = {"Animal mais alto do mundo"  ,                                         "Girafa"         , 15},
        [16] = {"Atualmente, quantos elementos qu�micos a tabela peri�dica possui?"   ,"118"            , 15},
        [17] = {"Usado para falar? M_C_O_O_E"   ,                                      "Microfone"      , 15},
        [18] = {"O que a palavra legend significa em portugu�s?"  ,                    "Lenda"          , 15},
		[19] = {"Quantos filmes h� em uma trilogia?"   ,                               "3"              , 15},
        [20] = {"Qual o planeta � famoso por possuir an�is em sua volta?"  ,           "Saturno"        , 15},
	    [21] = {"Quantas letras h� no alfabeto portugu�s, atualmente?"  ,              "26"             , 15},
	    [22] = {"Quantos minutos a luz do Sol demora para chegar � Terra?"  ,          "8"              , 15},
        [23] = {"Quantos dentes uma pessoa adulta possui, se n�o perdeu nenhum?"   ,   "32"             , 15},
        [24] = {"Quanto � 17 x 8 + 74 - 200 ?"  ,                                      "10"             , 15},
        [25] = {"Que level podemos criar MG?"  ,                                       "220"            , 15},
		[26] = {"Qual o acento gr�fico da palavra P�TIO?"  ,                           "Agudo"          , 15},
        [27] = {"Qual a maior estrela do Sistema Solar?"   ,                           "Sol"            , 15},
        [28] = {"Qual � o maior osso do corpo humano?"   ,                             "F�mur"          , 15},
        [29] = {"Quantas cores h� no arco-�ris?"  ,                                    "7"              , 15},
	    [30] = {"O que �, o que �: cai em p� e corre deitado??"   ,                    "Chuva"          , 15},
        [31] = {"Qual o nome da profiss�o que apaga inc�ndios?"   ,                    "Bombeiro"       , 15},
        [32] = {"Qual a fruta envenenada que a Branca de Neve comeu?"  ,               "Ma�a"           , 15},
		[33] = {"Quantos estados existem no Brasil?"  ,                                "26"             , 15},
        [34] = {"Quantas vezes o Brasil foi campe�o da copa do mundo?"   ,             "5"              , 15},
        [35] = {"Quantos dias t�m um ano bissexto?"   ,                                "366"            , 15},
        [36] = {"Quem Deus mandou construir uma arca antes do dil�vio??"  ,            "No�"            , 15},
		[37] = {"Em que lugar vivem mais cangurus do que pessoas?"  ,                  "Austr�lia"      , 15},
	    [38] = {"Quantos olhos a maior parte das aranhas t�m?"   ,                     "8"              , 15},
        [39] = {"De que s�o constitu�dos os diamantes?"   ,                            "Carbono"        , 15},
        [40] = {"Qual a nacionalidade do soci�logo e fil�sofo Durkheim?"  ,            "Frances"        , 15},
		[41] = {"Como morreu Saddam Hussein?"  ,                                       "Enforcado"      , 15},
        [42] = {"Qual o primeiro personagem criado por Walt Disney?"   ,               "Mickey"         , 15},
        [43] = {"Manila � a capital de que pa�s?"   ,                                  "Filipinas"      , 15},
        [44] = {"Em que ano aconteceu a primeira Copa do Mundo?"  ,                    "1930"           , 15},
		[45] = {"Quantos Deuses tem no Olimpo?"  ,                                     "12"             , 15},
	    [46] = {"Afrodite � filha de que Deus?"   ,                                    "Urano"          , 15},
        [47] = {"Qual � o nome do monstro que mora em um labirinto?"   ,               "Minotauro"      , 15},
        [48] = {"Qual deusa nasceu da cabe�a de Zeus?"  ,                              "Atena"          , 15},
		[49] = {"Quem � a esposa de Zeus?"  ,                                          "Hera"           , 15},
        [50] = {"Quem foi rejeitado por nascer deficiente e ent�o jogado no mar?"   ,  "Hefesto"        , 15},
        [51] = {"De qual pa�s o clube Liverpool �?"   ,                                "Inglaterra"     , 15},
        [52] = {"Qual � a sigla da liga dos Estados Unidos?"  ,                        "MLS"            , 15},
		[53] = {"Qual pais criou a Ta�a do Mundo?"   ,                                 "It�lia"         , 15},
        [54] = {"Qual o nome do machado de Thor?"  ,                                   "Mjolnir"        , 15},
		[55] = {"Qual time grande brasileiro j� foi rebaixado para a S�rie C?"  ,      "Fluminense"     , 15},
        [56] = {"Em qual m�s se comemora o Dia das M�es?"   ,                          "Maio"           , 15},
        [57] = {"Qual o primitivo de PAPELARIA?"   ,                                   "Papel"          , 15},
        [58] = {"Sorveteria � derivado de??"  ,                                        "Sorvete"        , 15},
        [59] = {"Em qual pa�s fica a sede da FIFA?" ,                                  "Sui�a"          , 15}
    };
end;

BridgeFunctionAttach("OnTimerThread","Quiz_TimerThread");

Quiz_TimerThread = function()
    if LQuiz.Phase == nil then
        local Hour = os.date("%X");
        local wDay = os.date("*t")["wday"];

        for Number in pairs(LQuiz.Configs["Schedule"]) do 
            local Selected = LQuiz.Configs["Schedule"][Number];
            if (Selected["wDay"] == nil or Selected["wDay"] == wDay) and (Selected["Hour"] == Hour) then
                LQuiz.Counter           = 0;
                LQuiz.Phase             = 1;
                LQuiz.QuestionsUsed     = {};
                LQuiz.NumberOfQuestions = Selected["NumberOfQuestions"];

                LogPrint("[Quiz]: Evento iniciado.");                
                break;
            end;
        end;
    elseif LQuiz.Phase == 1 then
        if LQuiz.Counter%60 == 0 then
            if LQuiz.Counter == 0 then 
                NoticeSendToAll(0, ". : [ Quiz ] : .");
                NoticeSendToAll(0, "PREPAREM-SE PARA TESTAR OS SEUS CONHECIMENTOS!");
                NoticeSendToAll(1, string.format("[Quiz]: Primeira pergunta em 60s, sequ�ncia de %d perguntas.", LQuiz.NumberOfQuestions));

                LQuiz.Counter = LQuiz.Counter + 1;
            else
                LQuiz.Counter  = 0;
                LQuiz.Phase    = 2;
                LQuiz.Selected = RandomGetNumber(#LQuiz.Configs["Questions"]);
                LogPrint("[Quiz]: Fase de perguntas iniciada.");
            end;
        else
            LQuiz.Counter = LQuiz.Counter + 1;
        end;     
    elseif LQuiz.Phase == 2 then
        if LQuiz.Counter == 0 or LQuiz.Counter == 60 then
            if LQuiz.NumberOfQuestions == 0 then
                NoticeSendToAll(0, ". : [ Quiz ] : .");
                NoticeSendToAll(0, "SEM MAIS PERGUNTAS, AT� A PR�XIMA!");

                LQuiz.Phase             = nil;
                LQuiz.Winner            = nil;
                LQuiz.Counter           = nil;
                LQuiz.Selected          = nil;
                LQuiz.QuestionsUsed     = nil;
                LQuiz.NumberOfQuestions = nil;

                LogPrint("[QUIZ]: Evento encerrado.");
            else

                while LQuiz.QuestionsUsed[LQuiz.Selected] ~= nil do 
                    LQuiz.Selected = math.random(1, #LQuiz.Configs["Questions"]); 
                    LQuiz.Selected = math.random(1, #LQuiz.Configs["Questions"]); 
                    LQuiz.Selected = math.random(1, #LQuiz.Configs["Questions"]); 
                end;

                NoticeSendToAll(1, string.format("[Quiz]: Pergunta -> %s", LQuiz.Configs["Questions"][LQuiz.Selected][1]));
                NoticeSendToAll(1, string.format("Valendo %d %s.", LQuiz.Configs["Questions"][LQuiz.Selected][3], LQuiz.Configs["RewardName"]));

                NoticeSendToAll(0, ". : [ Quiz ] : .");
                NoticeSendToAll(0, string.format("Pergunta: %s", LQuiz.Configs["Questions"][LQuiz.Selected][1]));
                NoticeSendToAll(0, string.format("Digite '%s <resp>' para responder.", LQuiz.Configs["CommandName"]));

                LQuiz.Winner            = "";
                LQuiz.Counter           = (LQuiz.Counter == 60) and 1 or LQuiz.Counter + 1;
                LQuiz.NumberOfQuestions = LQuiz.NumberOfQuestions - 1;

                LogPrint(string.format("[Quiz]: Pergunta selectionada: %d.", LQuiz.Selected));
            end;
        elseif LQuiz.Counter == 50 then
            NoticeSendToAll(1, "[Quiz]: Pr�xima pergunta em 10s, prepare-se!");

            if LQuiz.Winner == "" then
                NoticeSendToAll(1, "Tempo esgotado. Pr�xima pergunta em 10s, prepare-se!");
                LQuiz.Winner = "Tempo11Esgotado"; 
                LogPrint("[Quiz]: N�o houve ganhador para esta pergunta.");
            end;

            LQuiz.QuestionsUsed[LQuiz.Selected] = 1; 
            LQuiz.Counter       = LQuiz.Counter + 1;
        else
            LQuiz.Counter = LQuiz.Counter + 1;
        end;
    end;
end;

BridgeFunctionAttach("OnCommandManager", "Quiz_Command");

Quiz_Command = function(aIndex, code, arg)
    if LQuiz.Configs["CommandCode"] == code then
        if LQuiz.Phase ~= nil then
            if LQuiz.Phase == 1 then
                NoticeSend(aIndex, 1, "[Quiz]: Fique atento, logo aparecer� uma pergunta!!");
            else
                if LQuiz.Winner == "Tempo11Esgotado" then
                    NoticeSend(aIndex, 1, string.format("[Quiz]: Tempo esgotado. Pr�xima pergunta em %d segundos.", 60-LQuiz.Counter));
                elseif LQuiz.Winner ~= "" then
                    NoticeSend(aIndex, 1, string.format("[Quiz]: Algu�m j� acertou. Pr�xima pergunta em %d segundos.", 60-LQuiz.Counter));
                else
                    local resp = string.format(CommandGetArgString(arg, 0));
                    if string.upper(resp) == string.upper(LQuiz.Configs["Questions"][LQuiz.Selected][2]) then
                        local ValueOfPayment = LQuiz.Configs["Questions"][LQuiz.Selected][3];
                        LQuiz.Winner         = GetObjectName(aIndex);
                        LQuiz.QuestionsUsed[LQuiz.Selected] = 1;
                        NoticeSendToAll(0, ". : [ QUIZ ] : .");
                        NoticeSendToAll(0, string.format("%s acertou!!!! Ganhou %d %s.", LQuiz.Winner, ValueOfPayment, LQuiz.Configs["RewardName"]));
--[[Reward = { 
   
   WCoinC = 10,
   RandomWCoinC = {},
   }--]]
   
  --[[ QueryAccount = {

{
"UPDATE CashShopData SET WCoinC=WCoinC+5 WHERE AccountID='%s'";
"Voce Recebeu 10 WCoinC"
}

},--]]
                        SQLQuery(string.format("UPDATE CashShopData SET WCoinC = WCoinC + %d WHERE AccountID = '%s'", ValueOfPayment, GetObjectAccount(aIndex)));
                        SQLClose();

                        LogPrint(string.format("[Quiz]: %s acertou uma pergunta e recebeu %d %s.", LQuiz.Winner, ValueOfPayment, LQuiz.Configs["RewardName"]));
                    else
                        NoticeSend(aIndex, 1, "[Quiz]: Resposta incorreta. Tente novamente!");
                    end;
                end;
            end;
        else
            NoticeSend(aIndex, 1, "[Quiz]: Evento indispon�vel no momento.");
        end;

        return 1;
    end;
	
    return 0;
end;

LogColor(1, "[Script_Quiz] Carregado Com Sucesso!")	