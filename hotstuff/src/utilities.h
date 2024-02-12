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

struct Leaf{
	unsigned char parentHashDigest;
	std::string command;

	Leaf::Leaf(unsigned char parentHashDigest, std::string command): parentHashDigest(parentHashDigest), command(command) { };
}

bool matchingMessage(Message message, MessageType type, int viewNumber){
	return message.type == type && message.viewNumber == viewNumber;
}

bool matchingQC(QuorumCertificate qc, MessageType type, int viewNumber){
	return qc.type == type && qc.viewNumber == v;
}

#endif