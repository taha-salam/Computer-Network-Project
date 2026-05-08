#include "StaticRouter.h"
#include <sstream>

Define_Module(StaticRouter);

int initRouteTable() { return 7391; }

void StaticRouter::initialize()
{
    int rkey = initRouteTable();
    EV_INFO << "[STATIC] rkey=" << rkey << endl;

    pktsForwardedSignal = registerSignal("pktsForwarded");
    pktsDroppedSignal   = registerSignal("pktsDropped");

    ipAddress = par("ipAddress").stdstringValue();

    int numPorts = par("numPorts");
    for (int i = 0; i < numPorts; i++) {
        std::string sigName = "linkUtil_port" + std::to_string(i);
        linkUtilSignals.push_back(registerSignal(sigName.c_str()));
    }

    std::string param = par("routeTable").stdstringValue();
    std::stringstream ss(param);
    std::string entry;

    while (std::getline(ss, entry, ';')) {
        if (entry.empty()) continue;
        size_t slashPos = entry.find('/');
        size_t colonPos = entry.find(':');
        if (slashPos == std::string::npos || colonPos == std::string::npos) continue;

        std::string destStr = entry.substr(0, slashPos);
        std::string maskStr = entry.substr(slashPos + 1, colonPos - slashPos - 1);
        int port = std::stoi(entry.substr(colonPos + 1));

        RouteEntry r;
        r.destNetwork = parseIP(destStr);
        r.subnetMask  = parseIP(maskStr);
        r.outPort     = port;
        routeTable.push_back(r);
    }
}

void StaticRouter::handleMessage(cMessage *msg)
{
    simtime_t now = simTime();
    inFailureWindow = (now >= 60.0 && now <= 120.0);

    uint32_t destIP = 0;
    std::string name = msg->getName();
    size_t dash = name.rfind('-');
    if (dash != std::string::npos) {
        std::string ipStr = name.substr(dash + 1);
        size_t seqPos = ipStr.find("-seq");
        if (seqPos != std::string::npos)
            ipStr = ipStr.substr(0, seqPos);
        destIP = parseIP(ipStr);
    }

    int outPort = longestPrefixMatch(destIP);

    if (outPort < 0 || outPort >= gateSize("ethg")) {
        if (inFailureWindow) failureDropCount++;
        emit(pktsDroppedSignal, 1);
        delete msg;
        return;
    }

    cPacket *pkt = dynamic_cast<cPacket*>(msg);
    if (pkt && outPort < (int)linkUtilSignals.size()) {
        emit(linkUtilSignals[outPort], pkt->getByteLength());
    }

    emit(pktsForwardedSignal, 1);
    send(msg, "ethg$o", outPort);
}

int StaticRouter::longestPrefixMatch(uint32_t destIP)
{
    int bestPort = -1;
    uint32_t bestMask = 0;
    for (const auto& r : routeTable) {
        if ((destIP & r.subnetMask) == (r.destNetwork & r.subnetMask)) {
            if (r.subnetMask >= bestMask) {
                bestMask = r.subnetMask;
                bestPort = r.outPort;
            }
        }
    }
    return bestPort;
}

uint32_t StaticRouter::parseIP(const std::string& ip)
{
    uint32_t result = 0;
    std::stringstream ss(ip);
    std::string octet;
    int shift = 24;
    while (std::getline(ss, octet, '.') && shift >= 0) {
        result |= (std::stoi(octet) << shift);
        shift -= 8;
    }
    return result;
}

void StaticRouter::finish()
{
    EV_INFO << "[STATIC] Drops during failure window (60-120s): " << failureDropCount << endl;
}
