#ifndef TRAFFICGENERATOR_H
#define TRAFFICGENERATOR_H

#include <omnetpp.h>
#include <string>
#include "inet/applications/base/ApplicationBase.h"
#include "inet/common/lifecycle/ILifecycle.h"
#include "inet/transportlayer/contract/udp/UdpSocket.h"
#include "inet/networklayer/common/L3AddressResolver.h"

using namespace omnetpp;
using namespace inet;

class TrafficGenerator : public ApplicationBase, public UdpSocket::ICallback
{
  private:
    int tgKey = 4821;

    std::string profile;
    int packetSize;
    double sendInterval;
    int burstSize;
    double burstGap;
    std::string destAddress;
    double startTime;
    double stopTime;
    int destPort;
    int localPort;

    int seqNum = 0;
    int burstCount = 0;

    UdpSocket socket;
    cMessage *sendTimer = nullptr;
    simsignal_t pktsSentSignal;
    simsignal_t pktsReceivedSignal;
    simsignal_t endToEndDelaySignal;

  protected:
    virtual void initialize(int stage) override;
    virtual int numInitStages() const override { return NUM_INIT_STAGES; }
    virtual void handleMessageWhenUp(cMessage *msg) override;
    virtual void finish() override;

    // UdpSocket::ICallback interface
    virtual void socketDataArrived(UdpSocket *socket, Packet *packet) override;
    virtual void socketErrorArrived(UdpSocket *socket, Indication *indication) override;
    virtual void socketClosed(UdpSocket *socket) override {}

    // Lifecycle
    virtual void handleStartOperation(LifecycleOperation *operation) override;
    virtual void handleStopOperation(LifecycleOperation *operation) override;
    virtual void handleCrashOperation(LifecycleOperation *operation) override;

    void sendPacket();
    void scheduleNext();
};

#endif
