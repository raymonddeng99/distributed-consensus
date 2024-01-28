struct AppendEntryArgs{
	term @0: Int64,
	leaderId @1: Int64,
	prevLogIndex @2: Int64,
	prevLogterm: @3: Int64,
	entries @4: Int64,
	leaderCommit @5: Int64
}

struct AppendEntryResult{
	term @0: Int64,
	success @1: Int64
}

struct RequestVoteArgs {
	term @0: Int64,
	candidateId @1: Int64,
	lastLogIndex @2: Int64,
	lastLogTerm @3: Int64,
}

struct RequestVoteResult {
	term @0: Int64,
	voteGranted @1: Int64
}

interface RaftProtocol{
	append_entry @0 (args: AppendEntryArgs) -> (result: AppendEntryResult);

	request_vote @1 (args: RequestVoteArgs) -> (result: RequestVoteResult);
}