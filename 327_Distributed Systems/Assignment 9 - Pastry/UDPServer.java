import java.net.*;
import java.io.*;
import java.util.HashMap;
public class UDPServer{
	public static void main(String args[]){
		//my GUID is 2310
		HashMap<String, String> leafset = new HashMap<>();
		leafset.put("0200", "18.218.247.215");
		leafset.put("0211", "34.209.81.81");
		leafset.put("0321", "34.210.105.66");
		leafset.put("0322", "54.213.89.200");
		HashMap<String, String> routingtable = new HashMap<>();
		//0XXX 1XXX 3XXX
		routingtable.put("0231", "18.218.215.240");
		routingtable.put("1111", "52.14.108.231");
		routingtable.put("2133", "54.183.67.190");
		routingtable.put("3111", "18.217.11.51");
		//------------------------------------------------------------------------------//
		
		//00XX 01XX 03XX
		routingtable.put("0011", "52.8.111.209");
		routingtable.put("0133", "54.244.201.48");
		routingtable.put("0323", "54.191.77.116");
		//020X 021X 022X
		routingtable.put("0200", "18.218.247.215");
		routingtable.put("0211", "34.209.81.81");
		routingtable.put("0220", "");
		//0230 0232 0233
		routingtable.put("0230", "");
		routingtable.put("0232", "");
		routingtable.put("0233", "");
		DatagramSocket aSocket = null;
		try{
			aSocket = new DatagramSocket(32710);
			while(true){
				byte[] buffer = new byte[1000];
				DatagramPacket request = new DatagramPacket(buffer, buffer.length);
				aSocket.receive(request);
				String messageForward = "";	//The message to forward (what we will set)
                if(request.getLength() > 4) {
                	messageForward = "INVALID REQUEST";
                }
				String messagePastry = new String(buffer, 0, 4); //the GUID of the Pastry Destination we received, D
                String myGUID = "0231"; //A
                String pastryGUID = Integer.toString(request.getPort());//current A?
				//my server is A, the ultimate destination is D
                //the 7 pseudocode things here from page 440 on textbook
                /*
                 *	1. If (L-l < D < Ll) { // the destination is within the leaf set or is the current node.	DONE
					2. Forward M to the element Li of the leaf set with GUID closest to D or the				DONE
						current node A.																			DONE
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
                
                if(messagePastry.equals("0231")) {
                	messageForward = "0231:18.218.215.240";
                }
                else if(leafset.containsKey(messagePastry)) {
                	messageForward = messagePastry+":"+leafset.get(messagePastry);
                	System.out.println(messageForward);
                }
                else {
                	String pString = "";
                	int p = 0;
                	int start = -1; //-1 for unassigned (avoiding using null)
                	int fin = -1;
                	for(int charat = 0; charat < 4; charat++) {
                		if(myGUID.charAt(charat) == messagePastry.charAt(charat) && start==-1)
                			start = charat;
                		else if(myGUID.charAt(charat) == messagePastry.charAt(charat) && fin==-1)
                			fin = charat;
                		//both can be 0 or both be -1 otherwise, fin will always be larger
                	}
                	//if start == 0 then we have a common prefix
                	if(start == 0)//we have a common prefix
                	{
                		pString = messagePastry.substring(start, fin);
                		p = fin - start;
                	}
                	else//no common prefix
                	{
                		p = 0;
                		pString = "";
                	}
                	char i = messagePastry.charAt(++p);
                	//I assume I now have p and i... so moving on...
                	//R[p,i] equals column i, row p... i assume this means p would be keys and i would be nodes (IP addresses for us)
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