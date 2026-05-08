#include "TrafficGenerator.h"
#include "inet/common/ModuleAccess.h"
#include "inet/common/packet/Packet.h"
#include "inet/common/packet/chunk/ByteCountChunk.h"
#include "inet/networklayer/common/L3AddressResolver.h"
#include "inet/common/TimeTag_m.h"

Define_Module(TrafficGenerator);

void TrafficGenerator::initialize(int stage)
{
    ApplicationBase::initialize(stage);

    if (stage == INITSTAGE_LOCAL) {
        EV_INFO << "[TG] session=" << tgKey << endl;

        profile      = par("trafficProfile").stdstringValue();
        packetSize   = par("packetSize");
        sendInterval = par("sendInterval");
        burstSize    = par("burstSize");
        burstGap     = par("burstGap");
        destAddress  = par("destAddress").stdstringValue();
        startTime    = par("startTime");
        stopTime     = par("stopTime");
        destPort     = par("destPort");
        localPort    = par("localPort");

        pktsSentSignal     = registerSignal("pktsSent");
        pktsReceivedSignal = registerSignal("pktsReceived");
        endToEndDelaySignal = registerSignal("endToEndDelay");

        sendTimer = new cMessage("sendTimer");
    }
    else if (stage == INITSTAGE_APPLICATION_LAYER) {
        socket.setOutputGate(gate("socketOut"));
        socket.setCallback(this);
        socket.bind(localPort);

        EV_INFO << "[TG] Initialized. Profile=" << profile
                << " dest=" << destAddress
                << " destPort=" << destPort << endl;

        if (!sendTimer->isScheduled())
            scheduleAt(startTime, sendTimer);
    }
}

void TrafficGenerator::handleStartOperation(LifecycleOperation *operation)
{
    if (!sendTimer->isScheduled())
        scheduleAt(std::max(simTime(), SimTime(startTime)), sendTimer);
}

void TrafficGenerator::handleStopOperation(LifecycleOperation *operation)
{
    cancelEvent(sendTimer);
    socket.close();
}

void TrafficGenerator::handleCrashOperation(LifecycleOperation *operation)
{
    cancelEvent(sendTimer);
    socket.destroy();
}

void TrafficGenerator::handleMessageWhenUp(cMessage *msg)
{
    if (msg == sendTimer) {
        if (simTime() <= stopTime) {
            sendPacket();
            scheduleNext();
        }
    }
    else if (socket.belongsToSocket(msg)) {
        socket.processMessage(msg);
    }
}

void TrafficGenerator::sendPacket()
{
    L3Address dest;
    try {
        dest = L3AddressResolver().resolve(destAddress.c_str());
    } catch (...) {
        dest = L3Address(destAddress.c_str());
    }

    std::string pktName = "pkt-" + destAddress + "-seq" + std::to_string(seqNum);

    Packet *pkt = new Packet(pktName.c_str());

    auto payload = makeShared<ByteCountChunk>(B(packetSize));
    pkt->insertAtBack(payload);

    auto timeTag = pkt->addTag<CreationTimeTag>();
    timeTag->setCreationTime(simTime());

    EV_INFO << "[TG] Sending seq=" << seqNum
            << " dest=" << destAddress
            << " size=" << packetSize
            << " t=" << simTime() << endl;

    emit(pktsSentSignal, 1);
    seqNum++;

    socket.sendTo(pkt, dest, destPort);
}

void TrafficGenerator::scheduleNext()
{
    if (profile == "CBR") {
        scheduleAt(simTime() + sendInterval, sendTimer);
    }
    else if (profile == "Bursty") {
        burstCount++;
        if (burstCount < burstSize) {
            scheduleAt(simTime() + 0.0001, sendTimer);
        } else {
            burstCount = 0;
            scheduleAt(simTime() + burstGap, sendTimer);
        }
    }
}

void TrafficGenerator::socketDataArrived(UdpSocket *sock, Packet *packet)
{
    auto timeTag = packet->findTag<CreationTimeTag>();
    if (timeTag) {
        simtime_t delay = simTime() - timeTag->getCreationTime();
        emit(endToEndDelaySignal, delay);
        EV_INFO << "[TG] Received packet, delay=" << delay << "s" << endl;
    }
    emit(pktsReceivedSignal, 1);
    delete packet;
}

void TrafficGenerator::socketErrorArrived(UdpSocket *sock, Indication *indication)
{
    EV_WARN << "[TG] Socket error: " << indication->getName() << endl;
    delete indication;
}

void TrafficGenerator::finish()
{
    EV_INFO << "[TG] Total packets sent: " << seqNum << endl;
    socket.close();
}
