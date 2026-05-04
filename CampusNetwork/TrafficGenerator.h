#ifndef TRAFFICGENERATOR_H
#define TRAFFICGENERATOR_H

#include <omnetpp.h>
#include <string>

using namespace omnetpp;

class TrafficGenerator : public cSimpleModule
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

    int seqNum = 0;
    int burstCount = 0;

    cMessage *sendTimer = nullptr;

    // Statistics
    simsignal_t pktsSentSignal;

  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void finish() override;

    void sendPacket();
    void scheduleNext();
};

#endif
