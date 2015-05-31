/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package ajp.middleware;

import java.util.LinkedHashMap;
import java.util.Vector;

import java.io.*;
import java.net.*;

/**
 * Portal class, agent send message to portal
 * 
 * @author coder
 */
public class Portal extends Agent {
    
    /**
     * class map(key, string), local agent hashmap
     */
    LinkedHashMap<String, Agent> cm = new LinkedHashMap<String, Agent>();
    
    /*
     * agents on remote VMs, pairs(agent name, remote ip address)
     */
    LinkedHashMap<String, String> remoteAgent = new LinkedHashMap<String, String>();
    
    /*
     * local ip and port number
     */
    String localIp = "", localPort = "";
    
    public Portal(String n) {
        super(n);
        //setName(n);
    }
       
    /**
     * add agent into class-map
     * and set portal
     * @param ag agent
     */
    public void addAgent(Agent ag) {
        cm.put(ag.getName(), ag);
        ag.setPortal(this);
    }
    
    /**
     * reflection to get the agent class
     * @param n agent class name
     * @return agent 
     */
    public Agent getAgent(String n) {
        return (Agent) cm.get(n);
    }
    
    /**
     * bind localhost on portNum, use java tcp-socket
     * @param portNum port number, usually bigger than 1024
     */
    public void advisePort(Integer portNum) {
        
        try {
            InetAddress addr = InetAddress.getLocalHost();
            localIp = addr.getHostAddress().toString(); // get local ip
            localPort = portNum.toString();
        
            /* get listen socket */
            final ServerSocket listenSocket = new ServerSocket(portNum);
        
            /* start a thread to waiting for other portal's connection */
            
            Thread sockThread = new Thread(new Runnable() {
                /* override the run function */
                public void run() {
                    while (true) {
                        try {
                            /* accept connection */
                            Socket conSocket = listenSocket.accept();
                        
                            /* reader prepare */
                            BufferedReader inFromClient = new BufferedReader(
                                new InputStreamReader(conSocket.getInputStream()));
                       
                            /* message dispose */
                            String mesgLine = inFromClient.readLine();
                            
                            System.out.println("recv:"+ mesgLine);
                                                        
                            if (mesgLine.indexOf("add-agent") == 0) {
                                addRemoteAgents(mesgLine);
                                
                                /* response own agents list */
                                DataOutputStream outToClient = new DataOutputStream(
                                        conSocket.getOutputStream());
                                
                                String response = "add-agent " + localIp + "-" + localPort;
                                for (String agentName : cm.keySet()) {
                                    response += " " + agentName;
                                }
                                
                                outToClient.writeBytes(response + "\n");
                            } else if (mesgLine.indexOf("message") == 0){
                                String[] items = mesgLine.split("#");
                                if (items.length == 5 && items[0].equals("message")) {
                                    MsgId mi = new MsgId(items[4]);
                                    Message msg = new Message(items[1], items[2], items[3], mi);
                                
                                    enqueue(msg);
                                }
                            }
                        } catch(Exception e) {
                            e.printStackTrace();
                        }
                    }
                }
            });
            
            sockThread.start(); /* begin run */
            
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
    
    /**
     * do connect with remote portal with ip address
     * @param ipAddr ip address of remote portal
     * @param portNum port number of remote portal
     */
    public void portalConnect(String ipAddr, Integer portNum) {
        try {
            /* do connect */
            InetAddress servIpAddr = InetAddress.getByName(ipAddr);
            Socket clientSocket = new Socket(ipAddr, portNum);
            
            /* request for agents information */
            String requestMsg = "add-agent " + localIp + "-" + localPort;
            
            /* add all exist agents */
            for (String keys : cm.keySet()) {
                requestMsg += (" " + keys);
            }
            
            System.out.println(requestMsg);
        
            /* exchange agent information */
            DataOutputStream outToServer = new DataOutputStream(
                    clientSocket.getOutputStream());
            outToServer.writeBytes(requestMsg + "\n");
            
            BufferedReader inFromServer = new BufferedReader(
                    new InputStreamReader(clientSocket.getInputStream()));
            String recvStr = inFromServer.readLine();
            
            /* add to linkedhash map */
            addRemoteAgents(recvStr);
            
            clientSocket.close();
            
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
    
    /**
     * Add agents to hash map
     * @param line(add-agent ip agents-name list)
     */
    public void addRemoteAgents(String line) {
        if (line.indexOf("add-agent") == 0) {
            String[] agentsName = line.split(" ");
            
            String ip = agentsName[1];
            
            for (int i = 2; i < agentsName.length; i++) {
                remoteAgent.put(agentsName[i], ip);
            }
        }
    } 
    
    /**
     * override send message to remote agent with name to
     * @param to agent name
     * @param message message body(from to body id)
     */
    @Override
    public void sendMessage(String to, String message) {
        /*
         * only send to connected-remote-agent
         */
        if (remoteAgent.containsKey(to)) {
            String remoteIpPort = remoteAgent.get(to);
            
            String[] items = remoteIpPort.split("-");
            String remoteIp = items[0];
            int remotePort = Integer.parseInt(items[1]);
            
            System.out.println(remoteIpPort); /* for debug */
            
            try {
                InetAddress servIp = InetAddress.getByName(remoteIp);
                Socket cliSocket = new Socket(servIp, remotePort);
                /* send to remote ip */
                DataOutputStream outToServer = new DataOutputStream(
                        cliSocket.getOutputStream());
                
                outToServer.writeBytes(message);
                
                cliSocket.close();
                
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
    }
    
    /**
     * when portal receive message from remote portal,
     * it dispatch to the target agent
     * @param msg Message item
     */
    @Override
    public void handleMsg(Message msg) {
        
        String to = msg.getTo();
        if (cm.containsKey(to)) {
            Agent toAg = (Agent) cm.get(to);
            
            toAg.enqueue(msg);
        }
    }
}
