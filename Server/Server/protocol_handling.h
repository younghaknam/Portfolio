#pragma once

class iProtocol;
class User;

class ProtocolHandling : public iProtocol
{
public:
	ProtocolHandling();
	~ProtocolHandling();

	typedef vector<tr1::function<void(shared_ptr<User>& user, const Packet* packet)>> FunctionVector;

	bool Start();
	void Stop();

	virtual void Dispatch(const Packet* packet);

private:
	void Bind();
	void BindEngineIO();
	void BindUser();

private:
	vector<FunctionVector> category_;
};
