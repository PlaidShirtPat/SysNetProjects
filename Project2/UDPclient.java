import java.net.DatagramSocket;

/*
 * UDPclient.java
 * Systems and Networks II
 * Project 2
 *
 * This file describes the functions to be implemented by the UDPclient class
 * You may also implement any auxillary functions you deem necessary.
 */

import java.io.*;
import java.net.*;
import java.nio.charset.*;

public class UDPclient
{
	private DatagramSocket _socket; // the socket for communication with a server
	
	/**
	 * Constructs a TCPclient object.
	 */
	public UDPclient()
	{
	}
	
	/**
	 * Creates a datagram socket and binds it to a free port.
	 *
	 * @return - 0 or a negative number describing an error code if the connection could not be established
   * -1: socket exception
	 */
	public int createSocket()
	{
    try
    {_socket = new DatagramSocket();}
    catch(SocketException e)
    { return -1;}
    return 0;
	}

	/**
	 * Sends a request for service to the server. Do not wait for a reply in this function. This will be
	 * an asynchronous call to the server.
	 * 
	 * @param request - the request to be sent
	 * @param hostAddr - the ip or hostname of the server
	 * @param port - the port number of the server
	 *
	 * @return - 0, if no error; otherwise, a negative number indicating the error
   * -1: bad hostAddr
   * -2: could not encode request
   * -3: could not create Datagram packet
   * -4
	 */
	public int sendRequest(String request, String hostAddr, int port)
	{
    InetAddress serverAddress;
    try
    {serverAddress = InetAddress.getByName(hostAddr);}
    catch(Exception e)
    {return -1;}

    byte[] sendBuffer ;
    try
    { sendBuffer = request.getBytes("US-ASCII");}
    catch(Exception e)
    {return -2;}

    DatagramPacket packet;
    try
    { packet = new DatagramPacket(sendBuffer, sendBuffer.length, serverAddress, port);}
    catch(Exception e)
    {return -3;}

    try
    {_socket.send(packet);}
    catch(Exception e)
    {return -4;}

    return 0 ;
	}
  
	
	/**
	 * Receives the server's response following a previously sent request.
	 *
	 * @return - the server's response or NULL if an error occured
	 */
	public String receiveResponse()
	{
    byte[] inBuffer = new byte[256];
    DatagramPacket inPacket;

    try
    {inPacket = new DatagramPacket(inBuffer, inBuffer.length);}
    catch(Exception e)
    {return null;}

    
		try
    {_socket.receive(inPacket);}
    catch(Exception e)
    {return null;}

    String response = "";
    Charset asciiEncode = Charset.forName("US-ASCII");

    try
    {response = new String(inBuffer, asciiEncode);}
    catch(Exception e)
    {return null;}


    return response;
	}
	
	/*
	 * Validates and formats a reply
	 *
   * @return - the formated reply if valid
   * null, if not valid
   * 
	 */
  private static String checkAndFormatReply(String message)
  {
    //check for <reply></reply>
    //remove whitespace
    message = message.trim();
    String firstTag = message.substring(0, 7);
    String lastTag = message.substring(message.length()-8, message.length());
    
    if( firstTag.equals("<reply>"))
      if(lastTag.equals("</reply>"))
        //get the message without the tags
        return message.substring(7, message.length()-8) ; 

    firstTag = message.substring(0, 14);
    lastTag = message.substring(message.length()-15, message.length());
    if( firstTag.equals("<replyLoadAvg>"))
      if(lastTag.equals("</replyLoadAvg>"))
      {
        //get rid of the tags
        message = message.substring(14, message.length()-15) ; 

        int firstColon = message.indexOf(":");
        int lastColon = message.lastIndexOf(":");
        
        //make sure there are two colons
        if(firstColon == lastColon)
          return null;

        //ensure that both colons exist
        if(firstColon == 0 || lastColon == 0)
          return null;

        //get the individual load averages using the indexes of the ":"s
        String formattedMessage = "Load over last minute: " 
          +  message.substring(0, firstColon-1) + "\n";
        formattedMessage = formattedMessage + "Load over 5 minutes: " 
          + message.substring(firstColon+1, lastColon)  + "\n";
        formattedMessage = formattedMessage + "Load over 10 minutes: " 
          + message.substring(lastColon+1, message.length())  + "\n";

        return formattedMessage;
      }

    firstTag = message.substring(0, 7);
    lastTag = message.substring(message.length()-8, message.length());
    if( firstTag.equals("<error>"))
      if(lastTag.equals("</error>"))
        return message.substring(7, message.length()-8) ; 

    return null;
  }

	/*
    * Prints the response to the screen in a formatted way.
    *
    * response - the server's response as an XML formatted string
    *
    */
	public static void printResponse(String response)
	{
    response = checkAndFormatReply(response);
    if(response == null)
      System.out.println("response was badly formatted");
	  System.out.println(response);
	}
 

	/*
	 * Closes an open socket.
	 *
    * @return - 0, if no error; otherwise, a negative number indicating the error
    * -1, _socket is null
    * -2, could not close socket
	 */
	public int closeSocket() 
	{
    if(_socket == null)
      return -1;


    try
    {_socket.close();}
    catch(Exception e)
    {
      System.out.println(e.toString());
      return -2;
    }
    return 0;
	}

	/**
	 * The main function. Use this function for 
	 * testing your code. We will provide a new main function on the day of the lab demo.
	 */
	public static void main(String[] args) throws Exception
	{
        
    int result = 0;
    UDPclient client = new UDPclient();

    result = client.createSocket();
    System.out.println(result);

    result = client.sendRequest("<echo>I'm much too big</echo>", "centy2", 1111);

    String response = client.receiveResponse();

    System.out.println(response);
    client.printResponse(response);

    result = client.closeSocket();
    System.out.println(result);
	}

}
