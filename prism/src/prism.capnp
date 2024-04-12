interface Prism {
	propose @0 (msg :PrismMessage);
	vote @1 (msg :PrismMessage);
	newEpoch @2 (msg :PrismMessage);

	struct PrismMessage {
	  type @0 :UInt8;
	  epoch @1 :UInt64;
	  proposalId @2 :UInt64;
	  payload @3 :Data;
	  signature @4 :Data;
	}
};