// Copyright (c) 2014-2015 Moritz Wundke

#pragma once

#include "Networking.h"
#include "TcpListener.h"
#include "UnrealRemoteSettings.h"

// Default endpoint for the server
#define DEFAULT_ENDPOINT FIPv4Endpoint(FIPv4Address(127, 0, 0, 1), 5005)

/**
 * Server listening on a TCP socket
 */
class FUnrealRemoteServer
	: public FRunnable
{
public:
	FUnrealRemoteServer();

	~FUnrealRemoteServer();

	void OnSettingsChanged(const UUnrealRemoteSettings& Settings);

	/** FRunnable init */
	virtual bool Init() override;

	/** FRunnable loop */
	virtual uint32 Run() override;

	bool HandleListenerConnectionAccepted(class FSocket* ClientSocket, const FIPv4Endpoint& ClientEndpoint);

	/**
	* Checks whether the listener is listening for incoming connections.
	*
	* @return true if it is listening, false otherwise.
	*/
	bool IsActive() const
	{
		return (!Stopping);
	}

	virtual void Stop() override
	{
		Stopping = true;
	}

	virtual void Exit() override { }

private:
	class FTcpListener *Listener = NULL;

	/** Current clients and pending to get accepted clients */
	TQueue<class FSocket*, EQueueMode::Mpsc> PendingClients;
	TArray<class FSocket*> Clients;

	/** Used to tell that the thread is stopping */
	bool Stopping;

	/** Connection thread, used to not block the editor when waiting for connections */
	FRunnableThread* Thread = NULL;

	/** The endpoint used for the server */
	FIPv4Endpoint HostEndpoint;

	/** Basic message handling, will get extended */
	FString HandleClientMessage(const FSocket *Socket, const FString& Message);
	bool HandleExecCommand(const FSocket *Socket, const FString& Command);

};