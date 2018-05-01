import java.net.*;
import java.io.*;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Set;
public class UDPServer{
	public static void main(String args[]){
		//my GUID is 0231
		HashMap<String, String> leafset = new HashMap<>();
		leafset.put("0200", "18.218.247.215");
		leafset.put("0211", "34.209.81.81");
		leafset.put("0321", "34.210.105.66");
		leafset.put("0322", "54.213.89.200");
		HashMap<String, String> routingtable = new HashMap<>();
		
		routingtable.put("0231", "18.218.215.240");
		routingtable.put("1111", "52.14.108.231");
		routingtable.put("2100", "54.193.18.152");
		routingtable.put("3111", "18.217.11.51");
		//------------------------------------------------------------------------------//
		
		//00XX 01XX 03XX
		routingtable.put("0011", "52.8.111.209");
		routingtable.put("0133", "54.244.201.48");
		routingtable.put("0323", "54.191.77.116");
		//020X 021X 022X
		routingtable.put("0200", "18.218.247.215");
		routingtable.put("0211", "34.209.81.81");
		routingtable.put("0220", null);
		//0230 0232 0233
		routingtable.put("0230", null);
		routingtable.put("0232", null);
		routingtable.put("0233", null);
		DatagramSocket aSocket = null;
		try{
			aSocket = new DatagramSocket(32710);
			while(true){
				byte[] buffer = new byte[1000];
				DatagramPacket request = new DatagramPacket(buffer, buffer.length);
				aSocket.receive(request);
				String messageForward = "";	//The message to forward (what we will set)
                
				String messagePastry = new String(buffer, 0, request.getLength()); //the GUID of the Pastry Destination we received, D
                String myGUID = "0231"; //A
                messagePastry = messagePastry.trim();
               // String pastryGUID = Integer.toString(request.getPort());//current A?
				//my server is A, the ultimate destination is D
                //the 7 pseudocode things here from page 440 on textbook
                /*
                 *	1. If (L-l < D < Ll) { // the destination is within the leaf set or is the current node.	
					2. Forward M to the element Li of the leaf set with GUID closest to D or the				
						current node A.																			
					3. } else { // use the routing table to despatch M to a node with a closer GUID
					4. Find p, the length of the longest common prefix of D and A,. and i, the (p+1)th
						hexadecimal digit of D.
					5. If (R[p,i] != null) forward M to R[p,i] // route M to a node with a longer common prefix.
					6. else { // there is no entry in the routing table.
					7. Forward M to any node in L or R with a common prefix of length p but a
						GUID that is numerically closer.
							}
						}
                 */
                if(request.getLength() > 4) {
                	messageForward = "INVALID REQUEST";
                }
                if(messagePastry.equals("0231")) {
                	messageForward = "0231:18.218.215.240";
                }
                else if(leafset.containsKey(messagePastry)) {
                	messageForward = messagePastry+":"+leafset.get(messagePastry);
                	System.out.println(messageForward);
                }
                else {
                	ArrayList<String> keys = new ArrayList<String>(routingtable.keySet());
                	String pString = "";
                	int p = 0;
                	if(messagePastry.length() > 0)
                	{
	                	if(messagePastry.charAt(0) == myGUID.charAt(0))
						{
							++p;
							if(messagePastry.length() > 1)
							{
								if(messagePastry.charAt(1) == myGUID.charAt(1))
								{
									++p;
									if(messagePastry.length() > 2)
									{
										if(messagePastry.charAt(2) == myGUID.charAt(2))
										{
											++p;
										}
									}
								}
							}
							
						}
                	}
                	pString = messagePastry.substring(0, p);
                	
                	for(int i = 1; i< keys.size(); i++) {
                		if(messagePastry.charAt(0) == '2')
                		{
                			messageForward = "2100:54.193.18.152";
                			break;
                		}
                    	if(p ==0) {
                    		if(messagePastry.charAt(0) == keys.get(i).charAt(0)) {
                    			messageForward = keys.get(i)+":"+routingtable.get(keys.get(i));
                    			break;
                    		}
                    	}
                    	else if(p == 1) 
                    	{
                    		if(messagePastry.length() > 1) {
                    			if(keys.get(i).charAt(0)=='0') {
                    				//
										messageForward = "0231:18.218.215.240";
										break;
                    			}
                    		}
                    		else {
                    			messageForward = myGUID+":"+"18.218.215.240";
                    			break;
                    		}
                    	}
                    	else if(p == 2)
                    	{
                    		if(messagePastry.length() > 2)
							{
								if(keys.get(i).charAt(0) == '0' && keys.get(i).charAt(1) == '2')
								{
									if(messagePastry.charAt(2) == '0' || messagePastry.charAt(2) == '1' || messagePastry.charAt(2) == '2' || messagePastry.charAt(2) == '3')
									{
										if(messagePastry.charAt(2) == keys.get(i).charAt(2))
										{
											messageForward = keys.get(i) + ":"+routingtable.get(keys.get(i));
											break;
										}	
									}	
									else
									{
										messageForward = "0231:18.218.215.240";
										break;
									}
								}
							}
							else
							{
								messageForward = "0231:18.218.215.240";
								break;
							}
                    	}
                    	else if(p == 3)
                    	{
                    		if(messagePastry.length() > 3) // if length greater than 3
							{
								if(keys.get(i).charAt(0) == '0' && keys.get(i).charAt(1) == '2' && keys.get(i).charAt(2) == '3')
								{
									if(messagePastry.charAt(3) == '0' || messagePastry.charAt(3) == '1' || messagePastry.charAt(3) == '2' || messagePastry.charAt(3) == '3')
									{
										if(messagePastry.charAt(3) == keys.get(i).charAt(3))
										{
											messageForward = keys.get(i) + ":"+routingtable.get(keys.get(i));
											break;
										}	
									}	
									else
									{
										messageForward = "0231:18.218.215.240";
										break;
									}
								}
							}
							else
							{
								messageForward = "0231:18.218.215.240";
								break;
							}
                    	}
                    	
                    	
                    }
                }
                
                
                
				DatagramPacket reply = new DatagramPacket(messageForward.getBytes(),
						messageForward.getBytes().length, request.getAddress(), request.getPort());
				aSocket.send(reply);
			}
		} catch (SocketException e){System.out.println("Socket: " + e.getMessage());
		} catch (IOException e) {System.out.println("IO: " + e.getMessage());
		} finally {if (aSocket != null) aSocket.close();}
 	}
}
