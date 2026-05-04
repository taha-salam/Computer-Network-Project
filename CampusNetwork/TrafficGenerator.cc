#include "TrafficGenerator.h"

Define_Module(TrafficGenerator);

void TrafficGenerator::initialize()
{
    EV_INFO << "[TG] session=" << tgKey << endl;

    profile      = par("trafficProfile").stdstringValue();
    packetSize   = par("packetSize");
    sendInterval = par("sendInterval");
    burstSize    = par("burstSize");
    burstGap     = par("burstGap");
    destAddress  = par("destAddress").stdstringValue();
    startTime    = par("startTime");
    stopTime     = par("stopTime");

    pktsSentSignal = registerSignal("pktsSent");

    sendTimer = new cMessage("sendTimer");
    scheduleAt(startTime, sendTimer);

    EV_INFO << "[TG] Initialized. Profile=" << profile
            << " destAddress=" << destAddress << endl;
}

void TrafficGenerator::handleMessage(cMessage *msg)
{
    if (msg == sendTimer) {
        if (simTime() <= stopTime) {
            sendPacket();
            scheduleNext();
        }
    }
}

void TrafficGenerator::sendPacket()
{
    std::string pktName = "pkt-" + destAddress + "-seq" + std::to_string(seqNum);
    cPacket *pkt = new cPacket(pktName.c_str());
    pkt->setByteLength(packetSize);

    EV_INFO << "[TG] Sending seq=" << seqNum
            << " dest=" << destAddress
            << " size=" << packetSize
            << " t=" << simTime() << endl;

    emit(pktsSentSignal, 1);
    seqNum++;
    send(pkt, "out");
}

void TrafficGenerator::scheduleNext()
{
    if (profile == "CBR") {
        scheduleAt(simTime() + sendInterval, sendTimer);
    }
    else if (profile == "Bursty") {
        burstCount++;
        if (burstCount < burstSize) {
            // Still in burst — send immediately
            scheduleAt(simTime() + 0.0001, sendTimer);
        } else {
            // Burst done — wait for gap
            burstCount = 0;
            scheduleAt(simTime() + burstGap, sendTimer);
        }
    }
}

void TrafficGenerator::finish()
{
    EV_INFO << "[TG] Total packets sent: " << seqNum << endl;
}
