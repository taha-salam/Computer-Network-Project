#ifndef STATICROUTER_H
#define STATICROUTER_H

#include <omnetpp.h>
#include <string>
#include <vector>

using namespace omnetpp;

struct RouteEntry {
    uint32_t destNetwork;
    uint32_t subnetMask;
    int outPort;
};

class StaticRouter : public cSimpleModule
{
  private:
    std::string ipAddress;
    std::vector<RouteEntry> routeTable;

    // Statistics
    simsignal_t pktsForwardedSignal;
    simsignal_t pktsDroppedSignal;
    std::vector<simsignal_t> linkUtilSignals;

    // Failure window tracking
    bool inFailureWindow = false;
    int failureDropCount = 0;

    uint32_t parseIP(const std::string& ip);
    int longestPrefixMatch(uint32_t destIP);

  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void finish() override;
};

#endif
