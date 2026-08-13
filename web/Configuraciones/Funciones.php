<?php

require_once __DIR__ . "/SQL.php";
require_once __DIR__ . "/../Seguridad/mssql_compat.php";

// PARA BLOQUEAR ALGUNA PALABRA SE TIENE QUE AGREGAR A LA LISTA EN MINUSCULAS
$badchars = array ("drop","select","update","delete","where","-1","-2","-3","-4","-5","-6","-7","-8","-9","shutdown","or","--","or=","del", ")--","character","dbo","set","\"",">","<","|","*","%","$","#","insert","distinct","having","truncate","replace","handler","like","procedure","limit","order by","group by","warehouse","del","$","sele"," +"," + ","+","+ dx"," dx","memb_info","from","add","union","webzen", "admin", "staff");

function criar_log_inject($digitou)
{

	$data = date("d.m.y - H:i:s"); // Pega a Data e Hora para grava no Log
	$navegador = isset($_SERVER['HTTP_USER_AGENT']) ? $_SERVER['HTTP_USER_AGENT'] : '';
	$requested = isset($_SERVER['REQUEST_URI']) ? $_SERVER['REQUEST_URI'] : '';
	$metodo = isset($_SERVER['REQUEST_METHOD']) ? $_SERVER['REQUEST_METHOD'] : 'CLI';
	$ip = isset($_SERVER['REMOTE_ADDR']) ? $_SERVER['REMOTE_ADDR'] : '0.0.0.0';
	$host = $ip;

	$arquivo = __DIR__ . "/../Logs/Seguridad.log";
	$log = "IP: $ip \r\n";
	$log .= "IP Reverso: $host \r\n";
	$log .= "Data: $data \r\n";
	$log .= "Navegador: $navegador \r\n";
	$log .= "Pagina: $requested \r\n";
	$log .= "Metodo: $metodo \r\n";
	$log .= $digitou;

	$qIps = mssql_query("Select memb___id From MEMB_STAT Where ip='".$ip."'");
	if(mssql_num_rows($qIps) <= 0)
	{
		$log .= "IP Registrada en el Server: No \r\n";
	}
	else
	{
		for($a=0;$a<mssql_num_rows($qIps);$a++)
		{
			$name = mssql_fetch_row($qIps);
			$log .= "Login: $name[0]\r\n";
		}

	}
	$log .= "\r\n\r\n";
	$log .= "----------------------------------------------------------------------------------------\r\n\r\n";
	@mkdir(dirname($arquivo), 0755, true);
	$fp=@fopen($arquivo, "a");
	if(!$fp) return;
	fwrite($fp, $log);
	fclose($fp);

}

function clean_variable($var)
{
	$newvar = preg_replace('/[^a-zA-Z0-9\_\-]/', '', (string) $var);
	$newvar =  strtolower($newvar);
	return $newvar;
}

function verificar_email($email)
{

	$mail_correcto = True;
	if ((strlen($email) >= 6) && (substr_count($email,"@") == 1) &&
	(substr($email,0,1) != "@") && (substr($email,strlen($email)-1,1) != "@"))
	{
		if ((!strstr($email,"'")) && (!strstr($email,"\"")) && (!strstr($email,"\\")) &&
		(!strstr($email,"\$")) && (!strstr($email," ")))
		{
			//vejo se tem caracter .
			if (substr_count($email,".")>= 1)
			{
				//obtenho a terminação do dominio
				$term_dom = substr(strrchr ($email, '.'),1);
				//verifico que a terminação do dominio seja correcta
				if (strlen($term_dom)>1 && strlen($term_dom)<5 && (!strstr($term_dom,"@")) )
				{
					//verifico que o de antes do dominio seja correcto
					$antes_dom = substr($email,0,strlen($email) - strlen($term_dom) - 1);
					$caracter_ult = substr($antes_dom,strlen($antes_dom)-1,1);
					if ($caracter_ult != "@" && $caracter_ult != ".")
					{
						$mail_correcto = False;
					}
				}
			}
		}
	}

	return $mail_correcto;
}

function criar_log_painel($digitou, $paginalog)
{

	$data = date("d.m.y - H:i:s"); // Pega a Data e Hora para grava no Log
	$ip = isset($_SERVER['REMOTE_ADDR']) ? $_SERVER['REMOTE_ADDR'] : '0.0.0.0';
	$host = $ip;

	$arquivo = __DIR__ . "/../Logs/panel.log"; // nome do arquivo de log
	@mkdir(dirname($arquivo), 0755, true);
	$fp_painel = @fopen($arquivo, "a");
	if(!$fp_painel) return;

	$log_painel = "Pagina: $paginalog \r\n";
	$log_painel.= "Data: $data \r\n";
	$log_painel.= "IP: $ip \r\n";
	$log_painel.= "IP Reverso: $host \r\n";
	$log_painel.= "Ação: $digitou \r\n";
	$log_painel.= "\r\n\r\n";
	$log_painel.= "----------------------------------------------------------------------------------------\r\n\r\n";

	fwrite($fp_painel, $log_painel);
	fclose($fp_painel);

}

function criar_log_sql($mensagemerro, $queryexecutada, $paginalog)
{

	$data = date("d.m.y - H:i:s"); // Pega a Data e Hora para grava no Log
	$ip = isset($_SERVER['REMOTE_ADDR']) ? $_SERVER['REMOTE_ADDR'] : '0.0.0.0';
	$host = $ip;

	$arquivo = __DIR__ . "/../Logs/error_sql.log"; // nome do arquivo de log
	@mkdir(dirname($arquivo), 0755, true);
	$fp_erro_sql = @fopen($arquivo, "a");
	if(!$fp_erro_sql) return;

	$log_erro_sql = "Pagina: $paginalog \r\n";
	$log_erro_sql.= "Data: $data \r\n";
	$log_erro_sql.= "IP: $ip \r\n";
	$log_erro_sql.= "IP Reverso: $host \r\n";
	$log_erro_sql.= "Mensagem de Erro: $mensagemerro \r\n\r\n";
	$log_erro_sql.= "SQL: '".nl2br($queryexecutada)."'\r\n";
	$log_erro_sql.= "\r\n\r\n";
	$log_erro_sql.= "----------------------------------------------------------------------------------------\r\n\r\n";

	fwrite($fp_erro_sql, $log_erro_sql);
	fclose($fp_erro_sql);

}
?>