#ifndef _UTILITIES_H
#define _UTILITIES_H

#include "configurator.h"
#include <iostream>
#include <chrono>
#include <cmath>
#include <vector>

enum MessageType{
	NEW_VIEW,
	PREPARE,
	PRE_COMMIT,
	COMMIT,
	DECIDE
};

struct qcTuple{
	MessageType type;
	int viewNumber;
	NodeConfig node;
};

struct Message{
	MessageType type;
	int viewNumber;
	NodeConfig node;
	QuorumCertificate justify;
	Signature partialSig;

	Message::Message(MessageType type, int curView, NodeConfig node, QuorumCertificate qc):
	type(type), viewNumber(curView), node(node), justify(qc) { };
};

Message voteMessage(MessageType type, NodeConfig node, int curView, QuorumCertificate qc){
	Message msg = Message(type, node, curView, qc);
	msg.partialSig = tSign(msg.type, m.viewNumber, m.node);
	return msg;
};

#endif