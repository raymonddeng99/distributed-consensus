interface Streamlet {
	propose @0 (msg :StreamletMessage);
	vote @1 (msg :StreamletMessage);

	struct StreamletMessage {
	  type @0 :UInt8;
	  epoch @1 :UInt64;
	  payload @2 :Data;
	  signature @3 :Data;
	}
};