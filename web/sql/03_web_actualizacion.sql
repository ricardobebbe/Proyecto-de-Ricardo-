/* ------------------------------------------------------------------
   AT_Web - Actualizacion 2026 (configuracion web, banco, resets)
   Ejecutar en la base MuOnline. Es seguro correrlo varias veces.
   ------------------------------------------------------------------ */
USE MuOnline;
GO

/* --- MEMB_INFO: columnas usadas por la web ------------------------ */
IF COL_LENGTH('dbo.MEMB_INFO','CuentaGM')    IS NULL ALTER TABLE dbo.MEMB_INFO ADD CuentaGM    int NOT NULL DEFAULT 0;
IF COL_LENGTH('dbo.MEMB_INFO','vip')         IS NULL ALTER TABLE dbo.MEMB_INFO ADD vip         int NOT NULL DEFAULT 0;
IF COL_LENGTH('dbo.MEMB_INFO','datavip')     IS NULL ALTER TABLE dbo.MEMB_INFO ADD datavip     datetime NULL;
IF COL_LENGTH('dbo.MEMB_INFO','diasdevip')   IS NULL ALTER TABLE dbo.MEMB_INFO ADD diasdevip   int NULL;
IF COL_LENGTH('dbo.MEMB_INFO','vencimiento') IS NULL ALTER TABLE dbo.MEMB_INFO ADD vencimiento datetime NULL;
IF COL_LENGTH('dbo.MEMB_INFO','banco')       IS NULL ALTER TABLE dbo.MEMB_INFO ADD banco       bigint NOT NULL DEFAULT 0;
IF COL_LENGTH('dbo.MEMB_INFO','WebFecha')    IS NULL ALTER TABLE dbo.MEMB_INFO ADD WebFecha    datetime NULL;
IF COL_LENGTH('dbo.MEMB_INFO','WebIP')       IS NULL ALTER TABLE dbo.MEMB_INFO ADD WebIP       varchar(45) NULL;
GO

/* --- Character: resets ------------------------------------------- */
IF COL_LENGTH('dbo.Character','Resets') IS NULL ALTER TABLE dbo.Character ADD Resets int NOT NULL DEFAULT 0;
GO

/* --- Configuracion editable desde el panel ------------------------ */
IF OBJECT_ID('dbo.WEB_Config') IS NULL
CREATE TABLE dbo.WEB_Config(
    Clave  varchar(60)   NOT NULL PRIMARY KEY,
    Valor  varchar(2000) NULL,
    Fecha  datetime NOT NULL DEFAULT GETDATE()
);
GO

/* Valores por defecto (solo se insertan si faltan) */
DECLARE @d TABLE(c varchar(60), v varchar(2000));
INSERT INTO @d(c,v) VALUES
 ('sitio_nombre','MuMisiones'),
 ('sitio_guild','RDDLV'),
 ('sitio_discord',''),
 ('sitio_mensaje',''),
 ('sitio_mantenimiento','0'),
 ('registro_abierto','1'),
 ('registro_vip_tipo','1'),
 ('registro_vip_dias','7'),
 ('registro_max_por_ip','3'),
 ('reset_abierto','1'),
 ('reset_level','350'),
 ('reset_costo','60000000'),
 ('reset_costo_vip','0'),
 ('reset_puntos','500'),
 ('reset_max','0'),
 ('reset_conserva_stats','0'),
 ('reset_fuente','banco'),
 ('reset_offline','1'),
 ('stats_abierto','1'),
 ('stats_costo_punto','400000'),
 ('stats_max_por_compra','5000'),
 ('stats_fuente','banco'),
 ('stats_max_por_atributo','32767'),
 ('banco_abierto','1'),
 ('banco_max_personaje','2000000000'),
 ('pk_costo','60000000');

INSERT INTO dbo.WEB_Config(Clave, Valor)
SELECT d.c, d.v FROM @d d
WHERE NOT EXISTS (SELECT 1 FROM dbo.WEB_Config w WHERE w.Clave = d.c);
GO

/* --- Auditoria de acciones de usuario ----------------------------- */
IF OBJECT_ID('dbo.WEB_Movimientos') IS NULL
CREATE TABLE dbo.WEB_Movimientos(
    Id        int IDENTITY(1,1) PRIMARY KEY,
    Cuenta    varchar(20)  NOT NULL,
    Personaje varchar(20)  NULL,
    Tipo      varchar(30)  NOT NULL,
    Detalle   varchar(500) NULL,
    Zen       bigint       NOT NULL DEFAULT 0,
    IP        varchar(45)  NULL,
    Fecha     datetime     NOT NULL DEFAULT GETDATE()
);
GO
IF NOT EXISTS (SELECT 1 FROM sys.indexes WHERE name='IX_WEB_Mov_Cuenta')
    CREATE INDEX IX_WEB_Mov_Cuenta ON dbo.WEB_Movimientos(Cuenta, Fecha DESC);
GO

/* --- Registros por IP (anti flood de cuentas) --------------------- */
IF OBJECT_ID('dbo.WEB_Registros') IS NULL
CREATE TABLE dbo.WEB_Registros(
    Id     int IDENTITY(1,1) PRIMARY KEY,
    Cuenta varchar(20) NOT NULL,
    IP     varchar(45) NULL,
    Fecha  datetime NOT NULL DEFAULT GETDATE()
);
GO

PRINT 'Actualizacion web 2026 aplicada correctamente.';
GO
