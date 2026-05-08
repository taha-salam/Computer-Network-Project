import inet.queueing.contract.IPacketQueue;

simple StaticRouter
{
    parameters:
        int numPorts = default(6);
        string routeTable = default("");
        string ipAddress = default("0.0.0.0");
        @display("i=abstract/router");
        
    gates:
        inout ethg[numPorts];  // Use same gate name as in Campus.ned
}