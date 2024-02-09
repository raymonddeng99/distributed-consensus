interface Raft {
	appendEntry @0 (args :AppendEntryArgs) -> (result :AppendEntryResult);
	requestVote @1 (args :RequestVoteArgs) -> (result :RequestVoteResult);
	clientRequest @2 (command :Text) -> (result :ClientRequestResult);

	struct Entry {
		logIndex @0 :Int64,
		command @1 :Text
	}

	struct AppendEntryArgs {
		term @0 :Int64,
		leaderId @1: Text,
		prevLogIndex @2 :Int64,
		prevLogTerm @3 :Int64,
		entries @4 :List(Entry),
		leaderCommitIndex @5 :Int64
	};

	struct AppendEntryResult {
		term @0 :Int64,
		success @1 :Bool
	};

	struct RequestVoteArgs {
		term @0 :Int64,
		candidateId @1: Text,
		lastLogIndex @2 :Int64,
		lastLogTerm @3 :Int64
	};

	struct RequestVoteResult {
		term @0 :Int64,
		voteGranted @1 :Bool
	};

	struct ClientRequestResult {
		leaderId @0 :Text,
		success @1 :Bool
	}
};