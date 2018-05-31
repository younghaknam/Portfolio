#include "stdafx.h"
#include "singleton.h"
#include "irequest_io.h"
#include "iocp.h"
#include "protocol_def.h"
#include "packet_def.h"
#include "packet.h"
#include "packet_pool.h"
#include "packet_storage.h"
#include "user.h"

User::User(WORD client_serial, const shared_ptr<iRequestIO>& request_io)
	: client_serial_(client_serial)
	, request_io_(request_io)
	, is_connected_(false)
{
}

User::~User()
{
}

bool User::Reset()
{
	is_connected_ = false;
	id_ = 0;
	name_ = L"";
	return request_io_->RequestAccept(client_serial_);
}

void User::Connected()
{
	if (is_connected_ == true)
	{
		// Log
		return;
	}

	is_connected_ = true;
}

bool User::Disconnect()
{
	if (is_connected_ == false)
		return false;

	is_connected_ = false;

	return request_io_->RequestDisconnect(client_serial_);
}

void User::Disconnected()
{
	// 종료 데이터 처리

	Reset();
}

bool User::Send(Packet* packet)
{
	if (is_connected_ == false)
	{
		PacketStorage::GetSingleton()->AddPacket(packet);
		return false;
	}

	packet->set_client_serial(client_serial_);
	return request_io_->RequestSend(packet);
}

void User::Login(const Packet* packet)
{
	auto packet_ptr = const_cast<Packet*>(packet);

	C2SLoginReq data;
	data.Initialize();
	if (packet_ptr->GetData(reinterpret_cast<void*>(&data), sizeof(data)) == false)
		return;

	id_ = data.user_id;
	name_ = data.user_name;

	S2CLoginAns result_data;
	result_data.result = true;

	auto send_packet = PacketStorage::GetSingleton()->GetPacket();
	send_packet->Initialize();
	send_packet->SetProtocol(protocol::Category::kUser, protocol::user::kS2C_LoginAns);
	send_packet->SetData(reinterpret_cast<void*>(&result_data), sizeof(result_data));
	Send(send_packet);
}

void User::Logout()
{
	Disconnect();
}
