'use strict';
/* =====================================================================
   [Mu]Misiones - logica del front
   Todas las llamadas usan rutas RELATIVAS (/api/...), nunca localhost,
   para que funcionen detras del proxy del preview.
   ===================================================================== */

const $ = (s) => document.querySelector(s);
const $$ = (s) => document.querySelectorAll(s);

let USUARIO = null;

// ---------------------------------------------------------------- api
async function api(ruta, opciones = {}) {
  const res = await fetch('/api' + ruta, Object.assign({
    headers: { 'Content-Type': 'application/json' },
    credentials: 'same-origin',
  }, opciones));
  let cuerpo = {};
  try { cuerpo = await res.json(); } catch (e) { /* respuesta vacia */ }
  return { ok: res.ok, datos: cuerpo };
}

// ---------------------------------------------------------------- ui
function toast(msg, malo = false) {
  const t = $('#toast');
  t.textContent = msg;
  t.className = 'toast visible' + (malo ? ' malo' : '');
  clearTimeout(t._t);
  t._t = setTimeout(() => { t.className = 'toast'; }, 3200);
}

function abrirModal(panel) {
  $$('.panel-modal').forEach((p) => p.classList.remove('activo'));
  const destino = document.querySelector(`[data-panel="${panel}"]`);
  if (destino) destino.classList.add('activo');
  $('#modal').classList.add('visible');
  if (panel === 'cuenta') cargarCuenta();
}

function cerrarModal() {
  $('#modal').classList.remove('visible');
  $$('.aviso').forEach((a) => { a.className = 'aviso'; a.textContent = ''; });
}

function aviso(id, texto, ok = false) {
  const el = $(id);
  el.textContent = texto;
  el.className = 'aviso ' + (ok ? 'ok' : 'error');
}

function esc(s) {
  return String(s == null ? '' : s).replace(/[&<>"']/g, (c) => ({
    '&': '&amp;', '<': '&lt;', '>': '&gt;', '"': '&quot;', "'": '&#39;',
  })[c]);
}

// ------------------------------------------------------------ cabecera
function pintarAcciones() {
  const caja = $('#acciones');
  if (USUARIO) {
    caja.innerHTML =
      `<button class="btn btn-borde btn-chico" data-abrir="cuenta">${esc(USUARIO)}</button>`;
  } else {
    caja.innerHTML =
      '<button class="btn btn-borde btn-chico" data-abrir="login">Ingresar</button>' +
      '<button class="btn btn-chico" data-abrir="registro">Registrarse</button>';
  }
}

// -------------------------------------------------------------- estado
async function cargarEstado() {
  const { datos } = await api('/estado');
  if (!datos || !datos.estadisticas) return;
  USUARIO = datos.usuario;
  $('#s-online').textContent = datos.estadisticas.online;
  $('#s-cuentas').textContent = datos.estadisticas.cuentas;
  $('#s-pjs').textContent = datos.estadisticas.personajes;
  pintarAcciones();
}

// ------------------------------------------------------------ noticias
async function cargarNoticias() {
  const { datos } = await api('/noticias');
  $('#noticias').innerHTML = (datos || []).map((n) => `
    <article class="noticia">
      <div class="fecha">${esc(n.fecha)}</div>
      <h4>${esc(n.titulo)}</h4>
      <p>${esc(n.texto)}</p>
    </article>`).join('');
}

// ------------------------------------------------------------- ranking
async function cargarRanking() {
  const { datos } = await api('/ranking');
  const medalla = (n) => (n === 1 ? ' oro' : n === 2 ? ' plata' : n === 3 ? ' bronce' : '');
  $('#tb-ranking').innerHTML = (datos || []).map((p) => `
    <tr>
      <td><span class="puesto${medalla(p.puesto)}">${p.puesto}</span></td>
      <td><b>${esc(p.nombre)}</b></td>
      <td>${esc(p.clase)}</td>
      <td>${p.nivel}</td>
      <td><b style="color:var(--oro)">${p.resets}</b></td>
    </tr>`).join('');
}

// -------------------------------------------------------------- tienda
async function cargarTienda() {
  const { datos } = await api('/tienda');
  if (!datos) return;

  $('#paquetes').innerHTML = (datos.paquetes || []).map((p) => `
    <div class="producto${p.destacado ? ' destacado' : ''}">
      ${p.destacado ? '<span class="cinta">MAS ELEGIDO</span>' : ''}
      <div class="titulo">${esc(p.nombre)}</div>
      <div class="valor">$${p.precio.toLocaleString('es-AR')}</div>
      <div class="detalle">${p.creditos} créditos para gastar en la tienda</div>
      <button class="btn ancho" data-comprar="${esc(p.id)}">Comprar</button>
    </div>`).join('');

  $('#vips').innerHTML = (datos.vips || []).map((v) => `
    <div class="producto">
      <div class="titulo">${esc(v.nombre)}</div>
      <div class="valor">${v.costo}<span style="font-size:14px;color:var(--apagado)"> cr</span></div>
      <div class="bonus"><span>+${v.exp}% exp</span><span>+${v.drop}% drop</span></div>
      <div class="detalle">Duración: ${v.dias} días</div>
      <button class="btn btn-borde ancho" data-vip="${esc(v.id)}">Activar</button>
    </div>`).join('');
}

// -------------------------------------------------------------- cuenta
async function cargarCuenta() {
  const { ok, datos } = await api('/cuenta');
  if (!ok) { abrirModal('login'); return; }

  const c = datos.cuenta;
  const vipNombre = ['Sin VIP', 'Vip Bronze', 'Vip Prata', 'Vip Ouro'][c.vip] || 'Sin VIP';

  const pjs = (c.personajes || []).map((p) => `
    <div class="pj">
      <div><b>${esc(p.nombre)}</b><small>${esc(p.clase)}</small></div>
      <div style="text-align:right">
        <b>Nivel ${p.nivel}</b><small>${p.resets} resets</small>
      </div>
    </div>`).join('') || '<p class="modal-sub">Todavía no tenés personajes.</p>';

  const pedidos = (datos.pedidos || []).slice(0, 5).map((p) => `
    <div class="fila-dato">
      <span>${esc(p.detalle)}</span>
      <b style="color:var(--verde);font-size:12.5px">${esc(p.estado)}</b>
    </div>`).join('') || '<p class="modal-sub">Sin movimientos todavía.</p>';

  $('#caja-cuenta').innerHTML = `
    <div class="fila-dato"><span>Usuario</span><b>${esc(c.usuario)}</b></div>
    <div class="fila-dato"><span>Email</span><b>${esc(c.email)}</b></div>
    <div class="fila-dato"><span>Créditos</span><b style="color:var(--oro)">${c.creditos}</b></div>
    <div class="fila-dato"><span>Rango</span>
      <span class="insignia${c.vip ? '' : ' gris'}">${esc(vipNombre)}</span></div>
    <div class="titulito">Personajes</div>
    <div class="lista-pj">${pjs}</div>
    <div class="titulito">Últimos movimientos</div>
    ${pedidos}`;
}

// --------------------------------------------------------------- login
$('#f-login').addEventListener('submit', async (e) => {
  e.preventDefault();
  const f = new FormData(e.target);
  const { ok, datos } = await api('/login', {
    method: 'POST',
    body: JSON.stringify({ usuario: f.get('usuario'), clave: f.get('clave') }),
  });
  if (!ok) { aviso('#av-login', datos.error || 'No se pudo entrar.'); return; }
  USUARIO = datos.usuario;
  pintarAcciones();
  cerrarModal();
  toast('Bienvenido, ' + USUARIO);
  e.target.reset();
});

// ------------------------------------------------------------ registro
$('#f-registro').addEventListener('submit', async (e) => {
  e.preventDefault();
  const f = new FormData(e.target);
  const { ok, datos } = await api('/registro', {
    method: 'POST',
    body: JSON.stringify({
      usuario: f.get('usuario'),
      email: f.get('email'),
      clave: f.get('clave'),
      clave2: f.get('clave2'),
    }),
  });
  if (!ok) { aviso('#av-registro', datos.error || 'No se pudo registrar.'); return; }
  USUARIO = datos.usuario;
  pintarAcciones();
  cerrarModal();
  toast('¡Cuenta creada! Ya podés jugar.');
  cargarEstado();
  e.target.reset();
});

// --------------------------------------------------------------- salir
$('#b-salir').addEventListener('click', async () => {
  await api('/logout', { method: 'POST' });
  USUARIO = null;
  pintarAcciones();
  cerrarModal();
  toast('Sesión cerrada.');
});

// ------------------------------------------------------- clicks globales
document.addEventListener('click', async (e) => {
  const abrir = e.target.closest('[data-abrir]');
  if (abrir) { abrirModal(abrir.dataset.abrir); return; }

  const ir = e.target.closest('[data-ir]');
  if (ir) { e.preventDefault(); abrirModal(ir.dataset.ir); return; }

  const comprar = e.target.closest('[data-comprar]');
  if (comprar) {
    if (!USUARIO) { abrirModal('login'); toast('Iniciá sesión para comprar.', true); return; }
    const { ok, datos } = await api('/comprar', {
      method: 'POST', body: JSON.stringify({ id: comprar.dataset.comprar }),
    });
    toast(ok ? `${datos.compra} acreditado. Saldo: ${datos.creditos}` : datos.error, !ok);
    return;
  }

  const vip = e.target.closest('[data-vip]');
  if (vip) {
    if (!USUARIO) { abrirModal('login'); toast('Iniciá sesión para activar VIP.', true); return; }
    const { ok, datos } = await api('/vip', {
      method: 'POST', body: JSON.stringify({ id: vip.dataset.vip }),
    });
    toast(ok ? `${datos.vip} activado. Saldo: ${datos.creditos}` : datos.error, !ok);
    return;
  }
});

$('#cerrar').addEventListener('click', cerrarModal);
$('#modal').addEventListener('click', (e) => { if (e.target.id === 'modal') cerrarModal(); });
document.addEventListener('keydown', (e) => { if (e.key === 'Escape') cerrarModal(); });

// --------------------------------------------------------------- inicio
cargarEstado();
cargarNoticias();
cargarRanking();
cargarTienda();
