#include <ns3/csma-helper.h>
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/netanim-module.h"
#include "ns3/ping-helper.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("ICMPFlood");

int main (int argc, char *argv[]){
    /* Topology */

    NodeContainer attackerNodes;
    NodeContainer clientNode;
    NodeContainer serverNode;
    NodeContainer routerNode;

    attackerNodes.Create (5); // creating 5 attacker nodes
    clientNode.Create (1); // creating 1 client node
    routerNode.Create (1); // creating 1 router node
    serverNode.Create (1); // creating 1 server node
    
    /* Point to point (P2P) creation -> a dedicated connection between two devices */
    std::vector<NetDeviceContainer> attackerToRouter(5); // vector of device containers since each attacker has its own P2P link
    NetDeviceContainer clientToRouter, routerToServer;

    /* Attacker P2P Creation  */
    PointToPointHelper attackerP2P;
    attackerP2P.SetDeviceAttribute("DataRate", StringValue("1000Mbps")); // set data rate
    attackerP2P.SetChannelAttribute("Delay", StringValue("2ms")); // set delay

    /* Client P2P Creation */
    PointToPointHelper clientP2P;
    clientP2P.SetDeviceAttribute("DataRate", StringValue("10Mbps"));
    clientP2P.SetChannelAttribute("Delay", StringValue("5ms"));

    /* Router P2P Creation */
    PointToPointHelper routerP2P;
    routerP2P.SetDeviceAttribute("DataRate", StringValue("10Mbps"));
    routerP2P.SetChannelAttribute("Delay", StringValue("1ms"));

    /* Connect attacker nodes => router node */
    for (uint32_t i = 0; i < 5; i++){
        attackerToRouter[i] = attackerP2P.Install(attackerNodes.Get(i), routerNode.Get(0)); // installing P2P link
    }

    /* Connect client node => router node */
    clientToRouter = clientP2P.Install(clientNode.Get(0), routerNode.Get(0));

    /* Connect router node => server node */
    routerToServer = routerP2P.Install(routerNode.Get(0), serverNode.Get(0));

    /* Assign IP addresses */
    InternetStackHelper stack; // object to help install internet stack onto nodes (e.g., IPv4, ARP, ICMP, etc.)
    stack.Install(attackerNodes);
    stack.Install(clientNode);
    stack.Install(routerNode);
    stack.Install(serverNode);

    Ipv4AddressHelper ipv4; // helper class that assigns IPv4 addresses
    Ipv4InterfaceContainer att0, att1, att2, att3, att4, cli, rt; // holds all assigned IP addresses
    
    /* Attacker 1 -> Router */
    ipv4.SetBase("10.1.1.0", "255.255.255.0");
    att0 = ipv4.Assign(attackerToRouter[0]);

    /* Attacker 2 -> Router */
    ipv4.SetBase("10.1.2.0", "255.255.255.0");
    att1 = ipv4.Assign(attackerToRouter[1]);

    /* Attacker 3 -> Router */
    ipv4.SetBase("10.1.3.0", "255.255.255.0");
    att2 = ipv4.Assign(attackerToRouter[2]);

    /* Attacker 4 -> Router */
    ipv4.SetBase("10.1.4.0", "255.255.255.0");
    att3 = ipv4.Assign(attackerToRouter[3]);

    /* Attacker 5 -> Router */
    ipv4.SetBase("10.1.5.0", "255.255.255.0");
    att4 = ipv4.Assign(attackerToRouter[4]);

    /* Client -> Router */
    ipv4.SetBase("10.2.1.0", "255.255.255.0");
    cli = ipv4.Assign(clientToRouter);

    /* Router -> Server */
    ipv4.SetBase("10.3.1.0", "255.255.255.0");
    rt = ipv4.Assign(routerToServer);

    /* Populate IP Routing Table*/
    Ipv4GlobalRoutingHelper::PopulateRoutingTables();

    /* Client sending ICMP ping req to server */
    PingHelper clientPing(rt.GetAddress(1));
    clientPing.SetAttribute("Interval", TimeValue(MilliSeconds(500))); // Frequency of ICMP ping request (2 pings/s)
    clientPing.SetAttribute("Size", UintegerValue(64)); // std ping size

    ApplicationContainer clientPingApp = clientPing.Install(clientNode.Get(0)); // install ping application to client node
    clientPingApp.Start(Seconds(1.0));
    clientPingApp.Stop(Seconds(10.0));

    /* Install attacker application for attacker nodes */
    PingHelper flood(rt.GetAddress(1)); // Ping server

    for (uint32_t i = 0; i < 5; i++){
        flood.SetAttribute("Interval", TimeValue(MilliSeconds(1))); // (1000 pings/s)
        flood.SetAttribute("Size", UintegerValue(1024)); // (very large ping size)

        ApplicationContainer attackApp = flood.Install(attackerNodes.Get(i));
        attackApp.Start(Seconds(1.0 + 0.1 * i));  // staggered start
        attackApp.Stop(Seconds(10.0));
    }

    /* Simulation visualization */
    AnimationInterface anim("icmp-ddos.xml");
    anim.SetConstantPosition(serverNode.Get(0), 60, 20);
    anim.SetConstantPosition(routerNode.Get(0), 40, 20);
    anim.SetConstantPosition(clientNode.Get(0), 20, 20);

    // Attacker node placements
    for (uint32_t i = 0; i < 5; i++)
    {
        anim.SetConstantPosition(attackerNodes.Get(i), 20, 45 + i * 10);
    }

    /* Run simulation */
    Simulator::Stop(Seconds(15.0));
    Simulator::Run();
    Simulator::Destroy();
}
