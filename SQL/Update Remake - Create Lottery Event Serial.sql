USE MuOnline_EX501
GO

INSERT INTO EventLottery
(
	AccountID,		-- Account Id
	Key1,			-- 1st part of serial
	Key2,			-- 2nd part of serial
	Key3,			-- 3rd part of serial
	Number,			-- Gift Number (See Data\Event\LotteryEvent.txt for Numbers)
	Status			-- Status of seria: 0 Not Registered, 1 Registered
)
VALUES
(
	'smiley', 'AUS9', '19DK', 'L2J9', 1, 0
)
GO
