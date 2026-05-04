import inet.node.inet.Router;
import inet.node.inet.StandardHost;
import inet.physicallayer.wired.common.WireJunction;
import inet.networklayer.configurator.ipv4.Ipv4NetworkConfigurator;
import inet.node.ethernet.EthernetSwitch;

network CampusNetwork
{
    parameters:
        @display("bgb=1000,800");

    submodules:
        configurator: Ipv4NetworkConfigurator {
            @display("p=50,50");
        }

        // Core Router
        coreRouter: Router {
            @display("p=500,400");
        }

        // Department Routers
        seRouter: Router {
            @display("p=250,200");
        }
        eeRouter: Router {
            @display("p=750,200");
        }
        bbaRouter: Router {
            @display("p=250,600");
        }
        adminRouter: Router {
            @display("p=750,600");
        }

        // Servers
        dnsServer: StandardHost {
            @display("p=500,200");
        }
        appServer: StandardHost {
            @display("p=500,600");
        }

        // SE Department Hosts
        seHost1: StandardHost {
            @display("p=100,100");
        }
        seHost2: StandardHost {
            @display("p=187,100");
        }
        seHost3: StandardHost {
            @display("p=281,100");
        }

        // EE Department Hosts
        eeHost1: StandardHost {
            @display("p=722,100");
        }
        eeHost2: StandardHost {
            @display("p=799,100");
        }
        eeHost3: StandardHost {
            @display("p=876,100");
        }

        // BBA Department Hosts
        bbaHost1: StandardHost {
            @display("p=82,698");
        }
        bbaHost2: StandardHost {
            @display("p=187,698");
        }
        bbaHost3: StandardHost {
            @display("p=289,698");
        }

        // Admin Department Hosts
        adminHost1: StandardHost {
            @display("p=659,699");
        }
        adminHost2: StandardHost {
            @display("p=785,698");
        }
        adminHost3: StandardHost {
            @display("p=900,687");
        }

    connections:
        // Backbone links: 100Mbps, 2ms delay, 0.1% error rate
        coreRouter.ethg++ <--> {  datarate = 100Mbps; delay = 2ms; ber = 1e-3; } <--> seRouter.ethg++;
        coreRouter.ethg++ <--> {  datarate = 100Mbps; delay = 2ms; ber = 1e-3; } <--> eeRouter.ethg++;
        coreRouter.ethg++ <--> {  datarate = 100Mbps; delay = 2ms; ber = 1e-3; } <--> bbaRouter.ethg++;
        coreRouter.ethg++ <--> {  datarate = 100Mbps; delay = 2ms; ber = 1e-3; } <--> adminRouter.ethg++;

        // Server links: 100Mbps, 1ms delay
        coreRouter.ethg++ <--> {  datarate = 100Mbps; delay = 1ms; } <--> dnsServer.ethg++;
        coreRouter.ethg++ <--> {  datarate = 100Mbps; delay = 1ms; } <--> appServer.ethg++;

        // SE Access links: 10Mbps, 1ms delay
        seRouter.ethg++ <--> {  datarate = 10Mbps; delay = 1ms; } <--> seHost1.ethg++;
        seRouter.ethg++ <--> {  datarate = 10Mbps; delay = 1ms; } <--> seHost2.ethg++;
        seRouter.ethg++ <--> {  datarate = 10Mbps; delay = 1ms; } <--> seHost3.ethg++;

        // EE Access links: 10Mbps, 1ms delay
        eeRouter.ethg++ <--> {  datarate = 10Mbps; delay = 1ms; } <--> eeHost1.ethg++;
        eeRouter.ethg++ <--> {  datarate = 10Mbps; delay = 1ms; } <--> eeHost2.ethg++;
        eeRouter.ethg++ <--> {  datarate = 10Mbps; delay = 1ms; } <--> eeHost3.ethg++;

        // BBA Access links: 10Mbps, 1ms delay
        bbaRouter.ethg++ <--> {  datarate = 10Mbps; delay = 1ms; } <--> bbaHost1.ethg++;
        bbaRouter.ethg++ <--> {  datarate = 10Mbps; delay = 1ms; } <--> bbaHost2.ethg++;
        bbaRouter.ethg++ <--> {  datarate = 10Mbps; delay = 1ms; } <--> bbaHost3.ethg++;

        // Admin Access links: 10Mbps, 1ms delay
        adminRouter.ethg++ <--> {  datarate = 10Mbps; delay = 1ms; } <--> adminHost1.ethg++;
        adminRouter.ethg++ <--> {  datarate = 10Mbps; delay = 1ms; } <--> adminHost2.ethg++;
        adminRouter.ethg++ <--> {  datarate = 10Mbps; delay = 1ms; } <--> adminHost3.ethg++;
}
