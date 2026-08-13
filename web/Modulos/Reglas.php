<?php
if (!defined('AT_CONEXION')) { include dirname(__DIR__) . '/Seguridad/Conexion.php'; }
require_once dirname(__DIR__) . '/Configuraciones/Tema.php';

/* Reglamento de Mu Misiones - editá el array para cambiar el texto */
$reglas = array(
    array('titulo' => 'NO ESTA PERMITIDO NINGÚN TIPO DE TRAMPA.', 'parrafos' => array(
        'Ni cheats, ni dupes, ni aceleradores, ni otros trucos están permitidos y todo uso de alguna de estas herramientas llevan a la eliminación de la cuenta (En el caso de ser full se bloqueara la cuenta permanentemente.).',
        'Ante el bloqueo del vault automático (Por dupe) deberás esperar al próximo mantenimiento. Tu vault será vaciado completamente y no recuperaras ningún ítem pero se te devolverá el uso del vault.',
    )),
    array('titulo' => 'AFK (DEJAR TRABADO) NO ES PERMITIDO. NI MATANDO NI EN LA CIUDAD', 'parrafos' => array(
        'Esto favorece a que la gente que pasa más horas jugando sea la que este más arriba en el ranking. Mu Misiones no permite AFKERS y son castigados con ban. La primera vez es desconexión, la segunda ban por 2 semanas, la tercera 1 mes. A la cuarta vez se procederá a eliminar el personaje (En el caso de ser full se bloqueara el personaje permanentemente.). Si el jugador cometiera AFK con diferentes personajes estos contaran como infracciones acercándolo más al ban permanente (Que afectara al ultimo personaje encontrado AFK).',
    )),
    array('titulo' => 'KS (ROBAR MONSTRUOS) ESTA PERMITIDO (INCLUSIVE los dorados).', 'parrafos' => array(
        'No existe y no existirá nunca una regla sobre respetar spots, se lo dejamos al sentido común, a la buena onda de ayudar gente nueva o echarlos mediante el uso de PK (Leer Regla siguiente). Mu Misiones pide otro tipo de respeto, los spots deben ganárselos con onda o a la fuerza.',
        'Los BOKs (sean del nivel que sean) que caigan de los dorados pertenecen a quien lo agarre primero.',
    )),
    array('titulo' => 'HACERSE PK ESTA PERMITIDO', 'parrafos' => array(
        'El nuevo sistema de limpieza de PK permite quitárselo (via Web) a un costo de 60 millones de zen hasta 10 muertes, luego se cobraran 1,5 Millones por muerte adicional con un máximo de 100 muertes a un costo de: 200 Millones de Zen.',
        'Todos los domingos durante el mantenimiento se eliminara el pk de todo el Server automáticamente.',
    )),
    array('titulo' => 'NO SE ACEPTAN NOMBRE DE PERSONAJES OFENSIVOS', 'parrafos' => array(
        'Si tu personaje tiene un nombre que no se considerara correcto será renombrado. Si fuera un personaje creado con la intención de insultar u ocasionar problemas será eliminado, no devolviéndose ningún objeto del inventario ni puntaje/resets alguno.',
        'Tampoco se permite nicks que sugieran algún tipo de status o control sobre el Server, ni que permita que el resto de los usuarios confundan un personaje normal con un Owner, Admin o GM.',
        'El uso de nombres o símbolos racistas es castigado con la eliminación del personaje.',
        'En caso de que una guild tenga un nombre considerado ofensivo, racista o denigrante, se obligará al GuildMaster a cambiar su nombre dentro de las 48hs de creada la guild, pudiendo conservar los puntos obtenidos dentro de ese período de tiempo. De no contar con la cooperación del GuildMaster, la guild será eliminada sin derecho a reclamos, perdiéndose el zen usado en la creación de la misma, y el puntaje obtenido.',
    )),
    array('titulo' => 'EL USO DE PODERES EN LAS WARS ES DECISION DE USTEDES.', 'parrafos' => array(
        'Incluye auras, mana shield, Inner Strenght, Satanes, Ángeles y otros agregados que podrían modificar el curso de la war. El Server no se responsabiliza por los puntos perdidos.',
        'Se sobreentiende que los GMS conocen las capacidades/stats/sets del contrincante antes de aceptar la lucha.',
    )),
    array('titulo' => 'EL CAMBIO DE CLASE NO ESTA PERMITIDO - PARA NADIE.', 'parrafos' => array(
        'No se permite mover personajes de una cuenta a otra, ni mover stats a otro personaje dentro de la misma cuenta [con la excepción de que los vips pueden cambiar el nombre de su personaje UNA VEZ POR MES PAGO y mover personajes que estén dentro de otra de sus cuentas (únicamente si los mails de registro concuerdan y ambas cuentas son VIP)]. No se permite la suma de resets de un personaje a otro, bajo ningún punto de vista.',
    )),
    array('titulo' => 'SOLO SE PUEDE DIRIGIRSE A UN GM EN ESPAÑOL O EN INGLES.', 'parrafos' => array(
        'Existen GMS que solamente saben lo mas básico de español, si ellos no pueden responder sus dudas comuníquense con otro GM u OWNER utilizando los formularios de contacto o las direcciones de mail provistas. El foro posee una sección en español y otra en ingles para un mejor servicio y una respuesta mas rápida. También proveemos un servicio de CHAT. No presionen para obtener una respuesta, haz tu pregunta y aguarda ser respondido, ser educado ayuda a que se te responda lo mas rápido posible. El equipo del Mu Misiones no apoya el maltrato entre jugadores y el staff del Server, por lo tanto, si se considera que tu conducta hace mas lenta la respuesta hacia otros serás desconectado las veces que haga falta.',
    )),
    array('titulo' => 'RESPETARAS A LOS ADMIN Y GM\'S DEL JUEGO', 'parrafos' => array(
        'Tanto dentro como fuera del juego. El envió de correos insultantes o con agresividad serán no respondidos y se le sumara el ban correspondiente. Lo mismo aplica para el Chat, el foro y otras formas de interacción con el equipo del Mu Misiones. Cabe destacar que todos los medios de contacto ofrecidos nos dan acceso a tu IP y otros datos y que una vez iniciado un ataque/abuso/insulto podrían ser utilizados para dejarte fuera del servidor, la Web y todos sus componentes.',
    )),
    array('titulo' => 'NO HACERSE PASAR POR GM/ADMIN/DUEÑO', 'parrafos' => array(
        'El equipo del Mu Misiones trabaja arduamente para solucionar todos los problemas dentro y fuera del juego, por eso ellos son los únicos que tienen poder sobre el Server. Un jugador normal no puede hacerse llamar dueño o GM o admin bajo ningún punto de vista y en ninguna oportunidad, esto confunde a los demás y puede llevar a casos de scam (engaños). Este accionar se penalizara con BAN.',
        'Los representantes de este Server no tienen amigos, no conocen gente y no hacen favores ni otorgan ventajas cuando están trabajando sobre el mismo, por eso, decir que se obtiene algún beneficio dentro del juego por ser conocido/amigo/familiar/etc de un miembro del staff será también penalizado con BAN.',
    )),
    array('titulo' => 'GMS NO PUEDEN DARTE NI ITEMS NI PARTY NI OTROS PRIVILEGIOS.', 'parrafos' => array(
        'Esta regla aplica para todos y cada uno de los jugadores, no cumplirla terminara normalmente en una desconexión y si fuera con intención de molestar terminara en un BAN. Solamente durante eventos se otorgaran premios.',
    )),
    array('titulo' => 'LOS TEMAS REFERENTES AL SERVIDOR SERÁN TRATADOS SOLAMENTE POR LOS PERSONAJES DE LAS GUILDS =GM=/ADMIN/OWNERS', 'parrafos' => array(
        'Los GMS/ADMIN/OWNERS están identificados por su "rótulo", o respectiva guild con ese nombre; los temas referentes a la administración y/o moderación del Server deben de ser tratados con ellos y no con personajes regulares (sin privilegios de GM/ADMIN/OWNER, personajes normales, creados en una cuenta normal).',
        'Si los GMS/ADMIN/OWNERS decidiesen jugar con un personaje regular, nada los obliga desde ningún punto de vista, a asumir su función de GM/ADMIN/OWNER mientras estén conectados con dicho personaje.',
        'En el caso de un GM/ADMIN/OWNER jugar con un personaje regular, esto no implica desde ningún punto privilegios, dígase ítems editados, resets, etc. (Jugarán su personaje como todos los demás)',
        'Si ustedes tratan temas referentes al servidor con personajes particulares, van a ser sancionados con días de ban.',
        'Si necesitan ayuda inmediata, (y no pueden esperar a que un GM/ADMIN/OWNER este en línea) tienen a su disposición medios para comunicarse con las autoridades del servidor. (Foro, chat, e-mail y Discord de contacto)',
    )),
    array('titulo' => 'NO INSULTARAS. LOS INSULTOS SON CASTIGADOS CON BLOQUEO O ELIMINACIÓN', 'parrafos' => array(
        'Queda a responsabilidad del usuario el envió de las imágenes de la agresión a contacto@mumisiones.com, no hay bloqueo de palabra, la foto es un elemento 100% necesario.',
        'No es 100% necesario que la frase posea una palabra que sea considerada insulto, una amenaza contra la persona u el Server, el racismo, la agresión contra la familia y la discriminación física o mental pueden ser suficientes para un bloqueo o para la eliminación de tu cuenta.',
    )),
    array('titulo' => 'EL STAFF DEL SERVER TIENE DERECHO A UN JUEGO JUSTO E IGUALITARIO.', 'parrafos' => array(
        'Así como el staff del Mu Misiones se encargara de proteger los derechos de ustedes para que ustedes jueguen limpia y honestamente, nosotros, el staff demandamos que en nuestra hora de jugar se nos deje hacerlo pacíficamente, sin molestarnos por casos del Server, para eso tienen las siguientes vías de comunicación:Chat',
        '- Mails - Foro',
        'El abuso de esta regla podrá ser castigado con ban',
        'permanente',
        '.',
    )),
    array('titulo' => 'TU INFORMACIÓN PERSONAL SERÁ MANTENIDA EN SECRETO POR VOS.', 'parrafos' => array(
        'La creación de la cuenta DEBE SER CON DATOS REALES, serán necesarios para que luego puedas cambiar la contraseña y efectuar otras tramites. Como la contraseña fue elegida por vos, MU MISIONES ONLINE NO SE HACE RESPONSABLE por robo de cuentas, ni ítems, guilds o eliminación de personajes.',
        'El equipo del Mu Misiones trabaja arduamente para mantener tu información privada bien resguardada, el resto depende solamente de vos. Si tenes dudas sobre keyloggers y otros programas que pudieran estar afectando a tu privacidad en el foro hemos creado guías para que puedas jugar bien y seguro, estés donde estés. Al haber hecho click y aceptado las reglas se sobreentiende que leyeron la guía y que bajo ningún punto MU MISIONES ONLINE discutirá la devolución de ningún ítem o guild.',
        'Las cuentas vips podrán recibir un % (la cantidad desde el ultimo mantenimiento) de los resets si un personaje fuera eliminado. Podrán convertirse en vips aquellos que deseen recuperar sus resets perdidos solo mientras se subscriban al VIP antes de 1 mes de eliminado su personaje.',
    )),
    array('titulo' => 'SOBRE EL ROBO DE ITEMS (Solo para vips y personajes full)', 'parrafos' => array(
        'Una vez que haz perdido tus ítems no hay devolución, pero todavía puedes hacer justicia! El staff del Mu Misiones esta capacitado para obtener información sobre todos los trades que ejecuto cada personaje. Si te robaron, puedes hacer una consulta y obtener información sobre quien fue. Si hubiera un culpable MU MISIONES SE COMPROMETE A BLOQUEARLE LA CUENTA PARA SU POSTERIOR ELIMINACIÓN (En el caso de ser full se bloqueara la cuenta permanentemente.). No devolvemos ítems pero tampoco dejaremos que quede impune la persona que del otro lado les robo.',
        'Todos los reclamos deben ser hechos en el plazo de 2 semanas posterior al robo, luego se eliminan los logs y no hay forma de obtener esa información.',
    )),
    array('titulo' => 'REGLAS PARA GUILDS', 'parrafos' => array(
        'La creación de guilds supone un costo de 50Billones de zen, tener un mínimo de 50 resets, un mínimo de level 250, y estar usando ALAS o un DINORANT. El Server NO SE HACE RESPONSABLE por la pérdida de zen por no haber cumplido todos los requisitos.',
        'Todos los guilds deberán tener 10 o mas jugadores para seguir existiendo, deberán también sumar puntos frecuentemente y no podrán tener mas de 35 miembros.',
        'Las wars ilegales son penalizadas con la eliminación del guild y el baneo del GuildMaster.',
        'Abandonar wars no va contra las reglas, pero hace que tu guild pierda credibilidad y por ende nadie competirá contra ustedes, lo que hará que no sumen puntos y finalmente pierdan el guild.',
    )),
    array('titulo' => 'MU MISIONES NO INDEMNIZARA A NADIE.', 'parrafos' => array(
        'Ante un eventual problema Mu Misiones Online no indemnizara a ningún jugador por la cantidad de tiempo/dinero/otros invertidos en el Server. El costo del VIP no será devuelto en caso de baneo de la cuenta y seguirá corriendo hasta que se agote, donde luego el ban se convertirá en eliminación de la cuenta por inactividad. Mu Misiones retiene el poder de mantener las cuentas con personajes full por el mismo tiempo que dure la actividad del Server.',
        'Mu Misiones no avala la venta de cuentas por Dinero y no se aceptaran reclamos de ningún tipo sobre cuentas que presenten esa característica',
    )),
);

at_head('Reglas del servidor');
?>
<div class="at-msg at-info">Al crear tu cuenta aceptás este reglamento. El desconocimiento de las reglas no evita la sanción.</div>
<?php foreach ($reglas as $i => $r): ?>
  <div class="at-card">
    <h2 style="margin-top:0"><?= (int) $i + 1 ?>. <?= at_h($r['titulo']) ?></h2>
    <?php foreach ($r['parrafos'] as $p): ?><p><?= at_h($p) ?></p><?php endforeach; ?>
  </div>
<?php endforeach; ?>
<div class="at-card">
  <h2>Dudas sobre el reglamento</h2>
  <p>Consultá al staff por el <a href="ir.php?amz=Contacto">formulario de contacto</a> o en el Discord oficial de <?= AT_MARCA ?>.</p>
</div>
<?php at_foot(); ?>
