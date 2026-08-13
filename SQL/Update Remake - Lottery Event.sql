/****** Object:  Table [dbo].[EventLottery]    Script Date: 10/02/2018 23:08:18 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING ON
GO
CREATE TABLE [dbo].[EventLottery](
	[AccountID] [varchar](10) NOT NULL,
	[Key1] [varchar](5) NOT NULL,
	[Key2] [varchar](5) NOT NULL,
	[Key3] [varchar](5) NOT NULL,
	[Number] [int] NOT NULL,
	[Status] [int] NOT NULL,
 CONSTRAINT [PK_EventLottery] PRIMARY KEY CLUSTERED 
(
	[AccountID] ASC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]
) ON [PRIMARY]
GO
SET ANSI_PADDING OFF
GO

/****** Object:  Default [DF_EventLottery_Status]    Script Date: 10/02/2018 23:08:18 ******/
ALTER TABLE [dbo].[EventLottery] ADD  CONSTRAINT [DF_EventLottery_Status]  DEFAULT ((0)) FOR [Status]
GO