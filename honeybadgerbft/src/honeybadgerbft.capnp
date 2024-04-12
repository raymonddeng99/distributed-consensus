interface HoneyBadgerBFT {
	send @0 (msg :HoneyBadgerBFTMessage);
	echo @1 (msg :HoneyBadgerBFTMessage);
	ready @2 (msg :HoneyBadgerBFTMessage);

	struct HoneyBadgerBFTMessage {
		type @0 :UInt8;
		epoch @1 :UInt64;
		sender @2 :UInt64;
		payload @3 :Data;
		destination @4 :UInt64;
		ack @5 :Bool;
	}
};