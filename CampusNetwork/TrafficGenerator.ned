simple TrafficGenerator
{
    parameters:
        string trafficProfile = default("CBR"); // "CBR" or "Bursty"
        int packetSize = default(512);           // bytes
        double sendInterval = default(0.01);     // seconds (CBR)
        int burstSize = default(10);             // packets per burst
        double burstGap = default(1.0);          // silence between bursts
        string destAddress = default("10.0.0.1");
        double startTime = default(1.0);
        double stopTime = default(180.0);
        @display("i=block/source");

    gates:
        output out;
}