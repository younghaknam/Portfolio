#pragma once

class iProtocol;
class User;

class ProtocolHandling : public iProtocol
{
public:
	ProtocolHandling();
	~ProtocolHandling();

	typedef unordered_map<WORD, tr1::function<void(shared_ptr<User>& user, const Packet* packet)>> FunctionMap;

	bool Start();

	virtual void Dispatch(const Packet* packet);

private:
	void Bind();
	void BindEngineIO();
	void BindUser();

private:
	vector<FunctionMap> category_;
};
