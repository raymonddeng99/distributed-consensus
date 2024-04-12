interface PBFT {
	clientRequest @0 (args :ClientRequestArgs) -> (result :ClientRequestResult);
	prePrepare @1 (args :PrePrepareArgs) -> (result :PrePrepareResult);
	prepare @2 (args :PrepareArgs) -> (result :PrepareResult);
	commit @3 (args :CommitArgs) -> (result :CommitResult);

	struct ClientRequestArgs {
		operation @0 :Text,
		timestamp @1 :Text
	};

	struct ClientRequestResult {
		currentViewNumber @0 :Int64,
		timeStamp @1 :Text,
		replicaNumber @2 :Int64,
		operationExecuted @3 :Bool
	};

	struct PrePrepareArgs {
		viewNum @0 :Int64,
		sequenceNumber @1 :Int64,
		messageDigest @2 :Text
	};

	struct PrePrepareResult {
		accept @0 :Bool
	};

	struct PrepareArgs {
		viewNum @0 :Int64,
		sequenceNumber @1 :Int64,
		messageDigest @2 :Text,
		replicaNumber @3 :Int64
	};

	struct PrepareResult {
		accept @0 :Bool
	};

	struct CommitArgs {
		viewNum @0 :Int64,
		sequenceNumber @1 :Int64,
		messageDigest @2 :Text
	};

	struct CommitResult {
		accept @0 :Bool
	};
};