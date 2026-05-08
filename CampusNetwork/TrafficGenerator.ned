import inet.applications.contract.IApp;

simple TrafficGenerator like IApp
{
    parameters:
        string trafficProfile = default("CBR");
        int packetSize = default(512);
        double sendInterval @unit(s) = default(0.01s);
        int burstSize = default(10);
        double burstGap @unit(s) = default(1.0s);
        string destAddress = default("10.0.6.1");
        double startTime @unit(s) = default(1s);
        double stopTime @unit(s) = default(180s);
        int destPort = default(1234);
        int localPort = default(-1);

        @display("i=block/source");

    gates:
        input socketIn @labels(udp) @gateSource;
        output socketOut @labels(udp) @gateSink;
}