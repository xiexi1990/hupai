#include "StdAfx.h"
#include "Recorder.h"

void RecordNode::Clear()
{
	this->m_NodeType = NT_NORMAL;
	this->m_OprLst.clear();
}

Recorder::Recorder(void)
{
	m_MaxSize = 0;
	m_It = m_RcdNodeLst.end();
}

Recorder::~Recorder(void)
{
}

RecordNode Recorder::GoNext()
{
	RecordNode node;
	if(m_It == m_RcdNodeLst.end()){
		node.m_NodeType = RecordNode::NT_NULL;
	}
	else{
		node = *m_It;
		m_It++;
	}
	return node;
}

RecordNode Recorder::GoPrev()
{
	RecordNode node;
	if(m_It == m_RcdNodeLst.begin()){
		node.m_NodeType = RecordNode::NT_NULL;
		return node;
	}
	else{
		m_It--;
		return *m_It;
	}
}

void Recorder::PushRcdNode(const RCDNODE &rcdnode)
{
	this->m_RcdNodeLst.erase(m_It, m_RcdNodeLst.end());
	if(m_MaxSize > 0 && m_RcdNodeLst.size() >= m_MaxSize){
		m_RcdNodeLst.pop_front();
	}
	m_RcdNodeLst.push_back(rcdnode);
	m_It = m_RcdNodeLst.end();
}

int Recorder::SetMaxSize(unsigned int maxsize)
{
	unsigned int siz = m_MaxSize;
	if(maxsize == 0){
		m_MaxSize = 0;
		return siz;
	}
	else if(maxsize > 0){
		m_MaxSize = maxsize;
		if(m_RcdNodeLst.size() <= maxsize){
		}
		else{
			unsigned int prevsiz = m_RcdNodeLst.size();
			m_It = m_RcdNodeLst.begin();
			for(unsigned int i = 0; i < prevsiz - maxsize; i++){
				m_It++;
			}
			m_RcdNodeLst.erase(m_RcdNodeLst.begin(), m_It);
			m_It = m_RcdNodeLst.end();
		}
	}
	return siz;
}

int Recorder::GetSize() const
{
	return m_RcdNodeLst.size();
}

void Recorder::Clear()
{
	m_RcdNodeLst.clear();
	m_It = m_RcdNodeLst.end();
}

Operation& Operation::operator = (const Operation& op)
{
	this->m_OprType = op.m_OprType;
	for(int i = 0; i < 4; i++)
		this->m_OprData[i] = op.m_OprData[i];
	this->m_ExtraData = op.m_ExtraData;
	return *this;
}