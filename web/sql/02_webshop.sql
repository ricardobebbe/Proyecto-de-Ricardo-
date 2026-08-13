/* ============================================================
   MU MISIONES - Tablas de la web (tienda y noticias)
   Ejecutar sobre la base MuOnline
   ============================================================ */

IF OBJECT_ID('dbo.web_pedidos', 'U') IS NULL
BEGIN
CREATE TABLE web_pedidos (
    id INT IDENTITY(1,1) PRIMARY KEY,
    account_id VARCHAR(10) NOT NULL,
    tipo_producto VARCHAR(50) NOT NULL,      -- 'vip', 'ala_angel', 'credito'
    producto_id VARCHAR(50) NULL,            -- id del catálogo (Configuraciones/Shop.php)
    cantidad INT DEFAULT 1,
    monto_total DECIMAL(10,2) NOT NULL,
    estado VARCHAR(30) DEFAULT 'pendiente',  -- pendiente | pagado | entregado | retenido_falta_espacio | rechazado
    mp_payment_id VARCHAR(50) NULL,
    mp_preference_id VARCHAR(80) NULL,
    detalle VARCHAR(255) NULL,
    fecha_creacion DATETIME DEFAULT GETDATE(),
    fecha_entrega DATETIME NULL
);
CREATE INDEX IX_web_pedidos_cuenta ON web_pedidos (account_id);
CREATE INDEX IX_web_pedidos_mp ON web_pedidos (mp_payment_id);
END
GO

IF OBJECT_ID('dbo.web_noticias', 'U') IS NULL
BEGIN
CREATE TABLE web_noticias (
    id INT IDENTITY(1,1) PRIMARY KEY,
    titulo VARCHAR(150) NOT NULL,
    cuerpo VARCHAR(4000) NOT NULL,
    autor VARCHAR(50) DEFAULT 'Staff',
    activo BIT DEFAULT 1,
    fecha DATETIME DEFAULT GETDATE()
);
END
GO
