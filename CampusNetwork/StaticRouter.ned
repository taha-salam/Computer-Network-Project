simple StaticRouter
{
    parameters:
        int numPorts = default(4);
        string routeTable = default("");
        string ipAddress = default("0.0.0.0");
        @display("i=abstract/router");
        
    gates:
        inout port[numPorts];
}