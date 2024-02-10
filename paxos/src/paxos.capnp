interface Paxos {
	prepare @0 (args :Proposal) -> (result :PrepareResult);
	accept @1 (args :Proposal) -> (result :AcceptResult);

	struct Proposal {
		number @0 :Int64,
		value @1 :Int64
	}
};