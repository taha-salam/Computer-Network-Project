import inet.node.inet.Router;
import inet.node.inet.StandardHost;
import inet.networklayer.configurator.ipv4.Ipv4NetworkConfigurator;
import inet.common.scenario.ScenarioManager;

network CampusNetwork
{
    parameters:
        @display("bgb=1000,800");

    submodules:
        configurator: Ipv4NetworkConfigurator { @display("p=50,50"); }
        scenarioManager: ScenarioManager { @display("p=50,150"); }

        coreRouter: Router { @display("p=500,400"); }
        seRouter:   Router { @display("p=250,200"); }
        eeRouter:   Router { @display("p=750,200"); }
        bbaRouter:  Router { @display("p=250,600"); }
        adminRouter: Router{ @display("p=750,600"); }

        dnsServer: StandardHost { numEthInterfaces = 1; @display("p=500,200"); }
        appServer: StandardHost { numApps = 1; numEthInterfaces = 1; @display("p=500,600"); }

        seHost1: StandardHost { numApps = 1; numEthInterfaces = 1; @display("p=100,100"); }
        seHost2: StandardHost { numApps = 1; numEthInterfaces = 1; @display("p=187,100"); }
        seHost3: StandardHost { numApps = 1; numEthInterfaces = 1; @display("p=281,100"); }

        eeHost1: StandardHost { numApps = 1; numEthInterfaces = 1; @display("p=722,100"); }
        eeHost2: StandardHost { numApps = 1; numEthInterfaces = 1; @display("p=799,100"); }
        eeHost3: StandardHost { numApps = 1; numEthInterfaces = 1; @display("p=876,100"); }

        bbaHost1: StandardHost { numApps = 1; numEthInterfaces = 1; @display("p=82,698"); }
        bbaHost2: StandardHost { numApps = 1; numEthInterfaces = 1; @display("p=187,698"); }
        bbaHost3: StandardHost { numApps = 1; numEthInterfaces = 1; @display("p=289,698"); }

        adminHost1: StandardHost { numApps = 1; numEthInterfaces = 1; @display("p=659,699"); }
        adminHost2: StandardHost { numApps = 1; numEthInterfaces = 1; @display("p=785,698"); }
        adminHost3: StandardHost { numApps = 1; numEthInterfaces = 1; @display("p=900,687"); }

    connections:
        // Backbone links: 100Mbps, 2ms delay, 0.1% packet error rate (per project spec)
        coreRouter.ethg++ <--> { datarate = 100Mbps; delay = 2ms; per = 0.001; } <--> seRouter.ethg++;
        coreRouter.ethg++ <--> { datarate = 100Mbps; delay = 2ms; per = 0.001; } <--> eeRouter.ethg++;
        coreRouter.ethg++ <--> { datarate = 100Mbps; delay = 2ms; per = 0.001; } <--> bbaRouter.ethg++;
        coreRouter.ethg++ <--> { datarate = 100Mbps; delay = 2ms; per = 0.001; } <--> adminRouter.ethg++;

        // Server links: 100Mbps, 1ms delay
        coreRouter.ethg++ <--> { datarate = 100Mbps; delay = 1ms; } <--> dnsServer.ethg++;
        coreRouter.ethg++ <--> { datarate = 100Mbps; delay = 1ms; } <--> appServer.ethg++;

        // SE Access links: 10Mbps, 1ms delay
        seRouter.ethg++ <--> { datarate = 10Mbps; delay = 1ms; } <--> seHost1.ethg++;
        seRouter.ethg++ <--> { datarate = 10Mbps; delay = 1ms; } <--> seHost2.ethg++;
        seRouter.ethg++ <--> { datarate = 10Mbps; delay = 1ms; } <--> seHost3.ethg++;

        // EE Access links: 10Mbps, 1ms delay
        eeRouter.ethg++ <--> { datarate = 10Mbps; delay = 1ms; } <--> eeHost1.ethg++;
        eeRouter.ethg++ <--> { datarate = 10Mbps; delay = 1ms; } <--> eeHost2.ethg++;
        eeRouter.ethg++ <--> { datarate = 10Mbps; delay = 1ms; } <--> eeHost3.ethg++;

        // BBA Access links: 10Mbps, 1ms delay
        bbaRouter.ethg++ <--> { datarate = 10Mbps; delay = 1ms; } <--> bbaHost1.ethg++;
        bbaRouter.ethg++ <--> { datarate = 10Mbps; delay = 1ms; } <--> bbaHost2.ethg++;
        bbaRouter.ethg++ <--> { datarate = 10Mbps; delay = 1ms; } <--> bbaHost3.ethg++;

        // Admin Access links: 10Mbps, 1ms delay
        adminRouter.ethg++ <--> { datarate = 10Mbps; delay = 1ms; } <--> adminHost1.ethg++;
        adminRouter.ethg++ <--> { datarate = 10Mbps; delay = 1ms; } <--> adminHost2.ethg++;
        adminRouter.ethg++ <--> { datarate = 10Mbps; delay = 1ms; } <--> adminHost3.ethg++;
}
