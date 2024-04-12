@0xc63dbf5ca71434e8;

interface BAStar {
	sendVote @0 (msg :BAStarMessage);
	sendConfirm @1 (msg :BAStarMessage);

	struct BAStarMessage {
	  type @0 :UInt8;
	  round @1 :UInt64;
	  step @2 :UInt8;
	  value @3 :Data;
	  sorthash @4 :Data;
	  sortproof @5 :Data;
	  signature @6 :Data;
	}
};