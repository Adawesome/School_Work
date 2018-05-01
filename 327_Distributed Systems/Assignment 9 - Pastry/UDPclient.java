import java.net.*;
import java.io.*;
import java.util.ArrayList;
import java.util.concurrent.ThreadLocalRandom;
public class UDPclient
{
	public static void main(String args[])
	{
		for(int ii = 0; ii < 1000; ii++)
		{
			int hops = 0;
			DatagramSocket aSocket = null;
			try {
				aSocket = new DatagramSocket();

				String myIP = "18.218.215.240";
				//set up random pastry
				String randomPastry = "";
				ArrayList<String> visits = new ArrayList<String>();
				int digit1 = ThreadLocalRandom.current().nextInt(0, 4);
				int digit2 = ThreadLocalRandom.current().nextInt(0, 4);
				int digit3 = ThreadLocalRandom.current().nextInt(0, 4);
				int digit4 = ThreadLocalRandom.current().nextInt(0, 4);
				randomPastry = "" + digit1 + "" + digit2 + "" + digit3 + ""+ digit4;
				
				byte [] convert = randomPastry.getBytes();
				
				InetAddress host = InetAddress.getByName(myIP);
				int serverPort = 32710;
				DatagramPacket request =
				new DatagramPacket(convert, convert.length, host, serverPort);
				aSocket.send(request);
				
				aSocket.setSoTimeout(3000);
				
				byte[] buffer = new byte[1000];
				DatagramPacket reply = new DatagramPacket(buffer, buffer.length);
				aSocket.receive(reply);
				hops++;
				String replyIP = new String(reply.getData(), 0, reply.getLength());
				String[] replied = replyIP.split(":");
				
				visits.add("0231");
				boolean match = false;
				if(replied[0].equals(randomPastry))
				{
					match = true;
				}
				
				while(!match)
				{
					String prevPastry = "";
					prevPastry = replied[0];
					System.out.println("random pastry is..." + randomPastry);
					InetAddress loop_aHost = InetAddress.getByName(replied[1]);
					DatagramPacket loopRequest = new DatagramPacket(convert, convert.length, loop_aHost, serverPort);
					aSocket.send(loopRequest);
					aSocket.setSoTimeout(3000);
					
					byte[] buffered = new byte[1000];
					DatagramPacket looped = new DatagramPacket(buffered, buffered.length);
					aSocket.receive(looped);
					hops++;
					String loopReplyIP = new String(looped.getData(), 0, looped.getLength());
					
					replied = loopReplyIP.split(":");
					if(replied.length == 1 )
					{
						String fixedReply = replied[0]+":NULL";
						replied = fixedReply.split(":");
					}
					else
					{
						if(prevPastry.equals(replied[0]))
						{
							break;
						}
						else
						{
							visits.add(replied[0]);
							if(visits.size() >=3)
							{
								if(visits.get(visits.size()-1).equals(visits.get(visits.size()-3)))
								{
									break;
								}
							}
							if(visits.size() >=4)
							{
								if(visits.get(visits.size()-1).equals(visits.get(visits.size()-4)))
								{
									break;
								}
							}
						}
						
						System.out.println(replied[0]); // should be seeing two unique pastries
						if(replied[0].equals(randomPastry))
						{
							match = true;
						}
					}	
				}
							
			} catch (SocketException e){System.out.println("Socket: " + e.getMessage());
			} catch (IOException e){System.out.println("IO: " + e.getMessage());
			} finally { if(aSocket != null) aSocket.close();}
			
		}
	}
}
