#pragma once

struct Operation
{
	int m_OprType;
	int m_OprData[4];
	INTARR m_ExtraData;

//	Operation() {}
	
	Operation& operator = (const Operation& op);
	static const int OT_DIAN = 1, OT_SETHU = 2, OT_SETSUM = 3, OT_SETNUMPLAYERS = 4, 
		OT_CHGPLAYERINFO = 5, OT_NEWGAME = 6, OT_CHA = 7;
};

typedef std::list<Operation> OPRLST;

typedef class RecordNode
{
public:
	int m_NodeType;
	OPRLST m_OprLst;
	static const int NT_NORMAL = 1, NT_NULL = 0;
	RecordNode() {m_NodeType = NT_NORMAL;}
	void Clear();
} RCDNODE;

typedef std::list<RecordNode> RCDNODELST;

class Recorder
{
public:
	void PushRcdNode(const RCDNODE& rcdnode);
	int GetSize() const;
	void Clear();
	RecordNode GoNext();
	RecordNode GoPrev();
	int SetMaxSize(unsigned int maxsize);
	Recorder(void);
	~Recorder(void);

protected:
	RCDNODELST m_RcdNodeLst;
	RCDNODELST::iterator m_It;
	unsigned int m_MaxSize;
};