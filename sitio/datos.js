'use strict';
/**
 * datos.js - Capa de datos del sitio.
 *
 * Guarda cuentas, personajes y pedidos en un JSON local (datos.json).
 * Es una DEMO: el servidor real usa Microsoft SQL Server con la base
 * MuOnline. Cuando se conecte contra el servidor de verdad, hay que
 * reemplazar este archivo por consultas a MEMB_INFO / Character.
 *
 * Las contraseñas se guardan con scrypt + salt, nunca en texto plano.
 */

const fs = require('fs');
const path = require('path');
const crypto = require('crypto');

const ARCHIVO = path.join(__dirname, 'datos.json');

const CLASES = ['Dark Wizard', 'Dark Knight', 'Fairy Elf', 'Magic Gladiator'];

// -----------------------------------------------------------------------
//  Persistencia
// -----------------------------------------------------------------------

function vacio() {
  return { cuentas: [], personajes: [], pedidos: [], noticias: [] };
}

function cargar() {
  try {
    return JSON.parse(fs.readFileSync(ARCHIVO, 'utf8'));
  } catch (e) {
    return vacio();
  }
}

function guardar(db) {
  fs.writeFileSync(ARCHIVO, JSON.stringify(db, null, 2));
}

let DB = cargar();

// -----------------------------------------------------------------------
//  Contraseñas
// -----------------------------------------------------------------------

function hashear(clave) {
  const salt = crypto.randomBytes(16).toString('hex');
  const hash = crypto.scryptSync(clave, salt, 64).toString('hex');
  return salt + ':' + hash;
}

function verificar(clave, guardado) {
  try {
    const [salt, hash] = String(guardado).split(':');
    if (!salt || !hash) return false;
    const prueba = crypto.scryptSync(clave, salt, 64);
    const real = Buffer.from(hash, 'hex');
    // Comparación en tiempo constante: evita filtrar información por timing.
    return real.length === prueba.length && crypto.timingSafeEqual(real, prueba);
  } catch (e) {
    return false;
  }
}

// -----------------------------------------------------------------------
//  Cuentas
// -----------------------------------------------------------------------

/** Reglas de MEMB_INFO: 4-10 caracteres, solo letras y números. */
function validarUsuario(u) {
  if (!u || u.length < 4 || u.length > 10) {
    return 'El usuario debe tener entre 4 y 10 caracteres.';
  }
  if (!/^[A-Za-z0-9]+$/.test(u)) {
    return 'El usuario solo puede tener letras y numeros.';
  }
  return null;
}

function validarClave(c) {
  if (!c || c.length < 4 || c.length > 10) {
    return 'La contrasena debe tener entre 4 y 10 caracteres.';
  }
  return null;
}

function buscarCuenta(usuario) {
  const u = String(usuario || '').toLowerCase();
  return DB.cuentas.find((c) => c.usuario.toLowerCase() === u) || null;
}

function crearCuenta({ usuario, clave, email }) {
  let error = validarUsuario(usuario);
  if (error) return { error };

  error = validarClave(clave);
  if (error) return { error };

  if (!email || !/^[^@\s]+@[^@\s]+\.[^@\s]+$/.test(email)) {
    return { error: 'El email no es valido.' };
  }
  if (buscarCuenta(usuario)) {
    return { error: 'Ese usuario ya esta registrado.' };
  }
  if (DB.cuentas.some((c) => c.email.toLowerCase() === email.toLowerCase())) {
    return { error: 'Ese email ya esta en uso.' };
  }

  const cuenta = {
    id: DB.cuentas.length + 1,
    usuario,
    clave: hashear(clave),
    email,
    creditos: 0,
    vip: 0,
    vipVence: null,
    creado: new Date().toISOString(),
  };
  DB.cuentas.push(cuenta);

  // Personaje inicial, para que la cuenta nueva tenga algo que mostrar.
  DB.personajes.push({
    cuenta: usuario,
    nombre: usuario,
    clase: CLASES[Math.floor(Math.random() * CLASES.length)],
    nivel: 1,
    resets: 0,
    zen: 0,
  });

  guardar(DB);
  return { cuenta };
}

function login(usuario, clave) {
  const cuenta = buscarCuenta(usuario);
  if (!cuenta || !verificar(clave, cuenta.clave)) {
    return { error: 'Usuario o contrasena incorrectos.' };
  }
  return { cuenta };
}

function cuentaPublica(usuario) {
  const c = buscarCuenta(usuario);
  if (!c) return null;
  return {
    usuario: c.usuario,
    email: c.email.replace(/^(.{2}).*(@.*)$/, '$1****$2'),
    creditos: c.creditos,
    vip: c.vip,
    vipVence: c.vipVence,
    personajes: DB.personajes.filter(
      (p) => p.cuenta.toLowerCase() === c.usuario.toLowerCase()
    ),
  };
}

// -----------------------------------------------------------------------
//  Rankings
// -----------------------------------------------------------------------

function ranking(limite = 10) {
  return DB.personajes
    .slice()
    .sort((a, b) => b.resets - a.resets || b.nivel - a.nivel)
    .slice(0, limite)
    .map((p, i) => ({
      puesto: i + 1,
      nombre: p.nombre,
      clase: p.clase,
      nivel: p.nivel,
      resets: p.resets,
    }));
}

// -----------------------------------------------------------------------
//  Tienda
// -----------------------------------------------------------------------

/** Paquetes de créditos. Los VIP salen de Data/Custom/CustomBuyVip.txt. */
const PAQUETES = [
  { id: 'c100', nombre: '100 Creditos', creditos: 100, precio: 1500 },
  { id: 'c300', nombre: '300 Creditos', creditos: 300, precio: 4000, destacado: true },
  { id: 'c700', nombre: '700 Creditos', creditos: 700, precio: 8000 },
];

const VIPS = [
  { id: 'vip0', nombre: 'Vip Bronze', exp: 10, drop: 10, dias: 30, costo: 80 },
  { id: 'vip1', nombre: 'Vip Prata', exp: 20, drop: 20, dias: 30, costo: 95 },
  { id: 'vip2', nombre: 'Vip Ouro', exp: 30, drop: 30, dias: 30, costo: 200 },
];

function comprarPaquete(usuario, idPaquete) {
  const cuenta = buscarCuenta(usuario);
  if (!cuenta) return { error: 'Cuenta no encontrada.' };

  const paq = PAQUETES.find((p) => p.id === idPaquete);
  if (!paq) return { error: 'Paquete invalido.' };

  // DEMO: se acreditan al instante. En producción los créditos se suman
  // recién cuando el webhook de MercadoPago confirma el pago.
  cuenta.creditos += paq.creditos;
  DB.pedidos.push({
    id: DB.pedidos.length + 1,
    usuario: cuenta.usuario,
    detalle: paq.nombre,
    monto: paq.precio,
    estado: 'acreditado',
    fecha: new Date().toISOString(),
  });
  guardar(DB);
  return { cuenta, paquete: paq };
}

function comprarVip(usuario, idVip) {
  const cuenta = buscarCuenta(usuario);
  if (!cuenta) return { error: 'Cuenta no encontrada.' };

  const vip = VIPS.find((v) => v.id === idVip);
  if (!vip) return { error: 'VIP invalido.' };

  if (cuenta.creditos < vip.costo) {
    return { error: 'No te alcanzan los creditos. Necesitas ' + vip.costo + '.' };
  }

  cuenta.creditos -= vip.costo;
  cuenta.vip = VIPS.indexOf(vip) + 1;
  const vence = new Date();
  vence.setDate(vence.getDate() + vip.dias);
  cuenta.vipVence = vence.toISOString();

  DB.pedidos.push({
    id: DB.pedidos.length + 1,
    usuario: cuenta.usuario,
    detalle: vip.nombre + ' (' + vip.dias + ' dias)',
    monto: vip.costo + ' creditos',
    estado: 'activado',
    fecha: new Date().toISOString(),
  });
  guardar(DB);
  return { cuenta, vip };
}

function pedidosDe(usuario) {
  const u = String(usuario || '').toLowerCase();
  return DB.pedidos
    .filter((p) => p.usuario.toLowerCase() === u)
    .sort((a, b) => new Date(b.fecha) - new Date(a.fecha));
}

// -----------------------------------------------------------------------
//  Estadísticas y noticias
// -----------------------------------------------------------------------

function estadisticas() {
  return {
    cuentas: DB.cuentas.length,
    personajes: DB.personajes.length,
    // El "online" es simulado: sin GameServer real no hay dato verdadero.
    online: Math.floor(Math.random() * 40) + 12,
  };
}

function noticias() {
  return DB.noticias;
}

// -----------------------------------------------------------------------
//  Datos de arranque
// -----------------------------------------------------------------------

function sembrar() {
  if (DB.cuentas.length > 0) return;

  const demo = [
    ['Zeus', 'Dark Knight', 350, 42],
    ['Athena', 'Fairy Elf', 350, 38],
    ['Kratos', 'Dark Knight', 350, 31],
    ['Merlin', 'Dark Wizard', 348, 27],
    ['Rhea', 'Magic Gladiator', 350, 24],
    ['Odin', 'Dark Knight', 341, 19],
    ['Freya', 'Fairy Elf', 337, 15],
    ['Loki', 'Dark Wizard', 322, 11],
    ['Thor', 'Magic Gladiator', 318, 8],
    ['Hera', 'Fairy Elf', 295, 5],
  ];

  demo.forEach(([nombre, clase, nivel, resets]) => {
    DB.personajes.push({
      cuenta: nombre,
      nombre,
      clase,
      nivel,
      resets,
      zen: Math.floor(Math.random() * 900000000),
    });
  });

  DB.noticias = [
    {
      fecha: '2026-08-16',
      titulo: 'Nuevo balance aplicado',
      texto:
        'Se corrigieron tres errores que trababan la barra de experiencia y ' +
        'se ajusto el combate: menos dano, mas vida y menos dependencia del reset.',
    },
    {
      fecha: '2026-08-14',
      titulo: 'Servidor configurado en modo HARD',
      texto:
        'Experiencia x20, drop 10%. Progresion clasica, pensada para jugadores ' +
        'que disfrutan el desafio del 97x original.',
    },
    {
      fecha: '2026-08-10',
      titulo: 'Sistema VIP disponible',
      texto:
        'Ya se pueden comprar los rangos Bronze, Prata y Ouro con creditos, ' +
        'con bonus de experiencia y drop.',
    },
  ];

  guardar(DB);
}

sembrar();

module.exports = {
  crearCuenta,
  login,
  cuentaPublica,
  ranking,
  comprarPaquete,
  comprarVip,
  pedidosDe,
  estadisticas,
  noticias,
  PAQUETES,
  VIPS,
};
