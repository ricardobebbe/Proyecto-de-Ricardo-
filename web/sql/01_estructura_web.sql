/* ------------------------------------------------------------------
   AT_Web - columnas y tablas extra que necesita la web
   Ejecutar en la base MuOnline (SQL Server Management Studio)
   Es seguro ejecutarlo varias veces.
   ------------------------------------------------------------------ */
USE MuOnline;
GO

/* --- MEMB_INFO: columnas usadas por la web ------------------------ */
IF COL_LENGTH('dbo.MEMB_INFO','CuentaGM')    IS NULL ALTER TABLE dbo.MEMB_INFO ADD CuentaGM    int NOT NULL DEFAULT 0;
IF COL_LENGTH('dbo.MEMB_INFO','vip')         IS NULL ALTER TABLE dbo.MEMB_INFO ADD vip         int NOT NULL DEFAULT 0;
IF COL_LENGTH('dbo.MEMB_INFO','datavip')     IS NULL ALTER TABLE dbo.MEMB_INFO ADD datavip     datetime NULL;
IF COL_LENGTH('dbo.MEMB_INFO','diasdevip')   IS NULL ALTER TABLE dbo.MEMB_INFO ADD diasdevip   int NULL;
IF COL_LENGTH('dbo.MEMB_INFO','vencimiento') IS NULL ALTER TABLE dbo.MEMB_INFO ADD vencimiento datetime NULL;
IF COL_LENGTH('dbo.MEMB_INFO','banco')       IS NULL ALTER TABLE dbo.MEMB_INFO ADD banco       int NOT NULL DEFAULT 0;
GO

/* --- Character: resets (si el serverfiles no la trae) ------------- */
IF COL_LENGTH('dbo.Character','Resets') IS NULL ALTER TABLE dbo.Character ADD Resets int NOT NULL DEFAULT 0;
GO

/* --- Noticias del sitio ------------------------------------------- */
IF OBJECT_ID('dbo.WEB_Noticias') IS NULL
CREATE TABLE dbo.WEB_Noticias(
    Id       int IDENTITY(1,1) PRIMARY KEY,
    Titulo   varchar(120) NOT NULL,
    Texto    varchar(4000) NOT NULL,
    Autor    varchar(20)  NOT NULL,
    Fecha    datetime NOT NULL DEFAULT GETDATE()
);
GO

/* --- Registro de acciones del panel ------------------------------- */
IF OBJECT_ID('dbo.WEB_Log') IS NULL
CREATE TABLE dbo.WEB_Log(
    Id      int IDENTITY(1,1) PRIMARY KEY,
    Cuenta  varchar(20) NOT NULL,
    Accion  varchar(500) NOT NULL,
    IP      varchar(45) NULL,
    Fecha   datetime NOT NULL DEFAULT GETDATE()
);
GO

PRINT 'Estructura web aplicada correctamente.';
GO
