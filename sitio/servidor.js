'use strict';
/**
 * servidor.js - Sitio web de [Mu]Misiones (97x)
 *
 * Servidor HTTP sin dependencias externas: solo modulos nativos de Node.
 * Sirve la web y una pequena API JSON que consume el front.
 *
 * Arranque:  node sitio/servidor.js
 */

const http = require('http');
const fs = require('fs');
const path = require('path');
const crypto = require('crypto');

const datos = require('./datos');

const PUERTO = process.env.PORT || 3000;
const PUBLICO = path.join(__dirname, 'publico');

// -----------------------------------------------------------------------
//  Sesiones en memoria (cookie firmada)
// -----------------------------------------------------------------------

const sesiones = new Map();

function nuevaSesion(usuario) {
  const id = crypto.randomBytes(24).toString('hex');
  sesiones.set(id, { usuario, creada: Date.now() });
  return id;
}

function leerCookie(req, nombre) {
  const raw = req.headers.cookie || '';
  for (const parte of raw.split(';')) {
    const [k, ...v] = parte.trim().split('=');
    if (k === nombre) return decodeURIComponent(v.join('='));
  }
  return null;
}

function usuarioDe(req) {
  const sid = leerCookie(req, 'sid');
  if (!sid) return null;
  const s = sesiones.get(sid);
  return s ? s.usuario : null;
}

// -----------------------------------------------------------------------
//  Utilidades HTTP
// -----------------------------------------------------------------------

function json(res, obj, codigo = 200, cabeceras = {}) {
  const cuerpo = JSON.stringify(obj);
  res.writeHead(codigo, Object.assign({
    'Content-Type': 'application/json; charset=utf-8',
    'Content-Length': Buffer.byteLength(cuerpo),
    'Cache-Control': 'no-store',
  }, cabeceras));
  res.end(cuerpo);
}

function leerCuerpo(req) {
  return new Promise((resolve) => {
    let d = '';
    req.on('data', (c) => {
      d += c;
      // Corte defensivo: no aceptamos cuerpos gigantes.
      if (d.length > 1e6) req.destroy();
    });
    req.on('end', () => {
      try {
        resolve(JSON.parse(d || '{}'));
      } catch (e) {
        resolve({});
      }
    });
  });
}

const TIPOS = {
  '.html': 'text/html; charset=utf-8',
  '.css': 'text/css; charset=utf-8',
  '.js': 'application/javascript; charset=utf-8',
  '.svg': 'image/svg+xml',
  '.zip': 'application/zip',
  '.png': 'image/png',
  '.ico': 'image/x-icon',
};

function servirEstatico(req, res, ruta) {
  // Se normaliza para que nadie pueda salir de la carpeta publica.
  const limpio = path.normalize(ruta).replace(/^(\.\.[/\\])+/, '');
  let archivo = path.join(PUBLICO, limpio);

  if (!archivo.startsWith(PUBLICO)) {
    res.writeHead(403).end('Prohibido');
    return;
  }
  if (fs.existsSync(archivo) && fs.statSync(archivo).isDirectory()) {
    archivo = path.join(archivo, 'index.html');
  }
  if (!fs.existsSync(archivo)) {
    res.writeHead(404, { 'Content-Type': 'text/html; charset=utf-8' });
    res.end('<h1>404</h1><p><a href="/">Volver al inicio</a></p>');
    return;
  }

  const ext = path.extname(archivo).toLowerCase();
  res.writeHead(200, { 'Content-Type': TIPOS[ext] || 'application/octet-stream' });
  fs.createReadStream(archivo).pipe(res);
}

// -----------------------------------------------------------------------
//  API
// -----------------------------------------------------------------------

async function api(req, res, url) {
  const ruta = url.pathname.replace(/^\/api/, '');
  const metodo = req.method;

  // --- publico ---------------------------------------------------------
  if (ruta === '/estado' && metodo === 'GET') {
    return json(res, {
      estadisticas: datos.estadisticas(),
      usuario: usuarioDe(req),
    });
  }

  if (ruta === '/noticias' && metodo === 'GET') {
    return json(res, datos.noticias());
  }

  if (ruta === '/ranking' && metodo === 'GET') {
    return json(res, datos.ranking(10));
  }

  if (ruta === '/tienda' && metodo === 'GET') {
    return json(res, { paquetes: datos.PAQUETES, vips: datos.VIPS });
  }

  // --- registro --------------------------------------------------------
  if (ruta === '/registro' && metodo === 'POST') {
    const b = await leerCuerpo(req);
    if (b.clave !== b.clave2) {
      return json(res, { error: 'Las contrasenas no coinciden.' }, 400);
    }
    const r = datos.crearCuenta({
      usuario: String(b.usuario || '').trim(),
      clave: String(b.clave || ''),
      email: String(b.email || '').trim(),
    });
    if (r.error) return json(res, { error: r.error }, 400);

    const sid = nuevaSesion(r.cuenta.usuario);
    return json(res, { ok: true, usuario: r.cuenta.usuario }, 200, {
      'Set-Cookie': `sid=${sid}; HttpOnly; Path=/; SameSite=Lax; Max-Age=86400`,
    });
  }

  // --- login / logout --------------------------------------------------
  if (ruta === '/login' && metodo === 'POST') {
    const b = await leerCuerpo(req);
    const r = datos.login(String(b.usuario || '').trim(), String(b.clave || ''));
    if (r.error) return json(res, { error: r.error }, 401);

    const sid = nuevaSesion(r.cuenta.usuario);
    return json(res, { ok: true, usuario: r.cuenta.usuario }, 200, {
      'Set-Cookie': `sid=${sid}; HttpOnly; Path=/; SameSite=Lax; Max-Age=86400`,
    });
  }

  if (ruta === '/logout' && metodo === 'POST') {
    const sid = leerCookie(req, 'sid');
    if (sid) sesiones.delete(sid);
    return json(res, { ok: true }, 200, {
      'Set-Cookie': 'sid=; HttpOnly; Path=/; Max-Age=0',
    });
  }

  // --- privado ---------------------------------------------------------
  const usuario = usuarioDe(req);

  if (ruta === '/cuenta' && metodo === 'GET') {
    if (!usuario) return json(res, { error: 'No hay sesion iniciada.' }, 401);
    return json(res, {
      cuenta: datos.cuentaPublica(usuario),
      pedidos: datos.pedidosDe(usuario),
    });
  }

  if (ruta === '/comprar' && metodo === 'POST') {
    if (!usuario) return json(res, { error: 'Inicia sesion para comprar.' }, 401);
    const b = await leerCuerpo(req);
    const r = datos.comprarPaquete(usuario, String(b.id || ''));
    if (r.error) return json(res, { error: r.error }, 400);
    return json(res, { ok: true, creditos: r.cuenta.creditos, compra: r.paquete.nombre });
  }

  if (ruta === '/vip' && metodo === 'POST') {
    if (!usuario) return json(res, { error: 'Inicia sesion para activar VIP.' }, 401);
    const b = await leerCuerpo(req);
    const r = datos.comprarVip(usuario, String(b.id || ''));
    if (r.error) return json(res, { error: r.error }, 400);
    return json(res, { ok: true, creditos: r.cuenta.creditos, vip: r.vip.nombre });
  }

  return json(res, { error: 'Ruta no encontrada' }, 404);
}

// -----------------------------------------------------------------------
//  Servidor
// -----------------------------------------------------------------------

const servidor = http.createServer(async (req, res) => {
  const url = new URL(req.url, 'http://localhost');

  try {
    if (url.pathname.startsWith('/api/')) {
      return await api(req, res, url);
    }
    servirEstatico(req, res, url.pathname === '/' ? '/index.html' : url.pathname);
  } catch (e) {
    console.error('[error]', e.message);
    if (!res.headersSent) json(res, { error: 'Error interno' }, 500);
  }
});

// Bind en 0.0.0.0: hace falta para que el preview del sandbox lo alcance.
servidor.listen(PUERTO, '0.0.0.0', () => {
  console.log('[Mu]Misiones - sitio escuchando en http://0.0.0.0:' + PUERTO);
});
